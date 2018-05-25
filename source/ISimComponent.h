
#pragma once

#include "SimClock.h"
#include "Renderer.h"

#include "RTTI.h"

namespace Ecosim
{
	/**	An object in the simulation that can
	 *	be updated and rendered.
	 */
	class ISimComponent abstract : public RTTI
	{
		RTTI_DECLARATIONS(ISimComponent, RTTI)

	public:

		ISimComponent(const ISimComponent& other) = delete;
		ISimComponent& operator=(const ISimComponent& other) = delete;
		ISimComponent(ISimComponent&& other) = delete;
		ISimComponent& operator=(ISimComponent&& other) = delete;

		/**	@brief Constructor.
		 */
		ISimComponent() = default;

		/**	@brief Destructor function.
		 */
		virtual ~ISimComponent() = default;

		/**	@brief Initializer function.
		 */
		virtual void init();

		/**	@brief Shutdown function.
		 */
		virtual void shutdown();

		/**	@brief Update function.
		 *
		 *	@param clock The simulation's timekeeper.
		 */
		virtual void update(const SimClock& clock);

		/**	@brief Render function.
		 *
		 *	@param renderer The simulation rendering object.
		 */
		virtual void render(Renderer& renderer);
	};
}
