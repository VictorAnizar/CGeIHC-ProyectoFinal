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
float rotaCanon = 0;
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

//Texturas tablero
Texture AmTexture;
Texture AzTexture;
Texture RoTexture;
Texture VeTexture;
Texture D4Texture;
Texture D8Texture;
Texture TestTexture;

//Texturas Doom 
Texture Doom1Tex;
Texture Doom2Tex;
Texture Doom3Tex;
Texture Doom4Tex;
Texture Doom5Tex;
Texture Doom6Tex;
Texture Doom7Tex;
Texture Doom8Tex;
Texture Doom9Tex;
Texture Doom10Tex;
//Texture Doom11Tex;

//Texturas Minion
Texture MinionTexture;
Texture EvilMinionTexture;
Texture MaquinaDulcesMinionTexture;
Texture MacetMinionTexture;
Texture HulaMinionTexture;
Texture GruTexture;
Texture CarroGruTexture;
Texture BabyMinionTexture;
Texture VectorTexture;
Texture VectorFortressTexture;

//Modelos Minion
Model MinionHula;
Model MinionNormal;
Model MinionMorado;
Model Vector;
Model Gru;
Model CarroGru;
Model FortalezaVector;
Model MinionBebe;
Model MacetaMinion;
Model MinionMaquinaDulces;

//Modelos Doom
Model DoomIIMap1Room1;
Model DoomIIMap1Room2;
Model DoomE1M1Room1;
Model DoomE1M1Room2;
Model RevCuerpo;
Model RevCanons;
Model ZombieCuerpo;
Model ZombiePiernaD;
Model ZombiePiernaI;
Model ZombieBrazoD;
Model ZombieBrazoI;
Model ArachCuerpo;
Model ArachCanon;
Model WolfCuerpo;
Model WolfCabeza;
Model GargCuerpo;
Model GargAlaD;
Model GargAlaI;
Model GargBrazoD;
Model GargBrazoI;
Model Cacodemon;
Model Dopefish;

