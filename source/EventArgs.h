
#pragma once

namespace Ecosim
{
	class IResource;
	class Agent;

	/**	Event argument for Resource deactivation.
	 */
	struct ResourceDeactivate
	{
		IResource* resource;
	};

	/**	Event argument for Agent death.
	 */
	struct AgentDeath
	{
		Agent* agent;
	};
}
