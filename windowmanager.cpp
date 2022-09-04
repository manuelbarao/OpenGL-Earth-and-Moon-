#include "headerfiles/windowmanager.h"

void cWindowManager::setWindowSize(int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
}

int cWindowManager::getWindowHeight()
{

	return height;
}

int cWindowManager::getWindowWidth()
{
	return width;
}

std::string cWindowManager::getWindowTitle()
{
	return windowTitle;
}

const char *cWindowManager::getWindowTitleAsChar()
{
	return windowTitle.c_str();
}

bool cWindowManager::initializeGLFW()
{
	// Inicializar a biblioteca glfw
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "GLFW initialization error" << std::endl;
		return false;
	}
	else if (glfwInit() == GLFW_TRUE) {
		std::cout << "GLFW initialization sucess" << std::endl;
		return true;
	}
	return false;
}

bool cWindowManager::createWindow()
{
	// Creates the window
		window = glfwCreateWindow(width,height,windowTitle.c_str(), monitor, windowShare);
		glfwWindowHint(GLFW_DEPTH_BITS, 32);
		if (!window)
		{
			std::cout << "Error creating the window" << std::endl;
			glfwTerminate();
			return false;
		}
		return true;
}

GLFWwindow *cWindowManager::getWindow()
{
	return window;
}

bool cWindowManager::glewInitInterface()
{
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Erro ao inicializar o GLEW" << std::endl;
		glfwTerminate();
		return false;
	}

	GLint GLMajorVersion = 0;
	GLint GLMinorVersion = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &GLMajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &GLMinorVersion);
	std::cout << "OpenGL Version  : " << GLMajorVersion << "." << GLMinorVersion << std::endl;
	std::cout << "OpenGL Vendor   : " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL Renderer : " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version  : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version    : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	return true;
}

void cWindowManager::setWindowTitle(std::string newTitle)
{
	windowTitle = newTitle;
}

cWindowManager::~cWindowManager() {

	glfwDestroyWindow(window);
	glfwTerminate();



}
