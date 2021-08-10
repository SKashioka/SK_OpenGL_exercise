// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, 10 );
glm::vec3 rotation = glm::vec3( 0, 0, 0 );
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;
float rad = 1.0f* 3.14f/180.0f;



void computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 512/2, 512/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float( 512/2 - xpos );
	verticalAngle   += mouseSpeed * float( 512/2 - ypos );
    
	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );
    
    glm::mat3 rot_x =glm::mat3(
                             glm::vec3(1.0f,  0.0f  ,   0.0f),
                             glm::vec3(0.0f, cos(rad), -sin(rad)),
                             glm::vec3(0.0f, sin(rad),  cos(rad))
                             );
   
    glm::mat3 inv_rot_x =glm::mat3(
                             glm::vec3(1.0f,  0.0f  ,   0.0f),
                             glm::vec3(0.0f, cos(-rad), -sin(-rad)),
                             glm::vec3(0.0f, sin(-rad),  cos(-rad))
                             );
   
    glm::mat3 rot_y =glm::mat3(
                             glm::vec3(cos(rad*deltaTime), 0.0f, sin(rad*deltaTime)),
                             glm::vec3(0.0f,1.0f,0.0f),
                             glm::vec3(-sin(rad*deltaTime), 0.0f, cos(rad*deltaTime))
                             );
    
    glm::mat3 rot_z =glm::mat3(
                             glm::vec3(cos(rad*deltaTime), -sin(rad*deltaTime), 0.0f),
                             glm::vec3(-sin(rad*deltaTime), cos(rad*deltaTime), 0.0f),
                             glm::vec3(0.0f, 0.0f, 1.0f)
                             );
    
    

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}
    
    if(glfwGetKey( window, GLFW_KEY_0) == GLFW_PRESS){
        position = rot_x * position;
        direction = rot_x * direction;
        up = rot_x * up;
    }

    if(glfwGetKey( window, GLFW_KEY_1) == GLFW_PRESS){
        position = inv_rot_x * position;
        direction = inv_rot_x * direction;
        up = inv_rot_x * up;
    }
    
	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 3.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
                                position,           // Camera is here
								position + direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}
