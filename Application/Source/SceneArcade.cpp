#include "SceneArcade.h"
#include "GL\glew.h"

// GLM Headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

//Include GLFW
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "KeyboardController.h"
#include "LoadTGA.h"
#include "MouseController.h"

#include <iostream>
#include <string> 
using namespace std;

SceneArcade::SceneArcade()
{
	launch1 = false;
	launch2 = false;
	skeeballIntro = false;
	highStrikerIntro = false;
	playing1 = false;
	playing2 = false;
	balls = 9;
	angle = 85.f;
	arrowAngle = 180.f;
	added = false;
	gameOver = false;
	skeeballOutro = true;
	exitPrompt = false;
}

SceneArcade::~SceneArcade()
{
}

void SceneArcade::Init()
{
	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glDisable(GL_CULL_FACE);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Load the shader programs
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	glUseProgram(m_programID);

	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");

	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	//m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	//m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	//m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	//m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	//m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	//m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	//m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	//m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	//m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	//m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	//m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

	//m_parameters[U_LIGHT3_TYPE] = glGetUniformLocation(m_programID, "lights[3].type");
	//m_parameters[U_LIGHT3_POSITION] = glGetUniformLocation(m_programID, "lights[3].position_cameraspace");
	//m_parameters[U_LIGHT3_COLOR] = glGetUniformLocation(m_programID, "lights[3].color");
	//m_parameters[U_LIGHT3_POWER] = glGetUniformLocation(m_programID, "lights[3].power");
	//m_parameters[U_LIGHT3_KC] = glGetUniformLocation(m_programID, "lights[3].kC");
	//m_parameters[U_LIGHT3_KL] = glGetUniformLocation(m_programID, "lights[3].kL");
	//m_parameters[U_LIGHT3_KQ] = glGetUniformLocation(m_programID, "lights[3].kQ");
	//m_parameters[U_LIGHT3_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[3].spotDirection");
	//m_parameters[U_LIGHT3_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[3].cosCutoff");
	//m_parameters[U_LIGHT3_COSINNER] = glGetUniformLocation(m_programID, "lights[3].cosInner");
	//m_parameters[U_LIGHT3_EXPONENT] = glGetUniformLocation(m_programID, "lights[3].exponent");

	//m_parameters[U_LIGHT4_TYPE] = glGetUniformLocation(m_programID, "lights[4].type");
	//m_parameters[U_LIGHT4_POSITION] = glGetUniformLocation(m_programID, "lights[4].position_cameraspace");
	//m_parameters[U_LIGHT4_COLOR] = glGetUniformLocation(m_programID, "lights[4].color");
	//m_parameters[U_LIGHT4_POWER] = glGetUniformLocation(m_programID, "lights[4].power");
	//m_parameters[U_LIGHT4_KC] = glGetUniformLocation(m_programID, "lights[4].kC");
	//m_parameters[U_LIGHT4_KL] = glGetUniformLocation(m_programID, "lights[4].kL");
	//m_parameters[U_LIGHT4_KQ] = glGetUniformLocation(m_programID, "lights[4].kQ");
	//m_parameters[U_LIGHT4_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[4].spotDirection");
	//m_parameters[U_LIGHT4_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[4].cosCutoff");
	//m_parameters[U_LIGHT4_COSINNER] = glGetUniformLocation(m_programID, "lights[4].cosInner");
	//m_parameters[U_LIGHT4_EXPONENT] = glGetUniformLocation(m_programID, "lights[4].exponent");

	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	// Initialise camera properties
	FPScamera.Init(glm::vec3 (0.f, 0.f, 0.f), glm::vec3(0, 0, 15), glm::vec3(0, 1, 0));

	// Init VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("Axes", 10000.f, 10000.f, 10000.f);
	meshList[GEO_LIGHT] = MeshBuilder::GenerateSphere("Light", Color(1.f, 1.f, 1.f), 1.f, 16, 16);

	// Initialize skybox
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("Plane", Color(1.f, 1.f, 1.f), 200.f, 50.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//Arcade_Front.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("Plane", Color(1.f, 1.f, 1.f), 200.f, 50.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//Arcade_Back.tga");
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("Plane", Color(1.f, 1.f, 1.f), 200.f, 50.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//Arcade_Left.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("Plane", Color(1.f, 1.f, 1.f), 200.f, 50.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//Arcade_Right.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("Plane", Color(1.f, 1.f, 1.f), 200.f, 200.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//Arcade_Top.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("Plane", Color(1.f, 1.f, 1.f), 200.f, 200.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//Arcade_Bottom.tga");

	// Initialize skeeball machine
	meshList[GEO_SKEEBALL] = MeshBuilder::GenerateOBJ("skeeball", "OBJ//Skeeball.obj");
	meshList[GEO_SKEEBALL]->textureID = LoadTGA("Image//Skeeball.tga");

	// Initialize ball
	meshList[GEO_BALL] = MeshBuilder::GenerateSphere("Ball", Color(1.0f, 1.0f, 1.0f), 1.f, 16, 16);

	// Initialize wall
	meshList[GEO_WALL] = MeshBuilder::GenerateCube("Wall", Color(1.0f, 1.0f, 1.0f), 1.f);

	// Initialize skeeball intro GUI
	meshList[GEO_SKEEBALL_INTRO_GUI] = MeshBuilder::GenerateQuad("Plane", Color(1.f, 1.f, 1.f), 500.f, 500.f);
	meshList[GEO_SKEEBALL_INTRO_GUI]->textureID = LoadTGA("Image//SkeeballIntroGUI.tga");

	// Initialize power meter GUI
	meshList[GEO_POWERMETER_GUI] = MeshBuilder::GenerateQuad("Plane", Color(1.f, 1.f, 1.f), 500.f, 500.f);
	meshList[GEO_POWERMETER_GUI]->textureID = LoadTGA("Image//PowerMeterGUI.tga");

	// Initialize ball counter GUI
	meshList[GEO_BALL_COUNTER_GUI] = MeshBuilder::GenerateQuad("Plane", Color(1.f, 1.f, 1.f), 500.f, 500.f);
	meshList[GEO_BALL_COUNTER_GUI]->textureID = LoadTGA("Image//BallCounterGUI.tga");

	// Initialize arrow
	meshList[GEO_ARROW] = MeshBuilder::GenerateOBJ("Arrow", "OBJ//Arrow.obj");
	meshList[GEO_ARROW]->textureID = LoadTGA("Image//Arrow.tga");

	// Initialize high striker machine
	meshList[GEO_HIGHSTRIKER] = MeshBuilder::GenerateOBJ("highstriker", "OBJ//HighStriker.obj");
	meshList[GEO_HIGHSTRIKER]->textureID = LoadTGA("Image//HighStriker.tga");

	// Initialize hammer
	meshList[GEO_HAMMER] = MeshBuilder::GenerateOBJ("highstriker", "OBJ//Hammer.obj");
	meshList[GEO_HAMMER]->textureID = LoadTGA("Image//Hammer.tga");

	// Initialize puck
	meshList[GEO_PUCK] = MeshBuilder::GenerateCube("Puck", Color(1.0f, 1.0f, 1.0f), 1.f);

	// Initialize high striker intro GUI
	meshList[GEO_HIGHSTRIKER_INTRO_GUI] = MeshBuilder::GenerateQuad("Plane", Color(1.f, 1.f, 1.f), 500.f, 500.f);
	meshList[GEO_HIGHSTRIKER_INTRO_GUI]->textureID = LoadTGA("Image//HighStrikerIntroGUI.tga");

	// Initialize cylinder (For skeeball goals)
	meshList[GEO_GOAL] = MeshBuilder::GenerateCube("Goal", Color(1.0f, 1.0f, 1.0f), 1.f);

	// Initialize circle
	meshList[GEO_GOAL] = MeshBuilder::GenerateCube("Goal", Color(1.0f, 1.0f, 1.0f), 1.f);

	// 16 x 16 is the number of columns and rows for the text
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//roboto.tga");

	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);

	glUniform1i(m_parameters[U_NUMLIGHTS], NUM_LIGHTS);

	// Skeeball Light 1
	light[0].position = glm::vec3(0.f, 26.f, 52.f);
	light[0].color.Set(1, 1, 1);
	light[0].type = Light::LIGHT_POINT;
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = 45.f;
	light[0].cosInner = 30.f;
	light[0].exponent = 3.f;
	light[0].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], cosf(glm::radians<float>(light[0].cosCutoff)));
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], cosf(glm::radians<float>(light[0].cosInner)));
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	// Skeeball Light 2
	light[1].position = glm::vec3(0.f, 26.f, 22.f);
	light[1].color.Set(1, 1, 1);
	light[1].type = Light::LIGHT_POINT;
	light[1].power = 1;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = 30.f;
	light[1].cosInner = 30.f;
	light[1].exponent = 3.f;
	light[1].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], cosf(glm::radians<float>(light[1].cosCutoff)));
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], cosf(glm::radians<float>(light[1].cosInner)));
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

	// High Striker Light
	light[2].position = glm::vec3(50.f, 26.f, 90.f);
	light[2].color.Set(1, 1, 1);
	light[2].type = Light::LIGHT_POINT;
	light[2].power = 1;
	light[2].kC = 1.f;
	light[2].kL = 0.01f;
	light[2].kQ = 0.001f;
	light[2].cosCutoff = 45.f;
	light[2].cosInner = 30.f;
	light[2].exponent = 3.f;
	light[2].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
	glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
	glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
	glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
	glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
	glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], cosf(glm::radians<float>(light[2].cosCutoff)));
	glUniform1f(m_parameters[U_LIGHT2_COSINNER], cosf(glm::radians<float>(light[2].cosInner)));
	glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);

	// Initialize skeeball machine
	m_skeeball = new GameObject(GameObject::GO_SKEEBALL);
	m_skeeball->pos = glm::vec3(0.f, -20.f, 100.f);
	m_skeeball->active = true;

	// Initialize ball
	m_ball = new GameObject(GameObject::GO_BALL);
	m_ball->pos = glm::vec3(0.f, -10.f, 35.f);
	m_ball->scale = glm::vec3(0.9f, 0.9f, 0.9f);
	m_ball->vel = glm::vec3(0.f, 0.f, 0.f);
	m_ball->mass = 1;
	m_ball->dir = glm::vec3(1.f, 0.f, 0.f), m_ball->multiplier = 1.0f / m_ball->mass;
	m_ball->active = true;

	// Initialize arrow
	m_arrow = new GameObject(GameObject::GO_ARROW);
	m_arrow->pos = glm::vec3(0.f, -10.f, 35.f);
	m_arrow->active = true;

	// Initialize high striker machine
	m_highstriker = new GameObject(GameObject::GO_HIGHSTRIKER);
	m_highstriker->pos = glm::vec3(50.f, -20.f, 90.f);
	m_highstriker->active = true;

	// Initialize hammer
	m_hammer = new GameObject(GameObject::GO_HAMMER);
	m_hammer->pos = glm::vec3(50.f, -13.f, 62.f);
	m_hammer->active = true;

	// Initialize puck
	m_puck = new GameObject(GameObject::GO_PUCK);
	m_puck->pos = glm::vec3(50.f, -20.f, 93.f);
	m_puck->vel = glm::vec3(0.f, 0.f, 0.f);
	m_puck->mass = 1;
	m_puck->active = true;

	// Initialize walls
	m_wall[0] = new GameObject(GameObject::GO_WALL);
	m_wall[0]->pos = glm::vec3(-13.f, -6.f, 55.f);
	m_wall[0]->scale = glm::vec3(10.f, 10.f, 55.f);
	m_wall[0]->active = true;

	m_wall[1] = new GameObject(GameObject::GO_WALL);
	m_wall[1]->pos = glm::vec3(13.f, -6.f, 55.f);
	m_wall[1]->scale = glm::vec3(10.f, 10.f, 55.f);
	m_wall[1]->active = true;

	m_wall[2] = new GameObject(GameObject::GO_WALL);
	m_wall[2]->pos = glm::vec3(-16.f, 7.f, 83.f);
	m_wall[2]->scale = glm::vec3(7.f, 35.f, 35.f);
	m_wall[2]->active = true;

	m_wall[3] = new GameObject(GameObject::GO_WALL);
	m_wall[3]->pos = glm::vec3(16.f, 7.f, 83.f);
	m_wall[3]->scale = glm::vec3(7.f, 35.f, 35.f);
	m_wall[3]->active = true;

	m_wall[4] = new GameObject(GameObject::GO_WALL);
	m_wall[4]->pos = glm::vec3(0.f, 4.5f, 97.f);
	m_wall[4]->scale = glm::vec3(25.f, 35.f, 10.f);
	m_wall[4]->active = true;

	// Initialize skeeball goals
	m_goal[0] = new GameObject(GameObject::GO_CYLINDER);
	m_goal[0]->pos = glm::vec3(0.f, 4.f, 93.8f);
	m_goal[0]->scale = glm::vec3(12.f, 9.8f, 12.f);
	m_goal[0]->active = true;

	m_goal[1] = new GameObject(GameObject::GO_CYLINDER);
	m_goal[1]->pos = glm::vec3(0.f, 7.5f, 88.f);
	m_goal[1]->scale = glm::vec3(3.f, 2.25f, 3.f);
	m_goal[1]->active = true;

	m_goal[2] = new GameObject(GameObject::GO_CYLINDER);
	m_goal[2]->pos = glm::vec3(0.f, 11.f, 91.f);
	m_goal[2]->scale = glm::vec3(3.f, 2.25f, 3.f);
	m_goal[2]->active = true;

	m_goal[3] = new GameObject(GameObject::GO_CYLINDER);
	m_goal[3]->pos = glm::vec3(7.2f, 13.3f, 93.f);
	m_goal[3]->scale = glm::vec3(3.f, 2.25f, 3.f);
	m_goal[3]->active = true;

	m_goal[4] = new GameObject(GameObject::GO_CYLINDER);
	m_goal[4]->pos = glm::vec3(-7.4f, 13.4f, 93.f);
	m_goal[4]->scale = glm::vec3(3.f, 2.25f, 3.f);
	m_goal[4]->active = true;

	//Calculating aspect ratio
	m_worldHeight = 500.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	enableLight = true;
}

