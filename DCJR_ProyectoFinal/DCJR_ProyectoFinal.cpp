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
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacion();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 0.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float rotPuerta = 90.0f;
float rotCapsula = -90.0f;
float rotCaps1;
float rotS = 180.0f;
float rotSilla;
float rotP;
float rotP2;
float rotP3;
float rotP4;



// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 PosIni(0.0f, 0.0f, 0.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

bool active;


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Keyframes
float posX =PosIni.x, posY = PosIni.y, posZ = PosIni.z, rotRodIzq = 0, rotRodDer = 0, rotHomIzq = 0, rotHomDer = 0, rotCabeza = 0, rot = 0, rotMet = 0;
float MposX = PosIni.x, MposY = PosIni.y, MposZ = PosIni.z;

#define MAX_FRAMES 15
int i_max_steps = 190;
int i_curr_steps = 0;

typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX Han
	float posY;		//Variable para PosicionY Han
	float posZ;		//Variable para PosicionZ Han
	float MposX;		//Variable para PosicionX Met
	float MposY;		//Variable para PosicionY Met
	float MposZ;		//Variable para PosicionZ Met
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float MincX;		//Variable para IncrementoX Met
	float MincY;		//Variable para IncrementoY Met
	float MincZ;		//Variable para IncrementoZ Met
	float rotRodIzq;
	float rotRodDer;
	float rotHomIzq;
	float rotHomDer;
	float rotCabeza;
	float rot;
	float rotInc; //rotInc Rodilla Izq
	float rotInc2; //rotInc Rodilla Der
	float rotInc3; //rotInc Hombro Izq
	float rotInc4; //rotInc Hombro Der
	float rotInc5; //rotInc Cabeza
	float rotInc6; //rotInc Rotacion Han Solo 

	float rotMet;
	

	//rotInc de Meteoritos
	float rotInc7;


}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(posX, -0.5, posZ),//POINT LIGHT PRINCIPAL
	glm::vec3(0.753f, 0.505f,-0.727f),//FOCO SALA PRINCIPAL 1
	glm::vec3(1.294f,0.502f,-1.087f),//FOCO SALA PRINCIPAL 1
	glm::vec3(0.996f,0.427f,-1.487f),//FOCO SALA PRINCIPAL 1
	glm::vec3(0.497f, 0.427f,-1.201f),//FOCO SALA PRINCIPAL 1
	glm::vec3(5.165f, -0.302f,-3.673f),//PANTALLA CABINA 1
	glm::vec3(5.108f,-0.105f,-3.673f),//PANTALLA CABINA 2
	glm::vec3(4.024f, -0.302f,-3.673f),//PANTALLA CABINA 3

};


glm::vec3 Light1 = glm::vec3(0);



void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);
	//printf("posX %f\n", posX);
	//printf("posY %f\n", posY);
	//printf("PosZ %f\n", posZ);
	//printf("rotRodIzq %f\n", rotRodIzq);
	//printf("rotRodDer %f\n", rotRodDer);
	//printf("rotHomIzq %f\n", rotHomIzq);
	//printf("rotHomDer %f\n", rotHomDer);
	//printf("rotCabeza %f\n", rotCabeza);
	//printf("rot %f\n", rot);

	printf("MposX %f\n", MposX);
	printf("MposY %f\n", MposY);
	printf("MPosZ %f\n", MposZ);
	printf("rotMet %f\n", rotMet);

	
	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;
	
	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].rotRodDer = rotRodDer;

	KeyFrame[FrameIndex].rotHomIzq = rotHomIzq;
	KeyFrame[FrameIndex].rotHomDer = rotHomDer;

	KeyFrame[FrameIndex].rotCabeza = rotCabeza;

	KeyFrame[FrameIndex].rot = rot;

	KeyFrame[FrameIndex].MposX = MposX;
	KeyFrame[FrameIndex].MposY = MposY;
	KeyFrame[FrameIndex].MposZ = MposZ;

	KeyFrame[FrameIndex].rotMet = rotMet;


	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	rotRodDer = KeyFrame[0].rotRodDer;

	rotHomIzq = KeyFrame[0].rotHomIzq;
	rotHomDer = KeyFrame[0].rotHomDer;

	rotCabeza = KeyFrame[0].rotCabeza;

	rot = KeyFrame[0].rot;

	MposX = KeyFrame[0].MposX;
	MposY = KeyFrame[0].MposY;
	MposZ = KeyFrame[0].MposZ;

	rotMet = KeyFrame[0].rotMet;


}

