
#pragma once

#include "ISubscriber.h"
#include "Agent.h"

namespace Ecosim
{
	/**	Simulation component that manages a list
	 *	of Agents.
	 */
	class AgentManager final : public ISimComponent, public ISubscriber
	{
	public:

		AgentManager(const AgentManager& other) = delete;
		AgentManager& operator=(const AgentManager& other) = delete;
		AgentManager(AgentManager&& other) = delete;
		AgentManager& operator=(AgentManager&& other) = delete;

		/**	@brief Constructor.
		 */
		AgentManager();

		/**	@brief Destructor.
		 */
		~AgentManager();

		/**	@brief Initializes a list of Agents using
		 *		   Genome files. Uses a pch macro to
		 *		   determine if it creates Predators
		 *		   and Prey, or normal Agents.
		 */
		virtual void init() override;

		/**	@brief Deletes all Agents.
		 */
		virtual void shutdown() override;

		/**	@brief Updates all living Agents.
		 *
		 *	@param clock The simulation's timekeeper.
		 */
		virtual void update(const SimClock& clock) override;

		/**	@brief Renders all living Agents. Also
		 *		   renders the NeuralNetwork of the
		 *		   currently selected Agent if flagged
		 *		   to do so.
		 *
		 *	@param renderer The simulation rendering object.
		 */
		virtual void render(Renderer& renderer) override;

		/**	@brief Selects the next Agent in the list.
		 */
		void selectNextAgent();

		/**	@brief Selects the previous Agent in the list.
		 */
		void selectPrevAgent();

		/**	@brief Toggles whether the Agents' Genomes
		 *		   are allowed to mutate after reproduction.
		 */
		void toggleAllowMutation();

		/**	@brief Toggles whether the selected Agent's
		 *		   NeuralNetwork will be rendered.
		 */
		void toggleDrawNetwork();

		/**	@brief Receives signals of Agent death, and
		 *		   breeds a new Genome for the dead Agent.
		 *
		 *	@param e The event object being handled.
		 */
		virtual void notify(const IPublisher& e) override;

	private:

		/**	@brief Writes the fitness of a dead Agent out
		 *		   to a file.
		 *
		 *	@param filename The file to which we write.
		 *	@param fitness The dead Agent's fitness.
		 */
		void writeFitnessToFile(const std::string& filename, float fitness) const;


		typedef std::vector<Agent*> Agents;
		Agents mAgents;

		uint32_t mSelectedAgentIndex;

		bool mAllowsMutation;
		bool mDrawsNetwork;
	};
}
