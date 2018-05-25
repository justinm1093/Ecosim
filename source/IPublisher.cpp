
#include "pch.h"
#include "IPublisher.h"

using namespace Ecosim;
using namespace std;

RTTI_DEFINITIONS(IPublisher)

IPublisher::IPublisher(Subscribers& subscribers) :
	mSubscribers(&subscribers)
{
}

//-------------------------------------------------------------

void IPublisher::deliver()
{
	// don't want to trash the list in the middle of processing, iterate over a copy
	Subscribers subs = *mSubscribers;
	for(ISubscriber* sub : subs)
	{
		sub->notify(*this);
	}
}

//-------------------------------------------------------------

uint32_t IPublisher::numSubscribers() const
{
	return static_cast<uint32_t>(mSubscribers->size());
}