void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	KeyFrame[playIndex].rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	KeyFrame[playIndex].rotInc2 = (KeyFrame[playIndex + 1].rotRodDer - KeyFrame[playIndex].rotRodDer) / i_max_steps;
	KeyFrame[playIndex].rotInc3 = (KeyFrame[playIndex + 1].rotHomIzq - KeyFrame[playIndex].rotHomIzq) / i_max_steps;
	KeyFrame[playIndex].rotInc4 = (KeyFrame[playIndex + 1].rotHomDer - KeyFrame[playIndex].rotHomDer) / i_max_steps;
	KeyFrame[playIndex].rotInc5 = (KeyFrame[playIndex + 1].rotCabeza - KeyFrame[playIndex].rotCabeza) / i_max_steps;
	KeyFrame[playIndex].rotInc6 = (KeyFrame[playIndex + 1].rot - KeyFrame[playIndex].rot) / i_max_steps;

	KeyFrame[playIndex].MincX = (KeyFrame[playIndex + 1].MposX - KeyFrame[playIndex].MposX) / i_max_steps;
	KeyFrame[playIndex].MincY = (KeyFrame[playIndex + 1].MposY - KeyFrame[playIndex].MposY) / i_max_steps;
	KeyFrame[playIndex].MincZ = (KeyFrame[playIndex + 1].MposZ - KeyFrame[playIndex].MposZ) / i_max_steps;
	KeyFrame[playIndex].rotInc7 = (KeyFrame[playIndex + 1].rotMet - KeyFrame[playIndex].rotMet) / i_max_steps;


}

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto: Halcon Milenario", nullptr, nullptr);

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
	printf("%f", glfwGetTime());

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	Shader SkyBoxshader("Shader/SkyBox.vs", "Shader/SkyBox.frag");

	Model Halcon((char*)"Models/Halcon/halcon.obj");
	Model modulosCabina((char*)"Models/Cabina/modulosCabina.obj");
	Model moduloPrincipal((char*)"Models/Cabina/moduloPrincipal.obj");
	Model focoRojo((char*)"Models/Focos/focoRojo.obj");
	Model focoVerde((char*)"Models/Focos/focoVerde.obj");
	Model focoCalido((char*)"Models/Focos/focoCalido.obj");
	Model focoCalido2((char*)"Models/Focos/focoCalido2.obj");
	Model Sillon((char*)"Models/Sillon/sillon.obj");
	Model mAjedrez((char*)"Models/mesaAjedrez/mAjedrez.obj");
	Model baseSilla((char*)"Models/SillaMando/baseSilla.obj");
	Model silla((char*)"Models/SillaMando/silla.obj");
	Model cama((char*)"Models/Cama/cama.obj");
	Model capsula((char*)"Models/Cama/capsula.obj");
	Model PiernaIzq((char*)"Models/lego2/piernaIzq.obj");
	Model PiernaDer((char*)"Models/lego2/piernaDer.obj");
	Model BrazoIzq((char*)"Models/lego2/BrazoIzq.obj");
	Model BrazoDer((char*)"Models/lego2/BrazoDer.obj");
	Model Esqueleto((char*)"Models/lego2/esqueleto.obj");
	Model Cabeza((char*)"Models/lego2/cabeza.obj");
	Model compuertaBase((char*)"Models/Cabina/compuertaBase.obj");
	Model compuertaBase2((char*)"Models/Cabina/compuertaBase2.obj");
	Model puerta1_Entrada1((char*)"Models/Puertas/PUERTA1_ENTRADA1.obj");
	Model puerta1_Entrada2((char*)"Models/Puertas/PUERTA1_ENTRADA2.obj");
	Model puerta2_Entrada1((char*)"Models/Puertas/PUERTA2_ENTRADA1.obj");
	Model puerta2_Entrada2((char*)"Models/Puertas/PUERTA2_ENTRADA2.obj");
	Model Meteoro1((char*)"Models/Meteoritos/Meteoro1.obj");
	Model Meteoro2((char*)"Models/Meteoritos/Meteoro2.obj");
	Model Meteoro3((char*)"Models/Meteoritos/Meteoro3.obj");
	Model Meteoro4((char*)"Models/Meteoritos/Meteoro4.obj");
	Model Meteoro5((char*)"Models/Meteoritos/Meteoro5.obj");

	// Build and compile our shader program

	//Inicialización de KeyFrames
	
	for(int i=0; i<MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].rotInc = 0;
		KeyFrame[i].rotRodDer = 0;
		KeyFrame[i].rotInc2 = 0;
		KeyFrame[i].rotInc3 = 0;
		KeyFrame[i].rotInc4 = 0;
		KeyFrame[i].rotInc5 = 0;
		KeyFrame[i].rotInc6 = 0;
		KeyFrame[i].rotInc7 = 0;
		KeyFrame[i].rotHomIzq = 0;
		KeyFrame[i].rotHomDer = 0;
		KeyFrame[i].rotCabeza = 0;
		KeyFrame[i].rot = 0;
		KeyFrame[i].MposX = 0;
		KeyFrame[i].MposY = 0;
		KeyFrame[i].MposZ = 0;
		KeyFrame[i].MincX = 0;
		KeyFrame[i].MincY = 0;
		KeyFrame[i].MincZ = 0;
		KeyFrame[i].rotMet = 0;
	}

	KeyFrame[0].posX = 0.0;
	KeyFrame[0].posY = 0.0;
	KeyFrame[0].posZ = 0.0;
	KeyFrame[0].rotRodIzq = 40.0;
	KeyFrame[0].rotRodDer = -33.0;
	KeyFrame[0].rotHomIzq = -45.0;
	KeyFrame[0].rotHomDer = 61.0;
	KeyFrame[0].rotCabeza = 0.0;
	KeyFrame[0].rot = 115.0;

	KeyFrame[1].posX = 0.38;
	KeyFrame[1].posY = 0.0;
	KeyFrame[1].posZ = -0.26;
	KeyFrame[1].rotRodIzq = -40.0;
	KeyFrame[1].rotRodDer = 16.0;
	KeyFrame[1].rotHomIzq = 64.0;
	KeyFrame[1].rotHomDer = -22.0;
	KeyFrame[1].rotCabeza = 0.0;
	KeyFrame[1].rot = 115.0;

	KeyFrame[2].posX = 0.83;
	KeyFrame[2].posY = 0.0;
	KeyFrame[2].posZ = -0.47;
	KeyFrame[2].rotRodIzq = 39.0;
	KeyFrame[2].rotRodDer = -20.0;
	KeyFrame[2].rotHomIzq = -33.0;
	KeyFrame[2].rotHomDer = 61.0;
	KeyFrame[2].rotCabeza = 0.0;
	KeyFrame[2].rot = 115.0;

	KeyFrame[3].posX = 1.359999;
	KeyFrame[3].posY = 0.0;
	KeyFrame[3].posZ = -0.71;
	KeyFrame[3].rotRodIzq = -45.0;
	KeyFrame[3].rotRodDer = 26.0;
	KeyFrame[3].rotHomIzq = 66.0;
	KeyFrame[3].rotHomDer = -12.0;
	KeyFrame[3].rotCabeza = 0.0;
	KeyFrame[3].rot = 115.0;

	KeyFrame[4].posX = 1.839999;
	KeyFrame[4].posY = 0.0;
	KeyFrame[4].posZ = -0.979999;
	KeyFrame[4].rotRodIzq = 29.0;
	KeyFrame[4].rotRodDer = -34.0;
	KeyFrame[4].rotHomIzq = -24.0;
	KeyFrame[4].rotHomDer = 55.0;
	KeyFrame[4].rotCabeza = 0.0;
	KeyFrame[4].rot = 115.0;

	KeyFrame[5].posX = 2.279998;
	KeyFrame[5].posY = 0.0;
	KeyFrame[5].posZ = -1.319999;
	KeyFrame[5].rotRodIzq = -30.0;
	KeyFrame[5].rotRodDer = 29.0;
	KeyFrame[5].rotHomIzq = 48.0;
	KeyFrame[5].rotHomDer = -21.0;
	KeyFrame[5].rotCabeza = 0.0;
	KeyFrame[5].rot = 115.0;
	KeyFrame[5].MposX = -11.999991;
	KeyFrame[5].MposY = 5.999997;
	KeyFrame[5].MposZ = -2.640000;
	KeyFrame[5].rotMet = 285.000000;

	KeyFrame[6].posX = 2.609998;
	KeyFrame[6].posY = 0.0;
	KeyFrame[6].posZ = -1.829999;
	KeyFrame[6].rotRodIzq = 38.0;
	KeyFrame[6].rotRodDer = -29.0;
	KeyFrame[6].rotHomIzq = -45.0;
	KeyFrame[6].rotHomDer = 60.0;
	KeyFrame[6].rotCabeza = 0.0;
	KeyFrame[6].rot = 189.0;
	KeyFrame[6].MposX = -6.479997;
	KeyFrame[6].MposY = 2.480000;
	KeyFrame[6].MposZ = 3.509997;
	KeyFrame[6].rotMet = 432.000000;

	KeyFrame[7].posX = 2.609998;
	KeyFrame[7].posY = 0.0;
	KeyFrame[7].posZ = -2.259998;
	KeyFrame[7].rotRodIzq = -40.0;
	KeyFrame[7].rotRodDer = 30.0;
	KeyFrame[7].rotHomIzq = 48.0;
	KeyFrame[7].rotHomDer = -21.0;
	KeyFrame[7].rotCabeza = 0.0;
	KeyFrame[7].rot = 189.0;
	KeyFrame[7].MposX = -2.800000;
	KeyFrame[7].MposY = 0.240000;
	KeyFrame[7].MposZ = 8.350012;
	KeyFrame[7].rotMet = 570.000000;

	KeyFrame[8].posX = 2.609998;
	KeyFrame[8].posY = 0.0;
	KeyFrame[8].posZ = -2.259998;
	KeyFrame[8].rotRodIzq = 0.0;
	KeyFrame[8].rotRodDer = -3.0;
	KeyFrame[8].rotHomIzq = 110.0;
	KeyFrame[8].rotHomDer = 110.0;
	KeyFrame[8].rotCabeza = 0.0;
	KeyFrame[8].rot = 189.0;
	KeyFrame[8].MposX = 1.82;
	KeyFrame[8].MposY = 0.240000;
	KeyFrame[8].MposZ = 11.700025;
	KeyFrame[8].rotMet = 744.000000;


	KeyFrame[9].posX = 2.609998;
	KeyFrame[9].posY = 0.0;
	KeyFrame[9].posZ = -2.259998;
	KeyFrame[9].rotRodIzq = 0.0;
	KeyFrame[9].rotRodDer = -3.0;
	KeyFrame[9].rotHomIzq = 110.0;
	KeyFrame[9].rotHomDer = 110.0;
	KeyFrame[9].rotCabeza = 19.0;
	KeyFrame[9].rot = 189.0;
	KeyFrame[9].MposX = 0.639999;
	KeyFrame[9].MposY = 0.240000;
	KeyFrame[9].MposZ = 20.299974;
	KeyFrame[9].rotMet = 909.0;


	KeyFrame[10].posX = 2.609998;
	KeyFrame[10].posY = 0.0;
	KeyFrame[10].posZ = -2.259998;
	KeyFrame[10].rotRodIzq = 0.0;
	KeyFrame[10].rotRodDer = -3.0;
	KeyFrame[10].rotHomIzq = 110.0;
	KeyFrame[10].rotHomDer = 110.0;
	KeyFrame[10].rotCabeza = -13.0;
	KeyFrame[10].rot = 189.0;
	KeyFrame[10].MposX = 0.239999;
	KeyFrame[10].MposY = 0.240000;
	KeyFrame[10].MposZ = 23.899920;
	KeyFrame[10].rotMet = 1095.0;


	KeyFrame[11].posX = 2.609998;
	KeyFrame[11].posY = 0.0;
	KeyFrame[11].posZ = -2.259998;
	KeyFrame[11].rotRodIzq = 0.0;
	KeyFrame[11].rotRodDer = -3.0;
	KeyFrame[11].rotHomIzq = 110.0;
	KeyFrame[11].rotHomDer = 110.0;
	KeyFrame[11].rotCabeza = 24.0;
	KeyFrame[11].rot = 189.0;
	KeyFrame[11].MposX = -9.359994;
	KeyFrame[11].MposY = 0.240000;
	KeyFrame[11].MposZ = 30.399820;
	KeyFrame[11].rotMet = 1302.0;


	KeyFrame[12].posX = 2.609998;
	KeyFrame[12].posY = 0.0;
	KeyFrame[12].posZ = -2.259998;
	KeyFrame[12].rotRodIzq = 0.0;
	KeyFrame[12].rotRodDer = -3.0;
	KeyFrame[12].rotHomIzq = 110.0;
	KeyFrame[12].rotHomDer = 110.0;
	KeyFrame[12].rotCabeza = -15.0;
	KeyFrame[12].rot = 189.0;
	KeyFrame[12].MposX = -9.359994;
	KeyFrame[12].MposY = 0.240000;
	KeyFrame[12].MposZ = 30.399820;
	KeyFrame[12].rotMet = 1400.0;


	KeyFrame[13].posX = 2.609998;
	KeyFrame[13].posY = 0.0;
	KeyFrame[13].posZ = -2.259998;
	KeyFrame[13].rotRodIzq = 0.0;
	KeyFrame[13].rotRodDer = -3.0;
	KeyFrame[13].rotHomIzq = 110.0;
	KeyFrame[13].rotHomDer = 110.0;
	KeyFrame[13].rotCabeza = 24.0;
	KeyFrame[13].rot = 189.0;
	KeyFrame[13].MposX = -9.359994;
	KeyFrame[13].MposY = 0.240000;
	KeyFrame[13].MposZ = 30.399820;
	KeyFrame[13].rotMet = 1500.0;


	KeyFrame[14].posX = 2.609998;
	KeyFrame[14].posY = 0.0;
	KeyFrame[14].posZ = -2.259998;
	KeyFrame[14].rotRodIzq = 0.0;
	KeyFrame[14].rotRodDer = -3.0;
	KeyFrame[14].rotHomIzq = 4.0;
	KeyFrame[14].rotHomDer = 10.0;
	KeyFrame[14].rotCabeza = 4.0;
	KeyFrame[14].rot = 189.0;
	KeyFrame[14].MposX = -9.359994;
	KeyFrame[14].MposY = 0.240000;
	KeyFrame[14].MposZ = 30.399820;
	KeyFrame[14].rotMet = 1602.0;



	FrameIndex = 15;

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};


	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1,&skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),&skyboxVertices,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

	// Load textures
	vector<const GLchar*> faces;

	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");
	
	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

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
		animacion();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// ============================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// ============================


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.3f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

		glm::vec3 LightP1;
		LightP1.x = abs(sin(glfwGetTime() * Light1.x));
		LightP1.y = abs(sin(glfwGetTime() * Light1.y));
		LightP1.z = sin(glfwGetTime() * Light1.z);

		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), LightP1.x, LightP1.y, LightP1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), LightP1.x, LightP1.y, LightP1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.01f, 0.01f, 0.01f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);

		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.988f, 0.945f, 0.670f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.988f, 0.945f, 0.670f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 2.5f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 25.0f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 1.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 1.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 2.5f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 25.0f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.988f, 0.945f, 0.670f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.988f, 0.945f, 0.670f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 2.5f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 25.0f);

		// Point light 5
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].position"), pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].ambient"), 0.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].diffuse"), 0.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].linear"), 2.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].quadratic"), 20.0f);

		// Point light 6
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].position"), pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].ambient"), 0.117f, 0.658f, 0.901f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].diffuse"), 0.117f, 0.658f, 0.901f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].linear"), 1.5f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].quadratic"), 15.0f);

		// Point light 7
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].position"), pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].ambient"), 0.050f, 0.247f, 0.949f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].diffuse"), 0.050f, 0.247f, 0.949f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].linear"), 1.5f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].quadratic"), 15.0f);

		// Point light 8
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].position"), pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].ambient"), 0.050f, 0.949f, 0.941f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].diffuse"), 0.050f, 0.949f, 0.941f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].linear"), 1.5f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].quadratic"), 15.0f);

		// Point light 9
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].position"), pointLightPositions[8].x, pointLightPositions[8].y, pointLightPositions[8].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].ambient"), 0.050f, 0.949f, 0.941f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].diffuse"), 0.050f, 0.949f, 0.941f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[8].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[8].linear"), 1.5f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[8].quadratic"), 15.0f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(10.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

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

		glBindVertexArray(VAO);
		glm::mat4 tmp = glm::mat4(1.0f); //Temp

		//Carga de modelo 
		//Personaje

		view = camera.GetViewMatrix();
		glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(1.955f+posX, -0.262f+posY, -1.17f+posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(4.545f, -0.279, -2.475f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Esqueleto.Draw(lightingShader);

		//Pierna Izq
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.955f + posX, -0.262f + posY, -1.17f + posZ));
		//model = glm::translate(model, glm::vec3(1.899f, -0.365f, -1.174f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.056f, -0.103f, -0.004f));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PiernaIzq.Draw(lightingShader);

		////Pierna Der
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.955f + posX, -0.262f + posY, -1.17f + posZ));
		//model = glm::translate(model, glm::vec3(2.015f, -0.365f, -1.174f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.06f, -0.103f, -0.004f));
		model = glm::rotate(model, glm::radians(-rotRodDer), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PiernaDer.Draw(lightingShader);

		//Brazo Derecho
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.955f + posX, -0.262f + posY, -1.17f + posZ));
		//model = glm::translate(model, glm::vec3(4.463f, -0.175f, -2.475f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.072f, 0.102f, 0.002f));
		model = glm::rotate(model, glm::radians(-rotHomDer), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BrazoDer.Draw(lightingShader);

		//Brazo Izquierdo
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.955f + posX, -0.262f + posY, -1.17f + posZ));
		//model = glm::translate(model, glm::vec3(4.623f, -0.175f, -2.475f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.073f, 0.105f, 0.005f));
		model = glm::rotate(model, glm::radians(-rotHomIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BrazoIzq.Draw(lightingShader);

		//Cabeza
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.955f + posX, -0.262f + posY, -1.17f + posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 0.206f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cabeza.Draw(lightingShader);


		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Halcon.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		modulosCabina.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		moduloPrincipal.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		focoRojo.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		focoVerde.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		focoCalido.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		focoCalido2.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Sillon.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		mAjedrez.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.614f, -0.435f, -4.241f));
		model = glm::rotate(model, glm::radians(rotS), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotSilla), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.614f, -0.435f, -4.241f));
		model = glm::rotate(model, glm::radians(rotS), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		baseSilla.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cama.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-0.789f, -0.303f, -1.215f));
		model = glm::rotate(model, glm::radians(rotCapsula), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotCaps1), glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		capsula.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		compuertaBase.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		compuertaBase2.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.947f+MposX, -0.764f+MposY, -16.958f+MposZ));
		model = glm::rotate(model, glm::radians(rotMet), glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Meteoro1.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.263F + MposX, -3.211f + MposY, -16.958f + MposZ));
		model = glm::rotate(model, glm::radians(rotMet), glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Meteoro2.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.822f+MposX, -1.97f+MposY, -21.082f+MposZ));
		model = glm::rotate(model, glm::radians(rotMet), glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Meteoro3.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.638f+MposX, 1.355f+MposY, -16.958f+MposZ));
		model = glm::rotate(model, glm::radians(rotMet), glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Meteoro4.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.069f+MposX, -1.757f+MposY, -17.491f+MposZ));
		model = glm::rotate(model, glm::radians(rotMet), glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Meteoro5.Draw(lightingShader);

		
		//1

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.959f, 0.0f, 0.684f));
		model = glm::rotate(model, glm::radians(rotPuerta), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotP), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		puerta1_Entrada1.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.959f, 0.0f, -0.588f));
		model = glm::rotate(model, glm::radians(rotPuerta), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotP2), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		puerta1_Entrada2.Draw(lightingShader);

		//2

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.938f, 0.0f, -0.585f));
		model = glm::rotate(model, glm::radians(rotPuerta), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotP3), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		puerta2_Entrada1.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.924f, 0.0f, 0.676f));
		model = glm::rotate(model, glm::radians(rotPuerta), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotP4), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		puerta2_Entrada2.Draw(lightingShader);


		

		glBindVertexArray(0);

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
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.01f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		for (GLuint i = 0; i < 5; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.01f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0); 


		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return 0;
}

