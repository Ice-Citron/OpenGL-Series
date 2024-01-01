# OpenGL-Series
This is my repository, to track down, and record everything that I have done, from following Cherno's 31 video OpenGL series. The goal is to deepen my understanding in the OpenGL specification, before I move forward towards finishing my Hazel engine for my CAS project.

Below will contain my notes regarding OpenGL, so that I can refer back to them in the future.

-------------------------------------------------------------------------------------
Episode 4: Vertex Buffers and Drawing a Triangle in OpenGL


Vertex Buffers (VBOs) and Vertex Arrays (VAOs) in OpenGL
    
--- Vertex Buffer Objects (VBOs)
    
    - Purpose: Store vertex data (like positions, colors, texture coordinates) in GPU memory.
    - Usage:
      1. Create and bind a VBO.
      2. Upload vertex data to the VBO from the CPU.
      3. Data in VBOs can be rendered directly by the GPU.
    - Advantage: Reduces CPU-GPU data transfer, enhancing rendering performance.
    
--- Vertex Array Objects (VAOs)
    
    - Purpose: Store the state/configuration for rendering the vertex data. They reference VBOs but do not store vertex data themselves.
    - Usage:
      1. Create and bind a VAO.
      2. Configure how to interpret the vertex data in VBOs (like attribute layout, stride).
      3. Bind the VAO when drawing to use its configuration.
    - Advantage: Simplifies rendering calls by storing vertex attribute configurations, reducing repetitive state setup.
    
--- Workflow
    
    - Create VBOs: Store actual vertex data in GPU memory.
    - Set Up VAOs: Define how the vertex data in VBOs should be interpreted for rendering.
    - Render: Bind the necessary VAO and issue draw calls. The VAO state guides how OpenGL processes vertex data from VBOs.



OpenGL Shaders

- Definition: Small programs written in OpenGL Shading Language (GLSL) that run on the GPU. They are used to control various stages of the graphics rendering pipeline in OpenGL.

--- Types of Shaders

    1. Vertex Shaders:
       - Process each vertex of 3D geometry.
       - Handle tasks like vertex transformations and lighting calculations.
    
    2. Fragment Shaders (Pixel Shaders):
       - Run for each pixel to be rendered.
       - Determine pixel color, texture mapping, and lighting effects.
    
    3. Geometry Shaders:
       - Process entire primitives (e.g., triangles, lines).
       - Capable of adding or modifying vertices.
    
    4. Tessellation Shaders:
       - Control subdivision of geometry.
       - Used for detailed surface representation.

--- How They Are Used

    -- Written in GLSL: Shaders are coded in GLSL, a C-like language specifically for graphics programming.
    -- Compilation and Linking:
       4.1 Shaders are compiled.
       4.2 Linked into a shader program.
    -- Execution:
       - Run on the GPU for efficient graphics processing.
       - Vertex and fragment shaders are essential, while geometry and tessellation shaders are optional.

--- Usage in Rendering Pipeline

    -- Control Over Rendering: Provide programmability at various stages of the rendering pipeline, allowing custom rendering effects.
    -- Shader Programs: Multiple shaders are combined into a shader program, which is then used to render graphics.

--- Benefits

    -- Flexibility: Custom control over the appearance of 3D scenes.
    -- Performance: Execution on the GPU offers high performance, especially for complex rendering tasks.



Binding and Unbinding in OpenGL

-- Binding in OpenGL

   -- Definition: Setting an object as the current active object of its type.
   -- Process:
     1. Bind an object to a specific target (like a texture to GL_TEXTURE_2D).
     2. Subsequent OpenGL calls affect this bound object.
   -- Uses:
     1. Texture Binding: Apply a texture to geometry.
     2. Buffer Binding: Prepare buffers for drawing operations.
     3. Shader Program Binding: Activate shaders for rendering.

-- Unbinding in OpenGL

   -- Definition: Detaching an object from its target, effectively deactivating it.
   -- Process:
     1. Unbind by setting the target to 0 or NULL.
     2. Indicates no object of that type should currently be active.
   -- Uses:
     1. Prevent Side Effects: Avoid unintended operations on objects.
     2. Resource Management: Free up resources and improve performance.

