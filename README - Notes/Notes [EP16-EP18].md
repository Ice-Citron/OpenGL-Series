-------------------------------------------------------------------------------------

Episode 16 -- Writing a Basic Renderer in OpenGL

-- Main points discusses (Because I am sleepy and can barely concentrate):

    1. Introduction to Rendering: Cherno introduces the concept of creating a renderer in OpenGL. The initial code setup involves classes that get created, bound, and drawn, but it lacks a central pipeline for rendering, leading to repetitive and error-prone code.

    2. The Role of a Renderer: The renderer acts as a central unit that manages the drawing process. Instead of manually handling draw calls and managing OpenGL states, the renderer abstracts these details, allowing for a more organized and efficient rendering process.

    3. Creating the Renderer Class: Cherno demonstrates how to create a Renderer class, which includes a draw function. This function takes in a vertex array, an index buffer, and a shader, and performs the drawing operation. The goal is to abstract away direct OpenGL calls (eg. GLCall(~~random_OpenGL())) from the main application code.

    4. Implementing Draw and Clear Functions: The Renderer class is expanded with a draw function that encapsulates the OpenGL draw calls, and a clear function that clears the screen. This further abstracts the OpenGL details from the main application logic.

    5. Discussion on Static vs. Singleton Renderer: Cherno discusses the design choice between making the Renderer class static (a singleton), or allowing multiple instances. The tutorial opts for a flexible approach without enforcing a singleton pattern.

    6. Moving OpenGL Calls to Renderer: The tutorial progresses by moving the last remaining OpenGL calls from the main file to the Renderer class, achieving a cleaner main application file that no longer directly interacts with OpenGL for rendering tasks.

    7. Setting Up for Future Topics: Cherno hints at future topics, including textures, 3D models, debugging interfaces, GUI systems, and text rendering. The discussion suggests that the series will delve into more complex OpenGL features and rendering techniques.


[//] GPT Question: what does cherno mean by that the vertex buffer is already bound to the vertex array object. and you only need to bound index buffer, shader, and vertex array

    In OpenGL, a Vertex Array Object (VAO) is a high-level abstraction that stores information about the configuration of vertex attributes and which Vertex Buffer Objects (VBOs) are associated with those attributes. When The Cherno mentions that "the vertex buffer is already bound to the vertex array object," he is referring to the fact that once you bind a VBO (Vertex Buffer Object) to a VAO, the VAO remembers which VBOs are bound to it for vertex attributes. This means that after configuring a VAO with specific VBOs for vertex data, you do not need to bind those VBOs again as long as you use the same VAO; the VAO retains the information about the VBOs it is associated with.

    However, for rendering, you still need to explicitly bind a few things:

        1. Vertex Array Object (VAO): By binding the VAO, you effectively bind all the VBOs (for vertex attributes) associated with it. This is because the VAO stores the state related to the vertex input configuration, including which VBOs to use for vertex attributes.

        2. Index Buffer: The Index Buffer (often a separate buffer object, sometimes called an Element Buffer Object or EBO in OpenGL terms) is used to specify the indices of the vertices to draw. This allows for more efficient memory usage and rendering by reusing vertices. Unlike VBOs associated with vertex attributes, the index buffer is not automatically remembered by the VAO. Therefore, you need to bind the index buffer explicitly when you want to use it for rendering.

        3. Shader Program: The shader program, which consists of vertex and fragment shaders (and possibly others like geometry shaders), needs to be bound before drawing. This is because OpenGL needs to know which shaders to use when processing the vertices and fragments during the rendering pipeline.

    In summary, when TheCherno says you only need to bind the index buffer, shader, and vertex array, he's highlighting the efficiency of using VAOs to remember vertex attribute configurations and associated VBOs, while also pointing out that some components, like the index buffer and shader program, still need to be explicitly bound every time you render. This setup minimizes the number of state changes and bindings you need to perform, which can lead to cleaner and more efficient rendering code.


[\\] Quick notes regarding Unbind() being a waste of performance

    (Note: Also mentioned that normally, programs will do it in such that in debug build, the unbinding will take place, however they are all removed via macros in release builds)

    [] In OpenGL development, the practice of unbinding resources such as Vertex Array Objects (VAOs), Vertex Buffer Objects (VBOs), and shaders after their use is a topic of consideration. Unbinding these resources is not a requirement for the correct functioning of an OpenGL application. While it can be beneficial for debugging purposes, helping to identify issues related to state management and accidental modifications, it is not strictly necessary for the application's operation. The OpenGL state machine ensures that resources remain active until explicitly replaced or unbound, meaning that subsequent operations will typically bind the necessary resources anew, rendering the unbinding step redundant.

    [] From a performance perspective, unnecessary unbinding operations can lead to a waste of computational resources. This is particularly relevant in performance-critical applications where efficiency is paramount. However, during the development and debugging phases, unbinding can aid in maintaining clear state management and potentially reduce bugs by preventing unintended interactions between rendering operations. Ultimately, the decision to unbind resources should balance the need for clear state management during development with the desire for optimal performance in the final application.