void SceneArcade::Update(double dt)
{
	HandleKeyPress();

	if (moveCamera)
	{
		PrevCameraPosition = FPScamera.position;
		PrevTargetPosition = FPScamera.target;
	}

	// Check skybox collision
	if (FPScamera.position.x >= 98
		|| FPScamera.position.x <= -98
		|| FPScamera.position.z >= 98
		|| FPScamera.position.z <= -98)
	{
		moveCamera = false;
		FPScamera.position = PrevCameraPosition;
		FPScamera.target = PrevTargetPosition;
	}
	else
	{
		moveCamera = true;
	}

	// Check skeeball machine collision
	if (FPScamera.position.z >= m_skeeball->pos.z - 80
		&& FPScamera.position.z <= m_skeeball->pos.z + 3
		&& FPScamera.position.x <= m_skeeball->pos.x + 17
		&& FPScamera.position.x >= m_skeeball->pos.x - 17)
	{
		moveCamera = false;
		FPScamera.position = PrevCameraPosition;
		FPScamera.target = PrevTargetPosition;
		playPrompt1 = true;

		// Check if user presses E when shown prompt to play skeeball
		if (KeyboardController::GetInstance()->IsKeyDown('E') && !playing1)
		{
			FPScamera.position = glm::vec3(0.f, 0.f, 17.f);
			FPScamera.target = glm::vec3(0.f, 0.f, 35.f);
			playing1 = true;
			skeeballIntro = true;
		}
	}
	else
	{
		playPrompt1 = false;
	}

	// Check high striker machine collision
	if (FPScamera.position.z >= m_highstriker->pos.z - 10
		&& FPScamera.position.z <= m_highstriker->pos.z + 10
		&& FPScamera.position.x <= m_highstriker->pos.x + 20
		&& FPScamera.position.x >= m_highstriker->pos.x - 10)
	{
		moveCamera = false;
		FPScamera.position = PrevCameraPosition;
		FPScamera.target = PrevTargetPosition;
		playPrompt2 = true;

		// Check if user presses E when shown prompt to play high striker
		if (KeyboardController::GetInstance()->IsKeyDown('E') && !playing2)
		{
			FPScamera.position = glm::vec3(50.f, 0.f, 72.f);
			FPScamera.target = glm::vec3(50.f, -0.3f, 73.f);
			playing2 = true;
			highStrikerIntro = true;
		}
	}
	else
	{
		playPrompt2 = false;
	}

	// Main loop for skeeball machine
	if (launch1 && playing1)
	{
		// need all the forces to reset
		m_ball->force = glm::vec3(0, 0, 0);

		// Calculate ball multiplier
		m_ball->multiplier = 1.0f / m_ball->mass;

		//Calculate the resulting acceleration
		// F = ma.  Hence a = F/M = F * 1/M
		m_ball->acc = m_ball->force * m_ball->multiplier;

		// set initial velocity (U)
		m_ball->temp = m_ball->vel;
	
		//commpute the resulting velocity.
		// v = u + at
		m_ball->vel = (float)dt * time_scale * m_ball->acc + m_ball->temp;
		// Apply friction
		m_ball->vel.z -= (float)dt * time_scale * m_ball->mass;

		// s = 1/2 (u+v)t
		m_ball->pos += 0.5f * (float)dt * time_scale * (m_ball->temp + m_ball->vel);

		// need all the forces to reset
		m_ball->force = glm::vec3(0, 0, 0);

		// If ball reaches ramp
		if (m_ball->pos.z >= 69 && m_ball->pos.z < 76)
		{
			// Calculate velocity up the ramp with gravatational force acting on it
			float velY = sqrt((m_ball->vel.z * m_ball->vel.z) - (m_ball->mass * m_ball->mass));
			m_ball->vel.y = velY;
		}
		else if (m_ball->pos.z > 76)
		{
			m_ball->vel.y -= 1.f;
		}

		// Check for ball & side wall collision for skeeball machine
		for (int i = 0; i < 4; ++i)
		{
			if (CheckSphereCubeCollision(m_ball, m_wall[i]))
			{
				CollisionResponse(m_ball, m_wall[i]);
			}
		}

		// Check whether ball lands in the 10 zone
		if (CheckSphereCubeCollision(m_ball, m_wall[4]) && !added)
		{
			skeeballScore += 10;
			added = true;
		}

		// Check whether ball lands in the 20 zone
		if (CheckSphereCubeCollision(m_ball, m_goal[0]) && !added)
		{
			skeeballScore += 20;
			added = true;
		}

		// Check whether ball lands in the 30 zone
		if (CheckSphereCubeCollision(m_ball, m_goal[1]) && !added)
		{
			skeeballScore += 30;
			added = true;
		}

		// Check whether ball lands in the 50 zone
		if (CheckSphereCubeCollision(m_ball, m_goal[2]) && !added)
		{
			skeeballScore += 50;
			added = true;
		}

		// Check whether ball lands in the 100 zone
		if ((CheckSphereCubeCollision(m_ball, m_goal[3]) || CheckSphereCubeCollision(m_ball, m_goal[4])) && !added)
		{
			skeeballScore += 100;
			added = true;
		}

		//If ball falls until y = -10.f or has stopped due to friction
		if (m_ball->pos.y <= -10.f || m_ball->vel.z <= 0.f)
		{
			launch1 = false;
			added = false;
			m_ball->vel = glm::vec3(0.f, 0.f, 0.f);
			m_ball->pos = glm::vec3(0.f, -10.f, 35.f);
			--balls;
			arrowAngle = 180.f;
		}

		// Check if player runs out of balls
		if (balls <= 0)
		{
			gameOver = true;
		}
	}

	// Main loop for high striker machine
	if (playing2 && !highStrikerIntro)
	{
		angle += 0.4;
		timer++;

		if (angle >= 90.f)
		{
			angle = 90.f;
		}

		if (timer >= 300)
		{
			launch2 = true;
			if (KeyboardController::GetInstance()->IsKeyDown('X'))
			{
				// Reset high striker machine
				timer = 0;
				playing2 = false;
				clicks = 0;
				hit = false;
				launch2 = false;
				height = 0;
				m_puck->acc = glm::vec3(0, 0, 0);
				m_puck->vel = glm::vec3(0, 0, 0);
			}
		}

		if (launch2)
		{
			angle += 3.f;

			// need all the forces to reset
			m_puck->force = glm::vec3(0, 0, 0);

			// Calculate puck multiplier
			m_puck->multiplier = 1.0f / m_puck->mass;

			//Calculate the resulting acceleration
			// F = ma.  Hence a = F/M = F * 1/M
			m_puck->acc = m_puck->force * m_puck->multiplier;

			// set initial velocity (U)
			m_puck->temp = m_puck->vel;

			//commpute the velocity.
			// v = u + at
			m_puck->vel += (float)dt * time_scale * m_puck->acc;
			// Reulting velocity with gravity
			m_puck->vel.y -= 5.f;

			// s = 1/2 (u+v)t
			m_puck->pos += 0.5f * (float)dt * time_scale * (m_puck->temp + m_puck->vel);

			// need all the forces to reset
			m_puck->force = glm::vec3(0, 0, 0);

			// Tally max height reached by puck before it starts falling back down
			if (m_puck->vel.y >= 0)
			{
				height = (m_puck->pos.y + 20) / 9;
			}
		}
		// reset puck vel and pos when it falls back to original position
		if (m_puck->pos.y <= -20)
		{
			m_puck->vel.y = 0.f;
			m_puck->pos.y = -20;
		}
		// Limit how high puck can go (until the bell position)
		if (m_puck->pos.y >= 78)
		{
			m_puck->pos.y = 78;
			hit = true;
		}
	}

	// Check if user presses X to close the skeeball intro GUI
	if (KeyboardController::GetInstance()->IsKeyDown('X') && playing1)
	{
		skeeballIntro = false;
	}
	// Check if user presses X to close the high striker intro GUI
	else if (KeyboardController::GetInstance()->IsKeyDown('X') && playing2)
	{
		highStrikerIntro = false;
	}
	// Check if user presses X to close the skeeball end GUI
	if (KeyboardController::GetInstance()->IsKeyDown('X') && gameOver)
	{
		playing1 = false;
		skeeballOutro = false;
		// Reset skeeball machine
		balls = 9;
		skeeballScore = 0;
		gameOver = false;
		skeeballOutro = true;
	}

	// Move arrow direction
	if (playing1 && KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_LEFT) && !launch1)
	{
		arrowAngle += 1.f;
		m_ball->vel.x += 0.2f;
	}
	else if (playing1 && KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_RIGHT) && !launch1)
	{
		arrowAngle -= 1.f;
		m_ball->vel.x -= 0.2f;
	}

	// Limit arrow angle
	if (arrowAngle >= 225)
	{
		arrowAngle = 225;
	}
	else if (arrowAngle <= 135)
	{
		arrowAngle = 135;
	}

	// Limit m_ball vel x
	if (m_ball->vel.x >= 9.6)
	{
		m_ball->vel.x = 9.6;
	}
	else if (m_ball->vel.x <= -8.8)
	{
		m_ball->vel.x = -8.8;
	}

	// Check if player is inside EXIT box
	if (FPScamera.position.z >= -16.f
		&& FPScamera.position.z <= 23.f
		&& FPScamera.position.x >= -99.f
		&& FPScamera.position.x <= -78.f)
	{
		exitPrompt = true;
		if (KeyboardController::GetInstance()->IsKeyPressed('X'))
		{
			// Add code to change scene here
		}
	}
	else
	{
		exitPrompt = false;
	}

	for (int i = 0; i < 3; i++)
	{
		if (KeyboardController::GetInstance()->IsKeyDown('I'))
			light[i].position.z -= static_cast<float>(dt) * 10.f;
		if (KeyboardController::GetInstance()->IsKeyDown('K'))
			light[i].position.z += static_cast<float>(dt) * 10.f;
		if (KeyboardController::GetInstance()->IsKeyDown('J'))
			light[i].position.x -= static_cast<float>(dt) * 10.f;
		if (KeyboardController::GetInstance()->IsKeyDown('L'))
			light[i].position.x += static_cast<float>(dt) * 10.f;
		if (KeyboardController::GetInstance()->IsKeyDown('O'))
			light[i].position.y -= static_cast<float>(dt) * 10.f;
		if (KeyboardController::GetInstance()->IsKeyDown('P'))
			light[i].position.y += static_cast<float>(dt) * 10.f;
	}

	if (!playing1 && !playing2)
	{
		FPScamera.Update(dt);
	}

	HandleMouseInput();
}