-- Binding and Unbinding Workflow

   -- Bind: Activate an object (e.g., texture, buffer, shader program) for use.
   -- Operate: Perform the desired operations while the object is bound.
   -- Unbind: Deactivate the object to prevent accidental modifications.

-- Importance

   -- State Management: Essential for managing OpenGL’s state-centric architecture.
   -- Error Prevention: Helps avoid bugs by ensuring correct object usage.
   -- Performance: Efficient binding and unbinding can optimize rendering performance.

-------------------------------------------------------------------------------------
Episode 5: Vertex Attributes and Layouts in OpenGL


glVertexAttribPointer Method Overview

         void glVertexAttribPointer(	
            GLuint index,
            GLint size,
            GLenum type,
            GLboolean normalized,
            GLsizei stride,
            const void * pointer
         );
 
-- Purpose: Configures the format and layout of vertex attributes in a vertex buffer.
-- Usage: Typically used in the setup phase of OpenGL rendering to specify how vertex data is organized for rendering.

-- Parameters
   1. index: Attribute index in the shader.
      Specifies which attribute (e.g., position, color) to configure.
   2. size: Number of components per attribute.
      Common values: 2, 3, 4 (e.g., 3 for a 3D position vector).
   3. type: Data type of each component.
      E.g., GL_FLOAT for floating-point values.
   4. normalized: Whether to normalize data.
      GL_TRUE or GL_FALSE (use GL_TRUE for normalized data).
   5. stride: Byte offset between consecutive attributes.
      Set to 0 for tightly packed data, otherwise size in bytes of a complete vertex.
   6. pointer: Offset of the attribute in the buffer.
      Specifies where the data for the attribute begins in the buffer.

-- Example
        // Assuming position data is at index 0 and texture data is at index 1 in the shader
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
   - This setup is crucial for efficient GPU rendering, ensuring correct data interpretation per vertex.



Index Buffer Overview

- Definition: An index buffer is a memory buffer in graphics programming used to optimize rendering by reusing vertices.

-- How It Works:

    1. Stores indices pointing to vertices in a vertex buffer.
    2. Reduces redundancy by allowing multiple polygons to share vertices.
    3. Enhances efficiency, reducing memory and processing overhead.

-- Example:

    - Without Index Buffer: For a square (2 triangles), vertices A, B, C, D would be defined twice: A, B, C, B, C, D.
    - With Index Buffer: Define unique vertices A, B, C, D once, and use indices to define triangles: 0, 1, 2, 1, 2, 3 (assuming A=0, B=1, C=2, D=3).

-- Usage:

    - Common in 3D graphics for optimizing mesh rendering.
    - Integral in APIs like OpenGL, DirectX for defining mesh topology.

-- Benefits:

    - Reduces the amount of vertex data.
    - Decreases bandwidth usage between CPU and GPU.



Vertex Attributes in Graphics Programming

- Definition: Vertex attributes are properties of vertices in a 3D mesh, used in rendering pipelines to define the geometry and appearance of 3D models.

-- Key Attributes:

    1. Position: Coordinates in 3D space (x, y, z).
    2. Color: RGBA values for vertex coloring.
    3. Texture Coordinates (UVs): Mapping to texture images.
    4. Normals: Vectors for lighting calculations.
    5. Tangents and Bitangents: For advanced lighting techniques like normal mapping.
    6. Custom Attributes: Any additional data required by shaders.

-- Usage:

    - Processed by vertex shaders for transformations and lighting.
    - Essential for defining the shape and detail of 3D objects.

-- Impact:

    - Directly influences the visual output in 3D rendering.
    - Enables complex effects and realistic representations in 3D graphics.



Layout in Graphics Programming 

In graphics programming, a "layout" typically refers to the arrangement and organization of data in memory, particularly how vertex attributes are stored and accessed in a vertex buffer. This concept is crucial for ensuring that the graphics processing unit (GPU) correctly interprets the vertex data sent by the central processing unit (CPU). Here are the key aspects of layouts in graphics programming:

