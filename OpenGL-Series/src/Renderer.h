#pragma once

#include <GL/glew.h>

// MSVC specific function. __ means that its compiler intrinsic. This essentially inserts a breakpoint whenver an error is encountered. 
#define ASSERT(x) if (!x) __debugbreak(); // Only works in debug mode. 
#define GLCall(x) GLClearError();\
				x;\
				ASSERT(GLLogCall(#x, __FILE__, __LINE__))
				//'#x' converts x into a string // __FILE__ and __LINE__ is supported by all compilers, unlike __debugbreak(), which is only for MSVCs

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);


class Renderer {

};

