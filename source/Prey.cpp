
#include "pch.h"
#include "Prey.h"

#include "Predator.h"
#include "Resource.h"
#include "ResourceEffects.h"

using namespace Ecosim;
using namespace std;
using namespace glm;

RTTI_DEFINITIONS(Prey)

Prey::Prey() :
	Agent(PREY_NUM_INPUTS, PREY_COLOR)
{
	registerObj(*this);
}

//-------------------------------------------------------------

Prey::~Prey()
{
	unregisterObj(*this);
}

//-------------------------------------------------------------

void Prey::handleCollision(PhysicalCircle& other)
{
	Predator* asPredator = other.as<Predator>();
	if(asPredator != nullptr)
	{
		asPredator->updateHunger(0.2f);
		kill();
	}
}

//-------------------------------------------------------------

void Prey::updatePerceptionInputs()
{
	// we can see food, water, and predator
	updatePerceptionBlock(Resource<Food>::classTypeID(), 0);
	updatePerceptionBlock(Resource<Water>::classTypeID(), 1);
	updatePerceptionBlock(Predator::classTypeID(), 2);
}

//-------------------------------------------------------------

void Prey::updateCollisions()
{
	// we can collide with food, water, and other agents
	detectCollisions(Resource<Water>::classTypeID());
	detectCollisions(Resource<Food>::classTypeID());
	detectCollisions(Agent::classTypeID());
}
