#include "SceneGalaxy.h"
#include "GL\glew.h"

#include "Scene2.h"
#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "KeyboardController.h"


// GLM Headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

Scene2::Scene2()
{
}

Scene2::~Scene2()
{
}

void Scene2::Init()
{
	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glEnable(GL_CULL_FACE);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Load the shader programs
	m_programID = LoadShaders("Shader//TransformVertexShader.vertexshader",
								"Shader//SimpleFragmentShader.fragmentshader");
	glUseProgram(m_programID);

	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");

	// Init VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}

	// Initialise camera properties
	camera.Init(45.f, 45.f, 10.f);

	// Define the view matrix and set it with camera position, target position and up direction
	glm::mat4 view = glm::lookAt(
		camera.position,
		camera.target,
		camera.up
	);

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("Axes", 10000.f, 10000.f, 10000.f);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("Quad", 2, 2);
	meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("Circle", Color(1.f, 1.f, 1.f),1.f, 12);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sphere", Color(1.f, 1.f, 1.f),1.f, 12, 12);
	meshList[GEO_TORUS] = MeshBuilder::GenerateTorus("Torus", Color(1.f, 1.f, 1.f), 1.f,2.f, 12, 12);
}

void Scene2::Update(double dt)
{
	// Check for key press, you can add more for interaction
	HandleKeyPress();

	camera.Update(dt);
}

void Scene2::Render()
{
	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup Model View Projection matrix

	// Render objects

	// Define the model matrix and set it to identity
	glm::mat4 model = glm::mat4(1.0f);

	// Define the view matrix and set it with camera position, target position and up direction
	glm::mat4 view = glm::lookAt(
		camera.position,
		camera.target,
		camera.up
	);

	// Define the projection matrix
	glm::mat4 projection = glm::mat4(1.f);
	switch (projType) {
	case 0:
		projection = glm::ortho(-10.f, 10.f, -10.f, 10.f, 0.1f, 1000.f);
		break;
	default:
		projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	}


	// Calculate the Model-View-Project matrix
	glm::mat4 MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));


	meshList[GEO_AXES]->Render();
	meshList[GEO_QUAD]->Render();
	meshList[GEO_CIRCLE]->Render();
	meshList[GEO_SPHERE]->Render();
	meshList[GEO_TORUS]->Render();
}

void Scene2::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
		{
			delete meshList[i];
		}
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void Scene2::HandleKeyPress() 
{

	if (KeyboardController::GetInstance()->IsKeyPressed(0x31))
	{
		// Key press to enable culling
		glEnable(GL_CULL_FACE);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(0x32))
	{
		// Key press to disable culling
		glDisable(GL_CULL_FACE);
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(0x33))
	{
		// Key press to enable fill mode for the polygon
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(0x34))
	{
		// Key press to enable wireframe mode for the polygon
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
	{
		// Change to black background
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	}

	if (Application::IsKeyPressed('P')) {
		if (projType == 0)
			projType = 1;
		else
			projType = 0;
	}

	//if (Application::IsKeyPressed(0x31))
	//{
	//	// Key press to enable culling
	//	glEnable(GL_CULL_FACE);
	//}
	//if (Application::IsKeyPressed(0x32))
	//{
	//	// Key press to disable culling
	//	glDisable(GL_CULL_FACE);
	//}
	//if (Application::IsKeyPressed(0x33))
	//{
	//	// Key press to enable fill mode for the polygon
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	//}
	//if (Application::IsKeyPressed(0x34))
	//{
	//	// Key press to enable wireframe mode for the polygon
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
	//}
}