void animacion()
{

		//Movimiento del personaje

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += KeyFrame[playIndex].incX;
			posY += KeyFrame[playIndex].incY;
			posZ += KeyFrame[playIndex].incZ;

			MposX += KeyFrame[playIndex].MincX;
			MposY += KeyFrame[playIndex].MincY;
			MposZ += KeyFrame[playIndex].MincZ;

			rotRodIzq += KeyFrame[playIndex].rotInc;
			rotRodDer += KeyFrame[playIndex].rotInc2;
			rotHomIzq += KeyFrame[playIndex].rotInc3;
			rotHomDer += KeyFrame[playIndex].rotInc4;
			rotCabeza += KeyFrame[playIndex].rotInc5;
			rot += KeyFrame[playIndex].rotInc6;
			rotMet += KeyFrame[playIndex].rotInc7;

			i_curr_steps++;
		}
	}
			
}




// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode){
	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex > 1))
		{

			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}

	}


	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex<MAX_FRAMES)
		{
			saveFrame();
		}

	}

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
		if (active) {
			Light1 = glm::vec3(1.0f, 0.0f, 0.0f);
		}
		else
			Light1 = glm::vec3(0.0f, 0.0f, 0.0f);
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

// Moves/alters the camera positions based on user input
void DoMovement()
{

	if (keys[GLFW_KEY_1])
	{

		rot += 1;

	}

	if (keys[GLFW_KEY_2])
	{
		if (rotRodIzq < 80.0f)
			rotRodIzq += 1.0f;

	}

	if (keys[GLFW_KEY_3])
	{
		if (rotRodIzq > -45)
			rotRodIzq -= 1.0f;

	}

	if (keys[GLFW_KEY_4])
	{
		if (rotRodDer < 80.0f)
			rotRodDer += 1.0f;

	}

	if (keys[GLFW_KEY_5])
	{
		if (rotRodDer > -45)
			rotRodDer -= 1.0f;

	}

	if (keys[GLFW_KEY_6])
	{
		if (rotHomIzq < 110.0f)
			rotHomIzq += 1.0f;

	}

	if (keys[GLFW_KEY_7])
	{
		if (rotHomIzq > -45)
			rotHomIzq -= 1.0f;

	}

	if (keys[GLFW_KEY_8])
	{
		if (rotHomDer < 110.0f)
			rotHomDer += 1.0f;

	}

	if (keys[GLFW_KEY_9])
	{
		if (rotHomDer > -45)
			rotHomDer -= 1.0f;

	}

	if (keys[GLFW_KEY_Z])
	{
		if (rotCabeza < 45.0f)
			rotCabeza += 2.0f;

	}

	if (keys[GLFW_KEY_X])
	{
		if (rotCabeza > -45)
			rotCabeza -= 2.0f;

	}

	if (keys[GLFW_KEY_O])
	{
			rotSilla -= 1.0f;
	}

	if (keys[GLFW_KEY_P])
	{
		rotSilla += 1.0f;
	}

	if (keys[GLFW_KEY_C])
	{
		
		if (rotCaps1 < 115.0f)
		rotCaps1 += 1.0f;

	}

	if (keys[GLFW_KEY_V])
	{
		if (rotCaps1 > 5.0f)
		rotCaps1 -= 1.0f;

	}

	if (keys[GLFW_KEY_U])
	{

		if (rotP < 90.0f) {
			rotP += 1.0f;
			rotP2 -= 1.0f;
		}
	}

	if (keys[GLFW_KEY_I])
	{
		if (rotP > 0.0f) {
			rotP -= 1.0f;
			rotP2 += 1.0f;
		}
	}

	if (keys[GLFW_KEY_N])
	{

		if (rotP3 < 90.0f) {
			rotP3 += 1.0f;
			rotP4 -= 1.0f;
		}
	}

	if (keys[GLFW_KEY_M])
	{
		if (rotP3 > 0.0f) {
			rotP3 -= 1.0f;
			rotP4 += 1.0f;
		}
	}




	//Mov Meteoro
	if (keys[GLFW_KEY_DOWN])
	{
		MposZ += 0.05;
	}

	if (keys[GLFW_KEY_UP])
	{
		MposZ -= 0.08;
	}

	if (keys[GLFW_KEY_LEFT])
	{
		MposX -= 0.08;
	}

	if (keys[GLFW_KEY_RIGHT])
	{
		MposX += 0.08;
	}

	if (keys[GLFW_KEY_R])
	{
		MposY += 0.08;
	}

	if (keys[GLFW_KEY_T])
	{
		MposY -= 0.08;
	}

	if (keys[GLFW_KEY_LEFT_SHIFT])
	{
		rotMet += 3.0;
	}

	//Mov Personaje
	if (keys[GLFW_KEY_H])
	{
		posZ += 0.01;
	}

	if (keys[GLFW_KEY_Y])
	{
		posZ -= 0.01;
	}

	if (keys[GLFW_KEY_G])
	{
		posX -= 0.01;
	}

	if (keys[GLFW_KEY_J])
	{
		posX += 0.01;
	}



	// Camera controls
	if (keys[GLFW_KEY_W])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}