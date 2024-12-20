/*
	PROYECTO FINAL CGIHC
*/

/*
		Hacer:

			meter texturas al horno

			ir llenando arreglos de dir y pos para licuadora
*/

//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <random>
#include <string>

#include <glew.h>
#include <glfw3.h>
#include <map>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>
#include <irrKlang.h>
using namespace irrklang;
#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "StaticCamera.h"
#include "FollowCamera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h" 
#include "Skybox.h"
#include <thread>
#include <chrono>

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//valores especificos de posicion y rotacion para cada modelo (modificar con valores finales para cada modelo)
float direcciones[40] =
{
	360.0f, -90.0f, 90.0f, -180.0f, 360.0f, 90.0f, 180.0f, -180.0f, 90.0f, 180.0f,
	180.0f, 180.0f, 180.0f, 90.0f, -90.0f, -180.0f, 360.0f, -180.0f, -90.0f, -180.0f,
	360.0f, -90.0f, 360.0f, 90.0f, 360.0f, 360.0f, 360.0f, 360.0f, 360.0f, 360.0f,
	-180.0f, 360.0f, 2.0f, 360.0f, 90.0f, -90.0f, 360.0f, 90.0f, 0.0f, -90.0f
};
float posiciones[40] =
{
	0.2f, 2.0f, 0.0f,  0.2f,  0.0f,  0.0f,  0.5f,  0.2f,  0.2f,  0.5f,
	0.2f, 0.0f, 3.5f,  0.2f,  0.2f,  0.5f,  0.2f,  0.0f,  0.0f,  0.0f,
	1.0f, 0.0f, 0.5f,  0.2f,  0.2f,  0.0f,  0.2f,  0.0f,  0.2f,  0.5f,
	0.2f, 2.0f, 4.0f,  0.2f,  0.0f,  0.2f,  0.2f,  0.5f,  0.0f,  0.5f
};

float posLamparas[4][2] =
{
	{-10.0f, 10.0f}, //1
	{100.0f, 10.0f}, //2
	{100.0f, -120.0f}, //3	
	{-10.0f, -120.0f} //4
};

float pos[40][2] =
{
	{0.0f, 0.0f}, //1
	{10.1f, 0.0f}, //2
	{20.2f, 0.0f}, //3	
	{30.3f, 0.0f}, //4
	{40.4f, 0.0f}, //5
	{50.5f, 0.0f}, //6
	{60.6f, 0.0f}, //7
	{70.7f, 0.0f}, //8
	{80.8f, 0.0f}, //9
	{90.9f, 0.0f}, //10
	{90.9f, -10.1f},//11
	{90.9f, -20.2f}, //12
	{90.9f, -30.3f}, //13
	{90.9f, -40.4f}, //14
	{90.9f, -50.5f}, //15
	{90.9f, -60.6f}, //16
	{90.9f, -70.7f}, //17
	{90.9f, -80.8f}, //18
	{90.9, -90.9f}, //19
	{90.9f, -101.1f}, //20
	{90.9f, -111.1f}, //21
	{80.8f, -111.1f}, //22
	{70.7f, -111.1f}, //23
	{60.6f, -111.1f}, //24
	{50.5f, -111.1f}, //25
	{40.4f, -111.1f}, //26
	{30.3f, -111.1f}, //27
	{20.2f, -111.1f}, //28
	{10.1f, -111.1f}, //29
	{0.0f, -111.1f}, //30
	{0.0f, -101.1f}, //31
	{0.0f, -90.9f}, //32
	{0.0f, -80.8f}, //33
	{0.0f, -70.7f}, //34
	{0.0f, -60.6f}, //35
	{0.0f, -50.5f}, //36
	{0.0f, -40.4f}, //37
	{0.0f, -30.3f}, //38
	{0.0f, -20.2f}, //39
	{0.0f, -10.1f}, //40
};

//variables para animacion
int counter = 0;
int framesDados = 0;
int framesMov = 0;
int framesLicua = 0;
int framesCamin = 0; //propuesto xd
int maxFramesCamin = 30;
int numD4 = 0;
int numD8 = 0;
int numTotal = 0;
int casAct = 1; //casilla actual
int casDest = 0; //casilla destino
int casRest = 0; //casillas restantes por avanzar
float rotaDado4X, rotaDado4Y, rotaDado4Z;
float dirDado4X, dirDado4Y, dirDado4Z;
float rotaDado8X, rotaDado8Y, rotaDado8Z;
float dirDado8X, dirDado8Y, dirDado8Z;
float posDados;
float posInicMods = -10.0f;
float posAnimMods = -10.0f;
float cambioPosMods = 0.0f;
float dirAnimMods = 0.0f;
float posAvatarX = 0.0f;
float posAvatarZ = 0.0f;
float dirAvatar = 90.0f;
float cambioDirMods = 0.0f;
float rotationAngle = 0.0f;
float anguloLuzX = 0.0f;
float anguloLuzY = 0.0f;
float offsetCamaraX = -15.0f;
float offsetCamaraY = 5.0f;
float offsetCamaraZ = 0.0f;
float dirCamara = 0.0f;
bool animActiva = false;
bool esNoche = false;

//<<<<<<< HEAD
//variables animacion hold on bella
const GLfloat ciclo = 15.0f;  // Duración total del ciclo en segundos
const GLfloat totalEstados = 5;  // Número total de estados
const GLfloat duracion_estado = ciclo / totalEstados;  // Duración de cada estado
GLfloat rotacion_angulo = 0.0f; // Ángulo de rotación inferior


GLfloat Time = 0.0f;
GLfloat pasTime = 0.0f;
GLfloat lapso = 0.0f;


// Variables de movimiento y rotación
GLfloat movBellaX = 0.0f;  // Movimiento en el eje X
GLfloat movBellaY = 0.0f;  // Movimiento en el eje Y
GLfloat desplazamiento = 4.0f;  // Desplazamiento máximo en unidades



bool caminando = false;
int cameraMode = 0;


int textures;
GLfloat contadorDAYNIGHT = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera mainCamera(glm::vec3(0.0f, 40.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.5f, 0.1f);
FollowCamera followCamera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.1f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);
StaticCamera isoCamera(glm::vec3(50.0f, 50.0f, -140.0f), glm::vec3(0.0f, 0.0f, 1.0f), 5.0f, 0.1f);

//Texturas tablero
Texture AmTexture, AzTexture, RoTexture, VeTexture, pisoTexture;

//Texturas dados
Texture D4Texture, D8Texture;

//Texturas Doom 
Texture Doom1Tex, Doom2Tex, Doom3Tex, Doom4Tex, Doom5Tex, Doom6Tex, Doom7Tex, Doom8Tex, Doom9Tex, Doom10Tex;

//Texturas Minion
Texture MinionTexture, EvilMinionTexture, MaquinaDulcesMinionTexture, MacetMinionTexture, HulaMinionTexture, GruTexture, CarroGruTexture, BabyMinionTexture, VectorTexture, VectorFortressTexture;

//Texturas Minion Noche cocinadas
Texture MinionTextureNight, EvilMinionTextureNight, MaquinaDulcesMinionTextureNight, MacetMinionTextureNight, HulaMinionTextureNight, GruTextureNight, CarroGruTextureNight, BabyMinionTextureNight, VectorTextureNight, VectorFortressTextureNight;

//Texturas Minion Dia cocinadas
Texture MinionTextureDay, EvilMinionTextureDay, MaquinaDulcesMinionTextureDay, MacetMinionTextureDay, HulaMinionTextureDay, GruTextureDay, CarroGruTextureDay, BabyMinionTextureDay, VectorTextureDay, VectorFortressTextureDay;


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

//Textures Mario
Texture plantioFuego, mushroomKingdom, yoshiIsland, nubeReino, sorbeteTierra, sarasaLand, mundoRocoso, koppaReino, caminoEstrella, reinoLava;

//Modelos Minion
Model MinionHula, MinionNormal, MinionMorado, Vector, Gru, CarroGru, FortalezaVector, MinionBebe, MacetaMinion, MinionMaquinaDulces;
Model MinionAvatarCuerpo, MinionAvatarBrazoIzq, MinionAvatarBrazoDer, MinionAvatarPiernaIzq, MinionAvatarPiernaDer;

//Modelos Doom
Model DoomIIMap1Room1, DoomIIMap1Room2, DoomE1M1Room1, DoomE1M1Room2, RevCuerpo, ZombieCuerpo, ArachCuerpo, WolfCuerpo, GargCuerpo, Cacodemon, Dopefish;

//Modelos de Instancias de Mario Bros
Model entornoArena, entornoBowser, entornoEstrella, entornoFlores, entornoGomba, entornoLava, entornoNieve, entornoNubes, entornoRocoso, entornoYoshi;
//Modelos de Mario Bros
Model florFlama, goomba, castilloHielo, plantaPirana, whomp, luma, lakitu, sandman, yoshi, bowser;

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
Model madameT;

