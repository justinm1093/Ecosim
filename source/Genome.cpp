
#include "pch.h"
#include "Genome.h"

using namespace Ecosim;
using namespace std;
using namespace glm;

uint32_t Genome::sID = 0;
uint32_t Genome::sHighestInnovParsed = static_cast<uint32_t>(0 - 1);
uint32_t Genome::sHighestIDParsed = 0;

vector<Genome::MutationRateData> Genome::sMutationFuncs = 
{
	{ 1.0f, &Genome::mutateWeights },
	{ 0.075f, &Genome::mutateAddConnection },
	{ 0.075f, &Genome::mutateAddNeuron },
	{ 0.02f, &Genome::mutateDisable },
	{ 0.02f, &Genome::mutateEnable }
};

Genome::Genome(uint32_t id, uint32_t numInputs, bool usesNEAT, bool isPrey) :
	mID(id),
	mNumInputs(numInputs),
	mNextNeuronID(NETWORK_MAX_IN),
	mUsesNEAT(usesNEAT),
	mIsPrey(isPrey)
{
}

//-------------------------------------------------------------

Genome::Genome(const Genome& other) :
	Genome(nextID(), other.mNumInputs, other.mUsesNEAT, other.mIsPrey)
{
	mNextNeuronID = other.mNextNeuronID;
	mSizeGene = other.mSizeGene;

	// copy all genes from other
	for(Gene* gene : other.mGenes)
	{
		mGenes.push_back(new Gene(*gene));
	}
}

//-------------------------------------------------------------

Genome::~Genome()
{
	clear();
}

//-------------------------------------------------------------

Genome* Genome::crossover(const Genome& other, bool isFitnessEqual) const
{
	Genome* newGenome = new Genome(nextID(), mNumInputs, mUsesNEAT, mIsPrey);

	uint32_t myIndex = 0;
	uint32_t otherIndex = 0;
	uint32_t myLast = static_cast<uint32_t>(mGenes.size());
	uint32_t otherLast = static_cast<uint32_t>(other.mGenes.size());

	// while within bounds for both gene lists
	while(myIndex < myLast && otherIndex < otherLast)
	{
		Gene* myGene = mGenes[myIndex];
		Gene* otherGene = other.mGenes[otherIndex];
		uint32_t myInnov = myGene->getInnovation();
		uint32_t otherInnov = otherGene->getInnovation();

		// matching innovation -- inherits gene from random parent
		if(myInnov == otherInnov)
		{
			newGenome->mGenes.push_back(new Gene(Random::randomRange(0.0f, 100.0f) >= 50.0f ? *myGene : *otherGene));
			++myIndex; ++otherIndex;
		}
		
		// disjoint -- our innovation is earlier than other's -- child inherits gene from us
		else if(myInnov < otherInnov)
		{
			newGenome->mGenes.push_back(new Gene(*myGene));
			++myIndex;
		}

		// disjoint -- other's innovation is earlier than ours -- child inherits gene from other (if fitness equal)
		else
		{
			if(isFitnessEqual) 
			{
				newGenome->mGenes.push_back(new Gene(*otherGene));
			}
			++otherIndex;
		}
	}

	// has excess on our list
	for(; myIndex < myLast; ++myIndex)
	{
		newGenome->mGenes.push_back(new Gene(*mGenes[myIndex]));
	}

	// has excess on other list (only care if fitness is equal)
	if(isFitnessEqual)
	{
		for(; otherIndex < otherLast; ++otherIndex)
		{
			newGenome->mGenes.push_back(new Gene(*other.mGenes[otherIndex]));
		}
	}

	// inherit physical traits from random parent
	newGenome->mSizeGene = Random::randomRange(0.0f, 100.0f) >= 50.0f ? mSizeGene : other.mSizeGene;

	// set next neuron identifier
	newGenome->mNextNeuronID = std::max(mNextNeuronID, other.mNextNeuronID);

	return newGenome;
}

//-------------------------------------------------------------

