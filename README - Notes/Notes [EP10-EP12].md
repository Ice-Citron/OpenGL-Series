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
        || ASSERT(GLLogCall(#x, __FILE__, __LINE__)): Checks for any OpenGL errors that occurred during the execution of x. If an error is found, it triggers a breakpoint. The GLLogCall function logs the error with details about the function call, the file, and the line number where the error occurred.
    -- Usage: This macro is used to wrap OpenGL function calls. It automates the process of clearing errors, executing the function, and checking/logging errors. The use of #x converts the function call x into a string, which is useful for logging. __FILE__ and __LINE__ are standard preprocessor macros that provide the current file name and line number, respectively.

-- Usage in OpenGL

    \\ Every OpenGL Call: The GLCall() macro is used to wrap every OpenGL function call. This ensures that all OpenGL calls are preceded by error clearing and followed by error checking and logging.
    \\ Error Handling: By using this macro, error handling becomes more streamlined and consistent. It helps in quickly identifying the source of errors and reduces the boilerplate code needed for error checking.
    \\ Debugging: The inclusion of ASSERT within GLCall makes debugging more efficient. If an error occurs, the program breaks at the exact point of failure, allowing for immediate inspection in the debugger.

-- Example Usage

            GLCall(glFunction(arguments)); // glFunction is an OpenGL function

    In this example, glFunction(arguments) is an OpenGL function call wrapped inside the GLCall macro. This approach ensures that any errors caused by glFunction are immediately caught and handled appropriately.

    By using these macros, OpenGL development becomes more robust, as it systematically checks for errors and provides valuable debugging information, making the process of identifying and fixing OpenGL-related issues much more efficient.


[//] Further note (10/2/2024) -- You can run x; itself in GLCall(x);, because when an OpenGL function encounters an error, it typically does not halt the execution of your program. Instead, it will set an error flag in the OpenGL context's state machine, which can be queried later using glGetError(). This design allows your program to continue running even if an OpenGL error occurs, giving you the opportunity to handle the error as you see fit.


-------------------------------------------------------------------------------------

Episode 11: Uniforms in OpenGL

[\\] Below will essentially contatining notes from GPT, summarising what has happened throughout the video. 

-- Understanding Uniforms in OpenGL
    
    1. Definition of Uniforms

        // Overview: Uniforms are a type of variable used in shaders. They differ from attributes in that uniforms are set per draw call, not per vertex.
        // Comparison with Attributes: Attributes are associated with each vertex and are used to pass data like vertex coordinates, normals, colors, etc., to the vertex shader. In contrast, uniforms are used to pass data that is constant for all vertices in a single draw call, like transformation matrices or colors.
    
    2. Usage in Shaders

        // Setting Uniforms: Uniforms are set before each draw call, such as before glDrawElements() or glDrawArrays(). [understanding as of now: glDrawElements() is used when vertices are not in order, but you have an IBO index buffer object, to guide the state machine on what order to draw things // comparatively, glDrawArrays() expects everything to be in order, and draws the vertices in array sequentially]
        // Shader Binding Requirement: To set a uniform, the relevant shader program must be currently active. This is done using glUseProgram(shaderProgram).
        // Example of Setting a Uniform: If you have a uniform defined in your shader as uniform vec4 u_Color;, you would set its value using OpenGL functions after binding the shader program.

    <<>> Code Example:

            GLCall(int location = glGetUniformLocation(shader, "u_Color"));
            ASSERT((location != -1));
            GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

    3. Retrieving Uniform Location

        // Function Used: glGetUniformLocation(shaderProgram, "u_Color")
        // Purpose: This function retrieves the location (or ID) of a specific uniform variable within a given shader program.
        // Usage: The location is used in subsequent calls to set the uniform's value.
    
    4. Setting Uniform Values

        // Function Used: glUniform4f(location, v0, v1, v2, v3)
        // Purpose: This function sets the value of a uniform variable. The example glUniform4f is used for setting a vec4 type uniform.
        // Parameters:
            -- location: The location/ID of the uniform variable in the shader program obtained from glGetUniformLocation.
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


-- Notes on glClear(GL_COLOR_BUFFER_BIT);

    Code:

            GLCall(glClear(GL_COLOR_BUFFER_BIT));

    // is an OpenGL function call that clears the color buffer of the current framebuffer. When this function is executed, it clears the window to the color that was previously set by glClearColor.

    // Here's a breakdown of what's happening:

        1. glClear: This is the OpenGL function used to clear buffers to preset values. It can clear the color buffer, depth buffer, stencil buffer, or any combination of these.

        2. GL_COLOR_BUFFER_BIT: This is a constant passed to glClear that specifies which buffer to clear. In this case, it indicates that only the color buffer should be cleared. If you wanted to clear the depth buffer as well, you would use glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);.

    // When the glClear function is called with the GL_COLOR_BUFFER_BIT, it sets the entire color buffer to the same value, which is the color set by glClearColor (if glClearColor has been called before). This is typically done at the beginning of each frame in the rendering loop to reset the color buffer and prepare for a new set of rendering commands.

-------------------------------------------------------------------------------------

-- Notes on how does OpenGL render objects using VBOs (Vertex Buffer Objects) and IBOs (Index Buffer Objects) despite they have been unbinded. 

    || Regarding how is VBO retained till rendering
    || Here's the sequence of events that occur when you set up and use a VAO and VBO:

        1. Bind VAO: When you bind a VAO, you tell OpenGL that you're going to store vertex attribute states in this VAO.

        2. Bind VBO and Set Vertex Attribute Pointers: Binding the VBO and then calling glVertexAttribPointer while a VAO is bound causes the VAO to store the relationship between the vertex attributes and the currently bound VBO. The VAO keeps track of which buffer to use for the attributes and how to interpret the data within that buffer.

        3. Unbind VBO: After setting up the vertex attribute pointers, you can unbind the VBO. This is because the VAO still retains the information about which VBO was bound and how to interpret its data. The VBO is not actually detached from the VAO; only its binding to the target GL_ARRAY_BUFFER is removed.

        4. Unbind VAO (Optional): You can unbind the VAO to prevent accidental modification of its state. The VAO retains all its configurations.

        5. Rendering: When you want to render, you bind the VAO again. The VAO tells the GPU which VBOs to use and how to interpret the data in them for rendering. The GPU uses this information to fetch vertex data directly from the VBOs in its VRAM.

    || In summary, once configured and bound to a VAO, the VBOs don't need to be bound again for rendering as long as you use the same VAO. The GPU knows what to render because the VAO has stored all the necessary state, including which VBOs to use. This stored state points the GPU to the correct locations in VRAM where the vertex data resides.

    || Regarding how is IBO retained till rendering

        1. Creation and Binding: When you create an IBO and bind it with glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);, you are specifying that this buffer object will be used for element indices during drawing operations.

        2. Data Storage: By calling glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, usage); you are uploading the actual index data from your CPU memory to the GPU's VRAM. The indices is the array containing the indices, size is the size of this array in bytes, and usage hints at how the data will be accessed (e.g., GL_STATIC_DRAW for data that changes infrequently).

        3. State Saving in VAO: Unlike the vertex attributes which are linked to the VAO when glVertexAttribPointer is called, the association of an IBO with a VAO happens when you bind the IBO while a VAO is bound. The VAO stores the binding of the IBO, so you do not need to bind the IBO again as long as you use the same VAO.

        4. Unbinding IBO (Optional): After you have bound the IBO while the VAO is active, the VAO retains the information about the IBO. You can then unbind the IBO if desired. The VAO still knows which IBO to use when it's time to draw.

        5. Drawing: When issuing a draw call with glDrawElements, OpenGL uses the index data from the IBO currently bound to the VAO to determine the order in which to draw the vertices. Since this data is already in VRAM, the GPU can efficiently access and use it to render the geometry specified by the indices.

    || In summary, the IBO is sent to the GPU's VRAM and its binding is stored in the VAO. This allows the GPU to use the indices to access vertex data efficiently during rendering operations.

