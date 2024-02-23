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


-------------------------------------------------------------------------------------

Episode 16: Shader Abstraction in OpenGL

[\\\\] NOTE: Below are archive code regarding compiling shader for OpenGL from the main application.cpp file, as this episode will be abstracting all of these low-level shader details from the user, and thus these functions will now be removed from the code itself. 

        struct ShaderProgramSource {

            std::string VertexSource;
            std::string FragmentSource;
        };

        static ShaderProgramSource ParseShader(const std::string& filepath) {

            std::ifstream stream(filepath); // ifstream = input file stream

            enum class ShaderType {
                NONE = -1, VERTEX = 0, FRAGMENT = 1
            };

            std::string line;
            std::stringstream ss[2];
            ShaderType type = ShaderType::NONE;

            while (getline(stream, line)) { // getline(), imported from string lib. Returns true, if there are still more lines to read inside the file. 

                if (line.find("#shader") != std::string::npos) { // npos means hasn't found, or false. Thus, this if condition is, IF the word "shader" is found in the line....

                    // We need to then see which type of shader is it. 
                    if (line.find("vertex") != std::string::npos) {
                        type = ShaderType::VERTEX;
                    }
                    else if (line.find("fragment") != std::string::npos) {
                        type = ShaderType::FRAGMENT;
                    }
                }
                else {
                    // pushes line by line, the contents of the shaderfile into the stringstream.
                    ss[(int)type] << line << '\n'; 
                }
            
            }
            return { ss[0].str(), ss[1].str() };
        }

        static unsigned int CompileShader(unsigned int type, const std::string& source) {
            
            //  creates a shader object of the specified type (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER, for instance) and returns an integer identifier (ID) for the shader object
            unsigned int id = glCreateShader(type); 
            const char* src = source.c_str(); // converts source code to C-style string, because needed for OpenGL methods.

            // This line assosciated the source code (src) to the shader object specified by id. The 1 indicates that only one string is being passed, and the nullptr indicates
            // that the string is null-terminated (check notes for further info on params |ep10-12|)
            GLCall(glShaderSource(id, 1, &src, nullptr));
            GLCall(glCompileShader(id)); // Compiles the shader source code associated with the shader object id.

            int result;
            // retrieves the compilation status of the shader object id. The status is stored in result. If result is GL_FALSE, it indicates the shader did not compile successfully.
            GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result)); 

            if (result == GL_FALSE) {
                // This means shader didn't compile successfully.

                int length;
                //retrieves the length of the compilation error log(including the null terminator).This length is used to allocate enough space for the error message.
                GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

                char* message = (char*)alloca(length * sizeof(char)); // alloca allows you to allocate stuff dynamically. Allocating memory for error log.
                GLCall(glGetShaderInfoLog(id, length, &length, message)); 

                std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex shader." : "fragment shader.") << std::endl;
                std::cout << message << std::endl;

                GLCall(glDeleteShader(id));
                return 0;
            }

            // The function returns the ID of the compiled shader. This ID is used to reference the shader in other OpenGL functions, like when attaching it to a shader program.
            return id;
        }


        // returns an int, which should be the ID of the created shader program			
        /*
        |||| static function in global namespace just means it has internal linkage, or that the function itself won't be called outside of this cpp file/source file. There 
        is no functional difference if the static has been removed. It will still work. 
            Consider you have two source files, FileA.cpp and FileB.cpp, and both define a function void doSomething(). If doSomething is marked static in both files, each file 
            has its version of doSomething, and they do not interfere with each other. If doSomething is not marked static, however, the linker will see two global-scope functions 
            with the same name, leading to a conflict.
        */
        static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

            // This line creates a new shader program and returns its ID. A shader program in OpenGL is used to link together and manage multiple shaders 
            // (like vertex and fragment shaders).
            unsigned int program =  glCreateProgram();

            // IDs of compiled shader object. This id is used to reference this shader in other OpenGL functions. Like when attaching to a shader program.
            unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); 
            unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

            GLCall(glAttachShader(program, vs)); // Attahes compiled vertex and fragment shader, to the shader program. 
            GLCall(glAttachShader(program, fs));
            GLCall(glLinkProgram(program));      // This line links all attached shaders together in the shader program.
            
            // This line validates the shader program for the current OpenGL state. It's used to check whether the program can execute given the current state of bound 
            // vertex and fragment shaders.
            GLCall(glValidateProgram(program));
            
            GLCall(glDeleteShader(vs)); // After linking, the individual shader objects are no longer needed, so these lines delete them to free up resources.
            GLCall(glDeleteShader(fs));

            return program;
        }

[\\] Notes regarding std::unordered_map (HASH MAPS)

    The std::unordered_map is a standard library container that stores elements formed by the combination of a key value and a mapped value. It is implemented using a hash table, which allows for fast access to individual elements based on their keys.
        
    Here's a step-by-step explanation of how the unordered_map named m_UniformLocationCache is used in the given function GetUniformLocation:

        1. m_UniformLocationCache.find(name) searches the map for an element with the key name. The key is the name of the uniform variable in the shader program.

        2. m_UniformLocationCache.end() returns an iterator to the end of the map, which is not the same as the last element but a position following the last element. This is used to check if the find operation was successful.

        3. if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) checks if the uniform name is already in the map. If the find method returns an iterator that is not equal to end(), it means the element was found.

        4. If the element is found, return m_UniformLocationCache[name]; returns the value associated with the key name. In this context, this value is the location of the uniform variable in the shader program, which is an integer.

        5. If the element is not found (else case implied), GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str())); is called. This OpenGL function retrieves the location of a uniform variable from the shader program. m_RendererID is presumably the identifier of the shader program, and name.c_str() converts the std::string to a C-style string, which is required by OpenGL.

        6. The retrieved location is checked. If it is -1, this means the uniform variable does not exist in the shader program, and a warning message is printed to the console.

        7. m_UniformLocationCache[name] = location; adds the key-value pair to the map, with name as the key and location as the value.

        8. Finally, return location; returns the location of the uniform variable.

    By using an unordered_map, the function avoids repeatedly calling glGetUniformLocation, which is a relatively slow operation, for uniforms that have already been queried. The map serves as a cache, storing the locations of uniform variables so that subsequent queries can return the location immediately if it's already known. This can significantly improve performance, especially if GetUniformLocation is called frequently during rendering.




