
#include "pch.h"
#include "ISimComponent.h"

using namespace Ecosim;
using namespace std;
using namespace glm;

RTTI_DEFINITIONS(ISimComponent)

void ISimComponent::init()
{
}

//-------------------------------------------------------------

void ISimComponent::shutdown()
{
}

//-------------------------------------------------------------

void ISimComponent::update(const SimClock& clock)
{
	UNREFERENCED_PARAMETER(clock);
}

//-------------------------------------------------------------

void ISimComponent::render(Renderer& renderer)
{
	UNREFERENCED_PARAMETER(renderer);
}
