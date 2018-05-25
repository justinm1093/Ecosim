
#pragma once

#include "Agent.h"

namespace Ecosim
{
	/**	Agent class that can see Resource<Water> and Prey,
	 *	and can collide with Resource<Water> and Agents.
	 *
	 *	Predators automatically have 19 network inputs.
	 */
	class Predator final : public Agent
	{
		RTTI_DECLARATIONS(Predator, Agent)

	public:

		Predator(const Predator& other) = delete;
		Predator& operator=(const Predator& other) = delete;
		Predator(Predator&& other) = delete;
		Predator& operator=(Predator&& other) = delete;

		/**	@brief Constructor.
		 */
		Predator();

		/**	@brief Destructor.
		 */
		virtual ~Predator();

		/** @brief Eats Prey with which this object collides.
		 *
		 *	@param other The PhysicalCircle with which we are
		 *				 colliding.
		 */
		virtual void handleCollision(PhysicalCircle& other) override;

	protected:

		/**	@brief Updates perception data for Resource<Water>
		 *		   and Prey.
		 */
		virtual void updatePerceptionInputs() override;

		/**	@brief Detects and handles collisions with 
		 *		   Resource<Water> and Agent.
		 */
		virtual void updateCollisions() override;
	};
}
