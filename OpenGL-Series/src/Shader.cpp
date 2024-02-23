#include "Shader.h"

#include <iostream>
#include <fstream> // file stream
#include <sstream>

#include "Renderer.h"


Shader::Shader(const std::string& filepath)
	: m_Filepath(filepath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(m_Filepath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);

}

Shader::~Shader() {

	GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
	
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


unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {

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

// returns an int, which should be the ID of the created shader program --- [below is old comments for main file Application.cpp]
/*
|||| static function in global namespace just means it has internal linkage, or that the function itself won't be called outside of this cpp file/source file. There
is no functional difference if the static has been removed. It will still work.
	Consider you have two source files, FileA.cpp and FileB.cpp, and both define a function void doSomething(). If doSomething is marked static in both files, each file
	has its version of doSomething, and they do not interfere with each other. If doSomething is not marked static, however, the linker will see two global-scope functions
	with the same name, leading to a conflict.
*/
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

	// This line creates a new shader program and returns its ID. A shader program in OpenGL is used to link together and manage multiple shaders 
	// (like vertex and fragment shaders).
	unsigned int program = glCreateProgram();

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

void Shader::Bind() const {

	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {

	GLCall(glUseProgram(0));
}

void Shader::SetUniform1f(const std::string& name, float value) {
	// v1 in parameter means value_1
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
	// v1 in parameter means value_1
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

// Notes regarding std::unordered_map's usage here
/*   
	By using an unordered_map, the function avoids repeatedly calling glGetUniformLocation, which is a relatively slow operation, for uniforms that have already 
	been queried. The map serves as a cache, storing the locations of uniform variables so that subsequent queries can return the location immediately if it's
	already known. This can significantly improve performance, especially if GetUniformLocation is called frequently during rendering.
*/
int Shader::GetUniformLocation(const std::string& name) {
	
	// m_UniformLocationCache.end() returns an iterator to the end of the map, which is not the same as the last element but a position following the last element. This is 
	// used to check if the find operation was successful.
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		// The condition above checks if the uniform name is already in the map. If the find method returns an iterator that is not equal to end(), it means the element was found.

		// Returns the value associated with the key name.In this context, this value is the location of the uniform variable in the shader program, which is an integer.
		return m_UniformLocationCache[name]; 
	}

	// The OpenGL function below retreives the location of a uniform variable from the shader program. m_RendererID is presumably the identifier of the shader program, 
	// and name.c_str() converts the std::string to a C-style string, which is required by OpenGL.
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1) { std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl; }

	m_UniformLocationCache[name] = location; // Adds the key-value pair to the map, with name as the key and location as the value.
	return location;

}
