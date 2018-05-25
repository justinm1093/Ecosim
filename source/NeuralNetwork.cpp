
#include "pch.h"
#include "NeuralNetwork.h"

using namespace Ecosim;
using namespace std;
using namespace glm;

NeuralNetwork::NeuralNetwork(uint32_t numInputs) :
	mNumInputs(numInputs)
{
}

//-------------------------------------------------------------

NeuralNetwork::~NeuralNetwork()
{
	clearNetwork();
}

//-------------------------------------------------------------

void NeuralNetwork::createNetwork(const Genome& genome)
{
	// sanity check -- blow away everything present
	clearNetwork();

	// add input nodes to the network
	for(uint32_t i = 0; i < mNumInputs; ++i)
	{
		mNeurons[i] = new Neuron(i, Neuron::Type::SENSOR);
	}

	// add output nodes to the network
	uint32_t outputEnd = NETWORK_NUM_OUT + NETWORK_MAX_NODES;
	for(uint32_t i = NETWORK_MAX_NODES; i < outputEnd; ++i)
	{
		mNeurons[i] = new Neuron(i, Neuron::Type::OUTPUT);
	}

	// create connections and hidden nodes
	uint32_t genomeSize = genome.getGenomeLength();
	for(uint32_t i = 0; i < genomeSize; ++i)
	{
		// skip this gene if it is disabled (not in phenotype)
		const Gene& gene = genome[i];
		if(!gene.isDisabled())
		{
			uint32_t targetID = gene.getTarget();
			uint32_t sourceID = gene.getSource();

			// create nodes for the current connection gene if they don't already exist
			//		we know that these ones are HIDDEN -- the sensors and output are already created
			if(mNeurons[sourceID] == nullptr)
			{
				mNeurons[sourceID] = new Neuron(sourceID, Neuron::Type::HIDDEN);
			}
			if(mNeurons[targetID] == nullptr)
			{
				mNeurons[targetID] = new Neuron(targetID, Neuron::Type::HIDDEN);
			}

			// target node's inputs map the connection source to the connection weight
			mNeurons[targetID]->addInput(*mNeurons[sourceID], gene.getWeight());

			// adjust neuron positions for this connection
			mNeurons[sourceID]->setPosition(*mNeurons[targetID]);
			mNeurons[targetID]->setPosition(*mNeurons[sourceID]);
		}
	}
}

//-------------------------------------------------------------

void NeuralNetwork::clearNetwork()
{
	for(auto& neuron : mNeurons)
	{
		delete neuron.second;
	}
	mNeurons.clear();
}

//-------------------------------------------------------------

void NeuralNetwork::evaluate(const float* inputs, float* outputs)
{
	assert(inputs != nullptr);
	assert(outputs != nullptr);

	// set the input values on sensor neurons
	for(uint32_t i = 0; i < mNumInputs; ++i)
	{
		mNeurons[i]->setOutput(inputs[i]);
	}

	// calc output for each neuron in the network
	for(auto& neuron : mNeurons)
	{
		neuron.second->calculateOutput();
	}

	// set the outputs using the values on output neurons
	for(uint32_t i = 0; i < NETWORK_NUM_OUT; ++i)
	{
		outputs[i] = mNeurons[i + NETWORK_MAX_NODES]->getOutput();
	}
}

//-------------------------------------------------------------

void NeuralNetwork::render(Renderer& renderer)
{
	for(auto& neuron : mNeurons)
	{
		neuron.second->render(renderer);
	}
}

//-------------------------------------------------------------

uint32_t NeuralNetwork::getNumInputs() const
{
	return mNumInputs;
}
