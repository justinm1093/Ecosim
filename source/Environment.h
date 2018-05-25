
#pragma once

#include "ISubscriber.h"

#include "Resource.h"
#include "ResourceEffects.h"

namespace Ecosim
{
	/**	Simulation component that manages pools
	 *	of Resource objects.
	 *	
	 *	These pools are sorted in maps by type,
	 *	so Resource<Food>'s are kept separate
	 *	from Resource<Water>'s.
	 */
	class Environment final : public ISimComponent, public ISubscriber
	{
	public:

		Environment(const Environment& other) = delete;
		Environment& operator=(const Environment& other) = delete;
		Environment(Environment&& other) = delete;
		Environment& operator=(Environment&& other) = delete;

		/**	@brief Constructor.
		 */
		Environment();

		/**	@brief Destructor.
		 */
		~Environment();

		/**	@brief Initializes the Resource pools
		 *		   and Resource stats.
		 */
		virtual void init() override;

		/**	@brief Deletes all Resources.
		 */
		virtual void shutdown() override;

		/**	@brief Spawns Resources on a timer and 
		 *		   updates Resources in the active pool.
		 *
		 *	@param clock The simulation's timekeeper.
		 */
		virtual void update(const SimClock& clock) override;

		/**	@brief Renders all Resources in the active pool.
		 *
		 *	@param renderer The simulation rendering object.
		 */
		virtual void render(Renderer& renderer) override;

		/**	@brief Receives signals of Resource deactivation.
		 *		   Moves the deactivated Resource from the
		 *		   active pool to the inactive pool.
		 *
		 *	@param e The event object being handled.
		 */
		virtual void notify(const IPublisher& e) override;

	private:

		/**	@brief Moves the given Resource from the
		 *		   active pool to the inactive pool.
		 *
		 *	@param resource The deactivated Resource.
		 */
		void moveResource(IResource& resource);

		/**	@brief Moves a Resource<Food> from the inactive
		 *		   pool to the active pool. Creates a new
		 *		   Resource<Food> in the active pool if the
		 *		   inactive pool is empty.
		 */
		void spawnFood();

		/**	@brief Moves a Resource<Water> from the inactive
		 *		   pool to the active pool. Creates a new
		 *		   Resource<Water> in the active pool if the
		 *		   inactive pool is empty.
		 */
		void spawnWater();


		typedef std::function<void(Environment&)> SpawnFunc;
		typedef std::vector<IResource*> ResourcePool;

		std::unordered_map<std::uint64_t, SpawnFunc> mResourceSpawnFuncs;	
		std::unordered_map<std::uint64_t, ResourcePool> mInactivePools;
		std::unordered_map<std::uint64_t, ResourcePool> mActivePools;
		std::unordered_map<std::uint64_t, float> mResourceSpawnTimes;
		std::unordered_map<std::uint64_t, float> mResourceSpawnRates;
		
		float mTime;
	};
}
