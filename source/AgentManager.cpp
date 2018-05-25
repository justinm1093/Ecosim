
#include "pch.h"
#include "AgentManager.h"

#include "Predator.h"
#include "Prey.h"

#include "EventArgs.h"
#include "Event.h"

using namespace std::experimental::filesystem;
using namespace Ecosim;
using namespace std;
using namespace glm;

AgentManager::AgentManager() :
	mAllowsMutation(true),
	mDrawsNetwork(false)
{
	Event<AgentDeath>::subscribe(*this);
}

//-------------------------------------------------------------

AgentManager::~AgentManager()
{
	Event<AgentDeath>::unsubscribe(*this);
}

//-------------------------------------------------------------

void AgentManager::init()
{
	// for each genome ini file...
	for(auto& file : directory_iterator(DIR_GENOMES))
	{
		// read genome header information
		ifstream genomeFile;
		genomeFile.open(file.path().c_str(), ifstream::in);

		uint32_t id; bool usesNEAT, isPrey;
		genomeFile >> id >> usesNEAT >> isPrey;

#if USES_PREDATOR_PREY
		// create agent of the appropriate type
		Genome* genome = new Genome(id, isPrey ? PREY_NUM_INPUTS : PREDATOR_NUM_INPUTS, usesNEAT, isPrey);
		Agent* agent = isPrey ? new Prey() : nullptr;
		if(agent == nullptr)
		{
			agent = new Predator();
		}
#else
		// create agent
		Genome* genome = new Genome(id, NETWORK_MAX_IN, usesNEAT, isPrey);
		Agent* agent = new Agent();
#endif

		// parse genome
		genome->readFromFile(genomeFile);
		genomeFile.close();

		// activate agent
		mAgents.push_back(agent);
		agent->setGenome(*genome);
		agent->activate();
	}

	// set static trackers for Genome ID and Gene innovation
	Gene::setNextInnovation(Genome::sHighestInnovParsed + 1);
	Genome::setNextID(Genome::sHighestIDParsed + 1);

	// select agent
	mSelectedAgentIndex = 0;
}

//-------------------------------------------------------------

void AgentManager::shutdown()
{
	// remove existing genome files
	for(auto& file : directory_iterator(DIR_GENOMES))
	{
		std::experimental::filesystem::remove(file.path());
	}

	// write genomes to files and delete agents
	for(Agent* agent : mAgents)
	{
		agent->getGenome().writeToFile();
		delete agent;
	}
	mAgents.clear();
}

//-------------------------------------------------------------

void AgentManager::update(const SimClock& clock)
{
	for(Agent* agent : mAgents)
	{
		agent->update(clock);
	}
}

//-------------------------------------------------------------

void AgentManager::render(Renderer& renderer)
{
	for(Agent* agent : mAgents)
	{
		agent->render(renderer);
	}

	if(mDrawsNetwork)
	{
		mAgents[mSelectedAgentIndex]->drawNetwork(renderer);
	}
}

//-------------------------------------------------------------

void AgentManager::selectNextAgent()
{
	mAgents[mSelectedAgentIndex]->deselect();

	++mSelectedAgentIndex;
	mSelectedAgentIndex %= mAgents.size();

	if(mDrawsNetwork)
	{
		mAgents[mSelectedAgentIndex]->select();
	}
}

//-------------------------------------------------------------

void AgentManager::selectPrevAgent()
{
	mAgents[mSelectedAgentIndex]->deselect();

	--mSelectedAgentIndex;
	if(mSelectedAgentIndex >= mAgents.size())
	{
		mSelectedAgentIndex = static_cast<uint32_t>(mAgents.size() - 1);
	}

	if(mDrawsNetwork)
	{
		mAgents[mSelectedAgentIndex]->select();
	}
}

//-------------------------------------------------------------

void AgentManager::toggleAllowMutation()
{
	mAllowsMutation = !mAllowsMutation;
	if(mAllowsMutation)
	{
		cout << "Mutation -- enabled" << endl;
	}
	else
	{
		cout << "Mutation -- disabled" << endl;
	}
}

//-------------------------------------------------------------

void AgentManager::toggleDrawNetwork()
{
	mDrawsNetwork = !mDrawsNetwork;
	if(mDrawsNetwork)
	{
		mAgents[mSelectedAgentIndex]->select();
	}
	else
	{
		mAgents[mSelectedAgentIndex]->deselect();
	}
}

//-------------------------------------------------------------

void AgentManager::notify(const IPublisher& e)
{
	if(Event<AgentDeath>* agentDeath = e.as<Event<AgentDeath>>())
	{
		const AgentDeath& args = agentDeath->message();
		Agent* deadAgent = args.agent;

		// get best active agents to be parents for the replacement
		Agent* bestAgent = nullptr;
		float bestFitness = 0.0f;

		Agent* secondBestAgent = nullptr;
		float secondBestFitness = 0.0f;

		for(Agent* agent : mAgents)
		{
			// skip dead agents and agents that aren't of our type
			if(!agent->isAlive() || agent->instanceTypeID() != deadAgent->instanceTypeID())
			{
				continue;
			}

			if(bestAgent == nullptr)
			{
				// first agent is current best agent
				bestFitness = agent->getFitness();
				bestAgent = agent;
				continue;
			}

			float fitness = agent->getFitness();
			if(fitness > bestFitness)
			{
				// new best agent, demote old best agent to second best
				secondBestFitness = bestFitness;
				secondBestAgent = bestAgent;
				bestFitness = fitness;
				bestAgent = agent;
			}
			else if(fitness > secondBestFitness)
			{
				// else new second best agent, demote old second best
				secondBestFitness = fitness;
				secondBestAgent = agent;
			}
		}

		Genome* newGenome = nullptr;
		if(!mAllowsMutation || bestAgent == nullptr)
		{
			// no mutation allowed, or no best agent -- we create a clone of the dead agent
			newGenome = new Genome(deadAgent->getGenome());
		}
		else if(secondBestAgent == nullptr)
		{
			// no second best agent -- we create a clone of the best agent
			newGenome = new Genome(bestAgent->getGenome());
		}
		else
		{
			// 2 parents -- we create a child of the two best agents
			newGenome = bestAgent->getGenome().crossover(secondBestAgent->getGenome(), bestFitness == secondBestFitness);
		}

		assert(newGenome != nullptr);
		if(mAllowsMutation)
		{
			newGenome->mutate(deadAgent->getFitness());
		}

#if USES_PREDATOR_PREY
		writeFitnessToFile(newGenome->isPrey() ? PREY_FITNESS_FILE : PREDATOR_FITNESS_FILE, deadAgent->getFitness());
#else
		writeFitnessToFile(AGENT_FITNESS_FILE, deadAgent->getFitness());
#endif

		// reinit dead agent
		deadAgent->setGenome(*newGenome);
		deadAgent->activate();
	}
}

//-------------------------------------------------------------

void AgentManager::writeFitnessToFile(const string& filename, float fitness) const
{
	ofstream fitnessFile;

	fitnessFile.open(filename, ofstream::out | ofstream::app);
	if(fitnessFile.is_open())
	{
		fitnessFile << fitness << endl;
		fitnessFile.close();
	}
}