//BELLA AVATAR
Model bellatronco;
Model bellabrazo;
Model bellabrazotwo;
Model bellapierna1;
Model bellapiernatwo;

//Modelos entorno
Model Lampara, Sol;
Model rosaRoom;
Model Marte;
Model Volcan;
Model PuenteLondon;
Model EiffelTower;
Model Mountain;



Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;

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
	printf("Creando objetos...\n");
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
	printf("Creando shader...\n");
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void crearDados()
{
	printf("Creando Dados...\n");
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
}

void crearTablero(bool esNoche)
{
	if (esNoche) {
		//numeros de casillas considerando que se empieza en esquina
	//inferior izquierda y se recorre hacia la derecha, arriba, izquierda, abajo.
	//casillas rojas (seres vivos)
		RoTexture.UseTexture();
		rosaNC.UseTexture();
		crearCasilla(0.0f, 0.0f); //1
		crosworthNC.UseTexture();
		crearCasilla(30.3f, 0.0f); //4
		yoshiIsland.UseTexture();
		crearCasilla(60.6f, 0.0f); //7
		nubeReino.UseTexture();
		crearCasilla(90.9f, 0.0f); //10
		Doom6Tex.UseTexture();
		crearCasilla(90.9f, -30.3f); //13
		GruTextureNight.UseTexture();
		crearCasilla(90.9f, -90.9f); //19
		chipNC.UseTexture();
		crearCasilla(60.6f, -111.1f); //24
		koppaReino.UseTexture();
		crearCasilla(20.2f, -111.1f); //28
		Doom9Tex.UseTexture();
		crearCasilla(0.0f, -70.7f); //34
		MinionTextureNight.UseTexture();
		crearCasilla(0.0f, -30.3f); //38

		//casillas amarillas (fauna)
		plantioFuego.UseTexture();
		crearCasilla(10.1f, 0.0f); //2
		EvilMinionTextureNight.UseTexture();
		crearCasilla(50.5f, 0.0f); //6
		Doom8Tex.UseTexture();
		crearCasilla(80.8f, 0.0f); //9
		BabyMinionTextureNight.UseTexture();
		crearCasilla(90.9f, -20.2f); //12
		Doom4Tex.UseTexture();
		crearCasilla(90.9f, -40.4f); //14
		mrpottsNC.UseTexture();
		crearCasilla(90.9f, -70.7f); //17
		sarasaLand.UseTexture();
		crearCasilla(90.9f, -101.1f); //20
		Doom5Tex.UseTexture();
		crearCasilla(50.5f, -111.1f); //25
		VectorFortressTextureNight.UseTexture();
		crearCasilla(0.0f, -111.1f); //30
		caminoEstrella.UseTexture();
		crearCasilla(0.0f, -90.9f); //32
		lumiereNC.UseTexture();
		crearCasilla(0.0f, -40.4f); //37

		//casillas azules (edificios)
		CarroGruTextureNight.UseTexture();
		crearCasilla(20.2f, 0.0f); //3
		castilloNC.UseTexture();
		crearCasilla(70.7f, 0.0f); //8
		Doom7Tex.UseTexture();
		crearCasilla(90.9f, -10.1f); //11
		gastonNC.UseTexture();
		crearCasilla(90.9f, -50.5f); //15
		sorbeteTierra.UseTexture();
		crearCasilla(90.9f, -80.8f); //18
		mundoRocoso.UseTexture();
		crearCasilla(80.8f, -111.1f); //22
		MaquinaDulcesMinionTextureNight.UseTexture();
		crearCasilla(40.4f, -111.1f); //26
		Doom2Tex.UseTexture();
		crearCasilla(0.0f, -101.1f); //31
		plumetteNC.UseTexture();
		crearCasilla(0.0f, -50.5f); //36
		Doom1Tex.UseTexture();
		crearCasilla(0.0f, -10.1f); //39

		RoTexture.UseTexture();
		//casillas verdes (flora)
		mushroomKingdom.UseTexture();
		crearCasilla(40.4f, 0.0f); //5
		MacetMinionTextureNight.UseTexture();
		crearCasilla(90.9f, -60.6f); //16
		Doom10Tex.UseTexture();
		crearCasilla(90.9f, -111.1f); //21
		HulaMinionTextureNight.UseTexture();
		crearCasilla(70.7f, -111.1f); //23
		bestiaNC.UseTexture();
		crearCasilla(30.3f, -111.1f); //27
		Doom3Tex.UseTexture();
		crearCasilla(10.1f, -111.1f); //29
		madameNC.UseTexture();
		crearCasilla(0.0f, -80.8f); //33
		VectorTextureNight.UseTexture();
		crearCasilla(0.0f, -60.6f); //35
		reinoLava.UseTexture();
		crearCasilla(0.0f, -20.2f); //39
	}
	else {
		//numeros de casillas considerando que se empieza en esquina
	//inferior izquierda y se recorre hacia la derecha, arriba, izquierda, abajo.
	//casillas rojas (seres vivos)
		RoTexture.UseTexture();
		rosaC.UseTexture();
		crearCasilla(0.0f, 0.0f); //1
		crosworthC.UseTexture();
		crearCasilla(30.3f, 0.0f); //4
		yoshiIsland.UseTexture();
		crearCasilla(60.6f, 0.0f); //7
		nubeReino.UseTexture();
		crearCasilla(90.9f, 0.0f); //10
		Doom6Tex.UseTexture();
		crearCasilla(90.9f, -30.3f); //13
		GruTextureDay.UseTexture();
		crearCasilla(90.9f, -90.9f); //19
		chipC.UseTexture();
		crearCasilla(60.6f, -111.1f); //24
		koppaReino.UseTexture();
		crearCasilla(20.2f, -111.1f); //28
		Doom9Tex.UseTexture();
		crearCasilla(0.0f, -70.7f); //34
		MinionTextureDay.UseTexture();
		crearCasilla(0.0f, -30.3f); //38

		//casillas amarillas (fauna)
		plantioFuego.UseTexture();
		crearCasilla(10.1f, 0.0f); //2
		EvilMinionTextureDay.UseTexture();
		crearCasilla(50.5f, 0.0f); //6
		Doom8Tex.UseTexture();
		crearCasilla(80.8f, 0.0f); //9
		BabyMinionTextureDay.UseTexture();
		crearCasilla(90.9f, -20.2f); //12
		Doom4Tex.UseTexture();
		crearCasilla(90.9f, -40.4f); //14
		mrpottsC.UseTexture();
		crearCasilla(90.9f, -70.7f); //17
		sarasaLand.UseTexture();
		crearCasilla(90.9f, -101.1f); //20
		Doom5Tex.UseTexture();
		crearCasilla(50.5f, -111.1f); //25
		VectorFortressTextureDay.UseTexture();
		crearCasilla(0.0f, -111.1f); //30
		caminoEstrella.UseTexture();
		crearCasilla(0.0f, -90.9f); //32
		lumiereC.UseTexture();
		crearCasilla(0.0f, -40.4f); //37

		//casillas azules (edificios)
		CarroGruTextureDay.UseTexture();
		crearCasilla(20.2f, 0.0f); //3
		castilloC.UseTexture();
		crearCasilla(70.7f, 0.0f); //8
		Doom7Tex.UseTexture();
		crearCasilla(90.9f, -10.1f); //11
		gastonC.UseTexture();
		crearCasilla(90.9f, -50.5f); //15
		sorbeteTierra.UseTexture();
		crearCasilla(90.9f, -80.8f); //18
		mundoRocoso.UseTexture();
		crearCasilla(80.8f, -111.1f); //22
		MaquinaDulcesMinionTextureDay.UseTexture();
		crearCasilla(40.4f, -111.1f); //26
		Doom2Tex.UseTexture();
		crearCasilla(0.0f, -101.1f); //31
		plumetteC.UseTexture();
		crearCasilla(0.0f, -50.5f); //36
		Doom1Tex.UseTexture();
		crearCasilla(0.0f, -10.1f); //39

		RoTexture.UseTexture();
		//casillas verdes (flora)
		mushroomKingdom.UseTexture();
		crearCasilla(40.4f, 0.0f); //5
		MacetMinionTextureDay.UseTexture();
		crearCasilla(90.9f, -60.6f); //16
		Doom10Tex.UseTexture();
		crearCasilla(90.9f, -111.1f); //21
		HulaMinionTextureDay.UseTexture();
		crearCasilla(70.7f, -111.1f); //23
		bestiaC.UseTexture();
		crearCasilla(30.3f, -111.1f); //27
		Doom3Tex.UseTexture();
		crearCasilla(10.1f, -111.1f); //29
		madameC.UseTexture();
		crearCasilla(0.0f, -80.8f); //33
		VectorTextureDay.UseTexture();
		crearCasilla(0.0f, -60.6f); //35
		reinoLava.UseTexture();
		crearCasilla(0.0f, -20.2f); //39
	}
	
}

