
#include "pch.h"
#include "ResourceEffects.h"

using namespace Ecosim;
using namespace std;
using namespace glm;

RTTI_DEFINITIONS(Food)
RTTI_DEFINITIONS(Water)

float Food::sHungerYield = 0.0f;
float Water::sThirstYield = 0.0f;

void Food::applyEffect(Agent& agent)
{
	agent.updateHunger(sHungerYield);
}

//-------------------------------------------------------------

void Food::setYields(float hunger)
{
	sHungerYield = hunger;
}

//-------------------------------------------------------------

float Food::getHungerYield()
{
	return sHungerYield;
}

//=============================================================

void Water::applyEffect(Agent& agent)
{
	agent.updateThirst(sThirstYield);
}

//-------------------------------------------------------------

void Water::setYields(float thirst)
{
	sThirstYield = thirst;
}

//-------------------------------------------------------------

float Water::getThirstYield()
{
	return sThirstYield;
}