void Genome::mutate(float fitness)
{
	if(mUsesNEAT)
	{
		// try each NEAT mutation type
		for(MutationRateData& mutation : sMutationFuncs)
		{
			if(Random::randomRange(0.0f, 1.0f) < mutation.probability)
			{
				mutation.func(*this, fitness);
			}
		}
	}
	else
	{
		// no NEAT, just mutate connection weights
		mutateWeights(fitness);
	}

	// mutate physical traits
	float geneMutationChance = SimMath::mutationChanceFromFitness(fitness);
	if(Random::randomRange(0.0f, 1.0f) < geneMutationChance)
	{
		mSizeGene += Random::randomRange(-geneMutationChance * 2, geneMutationChance * 2);
	}
}

//-------------------------------------------------------------

void Genome::mutateWeights(float fitness)
{
	// perturb connection weight on each gene
	float geneMutationChance = SimMath::mutationChanceFromFitness(fitness);
	for(Gene* gene : mGenes)
	{
		if(Random::randomRange(0.0f, 1.0f) < geneMutationChance)
		{
			// lower fitness = higher mutation chance and larger range of possible value changes
			float geneWeight = gene->getWeight();
			gene->setWeight(geneWeight + Random::randomRange(-geneMutationChance * 2, geneMutationChance * 2));
		}
	}
}

//-------------------------------------------------------------

void Genome::mutateAddConnection(float fitness)
{
	UNREFERENCED_PARAMETER(fitness);

	// get random source and target id's
	uint32_t sourceID = randomNeuronID(true);
	uint32_t targetID = randomNeuronID(false);

	// add new gene if we don't already have one linking the two neurons
	if(!hasGene(sourceID, targetID))
	{
		addGene(Gene::nextInnovation(), sourceID, targetID, Random::randomRange(-1.0f, 1.0f), false);
	}
}

//-------------------------------------------------------------

void Genome::mutateAddNeuron(float fitness)
{
	UNREFERENCED_PARAMETER(fitness);
	if(!mGenes.empty())
	{
		// mutate add neuron if random gene is enabled
		Gene* gene = mGenes[Random::randomRange(0, 100000) % mGenes.size()];
		if(!gene->isDisabled())
		{
			gene->disable();

			// add new link from original source to new neuron, and from new neuron to original target
			addGene(Gene::nextInnovation(), gene->getSource(), mNextNeuronID, Random::randomRange(-1.0f, 1.0f), false);
			addGene(Gene::nextInnovation(), mNextNeuronID, gene->getTarget(), gene->getWeight(), false);

			++mNextNeuronID;
		}
	}
}

//-------------------------------------------------------------

void Genome::mutateDisable(float fitness)
{
	UNREFERENCED_PARAMETER(fitness);
	if(!mGenes.empty())
	{
		// disables random neuron
		Gene* gene = mGenes[Random::randomRange(0, 100000) % mGenes.size()];
		gene->disable();
	}
}

//-------------------------------------------------------------

void Genome::mutateEnable(float fitness)
{
	UNREFERENCED_PARAMETER(fitness);
	if(!mGenes.empty())
	{
		// enables random neuron
		Gene* gene = mGenes[Random::randomRange(0, 100000) % mGenes.size()];
		gene->enable();
	}
}

//-------------------------------------------------------------

uint32_t Genome::randomNeuronID(bool canReturnSensor)
{
	uint32_t returnID = 0;

	uint32_t numHidden = mNextNeuronID - NETWORK_MAX_IN;
	uint32_t numNeurons = mNumInputs + numHidden + NETWORK_NUM_OUT;

	float selectOutputProbability = static_cast<float>(NETWORK_NUM_OUT) / static_cast<float>(numNeurons);
	float selectInputProbability = static_cast<float>(mNumInputs) / static_cast<float>(numNeurons);

	float selection = Random::randomRange(0.0f, 1.0f);
	if(canReturnSensor && selection < selectInputProbability)
	{
		// returning input neuron id
		returnID = Random::randomRange(0, NETWORK_MAX_NODES) % mNumInputs;
	}
	else if(numHidden > 0 && selection >= selectInputProbability && selection < 1.0f - selectOutputProbability)
	{
		// have hidden nodes, returning hidden neuron id
		returnID = NETWORK_MAX_IN + Random::randomRange(0, NETWORK_MAX_NODES) % numHidden;
	}
	else
	{
		// returning output neuron id
		returnID = NETWORK_MAX_NODES + Random::randomRange(0, NETWORK_MAX_NODES) % NETWORK_NUM_OUT;
	}

	return returnID;
}

