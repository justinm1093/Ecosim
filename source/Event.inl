
template <typename T>
Event<T>::Event(const T& msg) :
	IPublisher(sSubscribers),
	mMsg(msg)
{
}

//-------------------------------------------------------------

template <typename T>
void Event<T>::subscribe(ISubscriber& sub)
{
	sSubscribers.push_back(&sub);
}

//-------------------------------------------------------------

template <typename T>
void Event<T>::unsubscribe(ISubscriber& sub)
{
	sSubscribers.erase(std::remove(sSubscribers.begin(), sSubscribers.end(), &sub), sSubscribers.end());
}

//-------------------------------------------------------------

template <typename T>
void Event<T>::unsubscribeAll()
{
	sSubscribers.clear();
}

//-------------------------------------------------------------

template <typename T>
std::uint32_t Event<T>::numSubscribers()
{
	return static_cast<uint32_t>(sSubscribers.size());
}

//-------------------------------------------------------------

template <typename T>
const T& Event<T>::message() const
{
	return mMsg;
}
