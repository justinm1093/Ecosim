
#include "pch.h"
#include "Simulation.h"

using namespace Ecosim;
using namespace std;
using namespace glm;

string sName = "EcoSim";
int32_t sWidth = 1500;
int32_t sHeight = 1000;

int32_t main(int32_t argc, char* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	// init random
	Random::seedRandom();

	// set renderer configuration
	RenderConfig renderConfig;
	renderConfig.name = sName;
	renderConfig.width = sWidth;
	renderConfig.height = sHeight;

	Renderer* renderer = Renderer::instance();
	renderer->setConfig(renderConfig);

	// set simulation configuration
	SimConfig simConfig;
	simConfig.eventQueue = EventQueue::instance();
	simConfig.renderer = renderer;

	Simulation* simulation = Simulation::instance();
	simulation->setConfig(simConfig);

	return simulation->run();
}
