#include "shaderutils.h"
#ifndef WINDOWMANAGER_HEADER
#define WINDOWMANAGER_HEADER
class cWindowManager {
public:
	void setWindowSize(int width, int height);
	int getWindowHeight();
	int getWindowWidth();
	std::string getWindowTitle();
	void setWindowTitle(std::string newTitle);
	const char* getWindowTitleAsChar();
	bool initializeGLFW();
	bool createWindow();
	GLFWwindow *getWindow();
	bool glewInitInterface();
	~cWindowManager();
private:
	 int width {0};
	 int height {0};
	 std::string windowTitle{ "" };
	 GLFWwindow* window{nullptr};
	 GLFWwindow* windowShare{ nullptr };
	 GLFWmonitor* monitor{ nullptr };

};
#endif