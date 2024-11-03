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

/*
	Texturas de Mario Bros
*/
//Entorno Piramide
Texture pyramid;
Texture arena;
Texture wood;
//Entorno Bowser

//Entorno Estrella

//Entorno Flores
Texture lava;

//Entorno Gomba

//Entorno Lava

//Entorno Nieve

//Entorno Nubes

//Entorno Rocoso

//Entorno Yoshi

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

//Modelos de Mario Bros
Model entornoArena;
Model entornoBowser;
Model entornoEstrella;
Model entornoFlores;
Model entornoGomba;
Model entornoLava;
Model entornoNieve;
Model entornoNubes;
Model entornoRocoso;
Model entornoYoshi;

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
	Shader *shader1 = new Shader();
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
	RoTexture.UseTexture();

	crearCasilla(0.0f, 0.0f); //1
	crearCasilla(30.3f, 0.0f); //4
	crearCasilla(60.6f, 0.0f); //7
	crearCasilla(90.9f, 0.0f); //10
	crearCasilla(90.9f, -30.3f); //13
	crearCasilla(90.9f, -90.9f); //19
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
	crearCasilla(90.9f, -40.4f); //14
	crearCasilla(90.9f, -70.7f); //17
	crearCasilla(90.9f, -101.1f); //20
	crearCasilla(50.5f, -111.1f); //25
	crearCasilla(0.0f, -111.1f); //30
	crearCasilla(0.0f, -90.9f); //32
	crearCasilla(0.0f, -40.4f); //37

	//casillas azules (fauna)
	AzTexture.UseTexture();

	crearCasilla(20.2f, 0.0f); //3
	crearCasilla(70.7f, 0.0f); //8
	crearCasilla(90.9f, -10.1f); //11
	crearCasilla(90.9f, -50.5f); //15
	crearCasilla(90.9f, -80.8f); //18
	crearCasilla(80.8f, -111.1f); //22
	crearCasilla(40.4f, -111.1f); //26
	crearCasilla(0.0f, -101.1f); //31
	crearCasilla(0.0f, -50.5f); //36
	crearCasilla(0.0f, -10.1f); //39

	//casillas verdes (flora)
	VeTexture.UseTexture();

	crearCasilla(40.4f, 0.0f); //5
	crearCasilla(90.9f, -60.6f); //16
	crearCasilla(90.9f, -111.1f); //21
	crearCasilla(70.7f, -111.1f); //23
	crearCasilla(30.3f, -111.1f); //27
	crearCasilla(10.1f, -111.1f); //29
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

	/*
			CARGANDO TEXTURAS DE MARIO BROS
	*/
	//Entorno de Arena
	pyramid = Texture("Textures/Piramide.png");
	pyramid.LoadTexture();
	arena = Texture("Textures/sand.png");
	arena.LoadTexture();
	wood = Texture("Textures/wood.png");
	wood.LoadTexture();

	//Entorno de Flores
	lava = Texture("Textures/lava.jpg");
	lava.LoadTexture();
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
	Minion = Model();
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

	//Cargando Modelos de Mario Bros
	entornoArena = Model();
	entornoArena.LoadModel("Models/MarioBros/entornoArena.obj");
	
	entornoBowser = Model();
	entornoBowser.LoadModel("Models/MarioBros/entornoBowser.obj");

	entornoEstrella = Model();
	entornoEstrella.LoadModel("Models/MarioBros/entornoEstrella.obj");

	entornoFlores = Model();
	entornoFlores.LoadModel("Models/MarioBros/entornoFlores.obj");

	entornoGomba = Model();
	entornoGomba.LoadModel("Models/MarioBros/entornoGomba.obj");

	entornoLava = Model();
	entornoLava.LoadModel("Models/MarioBros/entornoLava.obj");

	entornoNieve = Model();
	entornoNieve.LoadModel("Models/MarioBros/entornoNieve.obj");

	entornoNubes = Model();
	entornoNubes.LoadModel("Models/MarioBros/entornoNubes.obj");
	
	entornoRocoso = Model();
	entornoRocoso.LoadModel("Models/MarioBros/entornoRocoso.obj");

	entornoYoshi = Model();
	entornoYoshi.LoadModel("Models/MarioBros/entornoYoshi.obj");
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
		0.6f, 0.6f,
		0.0f, 0.0f, -1.0f);
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

		//Instancia de doom II mapa 1 cuarto 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 55.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//DoomIIMap1Room1.RenderModel();

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


		/*
					INSTANCIAS DE MARIO BROS
		*/

		

		//Entorno de flores
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		entornoFlores.RenderModel();

		//Reino champiñon
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 0.2f, 0.5f));
		model = glm::scale(model, glm::vec3(2.2f, 2.2f, 2.2f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		entornoGomba.RenderModel();

		//Isla de Yoshi
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		entornoYoshi.RenderModel();

		//Reino de nube
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.9f, 0.5f, -0.5f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		entornoNubes.RenderModel();

		//Tierra sorbete
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.9f, 0.5f, -80.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		entornoNieve.RenderModel();

		//Entorno de piramide
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.0f, 0.0f, -100.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		entornoArena.RenderModel();

		//Mundo rocoso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.5f, 0.0f, -110.5f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		entornoRocoso.RenderModel();

		//Reino Koopa 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.5f, 0.0f, -110.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		entornoBowser.RenderModel();

		// Camino estrella 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -90.5f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		entornoEstrella.RenderModel();

		// Reino de lava
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -20.5f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		entornoLava.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
