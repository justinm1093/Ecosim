
#pragma once

#include "ISimComponent.h"

#include "Genome.h"
#include "Neuron.h"

namespace Ecosim
{
	/** Manages an ordered map of interconnected
	 *	Neurons that produce outputs that affect
	 *	Agent steering behaviors.
	 */
	class NeuralNetwork final : public ISimComponent
	{
	public:

		NeuralNetwork(const NeuralNetwork& other) = delete;
		NeuralNetwork& operator=(const NeuralNetwork& other) = delete;
		NeuralNetwork(NeuralNetwork&& other) = delete;
		NeuralNetwork& operator=(NeuralNetwork&& other) = delete;

		/**	@brief Constructor.
		 *
		 *	@param numInputs The number of sensor Neurons that
		 *					 this network will have.
		 */
		explicit NeuralNetwork(std::uint32_t numInputs);

		/**	@brief Destructor.
		 */
		~NeuralNetwork();

		/**	@brief Creates a network topology using the Neuron
		 *		   connections described in a given Genome.
		 *
		 *	@param genome The Agent's Genome.
		 */
		void createNetwork(const Genome& genome);

		/**	@brief Destroys all Neurons in the network.
		 */
		void clearNetwork();

		/**	@brief Calculates the network outputs by evaluating
		 *		   all Neurons in the network.
		 *
		 *	@param inputs The values fed to the sensor Neurons.
		 *	@param outputs The values retrieved from the output Neurons.
		 */
		void evaluate(const float* inputs, float* outputs);

		/**	@brief Renders each Neuron and connections between them.
		 *
		 *	@param renderer The simulation rendering object.
		 */
		virtual void render(Renderer& renderer) override;

		/**	@brief Gets the number of sensor Neurons.
		 *
		 *	@return Returns mNumInputs.
		 */
		std::uint32_t getNumInputs() const;

	private:

		std::map<std::uint32_t, Neuron*> mNeurons;
		std::uint32_t mNumInputs;
	};
}
