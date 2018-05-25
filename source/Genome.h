
#pragma once

#include "Gene.h"

namespace Ecosim
{
	class AgentManager;

	/**	Manages a list of Gene objects. The "genotype"
	 *	of an Agent that describes the topology of its
	 *	NeuralNetwork.
	 */
	class Genome final
	{
		friend class AgentManager;
	public:

		Genome(Genome&& other) = delete;
		Genome& operator=(Genome&& other) = delete;
		Genome& operator=(const Genome& other) = delete;

		/** @brief Constructor.
		 *	
		 *	@param id The ID for this Genome.
		 *	@param numInputs The number of input Neurons the Agent has.
		 *	@param usesNEAT Says whether NEAT mutation is allowed.
		 *	@param isPrey Says whether the Genome is Predator or Prey.
		 */
		Genome(std::uint32_t id, std::uint32_t numInputs, bool usesNEAT = false, bool isPrey = true);

		/**	@brief Copy constructor.
		 *
		 *	@param other The other Genome.
		 */
		Genome(const Genome& other);

		/**	@brief Destructor.
		 */
		~Genome();

		/**	@brief Breeds this Genome with another to create a new Genome.
		 *
		 *	@param other The other Genome.
		 *	@param isFitnessEqual Says whether the fitness of this Genome's
		 *						  Agent is equal to that of other's.
		 *
		 *	@note Unless the flag says otherwise, the fitness of this Genome's
		 *		  Agent is assumed to be greater.
		 */
		Genome* crossover(const Genome& other, bool isFitnessEqual) const;

		/**	@brief Mutates the Genes. If NEAT mutation is allowed,
		 *		   new Genes may be added. If it is disallowed,
		 *		   only the connection weights are changed.
		 *
		 *	@param fitness The fitness of the Agent.
		 */
		void mutate(float fitness);

		/**	@brief Populates this Genome with Genes defined in a file.
		 *
		 *	@param file The filestream for this Genome.
		 */
		void readFromFile(std::ifstream& file);

		/**	@brief Saves this Genome to a file so that it may be loaded
		 *		   in the same state for a new run of the simulation.
		 */
		void writeToFile() const;

		/**	@brief Adds a new Gene to the Genome.
		 *
		 *	@param innovation The new historical marker.
		 *	@param source The source Neuron ID.
		 *	@param target The target Neuron ID.
		 *	@param weight The connection weight.
		 *	@param disabled Says whether the connection is disabled.
		 */
		void addGene(std::uint32_t innovation, std::uint32_t source, std::uint32_t target, float weight, bool disabled);

		/** @brief Clears all Genes in the Genome.
		 */
		void clear();

		/**	@brief Gets the Gene at a given index.
		 *	
		 *	@param index The location of the Gene we are retrieving.
		 *
		 *	@return Returns a reference to the Gene.
		 *
		 *	@note The index provided must be within bounds of the
		 *		  Gene list.
		 */
		const Gene& operator[](std::uint32_t index) const;

		/**	@brief Says whether the Genome contains a Gene that
		 *		   describes a connection between the given Neurons.
		 *
		 *	@param source The source Neuron ID.
		 *	@param target The target Neuron ID.
		 *
		 *	@return Returns true if the Genome has a Gene that links
		 *			the source and target Neurons. Otherwise, false.
		 */
		bool hasGene(std::uint32_t source, std::uint32_t target) const;

		/**	@brief Gets the stored size of this Genome's Agent.
		 *
		 *	@return Returns mSizeGene.
		 */
		float getSizeGene() const;

		/**	@brief Gets the number of Genes in this Genome.
		 *
		 *	@return Returns the length of mGenes.
		 */
		std::uint32_t getGenomeLength() const;

		/** @brief Gets the ID for this Genome.
		 *
		 *	@return Returns mID.
		 */
		std::uint32_t getID() const;

		/** @brief Says whether this Genome can undergo NEAT mutation.
		 *
		 *	@return Returns true if NEAT mutations are allowed on this
		 *			Genome. Otherwise, false.
		 */
		bool usesNEAT() const;

		/** @brief Says whether this Genome describes Prey.
		 *
		 *	@return Returns true if the Genome describes a
		 *			Prey Agent. Otherwise, false.
		 */
		bool isPrey() const;

	private:

		/**	@brief Has a random chance to perturb the connection weight
		 *		   of each Gene. This random chance is based on fitness.
		 *
		 *	@param fitness The fitness of the Agent.
		 */
		void mutateWeights(float fitness);

		/**	@brief Adds a new Gene describing a connection between
		 *		   random source and target Neurons. If the Neurons
		 *		   already share a connection, no Gene is added.
		 *
		 *	@param fitness The fitness of the Agent.
		 */
		void mutateAddConnection(float fitness);

		/**	@brief Adds new Genes describing a new Neuron placed
		 *		   along a preexisting connection.
		 *
		 *	@param fitness The fitness of the Agent.
		 */
		void mutateAddNeuron(float fitness);

		/**	@brief Disables a random Gene.
		 *
		 *	@param fitness The fitness of the Agent.
		 */
		void mutateDisable(float fitness);

		/**	@brief Enables a random Gene.
		 *
		 *	@param fitness The fitness of the Agent.
		 */
		void mutateEnable(float fitness);

		/**	@brief Gets the ID of a random Neuron in
		 *		   the Agent's phenotype.
		 *
		 *	@param canReturnSensor Says whether we can return
		 *						   the ID of a sensor Neuron.
		 *
		 *	@return Returns a random Neuron ID in the network.
		 */
		std::uint32_t randomNeuronID(bool canReturnSensor);

		/**	@brief Initializes the next Genome ID after 
		 *		   initializing Agents from their Genome 
		 *		   files.
		 *
		 *	@param id The next Genome ID.
		 */
		static void setNextID(std::uint32_t id);

		/** @brief Gets the next Genome ID.
		 *
		 *	@return Returns sID, then increments it.
		 */
		static std::uint32_t nextID();

		
		std::vector<Gene*> mGenes;
		float mSizeGene;

		std::uint32_t mID;
		std::uint32_t mNumInputs;
		std::uint32_t mNextNeuronID;

		bool mUsesNEAT;
		bool mIsPrey;

		static std::uint32_t sID;
		static std::uint32_t sHighestInnovParsed;
		static std::uint32_t sHighestIDParsed;


		typedef std::function<void(Genome&, float)> MutationFunc;
		struct MutationRateData
		{
			float probability;
			MutationFunc func;
		};
		static std::vector<MutationRateData> sMutationFuncs;
	};
}