void SceneArcade::RenderGUI() 
{
	// Render skeeball intro GUI
	if (skeeballIntro)
	{
		RenderMeshOnScreen(meshList[GEO_SKEEBALL_INTRO_GUI], 400, 300, 1, 1);
	}

	if (playing1)
	{
		// Render skeeball power meter GUI
		RenderMeshOnScreen(meshList[GEO_POWERMETER_GUI], 750, 120, 0.1, 0.25);

		// Render ball counter GUI
		RenderMeshOnScreen(meshList[GEO_BALL_COUNTER_GUI], 50, 50, 0.2, 0.2);
	}

	// Render high striker intro GUI
	if (highStrikerIntro)
	{
		RenderMeshOnScreen(meshList[GEO_HIGHSTRIKER_INTRO_GUI], 400, 300, 1, 1);
	}
}

void SceneArcade::Render()
{
	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Load view matrix stack and set it with camera position, target position and up direction
	viewStack.LoadIdentity();
	viewStack.LookAt(
		FPScamera.position.x, FPScamera.position.y, FPScamera.position.z,
		FPScamera.target.x, FPScamera.target.y, FPScamera.target.z,
		FPScamera.up.x, FPScamera.up.y, FPScamera.up.z);

	// Load identity matrix into the model stack
	modelStack.LoadIdentity();

	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		glm::vec3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		glm::vec3 lightDirection_cameraspace = viewStack.Top() * glm::vec4(lightDir, 0);
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, glm::value_ptr(lightDirection_cameraspace));
	}
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[0].position, 1);
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
		glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[0].spotDirection, 0);
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace));
	}
	else {
		// Calculate the light position in camera space
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[0].position, 1);
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
	}

	if (light[1].type == Light::LIGHT_DIRECTIONAL)
	{
		glm::vec3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
		glm::vec3 lightDirection_cameraspace = viewStack.Top() * glm::vec4(lightDir, 0);
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, glm::value_ptr(lightDirection_cameraspace));
	}
	else if (light[1].type == Light::LIGHT_SPOT)
	{
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[1].position, 1);
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
		glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[1].spotDirection, 0);
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace));
	}
	else {
		// Calculate the light position in camera space
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[1].position, 1);
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
	}

	if (light[2].type == Light::LIGHT_DIRECTIONAL)
	{
		glm::vec3 lightDir(light[2].position.x, light[2].position.y, light[2].position.z);
		glm::vec3 lightDirection_cameraspace = viewStack.Top() * glm::vec4(lightDir, 0);
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, glm::value_ptr(lightDirection_cameraspace));
	}
	else if (light[2].type == Light::LIGHT_SPOT)
	{
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[2].position, 1);
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
		glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[2].spotDirection, 0);
		glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace));
	}
	else {
		// Calculate the light position in camera space
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[2].position, 1);
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
	}

	// Render lights
	for (int i = 0; i < 3; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(light[i].position.x, light[i].position.y, light[i].position.z);
		modelStack.Scale(0.3f, 0.3f, 0.3f);
		RenderMesh(meshList[GEO_LIGHT], false);
		modelStack.PopMatrix();
	}

	// Render skybox
	RenderSkybox();

	// Render skeeball machine
	modelStack.PushMatrix();
	modelStack.Translate(m_skeeball->pos.x, m_skeeball->pos.y, m_skeeball->pos.z);
	modelStack.Scale(1.f, 1.f, 1.f);
	modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_SKEEBALL], enableLight);
	modelStack.PopMatrix();

	// Render ball
	if (m_ball->active)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_ball->pos.x, m_ball->pos.y, m_ball->pos.z);
		modelStack.Scale(m_ball->scale.x, m_ball->scale.y, m_ball->scale.z);
		RenderMesh(meshList[GEO_BALL], enableLight);
		modelStack.PopMatrix();
	}

	// Render aiming arrow
	if (playing1 && !launch1)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_arrow->pos.x, m_arrow->pos.y, m_arrow->pos.z);
		modelStack.Scale(0.3f, 0.5f, 0.5f);
		modelStack.Rotate(arrowAngle, 0.f, 1.f, 0.f);
		RenderMesh(meshList[GEO_ARROW], enableLight);
		modelStack.PopMatrix();
	}

	// Render high striker machine
	modelStack.PushMatrix();
	modelStack.Translate(m_highstriker->pos.x, m_highstriker->pos.y, m_highstriker->pos.z);
	modelStack.Scale(0.15f, 0.13f, 0.15f);
	modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_HIGHSTRIKER], enableLight);
	modelStack.PopMatrix();

	// Render hammer 1 (For display)
	if (!playing2)
	{
		modelStack.PushMatrix();
		modelStack.Rotate(15.f, 0.f, 0.f, 1.f);
		modelStack.Translate(55.5f, -30.f, 100.f);
		modelStack.Scale(9.f, 6.f, 9.f);
		modelStack.Rotate(-90.f, 1.f, 0.f, 0.f);
		RenderMesh(meshList[GEO_HAMMER], enableLight);
		modelStack.PopMatrix();
	}

	// Render hammer 2 (Actual moving hammer when playing)
	if (playing2)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_hammer->pos.x, m_hammer->pos.y, m_hammer->pos.z);
		modelStack.Rotate(angle, 1.f, 0.f, 0.f);
		modelStack.Rotate(-90.f, 1.f, 0.f, 0.f);
		modelStack.Scale(9.f, 6.f, 9.f);
		RenderMesh(meshList[GEO_HAMMER], enableLight);
		modelStack.PopMatrix();

		// Show click counter to player (changes colour according to number of clicks)
		if (clicks <= 15)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], to_string(clicks), Color(0, 1, 0), 50, 745, 30);
		}
		else if (clicks <= 25)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], to_string(clicks), Color(1, 1, 0), 50, 745, 30);
		}
		else
		{
			RenderTextOnScreen(meshList[GEO_TEXT], to_string(clicks), Color(1, 0, 0), 50, 745, 30);
		}
	}
	
	// Show prompt to player to play skeeball
	if (playPrompt1 && !playing1)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "[E]Play Skeeball", Color(1, 1, 1), 50, 200, 40);
	}

	if (playing1)
	{
		// Display power level (position depends on number of digits)
		string spower = to_string(trunc(power));
		int ipower = stoi(spower);
		spower = to_string(ipower);

		if (ipower < 10)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], spower, Color(1, 1, 1), 50, 765, 30);
		}
		else if (ipower < 100)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], spower, Color(1, 1, 1), 50, 750, 30);
		}
		else
		{
			RenderTextOnScreen(meshList[GEO_TEXT], spower, Color(1, 1, 1), 50, 735, 30);
		}

		// Display score (position depends on number of digits)
		if (skeeballScore < 10)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], to_string(skeeballScore), Color(1, 0, 0), 30, 412, 472);
		}
		else if (skeeballScore < 100)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], to_string(skeeballScore), Color(1, 0, 0), 30, 401, 472);
		}
		else
		{
			RenderTextOnScreen(meshList[GEO_TEXT], to_string(skeeballScore), Color(1, 0, 0), 30, 392, 472);
		}

		// Display ball counter (changes colour according to number of balls left)
		if (balls > 6)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "x" + to_string(balls), Color(0, 1, 0), 50, 110, 40);
		}
		else if (balls > 3)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "x" + to_string(balls), Color(1, 1, 0), 50, 110, 40);
		}
		else
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "x" + to_string(balls), Color(1, 0, 0), 50, 110, 40);
		}
	}

	// Display final total score onto the skeeball end GUI (position depends on number of digits)
	if (gameOver && skeeballOutro)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Final Score:", Color(1, 1, 1), 50, 275, 300);
		if (skeeballScore < 10)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], to_string(skeeballScore), Color(1, 1, 1), 45, 415, 250);
		}
		else if (skeeballScore < 100)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], to_string(skeeballScore), Color(1, 1, 1), 45, 405, 250);
		}
		else
		{
			RenderTextOnScreen(meshList[GEO_TEXT], to_string(skeeballScore), Color(1, 1, 1), 45, 390, 250);
		}
		RenderTextOnScreen(meshList[GEO_TEXT], "[X] Dismiss", Color(1, 1, 1), 47, 275, 200);
	}

	// Show prompt to player to play high striker
	if (playPrompt2 && !playing2)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "[E]Play High Striker", Color(1, 1, 1), 50, 140, 40);
	}

	// Show highest distance reached by puck after launch
	if (launch2 && playing2)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Height:" + to_string(height) + "m", Color(1, 1, 1), 50, 280, 300);
	}

	// If puck hits bell, show "DING!" on screen
	if (hit && playing2)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "DING!", Color(1, 1, 1), 50, 360, 550);
	}

	// Show prompt to exit high striker machine
	if (timer >= 300)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "[X] Dismiss", Color(1, 1, 1), 50, 300, 40);
	}

	// Show prompt to exit the scene
	if (exitPrompt)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "[X] Exit", Color(1, 1, 1), 50, 300, 40);
	}

	// Render puck
	modelStack.PushMatrix();
	modelStack.Translate(m_puck->pos.x, m_puck->pos.y, m_puck->pos.z);
	modelStack.Scale(3.f, 1.f, 1.f);
	RenderMesh(meshList[GEO_PUCK], enableLight);
	modelStack.PopMatrix();

	////Render wall
	//for (int i = 0; i < 4; ++i)
	//{
	//	modelStack.PushMatrix();
	//	modelStack.Translate(m_wall[i]->pos.x, m_wall[i]->pos.y, m_wall[i]->pos.z);
	//	modelStack.Scale(m_wall[i]->scale.x, m_wall[i]->scale.y, m_wall[i]->scale.z);
	//	RenderMesh(meshList[GEO_WALL], enableLight);
	//	modelStack.PopMatrix();
	//}

	//modelStack.PushMatrix();
	//modelStack.Translate(m_wall[4]->pos.x, m_wall[4]->pos.y, m_wall[4]->pos.z);
	//modelStack.Rotate(40.f, 1.f, 0.f, 0.f);
	//modelStack.Scale(m_wall[4]->scale.x, m_wall[4]->scale.y, m_wall[4]->scale.z);
	//RenderMesh(meshList[GEO_WALL], enableLight);
	//modelStack.PopMatrix();

	////Render cylinders
	//for (int i = 0; i < 5; ++i)
	//{
	//	modelStack.PushMatrix();
	//	modelStack.Translate(m_goal[i]->pos.x, m_goal[i]->pos.y, m_goal[i]->pos.z);
	//	modelStack.Rotate(-50.f, 1.f, 0.f, 0.f);
	//	modelStack.Scale(m_goal[i]->scale.x, m_goal[i]->scale.y, m_goal[i]->scale.z);
	//	RenderMesh(meshList[GEO_GOAL], enableLight);
	//	modelStack.PopMatrix();
	//}

	RenderGUI();
}

