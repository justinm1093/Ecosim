
#include "pch.h"
#include "IResource.h"

#include "EventQueue.h"
#include "EventArgs.h"
#include "Event.h"

using namespace Ecosim;
using namespace std;
using namespace glm;

RTTI_DEFINITIONS(IResource)

IResource::IResource(float size, const vec3& pos) :
	PhysicalCircle(size, pos),
	mTimeActive(0.0f),
	mIsActive(false)
{
	registerObj(*this);
}

//-------------------------------------------------------------

IResource::~IResource()
{
	unregisterObj(*this);
}

//-------------------------------------------------------------

void IResource::activate()
{
	mIsActive = true;
	mTimeActive = 0.0f;

	const glm::vec2& bounds = getBounds();
	setPosition(vec3(Random::randomRange(0.0f, bounds.x), Random::randomRange(0.0f, bounds.y), 0.0f));

	activateCollision();
}

//-------------------------------------------------------------

void IResource::deactivate()
{
	mIsActive = false;

	ResourceDeactivate args;
	args.resource = this;
	EventQueue::enqueue(*new Event<ResourceDeactivate>(args));

	deactivateCollision();
}

//-------------------------------------------------------------

float IResource::getTimeActive() const
{
	return mTimeActive;
}

//-------------------------------------------------------------

bool IResource::isActive() const
{
	return mIsActive;
}