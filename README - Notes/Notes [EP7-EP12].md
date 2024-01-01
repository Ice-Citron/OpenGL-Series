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
        = This generates one buffer object name and stores it in the variable "buffer".
        // If want to generate more than 1 object, need to pass in array for "GLuint* buffers", which will return an array of Buffer IDs.

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

    - In OpenGL, the term "target" when used in relation to buffer objects, refers to the intended use of the buffer. It is a way to specify what kind of operations you intend to perform with the buffer or how the buffer will be used. When you bind a buffer with "glBindBuffer", the "target" parameter tells OpenGL the purpose of the buffer, which in turn informs how OpenGL will manage the buffer's data.

    - Here are some common buffer object targets:

        1. GL_ARRAY_BUFFER: Used for vertex attributes, such as vertex coordinates, normals, colors, or texture coordinates.
        2. GL_ELEMENT_ARRAY_BUFFER: Used for element index data. This is typically used for indexing vertices in conjunction with glDrawElements or similar functions.
        3. GL_PIXEL_PACK_BUFFER: Used for operations that read pixel data from the OpenGL framebuffer, such as glReadPixels, where the read data will be stored in the buffer.
        4. GL_PIXEL_UNPACK_BUFFER: Used for operations that write pixel data to the OpenGL framebuffer, such as texture updates with glTexSubImage2D.
        5. GL_UNIFORM_BUFFER: Used to store uniform data for shaders, which can be shared among multiple program objects.
        6. GL_TRANSFORM_FEEDBACK_BUFFER: Used to store vertex data captured during transform feedback operations.
        7. GL_COPY_READ_BUFFER and GL_COPY_WRITE_BUFFER: Used for copying data between buffers with functions like glCopyBufferSubData.

    - When you bind a buffer to a target, the OpenGL context records that binding, and subsequent calls that use that target will affect the bound buffer. Different targets allow for separating the uses of different buffers without the need to re-bind them constantly. For example, you can have one buffer bound to GL_ARRAY_BUFFER for vertex data and another buffer bound to GL_ELEMENT_ARRAY_BUFFER for index data at the same time.

    - The target parameter is a powerful part of OpenGL's design, as it provides a generic mechanism for managing various types of data on the GPU in a stateful manner.

-------------------------------------------------------------------------------------