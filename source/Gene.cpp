
#include "pch.h"
#include "Gene.h"

using namespace Ecosim;
using namespace std;
using namespace glm;

uint32_t Gene::sInnovation = 0;

Gene::Gene(uint32_t innovation, uint32_t source, uint32_t target, float weight, bool disabled) :
	mInnovation(innovation),
	mSource(source),
	mTarget(target),
	mWeight(weight),
	mIsDisabled(disabled)
{
}

//-------------------------------------------------------------

Gene::Gene(const Gene& other) :
	mInnovation(other.mInnovation),
	mSource(other.mSource),
	mTarget(other.mTarget),
	mWeight(other.mWeight),
	mIsDisabled(other.mIsDisabled)
{
}

//-------------------------------------------------------------

Gene& Gene::operator=(const Gene& other)
{
	if(this != &other)
	{
		mInnovation = other.mInnovation;
		mSource = other.mSource;
		mTarget = other.mTarget;
		mWeight = other.mWeight;
		mIsDisabled = other.mIsDisabled;
	}

	return *this;
}

//-------------------------------------------------------------

void Gene::enable()
{
	mIsDisabled = false;
}

//-------------------------------------------------------------

void Gene::disable()
{
	mIsDisabled = true;
}

//-------------------------------------------------------------

void Gene::toggleEnabled()
{
	mIsDisabled = !mIsDisabled;
}

//-------------------------------------------------------------

void Gene::setWeight(float weight)
{
	mWeight = weight;
}

//-------------------------------------------------------------

uint32_t Gene::getInnovation() const
{
	return mInnovation;
}

//-------------------------------------------------------------

uint32_t Gene::getSource() const
{
	return mSource;
}

//-------------------------------------------------------------

uint32_t Gene::getTarget() const
{
	return mTarget;
}

//-------------------------------------------------------------

float Gene::getWeight() const
{
	return mWeight;
}

//-------------------------------------------------------------

bool Gene::isDisabled() const
{
	return mIsDisabled;
}

//-------------------------------------------------------------

void Gene::setNextInnovation(uint32_t innovation)
{
	sInnovation = innovation;
}

//-------------------------------------------------------------

uint32_t Gene::nextInnovation()
{
	return sInnovation++;
}