//Modelos entorno
Model Lampara;

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
	Doom6Tex.UseTexture();
	crearCasilla(90.9f, -30.3f); //13
	GruTexture.UseTexture();
	crearCasilla(90.9f, -90.9f); //19
	RoTexture.UseTexture();
	crearCasilla(60.6f, -111.1f); //24
	crearCasilla(20.2f, -111.1f); //28
	Doom9Tex.UseTexture();
	crearCasilla(0.0f, -70.7f); //34
	MinionTexture.UseTexture();
	crearCasilla(0.0f, -30.3f); //38


	//casillas amarillas (fauna)
	AmTexture.UseTexture();
	crearCasilla(10.1f, 0.0f); //2
	EvilMinionTexture.UseTexture();
	crearCasilla(50.5f, 0.0f); //6
	Doom8Tex.UseTexture();
	crearCasilla(80.8f, 0.0f); //9
	BabyMinionTexture.UseTexture();
	crearCasilla(90.9f, -20.2f); //12
	Doom4Tex.UseTexture();
	crearCasilla(90.9f, -40.4f); //14
	AmTexture.UseTexture();
	crearCasilla(90.9f, -70.7f); //17
	crearCasilla(90.9f, -101.1f); //20
	Doom5Tex.UseTexture();
	crearCasilla(50.5f, -111.1f); //25
	VectorFortressTexture.UseTexture();
	crearCasilla(0.0f, -111.1f); //30
	AmTexture.UseTexture();
	crearCasilla(0.0f, -90.9f); //32
	crearCasilla(0.0f, -40.4f); //37

	//casillas azules (edificios)
	CarroGruTexture.UseTexture();
	crearCasilla(20.2f, 0.0f); //3
	AzTexture.UseTexture();
	crearCasilla(70.7f, 0.0f); //8
	Doom7Tex.UseTexture();
	crearCasilla(90.9f, -10.1f); //11
	AzTexture.UseTexture();
	crearCasilla(90.9f, -50.5f); //15
	crearCasilla(90.9f, -80.8f); //18
	crearCasilla(80.8f, -111.1f); //22
	MaquinaDulcesMinionTexture.UseTexture();
	crearCasilla(40.4f, -111.1f); //26
	Doom2Tex.UseTexture();
	crearCasilla(0.0f, -101.1f); //31
	AzTexture.UseTexture();
	crearCasilla(0.0f, -50.5f); //36
	crearCasilla(0.0f, -10.1f); //39

	//casillas verdes (flora)
	VeTexture.UseTexture();
	crearCasilla(40.4f, 0.0f); //5
	MacetMinionTexture.UseTexture();
	crearCasilla(90.9f, -60.6f); //16
	Doom10Tex.UseTexture();
	crearCasilla(90.9f, -111.1f); //21
	HulaMinionTexture.UseTexture();
	crearCasilla(70.7f, -111.1f); //23
	VeTexture.UseTexture();
	crearCasilla(30.3f, -111.1f); //27
	Doom3Tex.UseTexture();
	crearCasilla(10.1f, -111.1f); //29
	VeTexture.UseTexture();
	crearCasilla(0.0f, -80.8f); //33
	VectorTexture.UseTexture();
	crearCasilla(0.0f, -60.6f); //35
	Doom1Tex.UseTexture();
	crearCasilla(0.0f, -20.2f); //39
	VeTexture.UseTexture();
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
	pisoTexture = Texture("Textures/pisonuevo.tga");
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

	Doom1Tex = Texture("Textures/casdoom0.png");
	Doom1Tex.LoadTextureA();
	Doom2Tex = Texture("Textures/casdoom1.png");
	Doom2Tex.LoadTextureA();
	Doom3Tex = Texture("Textures/casdoom2.png");
	Doom3Tex.LoadTextureA();
	Doom4Tex = Texture("Textures/casdoom3.png");
	Doom4Tex.LoadTextureA();
	Doom5Tex = Texture("Textures/casdoom4.png");
	Doom5Tex.LoadTextureA();
	Doom6Tex = Texture("Textures/casdoom5.png");
	Doom6Tex.LoadTextureA();
	Doom7Tex = Texture("Textures/casdoom6.png");
	Doom7Tex.LoadTextureA();
	Doom8Tex = Texture("Textures/casdoom7.png");
	Doom8Tex.LoadTextureA();
	Doom9Tex = Texture("Textures/casdoom8.png");
	Doom9Tex.LoadTextureA();
	Doom10Tex = Texture("Textures/casdoom9.png");
	Doom10Tex.LoadTextureA();

	MinionTexture = Texture("Textures/MinionNormal.png");
	MinionTexture.LoadTextureA();

	EvilMinionTexture = Texture("Textures/MinionMorado.png");
	EvilMinionTexture.LoadTextureA();
	
	MaquinaDulcesMinionTexture = Texture("Textures/MaquinaDulcesMinion.png");
	MaquinaDulcesMinionTexture.LoadTextureA();

	MacetMinionTexture = Texture("Textures/MacetaMinion.png");
	MacetMinionTexture.LoadTextureA();

	HulaMinionTexture = Texture("Textures/HulaMinion.png");
	HulaMinionTexture.LoadTextureA();

	GruTexture = Texture("Textures/Gru.png");
	GruTexture.LoadTextureA();

	CarroGruTexture = Texture("Textures/CarroGru.png");
	CarroGruTexture.LoadTextureA();

	VectorTexture = Texture("Textures/Vector.png");
	VectorTexture.LoadTextureA();

	BabyMinionTexture = Texture("Textures/BabyMinion.png");
	BabyMinionTexture.LoadTextureA();

	VectorFortressTexture = Texture("Textures/VectorFortress.png");
	VectorFortressTexture.LoadTextureA();

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
	//Iluminacion
	Lampara = Model();
	Lampara.LoadModel("Models/lampara.obj");

	//Minions
	MinionNormal = Model();
	MinionNormal.LoadModel("Models/MinionNormalTexturizado.obj");

	MinionHula = Model();
	MinionHula.LoadModel("Models/MinionHulalTexturizado.obj");

	MacetaMinion = Model();
	MacetaMinion.LoadModel("Models/MacetaMinionTexturizada.obj");
	
	MinionBebe = Model();
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

	//Doom
	GargCuerpo = Model();
	GargCuerpo.LoadModel("Models/Gargoyle_Cuerpo.obj"); 
	GargAlaI = Model();
	GargAlaI.LoadModel("Models/Gargoyle_AlaIzq.obj");
	GargAlaD = Model();
	GargAlaD.LoadModel("Models/Gargoyle_AlaDer.obj");
	printf("Cargado\n");
	GargBrazoI = Model();
	GargBrazoI.LoadModel("Models/Gargoyle_BrIzq.obj");
	printf("Cargado\n");
	GargBrazoD = Model();
	GargBrazoD.LoadModel("Models/Gargoyle_BrDer.obj");
	printf("Cargado\n");
	Cacodemon = Model();
	Cacodemon.LoadModel("Models/Cacodemon.obj");

	WolfCuerpo = Model();
	WolfCuerpo.LoadModel("Models/Wolf_Cuerpo.obj");
	WolfCabeza = Model();
	WolfCabeza.LoadModel("Models/Wolf_Cabeza.obj");
	
	RevCuerpo = Model();
	RevCuerpo.LoadModel("Models/Rev_Cuerpo.obj");
	RevCanons = Model();
	RevCanons.LoadModel("Models/Rev_Canons.obj");
	
	ZombieCuerpo = Model();
	ZombieCuerpo.LoadModel("Models/Zombie_Cuerpo.obj");
	ZombiePiernaD = Model();
	ZombiePiernaD.LoadModel("Models/Zombie_PiernaDer.obj");
	ZombiePiernaI = Model();
	ZombiePiernaI.LoadModel("Models/Zombie_PiernaIzq.obj");
	ZombieBrazoD = Model();
	ZombieBrazoD.LoadModel("Models/Zombie_BrDer.obj");
	printf("Cargado\n");
	ZombieBrazoI = Model();
	ZombieBrazoI.LoadModel("Models/Zombie_BrIzq.obj");

	ArachCuerpo = Model();
	ArachCuerpo.LoadModel("Models/Arach_Cuerpo.obj");
	ArachCanon = Model();
	ArachCanon.LoadModel("Models/Arach_Canon.obj");

	Dopefish = Model();
	Dopefish.LoadModel("Models/Dopefish.obj");

	DoomE1M1Room1 = Model();
	DoomE1M1Room1.LoadModel("Models/DOOMM1_IR.obj");

	DoomE1M1Room2 = Model();
	DoomE1M1Room2.LoadModel("Models/DOOMM1_GR.obj");

	DoomIIMap1Room1 = Model();
	DoomIIMap1Room1.LoadModel("Models/DOOMIIM1_IR.obj");

	DoomIIMap1Room2= Model();
	DoomIIMap1Room2.LoadModel("Models/DOOMIIM1_LR.obj");

}

