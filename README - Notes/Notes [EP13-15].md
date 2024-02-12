-------------------------------------------------------------------------------------

Episode 13 - Abstracting OpenGL into Classes


In the IndexBuffer and VertexBuffer classes, why are their ID objects called "m_RendererID":

        1. OpenGL Object Identification: OpenGL assigns a unique numeric ID (an unsigned integer) to each object created, such as textures, shaders, vertex buffers, index buffers, vertex arrays, frame buffers, etc. This ID is essential for OpenGL to identify and manage these objects.

        2. Renderer-Agnostic Naming: The prefix m_ is commonly used in C++ to denote member variables. The term RendererID is chosen to keep the naming generic and renderer-agnostic. Although this series focuses on OpenGL, which uses numeric IDs for object management, the naming convention m_RendererID is selected to potentially accommodate other rendering APIs that follow a similar system of object identification. This approach helps in keeping the code more generic and adaptable to different rendering systems.

        3. Abstraction and Encapsulation: By naming the variable m_RendererID, The Cherno emphasizes the abstraction layer being built around OpenGL's low-level API. This abstraction simplifies the management of OpenGL objects within higher-level constructs, like classes for vertex buffers and index buffers. It encapsulates the OpenGL-specific details (like the numeric ID) within a class that represents a vertex buffer or index buffer more conceptually.

        4. Future-Proofing and Flexibility: The choice of a generic name like m_RendererID over something more OpenGL-specific (e.g., glBufferID) provides flexibility for future modifications or extensions of the rendering system. It allows for the possibility of integrating other graphics APIs into the framework without significant naming or conceptual conflicts.

    In summary, the decision to use m_RendererID for identifying index buffers and vertex buffers in the class abstraction is driven by the need for a generic, flexible, and renderer-agnostic naming convention. This approach facilitates the encapsulation of OpenGL-specific details within a higher-level abstraction, making the code more adaptable and maintainable, especially in contexts where multiple rendering systems might be considered.


-------------------------------------------------------------------------------------

Episode 14 - Buffer Layout Abstraction in OpenGL

