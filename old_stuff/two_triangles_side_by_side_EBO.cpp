#include <stdio.h>

#include <glad/glad.h>
#include <glfw/include/GLFW/glfw3.h>

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
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
	GLFWwindow *window = glfwCreateWindow(800, 600, "Learn Open GL", NULL, NULL);

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

	// take out the trash
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// now we try to draw two triangles next to each other

	// now set up the vertices and indices
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		-0.25f, 0.5f, 0.0,
		0.0f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.25f, 0.5f, 0.0f,
	};

	int indices[] = {
		0, 1, 2,
		2, 3, 4
	};


	// setting up our buffer objects
	
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO); // Vertex Array Object
	glGenBuffers(1, &VBO); // Vertex Buffer Array
	glGenBuffers(1, &EBO); // Element Buffer Array

	glBindVertexArray(VAO); // vertex array is our VAO 

	glBindBuffer(GL_ARRAY_BUFFER, VBO); // VBO is our Array Buffer
	// put all vertices into array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // set EBO as element array buffer
	// put the indices into EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// set vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);

	// put the vertices into array buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// bind it into vertex array
	glBindVertexArray(0);


	// render loop

	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	printf("Successfully ran the test. Returning 0... \n");
	return 0; 
}