//-------------------------------------------------------------

void Genome::readFromFile(ifstream& file)
{
	// read size trait
	file >> mSizeGene;

	// read in all gene definitions
	uint32_t numGenes;
	file >> numGenes;

	uint32_t innovation, source, target;
	float weight;
	bool isDisabled;

	for(uint32_t i = 0; i < numGenes; ++i)
	{
		file >> innovation >> source >> target >> weight >> isDisabled;
		mGenes.push_back(new Gene(innovation, source, target, weight, isDisabled));

		// track highest neuron id we've seen
		if(source >= mNextNeuronID && source < NETWORK_MAX_NODES - 1)
		{
			mNextNeuronID = source + 1;
		}
		else if(target >= mNextNeuronID && target < NETWORK_MAX_NODES - 1)
		{
			mNextNeuronID = target + 1;
		}

		// track highest innovation number we've seen
		if(innovation > sHighestInnovParsed)
		{
			sHighestInnovParsed = innovation;
		}
	}

	// track highest genome id number we've seen
	if(mID > sHighestIDParsed)
	{
		sHighestIDParsed = mID;
	}
}

//-------------------------------------------------------------

void Genome::writeToFile() const
{
	ofstream genomeFile;
	stringstream filePath;
	filePath << DIR_GENOMES << "genome_" << mID << ".ini";
	genomeFile.open(filePath.str().c_str(), ofstream::out | ofstream::trunc);

	// write header information
	genomeFile << mID << endl << mUsesNEAT << endl << mIsPrey << endl; 

	// write size trait
	genomeFile << mSizeGene << endl;

	// write each gene definition
	genomeFile << mGenes.size() << endl;
	for(Gene* gene : mGenes)
	{
		genomeFile << 
			gene->getInnovation() << " " <<
			gene->getSource() << " " <<
			gene->getTarget() << " " <<
			gene->getWeight() << " " <<
			gene->isDisabled() << endl;
	}

	genomeFile.close();
}

//-------------------------------------------------------------

void Genome::addGene(uint32_t innovation, uint32_t source, uint32_t target, float weight, bool disabled)
{
	mGenes.push_back(new Gene(innovation, source, target, weight, disabled));
}

//-------------------------------------------------------------

void Genome::clear()
{
	for(Gene* gene : mGenes)
	{
		delete gene;
	}
	mGenes.clear();
}

//-------------------------------------------------------------

const Gene& Genome::operator[](uint32_t index) const
{
	assert(index < mGenes.size());
	assert(mGenes[index] != nullptr);

	return *mGenes[index];
}

//-------------------------------------------------------------

bool Genome::hasGene(uint32_t source, uint32_t target) const
{
	bool hasGene = false;
	for(Gene* gene : mGenes)
	{
		if(gene->getSource() == source && gene->getTarget() == target)
		{
			hasGene = true;
			break;
		}
	}
	return hasGene;
}

//-------------------------------------------------------------

float Genome::getSizeGene() const
{
	return mSizeGene;
}

//-------------------------------------------------------------

uint32_t Genome::getGenomeLength() const
{
	return static_cast<uint32_t>(mGenes.size());
}

//-------------------------------------------------------------

uint32_t Genome::getID() const
{
	return mID;
}

//-------------------------------------------------------------

bool Genome::usesNEAT() const
{
	return mUsesNEAT;
}

//-------------------------------------------------------------

bool Genome::isPrey() const
{
	return mIsPrey;
}

//-------------------------------------------------------------

void Genome::setNextID(std::uint32_t id)
{
	sID = id;
}

//-------------------------------------------------------------

uint32_t Genome::nextID()
{
	return sID++;
}
