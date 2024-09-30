//Práctica 7
//Vázquez Dávila Paul
//18/septiembre/2024
//Grupo: 6
// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"
// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypess
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

//Para el control de dia/noche
float moonRotate = 0.0f;
float sunRotate = 0.0f;
float movelightPosMoon = 0.0f;
float movelightPosSun = 0.0f;
bool moveMoon = false;
bool moveSun = true;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


// Light attributes
glm::vec3 lightPos(-0.5f, -0.5f, -2.5f);



//glm::vec3 lightPos(-0.5f, -0.5f, -2.5f);
//float movelightPosSun = 0.0f;
//float movelightPosMoon = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;


int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Materiales e Iluminacion - Vazquez Davila Paul", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");

    

    // Load models
    Model red_dog((char*)"Models/RedDog.obj");
    Model mush((char*)"Models/Mushroom.obj");
    Model saur((char*)"Models/dilophosaurus.obj");
    Model tree((char*)"Models/treePBR.obj");
    Model bee((char*)"Models/Bee.obj");
    Model snow((char*)"Models/CavePlatform1_Obj.obj");
    Model forest((char*)"Models/Forest_bark.obj");
    Model sun((char*)"Models/Sol.obj");
    Model moon((char*)"Models/Luna.obj");
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,


    };

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load textures

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    image = stbi_load("Models/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);
     
  
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.Use();
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");

        glUniform3f(lightPosLoc, lightPos.x + movelightPosSun, lightPos.y + movelightPosSun, lightPos.z + movelightPosSun);
        glUniform3f(lightPosLoc, lightPos.x + movelightPosMoon, lightPos.y + movelightPosMoon, lightPos.z + movelightPosMoon);
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


        // Set lights properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.3f, 0.3f, 0.4f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.2f, 0.7f, 0.4f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.3f, 0.6f, 0.2f);


        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));


        // Condicional para que solo tome las características de los materiales dependiendo del tipo de luz
        if (moveMoon == false) {
            // Material para la luz solar
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 1.0f, 1.0f, 0.8f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 0.8f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 1.0f, 1.0f, 0.9f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 80.0f);
        }
        else {
            // Material para la luz lunar
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.2f, 0.2f, 0.5f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.5f, 0.5f, 0.8f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.7f, 0.7f, 1.0f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 30.0f);
        }

        //// Draw the loaded model
        //glm::mat4 model(1);
        //model = glm::scale(model, glm::vec3(2.0f, 3.0f, 4.0f));
        //glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        //red_dog.Draw(lightingShader);
        ////glDrawArrays(GL_TRIANGLES, 0, 36);


        //// Draw the loaded model
        ////glm::mat4 model(1);
        //model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        //model = glm::translate(model, glm::vec3(10.0f, 0.0f, -10.0f));
        //glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        //glBindVertexArray(VAO);
        //mush.Draw(lightingShader);

        //Draw the loaded model                     Dinosaurio
        glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 0.0f));
        saur.Draw(shader);

        //                                          Perro
        model = glm::translate(model, glm::vec3(3.0f, 0.0f, -2.0f));
        model = glm::scale(model, glm::vec3(2.0f, 3.0f, 4.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        red_dog.Draw(shader);

        //                                          Árbol
        model = glm::translate(model, glm::vec3(3.0f, 0.0f, -2.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tree.Draw(shader);

        //                                          Abeja
        model = glm::translate(model, glm::vec3(-2.0f, 1.0f, 10.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        bee.Draw(shader);

        //                                          Roca
        model = glm::translate(model, glm::vec3(-10.0f, 1.0f, -70.0f));
        model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        snow.Draw(shader);

        //                                          Hongos
        model = glm::translate(model, glm::vec3(40.0f, 0.0f, 30.0f));
        model = glm::scale(model, glm::vec3(5.0f, 6.0f, 4.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        mush.Draw(shader);

        //                                          Suelo
        model = glm::translate(model, glm::vec3(-20.0f, -9.0f, 3.0f));
        model = glm::scale(model, glm::vec3(1.3f, 1.9f, 3.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        forest.Draw(shader);


       /* model = glm::translate(model, glm::vec3(-20.0f, -9.0f, 3.0f));
        model = glm::scale(model, glm::vec3(1.3f, 1.9f, 3.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        moon.Draw(shader);
        glBindVertexArray(0);*/


        //Fuente de luz 1 (Sol)
        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        if (moveSun) {
            glm::mat4 modelSol(1);

            // Calcula la posición en el arco basado en sunRotate
            float radius = -30.0f;
            float xPos = radius * cos(glm::radians(sunRotate));
            float yPos = radius * sin(glm::radians(sunRotate));
            glm::vec3 lightSunPos = glm::vec3(30.0f + xPos, 0.0f - yPos, 10.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lightSun.position"), lightSunPos.x, lightSunPos.y, lightSunPos.z);

            // Traslación para mover el sol en un arco
            modelSol = glm::translate(modelSol, glm::vec3(5.0f, -3.0f, 20.0f));
            modelSol = glm::translate(modelSol,glm::vec3(xPos, -yPos, 0.0f));  
            
            // Rotación alrededor de su propio eje Y
            modelSol = glm::rotate(modelSol, glm::radians(sunRotate), glm::vec3(4.0f, 1.0f, 0.0f));

            modelSol = glm::scale(modelSol, glm::vec3(0.6f)); // Escala el modelo

            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSol));
            glBindVertexArray(VAO);
            //glUniform1i(glGetUniformLocation(lampshader.Program, "material.diffuse"), 0);
            sun.Draw(lampshader);
            glBindVertexArray(0);
        }
 
        


        //Fuente de luz 2 LUNA

        if (moveMoon) {
            glm::mat4 modelLuna(1);

            // Calcula la posición en el arco basado en moonRotate
            float radius = 30.0f;
            float xPos = radius * cos(glm::radians(moonRotate));
            float yPos = radius * sin(glm::radians(moonRotate));
            glm::vec3 lightMoonPos = glm::vec3(30.0f + xPos, 0.0f - yPos, 10.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lightSun.position"), lightMoonPos.x, lightMoonPos.y, lightMoonPos.z);

            modelLuna = glm::translate(modelLuna, glm::vec3(0.0f, 0.0f, 5.0f));
            modelLuna = glm::translate(modelLuna,glm::vec3(xPos, -yPos, 0.0f));
            modelLuna = glm::rotate(modelLuna, glm::radians(moonRotate), glm::vec3(4.0f, 1.0f, 0.0f));
            modelLuna = glm::scale(modelLuna, glm::vec3(0.1f)); // Escala el modelo

            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelLuna));
            glBindVertexArray(VAO);
            //glActiveTexture(GL_TEXTURE0);
            ////glBindTexture(GL_TEXTURE_2D, moonTexture);
            //glUniform1i(glGetUniformLocation(lampshader.Program, "material.diffuse"), 0);
            moon.Draw(shader);
            glBindVertexArray(0);
        }
        

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (activanim)
    {
        if (rot > -90.0f)
            rot -= 0.1f;
    }

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }

    if (keys[GLFW_KEY_I])
    {
        moveMoon = true;
        moveSun = false;
    }

    if (keys[GLFW_KEY_K])
    {
        moveMoon = false;
        moveSun = true;

    }
    
    //Limitación a 180° de la luna
    if (keys[GLFW_KEY_L] && (moveMoon || moveSun)) {
        moonRotate -= 1.0f;
        if (moonRotate < -180.0f) {
            moonRotate = -180.0f;
        }
        sunRotate -= 1.0;
        if (sunRotate < 0.0f) {
            sunRotate = 0.0f;
        }
    }
    //Limitación a 180° deL sol
    if (keys[GLFW_KEY_O] && (moveSun || moveMoon)) {
        sunRotate += 1.0f;
        if (sunRotate > 180.0f) {
            sunRotate = 180.0f;
        }

        moonRotate += 1.0;
        if (moonRotate > 0.0f) {
            moonRotate = 0.0f;
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////
    // luz° de la luna
    if (keys[GLFW_KEY_L] && (moveMoon || moveSun)) {
        movelightPosMoon -= 0.1f;
      
        movelightPosSun -= 0.1f;
       
    }
    //luz° deL sol
    if (keys[GLFW_KEY_O] && (moveSun || moveMoon)) {
        movelightPosSun += 0.1f;

        movelightPosMoon += 0.1f;
       
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}


