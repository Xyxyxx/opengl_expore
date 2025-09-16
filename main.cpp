#include <stdio.h>

#include "glad/glad.h"
#include "glfw/include/GLFW/glfw3.h"


// really simple vertex shader
const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main() {\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
	"}\0";

// really simple fragment shader
const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main() {\n" 
	"	FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);\n"
	"}";

const char *fragmentShaderSourceYellow = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
	"	FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);\n"
	"}";

// declare all the function prototypes (I apologize for the bad coding practice)

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, true);
	}

}

int main() {

	if (!glfwInit()) {
		printf("GLFW failed to initalize\n");
		return 1; //error something went wrong!
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window object
	GLFWwindow *window = glfwCreateWindow(800, 600, "Testing", NULL, NULL);

	if (window == NULL) {
		printf("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); // make window the current context object

	// try to intialize glad
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		printf("Failed to initalize Glad\n");
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// make our shaders
	
	// vertex shader

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for compile errors
	int success = 0;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s\n", infoLog);
	}

	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for compile errors
	success = 0;
	infoLog[0] = '\0';
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s\n", infoLog);
	}

	// yellow shader
	unsigned int yellowFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(yellowFragmentShader, 1, &fragmentShaderSourceYellow, NULL);
	glCompileShader(yellowFragmentShader);

	success = 0;
	infoLog[0] = 0;
	glGetShaderiv(yellowFragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(yellowFragmentShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENTYELLOW::COMPILATION_FAILED\n %s\n", infoLog);
	}

	// link shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	// check for linking errors
	success = 0;
	infoLog[0] = '\0';
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR::PROGRAM::SHADER::LINKING_FAILED\n %s\n", infoLog);
	}

	// yellow shader
	unsigned int yellowShaderProgram = glCreateProgram();
	glAttachShader(yellowShaderProgram, vertexShader);
	glAttachShader(yellowShaderProgram, yellowFragmentShader);
	glLinkProgram(yellowShaderProgram);


	// check for linking errors again
	success = 0;
	infoLog[0] = '\0';
	glGetProgramiv(yellowShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(yellowShaderProgram, 512, NULL, infoLog);
		printf("ERROR::PROGRAM::YELLOWSHADER::LINKING_FAILED\n %s\n", infoLog);
	}
	// take out the trash
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(yellowFragmentShader);


	// now we try to draw two triangles next to each other
	// this time using two different VAOs and VBOs

	// now set up the vertices and indices
	float triangle_1[] = {
		-0.5f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.0f,
		-0.25f, 0.0f, 0.0f,
	};

	float triangle_2[] = {
		0.0f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.25f, 0.0f, 0.0f,
	};


	float triangle_3[] = {
		-0.25f, 0.0f, 0.0f,
		0.25f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f,
	};


	// setting up our buffer objects
	
	unsigned int VBO1, VAO1;
	glGenVertexArrays(1, &VAO1); // Vertex Array Object
	glGenBuffers(1, &VBO1); // Vertex Buffer Array

	glBindVertexArray(VAO1); // vertex array is our VAO 

	glBindBuffer(GL_ARRAY_BUFFER, VBO1); // VBO is our Array Buffer
	// put triangle_1 into VBO1
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_1), triangle_1, GL_STATIC_DRAW);

	// set vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);

	// for the 2nd triangle
	unsigned int VBO2, VAO2;
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);

	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	// put triangle 2 into VBO2
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_2), triangle_2, GL_STATIC_DRAW);

	// set vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);
	
	// another fucking VAO and VBO
	unsigned int VBO3, VAO3;
	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);

	glBindVertexArray(VAO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	// put triangle 3 into VBO3
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_3), triangle_3, GL_STATIC_DRAW);

	// set vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	
	// render loop

	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		//render
		glClearColor(0.2f, 0.8f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// draw second triangle
		//glUseProgram(yellowShaderProgram);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glBindVertexArray(VAO3);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glDeleteVertexArrays(1, &VAO1);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &VBO2);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(yellowShaderProgram);

	printf("Successfully ran the test. Returning 0... \n");
	return 0; 
}
