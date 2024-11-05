/*
	PROYECTO FINAL CGIHC
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <random>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h" 
#include "Skybox.h"

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animaci�n
int counter = 0;
int framesAnim = 0;
int framesMov = 0;
int numD4 = 0;
int numD8 = 0;
int numTotal = 0;
int casAct = 0;
float rotaDado4X;
float rotaDado4Y;
float rotaDado4Z;
float dirDado4X;
float dirDado4Y;
float dirDado4Z;
float rotaDado8X;
float rotaDado8Y;
float rotaDado8Z;
float dirDado8X;
float dirDado8Y;
float dirDado8Z;
float posDados;
int textures ;
GLfloat contadorDAYNIGHT = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;

Texture AmTexture;
Texture AzTexture;
Texture RoTexture;
Texture VeTexture;
Texture D4Texture;
Texture D8Texture;
Texture TestTexture;


//MUNDO BELLA 

//TEXTURAS DIA
Texture lumiere;
Texture rosa;
Texture gaston;
Texture crosworth;
Texture bestia;
Texture plumette;
Texture castillo;
Texture madame;
Texture mrpotts;
Texture chip;

//TEXTURAS DIA COCINADAS
Texture lumiereC;
Texture rosaC;
Texture gastonC;
Texture crosworthC;
Texture bestiaC;
Texture plumetteC;
Texture castilloC;
Texture madameC;
Texture mrpottsC;
Texture chipC;

//TEXTURAS NOCHE

Texture lumiereN;
Texture rosaN;
Texture gastonN;
Texture crosworthN;
Texture bestiaN;
Texture plumetteN;
Texture castilloN;
Texture madameN;
Texture mrpottsN;
Texture chipN;
Skybox skyboxN;

//TEXTURAS NOCHE COCINADAS

Texture lumiereNC;
Texture rosaNC;
Texture gastonNC;
Texture crosworthNC;
Texture bestiaNC;
Texture plumetteNC;
Texture castilloNC;
Texture madameNC;
Texture mrpottsNC;
Texture chipNC;

Texture genericT;

//Minion
Model Minion;
Model MinionMorado;
Model Vector;
Model Gru;
Model CarroGru;
Model FortalezaVector;
Model MinionBebe;
Model MacetaMinion;
Model MinionMaquinaDulces;
Model DoomIIMap1Room1;

//Bella
Model mrpottsT;
Model lumiereT;
Model cogsworthT;
Model bellaT;
Model gastonT;
Model bestiaT;
Model rosaT;
Model castilloT;
Model chipT;
Model plumetteT;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";
// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//c�lculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] =
	{
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] =
	{
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};


	GLfloat casillaVertices[] =
	{
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f
	};

	unsigned int casillaIndices[] =
	{
		0, 2, 1,
		1, 2, 3
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(casillaVertices, casillaIndices, 32, 6);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void crearDados()
{
	GLfloat verticesOcta[] =
	{
		// tras sup 5
		-1.0f, 0.0f, -1.0f,		0.412f, 0.010f,		0.0f, -1.0f, 1.0f, // izq abajo
		1.0f, 0.0f, -1.0f,		0.006f, 0.010f,		0.0f, -1.0f, 1.0f, // der abajo
		0.0f,  1.0f, 0.0f, 		0.203f, 0.398f,		0.0f, -1.0f, 1.0f, // punta sup
		// der sup 3
		1.0f, 0.0f, -1.0f,		0.796f, 0.496f,		-1.0f, -1.0f, 0.0f, // der abajo
		1.0f, 0.0f,  1.0f,		0.402f, 0.496f,		-1.0f, -1.0f, 0.0f, // der arriba
		0.0f,  1.0f, 0.0f, 		0.600f, 0.887f,		-1.0f, -1.0f, 0.0f, // punta sup
		// front sup 1
		-1.0f, 0.0f,  1.0f,		0.006f, 0.500f,		0.0f, -1.0f, -1.0f, // izq arriba
		1.0f, 0.0f,  1.0f,		0.398f, 0.500f,		0.0f, -1.0f, -1.0f, // der arriba
		0.0f,  1.0f, 0.0f, 		0.201f, 0.891f,		0.0f, -1.0f, -1.0f, // punta sup
		// izq sup 7
		-1.0f, 0.0f,  1.0f,		0.799f, 0.010f,		1.0f, -1.0f, 0.0f, // izq arriba
		-1.0f, 0.0f, -1.0f,		0.420f, 0.010f,		1.0f, -1.0f, 0.0f, // izq abajo
		0.0f,  1.0f, 0.0f, 		0.600f, 0.398f,		1.0f, -1.0f, 0.0f, // punta sup

		// front inf 2
		-1.0f, 0.0f, -1.0f,		0.598f, 0.463f,		0.0f, 1.0f, 1.0f, // izq abajo
		1.0f, 0.0f, -1.0f,		0.203f, 0.463f,		0.0f, 1.0f, 1.0f, // der abajo
		0.0f, -1.0f, 0.0f, 		0.400f, 0.066f,		0.0f, 1.0f, 1.0f, // punta inf
		// der inf 4
		1.0f, 0.0f, -1.0f,		0.996f, 0.463f,		-1.0f, 1.0f, 0.0f, // der abajo
		1.0f, 0.0f,  1.0f,		0.600f, 0.463f,		-1.0f, 1.0f, 0.0f, // der arriba
		0.0f, -1.0f, 0.0f, 		0.799f, 0.066f,		-1.0f, 1.0f, 0.0f, // punta inf
		// tras inf 6
		-1.0f, 0.0f,  1.0f,		0.203f, 0.970f,		0.0f, 1.0f, -1.0f, // izq arriba
		1.0f, 0.0f,  1.0f,		0.598f, 0.970f,		0.0f, 1.0f, -1.0f, // der arriba
		0.0f, -1.0f, 0.0f, 		0.400f, 0.596f,		0.0f, 1.0f, -1.0f, // punta inf
		// izq inf 8
		-1.0f, 0.0f,  1.0f,		0.996f, 0.970f,		1.0f, 1.0f, 0.0f, // izq arriba
		-1.0f, 0.0f, -1.0f,		0.600f, 0.970f,		1.0f, 1.0f, 0.0f, // izq abajo
		0.0f, -1.0f, 0.0f, 		0.799f, 0.596f,		1.0f, 1.0f, 0.0f  // punta inf
	};

	GLuint indicesOcta[] =
	{
		// piramide cuadrangular superior
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,

		// piramide cuadrangular inferior
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23
	};

	GLfloat verticesCuad[] =
	{
		// front  1
		-1.0f, 0.0f, 0.0f,		0.006f, 0.492f,		0.0f, -1.0f, -1.0f, // izq abajo
		1.0f, 0.0f, 0.0f,		0.492f, 0.492f,		0.0f, -1.0f, -1.0f, // der abajo
		0.0f,  1.5f, -0.865f, 	0.246f, 0.994f,		0.0f, -1.0f, -1.0f, // punta sup
		// der  2
		1.0f, 0.0f, 0.0f,		0.500f, 0.500f,		-1.0f, -1.0f, 0.0f, // der abajo
		0.0f, 0.0f,  -1.73f,	0.994f, 0.500f,		-1.0f, -1.0f, 0.0f, // trasera
		0.0f,  1.5f, -0.865f, 	0.744f, 0.994f,		-1.0f, -1.0f, 0.0f, // punta sup
		// izq  3
		-1.0f, 0.0f, 0.0f,		0.492f, 0.006f,		1.0f, -1.0f, 0.0f, // izq abajo
		0.0f, 0.0f,  -1.73f,	0.006f, 0.006f,		1.0f, -1.0f, 0.0f, // trasera
		0.0f,  1.5f, -0.865f, 	0.246f, 0.490f,		1.0f, -1.0f, 0.0f, // punta sup
		// base 4
		-1.0f, 0.0f, 0.0f,		0.990f, 0.006f,		0.0f, 1.0f, 0.0f, // izq abajo
		1.0f, 0.0f, 0.0f,		0.500f, 0.006f,		0.0f, 1.0f, 0.0f, // der abajo
		0.0f, 0.0f,  -1.73f,	0.744f, 0.492f,		0.0f, 1.0f, 0.0f  // trasera
	};

	GLuint indicesCuad[] =
	{
		// piramide triangular 
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
	};

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(verticesOcta, indicesOcta, 192, 24);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(verticesCuad, indicesCuad, 96, 12);
	meshList.push_back(obj6);
}

void crearCasilla(float posX, float posZ)
{
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	shaderList[0].UseShader();
	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformColor = shaderList[0].getColorLocation();
	glm::mat4 model(1.0);
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(posX, 0.2f, posZ)); //para mover todo el tablero se restan o suman valores a posX y posZ
	model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	meshList[3]->RenderMesh();
	//printf("+");
}

void crearTablero()
{
	//numeros de casillas considerando que se empieza en esquina
	//inferior izquierda y se recorre hacia la derecha, arriba, izquierda, abajo.
	//printf("casillas: ");
	//casillas rojas (seres vivos)

	rosa.UseTexture();
	
	/*
	if (textures) {
		mrpottsNC.UseTexture();
	}
	else {
		mrpottsN.UseTexture();
	}
	
	if (contadorDAYNIGHT <= 40.0f) {
		mrpottsN.UseTexture(); // Usa la primera textura
	}
	else if (contadorDAYNIGHT == 25.0f) {
		mrpottsNC.UseTexture(); // Usa la segunda textura
	}
	else if (contadorDAYNIGHT <= 100.0f) {
		mrpotts.UseTexture(); // Usa la tercera textura
	}
	else if (contadorDAYNIGHT == 70.0f) {
		mrpottsC.UseTexture(); // Usa la cuarta textura
	}*/

	crearCasilla(0.0f, 0.0f); //1

	

	/*
	if (textures) {
		
			chip.UseTexture(); 
		
	}
	else {
		chipN.UseTexture();
	}*/

	crosworth.UseTexture();
	crearCasilla(30.3f, 0.0f); //4
	

	crearCasilla(60.6f, 0.0f); //7
	

	crearCasilla(90.9f, 0.0f); //10
	

	crearCasilla(90.9f, -30.3f); //13
	

	crearCasilla(90.9f, -90.9f); //19

	chip.UseTexture();
	crearCasilla(60.6f, -111.1f); //24
	

	crearCasilla(20.2f, -111.1f); //28
	

	crearCasilla(0.0f, -70.7f); //34
	

	crearCasilla(0.0f, -30.3f); //38

	//casillas amarillas (edificios)
	AmTexture.UseTexture();

	crearCasilla(10.1f, 0.0f); //2
	crearCasilla(50.5f, 0.0f); //6
	crearCasilla(80.8f, 0.0f); //9
	crearCasilla(90.9f, -20.2f); //12
	gaston.UseTexture();
	crearCasilla(90.9f, -40.4f); //14
	mrpotts.UseTexture();
	crearCasilla(90.9f, -70.7f); //17
	crearCasilla(90.9f, -101.1f); //20
	crearCasilla(50.5f, -111.1f); //25
	crearCasilla(0.0f, -111.1f); //30
	crearCasilla(0.0f, -90.9f); //32

	lumiere.UseTexture();
	crearCasilla(0.0f, -40.4f); //37

	//casillas azules (fauna)
	AzTexture.UseTexture();

	crearCasilla(20.2f, 0.0f); //3
	castillo.UseTexture();
	crearCasilla(70.7f, 0.0f); //8
	crearCasilla(90.9f, -10.1f); //11
	crearCasilla(90.9f, -50.5f); //15
	crearCasilla(90.9f, -80.8f); //18
	crearCasilla(80.8f, -111.1f); //22
	crearCasilla(40.4f, -111.1f); //26
	crearCasilla(0.0f, -101.1f); //31
	plumette.UseTexture();
	crearCasilla(0.0f, -50.5f); //36
	crearCasilla(0.0f, -10.1f); //39

	//casillas verdes (flora)
	VeTexture.UseTexture();

	crearCasilla(40.4f, 0.0f); //5
	crearCasilla(90.9f, -60.6f); //16
	crearCasilla(90.9f, -111.1f); //21
	crearCasilla(70.7f, -111.1f); //23
	bestia.UseTexture();
	crearCasilla(30.3f, -111.1f); //27
	crearCasilla(10.1f, -111.1f); //29
	madame.UseTexture();
	crearCasilla(0.0f, -80.8f); //33
	crearCasilla(0.0f, -60.6f); //35
	TestTexture.UseTexture();
	crearCasilla(0.0f, -20.2f); //39
	//printf("\n\n");
}

