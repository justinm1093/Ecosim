
#include "pch.h"
#include "Renderer.h"

using namespace Ecosim;
using namespace std;
using namespace glm;

const float DEG_TO_RAD = PI / 180.0f;

Renderer* Renderer::sInstance = nullptr;

Renderer* Renderer::instance()
{
	if(sInstance == nullptr)
	{
		sInstance = new Renderer();
	}
	return sInstance;
}

//-------------------------------------------------------------

Renderer::Renderer() :
	mWindow(nullptr),
	mOrthoBounds(0.0f, 0.0f)
{
}

//-------------------------------------------------------------

bool Renderer::init()
{
	bool result = false;

	if(mConfig != nullptr && glfwInit())
	{
		mWindow = glfwCreateWindow(mConfig->width, mConfig->height, mConfig->name.c_str(), nullptr, nullptr);
		if(mWindow)
		{
			// init ortho size to default window size
			mOrthoBounds.x = (float)mConfig->width;
			mOrthoBounds.y = (float)mConfig->height;

			glfwMakeContextCurrent(mWindow);
			glfwSetWindowShouldClose(mWindow, GLFW_FALSE);

			// enforce default window and viewport size
			resizeWindow(mWindow, mConfig->width, mConfig->height);
			resizeGL(mWindow, mConfig->width, mConfig->height);

			// setting glfw window callbacks
			glfwSetWindowSizeCallback(mWindow, Renderer::resizeWindow);
			glfwSetFramebufferSizeCallback(mWindow, Renderer::resizeGL);

			result = true;
		}
	}

	return result;
}

//-------------------------------------------------------------

void Renderer::shutdown()
{
	if(mWindow)
	{
		glfwDestroyWindow(mWindow);
	}
	glfwTerminate();
}

//-------------------------------------------------------------

void Renderer::startFrame()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
}

//-------------------------------------------------------------

void Renderer::endFrame()
{
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
	glFlush();
}

//-------------------------------------------------------------

void Renderer::drawColoredQuad(const vec3& origin, const vec2& size, const vec3& color)
{
	glBegin(GL_QUADS);

	glColor3f(color.r, color.g, color.b);					// top left
	glVertex3f(origin.x, origin.y, 0);

	glColor3f(color.r, color.g, color.b);					// top right
	glVertex3f(origin.x + size.x, origin.y, 0);

	glColor3f(color.r, color.g, color.b);					// bottom right
	glVertex3f(origin.x + size.x, origin.y + size.y, 0);

	glColor3f(color.r, color.g, color.b);					// bottom left
	glVertex3f(origin.x, origin.y + size.y, 0);

	glEnd();
}

//-------------------------------------------------------------

void Renderer::drawColoredCircle(const vec3& origin, float radius, const vec3& color)
{
	glBegin(GL_TRIANGLE_FAN);

	glColor3f(color.r, color.g, color.b);
	glVertex3f(origin.x, origin.y, 0);

	float radians = 0.0f;

	for(int i = 0; i <= 360; i += 30)
	{
		radians = i * DEG_TO_RAD;
		glVertex2f(origin.x + (cos(radians) * radius), origin.y + (sin(radians) * radius));
	}

	glEnd();
}

//-------------------------------------------------------------

void Renderer::drawRay(const vec3& origin, const vec3& direction, float length, const vec3& color)
{
	glBegin(GL_LINES);

	glColor3f(color.r, color.g, color.b);

	glVertex3f(origin.x, origin.y, 0);
	glVertex3f(origin.x + direction.x * length, origin.y + direction.y * length, 0);

	glEnd();
}

//-------------------------------------------------------------

void Renderer::drawLine(const vec3& point_1, const vec3& point_2, const vec3& color)
{
	glBegin(GL_LINES);

	glColor3f(color.r, color.g, color.b);

	glVertex3f(point_1.x, point_1.y, 0);
	glVertex3f(point_2.x, point_2.y, 0);

	glEnd();
}

//-------------------------------------------------------------

void Renderer::setKeyInputCallback(GLFWkeyfun callback)
{
	glfwSetKeyCallback(mWindow, callback);
}

//-------------------------------------------------------------

void Renderer::setConfig(RenderConfig& config)
{
	mConfig = &config;
}

//-------------------------------------------------------------

const vec2& Renderer::getOrthoBounds() const
{
	return mOrthoBounds;
}

//-------------------------------------------------------------

void Renderer::flagForClose() const
{
	glfwSetWindowShouldClose(mWindow, GLFW_TRUE);
}

//-------------------------------------------------------------

bool Renderer::isValid() const
{
	return !glfwWindowShouldClose(mWindow);
}

//-------------------------------------------------------------

void Renderer::resizeWindow(GLFWwindow* window, int32_t width, int32_t height)
{
	glfwSetWindowSize(window, width, height);
}

//-------------------------------------------------------------

void Renderer::resizeGL(GLFWwindow* window, int32_t width, int32_t height)
{
	UNREFERENCED_PARAMETER(window);

	// resize viewport
	glViewport(0, 0, (GLsizei)(width), (GLsizei)(height));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// resize ortho camera
	glOrtho(0, sInstance->mOrthoBounds.x, sInstance->mOrthoBounds.y, 0, -800.0, 800.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
