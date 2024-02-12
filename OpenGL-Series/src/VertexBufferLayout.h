#pragma once

#include <vector>
#include <stdexcept>

#include "Renderer.h"


struct VertexBufferElement {

	unsigned int type;
	unsigned int count;
	unsigned char normalised;

public:

	VertexBufferElement(unsigned int type, unsigned int count, unsigned char normalised) 
		: type(type), count(count), normalised(normalised)
	{}

	~VertexBufferElement() {}

	static unsigned int GetSDizeOfType(unsigned int type) {

		switch (type) {
			case GL_FLOAT:
				return sizeof(GLfloat);
				break;
			case GL_UNSIGNED_INT:
				return sizeof(GLuint);
				break;
			case GL_UNSIGNED_BYTE:
				return sizeof(GLubyte);
				break;
		}

		ASSERT(false); // Assert due to unknown type detected. This will invoke __debugbreak;
		return 0;
	}
};

class VertexBufferLayout {

private:

	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:

	VertexBufferLayout() 
		: m_Stride(0)
	{}

	~VertexBufferLayout() {}


	template<typename T>
	void Push(unsigned int count) {
		throw std::runtime_error("Invalid type used for VAA.");
	}

	// The push functions are to set the strides for a VAA, which will be bound to the next VBO within a VAO. // Strides = Byte offset between consecutive attribute data.
	/*
	Notes regarding "Emplace Initialisation"

			std::vector<VertexBufferElement> m_Elements;
			m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		or 
			m_Elements.emplace_back(GL_FLOAT, count, GL_FALSE);

	This process is known as "emplace" construction in the context of container classes, although you're using push_back here, which also supports this initialization style 
	since C++11.

	The key points about this initialization method are:

		1. Uniform Initialization: The use of curly braces {} provides a uniform syntax for initializing objects, whether they are aggregates, classes with constructors, 
		   arrays, etc.
		2. Direct Construction in Container: The object is constructed directly inside the container. This is more efficient than creating a temporary object and then
		   copying or moving it into the container. Although you're using push_back in this example, methods like emplace_back are specifically designed to construct objects 
		   in place by forwarding their arguments to the constructor of the element type.
		3. No Explicit Constructor Call: You don't need to explicitly call the constructor or use new. The arguments inside the curly braces are passed to the constructor 
		   of the VertexBufferElement, matching the parameters to the constructor's signature.

	This technique is widely used for its simplicity and efficiency, especially when adding objects to containers without the overhead of extra copies or dynamic memory 
	allocation (as with new).
	*/
	template<>
	void Push<float>(unsigned int count) { 
	
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE }); 
		m_Stride += VertexBufferElement::GetSDizeOfType(GL_FLOAT) * count; // sizeof(GLfloat) * count
	}

	template<>
	void Push<unsigned int>(unsigned int count) { 
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE }); 
		m_Stride += VertexBufferElement::GetSDizeOfType(GL_UNSIGNED_INT) * count; // sizeof(GLuint) * count
	}
	
	template<>
	void Push<unsigned char>(unsigned int count) {
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += VertexBufferElement::GetSDizeOfType(GL_UNSIGNED_BYTE) * count; // sizeof(GLubyte) * count
	}

	// Getters are used by VertexArray class' AddBuffer() method. Which setups the VAA sequentially based on the order in m_Elements vector which contains VectorBufferElement. 
	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};

