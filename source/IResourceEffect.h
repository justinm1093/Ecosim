
#pragma once

#include "pch.h"

#include "Agent.h"
#include "RTTI.h"

namespace Ecosim
{
	/**	Abstract payload for a Resource.
	 *	Encapsulates the ability for a Resource's
	 *	templated argument to apply some effect
	 *	to a colliding Agent.
	 */
	class IResourceEffect abstract : public RTTI
	{
		RTTI_DECLARATIONS(IResourceEffect, RTTI)

	public:

		IResourceEffect(const IResourceEffect& other) = delete;
		IResourceEffect& operator=(const IResourceEffect& other) = delete;
		IResourceEffect(IResourceEffect&& other) = delete;
		IResourceEffect& operator=(IResourceEffect&& other) = delete;

		/**	@brief Constructor.
		 */
		IResourceEffect() = default;

		/**	@brief Destructor.
		 */
		virtual ~IResourceEffect() = default;

		/**	@brief Applies some effect to an Agent.
		 *
		 *	@param agent The colliding Agent.
		 */
		virtual void applyEffect(Agent& agent) = 0;
	};
}
