#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream> // file stream
#include <string> // string stream
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"


int main()
{
	GLFWwindow* window;

	/* Initialises the library */
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

	// Vertex Array Object (VAO) and Vertex Buffer Object (VBO) and Index Buffer Object (IBO)
	VertexArray*  va = new VertexArray();
	VertexBuffer* vb = new VertexBuffer(positions, (4 * 2) * sizeof(float));
	IndexBuffer*  ib =  new IndexBuffer(indices, 6);

	VertexBufferLayout* layout = new VertexBufferLayout();
	layout->Push<float>(2);
	va->AddBuffer(*vb, *layout);


	Shader* shader = new Shader("res/shaders/Basic.shader");
	shader->Bind();
	shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
	
	shader->Unbind(); // GLCall(glUseProgram(0); 
	va->Unbind();     // GLCall(glBindVertexArray(0));
	vb->Unbind();     // GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	ib->Unbind();     // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	Renderer* renderer = new Renderer();

	float r = 0.0f;
	float increment = 0.01f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer->Clear(); // GLCall(glClear(GL_COLOR_BUFFER_BIT));
		renderer->Draw(*va, *ib, *shader); // this now bind the VAO, IBO and Shader

		// uniform is set per draw call, unlike vertex attributes which are set per vertex. 
		shader->SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);


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

	delete renderer;
	delete shader;
	delete va;
	delete vb;
	delete layout;
	delete ib;

	glfwTerminate();
	return 0;
}