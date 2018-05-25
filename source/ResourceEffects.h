
#pragma once

#include "IResourceEffect.h"

namespace Ecosim
{
	/**	ResourceEffect that applies a modifier
	 *	to an Agent's hunger.
	 */
	class Food final : public IResourceEffect
	{
		RTTI_DECLARATIONS(Food, IResourceEffect)

	public:

		Food(const Food& other) = delete;
		Food& operator=(const Food& other) = delete;
		Food(Food&& other) = delete;
		Food& operator=(Food&& other) = delete;

		/**	@brief Constructor.
		 */
		Food() = default;

		/**	@brief Destructor.
		 */
		virtual ~Food() = default;

		/**	@brief Applies a modifier to the Agent's hunger.
		 *
		 *	@param agent The colliding Agent.
		 */
		virtual void applyEffect(Agent& agent) override;

		/**	@brief Sets the effect's hunger yield.
		 *
		 *	@param hunger The new effect yield.
		 */
		static void setYields(float hunger);

		/**	@brief Gets the effect yield.
		 *
		 *	@return hunger Returns sHungerYield
		 */
		static float getHungerYield();

	private:

		static float sHungerYield;
	};

	//=============================================================

	/**	ResourceEffect that applies a modifier
	 *	to an Agent's thirst.
	 */
	class Water final : public IResourceEffect
	{
		RTTI_DECLARATIONS(Water, IResourceEffect)

	public:

		Water(const Water& other) = delete;
		Water& operator=(const Water& other) = delete;
		Water(Water&& other) = delete;
		Water& operator=(Water&& other) = delete;

		/**	@brief Constructor.
		 */
		Water() = default;

		/**	@brief Destructor.
		 */
		virtual ~Water() = default;

		/**	@brief Applies a modifier to the Agent's thirst.
		 *
		 *	@param agent The colliding Agent.
		 */
		virtual void applyEffect(Agent& agent) override;

		/**	@brief Sets the effect's thirst yield.
		 *
		 *	@param thirst The new effect yield.
		 */
		static void setYields(float thirst);

		/**	@brief Gets the effect yield.
		 *
		 *	@return hunger Returns sThirstYield
		 */
		static float getThirstYield();

	private:

		static float sThirstYield;
	};
}
