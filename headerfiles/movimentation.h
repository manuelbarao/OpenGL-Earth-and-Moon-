#include "camera.h"
#ifndef moviment_HEADER
#define moviment_HEADER
class cMovimentationHandler {
public:
	void setCallBacks(SimpleCamera NewCamera);
	 static void MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int Modifiers);

	 static void MouseMotionCallback(GLFWwindow* Window, double X, double Y);

	 static void KeyCallback(GLFWwindow* Window, int Key, int ScanCode, int Action, int Modifers);
	 static SimpleCamera *Camera;
	
private:
	
};

#endif
