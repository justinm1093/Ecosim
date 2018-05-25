

#include "pch.h"
#include "Agent.h"

#include "EventQueue.h"
#include "EventArgs.h"
#include "Event.h"

#include "Resource.h"
#include "ResourceEffects.h"

using namespace Ecosim;
using namespace std;
using namespace glm;

RTTI_DEFINITIONS(Agent)

// perception subdivision constants
const float PERCEPTION_SUBDIV_WIDTH = PI / PERCEPTION_NUM_VISION_CONES;
const uint32_t PERCEPTION_HALF_SUBDIVS = PERCEPTION_NUM_VISION_CONES / 2;

// neural network output array offsets
const uint32_t OUTPUT_SPEED = 0;
const uint32_t OUTPUT_STEER_L = 1;
const uint32_t OUTPUT_STEER_R = 2;

// status constants
const float MAX_LIFETIME = 600.0f;
const float HUNGER_RATE = 100.0f;
const float THIRST_RATE = 100.0f;
const float MAX_HUNGER = 1.0f;
const float MAX_THIRST = 1.0f;
const float MAX_ENERGY = 1.0f;

// misc constants
const float DIGEST_EFFICIENCY = 0.97f;
const float MOVE_COEF = 0.015f;

// fitness weights
const float FITNESS_WEIGHT_TIME = 0.8f;
const float FITNESS_WEIGHT_HEALTH = 0.2f;
const float FITNESS_WEIGHT_HUNGER = 0.15f;
const float FITNESS_WEIGHT_THIRST = 0.15f;
const float FITNESS_WEIGHT_ENERGY = 0.7f;

// display constants
const glm::vec3 SELECTED_COLOR = glm::vec3(0, 0.85f, 0);
const float SELECTED_FLASH_DURATION = 0.6f;


Agent::Agent(uint32_t numInputs, const glm::vec3& color, const glm::vec3& pos, const glm::vec3& facing) :
	PhysicalCircle(0.0f, pos),
	mColor(color),
	mUnselectedColor(color),
	mBrain(new NeuralNetwork(numInputs)),
	mDNA(nullptr),
	mIsAlive(true),
	mIsSelected(false)
{
	// verifying values for the input block
	//		we get the number of perception blocks, then set the last 3 inputs to status
	assert(numInputs >= NETWORK_MIN_IN && numInputs <= NETWORK_MAX_IN);
	mNumPerceptionBlocks = (numInputs - NETWORK_NUM_IN_STATUS) / PERCEPTION_NUM_VISION_CONES;
	INPUT_ENERGY = numInputs - 1;
	INPUT_THIRST = numInputs - 2;
	INPUT_HUNGER = numInputs - 3;

	// register this object, create input and output blocks
	registerObj(*this);
	setFacing(facing);

	mInputs = new float[numInputs]();
	mOutputs = new float[NETWORK_NUM_OUT]();
}

//-------------------------------------------------------------

Agent::~Agent()
{
	unregisterObj(*this);

	delete mBrain;
	delete mDNA;

	delete[] mInputs;
	delete[] mOutputs;
}

//-------------------------------------------------------------

void Agent::activate()
{
	assert(mDNA != nullptr);

	// reset physical stats
	mSize = mDNA->getSizeGene();
	mInvHungerRate = 1.0f / HUNGER_RATE;
	mInvThirstRate = 1.0f / THIRST_RATE;
	mMaxSpeed = SimMath::moveSpeedFromSize(mSize);
	mMaxSteer = SimMath::steerSpeedFromSize(mSize);

	mInputs[INPUT_HUNGER] = Random::randomRange(MAX_HUNGER * 0.9f, MAX_HUNGER);
	mInputs[INPUT_THIRST] = Random::randomRange(MAX_THIRST * 0.9f, MAX_THIRST);
	mInputs[INPUT_ENERGY] = Random::randomRange(MAX_ENERGY * 0.9f, MAX_ENERGY);

	mTimeAlive = 0.0f;
	mIsAlive = true;

	// reset collision
	activateCollision();
	setRadius(mSize);

	// set new position
	const glm::vec2& bounds = getBounds();
	setPosition(glm::vec3(Random::randomRange(0.0f, bounds.x), Random::randomRange(0.0f, bounds.y), 0.0f));

	// create neural network using genome
	mBrain->createNetwork(*mDNA);
}

