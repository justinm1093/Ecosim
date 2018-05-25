
#include "pch.h"
#include "SimObject.h"

using namespace Ecosim;
using namespace std;
using namespace glm;

RTTI_DEFINITIONS(SimObject)

SimObject::SimObject(const vec3& pos) :
	ISimComponent(),
	mPosition(pos)
{
}

//-------------------------------------------------------------

void SimObject::setPosition(const vec3& newPos)
{
	mPosition = newPos;
}

//-------------------------------------------------------------

const vec3& SimObject::getPosition() const
{
	return mPosition;
}