void cargarTexturas()
{
	printf("Cargando texturas de entorno...\n");
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
	
	//TEXTURAS DE DOOM
	printf("Cargando texturas de Doom...\n");
	Doom1Tex = Texture("Textures/1_M1CIN_D.png");
	Doom1Tex.LoadTextureA();
	Doom2Tex = Texture("Textures/2_E1M1CIN_D.png");
	Doom2Tex.LoadTextureA();
	Doom3Tex = Texture("Textures/3_E1M1CIC_D.png");
	Doom3Tex.LoadTextureA();
	Doom4Tex = Texture("Textures/4_M1CIC_D.png");
	Doom4Tex.LoadTextureA();
	Doom5Tex = Texture("Textures/5_WOLF_D.png");
	Doom5Tex.LoadTextureA();
	Doom6Tex = Texture("Textures/6_GARG_D.png");
	Doom6Tex.LoadTextureA();
	Doom7Tex = Texture("Textures/7_ZOM_D.png");
	Doom7Tex.LoadTextureA();
	Doom8Tex = Texture("Textures/8_REV_D.png");
	Doom8Tex.LoadTextureA();
	Doom9Tex = Texture("Textures/9_ARACH_D.png");
	Doom9Tex.LoadTextureA();
	Doom10Tex = Texture("Textures/10_FISH_D.png");
	Doom10Tex.LoadTextureA();

	printf("Cargando texturas de Minions Dia...\n");
	MinionTextureDay = Texture("Textures/MinionNormal.png");
	MinionTextureDay.LoadTextureA();
	EvilMinionTextureDay = Texture("Textures/MinionMoradoC.png");
	EvilMinionTextureDay.LoadTextureA();
	MaquinaDulcesMinionTextureDay = Texture("Textures/MaquinaDulcesMinionC.png");
	MaquinaDulcesMinionTextureDay.LoadTextureA();
	MacetMinionTextureDay = Texture("Textures/MacetaMinionC.png");
	MacetMinionTextureDay.LoadTextureA();
	HulaMinionTextureDay = Texture("Textures/HulaMinionC.png");
	HulaMinionTextureDay.LoadTextureA();
	GruTextureDay = Texture("Textures/GruC.png");
	GruTextureDay.LoadTextureA();
	CarroGruTextureDay = Texture("Textures/CarroGruC.png");
	CarroGruTextureDay.LoadTextureA();
	VectorTextureDay = Texture("Textures/VectorC.png");
	VectorTextureDay.LoadTextureA();
	BabyMinionTextureDay = Texture("Textures/BabyMinionC.png");
	BabyMinionTextureDay.LoadTextureA();
	VectorFortressTextureDay = Texture("Textures/VectorFortressC.png");
	VectorFortressTextureDay.LoadTextureA();

	printf("Cargando texturas de Minions noche...\n");
	MinionTextureNight = Texture("Textures/MinionNormalNoche.png");
	MinionTextureNight.LoadTextureA();
	EvilMinionTextureNight = Texture("Textures/MinionMoradoNoche.png");
	EvilMinionTextureNight.LoadTextureA();
	MaquinaDulcesMinionTextureNight = Texture("Textures/MaquinaDulcesMinionNoche.png");
	MaquinaDulcesMinionTextureNight.LoadTextureA();
	MacetMinionTextureNight = Texture("Textures/MacetaMinionNoche.png");
	MacetMinionTextureNight.LoadTextureA();
	HulaMinionTextureNight = Texture("Textures/HulaMinionNoche.png");
	HulaMinionTextureNight.LoadTextureA();
	GruTextureNight = Texture("Textures/GruNoche.png");
	GruTextureNight.LoadTextureA();
	CarroGruTextureNight = Texture("Textures/CarroGruNoche.png");
	CarroGruTextureNight.LoadTextureA();
	VectorTextureNight = Texture("Textures/VectorNoche.png");
	VectorTextureNight.LoadTextureA();
	BabyMinionTextureNight = Texture("Textures/BabyMinionNoche.png");
	BabyMinionTextureNight.LoadTextureA();
	VectorFortressTextureNight = Texture("Textures/VectorFortressNoche.png");
	VectorFortressTextureNight.LoadTextureA();

	//TEXTURAS MUNDO BELLA
	printf("Cargando texturas de Bella...\n");
	lumiereN = Texture("Textures/lumierenight.png");
	lumiereN.LoadTextureA();
	rosaN = Texture("Textures/inicionight.png");
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
	rosa = Texture("Textures/inicio.png");
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
	madame = Texture("Textures/madamecook.png");
	madame.LoadTextureA();
	mrpotts = Texture("Textures/mrpotts.png");
	mrpotts.LoadTextureA();
	chip = Texture("Textures/chip.png");
	chip.LoadTextureA();

	lumiereC = Texture("Textures/lumieredaycook.png");
	lumiereC.LoadTextureA();
	rosaC = Texture("Textures/rosadaycook.png");
	rosaC.LoadTextureA();
	gastonC = Texture("Textures/gastondaycook.png");
	gastonC.LoadTextureA();
	crosworthC = Texture("Textures/cogsworthdaycook.png");
	crosworthC.LoadTextureA();
	bestiaC = Texture("Textures/bestiadaycook.png");
	bestiaC.LoadTextureA();
	plumetteC = Texture("Textures/plumettedaycook.png");
	plumetteC.LoadTextureA();
	castilloC = Texture("Textures/castillodaycook.png");
	castilloC.LoadTextureA();
	madameC = Texture("Textures/madamecook.png");
	madameC.LoadTextureA();
	mrpottsC = Texture("Textures/mrpottsdaycook.png");
	mrpottsC.LoadTextureA();
	chipC = Texture("Textures/chipdaycook.png");
	chipC.LoadTextureA();

	//TEXTURAS MARIO BROS
	printf("Cargando texturas de Mario Bros...\n");

	plantioFuego = Texture("Textures/florFlama.png");
	plantioFuego.LoadTextureA();
	mushroomKingdom = Texture("Textures/goomba.png");
	mushroomKingdom.LoadTextureA();
	yoshiIsland = Texture("Textures/Yoshi.png");
	yoshiIsland.LoadTextureA();
	nubeReino = Texture("Textures/lakitu.png");
	nubeReino.LoadTextureA();
	sorbeteTierra = Texture("Textures/castilloSorbete.png");
	sorbeteTierra.LoadTextureA();
	sarasaLand = Texture("Textures/sandMan.png");
	sarasaLand.LoadTextureA();
	mundoRocoso = Texture("Textures/whompT.png");
	mundoRocoso.LoadTextureA();
	koppaReino = Texture("Textures/bowser.png");
	koppaReino.LoadTextureA();
	caminoEstrella = Texture("Textures/luma.png");
	caminoEstrella.LoadTextureA();
	reinoLava = Texture("Textures/plantaP.png");
	reinoLava.LoadTextureA();

}

void animacionCaida()
{
	posDados = posDados - 0.1f;
	//printf("[Frame %d] Pos dado: %f\n", framesDados, posDadoY);
}

void animacionGiroD4(float rotaX, float rotaY, float rotaZ)
{
	dirDado4X = dirDado4X + (rotaX * 4.0f);
	dirDado4Y = dirDado4Y + (rotaY * 4.0f);
	dirDado4Z = dirDado4Z + (rotaZ * 4.0f);
	//printf("[Frame %d] Dir dado: %f, %f, %f\n", framesDados, dirDadoX, dirDadoY, dirDadoZ);
}

void animacionGiroD8(float rotaX, float rotaY, float rotaZ)
{
	dirDado8X = dirDado8X + (rotaX * 4.0f);
	dirDado8Y = dirDado8Y + (rotaY * 4.0f);
	dirDado8Z = dirDado8Z + (rotaZ * 4.0f);
	//printf("[Frame %d] Dir dado: %f, %f, %f\n", framesDados, dirDadoX, dirDadoY, dirDadoZ);
}

