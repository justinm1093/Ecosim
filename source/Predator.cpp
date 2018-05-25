
#include "pch.h"
#include "Predator.h"

#include "Prey.h"
#include "Resource.h"
#include "ResourceEffects.h"

using namespace Ecosim;
using namespace std;
using namespace glm;

RTTI_DEFINITIONS(Predator)

Predator::Predator() :
	Agent(PREDATOR_NUM_INPUTS, PREDATOR_COLOR)
{
	registerObj(*this);
}

//-------------------------------------------------------------

Predator::~Predator()
{
	unregisterObj(*this);
}

//-------------------------------------------------------------

void Predator::handleCollision(PhysicalCircle& other)
{
	Prey* asPrey = other.as<Prey>();
	if(asPrey != nullptr)
	{
		updateHunger(0.2f);
		asPrey->kill();
	}
}

//-------------------------------------------------------------

void Predator::updatePerceptionInputs()
{
	// we can see water and prey
	updatePerceptionBlock(Resource<Water>::classTypeID(), 0);
	updatePerceptionBlock(Prey::classTypeID(), 1);
}

//-------------------------------------------------------------

void Predator::updateCollisions()
{
	// we can collide with water and other agents
	detectCollisions(Resource<Water>::classTypeID());
	detectCollisions(Agent::classTypeID());
}
