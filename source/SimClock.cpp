
#include "pch.h"
#include "SimClock.h"

using namespace Ecosim;
using namespace std;
using namespace std::chrono;

const float ONE_THOUSANDTH = 0.001f;
const float MAX_FRAME_RATE = 60;
const float FRAME_DURATION = 1 / MAX_FRAME_RATE;

SimClock::SimClock(uint32_t maxTimeScale) :
	mMaxTimeScale(maxTimeScale),
	mTotalTime(0.0f),
	mDeltaTime(0.0f)
{
	reset();
}

//-------------------------------------------------------------

void SimClock::reset()
{
	mLastTime = mCurrTime = mStartTime = high_resolution_clock::now();
	mTimeScale = 1;
}

//-------------------------------------------------------------

void SimClock::update()
{
	mCurrTime = high_resolution_clock::now();

	mDeltaTime = duration_cast<milliseconds>(mCurrTime - mLastTime).count() * ONE_THOUSANDTH;
	mTotalTime = duration_cast<milliseconds>(mCurrTime - mStartTime).count() * ONE_THOUSANDTH;

	if(mDeltaTime < FRAME_DURATION)
	{
		uint32_t waitTime = (uint32_t)((FRAME_DURATION - mDeltaTime) * 1000);
		this_thread::sleep_for(milliseconds(waitTime));

		mCurrTime = high_resolution_clock::now();

		mDeltaTime = duration_cast<milliseconds>(mCurrTime - mLastTime).count() * ONE_THOUSANDTH;
		mTotalTime = duration_cast<milliseconds>(mCurrTime - mStartTime).count() * ONE_THOUSANDTH;
	}

	// one time calculation for scaled delta time -- no point in redoing this every time we may need it
	mDeltaTimeScaled = mDeltaTime * mTimeScale;

	mLastTime = mCurrTime;
}

//-------------------------------------------------------------

void SimClock::incrementTimeScale()
{
	if(mTimeScale < mMaxTimeScale)
	{
		++mTimeScale;
		cout << "Time Scale -- " << mTimeScale << endl;
	}
}

//-------------------------------------------------------------

void SimClock::decrementTimeScale()
{
	if(mTimeScale > 1)
	{
		--mTimeScale;
		cout << "Time Scale -- " << mTimeScale << endl;
	}
}

//-------------------------------------------------------------

const high_resolution_clock::time_point& SimClock::getStartTime() const
{
	return mStartTime;
}

//-------------------------------------------------------------

float SimClock::getTotalTime() const
{
	return mTotalTime;
}

//-------------------------------------------------------------

float SimClock::getDeltaTime() const
{
	return mDeltaTime;
}

//-------------------------------------------------------------

float SimClock::getDeltaTimeScaled() const
{
	return mDeltaTimeScaled;
}

//-------------------------------------------------------------

uint32_t SimClock::getTimeScale() const
{
	return mTimeScale;
}