void cargarModelos()
{
	//Iluminacion
	printf("Cargando modelos de entorno...\n");
	Lampara = Model();
	Lampara.LoadModel("Models/Entorno/lampara.obj");

	Sol = Model();
	Sol.LoadModel("Models/Entorno/SolTexturizado.obj");

	//Minions
	printf("Cargando modelos de Minions...\n");
	MinionNormal = Model();
	MinionNormal.LoadModel("Models/Minions/MinionNormalTexturizado.obj");

	MinionHula = Model();
	MinionHula.LoadModel("Models/Minions/MinionHulalTexturizado.obj");

	MacetaMinion = Model();
	MacetaMinion.LoadModel("Models/Minions/MacetaMinionTexturizada.obj");

	MinionBebe = Model();
	MinionBebe.LoadModel("Models/Minions/MinionBebeTexturizado2.obj");

	MinionMorado = Model();
	MinionMorado.LoadModel("Models/Minions/MinionMoradoTexturizado.obj");

	Vector = Model();
	Vector.LoadModel("Models/Minions/VectorTexturizado.obj");

	Gru = Model();
	Gru.LoadModel("Models/Minions/GruTexturizado.obj");

	CarroGru = Model();
	CarroGru.LoadModel("Models/Minions/CarroGruTexturizado3.obj");

	FortalezaVector = Model();
	FortalezaVector.LoadModel("Models/Minions/FortalezaVectorTexturizada4.obj");

	MinionMaquinaDulces = Model();
	MinionMaquinaDulces.LoadModel("Models/Minions/MinionMaquinaDulcesTexturizado.obj");

	MinionAvatarCuerpo = Model();
	MinionAvatarCuerpo.LoadModel("Models/Minions/MinionAvatarCuerpo.obj");

	MinionAvatarBrazoIzq = Model();
	MinionAvatarBrazoIzq.LoadModel("Models/Minions/MinionAvatarBrazoIzq.obj");

	MinionAvatarBrazoDer = Model();
	MinionAvatarBrazoDer.LoadModel("Models/Minions/MinionAvatarBrazoIzqDer.obj");

	MinionAvatarPiernaDer = Model();
	MinionAvatarPiernaDer.LoadModel("Models/Minions/MinionAvatarPiernaDer.obj");

	MinionAvatarPiernaIzq = Model();
	MinionAvatarPiernaIzq.LoadModel("Models/Minions/MinionAvatarPiernaIzq.obj");

	//Doom
	printf("Cargando modelos de Doom...\n");
	GargCuerpo = Model();
	GargCuerpo.LoadModel("Models/Doom/Gargoyle.obj");

	Cacodemon = Model();
	Cacodemon.LoadModel("Models/Doom/Cacodemon.obj");

	WolfCuerpo = Model();
	WolfCuerpo.LoadModel("Models/Doom/SentinelWolf.obj");

	RevCuerpo = Model();
	RevCuerpo.LoadModel("Models/Doom/Revenant.obj");

	ZombieCuerpo = Model();
	ZombieCuerpo.LoadModel("Models/Doom/Zombie.obj");

	ArachCuerpo = Model();
	ArachCuerpo.LoadModel("Models/Doom/Arachnotron.obj");

	Dopefish = Model();
	Dopefish.LoadModel("Models/Doom/Dopefish.obj");

	DoomE1M1Room1 = Model();
	DoomE1M1Room1.LoadModel("Models/Doom/DOOMM1_IR.obj");

	DoomE1M1Room2 = Model();
	DoomE1M1Room2.LoadModel("Models/Doom/DOOMM1_GR.obj");

	DoomIIMap1Room1 = Model();
	DoomIIMap1Room1.LoadModel("Models/Doom/DOOMIIM1_IR.obj");

	DoomIIMap1Room2 = Model();
	DoomIIMap1Room2.LoadModel("Models/Doom/DOOMIIM1_LR.obj");

	//Bella

	printf("Cargando modelos de Bella...\n");
	mrpottsT = Model();
	mrpottsT.LoadModel("Models/mrpotts.obj");

	madameT = Model();
	madameT.LoadModel("Models/ropero.obj");

	plumetteT = Model();
	plumetteT.LoadModel("Models/plum.obj");

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


	lumiereT = Model();
	lumiereT.LoadModel("Models/lumieretexturizado.obj");

	cogsworthT = Model();
	cogsworthT.LoadModel("Models/cogsworthtextre.obj");


	bellaT = Model();
	bellaT.LoadModel("Models/bella.obj");

	//Models avatar bella
	bellatronco = Model();
	bellatronco.LoadModel("Models/bellatronco.obj");
	bellabrazo = Model();
	bellabrazo.LoadModel("Models/bellabrazo.obj");
	bellabrazotwo = Model();
	bellabrazotwo.LoadModel("Models/bellabrazotwo.obj");
	bellapierna1 = Model();
	bellapierna1.LoadModel("Models/bellapierna1.obj");
	bellapiernatwo = Model();
	bellapiernatwo.LoadModel("Models/bellapiernatwo.obj");

	//MODELOS CIUDAD
	rosaRoom = Model();
	rosaRoom.LoadModel("Models/Entorno/rosaroom.obj");
	Marte = Model();
	Marte.LoadModel("Models/Entorno/Marte.obj");
	Volcan = Model();
	Volcan.LoadModel("Models/Entorno/Volcan.obj");
	Mountain = Model();
	Mountain.LoadModel("Models/Entorno/ElementoAgua.obj");
	EiffelTower = Model();
	EiffelTower.LoadModel("Models/Entorno/Eiffel.obj");
	PuenteLondon = Model();
	PuenteLondon.LoadModel("Models/Entorno/puente.obj");



	//Cargando Modelos de entornos de Mario Bros
	printf("Cargando modelos de Mario Bros...\n");
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

	//Cargando Modelos de elementos de casilla MarioBros
	florFlama = Model();
	florFlama.LoadModel("Models/MarioBros/florFlama.obj");

	goomba = Model();
	goomba.LoadModel("Models/MarioBros/goomba.obj");

	yoshi = Model();
	yoshi.LoadModel("Models/MarioBros/yoshi.obj");

	lakitu = Model();
	lakitu.LoadModel("Models/MarioBros/lakitu.obj");

	castilloHielo = Model();
	castilloHielo.LoadModel("Models/MarioBros/castilloSorbete.obj");

	sandman = Model();
	sandman.LoadModel("Models/MarioBros/sandMan.obj");

	whomp = Model();
	whomp.LoadModel("Models/MarioBros/whomp.obj");

	bowser = Model();
	bowser.LoadModel("Models/MarioBros/bowserhigh.obj");

	luma = Model();
	luma.LoadModel("Models/MarioBros/luma.obj");

	plantaPirana = Model();
	plantaPirana.LoadModel("Models/MarioBros/plantaPirana.obj");

}

