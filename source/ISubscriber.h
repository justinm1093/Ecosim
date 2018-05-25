
#pragma once

namespace Ecosim
{
	class IPublisher;

	/**	Abstract class that encapsulates the ability
	 *	to receive Events.
	 *
	 *	ISubscribers may subscribe and unsubscribe to
	 *	Events.
	 */
	class ISubscriber
	{
	public:

		/**	@brief Receives a delivered event.
		 *	
		 *	@param e The event object being handled.
		 */
		virtual void notify(const IPublisher& e) = 0;
	};
}
