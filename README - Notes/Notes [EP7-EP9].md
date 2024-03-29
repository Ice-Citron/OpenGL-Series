-------------------------------------------------------------------------------------

Episode 7: Writing a Shader in OpenGL


Vertex Shader and Fragment Shader

    1. Vertex Shader:

        - The vertex shader is the first programmable stage in the OpenGL rendering pipeline.
        - It processes each vertex of 3D geometry individually.
        - Its primary purpose is to transform vertex data (such as position, normal, and texture coordinates) according to the operations programmed into it.
        - Typical operations performed in a vertex shader include transforming the 3D coordinates of vertices into different spaces (like world space, view space, or projection space), lighting calculations, and computing texture coordinates.
        - Vertex shaders can also pass per-vertex data to fragment shaders, which can include lighting information, texture coordinates, or any other data relevant for rendering.

    2. Fragment Shader:

        - The fragment shader, also known as a pixel shader, operates on fragments. A fragment is a potential pixel with its associated data like color, depth, and texture coordinates.
        - It determines the final color and other attributes of each pixel.
        - Fragment shaders are used for more detailed and complex pixel-level operations, like texture mapping, color blending, lighting, and applying visual effects.
        - They compute the pixel's final color based on various factors, including the lighting calculations passed from the vertex shader, texture data, and other material properties.

    -- Both shaders are written in GLSL (OpenGL Shading Language) and are compiled and linked into the OpenGL program at runtime. This design allows for a great deal of flexibility and power in rendering, enabling developers to create a wide range of visual effects and transformations in 3D graphics.



glBindBuffer(GL_ARRAY_BUFFER, 0);

    The glBindBuffer(GL_ARRAY_BUFFER, 0); call in OpenGL effectively unbinds any buffer that was previously bound to the GL_ARRAY_BUFFER target. This is a way to clean up and ensure that no buffer is bound to GL_ARRAY_BUFFER anymore. It prevents any accidental modification to the buffer that was just used, as subsequent GL_ARRAY_BUFFER operations will have no effect until another buffer is bound.

    In the context of the code snippet you've provided, it looks like the buffer is being unbound after setting up vertex data and configuring vertex attribute pointers. This is a common practice to avoid inadvertently making changes to the buffer after it has been set up. It's essentially a safety measure to prevent bugs that could arise from unintended writes to a buffer that's no longer meant to be modified.


glGenBuffer() and glBufferData()

    - glGenBuffers and glBufferData are both OpenGL functions used for buffer object management.

 1. glGenBuffers(GLsizei n, GLuint* buffers);

    - Functionality: glGenBuffers generates buffer object IDs.
    - Usage: glGenBuffers(GLsizei n, GLuint *buffers);
        = "n" is the number of buffer object IDs to generate.
        = "buffers" is an array in which the generated buffer object names are stored.
    - What it does: When you call "glGenBuffers", it creates unique IDs (names) for buffer objects. These are not actual buffer objects yet, but rather IDs by which you can refer to buffer objects once you bind them.
    - Return Value: It doesn't return a value directly. Instead, it writes the generated IDs to the array you pass to it. In your code, "unsigned int buffer"; is a single unsigned int where the ID of the new buffer is stored.
    - Example from your code:
        = "unsigned int buffer;"
        = "glGenBuffers(1, &buffer);"
        = This generates one buffer object ID and stores it in the unsigned int variable "buffer".
        // If want to generate more than 1 object, change the "GLsize i" parameter and pass in array for "GLuint* buffers", which will return an array of Buffer IDs.

 2. glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);

    - Functionality: "glBufferData" creates and initializes a buffer object's data store.
    - Usage: glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
        = 'target' is the buffer object target. The target can be GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, etc., specifying the purpose of the buffer.
        = 'size' is the size in bytes of the buffer's new data store.
        = 'data' points to the data that will be copied into the data store. If data is NULL, the data store is still created, but the data initialization is undefined.
        = 'usage' is a hint to the GL implementation on how the data will be accessed. For example, GL_STATIC_DRAW indicates the data will not change often and is used to draw.
    - What it does: When you call 'glBufferData', it allocates memory for the buffer object currently bound to 'target' and initializes it with the data you provide. If you pass 'NULL' for the data, the buffer is created, but its contents are undefined (useful for later specifying the data).
    - Example from your code:
        = glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (3 * 2), positions, GL_STATIC_DRAW);
        = This binds the 'positions' array data to the previously created buffer object (with ID stored in 'buffer'), telling OpenGL how much data it needs to allocate, and providing a hint (GL_STATIC_DRAW) that this data will be set once and used many times for drawing operations.
    
    Together, these functions are used to allocate memory on the GPU and define how the memory will be used in terms of storage and access patterns. In the context of rendering, they are essential for uploading vertex data to the GPU so it can be used for rendering operations.



