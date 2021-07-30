//
//  main.cpp
//  GL_matrix
//
//  Created by 柏岡秀哉 on 2021/07/30.
//

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "../../common/shader.hpp"

int main(void){
    // Initialise GLFW
    if(!glfwInit()){
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Open a window and create its OpenGL context
    window = glfwCreateWindow(512, 512, "Matrix test", NULL, NULL);
    if(window == NULL){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    glewExperimental = true;
    if(glewInit() != GLEW_OK){
        fprintf(stderr, "failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    
    // Ensure we capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // Dark background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID); // generate vertex array object names https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGenVertexArrays.xhtml
    glBindVertexArray(VertexArrayID); // bind a vertex array object https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindVertexArray.xhtml
    
    // Create and compile our GLSL program rom the shaders
    GLuint programID = LoadShaders(
                                   "/Users/shuya/SKashi/OpenGL/GL_exercise/GL_triangle/GL_triangle/SimpleVertex.v",
                                   "/Users/shuya/SKashi/OpenGL/GL_exercise/GL_triangle/GL_triangle/SimpleFragment.f" );
    
    
    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    
    // Projection matrix: 45inf Failed of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.f), 4.0f/3.0f, 0.1f, 100.0f);
    // Or, for an ortho camera:
    //    glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
    
    // Camera matrix
    glm:mat4 View   =glm::lookAt(
                             glm::vec3(4,3,3), // Camera is at (4,3,3), in the World Space
                             glm::vec3(0,0,0), // and looks at the origin
                             glm::vec3(0,1,0)  // Head is up (set to 0, -1, 0 to look upside-down)
                             );
    
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model      = glm::mat4(1.0f);
    // Model matrix
    glm:: mat4 MVP  = Projection * View * Model; // Remember, matrix multiplication is the other way around
    
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f, 0.0f,
    };
    
    
    
}




