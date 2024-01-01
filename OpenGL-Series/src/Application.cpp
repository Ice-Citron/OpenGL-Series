#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>



static unsigned int CompileShader(unsigned int type, const std::string& source) {
	
	//  creates a shader object of the specified type (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER, for instance) and returns an integer identifier for the shader object
	unsigned int id = glCreateShader(type); 
	const char* src = source.c_str(); // converts source code to C-style string, because needed for OpenGL methods.

	// This line sets the source code (src) for the shader object specified by id. The 1 indicates that only one string is being passed, and the nullptr indicates that 
	// the string is null-terminated.
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id); // Compiles the shader source code associated with the shader object id.

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		// This means shader didn't compile successfully.
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char)); // alloca allows you to allocate stuff dynamically
		glGetShaderInfoLog(id, length, &length, message); 

		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex shader." : "fragment shader.") << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id);
		return 0;
	}

	// The function returns the ID of the compiled shader. This ID is used to reference the shader in other OpenGL functions, like when attaching it to a shader program.
	return id;
}


// returns an int, which should be the ID of the created shader program
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

	// This line creates a new shader program and returns its ID. A shader program in OpenGL is used to link together and manage multiple shaders 
	// (like vertex and fragment shaders).
	unsigned int program =  glCreateProgram();

	// IDs of compiled shader object. This id is used to reference this shader in other OpenGL functions. Like when attaching to a shader program.
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); 
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs); // Attahes compiled vertex and fragment shader, to the shader program. 
	glAttachShader(program, fs);
	glLinkProgram(program);      // This line links all attached shaders together in the shader program.

	// This line validates the shader program for the current OpenGL state. It's used to check whether the program can execute given the current state of bound 
	// vertex and fragment shaders.
	glValidateProgram(program);

	glDeleteShader(vs); // After linking, the individual shader objects are no longer needed, so these lines delete them to free up resources.
	glDeleteShader(fs);

	return program;
}


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);


	if (glewInit() != GLEW_OK)
		std::cout << "GLEW Error!" << std::endl;

	// Prints in console showcasing OpenGL version, 4.6.0 in this case for my ROG G16
	std::cout << (const char*)glGetString(GL_VERSION) << std::endl;

	
	float positions[3 * 2] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};
	
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (3 * 2), positions, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(0); 

	// Unbinds the current array buffer. This is a safety measure. The vertex attribute pointers are already associated with the vertex data in the buffer, so 
	// unbinding the buffer won't affect the association.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	//Creating Shader
	std::string vertexShader = "";


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}