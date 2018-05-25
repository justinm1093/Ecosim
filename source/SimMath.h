
#pragma once

namespace Ecosim
{
	/**	Static utility class for important math functions.
	 *	Nice to have all of this stuff in one place.
	 */
	class SimMath final
	{
	public:

		/** @brief Sigmoid function for Neuron activation.
		 *
		 *	@param val The Neuron value.
		 *
		 *	@return Returns the Neuron's normalized output.
		 */
		static float neuronSigmoid(float val);

		/**	@brief Sigmoid function that determines an Agent's
		 *		   top move speed based on size.
		 *
		 *	@param size The Agent's size.
		 *
		 *	@return Returns the Agent's top move speed.
		 */
		static float moveSpeedFromSize(float size);

		/**	@brief Sigmoid function that determines an Agent's
		 *		   top turn speed based on size.
		 *
		 *	@param size The Agent's size.
		 *
		 *	@return Returns the Agent's steer move speed.
		 */
		static float steerSpeedFromSize(float size);

		/**	@brief Sigmoid function that determines a Gene's
		 *		   chance for mutation based on Agent fitness.
		 *
		 *	@param fitness The Agent's fitness.
		 *
		 *	@return Returns the Gene's chance for mutation on
		 *			the range [0, 1]
		 */
		static float mutationChanceFromFitness(float fitness);

		/**	@brief Clamps a given number to within a given range.
		 *
		 *	@param val The number to clamp.
		 *	@param min The minimum value in the range.
		 *	@param max The maximum value in the range.
		 *
		 *	@return Returns the number clamped to the range [min, max]
		 */
		static float clampNum(float val, float min, float max);

		/** @brief Linearly interpolates a value to the range
		 *		   [0, 1] based on its relationship with a 
		 *		   given range.
		 *
		 *	@param val The number to interpolate.
		 *	@param min The minimum value in the range.
		 *	@param max The maximum value in the range.
		 *
		 *	@return Returns the number interpolated to the range [0, 1]
		 *			based on its relationship with min and max.
		 *
		 *	@note If 'val' is less than 'min' the fucntion returns 0.
		 *		  If 'val' is more than 'max' the function returns 1.
		 */
		static float lerp(float val, float min, float max);
	};
}
