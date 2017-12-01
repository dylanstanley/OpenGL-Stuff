// Include GLFW
#include <glfw3.h>

extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

						   // Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"



glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}

const double PI = std::atan(1.0) * 4;
// position
glm::vec3 position = glm::vec3(0, 0, 5);
// horizontal angle : toward -Z
float horizontalAngle = PI;
// vertical angle : 0, look at the horizon
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 10.0f; // 3 units / second
float mouseSpeed = 0.05f;
double xpos, ypos = 0;
double oldXPos, oldYPos = 0;
void computeMatricesFromInputs() {
	static double lastTime = glfwGetTime();

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position

	glfwGetCursorPos(window, &xpos, &ypos);

	//Reset mouse pos for next frame

	// Compute new orientation
	horizontalAngle -= mouseSpeed * deltaTime * (xpos - oldXPos);
	verticalAngle -= mouseSpeed * deltaTime * (ypos - oldYPos);
	verticalAngle = clamp(verticalAngle, float(-PI / 2.0f), float(PI / 2.0f));

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - PI / 2.0f),
		0,
		cos(horizontalAngle - PI / 2.0f)
	);
	// Up vector : perpendicular to both direction and right
	glm::vec3 up = glm::cross(right, direction);

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
	}
	// Move up
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		position += up * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		position -= up * deltaTime * speed;
	}
	//Reset camera position and angle
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		position = glm::vec3(-15, 15, 15);
		// horizontal angle : toward -Z
		horizontalAngle = PI * 0.75f;
		// vertical angle : 0, look at the horizon
		verticalAngle = -PI * 0.2f;
		// Initial Field of View
		initialFoV = 45.0f;

		speed = 10.0f; // 3 units / second
		mouseSpeed = 0.05f;
		xpos, ypos = 0;
		oldXPos, oldYPos = 0;
		glfwSetCursorPos(window, 0, 0);
	}
	float FoV = initialFoV;

	// Projection matrix : 45&deg; Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 16.0f / 9.0f, 0.1f, 1000.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	oldXPos = xpos;
	oldYPos = ypos;

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}