[\\] (10/2/2024) Further notes regarding how VAAs and IBOs work in the same VAOs, if they are not renewed. Essentially, VAAs and IBOs once declared and binded, will be stored inside the VAO state machine. So, unless the VAA or IBO is redeclared, they will remain the same inside the VAO state machine, and they will be applied to future VBOs that's declared. 

    -- Prompt:

        what i meant was, if you want to use the same vertex attribute pointers as previously, if you don't set new VAA, would this result in the previously setted VAA to be applied to this new VBO? 

        and i am wondering whether is this same logic also applies to new VBOs too, in which, if i don't explicitly declare a new IBO, would the previous IBO be used?

        all of the above is assuming that a VAO is already bound throughout and isnt changed

    -- Response: 
    
        || Ah, I understand your question now. If a VAO is already bound and you bind a new VBO without setting new vertex attribute pointers, the new VBO will use the vertex attribute pointers (VAA) that were previously set while the VAO was bound. This is because the VAA state is stored within the VAO and doesn't change just because you bind a new VBO.

        || Similarly, if you bind a new VBO and don't bind a new IBO (assuming an IBO was already bound while this VAO was bound), then the previously bound IBO will be used for indexed drawing commands like glDrawElements.

        || Here’s a step-by-step breakdown:

            1. Bind VAO: You bind a VAO to start recording state.
            2. Bind VBO and Set VAA: You bind a VBO and call glVertexAttribPointer to set up the vertex attribute pointers. This state is now saved in the currently bound VAO.
            3. Bind IBO: You bind an IBO. This is also now part of the VAO's state.
            4. Bind New VBO (No New VAA): If you bind a new VBO and do not call glVertexAttribPointer again, the previously set vertex attribute pointers from the VAO will still be in effect. This means the new VBO must have the same vertex layout as what was defined in the VAA.
            5. No New IBO: If you don't bind a new IBO, the VAO will use the one that was previously bound for any glDrawElements calls.

        || This behavior allows you to switch between VBOs easily, as long as they share the same vertex layout, without having to redefine the vertex attribute pointers. It's common in applications where you have multiple VBOs with the same vertex structure but different vertex data (like different meshes with the same attributes).

        || So, under a bound VAO, the last VAA setup and IBO binding will persist and apply to subsequent VBOs and draw calls unless explicitly changed.

