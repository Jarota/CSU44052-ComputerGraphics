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

class Object {

  public:
    GLuint shaderProgram;
    GLuint vao;
    GLuint vertexBuffer;

    Object() {
        vao = 0;
        vertexBuffer = 0;
        shaderProgram = 0;
    }

    Object(GLfloat vertices[], GLuint shaderProgram) {
        shaderProgram = shaderProgram;

    	glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

    	glGenBuffers(1, &vertexBuffer);
    	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }

    void destroy() {
        glDeleteBuffers(1, &vertexBuffer);
    	glDeleteVertexArrays(1, &vao);
    	glDeleteProgram(shaderProgram);
    }

    void render() {
        glBindVertexArray(vao);
        // Use our shader
        glUseProgram(shaderProgram);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

        //glDisableVertexAttribArray(0);
    }
};

Object* triangle1 = new Object();
Object* triangle2 = new Object();


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

    triangle1 = new Object(t1, redProgramID);

    triangle2 = new Object(t2, yellowProgramID);

    // Succesful initialsation
    return 0;
}

void draw() {
    // Clear the screen
    glClear( GL_COLOR_BUFFER_BIT );

    triangle1->render();
    triangle2->render();

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main( void )
{
	int res = init();
    if ( res != 0 ){
        fprintf(stderr, "Failed to initialize program.\n");
        return res;
    }

	do{
        draw();
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

    triangle1->destroy();
    triangle2->destroy();
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}
