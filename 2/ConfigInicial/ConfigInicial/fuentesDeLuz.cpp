//Práctica 8
//Vázquez Dávila Paul
//Fecha de entrega: 06/octubre/2024
//Grupo: 6


/// La luz debe cambiar dinámicamente, luz establecida en un lugar y con las teclas debe cambiar la intensidad de luz (en general las características). sustituir por variables.
#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

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
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// Variables globales para el color especular
float specularR0 = 1.0f;  // Componente roja de la luz especular
float specularG0 = 0.3f;  // Componente verde de la luz especular
float specularB0 = 0.3f;  // Componente azul de la luz especular

float specularR1 = 1.0f;
float specularG1 = 0.3f;
float specularB1 = 0.3f;

float specularR2 = 1.0f;
float specularG2 = 0.3f;
float specularB2 = 0.3f;

float specularR3 = 1.0f;
float specularG3 = 0.3f;
float specularB3 = 0.3f;
// Variables globales para el color constant, lineal y quadratic
// En donde más cambio muestra es en Directional light
float constant = 0.3f;
float linear = 0.3f;
float quadratic = 0.3f;


// Paso de ajuste de la especular
float specularStep = 0.1f;


glm::vec3 Light1 = glm::vec3(0);


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame
GLFWwindow* window;
int main()
{

	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFW window object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Fuentes de luz - Paul Vazquez Davila", nullptr, nullptr);

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
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	
	Model Cat((char*)"Models/Cat.obj");
	Model Bulb((char*)"Models/Bulb.obj");  
	Model Wlamp((char*)"Models/Sol.obj");
	Model Atlas((char*)"Models/Moon_night.obj");

	Model red_dog((char*)"Models/RedDog.obj");
	Model mush((char*)"Models/Mushroom.obj");
	Model saur((char*)"Models/dilophosaurus.obj");
	Model tree((char*)"Models/treePBR.obj");
	Model bee((char*)"Models/Bee.obj");
	Model snow((char*)"Models/CavePlatform1_Obj.obj");
	Model forest((char*)"Models/Forest_bark.obj");
	Model sun((char*)"Models/Sol.obj");
	Model moon((char*)"Models/Luna.obj");



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

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		
		
		//Load Model
	

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

        //glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),0.1f,0.1f,0.1f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),0.1f, 0.1f, 0.1f);
		/*glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), co, li, qu);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), co, li, qu);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), co, li, qu);*/


		// Point light 1
	    glm::vec3 lightColor;
		lightColor.x= abs(sin(glfwGetTime() *Light1.x));
		lightColor.y= abs(sin(glfwGetTime() *Light1.y));
		lightColor.z= sin(glfwGetTime() *Light1.z);

		
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x,lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x,lightColor.y,lightColor.z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), specularR0, specularG0, specularB0);  //Variables para el cambio de luz specular
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.022f);   // Reduce el valor para una luz más intensa
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.0019f); // Reduce aún más para una caída suave




		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, specularB1);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.0f, specularG1, 0.0f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), specularR1, 0.05f, 0.05f); //Variables para el cambio de luz specular
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), constant); //Variables para el cambio de luz constant
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), linear); //Variables para el cambio de luz linear
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), quadratic); //Variables para el cambio de luz quadratic

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.0f, 0.0f, specularB2);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, specularG2, 0.0f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), specularR2, 0.0f, 0.0f); //Variables para el cambio de luz specular
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), constant);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), linear);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), quadratic);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.0f, 0.0f, specularB3);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.0f, specularG3, 0.0f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), specularR3, 0.0f, 0.0f); //Variables para el cambio de luz specular
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), constant);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), linear);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), quadratic);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.3f, 0.3f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"),0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(10.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(12.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 8.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);

	

		//Carga de modelo 
        /*view = camera.GetViewMatrix();	
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);*/


		//Draw the loaded model                     Dinosaurio
		glm::mat4 modelDino(1);
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		modelDino = glm::rotate(modelDino, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		modelDino = glm::translate(modelDino, glm::vec3(-8.0f, 0.0f, 0.0f));
		saur.Draw(lightingShader);

		//                                          Perro
		model = glm::translate(model, glm::vec3(3.0f, 0.0f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 3.0f, 4.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		red_dog.Draw(lightingShader);

		//                                          Árbol
		model = glm::translate(model, glm::vec3(3.0f, 0.0f, -2.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		tree.Draw(lightingShader);

		//                                          Abeja
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, 10.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		bee.Draw(lightingShader);

		//                                          Roca
		model = glm::translate(model, glm::vec3(-10.0f, 1.0f, -70.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		snow.Draw(lightingShader);

		//                                          Hongos
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, 30.0f));
		model = glm::scale(model, glm::vec3(5.0f, 6.0f, 4.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		mush.Draw(lightingShader);

		//                                          Suelo
		model = glm::translate(model, glm::vec3(-20.0f, -9.0f, 3.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.9f, 3.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		forest.Draw(lightingShader);
	
		//model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(0.0f, 3.0f, -3.0f));
		//model = glm::scale(model, glm::vec3(0.9f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		////glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
	 //   Dog.Draw(lightingShader);
		////glDisable(GL_BLEND);  //Desactiva el canal alfa 
		//glBindVertexArray(0);

		//model = glm::mat4(1);
		//model = glm::scale(model, glm::vec3(0.1f));
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		//
		//Dog.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		//glBindVertexArray(0);
	

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.0f, 6.0f, 1.0f));
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.8f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		
		/// Model 1
		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0] = glm::vec3(10.0f, 10.0f, 12.0f));  //Traslación fija para pointlight 1
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.9f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		Bulb.Draw(lightingShader);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		

		/// Model 2
		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[1] = glm::vec3(-10.0f, 10.0f, 12.0f)); //Traslación fija para pointlight 2
		model = glm::scale(model, glm::vec3(0.6f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		Cat.Draw(lightingShader);
		
		/// Model 3
		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[2] = glm::vec3(-10.0f, 10.0f, -12.0f)); //Traslación fija para pointlight 3
		//model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.9f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		Wlamp.Draw(lightingShader);

		/// Model 4
		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[3] = glm::vec3(10.0f, 10.0f, -12.0f)); //Traslación fija para pointlight 4
		//model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(7.0f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		Atlas.Draw(lightingShader);
		glDrawArrays(GL_TRIANGLES, 0, 36);



		glBindVertexArray(0);
		

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
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

	/*if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.01f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}*/

	//////////////////////////////////////////////// Cambios para Point light 1
	if (keys[GLFW_KEY_R])
	{
		specularR0 += 0.01f;
		if (specularR0 > 6.0f) specularR0 = 6.0f;  // Limitar el valor máximo a 3.0f
	}
	if (keys[GLFW_KEY_F])
	{
		specularR0 -= 0.01f;
		if (specularR0 < 0.0f) specularR0 = 0.0f;  // Limitar el valor mínimo a 0.0f
	}

	if (keys[GLFW_KEY_T])
	{
		specularG0 += 0.01f;
		if (specularG0 > 3.0f) specularG0 = 3.0f;  // Limitar el valor máximo a 3.0f
	}
	if (keys[GLFW_KEY_G])
	{
		specularG0 -= 0.01f;
		if (specularG0 < 0.0f) specularG0 = 0.0f;  // Limitar el valor mínimo a 0.0f
	}

	if (keys[GLFW_KEY_Y])
	{
		specularB0 += 0.01f;
		if (specularB0 > 3.0f) specularB0 = 3.0f;  // Limitar el valor máximo a 3.0f
	}
	if (keys[GLFW_KEY_H])
	{
		specularB0 -= 0.01f;
		if (specularB0 < 0.0f) specularB0 = 0.0f;  // Limitar el valor mínimo a 0.0f
	}


	/////////////////////////////////////////////// Cambios para Point light 2
	if (keys[GLFW_KEY_U])
	{
		specularR1 += 0.01f;
		if (specularR1 > 3.0f) specularR1 = 3.0f;  
	}
	if (keys[GLFW_KEY_J])
	{
		specularR1 -= 0.01f;
		if (specularR1 < 0.0f) specularR1 = 0.0f;  
	}

	if (keys[GLFW_KEY_I])
	{
		specularG1 += 0.01f;
		if (specularG1 > 3.0f) specularG1 = 3.0f;  
	}
	if (keys[GLFW_KEY_K])
	{
		specularG1 -= 0.01f;
		if (specularG1 < 0.0f) specularG1 = 0.0f;  
	}

	if (keys[GLFW_KEY_O])
	{
		specularB1 += 0.01f;
		if (specularB1 > 3.0f) specularB1 = 3.0f;  
	}
	if (keys[GLFW_KEY_L])
	{
		specularB1 -= 0.01f;
		if (specularB1 < 0.0f) specularB1 = 0.0f;  
	}
	////////////////////////////////////////////// Cambios para Point light 3

	if (keys[GLFW_KEY_7])
	{
		specularR2 += 0.01f;
		if (specularR2 > 3.0f) specularR2 = 3.0f; 
	}
	if (keys[GLFW_KEY_4])
	{
		specularR2 -= 0.01f;
		if (specularR2 < 0.0f) specularR2 = 0.0f;  
	}

	if (keys[GLFW_KEY_8])
	{
		specularG2 += 0.01f;
		if (specularG2 > 3.0f) specularG2 = 3.0f;  
	}
	if (keys[GLFW_KEY_5])
	{
		specularG2 -= 0.01f;
		if (specularG2 < 0.0f) specularG2 = 0.0f;  
	}

	if (keys[GLFW_KEY_9])
	{
		specularB2 += 0.01f;
		if (specularB2 > 3.0f) specularB2 = 3.0f;  
	}
	if (keys[GLFW_KEY_6])
	{
		specularB2 -= 0.01f;
		if (specularB2 < 0.0f) specularB2 = 0.0f;  
	}
	/////////////////////////////////////////////// Cambios para Point light 4

	if (keys[GLFW_KEY_UP])
	{
		specularR3 += 0.01f;
		if (specularR3 > 3.0f) specularR3 = 3.0f;  
	}
	if (keys[GLFW_KEY_DOWN])
	{
		specularR3 -= 0.01f;
		if (specularR3 < 0.0f) specularR3 = 0.0f;  
	}

	if (keys[GLFW_KEY_LEFT])
	{
		specularG3 += 0.01f;
		if (specularG3 > 3.0f) specularG3 = 3.0f;  
	}
	if (keys[GLFW_KEY_RIGHT])
	{
		specularG3 -= 0.01f;
		if (specularG3 < 0.0f) specularG3 = 0.0f;  
	}

	if (keys[GLFW_KEY_Z])
	{
		specularB3 += 0.01f;
		if (specularB3 > 3.0f) specularB3 = 3.0f;  
	}
	if (keys[GLFW_KEY_X])
	{
		specularB3 -= 0.01f;
		if (specularB3 < 0.0f) specularB3 = 0.0f;  
	}

	///////////////////////////////////////////// Cambios para Constant, linear y quadratic.


	if (keys[GLFW_KEY_B])
	{
		constant += 0.01f;
		if (constant > 3.0f) constant = 3.0f;
	}
	if (keys[GLFW_KEY_N])
	{
		constant -= 0.01f;
		if (constant < 0.0f) constant = 0.0f;
	}

	if (keys[GLFW_KEY_M])
	{
		linear += 0.01f;
		if (linear > 3.0f) linear = 3.0f;
	}
	if (keys[GLFW_KEY_1])
	{
		linear -= 0.01f;
		if (linear < 0.0f) linear = 0.0f;
	}

	if (keys[GLFW_KEY_2])
	{
		quadratic += 0.01f;
		if (quadratic > 3.0f) quadratic = 3.0f;
	}
	if (keys[GLFW_KEY_3])
	{
		quadratic -= 0.01f;
		if (quadratic < 0.0f) quadratic = 0.0f;
	}

}



// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
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

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
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