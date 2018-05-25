
#pragma once

#include "PhysicalCircle.h"

#include "Genome.h"
#include "NeuralNetwork.h"

namespace Ecosim
{
	/** A movable object that has a Genome and NeuralNetwork.
	 *	The NeuralNetwork drives the Agent's steering behavior.
	 *	The Genome evolves over time based on the fitness of
	 *	its Agent at the time of death.
	 */
	class Agent : public PhysicalCircle
	{
		RTTI_DECLARATIONS(Agent, PhysicalCircle)

	public:

		Agent(const Agent& other) = delete;
		Agent& operator=(const Agent& other) = delete;
		Agent(Agent&& other) = delete;
		Agent& operator=(Agent&& other) = delete;

		/**	@brief Constructor.
		 *
		 *	@param numInputs The number of inputs in the NeuralNetwork.
		 *	@param color The default color of the Agent.
		 *	@param pos The starting position.
		 *	@param facing The starting direction the Agent faces.
		 */
		Agent(std::uint32_t numInputs = AGENT_NUM_INPUTS, const glm::vec3& color = AGENT_COLOR, const glm::vec3& pos = glm::vec3(0, 0, 0), const glm::vec3& facing = glm::vec3(0, 1, 0));

		/**	@brief Destructor.
		 */
		virtual ~Agent();

		/**	@brief Resets an Agent's stats and variables for a
		 *		   new life. Initializes the topology of the
		 *		   NeuralNetwork using the Genome.
		 */
		void activate();

		/**	@brief Updates the Agent. Updates status variables,
		 *		   calculates network output, updates position
		 *		   and rotation.
		 *
		 *	@param clock The simulation's timekeeper.
		 */
		virtual void update(const SimClock& clock) override;

		/** @brief Renders the Agent.
		 *
		 *	@param renderer The simulation rendering object.
		 */
		virtual void render(Renderer& renderer) override;

		/**	@brief Renders the Agent's NeuralNetwork.
		 *
		 *	@param renderer The simulation rendering object.
		 */
		void drawNetwork(Renderer& renderer);

		/**	@brief Increases the "fullness" of the Agent.
		 *
		 *	@param amount The incoming change to hunger.
		 */
		void updateHunger(float amount);

		/**	@brief Increases the "hydration" of the Agent.
		 *
		 *	@param amount The incoming change to thirst.
		 */
		void updateThirst(float amount);

		/** @brief Kills the Agent. Deactivates collision and
		 *		   sends a signal notifying the AgentManager.
		 */
		void kill();

		/** @brief Selects the Agent. Starts the Agent flashing.
		 */
		void select();

		/** @brief Deselects the Agent. Stops the Agent flashing.
		 */
		void deselect();

		/** @brief Sets the normalized rotation vector on the Agent.
		 *
		 *	@param facing The unnormalized rotation vector.
		 */
		void setFacing(const glm::vec3& facing);

		/**	@brief Takes possession of a new Genome.
		 *
		 *	@param genome The new Genome.
		 *
		 *	@note The old Genome is destroyed. The Agent takes
		 *		  possession of the new Genome.
		 */
		void setGenome(Genome& genome);

		/** @brief Gets the normalized rotation vector.
		 *
		 *	@return Returns mFacing.
		 */
		const glm::vec3& getFacing() const;

		/** @brief Gets the Agent's NeuralNetwork.
		 *
		 *	@return Returns a reference to mBrain.
		 */
		const NeuralNetwork& getNetwork() const;

		/** @brief Gets the Agent's Genome.
		 *
		 *	@return Returns a reference to mDNA.
		 */
		const Genome& getGenome() const;

		/** @brief Gets a value from the Agent's inputs.
		 *
		 *	@param index The offset into the input array.
		 *
		 *	@return Returns the value at the indexed
		 *			offset in mInputs.
		 *
		 *	@note The given index must be in bounds for
		 *		  the mInputs array.
		 */
		float getInputValue(std::uint32_t index) const;

		/** @brief Gets a value from the Agent's outputs.
		 *
		 *	@param index The offset into the output array.
		 *
		 *	@return Returns the value at the indexed
		 *			offset in mOutputs.
		 *
		 *	@note The given index must be in bounds for
		 *		  the mOutputs array.
		 */
		float getOutputValue(std::uint32_t index) const;

		/** @brief Gets the Agent's fitness.
		 *
		 *	@return Returns the Agent's fitness, which is
		 *			a weighted average of the Agent's age
		 *			and health (which is a weighted average
		 *			of the Agent's hunger, thirst, and
		 *			energy).
		 */
		float getFitness() const;

		/**	 @brief Says whether the Agent is alive.
		 *
		 *	@return Returns true if the Agent is alive.
		 *			Otherwise, false.
		 */
		bool isAlive() const;

		/**	 @brief Says whether the Agent is selected.
		 *
		 *	@return Returns true if the Agent is selected.
		 *			Otherwise, false.
		 */
		bool isSelected() const;

	protected:

		/**	@brief Calls to update individual blocks of
		 *		   perception input for several types.
		 *
		 *	@note In order to easily handle different perception
		 *		  behaviors for different Agent types, this
		 *		  virtual function allows us to easily request
		 *		  updates to perception data for specific types
		 *		  and at specific blocks.
		 */
		virtual void updatePerceptionInputs();

		/**	@brief Calls to detect and handle collisions
		 *		   with several types.
		 *
		 *	@note In order to easily handle different collision
		 *		  behaviors for different Agent types, this
		 *		  virtual function allows us to easily request
		 *		  updates to collision for specific types and at 
		 *		  specific blocks.
		 */
		virtual void updateCollisions();

		/** @brief Updates a specific block of perception input
		 *		   for a given type.
		 *
		 *	@param typeID The type ID of a PhysicalCircle class
		 *				  that this Agent can see.
		 *	@param perceptionBlockID The ID of the block of
		 *							 perception data we are updating.
		 */
		void updatePerceptionBlock(std::uint64_t typeID, std::uint32_t perceptionBlockID);

		/**	@brief Updates collisions for a given type.
		 *
		 *	@param typeID The type ID of a PhysicalCircle class
		 *				  that this Agent can collide with.
		 */
		void detectCollisions(std::uint64_t typeID);

	private:

		/**	@brief Updates the hunger, thirst, and energy inputs
		 *		   that are fed to the NeuralNetwork. The Agent
		 *		   dies if any of these hit 0.
		 *
		 *	@param deltaSeconds The scaled delta time for this frame.
		 */
		void updateStatusInputs(float deltaSeconds);

		/**	@brief Updates position and rotation based on the outputs
		 *		   received from the NeuralNetwork.
		 *
		 *	@param deltaSeconds The scaled delta time for this frame.
		 */
		void applyMovement(float deltaSeconds);

	protected:

		std::uint32_t mNumPerceptionBlocks;
		std::uint32_t INPUT_HUNGER;
		std::uint32_t INPUT_THIRST;
		std::uint32_t INPUT_ENERGY;

	private:

		glm::vec3 mFacing;
		glm::vec3 mColor;
		glm::vec3 mUnselectedColor;

		NeuralNetwork* mBrain;
		Genome* mDNA;

		float* mInputs;
		float* mOutputs;

		float mTimeAlive;
		float mFlashingTimer;

		float mSize;
		float mMaxSpeed;
		float mMaxSteer;
		float mInvHungerRate;
		float mInvThirstRate;

		bool mIsAlive;
		bool mIsSelected;
	};
}
