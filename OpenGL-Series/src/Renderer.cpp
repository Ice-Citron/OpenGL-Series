#include "Renderer.h"

#include <iostream>


void GLClearError() {

	// Clears all the errors from OpenGL.
	// glGetError only returns 1 error at once, from a list of errors, thus in order to know what the errors are, you have to loop through, until GL_NO_ERROR is returned. 
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {

	while (GLenum error = glGetError()) {
		// Now prints the flag, and also function and file name, and line.
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ": " << line << std::endl;
		return false;
	}
	return true;
}