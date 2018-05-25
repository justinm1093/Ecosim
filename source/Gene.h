
#pragma once

namespace Ecosim
{
	class AgentManager;
	class Genome;

	/**	Encapsulates data that describes a connection 
	 *	between Neurons in an Agent's NeuralNetwork.
	 */
	class Gene final
	{
		friend class AgentManager;
		friend class Genome;

	public:

		Gene(Gene&& other) = delete;
		Gene& operator=(Gene&& other) = delete;

		/** @brief Constructor.
		 *	
		 *	@param innovation The historical marker for this Gene.
		 *	@param source The source Neuron ID.
		 *	@param target The target Neuron ID.
		 *	@param weight The weight of the connection.
		 *	@param disabled Says whether the connection is disabled.
		 */
		Gene(std::uint32_t innovation, std::uint32_t source, std::uint32_t target, float weight = 0.0f, bool disabled = false);

		/**	@brief Copy constructor.
		 *
		 *	@param other The other Gene.
		 */
		Gene(const Gene& other);

		/**	@brief Copy assignment.
		 *
		 *	@param other The other Gene.
		 */
		Gene& operator=(const Gene& other);

		/**	@brief Destructor.
		 */
		~Gene() = default;

		/**	@brief Enables the connection this
		 *		   Gene describes.
		 */
		void enable();

		/**	@brief Disables the connection this
		 *		   Gene describes.
		 */
		void disable();

		/**	@brief Toggles the connection this
		 *		   Gene describes.
		 */
		void toggleEnabled();

		/**	@brief Sets the weight of the connection
		 *		   this Gene describes.
		 *
		 *	@param weight The new weight.
		 */
		void setWeight(float weight);

		/**	@brief Gets the historical marker for
		 *		   this Gene.
		 *
		 *	@return Returns mInnovation.
		 */
		std::uint32_t getInnovation() const;

		/**	@brief Gets the source Neuron ID for
		 *		   this Gene.
		 *
		 *	@return Returns mSource.
		 */
		std::uint32_t getSource() const;

		/**	@brief Gets the target Neuron ID for
		 *		   this Gene.
		 *
		 *	@return Returns mTarget.
		 */
		std::uint32_t getTarget() const;

		/**	@brief Gets the connection weight between
		 *		   source and target.
		 *
		 *	@return Returns mWeight.
		 */
		float getWeight() const;

		/**	@brief Says whether this Gene is expressed
		 *		   in the NeuralNetwork as a connection.
		 *
		 *	@return Returns true if the Gene is disabled.
		 *			Otherwise, false.
		 */
		bool isDisabled() const;

	private:

		/**	@brief Initializes the historical markers for
		 *		   Genes after initializing Agents from
		 *		   their Genome files.
		 *
		 *	@param innovation The next historical marker.
		 */
		static void setNextInnovation(std::uint32_t innovation);

		/** @brief Gets the next historical marker for Genes.
		 *
		 *	@return Returns sInnovation, then increments it.
		 */
		static std::uint32_t nextInnovation();


		std::uint32_t mInnovation;

		std::uint32_t mSource;
		std::uint32_t mTarget;

		float mWeight;

		bool mIsDisabled;

		static std::uint32_t sInnovation;
	};
}
