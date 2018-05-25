
#include "pch.h"
#include "PhysicalCircle.h"

using namespace Ecosim;
using namespace std;
using namespace glm;

RTTI_DEFINITIONS(PhysicalCircle)

map<uint64_t, vector<PhysicalCircle*>> PhysicalCircle::sCollisionObjectLists;

vec2 PhysicalCircle::sBounds;
float PhysicalCircle::sMaxDistance;

PhysicalCircle::PhysicalCircle(float radius, const vec3& pos, bool isCollisionActive) :
	SimObject(pos),
	mRadius(radius),
	mIsCollisionActive(isCollisionActive)
{
	registerObj(*this);
}

//-------------------------------------------------------------

PhysicalCircle::~PhysicalCircle()
{
	unregisterObj(*this);
}

//-------------------------------------------------------------

void PhysicalCircle::handleCollision(PhysicalCircle& other)
{
	UNREFERENCED_PARAMETER(other);
}

//-------------------------------------------------------------

void PhysicalCircle::activateCollision()
{
	mIsCollisionActive = true;
}

//-------------------------------------------------------------

void PhysicalCircle::deactivateCollision()
{
	mIsCollisionActive = false;
}

//-------------------------------------------------------------

void PhysicalCircle::setRadius(float radius)
{
	mRadius = radius;
}

//-------------------------------------------------------------

float PhysicalCircle::getRadius() const
{
	return mRadius;
}

//-------------------------------------------------------------

bool PhysicalCircle::isCollisionActive() const
{
	return mIsCollisionActive;
}

//-------------------------------------------------------------

void PhysicalCircle::registerObj(PhysicalCircle& obj)
{
	sCollisionObjectLists[obj.instanceTypeID()].push_back(&obj);
}

//-------------------------------------------------------------

void PhysicalCircle::unregisterObj(PhysicalCircle& obj)
{
	vector<PhysicalCircle*>& list = sCollisionObjectLists[obj.instanceTypeID()];
	list.erase(remove(list.begin(), list.end(), &obj), list.end());
}

//-------------------------------------------------------------

void PhysicalCircle::setBounds(const vec2& bounds)
{
	sBounds = bounds;
	sMaxDistance = glm::sqrt((sBounds.x * sBounds.x) + (sBounds.y * sBounds.y));
}

//-------------------------------------------------------------

const vector<PhysicalCircle*>& PhysicalCircle::getCollisionObjList(uint64_t typeID)
{
	return sCollisionObjectLists[typeID];
}

//-------------------------------------------------------------

const vec2& PhysicalCircle::getBounds()
{
	return sBounds;
}

//-------------------------------------------------------------

float PhysicalCircle::getMaxDistance()
{
	return sMaxDistance;
}
