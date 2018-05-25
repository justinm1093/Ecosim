
#include "pch.h"
#include "Neuron.h"

using namespace Ecosim;
using namespace std;
using namespace glm;

map<Neuron::Type, Neuron::NeuronPositionData> Neuron::sPositionData = 
{
	{ Neuron::Type::SENSOR, { vec3(15.0f, 15.0f, 0.0f), 0 } },
	{ Neuron::Type::HIDDEN, { vec3(750.0f, 240.0f, 0.0f), NETWORK_MAX_IN } },
	{ Neuron::Type::OUTPUT, { vec3(1000.0f, 420.0f, 0.0f), NETWORK_MAX_NODES } }
};

const uint32_t NUM_NEURONS_PER_COL = 15;

const float NEURON_RADIUS = 10.0f;
const float NEURON_VERT_OFFSET = NEURON_RADIUS + 20.0f;
const float NEURON_HORIZ_OFFSET = NEURON_RADIUS + 20.0f;


Neuron::Neuron(uint32_t id, Type type) :
	mID(id),
	mType(type)
{
	// get position data based on type
	NeuronPositionData& positionData = sPositionData[mType];
	mPosition = positionData.origin;

	// handle position offsets
	if(mType != Type::HIDDEN)
	{
		mPosition.y += (mID - positionData.firstID) * NEURON_VERT_OFFSET;
	}
}

//-------------------------------------------------------------

void Neuron::calculateOutput()
{
	// only non-sensor neurons activate based on neuron output
	if(mType != Type::SENSOR)
	{
		// sum of all inputs to this neuron
		float sum = 0.0f;
		for(auto& input : mInputs)
		{
			sum += input.first->getOutput() * input.second;
		}

		if(mInputs.size() > 0)
		{
			mValue = SimMath::neuronSigmoid(sum);
		}
	}
}

//-------------------------------------------------------------

void Neuron::render(Renderer& renderer)
{
	// draw lines leading to all our inputs (red = negative weight, green = positive weight)
	for(auto& input : mInputs)
	{
		if(input.second < 0.0f)
		{
			renderer.drawLine(mPosition, input.first->mPosition, vec3(SimMath::lerp(glm::abs(input.second), 0.0f, 1.0f), 0.0f, 0.1f));
		}
		else
		{
			renderer.drawLine(mPosition, input.first->mPosition, vec3(0.0f, SimMath::lerp(input.second, 0.0f, 1.0f), 0.1f));
		}
	}

	// draw circle (red = negative stimulation, cyan = positive stimulation)
	if(mValue < 0.0f)
	{
		renderer.drawColoredCircle(mPosition, NEURON_RADIUS, vec3(SimMath::lerp(glm::abs(mValue), 0.0f, 1.0f), 0.0f, 0.1f));
	}
	else
	{
		renderer.drawColoredCircle(mPosition, NEURON_RADIUS, vec3(0.0f, SimMath::lerp(mValue, 0.0f, 1.0f), 0.1f));
	}
}

//-------------------------------------------------------------

void Neuron::addInput(Neuron& node, float weight)
{
	// connection origins paired with connection weights
	mInputs[&node] = weight;
}

//-------------------------------------------------------------

void Neuron::setPosition(Neuron& other)
{
	if(mType == Type::HIDDEN)
	{
		mPosition.x = 0.7f * mPosition.x + 0.3f * other.mPosition.x;
		mPosition.y = 0.7f * mPosition.y + 0.3f * other.mPosition.y;

		if(mPosition.x >= other.mPosition.x)
		{
			mPosition.x -= NEURON_HORIZ_OFFSET;
		}
		if(mPosition.x < sPositionData[Type::SENSOR].origin.x + NEURON_HORIZ_OFFSET * 2)
		{
			mPosition.x = sPositionData[Type::SENSOR].origin.x + NEURON_HORIZ_OFFSET * 2;
		}
		else if(mPosition.x > sPositionData[Type::OUTPUT].origin.x - NEURON_HORIZ_OFFSET)
		{
			mPosition.x = sPositionData[Type::OUTPUT].origin.x - NEURON_HORIZ_OFFSET;
		}
	}
}

//-------------------------------------------------------------

const vec3& Neuron::getPosition() const
{
	return mPosition;
}

//-------------------------------------------------------------

void Neuron::setOutput(float val)
{
	mValue = val;
}

//-------------------------------------------------------------

float Neuron::getOutput() const
{
	return mValue;
}

//-------------------------------------------------------------

uint32_t Neuron::getID() const
{
	return mID;
}

//-------------------------------------------------------------

Neuron::Type Neuron::getType() const
{
	return mType;
}
