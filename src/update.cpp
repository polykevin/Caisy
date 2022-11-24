#include "update.h"
#include "window.h"

System::System()
	: text(800, 600)
{
	// Load the text rendering system
	text.Load("Resources/fonts/Poly/Poly-Regular.otf", 48);
}

void System::update()
{
	// Delta time
	glfwSwapInterval(1);
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	// Shows the text 
	text.RenderText(textString, 10.0f, 550.0f, 0.8f, glm::vec3(0.3, 0.7f, 0.9f));
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);// move the camera
	text.TextShader.setMat4("view", view);
}
