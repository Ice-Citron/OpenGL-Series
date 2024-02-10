#include "IndexBuffer.h"

#include "Renderer.h"


IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) 
	: m_Count(count)
{
	
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); // this is specifying that this buffer object will be used for element indices during drawing operations. 
	// [below] Creates and initialises a buffer object's data store // Uploading index data from CPU RAM to GPU's VRAM. 
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {	GLCall(glDeleteBuffers(1, &m_RendererID)); }

void IndexBuffer::Bind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); }

void IndexBuffer::Unbind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

