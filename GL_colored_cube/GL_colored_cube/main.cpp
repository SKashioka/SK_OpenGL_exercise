//
//  main.cpp
//  GL_colored_cube
//
//  Created by 柏岡秀哉 on 2021/08/08.
//

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow *window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include "shader.hpp"


int main(int argc, const char * argv[]) {
    
    // initialise GLFW
    if(!glfwInit()){
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

    // Open a window and create its OpenGL contex;
    window = glfwCreateWindow( 512, 512, "Color Bod", NULL, NULL);
    if(window == NULL){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    glewExperimental = true;
    if(glewInit() != GLEW_OK){
        fprintf(stderr, "Failed to initialize GLEW");
        getchar();
        glfwTerminate();
        return -1;
    }
    
    // Ensure we can capture the escape key being pressed below.
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // Dark blue background;
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    //Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("/Users/shuya/SKashi/OpenGL/GL_exercise/GL_colored_cube/GL_colored_cube/TransformVertexShader.vertexshader", "/Users/shuya/SKashi/OpenGL/GL_exercise/GL_colored_cube/GL_colored_cube/ColorFragmentShader.fragmentshader");
    
    // Get a handlefor our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    
    // Projection matrix : 45 degrees, 4:3 ratio, display range: 0.1 uint <-> 100 uints
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);
    // Camera matrix
    glm::mat4 View1 = glm::lookAt(
                                 glm::vec3(4,3,-3), // camera is at (4,3,3), in world space
                                 glm::vec3(0,0,0), // and looks at the origin
                                 glm::vec3(0,1,0) // head is up (set to 0, -1, 0 to look upside-down)
                                 );
    glm::mat4 View2 = glm::lookAt(
                                 glm::vec3(3,3,3), // camera is at (4,3,3), in world space
                                 glm::vec3(0,0,0), // and looks at the origin
                                 glm::vec3(0,1,0) // head is up (set to 0, -1, 0 to look upside-down)
                                 );
    
    // Model Matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model1 = glm::mat4(1.0f);
    glm::mat4 Model2 = glm::translate(
                                        glm::mat4(1.0f),
                                        glm::vec3(-2.0f, 0.0f, 0.0f));
    
    
//    glm::vec4 transformedVector = mytranslateMatrix * Model; // guess the result

    
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP1 = Projection * View1 * Model1; // Remember, matrix multiplication is the other way arround
    glm::mat4 MVP2 = Projection * View2 * Model2; // Remember, matrix multiplication is the other way arround
   
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangle each, so this makes 6*2=12 triangles, and 12*3 vertives.
    static const GLfloat g_vertex_buffer_data[] = {
         1.0f,-1.0f,-1.0f, // 1
        -1.0f,-1.0f, 1.0f, // 1
        -1.0f, 1.0f, 1.0f, // 1
         1.0f, 1.0f,-1.0f, // 2
        -1.0f,-1.0f,-1.0f, // 2
        -1.0f, 1.0f,-1.0f, // 2
         1.0f,-1.0f, 1.0f, // 3
        -1.0f,-1.0f,-1.0f, // 3
         1.0f,-1.0f,-1.0f, // 3
         1.0f, 1.0f,-1.0f, // 4
         1.0f,-1.0f,-1.0f, // 4
        -1.0f,-1.0f,-1.0f, // 4
        -1.0f,-1.0f,-1.0f, // 5
        -1.0f, 1.0f, 1.0f, // 5
        -1.0f, 1.0f,-1.0f, // 5
         1.0f,-1.0f, 1.0f, // 6
        -1.0f,-1.0f, 1.0f, // 6
        -1.0f,-1.0f,-1.0f, // 6
        -1.0f, 1.0f, 1.0f, // 7
        -1.0f,-1.0f, 1.0f, // 7
         1.0f,-1.0f, 1.0f, // 7
         1.0f, 1.0f, 1.0f, // 8
         1.0f,-1.0f,-1.0f, // 8
         1.0f, 1.0f,-1.0f, // 8
         1.0f,-1.0f,-1.0f, // 9
         1.0f, 1.0f, 1.0f, // 9
         1.0f,-1.0f, 1.0f, // 9
         1.0f, 1.0f, 1.0f, // 10
         1.0f, 1.0f,-1.0f, // 10
        -1.0f, 1.0f,-1.0f, // 10
         1.0f, 1.0f, 1.0f, // 11
        -1.0f, 1.0f,-1.0f, // 11
        -1.0f, 1.0f, 1.0f, // 11
         1.0f, 1.0f, 1.0f, // 12
        -1.0f, 1.0f, 1.0f, // 12
         1.0f,-1.0f, 1.0f  // 12
    };
    
    // One color for each vertex. They were generated randomly.
    static const GLfloat g_color_buffer_data[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };
    
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    
    
    do{
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Use our shader
        glUseProgram(programID);
        
        // Sent our transformation to the currently boud shade,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP1[0][0]);
        
        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                              0, // attribute. No paticular reason for 0, but must match the layout in the shader
                              3, // size
                              GL_FLOAT, // type
                              GL_FALSE, // normalize
                              0, // stride
                              (void*)0 // array buffer offset
                              );

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
                              1,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              (void*)0
                              );
        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 12*3);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);
        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                              0, // attribute. No paticular reason for 0, but must match the layout in the shader
                              3, // size
                              GL_FLOAT, // type
                              GL_FALSE, // normalize
                              0, // stride
                              (void*)0 // array buffer offset
                              );
        
        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
                              1,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              (void*)0
                              );
        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 12*3);
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }// Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0);
    
    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    
    return 0;
    
    
    
    

    

    
    
}