//-------------------------------------------------------------

void Agent::update(const SimClock& clock)
{
	if(mIsAlive)
	{
		float deltaSecondsScaled = clock.getDeltaTimeScaled();

		// update perception and status
		updateStatusInputs(deltaSecondsScaled);
		updatePerceptionInputs();

		// evaluate move controls and update collisions
		mBrain->evaluate(mInputs, mOutputs);
		applyMovement(deltaSecondsScaled);
		updateCollisions();

		// update age, kill if too old
		mTimeAlive += deltaSecondsScaled;
		if(mTimeAlive >= MAX_LIFETIME)
		{
			kill();
		}

		// update flashing timer if selected
		if(mIsSelected)
		{
			mFlashingTimer += clock.getDeltaTime();
			if(mFlashingTimer > SELECTED_FLASH_DURATION)
			{
				mColor = mColor == mUnselectedColor ? SELECTED_COLOR : mUnselectedColor;
				mFlashingTimer = 0.0f;
			}
		}
	}
}

//-------------------------------------------------------------

void Agent::updatePerceptionInputs()
{
	// we can see and collide with food, water, and other agents
	updatePerceptionBlock(Resource<Food>::classTypeID(), 0);
	updatePerceptionBlock(Resource<Water>::classTypeID(), 1);
	updatePerceptionBlock(Agent::classTypeID(), 2);
}

//-------------------------------------------------------------

void Agent::updateCollisions()
{
	detectCollisions(Resource<Water>::classTypeID());
	detectCollisions(Resource<Food>::classTypeID());
	detectCollisions(Agent::classTypeID());
}

//-------------------------------------------------------------

void Agent::updatePerceptionBlock(uint64_t typeID, uint32_t perceptionBlockID)
{
	// are we in bounds for the perception data?
	if(perceptionBlockID < mNumPerceptionBlocks)
	{
		// declaring here so we don't have to constantly redo it inside the loop
		uint32_t visionConeID;
		uint32_t numObjsPerCone[PERCEPTION_NUM_VISION_CONES];
		float dotProduct;
		float angleBetween;
		float distanceBetween;
		float maxDist = getMaxDistance();
		glm::vec3 crossProduct;
		glm::vec3 vectorBetween;

		// reset perception data in this perception block
		uint32_t blockID = PERCEPTION_NUM_VISION_CONES * perceptionBlockID;
		for(uint32_t i = 0; i < PERCEPTION_NUM_VISION_CONES; ++i)
		{
			mInputs[blockID + i] = maxDist;
			numObjsPerCone[i] = 0;
		}

		// check each collidable object of the given type
		for(PhysicalCircle* collisionObj : getCollisionObjList(typeID))
		{
			// skip if object if collision is inactive
			if(!collisionObj->isCollisionActive() || collisionObj == this)
			{
				continue;
			}

			vectorBetween = collisionObj->getPosition() - mPosition;
			distanceBetween = glm::length(vectorBetween);
			vectorBetween = glm::normalize(vectorBetween);

			// check if current object is in our field of view
			dotProduct = glm::dot(mFacing, vectorBetween);
			if(dotProduct >= 0.0f)
			{
				// get the discrete vision cone this object is in
				crossProduct = glm::cross(mFacing, vectorBetween);
				angleBetween = glm::acos(dotProduct);

				visionConeID = crossProduct.z >= 0.0f ?
					PERCEPTION_HALF_SUBDIVS + static_cast<uint32_t>(angleBetween / PERCEPTION_SUBDIV_WIDTH) :
					PERCEPTION_HALF_SUBDIVS - static_cast<uint32_t>(angleBetween / PERCEPTION_SUBDIV_WIDTH) - 1;

				if(visionConeID >= PERCEPTION_NUM_VISION_CONES)
				{
					visionConeID = 0;
				}

				// input is the sum of distances of objects in that region (we also count the number of objects)
				mInputs[blockID + visionConeID] += distanceBetween;
				++numObjsPerCone[visionConeID];
			}
		}

		// dividing total distances by number of objects to get the average distance of things in each region
		//		then normalizing against the maximum distance
		for(uint32_t i = 0; i < PERCEPTION_NUM_VISION_CONES; ++i)
		{
			uint32_t inputIndex = blockID + i;
			if(numObjsPerCone[i] != 0)
			{
				mInputs[inputIndex] -= maxDist;
				mInputs[inputIndex] /= numObjsPerCone[i];
			}

			mInputs[inputIndex] /= -maxDist;
			++mInputs[inputIndex];
		}
	}
}

