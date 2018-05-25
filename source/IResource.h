
#pragma once

#include "PhysicalCircle.h"

namespace Ecosim
{
	/**	Resource class managed by Environment.
	 *	Handles a Resource's ability to be activated,
	 *	deactivated, and track a duration.
	 */
	class IResource abstract : public PhysicalCircle
	{
		RTTI_DECLARATIONS(IResource, PhysicalCircle)

	public:

		IResource(const IResource& other) = delete;
		IResource& operator=(const IResource& other) = delete;
		IResource(IResource&& other) = delete;
		IResource& operator=(IResource&& other) = delete;

		/** @brief Constructor.
		 *
		 *	@param size The collision size.
		 *	@param pos The starting position.
		 */
		IResource(float size, const glm::vec3& pos = glm::vec3(0, 0, 0));

		/**	@brief Destructor.
		 */
		virtual ~IResource();

		/**	@brief Resets this object's age and
		 *		   collision. Gives it a new position.
		 */
		void activate();

		/**	@brief Deactivates this object's collision.
		 *		   Sends a signal notifying the Environment.
		 */
		void deactivate();

		/** @brief Gets the length of time this object
		 *		   has been active.
		 *
		 *	@return Returns mTimeActive.
		 */
		float getTimeActive() const;

		/**	@brief Says whether this object is active.
		 *
		 *	@return Returns true if this object is active.
		 *			Otherwise, false.
		 */
		bool isActive() const;

	protected:

		float mTimeActive;
		bool mIsActive;
	};
}
