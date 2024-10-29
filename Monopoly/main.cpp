/*
Práctica 6: Texturizado
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

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

Model Minion;
Model MinionMorado;
Model Vector;
Model Gru;
Model CarroGru;
Model FortalezaVector;
Model MinionBebe;



Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";




//cálculo del promedio de las normales para sombreado de Phong
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

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);


	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	
	Minion = Model();
	Minion.LoadModel("Models/MinionNormalTexturizado.obj");

	MinionBebe = Model();
	MinionBebe.LoadModel("Models/MinionBebeTexturizado.obj");

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

	AmTexture = Texture("Textures/amarillo.png");
	AmTexture.LoadTextureA();
	RoTexture = Texture("Textures/rojo.png");
	RoTexture.LoadTextureA();
	AzTexture = Texture("Textures/azul.png");
	AzTexture.LoadTextureA();
	VeTexture = Texture("Textures/verde.png");
	VeTexture.LoadTextureA();

	
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
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
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		modelaux = model;

		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();

		//Instancia del minion 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Minion.RenderModel();


		//Instancia del minion morado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, -2.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(10.5f, 10.5f, 10.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MinionMorado.RenderModel();


		//Instancia del Dany flow (trash)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.0f, 0.0f, -20.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Vector.RenderModel();

		//Instancia de gru
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, 0.0f, -20.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.4f, 1.4f, 1.4f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gru.RenderModel();

		//Instancia de carro de gru
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-75.0f, 0.0f, -60.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(5.2f, 5.2f, 5.2f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CarroGru.RenderModel();


		//Instancia de Fortaleza de vector
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(75.0f, 0.0f, 60.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FortalezaVector.RenderModel();

		//Instancia del minion bebe 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, -10.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MinionBebe.RenderModel();


		//casillas de tablero

		//casillas rojas
		RoTexture.UseTexture();

		model = glm::mat4(1.0); //casilla 1
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 4
		model = glm::translate(model, glm::vec3(30.3f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 7
		model = glm::translate(model, glm::vec3(60.6f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 10
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 13
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -30.3f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 19
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -90.9f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 24
		model = glm::translate(model, glm::vec3(60.6f, 0.2f, -111.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 28
		model = glm::translate(model, glm::vec3(20.2f, 0.2f, -111.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 34
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -70.7f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 38
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -30.3f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		//casillas amarillas
		AmTexture.UseTexture();

		model = glm::mat4(1.0); //casilla 2
		model = glm::translate(model, glm::vec3(10.1f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 6
		model = glm::translate(model, glm::vec3(50.5f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 9
		model = glm::translate(model, glm::vec3(80.8f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 12
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -20.2f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 14
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -40.4f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 17
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -70.7f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 20
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -101.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 25
		model = glm::translate(model, glm::vec3(50.5f, 0.2f, -111.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 30
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -111.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 32
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -90.9f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 37
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -40.4f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		//casillas azules
		AzTexture.UseTexture();

		model = glm::mat4(1.0); //casilla 3
		model = glm::translate(model, glm::vec3(20.2f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 8
		model = glm::translate(model, glm::vec3(70.7f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 11
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -10.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 15
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -50.5f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 18
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -80.8f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 22
		model = glm::translate(model, glm::vec3(80.8f, 0.2f, -111.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 26
		model = glm::translate(model, glm::vec3(40.4f, 0.2f, -111.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 31
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -101.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 36
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -50.5f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 39
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -10.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		//casillas verdes
		VeTexture.UseTexture();

		model = glm::mat4(1.0); //casilla 5
		model = glm::translate(model, glm::vec3(40.4f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 16
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -60.6f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 21
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -111.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 23
		model = glm::translate(model, glm::vec3(70.7f, 0.2f, -111.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 27
		model = glm::translate(model, glm::vec3(30.3f, 0.2f, -111.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 29
		model = glm::translate(model, glm::vec3(10.1f, 0.2f, -111.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 33
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -80.8f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 35
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -60.6f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 39
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -20.2f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
