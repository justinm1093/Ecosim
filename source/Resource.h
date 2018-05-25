
#pragma once

#include "IResource.h"
#include "Agent.h"

namespace Ecosim
{
	/**	Templated Resource class that applies
	 *	an effect to an Agent when it collides
	 *	with one.
	 *
	 *	The effect is defined by the template
	 *	argument, which is assumed to inherit
	 *	IResourceEffect.
	 */
	template <typename T>
	class Resource final : public IResource
	{
		RTTI_DECLARATIONS(Resource<T>, IResource)

	public:

		Resource(const Resource& other) = delete;
		Resource& operator=(const Resource& other) = delete;
		Resource(Resource&& other) = delete;
		Resource& operator=(Resource&& other) = delete;

		/**	@brief Constructor.
		 */
		Resource();

		/**	@brief Destructor.
		 */
		virtual ~Resource();

		/**	@brief Initializes the traits that all objects
		 *		   of this Resource type share.
		 *
		 *	@param color The color of this Resource type.
		 *	@param lifetime The maximum age of this Resource type.
		 *	@param size The collision size of this Resource type.
		 *
		 *	@note The Resource type must be initialized before
		 *		  any objects of that type may be created.
		 */
		static void init(const glm::vec3& color, float lifetime, float size);

		/** @brief Udpates the age of this object. Deactivates
		 *		   if the maximum age is exceeded.
		 *
		 *	@param clock The simulation's timekeeper.
		 */
		virtual void update(const SimClock& clock) override;

		/** @brief Renders the Resource.
		 *
		 *	@param renderer The simulation rendering object.
		 */
		virtual void render(Renderer& renderer) override;

	protected:

		/**	@brief Applies the Resource's effect to an Agent.
		 *
		 *	@param other The PhysicalCircle with which we are
		 *				 colliding.
		 */
		virtual void handleCollision(PhysicalCircle& other) override;

	private:
		
		T mEffect;

		static glm::vec3 sColor;
		static float sSize;

		static float sLifetime;
	};

	template <typename T> RTTI_DEFINITIONS(Resource<T>)

	template <typename T> glm::vec3 Resource<T>::sColor = vec3(1.0f, 1.0f, 1.0f);
	template <typename T> float Resource<T>::sLifetime = 10.0f;
	template <typename T> float Resource<T>::sSize = 10.0f;

#include "resource.inl"
}
