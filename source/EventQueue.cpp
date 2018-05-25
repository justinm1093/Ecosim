
#include "pch.h"
#include "EventQueue.h"

using namespace Ecosim;
using namespace std;

EventQueue* EventQueue::sInstance = nullptr;

EventQueue* EventQueue::instance()
{
	if(sInstance == nullptr)
	{
		sInstance = new EventQueue();
	}
	return sInstance;
}

//-------------------------------------------------------------

EventQueue::EventQueue()
{
}

//-------------------------------------------------------------

void EventQueue::update()
{
	// don't want to trash event queue -- move contents to other queue
	Events events = std::move(mEvents);
	for(auto& e : events)
	{
		e->deliver();
	}

	events.clear();
}

//-------------------------------------------------------------

void EventQueue::enqueue(IPublisher& publisher)
{
	assert(sInstance != nullptr);
	sInstance->mEvents.push_back(shared_ptr<IPublisher>(&publisher));
}

//-------------------------------------------------------------

void EventQueue::clear()
{
	assert(sInstance != nullptr);
	sInstance->mEvents.clear();
}