void SceneArcade::RenderMesh(Mesh* mesh, bool enableLight)
{
	glm::mat4 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, glm::value_ptr(modelView));

	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = glm::inverseTranspose(modelView);
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, glm::value_ptr(modelView_inverse_transpose));

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}

	mesh->Render();

	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneArcade::Exit()
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

void SceneArcade::HandleKeyPress()
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

	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_0))
	{
		// Toggle light on or off
	/*	enableLight = !enableLight;*/

		if (light[0].power <= 0.1f)
			light[0].power = 1.f;
		else
			light[0].power = 0.1f;
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_9))
	{
		// Toggle light on or off
		//enableLight = !enableLight;

		light[0].color.Set(0.5f, 0.f, 0.f);
		glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_8))
	{
		// Toggle light on or off
		//enableLight = !enableLight;

		light[0].color.Set(1.f, 1.f, 1.f);
		glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	}
}

void SceneArcade::HandleMouseInput() {

	double x, y, ww, wh;
	Application::GetCursorPos(&x,&y);
	wh = Application::GetWindowHeight();
	ww = Application::GetWindowWidth();
	glm::vec3 mousePos(x * (m_worldWidth / ww), (wh - y - 57) * (m_worldHeight / wh), 0);

	if (MouseController::GetInstance()->IsButtonDown(GLFW_MOUSE_BUTTON_LEFT) && playing1 && !skeeballIntro && !launch1 && !gameOver)
	{
		// Increase / Decrease ball's veloctiy Z based on mouse position
		power = mousePos.y;
		m_ball->vel.z = mousePos.y / 5;
		m_ball->vel.y = mousePos.y / 30;

		// Limit ball velocity z
		if (m_ball->vel.z >= 20)
		{
			m_ball->vel.z = 20;
		}
		else if (m_ball->vel.z <= 0)
		{
			m_ball->vel.z = 0;
		}
		
		// Limit ball veloctiy y
		if (m_ball->vel.y >= 3.5)
		{
			m_ball->vel.y = 3.5;
		}
		else if (m_ball->vel.y <= 0)
		{
			m_ball->vel.y = 0;
		}

		// Limit power counter
		if (power >= 100)
		{
			power = 100;
		}
		else if (power <= 0)
		{
			power = 0;
		}

		pressed = true;
	}
	// If release LMB
	else if (pressed && MouseController::GetInstance()->IsButtonUp(GLFW_MOUSE_BUTTON_LEFT) && playing1 && !skeeballIntro && !gameOver)
	{
		launch1 = true;
		pressed = false;
	}

	// Increase / Decrease angle of rotation of hammer for every LMB down
	if (!launch2 && playing2 && !highStrikerIntro)
	{
		if (!pressed && MouseController::GetInstance()->IsButtonDown(GLFW_MOUSE_BUTTON_LEFT))
		{
			angle -= 4.5f;
			pressed = true;
			m_puck->vel.y += 3.4f;
			clicks++;
		}
		else if (pressed && MouseController::GetInstance()->IsButtonUp(GLFW_MOUSE_BUTTON_LEFT))
		{
			pressed = false;
		}
	}
}

