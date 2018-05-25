
#pragma once

#include "EventQueue.h"

#include "Environment.h"
#include "AgentManager.h"

namespace Ecosim
{
	/**	Collection of data used to initialize
	 *	the Simulation.
	 */
	struct SimConfig
	{
		EventQueue* eventQueue;
		Renderer* renderer;
	};

	//=============================================================

	/** Singleton object that manages the program loop.
	 *	Manages a list of ISimComponent objects that can
	 *	be updated and rendered.
	 *
	 *	Also stores pointers to the Renderer and EventQueue
	 *	singletons.
	 */
	class Simulation final
	{
	public:

		Simulation(const Simulation& other) = delete;
		Simulation& operator=(const Simulation& other) = delete;
		Simulation(Simulation&& other) = delete;
		Simulation& operator=(Simulation&& other) = delete;

		/** @brief Destructor.
		 */
		~Simulation() = default;

		/** @brief Handles the program loop.
		 *
		 *	@return Returns 0 if the program runs and 
		 *			exists successfully. Returns -1 if 
		 *			the program fails to initialize.
		 */
		int32_t run();

		/** @brief Sets the configuration variables for the Simulation.
		 *
		 *	@param config The config data.
		 */
		void setConfig(SimConfig& config);

		/**	@brief Gets the singleton instance of the
		 *		   Simulation.
		 *
		 *	@return Returns a pointer to the Simulation
		 *			singleton.
		 */
		static Simulation* instance();

	private:

		/** @brief Constructor.
		 */
		Simulation();

		/**	@brief Initializes the Renderer and all
		 *		   components needed for the simulation.
		 *
		 *	@return Returns true if the initialization was
		 *			successful. Returns false if the config
		 *			data, Renderer, or EventQueue is null,
		 *			or if the Renderer fails initialization.
		 */
		bool init();

		/** @brief Shuts down the Renderer and all components.
		 */
		void shutdown();

		/** @brief Updates components and delivers events
		 *		   posted this frame.
		 */
		void update();

		/**	@brief Renders components.
		 */
		void render();

		/**	@brief Calls on the simulation clock to increment
		 *		   the time scale.
		 */
		void incrementTimeScale();

		/**	@brief Calls on the simulation clock to decrement
		 *		   the time scale.
		 */
		void decrementTimeScale();

		/**	@brief Calls on the AgentManager to toggle mutations.
		 */
		void toggleAllowMutation();

		/**	@brief Calls on the AgentManager to toggle drawing
		 *		   the NeuralNetwork.
		 */
		void toggleDrawNetwork();

		/**	@brief Calls on the AgentManager to select the next
		 *		   Agent.
		 */
		void selectNextAgent();

		/**	@brief Calls on the AgentManager to select the previous
		 *		   Agent.
		 */
		void selectPrevAgent();

		/**	@brief Toggles the simulation paused state.
		 */
		void togglePaused();

		/**	@brief Flags the Renderer for shutdown.
		 */
		void flagForClose();

		/** @brief Static callback for GLFW keyboard events.
		 *
		 *	@param window Handle to the GLFW window.
		 *	@param key The input key.
		 *	@param scancode Scancode.
		 *	@param action The input action.
		 *	@param mods Secondary keys.
		 */
		static void receiveKeyboardInput(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
		

		std::vector<std::shared_ptr<ISimComponent>> mComponents;
		std::shared_ptr<AgentManager> mAgentManager;
		std::shared_ptr<Environment> mEnvironment;

		SimClock mClock;

		SimConfig* mConfig;
		EventQueue* mEventQueue;
		Renderer* mRenderer;

		bool mIsPaused;

		typedef std::unordered_map<int32_t, std::function<void(Simulation&)>> HandlerFuncs;
		static HandlerFuncs sInputHandlers;

		static Simulation* sInstance;
	};
}