//-------------------------------------------------------------

void Agent::detectCollisions(uint64_t typeID)
{
	float distanceBetween;
	glm::vec3 vectorBetween;

	// check each collidable object of the given type
	for(PhysicalCircle* collisionObj : getCollisionObjList(typeID))
	{
		// skip if object if collision is inactive
		if(!collisionObj->isCollisionActive() || collisionObj == this)
		{
			continue;
		}

		vectorBetween = collisionObj->getPosition() - mPosition;
		distanceBetween = glm::length(vectorBetween);
		vectorBetween = glm::normalize(vectorBetween);

		// handle collisions if we're close enough to the object
		if(distanceBetween < getRadius() + collisionObj->getRadius())
		{
			collisionObj->handleCollision(*this);
			mPosition = mPosition - (vectorBetween * 5.0f);
		}
	}
}

//-------------------------------------------------------------

void Agent::updateStatusInputs(float deltaSeconds)
{
	// decay hunger and thirst inputs
	float hungerDecay = (MAX_HUNGER * mInvHungerRate) * deltaSeconds;
	mInputs[INPUT_HUNGER] -= hungerDecay;
	mInputs[INPUT_THIRST] -= (MAX_THIRST * mInvThirstRate) * deltaSeconds;

	// increase energy as we digest
	mInputs[INPUT_ENERGY] += DIGEST_EFFICIENCY * hungerDecay;
	if(mInputs[INPUT_ENERGY] > MAX_ENERGY)
	{
		mInputs[INPUT_ENERGY] = MAX_ENERGY;
	}

	// flag for death if we starve or dehydrate
	if(mInputs[INPUT_HUNGER] < 0.0f || mInputs[INPUT_THIRST] < 0.0f)
	{
		kill();
	}
}

//-------------------------------------------------------------

void Agent::applyMovement(float deltaSeconds)
{
	if(mInputs[INPUT_ENERGY] > 0.0f)
	{
		mOutputs[OUTPUT_SPEED] = SimMath::clampNum(mOutputs[OUTPUT_SPEED], -1.0f, 1.0f);
		mOutputs[OUTPUT_STEER_L] = SimMath::clampNum(mOutputs[OUTPUT_STEER_L], -1.0f, 1.0f);
		mOutputs[OUTPUT_STEER_R] = SimMath::clampNum(mOutputs[OUTPUT_STEER_R], -1.0f, 1.0f);

		// rotate using net steer output
		float steer = (mOutputs[OUTPUT_STEER_R] - mOutputs[OUTPUT_STEER_L]) * mMaxSteer * deltaSeconds;
		setFacing(vec3(
			mFacing.x * glm::cos(steer) - mFacing.y * glm::sin(steer),
			mFacing.x * glm::sin(steer) + mFacing.y * glm::cos(steer),
			0));

		// move and clamp new position to bounds
		const glm::vec2& bounds = getBounds();
		setPosition(mPosition + mFacing * mOutputs[OUTPUT_SPEED] * mMaxSpeed * deltaSeconds);
		mPosition.x = SimMath::clampNum(mPosition.x, 0.0f, bounds.x);
		mPosition.y = SimMath::clampNum(mPosition.y, 0.0f, bounds.y);

		// decay energy according to amount moved and size
		mInputs[INPUT_ENERGY] -= glm::abs(mOutputs[OUTPUT_SPEED]) / mMaxSpeed * mSize * MOVE_COEF;
		if(mInputs[INPUT_ENERGY] < 0.0f)
		{
			mInputs[INPUT_ENERGY] = 0.0f;
		}
	}
}

