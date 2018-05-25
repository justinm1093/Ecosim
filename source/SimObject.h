
#pragma once

#include "ISimComponent.h"

namespace Ecosim
{
	/**	A simulation component that has position.
	 */
	class SimObject abstract : public ISimComponent
	{
		RTTI_DECLARATIONS(SimObject, ISimComponent)

	public:

		SimObject(const SimObject& other) = delete;
		SimObject& operator=(const SimObject& other) = delete;
		SimObject(SimObject&& other) = delete;
		SimObject& operator=(SimObject&& other) = delete;

		/**	@brief Constructor.
		 *
		 *	@param pos The starting position.
		 */
		explicit SimObject(const glm::vec3& pos = glm::vec3(0, 0, 0));

		/**	@brief Destructor.
		 */
		virtual ~SimObject() = default;

		/**	@brief Sets this object's position.
		 *
		 *	@param newPos The new position.
		 */
		void setPosition(const glm::vec3& newPos);

		/**	@brief Gets this object's position.
		 *
		 *	@return Returns mPosition.
		 */
		const glm::vec3& getPosition() const;

	protected:
		
		glm::vec3 mPosition;
	};
}
