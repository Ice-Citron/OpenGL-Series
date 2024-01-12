-------------------------------------------------------------------------------------

Episode 10 - Dealing with Errors in OpenGL [Summary from GPT of the video]

Code Analysis

            #define ASSERT(x) if (!x) __debugbreak(); // Only works in debug mode. 
            #define GLCall(x) GLClearError();\
                            x;\
                            ASSERT(GLLogCall(#x, __FILE__, __LINE__))

-- Macro Definitions

    1. ASSERT Macro

            #define ASSERT(x) if (!x) __debugbreak();

    -- Personal Note: __debugbreak(); is a MSVC compiler specific function, it essentially triggers a breakpoint, when your in debugger window. This is really helpful, because now you don't need to plot down breakpoints manually when debugging. 
    -- Functionality: This macro checks if the condition x is false. If it is false, the macro triggers a breakpoint (__debugbreak()), which halts the execution of the program in the debugger. This is particularly useful during development and debugging.
    -- Usage: It's used in debug mode to assert that a certain condition holds true. If the condition fails, it stops the program execution immediately, allowing the developer to inspect the state of the program at the point of failure.
    
    2. GLCall Macro

            #define GLCall(x) GLClearError();\
                            x;\
                            ASSERT(GLLogCall(#x, __FILE__, __LINE__))

    -- Personal Note: This is now used to wrap around every single OpenGL specific methods, so that when a black screen, or error actually occurs, we would immeadiately be informed which line has caused this, and which method through ASSERT(GLLogCall(~~~)). Essentially, the idea is to provide more informative error codes for us.
    -- Functionality: This macro is a multi-step process for handling OpenGL calls:
        || GLClearError(): Clears any existing OpenGL errors. (By looping continuously, invoking until there is no more OpenGL errors. )
        || x: Executes the OpenGL function call.
        || ASSERT(GLLogCall(#x, __FILE__, __LINE__)): Checks for any OpenGL errors that occurred during the execution of x. If an error is found, it triggers a breakpoint. The GLLogCall function likely logs the error with details about the function call, the file, and the line number where the error occurred.
    -- Usage: This macro is used to wrap OpenGL function calls. It automates the process of clearing errors, executing the function, and checking/logging errors. The use of #x converts the function call x into a string, which is useful for logging. __FILE__ and __LINE__ are standard preprocessor macros that provide the current file name and line number, respectively.

-- Usage in OpenGL

    \\ Every OpenGL Call: The GLCall() macro is used to wrap every OpenGL function call. This ensures that all OpenGL calls are preceded by error clearing and followed by error checking and logging.
    \\ Error Handling: By using this macro, error handling becomes more streamlined and consistent. It helps in quickly identifying the source of errors and reduces the boilerplate code needed for error checking.
    \\ Debugging: The inclusion of ASSERT within GLCall makes debugging more efficient. If an error occurs, the program breaks at the exact point of failure, allowing for immediate inspection in the debugger.

-- Example Usage

            GLCall(glFunction(arguments)); // glFunction is an OpenGL function

    In this example, glFunction(arguments) is an OpenGL function call wrapped inside the GLCall macro. This approach ensures that any errors caused by glFunction are immediately caught and handled appropriately.

    By using these macros, OpenGL development becomes more robust, as it systematically checks for errors and provides valuable debugging information, making the process of identifying and fixing OpenGL-related issues much more efficient.

-------------------------------------------------------------------------------------

Episode 11: Uniforms in OpenGL

[\\] Below will essentially contatining notes from GPT, summarising what has happened throughout the video. 

-- Understanding Uniforms in OpenGL
    
    1. Definition of Uniforms

        // Overview: Uniforms are a type of variable used in shaders. They differ from attributes in that uniforms are set per draw call, not per vertex.
        // Comparison with Attributes: Attributes are associated with each vertex and are used to pass data like vertex coordinates, normals, colors, etc., to the vertex shader. In contrast, uniforms are used to pass data that is constant for all vertices in a single draw call, like transformation matrices or colors.
    
    2. Usage in Shaders

        // Setting Uniforms: Uniforms are set before each draw call, such as before glDrawElements() or glDrawArrays().
        // Shader Binding Requirement: To set a uniform, the relevant shader program must be currently active. This is done using glUseProgram(shaderProgram).
        // Example of Setting a Uniform: If you have a uniform defined in your shader as uniform vec4 u_Color;, you would set its value using OpenGL functions after binding the shader program.

    3. Retrieving Uniform Location

        // Function Used: glGetUniformLocation(shaderProgram, "u_Color")
        // Purpose: This function retrieves the location (or ID) of a specific uniform variable within a given shader program.
        // Usage: The location is used in subsequent calls to set the uniform's value.
    
    4. Setting Uniform Values

        // Function Used: glUniform4f(location, v0, v1, v2, v3)
        // Purpose: This function sets the value of a uniform variable. The example glUniform4f is used for setting a vec4 type uniform.
        // Parameters:
            -- location: The location/id of the uniform variable in the shader program obtained from glGetUniformLocation.
            -- v0, v1, v2, v3: The values to set for the vec4 uniform.
    
    5. Behavior of Unused Uniforms

        // Compilation Stripping: If a uniform is declared in a shader but not used, it may be stripped away during shader compilation. This means it won't have a location, and attempts to set it will have no effect.

    6. Synchronization with Vertical Sync (VSync)

        // Function Used: glfwSwapInterval(1)
        // Purpose: This function is used to synchronize buffer swaps with the monitor's vertical refresh rate (VSync).
        // Effect: It helps in reducing screen tearing and can be used to limit the frame rate.

    Practical Example

            // Activate the shader program
            glUseProgram(shaderProgram);

            // Retrieve the location of the uniform variable
            int location = glGetUniformLocation(shaderProgram, "u_Color");

            // Set the value of the uniform - Example, setting color to red. 
            glUniform4f(location, 1.0f, 0.0f, 0.0f, 1.0f); 

            // Perform the draw call
            glDrawElements(...);

            // Synchronize with VSync
            glfwSwapInterval(1);

    In this example, the shader program is first activated. Then, the location of the u_Color uniform is retrieved. The uniform is set to a red color using glUniform4f. After setting the uniform, the draw call is made. Finally, glfwSwapInterval(1) is used to synchronize with the monitor's refresh rate.

-------------------------------------------------------------------------------------