What is "target" in terms of OpenGL buffer objects.

    - In OpenGL, the term "target" when used in relation to buffer objects, refers to the intended use of the buffer. It is a way to specify what kind of operations you intend to perform with the buffer or how the buffer will be used. When you bind a buffer with "glBindBuffer(GL_ARRAY_BUFFER, buffer_uint_id);", the "target" parameter tells OpenGL the purpose of the buffer, which in turn informs how OpenGL will manage the buffer's data.

    - Here are some common buffer object targets:

        1. GL_ARRAY_BUFFER: Used for vertex attributes, such as vertex coordinates, normals, colors, or texture coordinates.
        2. GL_ELEMENT_ARRAY_BUFFER: Used for element index data. This is typically used for indexing vertices in conjunction with glDrawElements or similar functions. (eg. index buffer objects)
        3. GL_PIXEL_PACK_BUFFER: Used for operations that read pixel data from the OpenGL framebuffer, such as glReadPixels, where the read data will be stored in the buffer.
        4. GL_PIXEL_UNPACK_BUFFER: Used for operations that write pixel data to the OpenGL framebuffer, such as texture updates with glTexSubImage2D.
        5. GL_UNIFORM_BUFFER: Used to store uniform data for shaders, which can be shared among multiple program objects.
        6. GL_TRANSFORM_FEEDBACK_BUFFER: Used to store vertex data captured during transform feedback operations.
        7. GL_COPY_READ_BUFFER and GL_COPY_WRITE_BUFFER: Used for copying data between buffers with functions like glCopyBufferSubData.

    - When you bind a buffer to a target, the OpenGL context records that binding, and subsequent calls that use that target will affect the bound buffer. Different targets allow for separating the uses of different buffers without the need to re-bind them constantly. For example, you can have one buffer bound to GL_ARRAY_BUFFER for vertex data and another buffer bound to GL_ELEMENT_ARRAY_BUFFER for index data at the same time.

    - The target parameter is a powerful part of OpenGL's design, as it provides a generic mechanism for managing various types of data on the GPU in a stateful manner.

-------------------------------------------------------------------------------------

Pre-Episode 8: How I Deal with Shaders in OpenGL


Analysing VertexShader and FragmentShader from EP7's code


