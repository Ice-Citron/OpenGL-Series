#pragma once

#include <string>
#include <unordered_map> // hash map


struct ShaderProgramSource {

	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {

private:

	std::string m_Filepath; // m_Filepath is used here only for debugging purposes
	unsigned int m_RendererID; // refer to notes on EP13-15 regarding why is it called m_RendererID // in this case m_RendererID is the ID of the shader programs. 
	std::unordered_map<std::string, int> m_UniformLocationCache; // caching for uniforms

public:

	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Setting uniforms
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:

	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
};

