
#include "pch.h"
#include "SimMath.h"

using namespace Ecosim;
using namespace std;
using namespace glm;

float SimMath::neuronSigmoid(float val)
{
	return 2.0f / (glm::exp(-1.5f * val) + 1.0f) - 1.0f;
}

//-------------------------------------------------------------

float SimMath::moveSpeedFromSize(float size)
{
	return 500.0f / (glm::exp(0.04f * size) + 1.0f);
}

//-------------------------------------------------------------

float SimMath::steerSpeedFromSize(float size)
{
	return 20.0f / (glm::exp(0.04f * size) + 1.0f);
}

//-------------------------------------------------------------

float SimMath::mutationChanceFromFitness(float fitness)
{
	return 0.15f / (glm::exp(4.0f * fitness) + 1.0f);
}

//-------------------------------------------------------------

float SimMath::clampNum(float val, float min, float max)
{
	if(val < min)
	{
		return min;
	}
	else if(val > max)
	{
		return max;
	}

	return val;
}

//-------------------------------------------------------------

float SimMath::lerp(float val, float min, float max)
{
	if(val < min)
	{
		return 0.0f;
	}
	else if(val > max)
	{
		return 1.0f;
	}

	return (val - min) / (max - min);
}