void renderizarModelosMinion(glm::mat4 model, GLuint uniformModel, glm::mat4 modelaux) 
{
	//Instancia del minion hula
	model = glm::mat4(1.0);
	if (animActiva && casAct == 23) 
	{
		model = glm::translate(model, glm::vec3(70.7f, posAnimMods, -121.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(70.7f, posInicMods, -121.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MinionHula.RenderModel(); // casilla23

	//Instancia de minion normal
	model = glm::mat4(1.0);
	if (animActiva && casAct == 38) 
	{
		model = glm::translate(model, glm::vec3(-10.1f, posAnimMods, -30.3f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(-10.1f, posInicMods, -30.3f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MinionNormal.RenderModel(); // casilla38

	//Instancia de maceta minion
	model = glm::mat4(1.0);
	if (animActiva && casAct == 16) 
	{
		model = glm::translate(model, glm::vec3(100.0f, posAnimMods, -60.6f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(100.0f, posInicMods, -60.6f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MacetaMinion.RenderModel(); // casilla16

	//Instancia del minion morado
	model = glm::mat4(1.0);
	if (animActiva && casAct == 6) 
	{
		model = glm::translate(model, glm::vec3(50.5f, posAnimMods, 10.0f));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(50.5f, posInicMods, 10.0f));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MinionMorado.RenderModel(); //casilla6

	//Instancia del Dany flow (trash)
	model = glm::mat4(1.0);
	if (animActiva && casAct == 35) 
	{
		model = glm::translate(model, glm::vec3(-10.1f, posAnimMods, -60.6f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(-10.1f, posInicMods, -60.6f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Vector.RenderModel(); // casilla35

	//Instancia de gru
	model = glm::mat4(1.0);
	if (animActiva && casAct == 19) 
	{
		model = glm::translate(model, glm::vec3(100.0f, posAnimMods, -90.9f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(100.0f, posInicMods, -90.9f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Gru.RenderModel(); // casilla19

	//Instancia de carro de gru
	model = glm::mat4(1.0);
	if (animActiva && casAct == 3) 
	{
		model = glm::translate(model, glm::vec3(20.0f, posAnimMods, 10.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(20.0f, posInicMods, 10.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	CarroGru.RenderModel(); //casilla3

	//Instancia de maquina dulces minion
	model = glm::mat4(1.0);
	if (animActiva && casAct == 26) 
	{
		model = glm::translate(model, glm::vec3(40.4f, posAnimMods, -121.0f));
		model = glm::scale(model, glm::vec3(0.12f, 0.12f, 0.12f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(40.4f, posInicMods, -121.0f));
		model = glm::scale(model, glm::vec3(0.12f, 0.12f, 0.12f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MinionMaquinaDulces.RenderModel(); // casilla26

	//Instancia de Fortaleza de vector
	model = glm::mat4(1.0);
	if (animActiva && casAct == 30) 
	{
		model = glm::translate(model, glm::vec3(0.0f, posAnimMods, -121.0f));
		model = glm::scale(model, glm::vec3(0.07f, 0.07f, 0.07f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(0.0f, posInicMods, -121.0f));
		model = glm::scale(model, glm::vec3(0.07f, 0.07f, 0.07f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	FortalezaVector.RenderModel(); // casilla30

	//Instancia del minion bebe
	model = glm::mat4(1.0);
	if (animActiva && casAct == 12) 
	{
		model = glm::translate(model, glm::vec3(100.0f, posAnimMods, -20.2f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(100.0f, posInicMods, -20.2f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MinionBebe.RenderModel(); // casilla12
}

void renderizarModelosBella(glm::mat4 model, GLuint uniformModel, glm::mat4 modelaux) 
{
	//INSTANCIAS MUNDO BELLA
		//Instancia mrpotts

	model = glm::mat4(1.0);
	if (animActiva && casAct == 17) 
	{
		model = glm::translate(model, glm::vec3(98.9f, posAnimMods, -70.70f));
		model = glm::scale(model, glm::vec3(0.80f, 0.80f, 0.80f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(98.9f, posInicMods, -70.70f));
		model = glm::scale(model, glm::vec3(0.80f, 0.80f, 0.80f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	mrpottsT.RenderModel(); // casilla17

	model = glm::mat4(1.0);
	if (animActiva && casAct == 4) 
	{
		model = glm::translate(model, glm::vec3(30.3f, posAnimMods, 10.0f));
		model = glm::scale(model, glm::vec3(.08f, .08f, .08f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(30.3f, posInicMods, 10.0f));
		model = glm::scale(model, glm::vec3(.08f, .08f, .08f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	cogsworthT.RenderModel(); // casilla4

	model = glm::mat4(1.0);
	if (animActiva && casAct == 15) 
	{
		model = glm::translate(model, glm::vec3(100.9f, posAnimMods, -50.50f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(100.9f, posInicMods, -50.50f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	gastonT.RenderModel(); // casilla15

	model = glm::mat4(1.0);
	if (animActiva && casAct == 27) 
	{
		model = glm::translate(model, glm::vec3(30.3f, posAnimMods, -120.1f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(30.3f, posInicMods, -120.1f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	bestiaT.RenderModel(); // casilla27

	model = glm::mat4(1.0);
	if (animActiva && casAct == 1)
	{
		model = glm::translate(model, glm::vec3(0.0f, posAnimMods, 10.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(0.0f, posInicMods, 10.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	rosaT.RenderModel(); //casilla1

	model = glm::mat4(1.0);
	if (animActiva && casAct == 8) 
	{
		model = glm::translate(model, glm::vec3(70.7f, posAnimMods, 10.0f));
		model = glm::scale(model, glm::vec3(.05f, .05f, .05f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(70.7f, posInicMods, 10.0f));
		model = glm::scale(model, glm::vec3(.05f, .05f, .05f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	castilloT.RenderModel(); //casilla8

	model = glm::mat4(1.0);
	if (animActiva && casAct == 24) 
	{
		model = glm::translate(model, glm::vec3(60.6f, posAnimMods, -120.1f));
		model = glm::scale(model, glm::vec3(.8f, .8f, .8f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(60.6f, posInicMods, -120.1f));
		model = glm::scale(model, glm::vec3(.8f, .8f, .8f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	chipT.RenderModel(); // casilla24

	model = glm::mat4(1.0);
	if (animActiva && casAct == 36) 
	{
		model = glm::translate(model, glm::vec3(-10.0f, posAnimMods, -40.4f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(-10.0f, posInicMods, -40.4f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	plumetteT.RenderModel(); // casilla36

	model = glm::mat4(1.0);
	if (animActiva && casAct == 33) 
	{
		model = glm::translate(model, glm::vec3(-10.0f, posAnimMods, -80.4f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(-10.0f, posInicMods, -80.4f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, 2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	madameT.RenderModel(); // casilla33

	model = glm::mat4(1.0);
	if (animActiva && casAct == 37) 
	{
		model = glm::translate(model, glm::vec3(-10.0f, posAnimMods, -40.4f));
		model = glm::scale(model, glm::vec3(.20f, .20f, .20f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(-10.0f, posInicMods, -40.4f));
		model = glm::scale(model, glm::vec3(.20f, .20f, .20f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	lumiereT.RenderModel(); // casilla37
}

void renderizarModelosDoom(glm::mat4 model, GLuint uniformModel, glm::mat4 modelaux) {
	//instancia de doom E1M1 cuarto inicial
	model = glm::mat4(1.0);
	if (animActiva && casAct == 31) 
	{
		model = glm::translate(model, glm::vec3(-10.1f, posAnimMods, -101.1f));
		model = glm::scale(model, glm::vec3(0.22f, 0.22f, 0.22f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(-10.1f, posInicMods, -101.1f));
		model = glm::scale(model, glm::vec3(0.22f, 0.22f, 0.22f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	DoomE1M1Room1.RenderModel(); //casilla31

	//instancia de doom E1M1 cuarto iconico
	model = glm::mat4(1.0);
	if (animActiva && casAct == 29) 
	{
		model = glm::translate(model, glm::vec3(11.1f, posAnimMods, -120.0f));
		model = glm::scale(model, glm::vec3(0.23f, 0.23f, 0.23f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(11.1f, posInicMods, -120.0f));
		model = glm::scale(model, glm::vec3(0.23f, 0.23f, 0.23f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	DoomE1M1Room2.RenderModel(); // casilla29

	//Instancia de doom II mapa 1 cuarto inicial
	model = glm::mat4(1.0);
	if (animActiva && casAct == 40) 
	{
		model = glm::translate(model, glm::vec3(-12.5f, posAnimMods, -8.8f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(-12.5f, posInicMods, -8.8f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	DoomIIMap1Room1.RenderModel(); // casilla40

	//Instancia de doom II mapa 1 cuarto iconico
	model = glm::mat4(1.0);
	if (animActiva && casAct == 14) 
	{
		model = glm::translate(model, glm::vec3(100.0f, posAnimMods, -40.4f));
		model = glm::scale(model, glm::vec3(0.28f, 0.28f, 0.28f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(100.0f, posInicMods, -40.4f));
		model = glm::scale(model, glm::vec3(0.28f, 0.28f, 0.28f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	DoomIIMap1Room2.RenderModel(); // casilla14

	//Instancia de gargoyle
	model = glm::mat4(1.0);
	if (animActiva && casAct == 13) 
	{
		model = glm::translate(model, glm::vec3(100.0f, posAnimMods, -30.3f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -40 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(100.0f, posInicMods, -30.3f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -40 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	GargCuerpo.RenderModel(); // casilla13

	//instancia de sentinel wolf
	model = glm::mat4(1.0);
	if (animActiva && casAct == 25) 
	{
		model = glm::translate(model, glm::vec3(50.5f, posAnimMods, -120.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(50.5f, posInicMods, -120.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	WolfCuerpo.RenderModel(); // casilla25

	//instancia de zombie
	model = glm::mat4(1.0);
	if (animActiva && casAct == 11) 
	{
		model = glm::translate(model, glm::vec3(100.0f, posAnimMods, -10.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(100.0f, posInicMods, -10.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	ZombieCuerpo.RenderModel(); //casilla11

	//instancia de revenant
	model = glm::mat4(1.0);
	if (animActiva && casAct == 9) 
	{
		model = glm::translate(model, glm::vec3(80.8f, posAnimMods, 9.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(80.8f, posInicMods, 9.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	RevCuerpo.RenderModel(); //casilla9

	//instancia de arachnotron
	model = glm::mat4(1.0);
	if (animActiva && casAct == 34) 
	{
		model = glm::translate(model, glm::vec3(-10.1f, posAnimMods, -70.7f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(-10.1f, posInicMods, -70.7f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	ArachCuerpo.RenderModel(); // casilla34

	//instancia de dopefish
	model = glm::mat4(1.0);
	if (animActiva && casAct == 21) 
	{
		model = glm::translate(model, glm::vec3(90.9f, posAnimMods, -120.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(90.9f, posInicMods, -120.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dopefish.RenderModel(); // casilla21
}

void renderizarModelosMario(glm::mat4 model, GLuint uniformModel, glm::mat4 modelaux) {

	
			//INSTANCIAS DE MARIO BROS

	//INSTANCIAS DE MARIO BROS

	


	//Flor flama
	model = glm::mat4(1.0);
	if (animActiva && casAct == 2)
	{
		model = glm::translate(model, glm::vec3(10.0f, posAnimMods, 10.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(10.0f, posInicMods, 10.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	florFlama.RenderModel(); // casilla2

	

	//Gomba
	model = glm::mat4(1.0);
	if (animActiva && casAct == 5)
	{
		model = glm::translate(model, glm::vec3(40.0f, posAnimMods, 10.5f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(40.0f, posInicMods, 10.5f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	goomba.RenderModel(); // casilla5





	//Yoshi
	model = glm::mat4(1.0);
	if (animActiva && casAct == 7)
	{
		model = glm::translate(model, glm::vec3(60.0f, posAnimMods, 10.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(60.0f, posInicMods, 10.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	yoshi.RenderModel(); //casilla7





	//Lakitu
	model = glm::mat4(1.0);
	if (animActiva && casAct == 10)
	{
		model = glm::translate(model, glm::vec3(90.9f, posAnimMods, 10.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(90.9f, posInicMods, 10.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	lakitu.RenderModel(); // casilla10


	

	//Castillo

	


	//Sandman
	model = glm::mat4(1.0);
	if (animActiva && casAct == 20)
	{
		model = glm::translate(model, glm::vec3(100.0f, posAnimMods, -100.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(100.0f, posInicMods, -100.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	sandman.RenderModel(); // casilla20


	


	//Whomp
	model = glm::mat4(1.0);
	if (animActiva && casAct == 22)
	{
		model = glm::translate(model, glm::vec3(80.5f, posAnimMods, -120.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.50f, 1.5f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(80.5f, posInicMods, -120.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.50f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	whomp.RenderModel(); // casilla22


	//Reino Koopa
	model = glm::mat4(1.0);
	if (animActiva && casAct == 18)
	{
		model = glm::translate(model, glm::vec3(101.0f, posAnimMods, -80.8f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(101.0f, posInicMods, -80.8f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	entornoBowser.RenderModel(); // casilla18


	//Bowser
	model = glm::mat4(1.0);
	if (animActiva && casAct == 28)
	{
		model = glm::translate(model, glm::vec3(20.5f, posAnimMods, -120.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(20.5f, posInicMods, -120.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));


	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	bowser.RenderModel(); // casilla28

	


	//Luma
	model = glm::mat4(1.0);
	if (animActiva && casAct == 32)
	{
		model = glm::translate(model, glm::vec3(-10.0f, posAnimMods, -90.5f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, dirAnimMods * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(-10.0f, posInicMods, -90.5f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));

	}
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	luma.RenderModel(); // casilla32




	//Planta Piraña
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -20.5f));
	model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//plantaPirana.RenderModel();
}

void renderizarModelosCiudad(glm::mat4 model, GLuint uniformModel, glm::mat4 modelaux) {
	//AGUA
	//Reino de nube
	
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(140.9f, 3.0f, 5.0f));
	model = glm::scale(model, glm::vec3(5.0f, 7.0f,5.0f));
	model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	entornoNubes.RenderModel();

	//Tierra sorbete
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-0.9f, 0.5f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	entornoNieve.RenderModel();

	//Camino estrella 
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(50.0f, 0.0f, 70.0f));
	model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	entornoEstrella.RenderModel();

	//Puente de Londres

	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(96.0f, 6.0f, 30.0f));
	model = glm::scale(model, glm::vec3(0.220f, 0.350f, 0.20f));
	model = glm::rotate(model, 145 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	PuenteLondon.RenderModel();

	//Cascada
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(130.0f, 3.0f, 0.0f));
	model = glm::scale(model, glm::vec3(15.0f, 10.0f, 15.0f));
	model = glm::rotate(model, -135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Mountain.RenderModel();
	//TIERRA 
	//Entorno de piramide
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(135.0f, 0.0f, -50.0f));
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	entornoArena.RenderModel();

	//Eiffel Tower
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(145.0f, 0.0f, -25.0f));
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	EiffelTower.RenderModel();



	//Mundo rocoso
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(140.5f, 0.2f, -100.5f));
	model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
	model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	entornoRocoso.RenderModel();

	//Rosa room
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-35.5f, 8.0f, -135.5f));
	model = glm::scale(model, glm::vec3(0.250f, 0.350f, 0.20f));
	model = glm::rotate(model, -135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	rosaRoom.RenderModel();

	//Reino Koopa 
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(110.5f, -1.50f, -150.5f));
	model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
	model = glm::rotate(model, -120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	entornoBowser.RenderModel();

	//Reino champiñon
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(60.0f, 0.2f, -155.5f));
	model = glm::scale(model, glm::vec3(9.0f,9.0f, 9.0f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	entornoGomba.RenderModel();


	//Isla de Yoshi
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(20.0f, 0.5f, -150.0f));
	model = glm::scale(model, glm::vec3(3.50f, 3.50f, 3.50f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	entornoYoshi.RenderModel();


	//FUEGO
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-58.0f, -1.0f, -70.0f));
	model = glm::scale(model, glm::vec3(4.0f, 6.0f, 7.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	entornoFlores.RenderModel();
	//volcan
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-70.0f, -.50f, 25.0f));
	model = glm::scale(model, glm::vec3(30.0f, 85.0f, 60.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Volcan.RenderModel();
	
	// Reino de lava
	model = glm::mat4(1.0);
	
	model = glm::translate(model, glm::vec3(-2.50f, 0.0f, 70.0f));
	model = glm::scale(model, glm::vec3(4.0f, 6.0f,5.0f));
	model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	entornoLava.RenderModel();

	//Marte
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-70.0f, -13.0f, -50.5f));
	model = glm::scale(model, glm::vec3(150.0f, 320.0f, 320.0f));
	model = glm::rotate(model, 95 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Marte.RenderModel();
	
	
}

void animacionLicuadora(float posFinal, float dirFinal)
{
	float aux = 0.0f;
	if (framesLicua == 1)
	{
		aux = 10.0f + posFinal;
		cambioPosMods = aux / 100.0f;

		aux = dirFinal - 720.0f;
		cambioDirMods = aux / 100.0f;
	}
	if (framesLicua <= 100) //animacion de "ida"
	{
		posAnimMods += cambioPosMods;
		dirAnimMods += cambioDirMods;
	}
	else if (framesLicua > 250 && framesLicua <= 350) //animacion de "regreso"
	{
		posAnimMods -= cambioPosMods;
		dirAnimMods -= cambioDirMods;
	}
}


//Función para el sonido 
void updateListenerPositionAndOrientation(Camera& cameraMain, ISoundEngine* engine) {
	glm::vec3 listenerPos = cameraMain.getCameraPosition();
	glm::vec3 listenerLookDir = cameraMain.getCameraDirection();
	glm::vec3 listenerUpVector(0.0f, 1.0f, 0.0f); 
	glm::vec3 listenerVelPerSecond(0.0f, 0.0f, 0.0f);

	
	engine->setListenerPosition(
		vec3df(listenerPos.x, listenerPos.y, listenerPos.z),
		vec3df(listenerLookDir.x, listenerLookDir.y, listenerLookDir.z),
		vec3df(listenerVelPerSecond.x, listenerVelPerSecond.y, listenerVelPerSecond.z),
		vec3df(listenerUpVector.x, listenerUpVector.y, listenerUpVector.z)
	);
}

 

void animacionCaminata()
{
	float paso = 10.1f / (float) maxFramesCamin; //distancia que camina por cada "paso" que da

	if (casAct >= 1 && casAct <= 9)
	{
		posAvatarX += paso;
	}
	if (casAct >= 10 && casAct <= 20)
	{
		posAvatarZ -= paso;
	}
	if (casAct >= 21 && casAct <= 29)
	{
		posAvatarX -= paso;
	}
	if (casAct >= 30 && casAct <= 40)
	{
		posAvatarZ += paso;
	}
}


int main()
{
	// start the sound engine with default parameters
	ISoundEngine* engine = irrklang::createIrrKlangDevice();

	if (!engine)
	{
		printf("Could not startup engine\n");
		return 0; // error starting up the engine
	}

	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	crearDados();
	CreateShaders();

	
	
	// Reproducir un sonido en una posición 3D específica
	vec3df soundPos(130.0f, 3.0f, 0.0f); // Posición del sonido en el espacio 3D
	ISound* sound = engine->play3D("media/agua.mp3", soundPos,true, false, true);
	if (sound) {
		sound->setMinDistance(1.0f); // Distancia mínima a la cual el sonido se escucha en su volumen máximo
		sound->setMaxDistance(2.0);
		sound->setVolume(1.0);
	}

	vec3df soundPos2(-2.50f, 0.0f, 70.0f); // Posición del sonido en el espacio 3D
	ISound* sound2 = engine->play3D("media/lava.mp3", soundPos2, true, false, true);
	if (sound2) {
		sound2->setMinDistance(1.0f); // Distancia mínima a la cual el sonido se escucha en su volumen máximo
		sound2->setMaxDistance(2.0);
		sound2->setVolume(1.0);
	}



	cargarTexturas();
	cargarModelos();

	printf("Cargando skybox...\n");
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

	printf("Creando luces...\n");
	//luz direccional, solo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.6f, 0.6f,
		0.0f, 0.0f, -1.0f);

	//contador de luces puntuales


	unsigned int pointLightCount = 0;

	//Luz blanca que ilumina al modelo del sol
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.5f, 0.3f,
		0.0f, 0.0f, 0.0f,
		0.2f, 0.01f, 0.001f);
	pointLightCount++;

	//Luz azul que seguira al avatar
	pointLights[1] = PointLight(0.0f, 0.0f, 1.0f,
		0.5f, 0.3f,
		0.0f, 0.0f, 0.0f,
		0.2f, 0.01f, 0.001f);
	pointLightCount++;

	//luz amarilla para las lamparas
	pointLights[2] = PointLight(1.0f, 1.0f, 0.4f,
		0.5f, 0.3f,
		0.0f, 0.0f, 0.0f,
		0.2f, 0.01f, 0.001f);
	pointLightCount++;

	unsigned int spotLightCount = 0;

	printf("Inicializando variables...\n");
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	rotaDado4X = 0.0f;
	rotaDado4Y = 0.0f;
	rotaDado4Z = 0.0f;
	rotaDado8X = 0.0f;
	rotaDado8Y = 0.0f;
	rotaDado8Z = 0.0f;
	posDados = 6.35f;
	// play some sound stream, looped
	engine->play2D("media/getout.ogg", true);
	engine->setSoundVolume(0.01);

	

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	float angulovaria = 0.0f;

	// Variables para el movimiento circular
	float r = 250.0f;     // Radio constante de la circunferencia
	float theta = 0.0f; // Ángulo polar inicial
	float deltaTheta = 0.003f; // Incremento del ángulo en cada frame (velocidad angular)

	int casilla;
	float movOffset = 0.5f;


	printf("Comenzando ejecucion...\n\n");

	printf(" [F]\tTirar los dados.\n");
	printf(" [K]\tAlternar entre camaras.\n\n");
	printf(" ===== En camara ortogonal =====\n");
	printf(" [W]+[D]\tAdelante.\n");
	printf(" [A]\t\tIzquierda.\n");
	printf(" [S]+[A]\tAtras.\n");
	printf(" [D]\t\tDerecha.\n");


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.9f * deltaTime;

		if (mainWindow.getTiroDados())
		{


			


			engine->play2D("media/DadoCaida.mp3");
			//engine->setSoundVolume(1.0f);


			mainWindow.resetTiroDados();

			numD4 = mainWindow.getNumDado4();
			printf(" >>Numero de dado blanco: [%d]\n", numD4);

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
			printf(" >>Numero de dado negro: [%d]\n", numD8);

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
			casRest = numTotal;

			//calcular la casilla destino
			casDest = casAct + numTotal;
			if (casDest >= 40) {
				int aux = casDest - 40;
				casDest = aux + 1;
			}

			framesDados = 1;
			
			//printf("El personaje se encuentra en la casilla [%d]\n\n", casAct);
			//printf("La ubicacion de la casilla es [%f, %f]", pos[casAct - 1][0], pos[casAct - 1][1]);
			animActiva = true;
		}

		//control para animacion de dados
		if (framesDados == 1)
		{
			//printf("[Animacion comenzada]\n");
			posDados = 6.35f;
			dirDado4X = 0.0f;
			dirDado4Y = 0.0f;
			dirDado4Z = 0.0f;
			dirDado8X = 0.0f;
			dirDado8Y = 0.0f;
			dirDado8Z = 0.0f;
			framesDados = 2;
		}
		else if (framesDados >= 2 && framesDados < 50)
		{
			animacionCaida();
			framesDados++;
		}
		else if (framesDados >= 50 && framesDados < 100)
		{
			animacionGiroD4(rotaDado4X, rotaDado4Y, rotaDado4Z);
			animacionGiroD8(rotaDado8X, rotaDado8Y, rotaDado8Z);
			framesDados++;
		}
		else if (framesDados == 100)
		{
			//printf("[Animacion terminada]\n\n\n");
			printf(" >>El personaje avanza %d casillas.\n", numTotal);
			framesDados = 0;
			caminando = true; //cuando termine la animacion de los dados, comienza la caminata
		}

		//Control de caminata
		if (caminando)
		{
			if (casRest > 0)
			{
				if (framesCamin < 30)
				{
					animacionCaminata();
					framesCamin++;
				}
				else
				{
					float auxSwap;
					framesCamin = 0;
					casRest--;

					casAct++;
					if (casAct == 41)
						casAct = 1;

					switch (casAct) //manejar rotaciones de personaje y camara
					{
						case 1:
							dirAvatar = 90.0f;
							offsetCamaraZ = 15.0f;
							offsetCamaraX = 0.0f;
							dirCamara = 0.0f;
							break;
						case 10:
							dirAvatar = 180.0f;
							offsetCamaraZ = 15.0f;
							offsetCamaraX = 0.0f;
							dirCamara = -90.0f;
							break;
						case 21:
							dirAvatar = -90.0f;
							offsetCamaraZ = 0.0f;
							offsetCamaraX = 15.0f;
							dirCamara = 180.0f;
							break;
						case 30:
							dirAvatar = 0.0f;
							offsetCamaraZ = -15.0f;
							offsetCamaraX = 0.0f;
							dirCamara = 90.0f;
							break;
						default:
							break;
					}
				}
			}
			else
			{
				caminando = false;
				framesLicua = 1; //al terminar caminata, comienza animacion de modelo de casilla
			}
		}

		//control para animacion licuadora
		if (framesLicua >= 1 && framesLicua < 350)
		{
			animacionLicuadora(posiciones[casAct - 1], direcciones[casAct - 1]);
			framesLicua++;
		}
		else if (framesLicua >= 350)
		{
			framesLicua = 0;
			animActiva = false;
		}



		/*
		Hacer:
			if casAct es igual a indice de una esquina
				se hace rotacion del avatar

			contador de 1 en 1 para casAct, para lograr que
			if casAct == 40
				reiniciar a casAct = 0 y seguir sumando los restantes
			prender bandera de paso por inicio para animaciones especiales
		*/

		//ANIMACION PAARA HOLD ON BELLA
		Time = now - pasTime;
		//Time += (now - pasTime) / limitFPS;
		pasTime = now;

		// Actualizar el tiempo transcurrido
		lapso += Time;

		// Verificar si hemos completado un ciclo completo
		if (lapso >= ciclo) {
			lapso = 0.0f;  // Resetear el tiempo transcurrido después de un ciclo completo
		}

		GLfloat Tiempo = fmod(lapso, ciclo);

		if (Tiempo < duracion_estado) {
			// Moviendo en el eje X hacia la derecha
			movBellaX = 4.0f * (Tiempo / duracion_estado);
			
			rotacion_angulo = 0.0f;
		}
		else if (Tiempo < 2 * duracion_estado) {
			// Moviendo en el eje X hacia el centro
			movBellaX = 4.0f * (1.0f - (Tiempo - duracion_estado) / duracion_estado);
			rotacion_angulo = 360.0f * ((Tiempo - duracion_estado) / duracion_estado);
			//rotacion_angulo = 0.0f;
		}
		else if (Tiempo < 3 * duracion_estado) {
			// Moviendo en el eje X hacia la izquierda
			movBellaX = -4.0f * ((Tiempo - 2 * duracion_estado) / duracion_estado);
			rotacion_angulo = 0.0f;
		}
		else if (Tiempo < 4 * duracion_estado) {
			// Moviendo en el eje X hacia el centro
			movBellaX = -4.0f * (1.0f - (Tiempo - 3 * duracion_estado) / duracion_estado);
			rotacion_angulo = 360.0f * ((Tiempo - 3 * duracion_estado) / duracion_estado);
		//	rotacion_angulo = 0.0f;
		}
		
		
		else {
			// Rotación de 360 grados
			movBellaX = 0.0f;
		
			rotacion_angulo = 360.0f * ((Tiempo - 4 * duracion_estado/2) / duracion_estado);
		}

//=======
//		//Recibir eventos del usuario
		glfwPollEvents();
		//camera.keyControl(mainWindow.getsKeys(), deltaTime);
		//camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		
		
	
		updateListenerPositionAndOrientation(mainCamera, engine);

			

		// Render loop
		glfwPollEvents();


		// Actualiza el modo de cámara según la tecla presionada
		mainCamera.switchCameraMode(cameraMode, mainWindow.getsKeys());

		glm::mat4 viewMatrix;
		glm::vec3 eyePosition;

		// Dependiendo del modo de cámara, calcula la vista y la posición de la cámara
		switch (cameraMode) {
		case 0: // Static Camera
			viewMatrix = isoCamera.calculateViewMatrix();
			eyePosition = isoCamera.getCameraPosition();
			break;
		case 1: // First-Person Camera
			mainCamera.keyControl(mainWindow.getsKeys(), deltaTime);
			mainCamera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			viewMatrix = mainCamera.calculateViewMatrix();
			eyePosition = mainCamera.getCameraPosition();
			break;
		case 2: // Follow Camera
			viewMatrix = followCamera.calculateViewMatrix();
			eyePosition = followCamera.getCameraPosition();
			break;
		}

		// Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Dibujar el skybox
		skybox.DrawSkybox(viewMatrix, projection);

		// Usar el shader
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		// Información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		// Enviar las matrices de proyección y vista al shader
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniform3f(uniformEyePosition, eyePosition.x, eyePosition.y, eyePosition.z);

		// Lógica de luces asociadas a la cámara activa
		glm::vec3 lowerLight = eyePosition;
		lowerLight.y -= 0.3f; // Ajuste de la posición de la luz spot

		// Actualizar la dirección de la luz spot solo si no es la cámara estática
		if (cameraMode != 0) {
			spotLights[0].SetFlash(lowerLight, mainCamera.getCameraDirection());
		}
		else {
			// Si es la cámara estática, no actualizamos la dirección de la luz spot
			spotLights[0].SetFlash(lowerLight, glm::vec3(0.0f, -1.0f, 0.0f)); // Dirección predeterminada
		}

		// Configurar las luces direccionales
		shaderList[0].SetDirectionalLight(&mainLight);

		// Configuración de luces de punto en función de si es de noche o no
		if (esNoche) {
			if (casAct == 1 || casAct == 10 || casAct == 20 || casAct == 30) {
				shaderList[0].SetPointLights(pointLights, pointLightCount);
			}
			else {
				shaderList[0].SetPointLights(pointLights, pointLightCount - 1);
			}
		}
		else {
			shaderList[0].SetPointLights(pointLights, pointLightCount - 2);
		}

		// Configurar las luces tipo spot (flash) en función de la cámara activa
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		//instancia de piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, -50.0f));
		model = glm::scale(model, glm::vec3(15.0f, 1.0f, 15.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		crearTablero(esNoche);

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

		renderizarModelosDoom(model, uniformModel, modelaux);

		renderizarModelosMinion(model, uniformModel, modelaux);

		renderizarModelosBella(model, uniformModel, modelaux);

		renderizarModelosMario(model, uniformModel, modelaux);

		renderizarModelosCiudad(model, uniformModel, modelaux);

		//instancia de cacodemon avatar
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.0f, 6.0f + (sin(glm::radians(angulovaria * 2))), -55.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, (90 + (5 * cos(glm::radians(angulovaria * 2.5f)))) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cacodemon.RenderModel();

		//Instancia del minion avatar
		//Cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(posAvatarX, 0.5f, posAvatarZ));

		// Llama a `followTarget` usando `model` y `dirCamara`
		if (cameraMode == 2) {
			followCamera.followTarget(model, offsetCamaraX, offsetCamaraY, offsetCamaraZ, dirCamara);
		}

		// Aplica transformaciones adicionales al modelo del Minion
		model = glm::rotate(model, dirAvatar * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		modelaux = model;


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MinionAvatarCuerpo.RenderModel();
		model = modelaux;

		//Brazos
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.3f));
		model = glm::rotate(model, sin(glm::radians(angulovaria)) * 15.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MinionAvatarBrazoIzq.RenderModel();
		model = modelaux;


		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.3f));
		model = glm::rotate(model, cos(glm::radians(angulovaria)) * 15.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MinionAvatarBrazoDer.RenderModel();
		model = modelaux;

		//Piernas
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, cos(glm::radians(angulovaria)) * 15.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MinionAvatarPiernaIzq.RenderModel();
		model = modelaux;

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, sin(glm::radians(angulovaria)) * 15.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MinionAvatarPiernaDer.RenderModel();

		//Iluminacion de personaje
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(pos[casAct - 1][0], 0.2f, pos[casAct - 1][1])); //para mover todo el tablero se restan o suman valores a posX y posZ
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[3]->RenderMesh();
		if (esNoche)
		{
			pointLights[1].SetPosicion(glm::vec3(model[3][0] + 0.0f, model[3][1] + 20.0f, model[3][2] + 0.0f));
		}

		// Calcular la posición en la circunferencia usando las ecuaciones paramétricas
		float x = r * std::cos(theta);
		float y = r * std::sin(theta);

		// Incrementar theta para el siguiente frame
		theta += deltaTheta;

		// Calcular la rotación necesaria para que el modelo mire hacia el centro
		// La rotación es igual a -theta en este caso
		rotationAngle = theta + glm::half_pi<float>(); // Ajuste de 90° para "mirar" al centro

		//Sol
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(x, y, 0.0f));
		model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sol.RenderModel();
		//Luz para que el sol siempre se vea iluminado
		pointLights[0].SetPosicion(glm::vec3(model[3][0] + 0.0f, model[3][1] + 5.0f, model[3][2] + 0.0f));

		//angulo de iluminacion del sol
		anguloLuzY = -round(y) / 250;
		anguloLuzX = 1.0f - anguloLuzY;

		//ciclo de dia y noche, cambia color y direccion de luz global sol
		if (y > 125.0f) //iluminacion max
		{
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
				0.6f, 0.6f,
				anguloLuzX, anguloLuzY, 0.0f);
			esNoche = false;
		}
		else if (y > 0.0f && y <= 125.0f) //max disminuida
		{
			mainLight = DirectionalLight(0.5f, 0.6f, 0.95f,
				0.6f, 0.6f,
				anguloLuzX, anguloLuzY, 0.0f);
			esNoche = false;
		}
		else if (y < 0.0f && y >= -125.0f) //max mas disminuida
		{
			mainLight = DirectionalLight(0.5f, 0.6f, 0.875f,
				0.6f, 0.6f,
				anguloLuzX, anguloLuzY, 0.0f);
			esNoche = true;
		}
		else if (y < -125.0f) //iluminacion min
		{
			mainLight = DirectionalLight(0.5f, 0.6f, 0.70f,
				0.6f, 0.6f,
				anguloLuzX, anguloLuzY, 0.0f);
			esNoche = true;
		}
		model = modelaux;

		

		//INSTANCIA DE BELLA ANIMACION HOLD ON
		// 
		// model = glm::rotate(model, rotacion_angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//Cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-12.0, 0.0, movBellaX));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotacion_angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		modelaux = model;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bellatronco.RenderModel();
		model = modelaux;

		//Brazos
		model = glm::translate(model, glm::vec3(0.108f, 1.42f, 0.0f));
		//model = glm::rotate(model, sin(glm::radians(angulovaria)) * 5.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bellabrazo.RenderModel();
		model = modelaux;


		model = glm::translate(model, glm::vec3(-0.108f, 1.415f, 0.0f));
		//model = glm::rotate(model, cos(glm::radians(angulovaria)) * 5.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bellabrazotwo.RenderModel();
		model = modelaux;

		//Piernas
		model = glm::translate(model, glm::vec3(0.05f, 0.950f, -0.20f));
		//model = glm::rotate(model, cos(glm::radians(angulovaria)) * 15.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bellapierna1.RenderModel();
		model = modelaux;

		model = glm::translate(model, glm::vec3(-.01f, 0.950f, -0.20f));
		//model = glm::rotate(model, sin(glm::radians(angulovaria)) * 15.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bellapiernatwo.RenderModel();
		model = modelaux;


		//Iluminacion

		//Instancia de lampara 1 azul
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 0.5f, 10.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, 135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();
		if (casAct == 1 && esNoche)
		{
			pointLights[2].SetPosicion(glm::vec3(posLamparas[0][0], 30.0f, posLamparas[0][1]));
		}

		//Instancia de lampara 2 amarilla
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 0.5f, 10.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, -135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();
		if (casAct == 10 && esNoche)
		{
			pointLights[2].SetPosicion(glm::vec3(posLamparas[1][0], 30.0f, posLamparas[1][1]));
		}

		//Instancia de lampara 3 verde
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 0.5f, -120.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();
		if (casAct == 20 && esNoche)
		{
			pointLights[2].SetPosicion(glm::vec3(posLamparas[2][0], 30.0f, posLamparas[2][1]));
		}

		//Instancia de lampara 4 roja
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.5f, -120.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();

		if (casAct == 30 && esNoche)
		{
			pointLights[2].SetPosicion(glm::vec3(posLamparas[3][0], 30.0f, posLamparas[3][1]));
		}

		

		
		




		

		glUseProgram(0);

		mainWindow.swapBuffers();
	}
	sound2->drop();
	sound->drop();
	engine->drop(); // delete engine

	return 0;
}