-- Vertex Shader

            std::string vertexShader = R"(
                #version 330 core
                
                layout(location = 0) in vec4 position;

                void main() {
                    gl_Position = position;
                }
            )";

    1. 'std::string vertexShader = R"()"; ': This is the start of a raw string literal in C++ which contains the code for the vertex shader.
    2. '#version 330 core': This specifies that the shader is using version 3.30 of GLSL in the core profile, which is a modern and portable version.
    3. 'layout(location = 0) in vec4 position;': This declares an input vertex attribute of type vec4 (a 4-component vector of floats). The 'layout(location = 0)' specifies that this attribute will be linked to the first attribute index of the vertex data passed to the GPU. (Needs to take in position/layout(location = 0) as vec4, because gl_Position is always a vec4 component // (x, y, z, w) -- xyz is used for 3D coordinates. w is used for perspective projection of vertex, eg, for orthographhic projections, 'w'==1.0).
    4. 'void main() {}': The entry point of the shader program.
    5. "gl_Position = position;": The main function sets the built-in gl_Position variable to the input position. This determines the position of the vertex in normalized device coordinates after all transformations.

-- Further Explanation (Vertex Shader's layout qualifier && gl_Position)

    [\\] Line: layout(location = 0) in vec4 position;

        || "layout(location = 0)": This part of the syntax is known as a layout qualifier. It tells the OpenGL shader program how the vertex data is organized. The "location = 0" part specifies the location index where this attribute can be found. When you create a vertex buffer and set up vertex attributes on the CPU side, you can bind your position data to this location so that the shader can access it. It's essentially an index to the position data in the buffer.

        || "in": This keyword declares "position" as an input to the vertex shader. Inputs to vertex shaders are usually the per-vertex data that come from vertex buffers, like positions, normals, texture coordinates, etc.

        || "vec4 position;": This declares a variable named "position" of type "vec4". A "vec4" is a four-component vector, and in this context, it's used to represent the position of a vertex in 3D space with an additional w-component for homogeneous coordinates. The components of this vector are typically (x, y, z, w).

    [\\] Line: gl_Position = position;
    
        || "gl_Position": This is a built-in GLSL variable that the vertex shader must set to tell OpenGL where the vertex is in clip space. Clip space is a coordinate system where OpenGL expects the coordinates to be between -1.0 and 1.0. Any coordinates that fall outside this range will be clipped (i.e., not drawn).

        || "= position;": This line of code assigns the value of the "position" input variable directly to "gl_Position". Since "position" is a "vec4", it directly matches the expected type of "gl_Position". This means that the vertex shader is outputting the vertex position unchanged from the input. It's not applying any transformation, like scaling, rotation, or translation, which you would see in a more complex shader that manipulates the object's shape or position in the scene.

    [\\] The vertex shader operates on each vertex of the geometry that you're drawing. When this shader runs, it takes the provided vertex positions, which are expected to be already in the correct coordinate space (since no transformation is being applied), and passes them through to the rasterizer, which will then interpolate these positions to find the positions of fragments/pixels on the screen. These fragments are then processed by the fragment shader to produce the final pixel colors.


-- Fragment Shader

            std::string fragmentShader = R"(
                #version 330 core

                layout(location = 0) out vec4 color;

                void main() {
                    color = vec4(1.0, 0.0, 0.0, 1.0);
                }
            )";

    1. "std::string fragmentShader = R"()"; ": Similar to the vertex shader, this is the start of a raw string literal in C++ which contains the code for the fragment shader.
    2. "#version 330 core": Again, this specifies that the shader is using version 3.30 of GLSL in the core profile.
    3. "layout(location = 0) out vec4 color; ": This declares an output variable color of type "vec4" which will be used to output the color of the fragment.
    4. void main() {} : The entry point of the shader program for the fragment shader.
    5. "color = vec4(1.0, 0.0, 0.0, 1.0); ": Inside the main function, it sets the color output to a "vec4" representing the color red with full opacity (RGBA: Red, Green, Blue, Alpha).


    \\ The vertex shader is responsible for transforming the vertices of a shape. In this case, it simply assigns the input position to the gl_Position without any transformation.

    \\ The fragment shader is responsible for defining the color of the pixels on the rendered shape. In this case, it colors every pixel it processes in red with full opacity.

    \\ Both shaders are fairly simple and form the most basic shader program possible, which will render a geometric shape in a solid red color.



Further Notes on Fragment Shader

-- Overview
    The fragment shader is a programmable unit in the OpenGL rendering pipeline that determines the final color and other attributes of each pixel (fragment) that will be rendered to the screen. It operates on each fragment generated by the rasterization of primitives (like triangles or lines) and has the responsibility of executing algorithms that define the visual characteristics of these pixels.

-- Usage
    Fragment shaders are used to perform complex per-pixel operations that can include:
    1. Computing colors based on lighting, texture data, and other factors.
    2. Texturing by sampling image data and applying it to pixels.
    3. Applying post-processing effects such as blurring or color correction.

-- Naming Conventions and Context
    
    The functionality of a fragment shader in OpenGL is determined not by the names of its variables but by their purpose and the context in which they're used. The layout(location = x) qualifier is pivotal, as it designates the corresponding framebuffer color attachment for the output variable. This allows for a flexible and descriptive coding style:

            // Output linked to the first color attachment
            layout(location = 0) out vec4 finalColor; 

    In the above line, finalColor is a user-defined name that serves as a clear identifier within the shader code. The association with the framebuffer's color attachment is established by the layout(location = 0).

-- Creative Uses and Advanced Techniques
    
    Fragment shaders are not limited to conventional rendering and can be used for generative art, procedural texturing, or even non-photorealistic rendering techniques. They give you pixel-level control over the final image, enabling a broad spectrum of visual effects. Here are some advanced uses:

    || Procedural Texturing: Creating patterns or textures on-the-fly within the shader itself, based on mathematical functions and algorithms.
    || Screen Space Effects: Implementing effects that rely on screen-space information, such as screen-space reflections, ambient occlusion, or depth of field.
    || Data Packing: Encoding multiple pieces of information into a single texture, which can then be unpacked and utilized within the shader for various effects.
    
    In essence, fragment shaders are a powerful tool in the OpenGL shading language that can be harnessed to perform simple tasks like texturing and coloring, as well as complex image processing and graphical effects. They are only limited by the imagination of the developer and the capabilities of the hardware.



Relationship between Vertex Attribute and Layout Qualifier (layout(location = 0))~

- In OpenGL, the location index for vertex attributes is determined by either explicitly specifying it in the shader code using the layout(location = x) qualifier or by querying and setting it programmatically from the OpenGL application code.

-Here's how you can determine and use the location index:

-- Explicit Specification in Shader

    You can explicitly specify the location index in your GLSL shader code using the layout(location = x) qualifier, where x is the index you choose. This is often used for better readability and control over shader inputs:

            layout(location = 5) in vec3 position;

    In this example, the vertex attribute position is assigned to location index 5. You are free to choose any index that suits your organization, as long as you are consistent between your shader code and the vertex attribute pointer setup in your OpenGL application code.

-- Programmatically Determining Location

    If you do not specify the location in the shader, you can query it in your OpenGL application using glGetAttribLocation:

            GLuint positionLocation = glGetAttribLocation(shaderProgram, "position");
            
    After this, you would use positionLocation when setting up your vertex attribute pointers:

            glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

-- Why Use Different Indices?
    
    The choice of index is somewhat arbitrary, and you can indeed use location = 5 for position if you want to. However, using sequential indices starting from 0 is a common convention because it can be more intuitive, especially when there are multiple attributes. It makes it easier to remember and manage the indices during the setup of vertex buffer objects (VBOs) and vertex array objects (VAOs).

-------------------------------------------------------------------------------------

// No notes for EP9. Index buffer episode.

// Final note [9/2/2024] - The OpenGL State Machine knows what is what in a VBO, in terms of its vertex attributes, is due to the shaders, for example the vertex shader, with "layout(location = 0) in vec4 position;" and "gl_Position = position;" tells OpenGL state machine what is what. 

// Further note - This is regarding the layout(location = 0) finalColor; in the fragment shader. 

    -- In a fragment shader, the layout(location = 0) out vec4 color; declaration specifies an output variable that will be used to write the final color of a fragment. This output is directed to a specific color buffer of the framebuffer, which, in most cases, is the buffer used for displaying the rendered image on the screen.

    -- Why Specify location = 0?
    
        The location qualifier in this context specifies which draw buffer the output is written to. When rendering to a framebuffer with multiple color attachments (for example, when performing off-screen rendering or when using advanced rendering techniques like deferred shading), you can have multiple outputs from the fragment shader, each writing to a different color attachment. The location index corresponds to the draw buffer index:

    \\ "location = 0" is the default and refers to the primary color buffer (often the only one used in simple rendering scenarios).
    \\ "location = 1", location = 2, etc., would refer to additional color buffers if your framebuffer configuration supports them.
    
    -- Can This Output Be Accessed?
        
        Yes, but how it is accessed depends on your rendering setup:

        \\ Default Framebuffer: For the default framebuffer (the screen), the output is directly visible as the rendered pixels.
        \\ Custom Framebuffers (FBOs): For custom framebuffers, the output can be accessed by binding the framebuffer as a texture in subsequent rendering operations or by reading the framebuffer data back to the CPU with functions like glReadPixels.
    
    -- Does the Order of Index Matter?
    
        Yes, the order of the index matters when you have multiple output variables in the fragment shader. The location index must match the intended draw buffer. For instance, if you're using a framebuffer with multiple color attachments for different purposes (e.g., one for color information, another for specular highlights), you need to ensure that your shader outputs match the framebuffer's attachment points. This means setting the correct location for each output variable.

    [] Example Use Case
    
        In a simple shader setup where you're only writing to one color buffer (the most common scenario for beginners and intermediate rendering tasks), you might not need to explicitly set the location if there's only one output. However, explicitly specifying layout(location = 0) out vec4 color; is considered good practice for clarity and for making your code more adaptable to future changes that might involve multiple outputs.

        In more complex shaders, especially those used in post-processing, deferred shading, or other advanced rendering techniques, you might have something like:

                layout(location = 0) out vec4 albedoColor;
                layout(location = 1) out vec4 normalData;
                layout(location = 2) out vec4 specularColor;

        Here, each output variable is associated with a different purpose, and the order (location) directly corresponds to how you've set up your framebuffer's color attachments.

[\\] Further prompt questions: i assume that by default, if i have not explicitly declared any framebuffer settings, openGL state machine would by default use the (location=0) output for colour?

    Response:

        == Yes, that's correct. In the absence of explicitly declared framebuffer settings to target multiple color attachments, the OpenGL state machine defaults to using the output variable of the fragment shader that is specified with layout(location = 0) for the color output. This output is directed to the default framebuffer's color buffer, which is typically the screen or the window surface that you're rendering to.

        == In simpler terms, when you write something like this in a fragment shader:

                #version 330 core
                layout(location = 0) out vec4 color;

        == And then don't explicitly configure multiple color attachments for a framebuffer, OpenGL automatically routes the data written to the color output variable to the default color buffer (which is the primary buffer used for display).

        == This behavior aligns with the most common rendering scenarios where only a single output is required for the fragment color, and thus, location = 0 is implicitly understood to be the target for the fragment's final color.

        == When using the default framebuffer (i.e., when rendering directly to the screen), there is typically only one color buffer active, and it is at location = 0. If you later decide to use Framebuffer Objects (FBOs) for off-screen rendering with multiple color attachments, you can then specify other locations (layout(location = N) out vec4 someOutput;) to write to other color buffers. These must be explicitly enabled and configured through OpenGL calls that attach textures or renderbuffers to the FBO's color attachment points.