
#pragma once

#include <chrono>

namespace Ecosim
{
	/**	Timekeeping class for the simulation.
	 */
	class SimClock final
	{
	public:

		SimClock(const SimClock& other) = delete;
		SimClock& operator=(const SimClock& other) = delete;
		SimClock(SimClock&& other) = delete;
		SimClock& operator=(SimClock&& other) = delete;

		/** @brief Constructor.
		 *
		 *	@param maxTimeScale The maximum allowable timescale.
		 */
		explicit SimClock(uint32_t maxTimeScale = MAX_TIMESCALE);

		/**	@brief Destructor.
		 */
		~SimClock() = default;

		/** @brief Resets the clock back to its starting state.
		 */
		void reset();

		/**	@brief Updates the clock's current times and delta
		 *		   times.
		 */
		void update();

		/** @brief Increases the speed of the clock, up to
		 *		   the maximum time scale.
		 */
		void incrementTimeScale();

		/**	@brief Decreases the speed of the clock, down to
		 *		   a minimum of 1.
		 */
		void decrementTimeScale();

		/** @brief Gets the starting time of the clock.
		 *
		 *	@return Returns mStartTime.
		 */
		const std::chrono::high_resolution_clock::time_point& getStartTime() const;

		/**	@brief Gets the total time, in seconds, that
		 *		   the clock has been running.
		 *
		 *	@return Returns mTotalTime.
		 */
		float getTotalTime() const;

		/**	@brief Gets the raw duration, in seconds, of
		 *		   the current frame.
		 *
		 *	@return Returns mDeltaTime.
		 */
		float getDeltaTime() const;

		/**	@brief Gets the duration of the current frame,
		 *		   adjusted according to the timescale.
		 *
		 *	@return Returns mDeltaTimeScaled.
		 */
		float getDeltaTimeScaled() const;

		/**	@brief Gets the current timescale.
		 *
		 *	@return Returns mTimeScale.
		 */
		uint32_t getTimeScale() const;

	private:

		std::chrono::high_resolution_clock::time_point mStartTime;
		std::chrono::high_resolution_clock::time_point mCurrTime;
		std::chrono::high_resolution_clock::time_point mLastTime;

		float mTotalTime;
		float mDeltaTime;
		float mDeltaTimeScaled;

		uint32_t mMaxTimeScale;
		uint32_t mTimeScale;
	};
}