//-------------------------------------------------------------

void Agent::render(Renderer& renderer)
{
	if(mIsAlive)
	{
#if DRAWS_VISION_CONES
		float angle = PERCEPTION_HALF_SUBDIVS * PERCEPTION_SUBDIV_WIDTH;
		for(float i = angle; i >= -angle - 0.01f; i -= PERCEPTION_SUBDIV_WIDTH)
		{
			vec3 v = vec3(mFacing.x * std::cos(i) + mFacing.y * std::sin(i), -mFacing.x * std::sin(i) + mFacing.y * std::cos(i), 0);
			v = glm::normalize(v);

			renderer.drawRay(mPosition + v * mSize, v, 1000.0f, vec3(1, 0, 0));
		}
#endif

		renderer.drawColoredCircle(mPosition, mSize, mColor);
		renderer.drawRay(mPosition, mFacing, mSize, vec3(0, 0, 0));
	}
}

//-------------------------------------------------------------

void Agent::drawNetwork(Renderer& renderer)
{
	assert(mBrain != nullptr);
	mBrain->render(renderer);
}

//-------------------------------------------------------------

void Agent::updateHunger(float amount)
{
	mInputs[INPUT_HUNGER] += amount;
	if(mInputs[INPUT_HUNGER] > MAX_HUNGER)
	{
		mInputs[INPUT_HUNGER] = MAX_HUNGER;
	}
}

//-------------------------------------------------------------

void Agent::updateThirst(float amount)
{
	mInputs[INPUT_THIRST] += amount;
	if(mInputs[INPUT_THIRST] > MAX_THIRST)
	{
		mInputs[INPUT_THIRST] = MAX_THIRST;
	}
}

//-------------------------------------------------------------

void Agent::kill()
{
	deactivateCollision();
	mIsAlive = false;

	AgentDeath args;
	args.agent = this;
	EventQueue::enqueue(*new Event<AgentDeath>(args));
}

//-------------------------------------------------------------

void Agent::select()
{
	mIsSelected = true;
	mFlashingTimer = 0.0f;
	mColor = SELECTED_COLOR;
}

//-------------------------------------------------------------

void Agent::deselect()
{
	mIsSelected = false;
	mColor = mUnselectedColor;
}

//-------------------------------------------------------------

void Agent::setFacing(const glm::vec3& facing)
{
	mFacing = facing;
	mFacing = normalize(mFacing);
}

//-------------------------------------------------------------

void Agent::setGenome(Genome& genome)
{
	// takes possession of incoming genome
	delete mDNA;
	mDNA = &genome;
}

//-------------------------------------------------------------

const glm::vec3& Agent::getFacing() const
{
	return mFacing;
}

//-------------------------------------------------------------

const NeuralNetwork& Agent::getNetwork() const
{
	assert(mBrain != nullptr);
	return *mBrain;
}

//-------------------------------------------------------------

const Genome& Agent::getGenome() const
{
	assert(mDNA != nullptr);
	return *mDNA;
}

//-------------------------------------------------------------

float Agent::getInputValue(std::uint32_t index) const
{
	assert(mBrain != nullptr);
	assert(index < mBrain->getNumInputs());
	return mInputs[index];
}

//-------------------------------------------------------------

float Agent::getOutputValue(std::uint32_t index) const
{
	assert(index < NETWORK_NUM_OUT);
	return mOutputs[index];
}

//-------------------------------------------------------------

float Agent::getFitness() const
{
	return
		FITNESS_WEIGHT_TIME * (mTimeAlive / MAX_LIFETIME) +
		FITNESS_WEIGHT_HEALTH * (
			FITNESS_WEIGHT_HUNGER * mInputs[INPUT_HUNGER] +
			FITNESS_WEIGHT_THIRST * mInputs[INPUT_THIRST] +
			FITNESS_WEIGHT_ENERGY * mInputs[INPUT_ENERGY]);
}

//-------------------------------------------------------------

bool Agent::isAlive() const
{
	return mIsAlive;
}

//-------------------------------------------------------------

bool Agent::isSelected() const
{
	return mIsSelected;
}