-- Definition

    1. Vertex Attributes: These are the properties of each vertex, like position, color, normal, texture coordinates, etc.
    2. Vertex Buffer: This is a memory buffer containing the vertex data.

-- Layout Specification

    1. Attribute Format: Specifies the type of each attribute (e.g., float, int) and whether the data is normalized.
    2. Attribute Size: Defines the number of components per attribute. For example, a position might have 3 components (x, y, z), while a color might have 4 components (red, green, blue, alpha).
    3. Stride: The size (in bytes) of each vertex in the buffer. It determines the byte offset between consecutive vertices.
    4. Offset: The byte offset of an attribute within a vertex. For example, if a vertex consists of a position (3 floats) followed by a color (4 bytes), the color attribute would have an offset of 12 bytes (the size of 3 floats).

-- Importance in the Graphics Pipeline

    1. Shader Compatibility: The layout must match the expectation of the vertex shader. If the shader expects a certain attribute at a certain location or index, the layout must ensure that this attribute is provided correctly.
    2. Memory Efficiency: Properly designed layouts can optimize memory usage and data transfer from the CPU to the GPU.
    3. Performance: Efficient layouts can also have a positive impact on rendering performance by optimizing memory accesses and reducing bandwidth usage.

-- Application

    In OpenGL, layout specification is often done using layout qualifiers in GLSL (the shader language) and functions like glVertexAttribPointer to describe the vertex buffer layout.

-------------------------------------------------------------------------------------

Episode 6: How Shaders Work in OpenGL


Summary of Main Points discussed

    1. Introduction to Shaders: The video begins with an introduction to shaders in OpenGL, explaining their significance in graphics programming. Shaders are programs that run on the GPU (Graphics Processing Unit) and are essential for instructing the GPU on how to render graphics.
    2. Vertex and Fragment Shaders: The focus is primarily on two types of shaders: vertex shaders and fragment shaders (also known as pixel shaders). These are the most commonly used shaders in graphics programming
    3. Role of Vertex Shaders: Vertex shaders are responsible for determining the position of vertices on the screen. They process vertex attributes (like positions specified in a buffer) and are crucial for defining how shapes like triangles are positioned and transformed in a scene.
    4. Role of Fragment Shaders: Fragment shaders, on the other hand, determine the color of each pixel. They are called for every pixel that needs to be rendered and are essential for coloring and shading the graphics.
    5. Performance Considerations: The video highlights the performance implications of vertex and fragment shaders. Since fragment shaders are called for each pixel, operations in them can be more computationally expensive compared to vertex shaders
    6. Shader Programming: Shaders are written as code (like GLSL - OpenGL Shading Language) and compiled and linked like any other program, but they run on the GPU.
    7. OpenGL Pipeline and Shaders: The video touches upon the OpenGL pipeline, explaining how shaders fit into the rendering process. The pipeline includes stages like issuing a draw call, running vertex shaders, and then fragment shaders to produce the final image on the screen.
    8. Shader Generation in Game Engines: The video also mentions that many game engines generate shaders dynamically based on the game's state and graphics settings.
    9. OpenGL State Machine and Shaders: Finally, it's explained that OpenGL works as a state machine, and shaders are part of this. When drawing, for example, a triangle, the appropriate shader is enabled, and necessary data is sent to it.



My Personal Research: Difference between Vertex Attribute Arrays and Vertex Array Objects

    In OpenGL, vertex attribute arrays and Vertex Array Objects (VAOs) are related but not the same thing. They serve different roles in the process of sending vertex data to the GPU.

-- Vertex Attribute Arrays
    
    - Purpose: They are part of the vertex data specification. Vertex attribute arrays refer to the individual arrays of vertex data, like position, color, normals, texture coordinates, etc.
    - Use: You enable and define vertex attribute arrays using functions like glEnableVertexAttribArray and glVertexAttribPointer.
    - Functionality: They define how the data for each vertex attribute is stored and accessed within a buffer (like format, type, stride, and offset).
    
