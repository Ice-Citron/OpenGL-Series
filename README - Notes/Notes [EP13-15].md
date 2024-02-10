-------------------------------------------------------------------------------------

Episode 13 - Abstracting OpenGL into Classes


In the IndexBuffer and VertexBuffer classes, why are their ID objects called "m_RendererID":

        1. OpenGL Object Identification: OpenGL assigns a unique numeric ID (an unsigned integer) to each object created, such as textures, shaders, vertex buffers, index buffers, vertex arrays, frame buffers, etc. This ID is essential for OpenGL to identify and manage these objects.

        2. Renderer-Agnostic Naming: The prefix m_ is commonly used in C++ to denote member variables. The term RendererID is chosen to keep the naming generic and renderer-agnostic. Although this series focuses on OpenGL, which uses numeric IDs for object management, the naming convention m_RendererID is selected to potentially accommodate other rendering APIs that follow a similar system of object identification. This approach helps in keeping the code more generic and adaptable to different rendering systems.

        3. Abstraction and Encapsulation: By naming the variable m_RendererID, The Cherno emphasizes the abstraction layer being built around OpenGL's low-level API. This abstraction simplifies the management of OpenGL objects within higher-level constructs, like classes for vertex buffers and index buffers. It encapsulates the OpenGL-specific details (like the numeric ID) within a class that represents a vertex buffer or index buffer more conceptually.

        4. Future-Proofing and Flexibility: The choice of a generic name like m_RendererID over something more OpenGL-specific (e.g., glBufferID) provides flexibility for future modifications or extensions of the rendering system. It allows for the possibility of integrating other graphics APIs into the framework without significant naming or conceptual conflicts.

    In summary, the decision to use m_RendererID for identifying index buffers and vertex buffers in the class abstraction is driven by the need for a generic, flexible, and renderer-agnostic naming convention. This approach facilitates the encapsulation of OpenGL-specific details within a higher-level abstraction, making the code more adaptable and maintainable, especially in contexts where multiple rendering systems might be considered.