void cargarTexturas()
{
	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AmTexture = Texture("Textures/amarillo.png");
	AmTexture.LoadTextureA();
	RoTexture = Texture("Textures/rojo.png");
	RoTexture.LoadTextureA();
	AzTexture = Texture("Textures/azul.png");
	AzTexture.LoadTextureA();
	VeTexture = Texture("Textures/verde.png");
	VeTexture.LoadTextureA();
	D4Texture = Texture("Textures/dado4.png");
	D4Texture.LoadTextureA();
	D8Texture = Texture("Textures/dado8.png");
	D8Texture.LoadTextureA();
	TestTexture = Texture("Textures/textTest.png");
	TestTexture.LoadTextureA();


	//TEXTURAS MUNDO BELLA

	lumiereN = Texture("Textures/lumierenight.png");
	lumiereN.LoadTextureA();
	rosaN = Texture("Textures/rosanight.png");
	rosaN.LoadTextureA();
	gastonN = Texture("Textures/gastonnight.png");
	gastonN.LoadTextureA();
	crosworthN = Texture("Textures/cogsworthnight.png");
	crosworthN.LoadTextureA();
	bestiaN = Texture("Textures/bestianight.png");
	bestiaN.LoadTextureA();
	plumetteN = Texture("Textures/plumettenight.png");
	plumetteN.LoadTextureA();
	castilloN = Texture("Textures/castillonight.png");
	castilloN.LoadTextureA();
	madameN = Texture("Textures/madamenight.png");
	madameN.LoadTextureA();
	mrpottsN = Texture("Textures/mrpottsnight.png");
	mrpottsN.LoadTextureA();
	chipN = Texture("Textures/chipnight.png");
	chipN.LoadTextureA();

	lumiereNC = Texture("Textures/lumierenightcook.png");
	lumiereNC.LoadTextureA();
	rosaNC = Texture("Textures/rosanightcook.png");
	rosaNC.LoadTextureA();
	gastonNC = Texture("Textures/gastonnightcook.png");
	gastonNC.LoadTextureA();
	crosworthNC = Texture("Textures/cogsworthnightcook.png");
	crosworthNC.LoadTextureA();
	bestiaNC = Texture("Textures/bestianightcook.png");
	bestiaNC.LoadTextureA();
	plumetteNC = Texture("Textures/plumettenightcook.png");
	plumetteNC.LoadTextureA();
	castilloNC = Texture("Textures/castillonightcook.png");
	castilloNC.LoadTextureA();
	madameNC = Texture("Textures/madamenightcook.png");
	madameNC.LoadTextureA();
	mrpottsNC = Texture("Textures/mrpottsnightcook.png");
	mrpottsNC.LoadTextureA();
	chipNC = Texture("Textures/chipnightcook.png");
	chipNC.LoadTextureA();


	lumiere = Texture("Textures/lumiere.png");
	lumiere.LoadTextureA();
	rosa = Texture("Textures/rosa.png");
	rosa.LoadTextureA();
	gaston = Texture("Textures/gaston.png");
	gaston.LoadTextureA();
	crosworth = Texture("Textures/cogsworth.png");
	crosworth.LoadTextureA();
	bestia = Texture("Textures/bestia.png");
	bestia.LoadTextureA();
	plumette = Texture("Textures/plumette.png");
	plumette.LoadTextureA();
	castillo = Texture("Textures/castillo.png");
	castillo.LoadTextureA();
	madame = Texture("Textures/madame.png");
	madame.LoadTextureA();
	mrpotts = Texture("Textures/mrpotts.png");
	mrpotts.LoadTextureA();
	chip = Texture("Textures/chip.png");
	chip.LoadTextureA();

	lumiereC = Texture("Textures/lumierecook.png");
	lumiereC.LoadTextureA();
	rosaC = Texture("Textures/rosacook.png");
	rosaC.LoadTextureA();
	gastonC = Texture("Textures/gastoncook.png");
	gastonC.LoadTextureA();
	crosworthC = Texture("Textures/cogsworthcook.png");
	crosworthC.LoadTextureA();
	bestiaC = Texture("Textures/bestiacook.png");
	bestiaC.LoadTextureA();
	plumetteC = Texture("Textures/plumettecook.png");
	plumetteC.LoadTextureA();
	castilloC = Texture("Textures/castillocook.png");
	castilloC.LoadTextureA();
	madameC = Texture("Textures/madamecook.png");
	madameC.LoadTextureA();
	mrpottsC = Texture("Textures/mrpottscook.png");
	mrpottsC.LoadTextureA();
	chipC = Texture("Textures/chipcook.png");
	chipC.LoadTextureA();

	genericT = Texture("Textures/Gru");
	genericT.LoadTextureA();


}