void renderizarModelosMinion(glm::mat4 model, GLuint uniformModel, glm::mat4 modelaux){
	//Instancia del minion hula
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(70.7f, 0.5f, -111.1f));
	model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MinionHula.RenderModel();

	//Instancia del minion normal
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 0.5f, -30.3f));
	model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MinionNormal.RenderModel();

	//Instancia de maceta minion 
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(90.9f, 0.5f, -60.6f));
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MacetaMinion.RenderModel();

	//Instancia del minion morado
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(50.5f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MinionMorado.RenderModel();

	//Instancia del Dany flow (trash)
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -60.6f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Vector.RenderModel();

	//Instancia de gru
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(90.9f, 0.0f, -90.9f));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Gru.RenderModel();

	//Instancia de carro de gru
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(20.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	CarroGru.RenderModel();

	//Instancia de maquina dulces minion
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(40.4f, 0.0f, -111.1f));
	model = glm::scale(model, glm::vec3(0.12f, 0.12f, 0.12f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MinionMaquinaDulces.RenderModel();

	//Instancia de Fortaleza de vector
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 0.5f, -111.1f));
	model = glm::scale(model, glm::vec3(0.07f, 0.07f, 0.07f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	FortalezaVector.RenderModel();

	//Instancia del minion bebe 
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(90.9f, 0.0f, -20.2f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MinionBebe.RenderModel();
}

void renderizarModelosDoom(glm::mat4 model, GLuint uniformModel, glm::mat4 modelaux) {
	//instancia de doom E1M1 cuarto inicial
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(10.1f, 0.5f, -10.0f));
	model = glm::scale(model, glm::vec3(0.17f, 0.17f, 0.17f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	DoomE1M1Room1.RenderModel();

	//instancia de doom E1M1 cuarto iconico
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(20.2f, 0.5f, -10.0f));
	model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	DoomE1M1Room2.RenderModel();

	//Instancia de doom II mapa 1 cuarto inicial
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-13.1f, 0.5f, -9.0f));
	model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	DoomIIMap1Room1.RenderModel();

	//Instancia de doom II mapa 1 cuarto final
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(40.4f, 0.5f, -10.0f));
	model = glm::scale(model, glm::vec3(0.28f, 0.28f, 0.28f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	DoomIIMap1Room2.RenderModel();

	rotaGarg += 0.5f;
	//Instancia de gargoyle
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 2.5f, 10.0f));
	//model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
	//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	GargCuerpo.RenderModel();
	model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
	modelaux = model;
	model = glm::rotate(model, rotaGarg * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	GargAlaI.RenderModel();
	model = modelaux;
	model = glm::rotate(model, -1 * rotaGarg * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	GargAlaD.RenderModel();

	//instancia de sentinel wolf
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 0.5f, 20.0f));
	//model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	WolfCuerpo.RenderModel();
	WolfCabeza.RenderModel();

	//instancia de zombie
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 2.5f, 30.0f));
	model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	ZombieCuerpo.RenderModel();
	ZombieBrazoD.RenderModel();
	ZombieBrazoI.RenderModel();
	ZombiePiernaD.RenderModel();
	ZombiePiernaI.RenderModel();

	//instancia de revenant
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(60.0f, 2.5f, 40.0f));
	model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	RevCuerpo.RenderModel();
	RevCanons.RenderModel();

	//instancia de arachnotron
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(60.0f, 2.5f, 55.0f));
	model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	ArachCuerpo.RenderModel();
	ArachCanon.RenderModel();

	//instancia de dopefish
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(70.0f, 2.5f, 55.0f));
	model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dopefish.RenderModel();

	//instancia de cacodemon
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 0.5f, 55.0f));
	model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//Cacodemon.RenderModel();
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
	skyboxFaces.push_back("Textures/Skybox/newsb1.tga");
	skyboxFaces.push_back("Textures/Skybox/newsb2.tga");
	skyboxFaces.push_back("Textures/Skybox/newsb5.tga");
	skyboxFaces.push_back("Textures/Skybox/newsb5.tga");
	skyboxFaces.push_back("Textures/Skybox/newsb3.tga");
	skyboxFaces.push_back("Textures/Skybox/newsb4.tga");

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

			funcion para animacion generica

			jerarquia de wolf
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

		//instancia de piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f, 1.0f, 15.0f));
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

		//instancia de doom E1M1 cuarto inicial crearCasilla(0.0f, -20.2f); //39
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.1f, 0.5f, -101.1f));
		model = glm::scale(model, glm::vec3(0.22f, 0.22f, 0.22f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DoomE1M1Room1.RenderModel();

		//instancia de doom E1M1 cuarto iconico crearCasilla(10.1f, -111.1f); //29
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(11.1f, 0.2f, -120.0f));
		model = glm::scale(model, glm::vec3(0.23f, 0.23f, 0.23f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DoomE1M1Room2.RenderModel();

		//Instancia de doom II mapa 1 cuarto inicial crearCasilla(0.0f, -101.1f); //31
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-12.5f, 0.5f, -18.9f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DoomIIMap1Room1.RenderModel();

		//Instancia de doom II mapa 1 cuarto iconico crearCasilla(90.9f, -40.4f); //14
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 0.2f, -40.4f));
		model = glm::scale(model, glm::vec3(0.28f, 0.28f, 0.28f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();

		//Instancia de gargoyle 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 3.5f, -30.3f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GargCuerpo.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 2.2f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GargAlaI.RenderModel();
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GargAlaD.RenderModel();
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GargBrazoD.RenderModel();
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GargBrazoI.RenderModel();

		//instancia de sentinel wolf crearCasilla(50.5f, -111.1f); //25
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.5f, 0.2f, -120.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		WolfCuerpo.RenderModel();
		WolfCabeza.RenderModel();

		//instancia de zombie crearCasilla(90.9f, -10.1f); //11
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 6.0f, -10.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ZombieCuerpo.RenderModel();
		ZombieBrazoD.RenderModel();
		ZombieBrazoI.RenderModel();
		ZombiePiernaD.RenderModel();
		ZombiePiernaI.RenderModel();

		//instancia de revenant crearCasilla(80.8f, 0.0f); //9
		//rotaCanon += 2.0f;
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.8f, 0.2f, 9.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RevCuerpo.RenderModel();
		RevCanons.RenderModel();

		//instancia de arachnotron crearCasilla(0.0f, -70.7f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.1f, 3.5f, -70.7f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ArachCuerpo.RenderModel();
		//model = glm::rotate(model, rotaCanon * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ArachCanon.RenderModel();

		//instancia de dopefish crearCasilla(90.9f, -111.1f); //21
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.9f, 1.0f, -120.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dopefish.RenderModel();

		//instancia de cacodemon
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 55.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Cacodemon.RenderModel();

		//Instancia del minion hula
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.7f, 0.5f, -121.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();

		//Instancia de lampara 4
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.5f, -120.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MinionNormal.RenderModel();

		//Instancia de maceta minion 
		model = glm::mat4(1.0); 
		model = glm::translate(model, glm::vec3(100.0f, 0.5f, -60.6f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MacetaMinion.RenderModel();

		//Instancia del minion morado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.5f, 0.0f, 10.0f));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MinionMorado.RenderModel();

		//Instancia del Dany flow (trash)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.1f, 0.0f, -60.6f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Vector.RenderModel();

		//Instancia de gru
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 0.0f, -90.9f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gru.RenderModel();

		//Instancia de carro de gru
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, 10.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CarroGru.RenderModel();

		//Instancia de maquina dulces minion
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.4f, 0.0f, -121.0f));
		model = glm::scale(model, glm::vec3(0.12f, 0.12f, 0.12f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MinionMaquinaDulces.RenderModel();

		//Instancia de Fortaleza de vector
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -121.0f));
		model = glm::scale(model, glm::vec3(0.07f, 0.07f, 0.07f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FortalezaVector.RenderModel();

		//Instancia del minion bebe 
		model = glm::mat4(1.0); 
		model = glm::translate(model, glm::vec3(100.0f, 0.0f, -20.2f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MinionBebe.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
