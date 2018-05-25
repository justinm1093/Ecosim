
#pragma once

#include "glfw3.h"

namespace Ecosim
{
	/**	Collection of data used to initialize
	 *	the Renderer.
	 */
	struct RenderConfig
	{
		std::string name;
		int32_t width;
		int32_t height;
	};

	//=============================================================

	/**	Singleton rendering class that wraps
	 *	GLFW and OpenGL calls.
	 */
	class Renderer final
	{
	public:

		Renderer(const Renderer& other) = delete;
		Renderer& operator=(const Renderer& other) = delete;
		Renderer(Renderer&& other) = delete;
		Renderer& operator=(Renderer&& other) = delete;

		/** @brief Destructor.
		 */
		~Renderer() = default;

		/** @brief Initializes GLFW and the window.
		 *	
		 *	@return Returns true if initialization was
		 *			successful. Otherwise, false.
		 */
		bool init();

		/**	@brief Shuts down GLFW and the window.
		 */
		void shutdown();

		/**	@brief Prepares OpenGL for the current frame.
		 */
		void startFrame();

		/**	@brief Closes OpenGL for the current frame.
		 */
		void endFrame();

		/**	@brief Draws a solid-color quad.
		 *
		 *	@param origin The xyz of the top-left corner of the quad.
		 *	@param size The x and y dimensions of the quad.
		 *	@param color The rgb color of the quad.
		 */
		void drawColoredQuad(const glm::vec3& origin, const glm::vec2& size, const glm::vec3& color);

		/**	@brief Draws a solid-color circle.
		 *
		 *	@param origin The xyz of the center of the circle.
		 *	@param radius The radius of the circle.
		 *	@param color The rgb color of the circle.
		 */
		void drawColoredCircle(const glm::vec3& origin, float radius, const glm::vec3& color);

		/**	@brief Draws a line in a direction from a point.
		 *
		 *	@param origin The xyz of the start of the line.
		 *	@param direction The xyz direction of the line.
		 *	@param length The length of the line.
		 *	@param color The rgb color of the line.
		 */
		void drawRay(const glm::vec3& origin, const glm::vec3& direction, float length, const glm::vec3& color);

		/**	@brief Draws a line between two points.
		 *
		 *	@param point_1 The xyz of the first point of the line.
		 *	@param point_2 The xyz of the second point of the line.
		 *	@param color The rgb color of the line.
		 */
		void drawLine(const glm::vec3& point_1, const glm::vec3& point_2, const glm::vec3& color);

		/** @brief Registers a function callback for GLFW inputs.
		 *
		 *	@param callback The callback function pointer
		 */
		void setKeyInputCallback(GLFWkeyfun callback);

		/** @brief Sets the configuration variables for the Renderer.
		 *
		 *	@param config The config data.
		 */
		void setConfig(RenderConfig& config);

		/** @brief Gets the size of the OpenGL orthographic camera.
		 *
		 *	@return Returns mOrthoBounds.
		 */
		const glm::vec2& getOrthoBounds() const;

		/**	@brief Flags the Renderer for shutdown on the
		 *		   next frame.
		 */
		void flagForClose() const;

		/**	@brief Says whether the Renderer is flagged
		 *		   for close.
		 *
		 *	@return Returns true if the Renderer is flagged
		 *			for close. Otherwise, false.
		 */
		bool isValid() const;

		/**	@brief Gets the singleton instance of the
		 *		   Renderer.
		 *
		 *	@return Returns a pointer to the Renderer
		 *			singleton.
		 */
		static Renderer* instance();

	private:

		/** @brief Constructor.
		 */
		Renderer();

		/**	@brief Static callback for GLFW window resize
		 *		   events.
		 *
		 *	@param window Handle to the GLFW window.
		 *	@param width The new window width.
		 *	@param height The new window height.
		 */
		static void resizeWindow(GLFWwindow* window, int32_t width, int32_t height);

		/**	@brief Static callback for GLFW camera resize
		 *		   events.
		 *
		 *	@param window Handle to the GLFW window.
		 *	@param width The new camera width.
		 *	@param height The new camera height.
		 */
		static void resizeGL(GLFWwindow* window, int32_t width, int32_t height);


		RenderConfig* mConfig;

		GLFWwindow* mWindow;
		glm::vec2 mOrthoBounds;

		static Renderer* sInstance;
	};
}
