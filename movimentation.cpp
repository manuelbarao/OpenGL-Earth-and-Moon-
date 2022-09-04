#include "headerfiles/movimentation.h"
SimpleCamera *cMovimentationHandler::Camera = nullptr;

 void cMovimentationHandler::MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int Modifiers)
{
	// std::cout << "Button: " << Button << " Action: " << Action << " Modifiers: " << Modifiers << std::endl;
	 
	 
	if (Button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (Action == GLFW_PRESS)
		{
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			double X, Y;
			glfwGetCursorPos(Window, &X, &Y);
			
			Camera->PreviousCursor = glm::vec2{ X, Y };
			Camera->bEnableMouseMovement = true;
		}
		else if (Action == GLFW_RELEASE)
		{
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			Camera->bEnableMouseMovement = false;
		}
	}
}

 void cMovimentationHandler::MouseMotionCallback(GLFWwindow* Window, double X, double Y)
{
	// std::cout << "X: " << X << " Y: " << Y << std::endl;
	 Camera->MouseMove(X, Y);
}

 void cMovimentationHandler::KeyCallback(GLFWwindow* Window, int Key, int ScanCode, int Action, int Modifers)
{
	// std::cout << "Key: " << Key << " ScanCode: " << ScanCode << " Action: " << Action << " Modifiers: " << Modifers << std::endl;	

	if (Action == GLFW_PRESS)
	{
		switch (Key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(Window, true);
			break;

		case GLFW_KEY_W:
			Camera->MoveForward(1.0f);
			break;

		case GLFW_KEY_S:
			Camera->MoveForward(-1.0f);
			break;

		case GLFW_KEY_A:
			Camera->MoveRight(-1.0f);
			break;

		case GLFW_KEY_D:
			Camera->MoveRight(1.0f);
			break;

		default:
			break;
		}
	}
	else if (Action == GLFW_RELEASE)
	{
		switch (Key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(Window, true);
			break;

		case GLFW_KEY_W:
			Camera->MoveForward(0.0f);
			break;

		case GLFW_KEY_S:
			Camera->MoveForward(0.0f);
			break;

		case GLFW_KEY_A:
			Camera->MoveRight(0.0f);
			break;

		case GLFW_KEY_D:
			Camera->MoveRight(0.0f);
			break;

		default:
			break;
		}
	}
}

void cMovimentationHandler::setCallBacks(SimpleCamera NewCamera)
{
	
}
