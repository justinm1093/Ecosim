
#include "pch.h"
#include "Random.h"

#include <ctime>

using namespace Ecosim;
using namespace std::chrono;
using namespace std;
using namespace glm;

void Random::seedRandom()
{
	srand(static_cast<uint32_t>(time(nullptr)));
}

//-------------------------------------------------------------

int32_t Random::randomRange(int32_t min, int32_t max)
{
	float r = static_cast<float>(rand());

	r /= static_cast<float>(RAND_MAX);
	r *= max - min;
	r += min;

	return static_cast<int32_t>(r);
}

//-------------------------------------------------------------

float Random::randomRange(float min, float max)
{
	float r = static_cast<float>(rand());

	r /= static_cast<float>(RAND_MAX);
	r *= max - min;
	r += min;

	return r;
}

//-------------------------------------------------------------

vec3 Random::randomVec(float radius)
{
	return vec3(randomRange(-radius, radius), randomRange(-radius, radius), 0);
}