[//] VAO, VBO, VAA, IBO have all been abstracted into their respective class objects in this episode, enabling easier usage, without user having to deal with low level OpenGL details. 

-- Notes regarding "Emplace Initialisation"

            std::vector<VertexBufferElement> m_Elements;
            m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
        or 
            m_Elements.emplace_back(GL_FLOAT, count, GL_FALSE);

    -- This process is known as "emplace" construction in the context of container classes, although you're using push_back here, which also supports this initialisation style 
    since C++11.

    -- The key points about this initialisation method are:

        1. Uniform Initialization: The use of curly braces {} provides a uniform syntax for initializing objects, whether they are aggregates, classes with constructors, arrays, etc.
        2. Direct Construction in Container: The object is constructed directly inside the container. This is more efficient than creating a temporary object and then copying or moving it into the container. Although you're using push_back in this example, methods like emplace_back are specifically designed to construct objects in place by forwarding their arguments to the constructor of the element type.
        3. No Explicit Constructor Call: You don't need to explicitly call the constructor or use new. The arguments inside the curly braces are passed to the constructor of the VertexBufferElement, matching the parameters to the constructor's signature.

    -- This technique is widely used for its simplicity and efficiency, especially when adding objects to containers without the overhead of extra copies or dynamic memory allocation (as with new).


[//] Random Note: You need to bind VBO before you start setting up its VAA which is bound to it. You CANNOT setup the VAA first then bind a VBO. 

    GPT4 Answer:

        In OpenGL, you must bind a VBO before you can set the vertex attribute pointers (the VAA) because the vertex attribute pointers refer to the memory in the currently bound VBO. The VAO remembers the vertex attribute configurations and which VBO is associated with those attributes. Here's the sequence you would typically follow:

            1. Bind the VAO.
            2. Bind the VBO.
            3. Set the vertex attribute pointers (VAA) with glVertexAttribPointer.
            4. Unbind the VBO if necessary.
            5. Unbind the VAO.

        So, to be clear, you cannot set up the VAA before any VBO is bound, because the VAA setup commands (glVertexAttribPointer, glEnableVertexAttribArray) reference the currently bound VBO's data storage to define how the vertex attributes should be read.

        However, once you have set up your VAA within a VAO, you can then bind different VBOs to the same VAO later on, as long as they conform to the expected layout defined by the VAA. Each VAO can have multiple VBOs associated with it, but only one VBO can be bound to a particular vertex attribute at any one time.

[//] VertexBufferLayout class - Template Meta-Programming method to prevent Generic type from being inserted. 

    -- The code you've provided shows a class that manages a VertexBufferLayout. This class appears to be designed for use with OpenGL, to define the layout of the vertex data that will be sent to the GPU. The use of templates and template specialization here is to provide a way to define vertex attribute data in a type-safe manner.

    -- Here's a breakdown of what is happening:

        <<>> Generic Template Method:
    
                    template<typename T>
                    void Push(unsigned int count) {
                        throw std::runtime_error("Invalid type used for VAA.");
                    }

            -- This is a generic template method that is supposed to be called with a specific type T. However, as written, it only throws a runtime error. This is likely a placeholder or a static assertion to prevent the method from being called with unsupported types.

        <<>> Template Specializations:

                    template<>
                    void Push<float>(unsigned int count) {
                        // Implementation for float
                    }
                    template<>
                    void Push<unsigned int>(unsigned int count) {
                        // Implementation for unsigned int
                    }
                    template<>
                    void Push<unsigned char>(unsigned int count) {
                        // Implementation for unsigned char
                    }

            -- These are template specializations for the Push method. Each specialization is tailored for a specific type (float, unsigned int, unsigned char). When you call Push with one of these types, the corresponding specialized method is used.

    ?? The specializations (The Push methods) perform two main tasks:

        {{}} They add a VertexBufferElement to the m_Elements vector. This vector holds the layout for the vertex attributes. Each element specifies the type of the attribute, the number of components, and whether the data should be normalized.
        {{}} They update the m_Stride member variable, which represents the total size (in bytes) of a single vertex by adding the size of the new attribute.

    Overview:  

        1. Understanding Template Specializations:
           The reason for the template specializations is that different types of vertex attributes (floats, integers, bytes) might have different ways they need to be handled. For example, the size of the type in the GPU memory (sizeof(T)) differs between types, and whether the data should be normalized may depend on what the data represents.

        2. Type-Safe Push Operations:
           By using template specialization, the class provides a type-safe way to add attributes to the layout. This way, you can't accidentally add an attribute of an unsupported type without causing a compile-time error (since the generic template would be instantiated and throw a runtime error).

    In summary, this VertexBufferLayout class is using template specializations to ensure that only supported data types can be used to define the vertex attributes. The specializations also handle the specifics for each data type, like setting the correct size and normalization flag. This approach provides safety and efficiency when constructing a vertex buffer layout for use in rendering with OpenGL.


[//] Notes regarding "Explicit (full) Template Specialisation" or "Template Specialisation"

            template<>
            void Push<float>(unsigned int count) {
                // Implementation for float
            }
            template<>
            void Push<unsigned int>(unsigned int count) {
                // Implementation for unsigned int
            }
            template<>
            void Push<unsigned char>(unsigned int count) {
                // Implementation for unsigned char
            }

    -- The Push<unsigned int> function in your code snippet is a specialization of a template function for the unsigned int type. In C++ template programming, this is known as an "explicit (full) template specialization."

    -- The template<> part before the function declaration tells the compiler that you're providing a full specialization of the Push template for a specific type—in this case, unsigned int. You do not need to use the typename keyword in this case because you're not defining a new template; you're providing a specific implementation for an already-declared template.

    <> Here's what's happening in each part of the specialization:

        ?? template<>: Declares that this is a full template specialization.
        ?? void Push<unsigned int>(unsigned int count): Specifies that this specialization of the Push template function will only be used when the template type T is unsigned int.

    <> In the context of your code, you would have a generic template function declared somewhere in your class or above in the code like this:

            template<typename T>
            void Push(unsigned int count);

    -- And then, you provide specialized implementations for specific types that you want to handle differently. This is useful in cases where the general implementation does not suffice or when you need to optimize or use type-specific logic for certain types.

    -- When you call Push with an unsigned int as the template argument, the compiler will use your specialized function instead of the generic template. For any type other than unsigned int, float, and unsigned char (since you've also provided specializations for those), the generic template would be instantiated—if it had a valid implementation—or cause a compile-time error if it's left as you've shown with "throw std::runtime_error("Invalid type used for VAA.");".