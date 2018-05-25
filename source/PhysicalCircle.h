
#pragma once

#include "SimObject.h"

namespace Ecosim
{
	/**	A SimObject that can collide with other
	 *	things that derive from PhysicalCircle.
	 *
	 *	PhysicalCircles have access to other objects
	 *	through lists that are mapped to different
	 *	type IDs.
	 *
	 *	On creation, PhysicalCircles should register 
	 *	themselves with their lists. They unregister
	 *	themselves on destruction. PhysicalCircles
	 *	may be kept in multiple lists, because they
	 *	are registered along their inheritance chain.
	 */
	class PhysicalCircle abstract : public SimObject
	{
		RTTI_DECLARATIONS(PhysicalCircle, SimObject)

	public:

		PhysicalCircle(const PhysicalCircle& other) = delete;
		PhysicalCircle& operator=(const PhysicalCircle& other) = delete;
		PhysicalCircle(PhysicalCircle&& other) = delete;
		PhysicalCircle& operator=(PhysicalCircle&& other) = delete;

		/**	@brief Constructor.
		 *	
		 *	@param radius The collision radius of this object.
		 *	@param pos The starting position.
		 *	@param isCollisionActive Says whether this object starts
		 *							 with active collision.
		 */
		PhysicalCircle(float radius, const glm::vec3& pos = glm::vec3(0, 0, 0), bool isCollisionActive = false);

		/** @brief Destructor.
		 */
		virtual ~PhysicalCircle();

		/** @brief Handles collisions with another PhysicalCircle.
		 *
		 *	@param other The colliding PhysicalCircle.
		 */
		virtual void handleCollision(PhysicalCircle& other);

		/** @brief Activates the ability of this object to collide
		 *		   with other PhysicalCircles.
		 */
		void activateCollision();

		/** @brief Deactivates the ability of this object to collide
		 *		   with other PhysicalCircles.
		 */
		void deactivateCollision();

		/**	@brief Sets the collision radius of this object.
		 *	
		 *	@param radius The new size.
		 */
		void setRadius(float radius);

		/**	@brief Gets the collision radius of this object.
		 *
		 *	@return Returns mRadius.
		 */
		float getRadius() const;

		/**	@brief Says whether this object has active collision.
		 *
		 *	@return Returns true if this object can collide.
		 *			Otherwise, false.
		 */
		bool isCollisionActive() const;

		/**	@brief Sets the physical bounds and maximum distance
		 *		   between PhysicalCircles.
		 *
		 *	@param bounds The new bounds.
		 */
		static void setBounds(const glm::vec2& bounds);

		/**	@brief Retrieves the list of PhysicalCircle objects
		 *		   registered with a given type ID.
		 *
		 *	@param typeID The typeID for PhysicalCircles we are requesting.
		 *
		 *	@return Returns a reference to the list of PhysicalCircles with
		 *			the given type ID.
		 */
		static const std::vector<PhysicalCircle*>& getCollisionObjList(std::uint64_t typeID);

		/**	@brief Gets the physical bounds for PhysicalCircles.
		 *
		 *	@return Returns sBounds.
		 */
		static const glm::vec2& getBounds();

		/**	@brief Gets the maximum distance that can be between
		 *		   PhysicalCircles.
		 *
		 *	@return Returns sMaxDistance.
		 */
		static float getMaxDistance();

	protected:

		/**	@brief Adds an object to the collision list mapped to 
		 *		   the object's type ID.
		 *
		 *	@param obj The new object.
		 */
		static void registerObj(PhysicalCircle& obj);

		/**	@brief Removes an object from the collision list mapped
		 *		   to the object's type ID.
		 *
		 *	@param obj The removed object.
		 */
		static void unregisterObj(PhysicalCircle& obj);

	private:

		float mRadius;
		bool mIsCollisionActive;

		static std::map<std::uint64_t, std::vector<PhysicalCircle*>> sCollisionObjectLists;

		static glm::vec2 sBounds;
		static float sMaxDistance;
	};
}
