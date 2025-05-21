#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>
#include <algorithm> 
#include <vector>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "cubeState.h"


float HalfPi = glm::half_pi<float>();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
char processCubeInput(GLFWwindow* window);



unsigned int loadCubemap(std::vector<std::string> faces);


const unsigned int SCR_WIDTH=800;
const unsigned int SCR_HEIGHT=800;



//cam
glm::vec3 camPos= glm::vec3(0.0f, 0.0f, 5.0f);
const float camRadius = 5.0f;

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 800.0 / 2.0;
//float fov = 45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame(initialized to zero)
float lastFrame = 0.0f;

//user Input flags
bool acceptInput = true;
bool sideRotating = false;

extern float a;
extern float e;

//initializing cube slices
slice white(0, 1, 2, 9, 10, 11, 18, 19, 20);
slice yellow(6, 7, 8, 15, 16, 17, 24, 25, 26);
slice orange(0, 1, 2, 3, 4, 5, 6, 7, 8);
slice red(18, 19, 20,21, 22, 23, 24, 25, 26);


GLFWwindow* window;

int initializeWindow()
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
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    //std::vector<slice> sliceVector=cubeInit();
    genCubePositonVecs();
    genCubeModelMat();
    genCubeModelBuffMat();
    

    white.centerAxis = glm::vec3(0.0, 1.0, 0.0);
    yellow.centerAxis = glm::vec3(0.0, 1.0, 0.0);
    orange.centerAxis = glm::vec3( 1.0, 0.0, 0.0);
    red.centerAxis = glm::vec3( 1.0, 0.0, 0.0);
    

 

    Shader ourShader("C:/Users/manan/OneDrive/Documents/CPP endsem project/cppProject/shader.vs", "C:/Users/manan/OneDrive/Documents/CPP endsem project/cppProject/shader.fs");

    std::string matName = "view";

    unsigned int CubeTopVAO;
    glGenVertexArrays(1, &CubeTopVAO);
    glBindVertexArray(CubeTopVAO);

    unsigned int CubeTopVBO;
    glGenBuffers(1, &CubeTopVBO);
    glBindBuffer(GL_ARRAY_BUFFER, CubeTopVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);



    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    //texture
    std::vector<std::string> faces
    {
            "C:/Users/manan/OneDrive/Documents/cubeFaceStickers/orange.jpg",//right
            "C:/Users/manan/OneDrive/Documents/cubeFaceStickers/red.jpg",
        "C:/Users/manan/OneDrive/Documents/cubeFaceStickers/white.jpg",     //top
            "C:/Users/manan/OneDrive/Documents/cubeFaceStickers/yellow.jpg",
            "C:/Users/manan/OneDrive/Documents/cubeFaceStickers/blue.jpg", //front
            "C:/Users/manan/OneDrive/Documents/cubeFaceStickers/green.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);




    

    ourShader.use();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);


    bool flag = true;
    bool xTurnComplete = false;

    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
       // cubeTurnKey=processCubeInput(window);
        // render
        // ------

        if (!sideRotating) processCubeInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        ourShader.use();



        //config view matrix
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(camPos,
            glm::vec3(0.0, 0.0, 0.0),
            glm::vec3(0.0, 1.0, 0.0));
        ourShader.setMat4("view", view);

        

 


        glBindVertexArray(CubeTopVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        

        
        for (int i = 0;i < 27;i++) {
            //config modelmatrix
            
            float angle = (float)glfwGetTime();
            
            /*
            if (i < 9) {
                model = glm::translate(model, -cubePositions[i]);
                model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0, 0.0, 0.0));
                model = glm::translate(model, cubePositions[i]);
                //model = glm::transpose(model);
            }*/
            
                white.turn(i, angle,1.0);
                orange.turn(i, angle, 2.0);

            if (angle)
                ourShader.setMat4("model", cubeModelBuffMat[i]);
                glDrawArrays(GL_TRIANGLES, 0, 36);
              
            
            
        }
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

char processCubeInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        sideRotating = true;
        return 'U';
    }
    else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        return 'R';
    }
    else return'N';

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX =- xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = -(lastY - ypos); // reversed since y-coordinates go from bottom to top(reversed it again for mouse inversion)
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    
    camPos.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    camPos.y = sin(glm::radians(pitch));
    camPos.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camPos *= camRadius;
    
}

