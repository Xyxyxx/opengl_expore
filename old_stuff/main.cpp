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


	/* 
	 * Lets try doing some triangle stuff!
	 *
	 */

	/*
	// vertices of a triangle are represented as an array of 6 floats
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // vertex 1
		0.5f, -0.5f, 0.0f, // vertex 2
		0.0f, 0.5f, 0.0f, // vertex 3
	};
	*/

	float vertices[] = {
		0.5f, 0.5f, 0.0f, // top right
		0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, // top left
	};

	unsigned int VBO; // ptr to vertex buffer object
	glGenBuffers(1, &VBO);

	// VBOs has lots of types
	// one type we can assign is as an array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// from now on, any buffer calls we make on the GL_ARRAY_BUFFER target will be used to configure VBO
	
	// so now can copy the data of vertices to the VBO
	// glBufferData(type of buffer target, size of data, data itself, how GPU handle data)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
	//GL_STATIC_DRAW: the data is only set once, and this is used many times
	//GL_DYNAMIC_DRAW: better used for data that is meant to be changed frequently. Will be put in faster place in memory
	
	// now we want to create our shaders
	

	// first work on vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// glShaderSource(shader to compile, 
	//	 how many strings we're passing as source code,\
	//	 actual source code of vertex shader,
	//	 leave 4th param as NULL for now)
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader); // actually compiling the shader

	// and then we can check for compile time errors:
	int success = 0;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s", infoLog);
	}
	//printf("Vertex compilation success!\n");

	// now work on fragment shader
	unsigned int fragmentShader = 0;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// again check compile time errors
	success = 0;
	infoLog[0] = '\0'; // basically empty string
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s", infoLog);
	}
	//printf("Fragment shader successfully compiled!\n");

	// shader program: final linked version of multiple shaders combined
	// to use the shaders we just compiled, need to link them to a shader program object
	// then activate this shader program when rendering objects
	// the activated shader program's shaders will be used when we issue render calls
	//
	// when linking shaders in the program, the outputs of each shader gets linked to input of next shader
	// also the case when you're linking errors if your output and inputs do not match
	
	// creating shader program object
	unsigned int shaderProgram = 0;
	shaderProgram = glCreateProgram(); 

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// we can also check if linking was successful
	success = 0;
	infoLog[0] = '\0';
	
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM:DID_NOT_LINK\n %s", infoLog);
	}
	//printf("Successfully linked shaders to program\n");

	/*
	// now we need to tell OpenGL how to interpret the vertex data
	// will use glVertexAttributePointer to do this
	// glVertexAttributePointer(which vertex attribute we want to configure - we actually set this in the
	//							vertex shader: layout (location = 0),
	//	 size of vertex attribute (for us vec3 means 3 values),
	//	 type of data (vec* in GLSL means vector of floats),
	//	 tells if data is normalized or not
	//	 stride: tells us space between consecutive vertex attributes.
	//	 offset where the position data begins in the buffer: since position data is at start, just set to 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

	// now we told openGL how to interpret the vertex attributes, we should then enable the attributes
	glEnableVertexAttribArray(0);

	// once successfully linked programs, we can activate the program
	glUseProgram(shaderProgram);
	// every shader and rendering call after this will now use the program object (and thus the shaders)

	// after this, we have to then draw the object
	// someOpenGLFunctionThatDrawsOurObject(); 

	// no longer need the shader objects anymore, so we can delete them.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// with this method, we have to repeat the above every time we want to draw an object
	// this is cumbersome, so is there a better way?
	*/
	// vertex array object

	/*	
	// initialization of VAO and other code here
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// 1: bind vertex array object
	glBindVertexArray(VAO);

	// 2: copy vertices array in buffer for OpenGL to use:
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3: set vertex attributes of pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);

	// then use drawing code in rend loop
	*/	

	// element buffer objects EBO:
	// similar to VAOs and VBOs
	// Buffer object that store indices indicating which vertices to draw
	// e.g. you want to draw a square:
	// you can draw as two separate triangles, but that has vertex overlap - inefficient
	// you can use EBO to make more efficient:

	/*
	float rec_verts[] = {
		0.5f, 0.5f, 0.0f, // top right
		0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, // top left
	};
	*/

	unsigned int indices[] {
		0, 1, 3, // first triangle
		1, 2, 3, // second triangle
	};

	// can then make EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	// similar to VBO, can bind and copy indices into EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// then for everything else
	
	//set up VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);


	//set vertex attribute of pointers

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);

	// render loop
	//
	// render loop should look like:
	// Loop:
	//	input
	//
	//	rendering
	//
	//	check and call events and swap buffers
	while (!glfwWindowShouldClose(window)) {
		// input		
		processInput(window);


		//rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// drawing objects:
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		
		// funcToDrawTriangle();
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// check and call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glfwTerminate(); //terminate the program

	printf("Successfully ran the test. Returning 0... \n");
	return 0; 
}
