
template <typename T>
Resource<T>::Resource() :
	IResource(sSize)
{
	registerObj(*this);
}

//-------------------------------------------------------------

template <typename T>
Resource<T>::~Resource()
{
	unregisterObj(*this);
}

//-------------------------------------------------------------

template <typename T>
void Resource<T>::init(const glm::vec3& color, float lifetime, float size)
{
	sLifetime = lifetime;
	sColor = color;
	sSize = size;
}

//-------------------------------------------------------------

template <typename T>
void Resource<T>::update(const SimClock& clock)
{
	if(mIsActive)
	{
		mTimeActive += clock.getDeltaTimeScaled();
		if(mTimeActive >= sLifetime)
		{
			deactivate();
		}
	}
}

//-------------------------------------------------------------

template <typename T>
void Resource<T>::render(Renderer& renderer)
{
	if(mIsActive)
	{
		renderer.drawColoredCircle(mPosition, sSize, sColor);
	}
}

//-------------------------------------------------------------

template <typename T>
void Resource<T>::handleCollision(PhysicalCircle& other)
{
	if(Agent* agent = other.as<Agent>())
	{
		mEffect.applyEffect(*agent);
		deactivate();
	}
}
