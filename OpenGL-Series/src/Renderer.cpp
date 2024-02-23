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

void Renderer::Clear() const {

	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
	// Check EP16-EP18 notes, no need to bind VBO, because VBO is remembered by the VAO, as in, the VAO remembers which VBO does its VAAs assosciates to. 
	// However VAO don't rememvber which IBO its assosciated to. 
	shader.Bind();
	va.Bind();
	ib.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}