[\\] Notes on State Machines

    -- In game development, especially within a game engine, the concept of state machines is indeed pervasive and can be observed at multiple layers of abstraction, from low-level rendering systems like OpenGL to high-level gameplay mechanics. The use of state machines is a fundamental design pattern that helps manage complexity by defining a finite set of states and the conditions under which transitions occur between these states.

    || Levels of State Machines in Game Engines
        
        1. Rendering System: At the rendering level (e.g., using OpenGL), the state machine concept is used to manage the current context, including settings for rendering, active textures, shaders, buffers, and more. OpenGL itself is often described as a state machine where commands change the state of the context, and subsequent commands operate within that state until it is changed again.

        2. Gameplay Systems: Higher up, game engines use state machines to manage game states (menu, in-game, pause, etc.) and transitions between them. This allows for clear separation of concerns and easier management of what should happen when the game's state changes.

        3. Characters and Entities: Individual entities or characters in a game can also have their state machines, managing states like idle, walking, attacking, etc. This is particularly useful in AI and animation systems, where an entity's behavior and appearance might change drastically based on its current state.

        4. UI and Interaction: UI systems may use state machines to manage different screens (main menu, options, game over screens) and interactions within each screen, providing a structured approach to UI development and management.

    || Visualizing the Structure
    
        ~~ Visualizing this as a massive OOP hierarchy with interlinked objects is quite apt. In object-oriented programming (OOP), each object or component might encapsulate its state machine, handling its internal state transitions based on inputs or events. These objects/components can then interact with each other, forming a complex web of state-driven behavior that constitutes the game's logic and rendering pipeline.

        ~~ Hierarchical State Machines: Sometimes, state machines are hierarchical, with higher-level state machines managing general states and lower-level machines handling more detailed states. For example, a character might have a high-level state machine for overall behavior (combat, exploration) and lower-level state machines for managing animation states within each behavior.

        ~~ Component-Based Architecture: Many modern game engines use a component-based architecture, where entities in the game are composed of various components, each potentially having its state machine. This approach provides modularity and flexibility, allowing for complex behaviors to be built up from simpler, reusable components.

    In summary, state machines are a core concept in game development, used to manage complexity and define clear, manageable behaviors across different systems within a game engine. Visualizing them as part of an OOP hierarchy or a network of interacting components is a useful way to understand the overall architecture of a game and its engine.

-------------------------------------------------------------------------------------