-- Vertex Array Objects (VAOs)
    
    - Purpose: A VAO is an OpenGL object that stores the state of all vertex attribute arrays and the associated vertex buffer objects (VBOs).
    - Use: You create a VAO with glGenVertexArrays, and bind it with glBindVertexArray.
    - Functionality: When a VAO is bound, all subsequent calls to glVertexAttribPointer, glEnableVertexAttribArray, and glDisableVertexAttribArray are stored in the VAO. This makes it easy to switch between different sets of vertex data and attribute configurations. When you want to draw an object, you simply bind the appropriate VAO.
    
-- Key Difference
    
    - Vertex Attribute Arrays are about the data and its structure, defining what the data represents and how it's laid out.
    - VAOs are about encapsulating and recalling the state related to vertex attribute arrays and VBOs. They are essentially a "snapshot" of the configurations you've set up for rendering, allowing for efficient switching between different vertex data setups.

    -- In summary, vertex attribute arrays define the structure of your vertex data, while VAOs are used to store and recall configurations for rendering that data.



Rendering Pipeline of OpenGL

    - The OpenGL rendering pipeline can be detailed through various stages, each involving specific OpenGL functions and methods. Here's an overview of the OpenGL pipeline stages along with associated methods:

    1. Application Stage (CPU Side)

        - Functionality: Scene setup, game logic, and issuing draw commands.
        - OpenGL Methods:
            - glutMainLoop(), glfwPollEvents(): For handling window and input events.
            - Scene graph management, animation calculations, etc. (not directly part of OpenGL but crucial for preparing what to render).

    2. Vertex Specification

        - Functionality: Defining the shape and basic attributes of each object.
        - OpenGL Methods:
            = glGenBuffers(), glBindBuffer(): For creating and binding Vertex Buffer Objects (VBOs).
            = glVertexAttribPointer(): Specifies the layout of vertex data.
            = glEnableVertexAttribArray(): Enables vertex attribute arrays.
            = glGenVertexArrays(), glBindVertexArray(): For creating and binding Vertex Array Objects (VAOs).

    3. Vertex Shading

        - Functionality: Processing each vertex individually (e.g., transforming vertex positions, lighting calculations).
        - OpenGL Methods:
            = Vertex shader in GLSL: Custom shader code for transforming vertices.

    4. Primitive Assembly

        - Functionality: Assembling vertices into primitives (e.g., triangles).
        - OpenGL Methods:
            = Implicit in draw calls like glDrawArrays() or glDrawElements().
   
    5. Rasterization

        - Functionality: Converts primitives into fragments for each pixel they cover.
        - OpenGL Methods:
            = Controlled by the OpenGL context, not directly exposed through methods. Settings like viewport dimensions set with glViewport().
    
    6. Fragment Shading

        - Functionality: Determines the color and other attributes of each fragment.
        - OpenGL Methods:
            = Fragment shader in GLSL: Custom shader code for calculating fragment colors, textures, and lighting.
    
    7. Depth and Stencil Testing

        - Functionality: Tests to determine if a fragment should be discarded.
        - OpenGL Methods:
            = glEnable(GL_DEPTH_TEST), glDepthFunc(): For depth testing.
            = glEnable(GL_STENCIL_TEST), glStencilFunc(), glStencilOp(): For stencil testing.
    
    8. Blending and Final Image

        - Functionality: Combining fragments with the frame buffer to produce the final pixel colors.
        - OpenGL Methods:
            = glEnable(GL_BLEND), glBlendFunc(): For blending operations.
            = Writing to the frame buffer (implicitly done by the pipeline).
    
-- Additional Features

    - Tessellation and Geometry Shaders: Optional shader stages for further processing.
        = Controlled via GLSL tessellation and geometry shaders.
    - Compute Shaders: For non-graphics computations.
        = Implemented with compute shaders in GLSL.
    
-- Post-Processing

    Not a direct part of the standard OpenGL pipeline but often implemented with additional rendering passes (using frame buffer objects).

-- This overview simplifies a complex process and assumes familiarity with GLSL for shader programming. Each OpenGL project might use these stages differently depending on its specific requirements and the OpenGL version.

-------------------------------------------------------------------------------------