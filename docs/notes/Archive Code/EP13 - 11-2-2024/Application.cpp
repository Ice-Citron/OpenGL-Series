#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream> // file stream
#include <string>
#include <sstream>


// MSVC specific function. __ means that its compiler intrinsic. This essentially inserts a breakpoint whenver an error is encountered. 
#define ASSERT(x) if (!x) __debugbreak(); // Only works in debug mode. 
#define GLCall(x) GLClearError();\
				x;\
				ASSERT(GLLogCall(#x, __FILE__, __LINE__))
				//'#x' converts x into a string // __FILE__ and __LINE__ is supported by all compilers, unlike __debugbreak(), which is only for MSVCs


static void GLClearError() {

	// Clears all the errors from OpenGL.
	// glGetError only returns 1 error at once, from a list of errors, thus in order to know what the errors are, you have to loop through, until GL_NO_ERROR is returned. 
	while (glGetError() != GL_NO_ERROR); 
}

static bool GLLogCall(const char* function, const char* file, int line) { 

	while (GLenum error = glGetError()) {
		// Now prints the flag, and also function and file name, and line.
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ": " << line << std::endl;
		return false;
	}
	return true;
}

struct ShaderProgramSource {

	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {

	std::ifstream stream(filepath); // ifstream = input file stream

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) { // getline(), imported from string lib. Returns true, if there are still more lines to read inside the file. 

		if (line.find("#shader") != std::string::npos) { // npos means hasn't found, or false. Thus, this if condition is, IF the word "shader" is found in the line....

			// We need to then see which type of shader is it. 
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			// pushes line by line, the contents of the shaderfile into the stringstream.
			ss[(int)type] << line << '\n'; 
		}
	
	}
	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
	
	//  creates a shader object of the specified type (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER, for instance) and returns an integer identifier (ID) for the shader object
	unsigned int id = glCreateShader(type); 
	const char* src = source.c_str(); // converts source code to C-style string, because needed for OpenGL methods.

	// This line assosciated the source code (src) to the shader object specified by id. The 1 indicates that only one string is being passed, and the nullptr indicates
	// that the string is null-terminated (check notes for further info on params |ep10-12|)
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id)); // Compiles the shader source code associated with the shader object id.

	int result;
	// retrieves the compilation status of the shader object id. The status is stored in result. If result is GL_FALSE, it indicates the shader did not compile successfully.
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result)); 

	if (result == GL_FALSE) {
		// This means shader didn't compile successfully.

		int length;
		//retrieves the length of the compilation error log(including the null terminator).This length is used to allocate enough space for the error message.
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

		char* message = (char*)alloca(length * sizeof(char)); // alloca allows you to allocate stuff dynamically. Allocating memory for error log.
		GLCall(glGetShaderInfoLog(id, length, &length, message)); 

		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex shader." : "fragment shader.") << std::endl;
		std::cout << message << std::endl;

		GLCall(glDeleteShader(id));
		return 0;
	}

	// The function returns the ID of the compiled shader. This ID is used to reference the shader in other OpenGL functions, like when attaching it to a shader program.
	return id;
}


// returns an int, which should be the ID of the created shader program			
/*
|||| static function in global namespace just means it has internal linkage, or that the function itself won't be called outside of this cpp file/source file. There 
is no functional difference if the static has been removed. It will still work. 
	Consider you have two source files, FileA.cpp and FileB.cpp, and both define a function void doSomething(). If doSomething is marked static in both files, each file 
	has its version of doSomething, and they do not interfere with each other. If doSomething is not marked static, however, the linker will see two global-scope functions 
	with the same name, leading to a conflict.
*/
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

	// This line creates a new shader program and returns its ID. A shader program in OpenGL is used to link together and manage multiple shaders 
	// (like vertex and fragment shaders).
	unsigned int program =  glCreateProgram();

	// IDs of compiled shader object. This id is used to reference this shader in other OpenGL functions. Like when attaching to a shader program.
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); 
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs)); // Attahes compiled vertex and fragment shader, to the shader program. 
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));      // This line links all attached shaders together in the shader program.
	
	// This line validates the shader program for the current OpenGL state. It's used to check whether the program can execute given the current state of bound 
	// vertex and fragment shaders.
	GLCall(glValidateProgram(program));
	
	GLCall(glDeleteShader(vs)); // After linking, the individual shader objects are no longer needed, so these lines delete them to free up resources.
	GLCall(glDeleteShader(fs));

	return program;
}


int main()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	// Setting up OpenGL Core Profile (Compatibility by default)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // These 2 line sets OpenGL version to 3.3.		|| Major version - 3.0 || Minor version - 0.3 || 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // alternative, for compatibilty profile instead -- GLFW_OPENGL_COMPAT_PROFILE 

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1); // turns on Vsync

	if (glewInit() != GLEW_OK)
		std::cout << "GLEW Error!" << std::endl;

	// Prints in console showcasing OpenGL version, 4.6.0 in this case for my ROG G16
	std::cout << (const char*)glGetString(GL_VERSION) << std::endl;

	
	float positions[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f
	};

	unsigned int indices[] = { 
		0, 1, 2,
		2, 3, 0 
	}; 

	// Vertex Array Object (VAO)
	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao)); // activates vao

	// Vertex Buffer
	unsigned int buffer;
	GLCall(glGenBuffers(1, &buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (4 * 2), positions, GL_STATIC_DRAW)); // creates and initialises a buffer object's data store
	
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
	GLCall(glEnableVertexAttribArray(0));

	// Index Buffer
	unsigned int ibo;
	GLCall(glGenBuffers(1, &ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo)); // this is specifying that this buffer object will be used for element indices during drawing operations. 
	// [below] Creates and initialises a buffer object's data store // Uploading index data from CPU RAM to GPU's VRAM. 
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW)); 


	// Unbinds the current array buffer. This is a safety measure. The vertex attribute pointers are already associated with the vertex data in the buffer, so 
	// unbinding the buffer won't affect the association.
	// GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); // will lead to black screen, only unbind if have proper VAO setup, check [ep10-12] notes.
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));


	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	GLCall(glUseProgram(shader));

	GLCall(int location = glGetUniformLocation(shader, "u_Color")); // naming convention of uniform. with 'u_'
	ASSERT((location != -1));
	GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

	float r = 0.0f;
	float increment = 0.01f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f)); // uniform is set per draw call, unlike vertex attributes which are set per vertex. 

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		if (r > 1.0f)
			increment = -0.01f;
		else if (r < 0.0f)
			increment = 0.01f;

		r += increment;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}