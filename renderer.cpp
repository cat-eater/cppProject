#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

float a = 0.2f;

float e = a / 2;
float c = e+ a;

//vertex shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";


int render()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float vertices[] = {
    -0.5f,-0.5f,0.0f,
    0.7f,0.3f,0.0f,
    0.7f,-0.1f,0.0f
    };

    float vertices2[] = {
    -0.52f,-0.7f,0.0f,
    0.7f,-0.3f,0.0f,
    0.2f,-0.5f,0.0f
    };

    float cubeVertices[] = {
        c,c,0.0f,      //0    
        c,e,0.0f,
        c,-e,0.0f,
        c,-c,0.0f,

        e,c,0.0f,      //4
        e,e,0.0f,
        e,-e,0.0f,
        e,-c,0.0f,

        -e,c,0.0f,     //8
        -e,e,0.0f,
        -e,-e,0.0f,
        -e,-c,0.0f,

        -c,c,0.0f,     //12
        -c,e,0.0f,
        -c,-e,0.0f,
        -c,-c,0.0f,
    };

    unsigned int cubeTopIndecies[] = {
        0,1,4,
        4,5,1,

        1,2,5,
        5,6,2,

        2,3,6,
        6,7,3,


        4,5,8,
        8,9,5,

        5,6,9,
        9,10,6,

        6,7,10,
        10,11,7,


        8,9,12,
        12,13,9,

        9,10,13,
        13,14,10,

        10,11,14,
        14,15,11
    };

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);   //specify the type of shader (here vertex shader, so GL_VERTEX_SHADER)
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);     //attach source code of the shader
    glCompileShader(vertexShader);                                  //compile the shader source code

    //fragment shader
     unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram;                         //A shader program object is the final linked version of multiple shaders combined
    shaderProgram = glCreateProgram();                  //after compiling the shaders, link them to the shaderProgram obj and activate it use them when rendering next
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);                       //activating shaderProgram

    glDeleteShader(vertexShader);                       //Shaders are deleted as they are already in the program
    glDeleteShader(fragmentShader);


    unsigned int VAO;                       //Vertex array object. can be bound just like a vertex BO and any vertex attribute calls from that point will be stored in the VAO 
    glGenVertexArrays(1, &VAO);             //no 
    glBindVertexArray(VAO);

    unsigned int VBO;                       //vertex buffer obj. used to send big batches of data to the GPU together!!
    glGenBuffers(1, &VBO);                  //generated a BO ID (1 here)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);     //binds BO to BO type target (VBO is bound to GL_ARRAY_BUFFER here). openGL has many buffer types
                                            //any call made on GL_ARRAY_BUFFER target will configure the BO     (here (GL_ARRAY_BUFFER target) = VBO)

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  //function copied user-defined data to the current bound BO

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);   //stored in VAO. Whenever VAO is bound, these attribue info will be used
    glEnableVertexAttribArray(0);


    unsigned int VAO2;                       //Vertex array object 2 for second triangle
    glGenVertexArrays(1, &VAO2);             //no 
    glBindVertexArray(VAO2);

    unsigned int vertBuffObj2;
    glGenBuffers(1, &vertBuffObj2);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffObj2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);   //stored in VAO. Whenever VAO is bound, these attribue info will be used
    glEnableVertexAttribArray(0);   //vertAttribue specifies how the openGL should interpreat input data


    
    unsigned int CubeTopVAO;
    glGenVertexArrays(1, &CubeTopVAO);
    glBindVertexArray(CubeTopVAO);

    unsigned int CubeTopVBO;
    glGenBuffers(1, &CubeTopVBO);
    glBindBuffer(GL_ARRAY_BUFFER, CubeTopVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    unsigned int CubeTopEBO;
    glGenBuffers(1, &CubeTopEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CubeTopEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeTopIndecies), cubeTopIndecies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glUseProgram(shaderProgram);        
       // glBindVertexArray(VAO);                         //first triangle
       // glDrawArrays(GL_TRIANGLES, 0, 3);

        
       // glBindVertexArray(VAO2);                        //second triangle
        //glDrawArrays(GL_TRIANGLES, 0, 3);
       
        glBindVertexArray(CubeTopVAO);
        glDrawElements(GL_TRIANGLES, 54, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    std::cout << "closing window" << std::endl;
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        std::cout << "Hit esc and quit" << std::endl;
        glfwSetWindowShouldClose(window, true);
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}