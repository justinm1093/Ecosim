
#pragma once

namespace Ecosim
{
	/**	Static utility class for generating random numbers.
	 */
	class Random final
	{
	public:

		/**	@brief Seeds the random number generator.
		 */
		static void seedRandom();

		/**	@brief Creates a random signed integer.
		 *
		 *	@param min The minimum value in the range, inclusive.
		 *	@param max The maximum value in the range, exclusive.
		 *
		 *	@return Returns a random integer in the range [min, max)
		 */
		static int32_t randomRange(int32_t min, int32_t max);

		/**	@brief Creates a random float.
		 *
		 *	@param min The minimum value in the range, inclusive.
		 *	@param max The maximum value in the range, exclusive.
		 *
		 *	@return Returns a random float in the range [min, max)
		 */
		static float randomRange(float min, float max);

		/**	@brief Creates a random xyz point within a radius.
		 *
		 *	@param radius The radius of the sphere within which
		 *				  the point will fall.
		 *
		 *	@return Returns a random position inside a sphere.
		 */
		static glm::vec3 randomVec(float radius);

	private:

		//
	};
}
