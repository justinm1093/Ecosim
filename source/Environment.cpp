
#include "pch.h"
#include "Environment.h"

#include "EventQueue.h"
#include "EventArgs.h"
#include "Event.h"

using namespace Ecosim;
using namespace std;
using namespace glm;

// resources
const uint32_t RESOURCE_POOL_SIZE = 25;

const glm::vec3 RESOURCE_COLOR_FOOD = glm::vec3(0.9f, 0.7f, 0.4f);
const glm::vec3 RESOURCE_COLOR_WATER = glm::vec3(0.4f, 0.6f, 0.8f);

const float RESOURCE_SPAWNRATE_FOOD = 3.0f;
const float RESOURCE_SPAWNRATE_WATER = 3.0f;

const float RESOURCE_YIELD_FOOD = 0.2f;
const float RESOURCE_YIELD_WATER = 0.2f;

const float RESOURCE_LIFETIME = 30.0f;
const float RESOURCE_SIZE = 10.0f;


Environment::Environment() :
	mTime(0.0f)
{
	Event<ResourceDeactivate>::subscribe(*this);
}

//-------------------------------------------------------------

Environment::~Environment()
{
	Event<ResourceDeactivate>::unsubscribe(*this);
}

//-------------------------------------------------------------

void Environment::init()
{
	mTime = 0.0f;

	uint64_t foodID = Resource<Food>::classTypeID();
	uint64_t waterID = Resource<Water>::classTypeID();

	// init resource stats
	Resource<Food>::init(RESOURCE_COLOR_FOOD, RESOURCE_LIFETIME, RESOURCE_SIZE);
	Resource<Water>::init(RESOURCE_COLOR_WATER, RESOURCE_LIFETIME, RESOURCE_SIZE);

	Food::setYields(RESOURCE_YIELD_FOOD);
	Water::setYields(RESOURCE_YIELD_WATER);

	// init spawn functions
	mResourceSpawnFuncs[foodID] = &Environment::spawnFood;
	mResourceSpawnFuncs[waterID] = &Environment::spawnWater;

	// init spawn times
	mResourceSpawnTimes[foodID] = mResourceSpawnRates[foodID] = RESOURCE_SPAWNRATE_FOOD;
	mResourceSpawnTimes[waterID] = mResourceSpawnRates[waterID] = RESOURCE_SPAWNRATE_WATER;

	// create pools
	mActivePools[foodID];
	mActivePools[waterID];

	ResourcePool& foodPool = mInactivePools[foodID];
	ResourcePool& waterPool = mInactivePools[waterID];

	for(uint32_t i = 0; i < RESOURCE_POOL_SIZE; ++i)
	{
		foodPool.push_back(new Resource<Food>());
		waterPool.push_back(new Resource<Water>());
	}
}

//-------------------------------------------------------------

void Environment::shutdown()
{
	for(auto& iter : mActivePools)
	{
		ResourcePool& activePool = mActivePools[iter.first];
		ResourcePool& inactivePool = mInactivePools[iter.first];

		// clear all resources in pools
		for(IResource* resource : activePool)
		{
			delete resource;
		}
		for(IResource* resource : inactivePool)
		{
			delete resource;
		}

		// clear pools
		activePool.clear();
		inactivePool.clear();
	}

	// clear all maps
	mActivePools.clear();
	mInactivePools.clear();
	mResourceSpawnFuncs.clear();
	mResourceSpawnTimes.clear();
	mResourceSpawnRates.clear();
}

//-------------------------------------------------------------

void Environment::update(const SimClock& clock)
{
	mTime += clock.getDeltaTimeScaled();

	// update resources
	for(auto& iter : mActivePools)
	{
		uint64_t resourceID = iter.first;

		// spawn resource if necessary
		if(mTime > mResourceSpawnTimes[resourceID])
		{
			mResourceSpawnTimes[resourceID] = mTime + mResourceSpawnRates[resourceID];
			mResourceSpawnFuncs[resourceID](*this);
		}

		// update active resources
		for(IResource* resource : mActivePools[resourceID])
		{
			resource->update(clock);
		}
	}
}

//-------------------------------------------------------------

void Environment::render(Renderer& renderer)
{
	for(auto& iter : mActivePools)
	{
		// render active resources
		for(IResource* resource : mActivePools[iter.first])
		{
			resource->render(renderer);
		}
	}
}

//-------------------------------------------------------------

void Environment::notify(const IPublisher& e)
{
	if(Event<ResourceDeactivate>* resourceDeactivate = e.as<Event<ResourceDeactivate>>())
	{
		const ResourceDeactivate& args = resourceDeactivate->message();
		assert(args.resource != nullptr);
		moveResource(*args.resource);
	}
}

//-------------------------------------------------------------

void Environment::moveResource(IResource& resource)
{
	uint64_t resourceID = resource.instanceTypeID();
	ResourcePool& activePool = mActivePools[resourceID];
	ResourcePool& inactivePool = mInactivePools[resourceID];

	activePool.erase(remove(activePool.begin(), activePool.end(), &resource), activePool.end());
	inactivePool.push_back(&resource);
}

//-------------------------------------------------------------

void Environment::spawnFood()
{
	uint64_t resourceID = Resource<Food>::classTypeID();

	ResourcePool& mInactivePool = mInactivePools[resourceID];
	ResourcePool& mActivePool = mActivePools[resourceID];

	if(mInactivePool.empty())
	{
		mActivePool.push_back(new Resource<Food>());
	}
	else
	{
		mActivePool.push_back(mInactivePool.back());
		mInactivePool.pop_back();
	}

	mActivePool.back()->activate();
}

//-------------------------------------------------------------

void Environment::spawnWater()
{
	uint64_t resourceID = Resource<Water>::classTypeID();

	ResourcePool& mInactivePool = mInactivePools[resourceID];
	ResourcePool& mActivePool = mActivePools[resourceID];

	if(mInactivePool.empty())
	{
		mActivePool.push_back(new Resource<Water>());
	}
	else
	{
		mActivePool.push_back(mInactivePool.back());
		mInactivePool.pop_back();
	}

	mActivePool.back()->activate();
}
