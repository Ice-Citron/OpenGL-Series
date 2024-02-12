#include "VertexArray.h"

#include "Renderer.h"


VertexArray::VertexArray() { GLCall(glGenVertexArrays(1, &m_RendererID)); }
VertexArray::~VertexArray() { GLCall(glDeleteVertexArrays(1, &m_RendererID)); }

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {

	Bind();
	vb.Bind();
	const std::vector<VertexBufferElement>& elements = layout.GetElements(); // Recommended to use "const auto& elements = layout.GetElements();" instead.
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++) {
		
		// For now it seems like this vector design doesn't enable for VAA to be interjected in front of a previously inserted VAA, due to the design of VertexBufferLayout.h
		// using .pushback() method from the Vector Class. This means that the VAA's index will be in sequential order, and their index location in the VertexShader will be 
		// dependent on their index in the "std::vector<VectorBufferElement> elements" object itself.
		const VertexBufferElement& element = elements[i];
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalised, layout.GetStride(), (const void*)offset));
		GLCall(glEnableVertexAttribArray(i));

		offset += element.count * VertexBufferElement::GetSDizeOfType(element.type); 
	}
}

void VertexArray::Bind() const { GLCall(glBindVertexArray(m_RendererID)); }
void VertexArray::Unbind() const { GLCall(glBindVertexArray(0)); }
