
#pragma once

#include "ISimComponent.h"

namespace Ecosim
{
	/**	A single "cell" in an Agent's NeuralNetwork.
	 *	Neurons produce output based on weighted
	 *	connections to other Neurons and a sigmoid
	 *	function.
	 *
	 *	Neuron connections are implemented as a mapping
	 *	of Neuron pointers and connection weights. This
	 *	way, a Neruon can't have redundant connections.
	 *	The output of each input is multiplied by the
	 *	weigth of that connection.
	 */
	class Neuron final : public ISimComponent
	{
	public:

		/**	Enumerated type for Neurons.
		 */
		enum class Type
		{
			SENSOR,
			OUTPUT,
			HIDDEN
		};

		Neuron(const Neuron& other) = delete;
		Neuron& operator=(const Neuron& other) = delete;
		Neuron(Neuron&& other) = delete;
		Neuron& operator=(Neuron&& other) = delete;

		/** @brief Constructor.
		 *
		 *	@param id The ID for this Neuron.
		 *	@param type The enumerated type of this Neuron.
		 */
		Neuron(std::uint32_t id, Type type);

		/** @brief Destructor.
		 */
		~Neuron() = default;

		/**	@brief Calculates the output of this Neuron based
		 *		   on the connected inputs, the connection
		 *		   weights, and a sigmoid function. Sets
		 *		   mValue.
		 */
		void calculateOutput();

		/**	@brief Renders this Neuron and all connections to
		 *		   its input Neurons. The color of the Neuron
		 *		   is based on its output value. The color of
		 *		   the connections is based on the weights.
		 *
		 *	@param renderer The simulation rendering object.
		 */
		void render(Renderer& renderer) override;

		/**	@brief Adds an input connection to this Neuron.
		 *
		 *	@param node The input Neuron.
		 *	@param weight The weight of the connection.
		 */
		void addInput(Neuron& node, float weight);

		/** @brief Adjusts the position of this Neuron
		 *		   for rendering.
		 *
		 *	@param other A Neuron connected to us.
		 */
		void setPosition(Neuron& other);

		/**	@brief Gets this Neuron's position.
		 *
		 *	@return Returns mPosition.
		 */
		const glm::vec3& getPosition() const;

		/** @brief Force-sets this Neuron's output value.
		 *
		 *	@param val The new output value.
		 */
		void setOutput(float val);

		/**	@brief Gets this Neuron's output value.
		 *
		 *	@return Returns mValue.
		 */
		float getOutput() const;

		/**	@brief Gets this Neuron's ID.
		 *
		 *	@return Returns mID.
		 */
		std::uint32_t getID() const;
		
		/**	@brief Gets this Neuron's enumerated type.
		 *
		 *	@return Returns mType.
		 */
		Type getType() const;

	private:

		std::unordered_map<Neuron*, float> mInputs;

		glm::vec3 mPosition;
		std::uint32_t mID;
		Type mType;

		float mValue;


		struct NeuronPositionData
		{
			glm::vec3 origin;
			std::uint32_t firstID;
		};

		static std::map<Type, NeuronPositionData> sPositionData;
	};
}