void animacionCaida()
{
	posDados = posDados - 0.025f;
	//printf("[Frame %d] Pos dado: %f\n", framesAnim, posDadoY);
}

void animacionGiroD4(float rotaX, float rotaY, float rotaZ)
{
	dirDado4X = dirDado4X + rotaX;
	dirDado4Y = dirDado4Y + rotaY;
	dirDado4Z = dirDado4Z + rotaZ;
	//printf("[Frame %d] Dir dado: %f, %f, %f\n", framesAnim, dirDadoX, dirDadoY, dirDadoZ);
}

void animacionGiroD8(float rotaX, float rotaY, float rotaZ)
{
	dirDado8X = dirDado8X + rotaX;
	dirDado8Y = dirDado8Y + rotaY;
	dirDado8Z = dirDado8Z + rotaZ;
	//printf("[Frame %d] Dir dado: %f, %f, %f\n", framesAnim, dirDadoX, dirDadoY, dirDadoZ);
}

void cargarModelos()
{
	/*
	Minion = Model();
	Minion.LoadModel("Models/MinionHulalTexturizado.obj");
	Minion.LoadModel("Models/MinionHulalTexturizado.obj");

	MacetaMinion = Model();
	MacetaMinion.LoadModel("Models/MacetaMinionTexturizada.obj");

	MinionBebe = Model();
	MinionBebe.LoadModel("Models/MinionBebeTexturizado2.obj");
	MinionBebe.LoadModel("Models/MinionBebeTexturizado2.obj");

	MinionMorado = Model();
	MinionMorado.LoadModel("Models/MinionMoradoTexturizado.obj");

	Vector = Model();
	Vector.LoadModel("Models/VectorTexturizado.obj");

	Gru = Model();
	Gru.LoadModel("Models/GruTexturizado.obj");

	CarroGru = Model();
	CarroGru.LoadModel("Models/CarroGruTexturizado3.obj");

	FortalezaVector = Model();
	FortalezaVector.LoadModel("Models/FortalezaVectorTexturizada4.obj");

	MinionMaquinaDulces = Model();
	MinionMaquinaDulces.LoadModel("Models/MinionMaquinaDulcesTexturizado.obj");

	DoomIIMap1Room1 = Model();
	DoomIIMap1Room1.LoadModel("Models/DOOMIIM2_IR.obj");
	*/
	//Bella
	
	mrpottsT = Model();
	mrpottsT.LoadModel("Models/mrpotts.obj");



	
	gastonT = Model();
	gastonT.LoadModel("Models/gaston2.obj");
	
	bestiaT = Model();
	bestiaT.LoadModel("Models/thebeastsecond.obj");
	
	rosaT = Model();
	rosaT.LoadModel("Models/rosatexturizada.obj");
	
	castilloT = Model();
	castilloT.LoadModel("Models/castillo.obj");
	
	chipT = Model();
	chipT.LoadModel("Models/chip.obj");
	
	plumetteT = Model();
	plumetteT.LoadModel("Models/plumette.obj");
	
	lumiereT = Model();
	lumiereT.LoadModel("Models/lumieretexturizado.obj");

	cogsworthT = Model();
	cogsworthT.LoadModel("Models/cogsworthtextre.obj");


	bellaT = Model();
	bellaT.LoadModel("Models/bella.obj");
	
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	crearDados();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	cargarTexturas();
	cargarModelos();

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.6f, 0.6f, //atenuacion
		450.0f, 75.0f, 0.0f);
	//contador de luces puntuales


	unsigned int pointLightCount = 0;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;



	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	rotaDado4X = 0.0f;
	rotaDado4Y = 0.0f;
	rotaDado4Z = 0.0f;
	rotaDado8X = 0.0f;
	rotaDado8Y = 0.0f;
	rotaDado8Z = 0.0f;
	posDados = 6.35f;

	printf("[F]\tTirar los dados.\n");

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Cambio de texturas entre el dia y la noche 

		contadorDAYNIGHT += deltaTime;

		
		if (contadorDAYNIGHT >= 100.0f) {
			textures = !textures;
			contadorDAYNIGHT = 0.0f;
		}
		/*
		if (contadorDAYNIGHT >= 100.0f) {
			
			contadorDAYNIGHT = 0.0f;
		}*/
		//Tirada Dados

		if (mainWindow.getTiroDados())
		{
			mainWindow.resetTiroDados();

			numD4 = mainWindow.getNumDado4();
			printf(">>Numero de dado blanco: [%d]\n", numD4);

			switch (numD4)
			{
			case 1:
				rotaDado4X = -0.3f; rotaDado4Y = 0.0f; rotaDado4Z = 0.0f;
				break;

			case 2:
				rotaDado4X = -0.35f; rotaDado4Y = -0.6f; rotaDado4Z = 0.0f;
				break;

			case 3:
				rotaDado4X = -0.35f; rotaDado4Y = 0.6f; rotaDado4Z = 0.0f;
				break;

			case 4:
				rotaDado4X = 0.9f; rotaDado4Y = 0.9f; rotaDado4Z = 0.0f;
				break;

			default:
				break;
			}

			numD8 = mainWindow.getNumDado8();
			printf(">>Numero de dado negro: [%d]\n", numD8);

			switch (numD8)
			{
			case 1:
				rotaDado8X = -0.25f; rotaDado8Y = 0.0f; rotaDado8Z = 0.0f;
				break;

			case 2:
				rotaDado8X = -1.12; rotaDado8Y = 0.0f; rotaDado8Z = 0.0f;
				break;

			case 3:
				rotaDado8X = -0.22f; rotaDado8Y = -0.45f; rotaDado8Z = 0.0f;
				break;

			case 4:
				rotaDado8X = 0.68f; rotaDado8Y = 0.45f; rotaDado8Z = 0.0f;
				break;

			case 5:
				rotaDado8X = -0.22f; rotaDado8Y = 0.9f; rotaDado8Z = 0.0f;
				break;

			case 6:
				rotaDado8X = 0.68f; rotaDado8Y = 0.9f; rotaDado8Z = 0.0f;
				break;

			case 7:
				rotaDado8X = -0.22f; rotaDado8Y = 0.45f; rotaDado8Z = 0.0f;
				break;

			case 8:
				rotaDado8X = 0.68f; rotaDado8Y = 1.35f; rotaDado8Z = 0.0f;
				break;

			default:
				break;
			}
			numTotal = numD4 + numD8;
			printf("El personaje avanza %d casillas.\n", numTotal);
			casAct += numTotal; //contador de casilla actual
			printf("El personaje se encuentra en la casilla [%d]\n\n", casAct);
			framesAnim = 1;
		}

		if (framesAnim == 1)
		{
			printf("[Animacion comenzada]\n");
			posDados = 6.35f;
			dirDado4X = 0.0f;
			dirDado4Y = 0.0f;
			dirDado4Z = 0.0f;
			dirDado8X = 0.0f;
			dirDado8Y = 0.0f;
			dirDado8Z = 0.0f;
			framesAnim = 2;
		}
		else if (framesAnim >= 2 && framesAnim < 200)
		{
			animacionCaida();
			framesAnim++;
		}
		else if (framesAnim >= 200 && framesAnim < 400)
		{
			animacionGiroD4(rotaDado4X, rotaDado4Y, rotaDado4Z);
			animacionGiroD8(rotaDado8X, rotaDado8Y, rotaDado8Z);
			framesAnim++;
		}
		else if (framesAnim == 400)
		{
			printf("[Animacion terminada]\n\n\n");
			framesAnim = 0;
		}

		/*
		Hacer:
			if casAct es igual a indice de una esquina
				se hace rotacion del personaje

			contador de 1 en 1 para casAct, para lograr que
			if casAct == 40
				reiniciar a casAct = 0 y seguir sumando los restantes
			prender bandera de paso por inicio para animaciones especiales

			escalar modelos a tamano similar
		*/

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la c�mara de tipo flash
		// sirve para que en tiempo de ejecuci�n (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);
		//shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		crearTablero();

		D8Texture.UseTexture();

		model = glm::mat4(1.0); //dado 8
		model = glm::translate(model, glm::vec3(55.0f, posDados, -55.0f));
		model = glm::scale(model, glm::vec3(1.4f, 1.6f, 1.4f));
		model = glm::rotate(model, dirDado8X * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, dirDado8Y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, dirDado8Z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[4]->RenderMesh();

		D4Texture.UseTexture();

		model = glm::mat4(1.0); //dado 4
		model = glm::translate(model, glm::vec3(45.0f, 0.5f + posDados, -55.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, dirDado4X * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, dirDado4Y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, dirDado4Z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[5]->RenderMesh();

		/*
		/////////////////////////////////////////////////////////////////////////////////////////
								INCLUIR ABAJO INSTANCIAS DE MODELOS
		/////////////////////////////////////////////////////////////////////////////////////////
		*/
		/*
		//Instancia de doom II mapa 1 cuarto 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 55.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DoomIIMap1Room1.RenderModel();

		//Instancia del minion 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Minion.RenderModel();

		//Instancia de maceta minion 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 20.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//MacetaMinion.RenderModel();

		//Instancia del minion morado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, -2.0f));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//MinionMorado.RenderModel();

		//Instancia del Dany flow (trash)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.0f, 0.0f, -20.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Vector.RenderModel();

		//Instancia de gru
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, 0.0f, -20.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Gru.RenderModel();

		//Instancia de carro de gru
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-75.0f, 0.0f, -60.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//CarroGru.RenderModel();

		//Instancia de maquina dulces minion
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -60.0f));
		model = glm::scale(model, glm::vec3(5.2f, 5.2f, 5.2f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//MinionMaquinaDulces.RenderModel();

		//Instancia de Fortaleza de vector
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(75.0f, 0.0f, 60.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//FortalezaVector.RenderModel();

		//Instancia del minion bebe 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, -10.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//MinionBebe.RenderModel();
		*/

		//INSTANCIAS MUNDO BELLA 
		//Instancia mrpotts

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.9f, 1.0f, -70.70f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.80f, 0.80f, 0.80f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mrpottsT.RenderModel();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.3f, 0.2f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(.05f, .05f, .05f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cogsworthT.RenderModel();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.9f, 1.0f, -40.40f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		gastonT.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.3f, 0.2f, -111.1f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bestiaT.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rosaT.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.7f, 0.2f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(.05f, .05f, .05f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		castilloT.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.6f, 0.2f, -111.1f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		chipT.RenderModel();
		
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -50.5f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(.03f, .03f, .03f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plumetteT.RenderModel();

						
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -40.4f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(.1f, .1f, .1f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lumiereT.RenderModel();
		//AVATAR
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bellaT.RenderModel();
		
		
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