void SceneArcade::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
{
	glDisable(GL_DEPTH_TEST);
	glm::mat4 ortho = glm::ortho(0.f, 800.f, 0.f, 600.f, -1000.f, 1000.f); // dimension of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();

	// To do: Use modelStack to position GUI on screen
	modelStack.PushMatrix();
	modelStack.Translate(x, y, 0.f);
	// To do: Use modelStack to scale the GUI
	modelStack.PushMatrix();
	modelStack.Scale(sizex, sizey, 1.f);

	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneArcade::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(0.f, 5.f, -100.f);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.f, 5.f, 100.f);
	modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-100.f, 5.f, 0.f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(100.f, 5.f, 0.f);
	modelStack.Rotate(-90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.f, 30.f, 0.f);
	modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.f, -20.f, 0.f);
	modelStack.Rotate(-90.f, 1.f, 0.f, 0.f);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
}

void SceneArcade::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	// Disable back face culling
	glDisable(GL_CULL_FACE);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		glm::mat4 characterSpacing = glm::translate(
			glm::mat4(0.5f),
			glm::vec3(i * 0.5f, 0, 0));
		glm::mat4 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_CULL_FACE);
}

void SceneArcade::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glm::mat4 ortho = glm::ortho(0.f, 800.f, 0.f, 600.f, -100.f, 100.f); // dimension of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		glm::mat4 characterSpacing = glm::translate(
			glm::mat4(0.6f),
			glm::vec3(i * 0.6f, 0, 0)
		);
		glm::mat4 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

