#pragma once

class VertexBuffer {

private:

	unsigned int m_RendererID; // Refer to EP13-15 Notes for naming reasoning of "m_RendererID"

public:

	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};

