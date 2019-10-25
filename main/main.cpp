// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include "../src/shader.hpp"

#define FLOATS_PER_VERTEX 3

class Object {
private:

    int numPoints;
    GLenum mode;
    GLuint vao;
    GLuint vertexBuffer;
    GLuint shaderProgram;

    void bind() {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    }

public:

    void init(int numPoints, GLenum mode) {
        this->numPoints = numPoints;
        this->mode = mode;

        glGenVertexArrays(1, &vao);
    	glBindVertexArray(vao);
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    }

    void setShaders(GLuint programID) {
        shaderProgram = programID;
    }

    void setData(GLfloat vertexBufferData[]) {
        bind();
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData)*numPoints*FLOATS_PER_VERTEX, vertexBufferData, GL_STATIC_DRAW);
    }

    void render() {
        bind();
        // Use our shader
		glUseProgram(shaderProgram);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Draw the object !
		glDrawArrays(mode, 0, numPoints);

		glDisableVertexAttribArray(0);
    }

    void cleanup() {
    	glDeleteBuffers(1, &vertexBuffer);
    	glDeleteVertexArrays(1, &vao);
    	glDeleteProgram(shaderProgram);
    }
};

Object triangle1;
Object triangle2;


/* Initialise window, VAOs, and Shaders */
int init() {
    // Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Computer Graphics", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Create and compile our GLSL program from the shaders
	GLuint redProgramID = LoadShaders( "../main/triangle.vert", "../main/triangle.frag" );
    GLuint yellowProgramID = LoadShaders( "../main/triangle.vert", "../main/yellow.frag" );

	GLfloat t1[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  0.0f, 0.0f,
	};
    GLfloat t2[] = {
		-1.0f, 1.0f, 0.0f,
		 1.0f, 1.0f, 0.0f,
		 0.0f,  0.0f, 0.0f,
	};

    triangle1.init(3, GL_TRIANGLES);
    triangle1.setData(t1);
    triangle1.setShaders(redProgramID);

    triangle2.init(3, GL_TRIANGLES);
    triangle2.setData(t2);
    triangle2.setShaders(yellowProgramID);

    // Successful initialisation
    return 0;
}

void draw() {
    // Clear the screen
    glClear( GL_COLOR_BUFFER_BIT );

    triangle1.render();
    triangle2.render();

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main( void )
{
	if (init()) {
        fprintf(stderr, "Failed to initialize.\n");
        return -1;
    }

	do{
        draw();
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

    triangle1.cleanup();
    triangle2.cleanup();
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}