[//] Note on OpenGL shader parse code.

            unsigned int id = glCreateShader(type); 
            const char* src = source.c_str(); // converts source code to C-style string, because needed for OpenGL methods.

            // This line sets the source code (src) for the shader object specified by id. The 1 indicates that only one string is being passed, and the nullptr indicates that 
            // the string is null-terminated.
            GLCall(glShaderSource(id, 1, &src, nullptr));
            GLCall(glCompileShader(id)); // Compiles the shader source code associated with the shader object id.

    The function glShaderSource takes the source code for a shader and associates it with a shader object, identified by the GLuint id that you obtained from glCreateShader.

    Breakdown of "glShaderSource" parameters:

        == id: This is the shader object's identifier, obtained from glCreateShader. It represents the shader you're currently working with.
        == 1: This specifies the number of strings in the array you're passing as the source code. In this case, you have just one string (src), so the number is 1.
        == &src: This is a pointer to the array of source strings. Since you have only one string containing the entire shader source code, you pass the address of src, which is a pointer to your source code string.
        == nullptr: This argument specifies an array of string lengths. Passing nullptr indicates that each string is null-terminated. This allows OpenGL to determine the length of each string automatically by searching for the null terminator. If you were passing strings that aren't null-terminated, you would instead provide an array of lengths here.

    << After you call glShaderSource, the source code in src is associated with the shader object id. The shader isn't compiled yet; it simply knows what its source code is. The actual compilation of the shader source code into a form that the GPU can execute happens when you call glCompileShader(id). This compiles the source code that was previously supplied to the shader object.

    << If the compilation is successful, the shader object id now contains the compiled shader. This compiled shader can then be attached to a shader program with glAttachShader and linked with other shaders to create an executable shader program that runs on the GPU.


[//] Notes regarding behaviour of IBO without VAO explicityly generated and enabled

    -- When there is no VAO being explicitly used, if you unbind the IBO with "GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));", because of the fact that the IBO is managed globally/the state machine instead of being stored in the VAO state machine, this means the IBO's state will not be retained. You can only unbind an IBO if you have already explicitly enabled a VAO. 
    -- Always Use VAOs: Modern OpenGL encourages (and in the core profile, requires) the use of VAOs to manage vertex input configurations. This not only helps with organizing your rendering setup but also with avoiding issues like the one you encountered. Even if your hardware and drivers allow you to get away with not using VAOs, it's best practice to always create and use them:

[//] More notes regarding OpenGL Global State Machine behaviour with binding and unbinding VAOs, regarding the binded || unbinded VBOs

    Question Prompt:
        i meant that, would the global opengl state machine still be able to see this unbinded old IBO if i switched to a new VAO. and also that, would the global state machine be able to see the old unbinded IBO if i unbinded the VAO itself. because if it cant access it, it will lead to a black screen like what occured in my case

    Prompt Answer:
        The OpenGL state machine's behavior regarding VAOs (Vertex Array Objects) and IBOs (Index Buffer Objects) can indeed be a bit nuanced, so let's clarify these specific scenarios:

        -- Switching to a New VAO and Global State Access to an Unbound IBO
        
        <<>> When you switch to a new VAO, the global OpenGL state regarding the currently bound IBO (as part of the GL_ELEMENT_ARRAY_BUFFER binding) does not automatically carry over from one VAO to another. Each VAO maintains its own element array buffer binding state (i.e., its associated IBO). Therefore:
        <<>> If you bind a new VAO, the global OpenGL state does not "see" the previously bound IBO through the new VAO unless you explicitly bind the IBO again while the new VAO is active.

        -- Unbinding the VAO and Global State Access to an Unbound IBO

            Unbinding a VAO (by binding to glBindVertexArray(0);) essentially makes the OpenGL context not use any VAO. In this state:

                <<>> The global OpenGL state regarding GL_ELEMENT_ARRAY_BUFFER (IBO) is separate from the VAO's IBO binding. If you unbound an IBO (glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);) before unbinding the VAO, and then unbind the VAO, there is no IBO bound in the global state.
                <<>> Drawing commands issued without a VAO bound will not have access to any IBO because the IBO binding is part of a VAO's state, not the global state (except in immediate mode or legacy OpenGL, which modern OpenGL with core profile deprecates).

        -- Implication for Rendering and the Black Screen Issue
        
            Given these details:

                <<>> If you experienced a black screen issue when you unbound an IBO and then switched VAOs or unbound the current VAO, it's because the IBO association is specific to each VAO. Unbinding the IBO without a VAO bound, or expecting an IBO binding to carry over to a newly bound VAO, would indeed lead to situations where indexed draw calls (glDrawElements) cannot access index data, resulting in rendering failures or a black screen.
                <<>> To ensure that your rendering uses the correct IBO, you must bind the IBO while the intended VAO is active. Each VAO remembers its own IBO, and switching VAOs or working without a VAO bound requires appropriate management of IBO bindings to maintain the correct rendering state.

        -- In summary, for correct rendering with VAOs and IBOs, always ensure that:

            1. The desired IBO is bound while the corresponding VAO is active.
            2. When switching VAOs, re-bind any IBOs as necessary for the new VAO.
            3. Understand that unbinding a VAO returns you to a state where the OpenGL context does not use VAO configurations, including IBO bindings, which can affect rendering outcomes if not managed correctly.

[////] IMPORTANT NOTE REGARDING VAO

    -- There has been a misunderstanding regarding how VAO works, whilst yes, it does retain the IBOs and VAAs etc, even after they are unbinded on the global state machine, but they are only binded and retained in the VAO state machine, under the circumstance of:

        Code for demonstration:

                GLuint vao;
                glGenVertexArrays(1, &vao);
                glBindVertexArray(vao);

                ***lines such as binding and setting up VBOs and IBOs

                glBindVertexArray(0);

                int main {
                    ***misc setup

                    glBindVertexArray(vao);
                    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);

                    ***miscellaneous...
                }

        1. After setting up the VAO and enabling it, if you want to disable IBOs or VAAs for example, you actually need to firstly unbind the VAO first, using "glBindVertexArray(0);", and then when you are going to draw again, as seen in the code above, you need to rebind the VAO, using glBindVertexArray(vao);, in such, the VAO state machine will provide with its previously "retained" IBOs and VAAs states, etc. 


-------------------------------------------------------------------------------------

Episode 12 - Vertex Arrays in OpenGL

[//] Notes regarding behaviour of glVertexAttribPointer 

    -- Essentially, glVertexAttribPointer is setup'd and linked to each vertex buffer. When you create a new VBO, you need to re-invoke a new set of glVertexAttribPointer, and tell the VAO State Machine what is the configuration of this new VBO. WARNING: Contrary to beliefs thus far, the VertexAttribArray settings don't actually carry over for all subsequent VBOs. 
        <<>> As mentioned in Notes [EP1-6], you can have multiple VAOs at the same time, for different VAAs configurations for different VBOs at the same time, enabling each VBO to have their own specific configuration at the same time without having to reconfigure them every time you want to use them. This is done by having multiple VAOs; binding a specific VAO; setting up a specific VBO; unbinding the specific VAO; repeat these few steps for each VBO. And now whenever you want to use a specific VBO with its configuration, just rebind the specific VAO, and you are free to proceed. 

    -- Prompt Notes:

        1. One Vertex Buffer per Call: The glVertexAttribPointer function is used to specify the layout of vertex attribute data in the currently bound vertex buffer. It associates a specific vertex attribute index (like position, color, texture coordinates, etc.) with the data format and layout in that buffer. This association is made for one vertex buffer at a time, per call. If you have multiple vertex attributes (e.g., position, color), you need to call glVertexAttribPointer for each attribute, specifying how each attribute's data is laid out in the buffer.

        2. Calling for Each New Vertex Buffer: If you create a new vertex buffer and want to use it for vertex attribute data, you need to bind this new buffer with glBindBuffer(GL_ARRAY_BUFFER, bufferID) and then call glVertexAttribPointer for each vertex attribute you want to associate with this new buffer. This step is necessary every time you switch to a different vertex buffer that you intend to use for vertex attributes.

        3. Vertex Attribute Array Association: The term "vertex attribute array" refers to the concept of enabling and specifying the layout of vertex attributes in the context of a Vertex Array Object (VAO). When you call glVertexAttribPointer, you're effectively telling OpenGL how to interpret the data within the bound vertex buffer for a specific attribute index. This setup is stored in the currently bound VAO. Therefore, a VAO can be thought of as a collection of these associations between vertex attributes and vertex buffers, along with the state needed to manage these attributes (like enabling vertex attribute arrays with glEnableVertexAttribArray).

        4. Efficiency with VAOs: By using VAOs, you can efficiently switch between different sets of vertex data and attribute layouts. You set up the associations between vertex attributes and buffers once per VAO, and then by binding a different VAO, you switch all those associations in one operation. This is much more efficient than binding buffers and specifying attribute pointers every time you want to draw something different.

    In summary, glVertexAttribPointer is indeed for specifying the layout of data in one vertex buffer per call, and you need to call it for each attribute for every new vertex buffer you intend to use for rendering. The associations made by these calls are stored in the VAO, allowing you to switch between different vertex data setups efficiently.



