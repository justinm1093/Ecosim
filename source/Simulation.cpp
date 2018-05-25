
#include "pch.h"
#include "Simulation.h"

using namespace Ecosim;
using namespace std;
using namespace glm;

Simulation* Simulation::sInstance = nullptr;
Simulation::HandlerFuncs Simulation::sInputHandlers = 
{
	{ GLFW_KEY_P, &Simulation::togglePaused },					// pause / unpause

	{ GLFW_KEY_UP, &Simulation::incrementTimeScale },			// speed up / slow down
	{ GLFW_KEY_DOWN, &Simulation::decrementTimeScale },

	{ GLFW_KEY_LEFT, &Simulation::selectPrevAgent },			// cycle selected agent
	{ GLFW_KEY_RIGHT, &Simulation::selectNextAgent },

	{ GLFW_KEY_M, &Simulation::toggleAllowMutation },			// toggle allow agents to mutate
	{ GLFW_KEY_SPACE, &Simulation::toggleDrawNetwork },			// toggle draw selected agent network

	{ GLFW_KEY_ESCAPE, &Simulation::flagForClose }				// close
};

Simulation* Simulation::instance()
{
	if(sInstance == nullptr)
	{
		sInstance = new Simulation();
	}
	return sInstance;
}

//-------------------------------------------------------------

Simulation::Simulation() :
	mClock(),
	mEventQueue(nullptr),
	mRenderer(nullptr),
	mIsPaused(false)
{
}

//-------------------------------------------------------------

int32_t Simulation::run()
{
	// failed init -- cannot run
	if(!init())
	{
		return -1;
	}

	while(mRenderer->isValid())
	{
		update();
		render();
	}

	shutdown();

	return 0;
}

//-------------------------------------------------------------

bool Simulation::init()
{
	bool result = false;

	// we have a config, and valid event queue and renderer
	if(mConfig != nullptr && mConfig->eventQueue != nullptr && mConfig->renderer != nullptr)
	{
		mEventQueue = mConfig->eventQueue;
		mRenderer = mConfig->renderer;

		if(mRenderer->init())
		{
			// misc inits
			mRenderer->setKeyInputCallback(Simulation::receiveKeyboardInput);
			PhysicalCircle::setBounds(mRenderer->getOrthoBounds());

			// create components
			mEnvironment = std::make_shared<Environment>();
			mAgentManager = std::make_shared<AgentManager>();

			// init components
			mEnvironment->init();
			mAgentManager->init();

			// add components to lists
			mComponents.push_back(mEnvironment);
			mComponents.push_back(mAgentManager);

			result = true;
		}
	}

	return result;
}

//-------------------------------------------------------------

void Simulation::shutdown()
{
	for(auto& component : mComponents)
	{
		component->shutdown();
	}
	mComponents.clear();

	mRenderer->shutdown();
}

//-------------------------------------------------------------

void Simulation::update()
{
	mClock.update();

	if(!mIsPaused)
	{
		for(auto& component : mComponents)
		{
			component->update(mClock);
		}

		// deliver any events that were posted this frame
		mEventQueue->update();
	}
}

//-------------------------------------------------------------

void Simulation::render()
{
	mRenderer->startFrame();

	for(auto& component : mComponents)
	{
		component->render(*mRenderer);
	}

	mRenderer->endFrame();
}

//-------------------------------------------------------------

void Simulation::setConfig(SimConfig& config)
{
	mConfig = &config;
}

//-------------------------------------------------------------

void Simulation::incrementTimeScale()
{
	mClock.incrementTimeScale();
}

//-------------------------------------------------------------

void Simulation::decrementTimeScale()
{
	mClock.decrementTimeScale();
}

//-------------------------------------------------------------

void Simulation::toggleAllowMutation()
{
	mAgentManager->toggleAllowMutation();
}

//-------------------------------------------------------------

void Simulation::toggleDrawNetwork()
{
	mAgentManager->toggleDrawNetwork();
}

//-------------------------------------------------------------

void Simulation::selectNextAgent()
{
	mAgentManager->selectNextAgent();
}

//-------------------------------------------------------------

void Simulation::selectPrevAgent()
{
	mAgentManager->selectPrevAgent();
}

//-------------------------------------------------------------

void Simulation::togglePaused()
{
	mIsPaused = !mIsPaused;
	if(mIsPaused)
	{
		cout << "Simulation -- paused" << endl;
	}
	else
	{
		cout << "Simulation -- unpaused" << endl;
	}
}

//-------------------------------------------------------------

void Simulation::flagForClose()
{
	mRenderer->flagForClose();
}

//-------------------------------------------------------------

void Simulation::receiveKeyboardInput(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(scancode);
	UNREFERENCED_PARAMETER(mods);

	if(action == GLFW_PRESS)
	{
		// only care about the key presses for which we have a mapped handler
		HandlerFuncs::iterator iter = sInputHandlers.find(key);
		if(iter != sInputHandlers.end())
		{
			iter->second(*sInstance);
		}
	}
}
