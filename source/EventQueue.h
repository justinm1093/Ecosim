
#pragma once

#include "IPublisher.h"

namespace Ecosim
{
	/**	Singleton class that manages a queue of Events.
	 */
	class EventQueue final
	{
	public:

		EventQueue(const EventQueue& other) = delete;
		EventQueue& operator=(const EventQueue& other) = delete;
		EventQueue(EventQueue&& other) = delete;
		EventQueue& operator=(EventQueue&& other) = delete;

		/** @brief Destructor.
		 */
		~EventQueue() = default;

		/** @brief Delivers all events in the queue.
		 */
		void update();

		/**	@brief Adds a new event to the queue.
		 *
		 *	@param publisher The new event.
		 */
		static void enqueue(IPublisher& publisher);

		/**	@brief Clears the queue without delivering
		 *		   any of the enqueued events.
		 */
		static void clear();

		/**	@brief Gets the singleton instance of the
		 *		   EventQueue.
		 *
		 *	@return Returns a pointer to the EventQueue
		 *			singleton.
		 */
		static EventQueue* instance();

	private:

		/** @brief Constructor.
		 */
		EventQueue();

		typedef std::vector<std::shared_ptr<IPublisher>> Events;
		Events mEvents;

		static EventQueue* sInstance;
	};
}
