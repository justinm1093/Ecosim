
#pragma once

#include "IPublisher.h"

namespace Ecosim
{
	/**	Templated Event class that stores its
	 *	arguments as the templated member.
	 *
	 *	Normally, the templated type is an EventArgs,
	 *	but it could be anything--int, Foo, etc.
	 *
	 *	Events use the publisher-subscriber pattern,
	 *	so Event<T>'s store lists of subscribers that
	 *	receive events of that type when delivered.
	 *
	 *	These lists are unique to templates, so an
	 *	Event<int>'s subscribers will not receive
	 *	Event<float>'s unless they also subscribe.
	 */
	template <typename T>
	class Event final : public IPublisher
	{
		RTTI_DECLARATIONS(Event<T>, IPublisher)

	public:

		Event(const Event& other) = delete;
		Event& operator=(const Event& other) = delete;
		Event(Event&& other) = delete;
		Event& operator=(Event&& other) = delete;

		/** @brief Constructor.
		 *
		 *	@param msg The event argument payload.
		 */
		explicit Event(const T& msg);

		/**	@brief Destructor.
		 */
		virtual ~Event() = default;

		/**	@brief Registers an object as a subscriber
		 *		   to this Event type.
		 *
		 *	@param sub Subscriber object.
		 */
		static void subscribe(ISubscriber& sub);

		/**	@brief Unregisters an object as a subscriber
		 *		   to this Event type.
		 *
		 *	@param sub Subscriber object.
		 */
		static void unsubscribe(ISubscriber& sub);

		/**	@brief Unregisters all subscriber objects
		 *		   for this Event type.
		 */
		static void unsubscribeAll();

		/** @brief Gets the number of objects registered
		 *		   as subscribers for this Event type.
		 *
		 *	@return Returns the length of sSubscribers.
		 */
		static std::uint32_t numSubscribers();

		/**	@brief Gets the argument payload.
		 *
		 *	@return Returns a reference to mMsg;
		 */
		const T& message() const;

	private:

		T mMsg;

		static Subscribers sSubscribers;
	};

	template <typename T> RTTI_DEFINITIONS(Event<T>)
	template <typename T> IPublisher::Subscribers Event<T>::sSubscribers;

#include "Event.inl"
}
