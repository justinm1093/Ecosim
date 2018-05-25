
#pragma once

#include "Agent.h"

namespace Ecosim
{
	/**	Agent class that can see Resource<Food>, 
	 *	Resource<Water>, and Predators, and can 
	 *	collide with Resource<Food>, Resource<Water>,
	 *	and Agents.
	 *
	 *	Prey automatically have 27 network inputs.
	 */
	class Prey final : public Agent
	{
		RTTI_DECLARATIONS(Prey, Agent)

	public:

		Prey(const Prey& other) = delete;
		Prey& operator=(const Prey& other) = delete;
		Prey(Prey&& other) = delete;
		Prey& operator=(Prey&& other) = delete;

		/**	@brief Constructor.
		 */
		Prey();

		/**	@brief Destructor.
		 */
		virtual ~Prey();

		/** @brief Is eaten by Predators with which this object collides.
		 *
		 *	@param other The PhysicalCircle with which we are
		 *				 colliding.
		 */
		virtual void handleCollision(PhysicalCircle& other) override;

	protected:

		/**	@brief Updates perception data for Resource<Food>, 
		 *		   Resource<Water>, and Predators.
		 */
		virtual void updatePerceptionInputs() override;

		/**	@brief Detects and handles collisions with 
		 *		   Resource<Food>, Resource<Water>, and Agent.
		 */
		virtual void updateCollisions() override;
	};
}