bool SceneArcade::CheckSphereCubeCollision(GameObject* sphere, GameObject* cube)
{
	// Get cube closest point to sphere center
	float cubeMinX = cube->pos.x - cube->scale.x / 2;
	float cubeMaxX = cube->pos.x + cube->scale.x / 2;
	float cubeMinY = cube->pos.y - cube->scale.y / 2;
	float cubeMaxY = cube->pos.y + cube->scale.y / 2;
	float cubeMinZ = cube->pos.z - cube->scale.z / 2;
	float cubeMaxZ = cube->pos.z + cube->scale.z / 2;

	const float x = max(cubeMinX, min(sphere->pos.x, cubeMaxX));
	const float y = max(cubeMinY, min(sphere->pos.y, cubeMaxY));
	const float z = max(cubeMinZ, min(sphere->pos.z, cubeMaxZ));

	// Find distance between closest point on cube & sphere
	const float distance = sqrt(
		(x - sphere->pos.x) * (x - sphere->pos.x) +
		(y - sphere->pos.y) * (y - sphere->pos.y) +
		(z - sphere->pos.z) * (z - sphere->pos.z)
	);

	// Check distance between closest point on cube and sphere
	if (distance <= sphere->scale.x / 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SceneArcade::CollisionResponse(GameObject* go1, GameObject* go2)
{
	go1->vel.x *= -1;
}

