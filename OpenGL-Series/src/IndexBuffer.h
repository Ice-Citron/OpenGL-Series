#pragma once

class IndexBuffer {

private:

	unsigned int m_RendererID; // Refer to EP13-15 Notes for naming reasoning of "m_RendererID"
	unsigned int m_Count;

public:

	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};
