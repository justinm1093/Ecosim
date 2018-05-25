
#pragma once

#include "ISubscriber.h"
#include "RTTI.h"

namespace Ecosim
{
	/** Abstract class used for the publisher-subscriber
	 *	pattern of event delivery.
	 *
	 *	The EventQueue manages a queue of objects of this
	 *	type so that events can be delivered agnostic of
	 *	their type or arguments.
	 */
	class IPublisher abstract : public RTTI
	{
		RTTI_DECLARATIONS(IPublisher, RTTI)

	protected:

		typedef std::vector<ISubscriber*> Subscribers;

	public:

		IPublisher(const IPublisher& other) = delete;
		IPublisher& operator=(const IPublisher& other) = delete;
		IPublisher(IPublisher&& other) = delete;
		IPublisher& operator=(IPublisher&& other) = delete;

		/** @brief Constructor.
		 *
		 *	@param subscribers The list of subscribers for
		 *		   the Event<T> that this object actually is.
		 */
		explicit IPublisher(Subscribers& subscribers);

		/**	@brief Destructor.
		 */
		virtual ~IPublisher() = default;

		/**	@brief Delivers this object to all subscribers.
		 */
		void deliver();

		/** @brief Gets the number of objects registered
		 *		   as subscribers for this Event type.
		 *
		 *	@return Returns the length of mSubscribers.
		 */
		std::uint32_t numSubscribers() const;

	protected:

		Subscribers* mSubscribers;
	};
}
