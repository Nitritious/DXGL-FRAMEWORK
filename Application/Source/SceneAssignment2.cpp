#include "SceneTexture.h"
#include "GL\glew.h"

// GLM Headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

//Include GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "KeyboardController.h"
#include "LoadTGA.h"
#include "SceneSkyBox.h"
#include "SceneGUI.h"
#include "MouseController.h"
#include "SceneAssignment2.h"


SceneAssignment2::SceneAssignment2()
{
}

SceneAssignment2::~SceneAssignment2()
{
}

void SceneAssignment2::Init()
{
	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glEnable(GL_CULL_FACE);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Load the shader programs
	m_programID = LoadShaders("Shader//Texture.vertexshader",
		"Shader//Text.fragmentshader");
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

	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

	m_parameters[U_LIGHT3_TYPE] = glGetUniformLocation(m_programID, "lights[3].type");
	m_parameters[U_LIGHT3_POSITION] = glGetUniformLocation(m_programID, "lights[3].position_cameraspace");
	m_parameters[U_LIGHT3_COLOR] = glGetUniformLocation(m_programID, "lights[3].color");
	m_parameters[U_LIGHT3_POWER] = glGetUniformLocation(m_programID, "lights[3].power");
	m_parameters[U_LIGHT3_KC] = glGetUniformLocation(m_programID, "lights[3].kC");
	m_parameters[U_LIGHT3_KL] = glGetUniformLocation(m_programID, "lights[3].kL");
	m_parameters[U_LIGHT3_KQ] = glGetUniformLocation(m_programID, "lights[3].kQ");
	m_parameters[U_LIGHT3_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[3].spotDirection");
	m_parameters[U_LIGHT3_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[3].cosCutoff");
	m_parameters[U_LIGHT3_COSINNER] = glGetUniformLocation(m_programID, "lights[3].cosInner");
	m_parameters[U_LIGHT3_EXPONENT] = glGetUniformLocation(m_programID, "lights[3].exponent");

	m_parameters[U_LIGHT4_TYPE] = glGetUniformLocation(m_programID, "lights[4].type");
	m_parameters[U_LIGHT4_POSITION] = glGetUniformLocation(m_programID, "lights[4].position_cameraspace");
	m_parameters[U_LIGHT4_COLOR] = glGetUniformLocation(m_programID, "lights[4].color");
	m_parameters[U_LIGHT4_POWER] = glGetUniformLocation(m_programID, "lights[4].power");
	m_parameters[U_LIGHT4_KC] = glGetUniformLocation(m_programID, "lights[4].kC");
	m_parameters[U_LIGHT4_KL] = glGetUniformLocation(m_programID, "lights[4].kL");
	m_parameters[U_LIGHT4_KQ] = glGetUniformLocation(m_programID, "lights[4].kQ");
	m_parameters[U_LIGHT4_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[4].spotDirection");
	m_parameters[U_LIGHT4_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[4].cosCutoff");
	m_parameters[U_LIGHT4_COSINNER] = glGetUniformLocation(m_programID, "lights[4].cosInner");
	m_parameters[U_LIGHT4_EXPONENT] = glGetUniformLocation(m_programID, "lights[4].exponent");

	m_parameters[U_LIGHT5_TYPE] = glGetUniformLocation(m_programID, "lights[5].type");
	m_parameters[U_LIGHT5_POSITION] = glGetUniformLocation(m_programID, "lights[5].position_cameraspace");
	m_parameters[U_LIGHT5_COLOR] = glGetUniformLocation(m_programID, "lights[5].color");
	m_parameters[U_LIGHT5_POWER] = glGetUniformLocation(m_programID, "lights[5].power");
	m_parameters[U_LIGHT5_KC] = glGetUniformLocation(m_programID, "lights[5].kC");
	m_parameters[U_LIGHT5_KL] = glGetUniformLocation(m_programID, "lights[5].kL");
	m_parameters[U_LIGHT5_KQ] = glGetUniformLocation(m_programID, "lights[5].kQ");
	m_parameters[U_LIGHT5_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[5].spotDirection");
	m_parameters[U_LIGHT5_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[5].cosCutoff");
	m_parameters[U_LIGHT5_COSINNER] = glGetUniformLocation(m_programID, "lights[5].cosInner");
	m_parameters[U_LIGHT5_EXPONENT] = glGetUniformLocation(m_programID, "lights[5].exponent");

	m_parameters[U_LIGHT6_TYPE] = glGetUniformLocation(m_programID, "lights[6].type");
	m_parameters[U_LIGHT6_POSITION] = glGetUniformLocation(m_programID, "lights[6].position_cameraspace");
	m_parameters[U_LIGHT6_COLOR] = glGetUniformLocation(m_programID, "lights[6].color");
	m_parameters[U_LIGHT6_POWER] = glGetUniformLocation(m_programID, "lights[6].power");
	m_parameters[U_LIGHT6_KC] = glGetUniformLocation(m_programID, "lights[6].kC");
	m_parameters[U_LIGHT6_KL] = glGetUniformLocation(m_programID, "lights[6].kL");
	m_parameters[U_LIGHT6_KQ] = glGetUniformLocation(m_programID, "lights[6].kQ");
	m_parameters[U_LIGHT6_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[6].spotDirection");
	m_parameters[U_LIGHT6_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[6].cosCutoff");
	m_parameters[U_LIGHT6_COSINNER] = glGetUniformLocation(m_programID, "lights[6].cosInner");
	m_parameters[U_LIGHT6_EXPONENT] = glGetUniformLocation(m_programID, "lights[6].exponent");

	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID,
		"colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");


	// Initialise camera properties
	PanCamera = 0;
	camera.Init(glm::vec3(playerposition.x, 1.2f, playerposition.z + 0.1f), glm::vec3(playerposition.x, playerposition.y, playerposition.z), glm::vec3(0, 1, 0));
	camera2.Init(glm::vec3(1, 1.2f, 1), glm::vec3(playerposition.x, playerposition.y, playerposition.z), glm::vec3(0, 1, 0));
	playerdirection = glm::normalize(camera.target - camera.position);
	//playerdirection = { 0, 0, -1 };

	playerposition = glm::vec3(0, 0, 5.5f);
	// Init VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("Axes", 10000.f, 10000.f, 10000.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sun", Color(1.f, 1.f, 1.f), 1.f, 16, 16);
	meshList[GEO_TEXTURECUBE] = MeshBuilder::GenerateCube("Cube", Color(0.5f, 0.5f, 0.5f), 1.f);
	meshList[GEO_TEXTURECUBE]->textureID = LoadTGA("Image//dirt.tga");
	/*meshList[GEO_PLANE] = MeshBuilder::GenerateQuad("Plane", Color(1.f, 1.f, 1.f), 10.f);
	meshList[GEO_PLANE]->textureID = LoadTGA("Image//color.tga");*/
	meshList[GEO_SPHERE_WHITE] = MeshBuilder::GenerateSphere("Sphere", Color(1.f, 1.f, 1.f), 1.f, 16, 16);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("Cube", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_PLAIN] = MeshBuilder::GenerateCube("Cube", Color(0.f, 1.f, 0.f), 1.f);
	meshList[GEO_CYLINDER] = MeshBuilder::GenerateCylinder("Cylinder", Color(0.5f, 0.5f, 0.5f), 1.f, 30.f, 1.f);
	meshList[GEO_CYLINDER_WHITE] = MeshBuilder::GenerateCylinder("Cylinder", Color(1.f, 1.f, 1.f), 1.f, 30.f, 1.f);
	meshList[GEO_HEMISPHERE] = MeshBuilder::GenerateHemisphere("Hemisphere", Color(0.5f, 0.5f, 0.5f), 1.f, 100.f, 1.f);
	meshList[GEO_HORN] = MeshBuilder::GenerateHalfTorus("Torus", Color(1.f, 1.f, 1.f), 1.f, 5.f, 12, 12);
	meshList[GEO_TOPHORN] = MeshBuilder::GenerateCone("Cone", Color(1.f, 1.f, 1.f), 30.f, 0.5f, 1.f);
	meshList[GEO_FEET] = MeshBuilder::GenerateCone("Cone", Color(0.f, 0.f, 0.f), 30.f, 0.5f, 1.f);

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("Plane", Color(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//Forest_Left.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("Plane", Color(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//Forest_Right.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("Plane", Color(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//Forest_Bottom.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("Plane", Color(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//Forest_Top.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("Plane", Color(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//Forest_Back.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("Plane", Color(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//Forest_Front.tga");
	meshList[GEO_GUI] = MeshBuilder::GenerateQuad("Plane", Color(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_GUI]->textureID = LoadTGA("Image//color.tga");
	// 16 x 16 is the number of columns and rows for the text
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_FENCE] = MeshBuilder::GenerateOBJMTL("Fence", "OBJ//Wood_Fence.obj",
		"OBJ//Wood_Fence.mtl"); //Woof
	meshList[GEO_FENCE]->textureID = LoadTGA("Image//Wood_Fence.tga");

	meshList[GEO_TARGET] = MeshBuilder::GenerateOBJMTL("Target", "OBJ//Target.obj",
		"OBJ//Target.mtl"); //cottage_diffuse
	meshList[GEO_TARGET]->textureID = LoadTGA("Image//Target.tga");

	meshList[GEO_TABLE] = MeshBuilder::GenerateOBJMTL("Table", "OBJ//Table.obj",
		"OBJ//Table.mtl"); //cottage_diffuse
	meshList[GEO_TABLE]->textureID = LoadTGA("Image//Table.tga");

	meshList[GEO_BOW] = MeshBuilder::GenerateOBJ("Bow", "OBJ//Bow.obj"); //cottage_diffuse
	meshList[GEO_BOW]->textureID = LoadTGA("Image//Bow.tga");

	meshList[GEO_BUTTON] = MeshBuilder::GenerateOBJ("Bow", "OBJ//button.obj"); //cottage_diffuse
	meshList[GEO_BUTTON]->textureID = LoadTGA("Image//button.tga");

	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateQuad("Crosshair", Color(0.f, 0.f, 0.f), 100.f);
	meshList[GEO_CROSSHAIR]->textureID = LoadTGA("Image//crosshair.tga");

	meshList[GEO_BOWSHOOT] = MeshBuilder::GenerateQuad("ShootEffect", Color(0.f, 0.f, 0.f), 100.f);
	meshList[GEO_BOWSHOOT]->textureID = LoadTGA("Image//BowShootEffect.tga");

	meshList[GEO_ROCK] = MeshBuilder::GenerateOBJ("Rock", "OBJ//rock.obj"); //cottage_diffuse
	meshList[GEO_ROCK]->textureID = LoadTGA("Image//rock.tga");

	meshList[GEO_PLANT] = MeshBuilder::GenerateOBJMTL("Plant", "OBJ//plant.obj",
		"OBJ//plant.mtl"); //cottage_diffuse
	meshList[GEO_PLANT]->textureID = LoadTGA("Image//plant.tga");

	meshList[GEO_PAPER] = MeshBuilder::GenerateOBJMTL("Plant", "OBJ//Paper.obj",
		"OBJ//paper.mtl"); //cottage_diffuse
	meshList[GEO_PAPER]->textureID = LoadTGA("Image//Paper.tga");

	meshList[GEO_AMONGUS] = MeshBuilder::GenerateOBJ("Amongus", "OBJ//Amongus.obj"); //cottage_diffuse
	meshList[GEO_AMONGUS]->textureID = LoadTGA("Image//Amongus.tga");

	meshList[GEO_AMONGUS2] = MeshBuilder::GenerateOBJ("Amongus", "OBJ//Amongus.obj"); //cottage_diffuse
	meshList[GEO_AMONGUS2]->textureID = LoadTGA("Image//Amongusyellow.tga");

	meshList[GEO_BOOTH] = MeshBuilder::GenerateOBJMTL("Booth", "OBJ//booth.obj", "OBJ//booth.mtl"); //cottage_diffuse
	meshList[GEO_BOOTH]->textureID = LoadTGA("Image//booth.tga");
	
	//meshList[GEO_SPHERE_BLUE] = MeshBuilder::GenerateSphere("Earth", Color(0.4f, 0.2f, 0.8f), 1.f, 12, 12);
	//meshList[GEO_SPHERE_GREY] = MeshBuilder::GenerateSphere("Moon", Color(0.5f, 0.5f, 0.5f), 1.f, 4, 4);

	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);

	/*moonRotation = 0.0f;
	earthRotation = 0.0f;
	sunRotation = 0.0f;

	currAnim = ANIM_SUN;*/

	glUniform1i(m_parameters[U_NUMLIGHTS], NUM_LIGHTS);

	light[0].position = glm::vec3(0, 5, 0);
	light[0].color.Set(1, 1, 1);
	light[0].type = Light::LIGHT_POINT;
	light[0].power = 0.4f;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = 45.f;
	light[0].cosInner = 30.f;
	light[0].exponent = 3.f;
	light[0].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	light[1].position = glm::vec3(-5, 5, -5);
	light[1].color.Set(1, 1, 1);
	light[1].type = Light::LIGHT_SPOT;
	light[1].power = 0.4f;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = 15.f;
	light[1].cosInner = 30.f;
	light[1].exponent = 3.f;
	light[1].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	light[2].position = glm::vec3(-3, 5, -8);
	light[2].color.Set(1, 1, 1);
	light[2].type = Light::LIGHT_SPOT;
	light[2].power = 0.4f;
	light[2].kC = 1.f;
	light[2].kL = 0.01f;
	light[2].kQ = 0.001f;
	light[2].cosCutoff = 15.f;
	light[2].cosInner = 30.f;
	light[2].exponent = 3.f;
	light[2].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	light[3].position = glm::vec3(-1, 5, -5);
	light[3].color.Set(1, 1, 1);
	light[3].type = Light::LIGHT_SPOT;
	light[3].power = 0.4f;
	light[3].kC = 1.f;
	light[3].kL = 0.01f;
	light[3].kQ = 0.001f;
	light[3].cosCutoff = 15.f;
	light[3].cosInner = 30.f;
	light[3].exponent = 3.f;
	light[3].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	light[4].position = glm::vec3(1, 5, -8);
	light[4].color.Set(1, 1, 1);
	light[4].type = Light::LIGHT_SPOT;
	light[4].power = 0.4f;
	light[4].kC = 1.f;
	light[4].kL = 0.01f;
	light[4].kQ = 0.001f;
	light[4].cosCutoff = 15.f;
	light[4].cosInner = 30.f;
	light[4].exponent = 3.f;
	light[4].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	light[5].position = glm::vec3(3, 5, -5);
	light[5].color.Set(1, 1, 1);
	light[5].type = Light::LIGHT_SPOT;
	light[5].power = 0.4f;
	light[5].kC = 1.f;
	light[5].kL = 0.01f;
	light[5].kQ = 0.001f;
	light[5].cosCutoff = 15.f;
	light[5].cosInner = 30.f;
	light[5].exponent = 3.f;
	light[5].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	light[6].position = glm::vec3(5, 5, -8);
	light[6].color.Set(1, 1, 1);
	light[6].type = Light::LIGHT_SPOT;
	light[6].power = 0.1f;
	light[6].kC = 1.f;
	light[6].kL = 0.01f;
	light[6].kQ = 0.001f;
	light[6].cosCutoff = 15.f;
	light[6].cosInner = 30.f;
	light[6].exponent = 3.f;
	light[6].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], cosf(glm::radians<float>(light[0].cosCutoff)));
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], cosf(glm::radians<float>(light[0].cosInner)));
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);
	glUniform1i(m_parameters[U_NUMLIGHTS], NUM_LIGHTS);

    glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], cosf(glm::radians<float>(light[1].cosCutoff)));
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], cosf(glm::radians<float>(light[1].cosInner)));
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);
	glUniform1i(m_parameters[U_NUMLIGHTS], NUM_LIGHTS);

	glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
	glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
	glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
	glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
	glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
	glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], cosf(glm::radians<float>(light[2].cosCutoff)));
	glUniform1f(m_parameters[U_LIGHT2_COSINNER], cosf(glm::radians<float>(light[2].cosInner)));
	glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);
	glUniform1i(m_parameters[U_NUMLIGHTS], NUM_LIGHTS);

	glUniform3fv(m_parameters[U_LIGHT3_COLOR], 1, &light[3].color.r);
	glUniform1i(m_parameters[U_LIGHT3_TYPE], light[3].type);
	glUniform1f(m_parameters[U_LIGHT3_POWER], light[3].power);
	glUniform1f(m_parameters[U_LIGHT3_KC], light[3].kC);
	glUniform1f(m_parameters[U_LIGHT3_KL], light[3].kL);
	glUniform1f(m_parameters[U_LIGHT3_KQ], light[3].kQ);
	glUniform1f(m_parameters[U_LIGHT3_COSCUTOFF], cosf(glm::radians<float>(light[3].cosCutoff)));
	glUniform1f(m_parameters[U_LIGHT3_COSINNER], cosf(glm::radians<float>(light[3].cosInner)));
	glUniform1f(m_parameters[U_LIGHT3_EXPONENT], light[3].exponent);
	glUniform1i(m_parameters[U_NUMLIGHTS], NUM_LIGHTS);

	glUniform3fv(m_parameters[U_LIGHT4_COLOR], 1, &light[4].color.r);
	glUniform1i(m_parameters[U_LIGHT4_TYPE], light[4].type);
	glUniform1f(m_parameters[U_LIGHT4_POWER], light[4].power);
	glUniform1f(m_parameters[U_LIGHT4_KC], light[4].kC);
	glUniform1f(m_parameters[U_LIGHT4_KL], light[4].kL);
	glUniform1f(m_parameters[U_LIGHT4_KQ], light[4].kQ);
	glUniform1f(m_parameters[U_LIGHT4_COSCUTOFF], cosf(glm::radians<float>(light[4].cosCutoff)));
	glUniform1f(m_parameters[U_LIGHT4_COSINNER], cosf(glm::radians<float>(light[4].cosInner)));
	glUniform1f(m_parameters[U_LIGHT4_EXPONENT], light[4].exponent);
	glUniform1i(m_parameters[U_NUMLIGHTS], NUM_LIGHTS);

	glUniform3fv(m_parameters[U_LIGHT5_COLOR], 1, &light[5].color.r);
	glUniform1i(m_parameters[U_LIGHT5_TYPE], light[5].type);
	glUniform1f(m_parameters[U_LIGHT5_POWER], light[5].power);
	glUniform1f(m_parameters[U_LIGHT5_KC], light[5].kC);
	glUniform1f(m_parameters[U_LIGHT5_KL], light[5].kL);
	glUniform1f(m_parameters[U_LIGHT5_KQ], light[5].kQ);
	glUniform1f(m_parameters[U_LIGHT5_COSCUTOFF], cosf(glm::radians<float>(light[5].cosCutoff)));
	glUniform1f(m_parameters[U_LIGHT5_COSINNER], cosf(glm::radians<float>(light[5].cosInner)));
	glUniform1f(m_parameters[U_LIGHT5_EXPONENT], light[5].exponent);
	glUniform1i(m_parameters[U_NUMLIGHTS], NUM_LIGHTS);

	glUniform3fv(m_parameters[U_LIGHT5_COLOR], 1, &light[5].color.r);
	glUniform1i(m_parameters[U_LIGHT5_TYPE], light[5].type);
	glUniform1f(m_parameters[U_LIGHT5_POWER], light[5].power);
	glUniform1f(m_parameters[U_LIGHT5_KC], light[5].kC);
	glUniform1f(m_parameters[U_LIGHT5_KL], light[5].kL);
	glUniform1f(m_parameters[U_LIGHT5_KQ], light[5].kQ);
	glUniform1f(m_parameters[U_LIGHT5_COSCUTOFF], cosf(glm::radians<float>(light[5].cosCutoff)));
	glUniform1f(m_parameters[U_LIGHT5_COSINNER], cosf(glm::radians<float>(light[5].cosInner)));
	glUniform1f(m_parameters[U_LIGHT5_EXPONENT], light[5].exponent);
	glUniform1i(m_parameters[U_NUMLIGHTS], NUM_LIGHTS);

	glUniform3fv(m_parameters[U_LIGHT6_COLOR], 1, &light[6].color.r);
	glUniform1i(m_parameters[U_LIGHT6_TYPE], light[6].type);
	glUniform1f(m_parameters[U_LIGHT6_POWER], light[6].power);
	glUniform1f(m_parameters[U_LIGHT6_KC], light[6].kC);
	glUniform1f(m_parameters[U_LIGHT6_KL], light[6].kL);
	glUniform1f(m_parameters[U_LIGHT6_KQ], light[6].kQ);
	glUniform1f(m_parameters[U_LIGHT6_COSCUTOFF], cosf(glm::radians<float>(light[6].cosCutoff)));
	glUniform1f(m_parameters[U_LIGHT6_COSINNER], cosf(glm::radians<float>(light[6].cosInner)));
	glUniform1f(m_parameters[U_LIGHT6_EXPONENT], light[6].exponent);
	glUniform1i(m_parameters[U_NUMLIGHTS], NUM_LIGHTS);

	enableLight = true;

	Cameraswitch = false;
	BowEquip = false;
	TargetHit1 = false;
	TargetHit2 = false;
	TargetHit3 = false;
	TargetHit4 = false;
	TargetHit5 = false;
	TargetHit6 = false;
}

void SceneAssignment2::Update(double dt)
{
	float temp = 1.f / dt;
	fps = glm::round(temp * 100.f) / 100.f;

	HandleKeyPress(dt);
	HandleMouseInput();

	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_1))
	{
		if (KeyboardController::GetInstance()->IsKeyDown('I'))
			light[0].position.z -= static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('K'))
			light[0].position.z += static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('J'))
			light[0].position.x -= static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('L'))
			light[0].position.x += static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('O'))
			light[0].position.y -= static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('P'))
			light[0].position.y += static_cast<float>(dt) * 5.f;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_2))
	{
		if (KeyboardController::GetInstance()->IsKeyDown('I'))
			light[1].position.z -= static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('K'))
			light[1].position.z += static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('J'))
			light[1].position.x -= static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('L'))
			light[1].position.x += static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('O'))
			light[1].position.y -= static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('P'))
			light[1].position.y += static_cast<float>(dt) * 5.f;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_5))
	{
		if (KeyboardController::GetInstance()->IsKeyDown('I'))
			light[2].position.z -= static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('K'))
			light[2].position.z += static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('J'))
			light[2].position.x -= static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('L'))
			light[2].position.x += static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('O'))
			light[2].position.y -= static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('P'))
			light[2].position.y += static_cast<float>(dt) * 5.f;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_6))
	{
		if (KeyboardController::GetInstance()->IsKeyDown('I'))
			light[3].position.z -= static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('K'))
			light[3].position.z += static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('J'))
			light[3].position.x -= static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('L'))
			light[3].position.x += static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('O'))
			light[3].position.y -= static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('P'))
			light[3].position.y += static_cast<float>(dt) * 5.f;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_7))
	{
		if (KeyboardController::GetInstance()->IsKeyDown('I'))
			light[4].position.z -= static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('K'))
			light[4].position.z += static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('J'))
			light[4].position.x -= static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('L'))
			light[4].position.x += static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('O'))
			light[4].position.y -= static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('P'))
			light[4].position.y += static_cast<float>(dt) * 5.f;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_T))
	{
		if (KeyboardController::GetInstance()->IsKeyDown('I'))
			light[5].position.z -= static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('K'))
			light[5].position.z += static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('J'))
			light[5].position.x -= static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('L'))
			light[5].position.x += static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('O'))
			light[5].position.y -= static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('P'))
			light[5].position.y += static_cast<float>(dt) * 5.f;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_Y))
	{
		if (KeyboardController::GetInstance()->IsKeyDown('I'))
			light[6].position.z -= static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('K'))
			light[6].position.z += static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('J'))
			light[6].position.x -= static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('L'))
			light[6].position.x += static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('O'))
			light[6].position.y -= static_cast<float>(dt) * 5.f;
		if (KeyboardController::GetInstance()->IsKeyDown('P'))
			light[6].position.y += static_cast<float>(dt) * 5.f;
	}

	camera.Update(dt);

	if (Cameraswitch == true)
	{
		camera2.Update(dt);
		camera2.position.x = cos(PanCamera) * 1 + playerposition.x;
		camera2.position.z = sin(PanCamera) * 1 + playerposition.z;
		camera2.target = glm::vec3(playerposition.x,playerposition.y + 0.5,playerposition.z);
	}

	//offset.x = cos(rotate((0,1,0))) + playerposition.x

	static const float ROTATE_SPEED = 100.0f;
	double deltaX = MouseController::GetInstance()->GetMouseDeltaX();
	//playerrotation += -deltaX * ROTATE_SPEED * 0.1 * static_cast<float>(dt);

	if (camera.position.x > -1.f && camera.position.x < 1.f && camera.position.z >= -1.f && camera.position.z <= 1.f && !BowEquip)
	{
		interaction = "Press 'E' to pick up.";
		if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_E))
		{
			BowEquip = true;
		}
	}
	else if (camera.position.x > -1.f && camera.position.x < 1.f && camera.position.z >= -1.f && camera.position.z <= 1.f && BowEquip)
	{
		interaction = "Press 'SPACE' to reset";
		if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_SPACE))
		{
			TargetHit1 = false;
			TargetHit2 = false;
			TargetHit3 = false;
			TargetHit4 = false;
			TargetHit5 = false;
			TargetHit6 = false;
		}
	}
	else if (playerposition.x > 0.8f && playerposition.x < 3.4f && playerposition.z >= 1.2f && playerposition.z <= 3.0f)
	{
		interaction = "Hold 'Q' to Talk";
		if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_Q))
		{
			interaction = "";
			NPCinteraction = "Hello! Pick up the Bow";
			NPCinteraction2 = "And Start Shooting!";
		}
		else if (KeyboardController::GetInstance()->IsKeyUp(GLFW_KEY_Q))
		{
			interaction = "Hold 'Q' to Talk";
			NPCinteraction = "";
			NPCinteraction2 = "";
		}
	}
	else
	{
		interaction = "";
		NPCinteraction = "";
		NPCinteraction2 = "";
	}
}

void SceneAssignment2::Render()
{
	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Load view matrix stack and set it with camera position, target position and up direction
	viewStack.LoadIdentity();
	//viewStack.Translate(2,2,2);
	/*viewStack.LookAt(
		camera.position.x  + offset.x, camera.position.y + offset.y, camera.position.z + offset.z,
		camera.target.x + offset.x, camera.target.y + offset.y, camera.target.z + offset.z,
		camera.up.x, camera.up.y, camera.up.z
	);*/
	if (Cameraswitch == false)
	{
		viewStack.LookAt(
			camera.position.x, camera.position.y, camera.position.z,
			camera.target.x, camera.target.y, camera.target.z,
			camera.up.x, camera.up.y, camera.up.z
		);
	}
	else {
		viewStack.LookAt(
			camera2.position.x, camera2.position.y, camera2.position.z,
			camera2.target.x, camera2.target.y, camera2.target.z,
			camera2.up.x, camera2.up.y, camera2.up.z
		);
	}
	//viewStack.LoadMatrix(glm::inverse(viewStack.Top()));
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

	if (light[3].type == Light::LIGHT_DIRECTIONAL)
	{
		glm::vec3 lightDir(light[3].position.x, light[3].position.y, light[3].position.z);
		glm::vec3 lightDirection_cameraspace = viewStack.Top() * glm::vec4(lightDir, 0);
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, glm::value_ptr(lightDirection_cameraspace));
	}
	else if (light[3].type == Light::LIGHT_SPOT)
	{
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[3].position, 1);
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
		glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[3].spotDirection, 0);
		glUniform3fv(m_parameters[U_LIGHT3_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace));
	}
	else {
		// Calculate the light position in camera space
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[3].position, 1);
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
	}

	if (light[4].type == Light::LIGHT_DIRECTIONAL)
	{
		glm::vec3 lightDir(light[4].position.x, light[4].position.y, light[4].position.z);
		glm::vec3 lightDirection_cameraspace = viewStack.Top() * glm::vec4(lightDir, 0);
		glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, glm::value_ptr(lightDirection_cameraspace));
	}
	else if (light[4].type == Light::LIGHT_SPOT)
	{
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[4].position, 1);
		glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
		glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[4].spotDirection, 0);
		glUniform3fv(m_parameters[U_LIGHT4_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace));
	}
	else {
		// Calculate the light position in camera space
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[4].position, 1);
		glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
	}

	if (light[5].type == Light::LIGHT_DIRECTIONAL)
	{
		glm::vec3 lightDir(light[5].position.x, light[5].position.y, light[5].position.z);
		glm::vec3 lightDirection_cameraspace = viewStack.Top() * glm::vec4(lightDir, 0);
		glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, glm::value_ptr(lightDirection_cameraspace));
	}
	else if (light[5].type == Light::LIGHT_SPOT)
	{
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[5].position, 1);
		glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
		glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[5].spotDirection, 0);
		glUniform3fv(m_parameters[U_LIGHT5_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace));
	}
	else {
		// Calculate the light position in camera space
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[5].position, 1);
		glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
	}

	if (light[6].type == Light::LIGHT_DIRECTIONAL)
	{
		glm::vec3 lightDir(light[6].position.x, light[6].position.y, light[6].position.z);
		glm::vec3 lightDirection_cameraspace = viewStack.Top() * glm::vec4(lightDir, 0);
		glUniform3fv(m_parameters[U_LIGHT6_POSITION], 1, glm::value_ptr(lightDirection_cameraspace));
	}
	else if (light[6].type == Light::LIGHT_SPOT)
	{
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[6].position, 1);
		glUniform3fv(m_parameters[U_LIGHT6_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
		glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[6].spotDirection, 0);
		glUniform3fv(m_parameters[U_LIGHT6_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace));
	}
	else {
		// Calculate the light position in camera space
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[0].position, 1);
		glUniform3fv(m_parameters[U_LIGHT6_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
	}

	modelStack.PushMatrix();
	// Render objects
	RenderMesh(meshList[GEO_AXES], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	// Render light
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	modelStack.Scale(0.1f, 0.1f, 0.1f);
	RenderMesh(meshList[GEO_SPHERE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	// Render light
	modelStack.Translate(light[1].position.x, light[1].position.y, light[1].position.z);
	modelStack.Scale(0.1f, 0.1f, 0.1f);
	RenderMesh(meshList[GEO_SPHERE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	// Render light
	modelStack.Translate(light[2].position.x, light[2].position.y, light[2].position.z);
	modelStack.Scale(0.1f, 0.1f, 0.1f);
	RenderMesh(meshList[GEO_SPHERE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	// Render light
	modelStack.Translate(light[3].position.x, light[3].position.y, light[3].position.z);
	modelStack.Scale(0.1f, 0.1f, 0.1f);
	RenderMesh(meshList[GEO_SPHERE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	// Render light
	modelStack.Translate(light[4].position.x, light[4].position.y, light[4].position.z);
	modelStack.Scale(0.1f, 0.1f, 0.1f);
	RenderMesh(meshList[GEO_SPHERE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	// Render light
	modelStack.Translate(light[5].position.x, light[5].position.y, light[5].position.z);
	modelStack.Scale(0.1f, 0.1f, 0.1f);
	RenderMesh(meshList[GEO_SPHERE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	// Render light
	modelStack.Translate(light[6].position.x, light[6].position.y, light[6].position.z);
	modelStack.Scale(0.1f, 0.1f, 0.1f);
	RenderMesh(meshList[GEO_SPHERE], false);
	modelStack.PopMatrix();


	RenderSkyBox();

	modelStack.PushMatrix();
	modelStack.Translate(0, -1, 0);
	modelStack.Scale(25, 2, 25);
	meshList[GEO_TEXTURECUBE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TEXTURECUBE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TEXTURECUBE]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
	meshList[GEO_TEXTURECUBE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_TEXTURECUBE], enableLight);
	modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(0, 0, 0);
	//modelStack.Scale(0.2, 0.2, 0.2);
	//meshList[GEO_BOOTH]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	//meshList[GEO_BOOTH]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	//meshList[GEO_BOOTH]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
	//meshList[GEO_BOOTH]->material.kShininess = 1.0f;
	//RenderMesh(meshList[GEO_BOOTH], enableLight);
	//modelStack.PopMatrix();
	
	//Fence
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -10);
	modelStack.Scale(0.02f, 0.02f, 0.02f);
	{
		modelStack.PushMatrix();
		modelStack.Translate(55, 0, 0);
		{
			modelStack.PushMatrix();
			modelStack.Translate(55, 0, 0);
			{
				modelStack.PushMatrix();
				modelStack.Translate(200, 0, 200);
				{
					modelStack.PushMatrix();
					modelStack.Translate(0, 0, 170); 
					{
						modelStack.PushMatrix();
						modelStack.Translate(0, 0, 170);
						modelStack.Rotate(90, 0, 2, 0);
						modelStack.Rotate(90, -1, 0, 0);
						meshList[GEO_FENCE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
						meshList[GEO_FENCE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
						meshList[GEO_FENCE]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
						meshList[GEO_FENCE]->material.kShininess = 1.0f;
						RenderMesh(meshList[GEO_FENCE], enableLight);
						modelStack.PopMatrix();
					}
					modelStack.Rotate(90, 0, 2, 0);
					modelStack.Rotate(90, -1, 0, 0);
					meshList[GEO_FENCE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
					meshList[GEO_FENCE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
					meshList[GEO_FENCE]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
					meshList[GEO_FENCE]->material.kShininess = 1.0f;
					RenderMesh(meshList[GEO_FENCE], enableLight);
					modelStack.PopMatrix();
				}
				modelStack.Rotate(90, 0, 2, 0);
				modelStack.Rotate(90, -1, 0, 0);
				meshList[GEO_FENCE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
				meshList[GEO_FENCE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
				meshList[GEO_FENCE]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
				meshList[GEO_FENCE]->material.kShininess = 1.0f;
				RenderMesh(meshList[GEO_FENCE], enableLight);
				modelStack.PopMatrix();
			}
			modelStack.Rotate(90, -1, 0, 0);
			meshList[GEO_FENCE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
			meshList[GEO_FENCE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
			meshList[GEO_FENCE]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
			meshList[GEO_FENCE]->material.kShininess = 1.0f;
			RenderMesh(meshList[GEO_FENCE], enableLight);
			modelStack.PopMatrix();
			
		}
		modelStack.Rotate(90, -1, 0, 0);
		meshList[GEO_FENCE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_FENCE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_FENCE]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_FENCE]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_FENCE], enableLight);
		modelStack.PopMatrix();
	}
	modelStack.Rotate(90, -1, 0, 0);
	meshList[GEO_FENCE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_FENCE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_FENCE]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
	meshList[GEO_FENCE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_FENCE], enableLight);
	modelStack.PopMatrix();

	//Fence
	modelStack.PushMatrix();
	modelStack.Translate(-4.3, 0, -10);
	modelStack.Scale(0.02f, 0.02f, 0.02f);
	{
		modelStack.PushMatrix();
		modelStack.Translate(-55, 0,0);
		{
			modelStack.PushMatrix();
			modelStack.Translate(-50, 0, 230);
			{
				modelStack.PushMatrix();
				modelStack.Translate(0, 0, 270);
				modelStack.Rotate(90, 0, 2, 0);
				modelStack.Rotate(90, -1, 0, 0);
				meshList[GEO_FENCE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
				meshList[GEO_FENCE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
				meshList[GEO_FENCE]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
				meshList[GEO_FENCE]->material.kShininess = 1.0f;
				RenderMesh(meshList[GEO_FENCE], enableLight);
				modelStack.PopMatrix();
			}
			modelStack.Rotate(90, 0, 2, 0);
			modelStack.Rotate(90, -1, 0, 0);
			meshList[GEO_FENCE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
			meshList[GEO_FENCE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
			meshList[GEO_FENCE]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
			meshList[GEO_FENCE]->material.kShininess = 1.0f;
			RenderMesh(meshList[GEO_FENCE], enableLight);
			modelStack.PopMatrix();
		}
		modelStack.Rotate(90, -1, 0, 0);
		meshList[GEO_FENCE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_FENCE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_FENCE]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_FENCE]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_FENCE], enableLight);
		modelStack.PopMatrix();
	}
	modelStack.Rotate(90, -1, 0, 0);
	meshList[GEO_FENCE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_FENCE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_FENCE]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
	meshList[GEO_FENCE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_FENCE], enableLight);
	modelStack.PopMatrix();

	//Table
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.5f, 0);
	modelStack.Scale(0.004f, 0.003f, 0.003f);
	//modelStack.Rotate(90, -1, 0, 0);
	meshList[GEO_TABLE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TABLE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TABLE]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
	meshList[GEO_TABLE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_TABLE], enableLight);
	modelStack.PopMatrix();

	//Bow
	if (!BowEquip)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0.65, 0);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Scale(0.008f, 0.008f, 0.008f);
		//modelStack.Rotate(90, -1, 0, 0);
		meshList[GEO_BOW]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_BOW]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_BOW]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_BOW]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_BOW], enableLight);
		modelStack.PopMatrix();
	}
	if (BowEquip)
	{
		modelStack.PushMatrix();
		modelStack.Translate(playerposition.x, 0, playerposition.z);
		modelStack.Translate(-0.65f, 0.5f, -0.4f);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Rotate(90, 0, 0, -1);
		{
			modelStack.PushMatrix();
			modelStack.Translate(0, 0.65f, 0);
			modelStack.Scale(0.008f, 0.008f, 0.008f);
			meshList[GEO_BOW]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
			meshList[GEO_BOW]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
			meshList[GEO_BOW]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
			meshList[GEO_BOW]->material.kShininess = 1.0f;
			RenderMesh(meshList[GEO_BOW], enableLight);
			modelStack.PopMatrix();

		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0.65f, 0);
		modelStack.Scale(0.1f, 0.1f, 0.1f);
		//modelStack.Rotate(90, -1, 0, 0);
		meshList[GEO_BUTTON]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_BUTTON]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_BUTTON]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_BUTTON]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_BUTTON], enableLight);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(-0.5f, 0.63f, 0);
	modelStack.Scale(0.023f, 0.023f, 0.023f);
	modelStack.Rotate(180, 0, 1, 0);
	//modelStack.Rotate(90, -1, 0, 0);
	meshList[GEO_PAPER]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PAPER]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PAPER]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
	meshList[GEO_PAPER]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_PAPER], enableLight);
	modelStack.PopMatrix();

	//Rock
	modelStack.PushMatrix();
	modelStack.Translate(7, 0, 2.5f);
	modelStack.Scale(0.3f, 0.3f, 0.3f);
	meshList[GEO_ROCK]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_ROCK]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_ROCK]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
	meshList[GEO_ROCK]->material.kShininess = 5.f;
	RenderMesh(meshList[GEO_ROCK], enableLight);
	modelStack.PopMatrix();

	//Plant
	modelStack.PushMatrix();
	modelStack.Translate(-6, 0, 2.5f);
	modelStack.Scale(1.f, 1.f, 1.f);
	meshList[GEO_PLANT]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PLANT]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PLANT]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
	meshList[GEO_PLANT]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_PLANT], enableLight);
	modelStack.PopMatrix();

	if (!TargetHit1)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-5, 0, -5);
		modelStack.Scale(0.015f, 0.015f, 0.015f);
		modelStack.Rotate(90, -1, 0, 0);
		meshList[GEO_TARGET]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_TARGET]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_TARGET], enableLight);
		modelStack.PopMatrix();
	}
	if (TargetHit1) 
	{
		modelStack.PushMatrix();
		modelStack.Translate(-5, 0, -5);
		modelStack.Scale(0.015f, 0.015f, 0.015f);
		modelStack.Rotate(150, -1, 0, 0);

		meshList[GEO_TARGET]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_TARGET]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_TARGET], enableLight);
		modelStack.PopMatrix();
	}

	if (!TargetHit2)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-3, 0, -8);
		modelStack.Scale(0.015f, 0.015f, 0.015f);
		modelStack.Rotate(90, -1, 0, 0);
		meshList[GEO_TARGET]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_TARGET]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_TARGET], enableLight);
		modelStack.PopMatrix();
	}
	if (TargetHit2)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-3, 0, -8);
		modelStack.Scale(0.015f, 0.015f, 0.015f);
		modelStack.Rotate(150, -1, 0, 0);

		meshList[GEO_TARGET]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_TARGET]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_TARGET], enableLight);
		modelStack.PopMatrix();
	}

	if (!TargetHit3)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-1, 0, -5);
		modelStack.Scale(0.015f, 0.015f, 0.015f);
		modelStack.Rotate(90, -1, 0, 0);

		meshList[GEO_TARGET]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_TARGET]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_TARGET], enableLight);
		modelStack.PopMatrix();
	}
	if (TargetHit3)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-1, 0, -5);
		modelStack.Scale(0.015f, 0.015f, 0.015f);
		modelStack.Rotate(150, -1, 0, 0);

		meshList[GEO_TARGET]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_TARGET]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_TARGET], enableLight);
		modelStack.PopMatrix();
	}

	if (!TargetHit4)
	{
		modelStack.PushMatrix();
		modelStack.Translate(1, 0, -8);
		modelStack.Scale(0.015f, 0.015f, 0.015f);
		modelStack.Rotate(90, -1, 0, 0);

		meshList[GEO_TARGET]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_TARGET]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_TARGET], enableLight);
		modelStack.PopMatrix();
	}
	if (TargetHit4)
	{
		modelStack.PushMatrix();
		modelStack.Translate(1, 0, -8);
		modelStack.Scale(0.015f, 0.015f, 0.015f);
		modelStack.Rotate(150, -1, 0, 0);

		meshList[GEO_TARGET]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_TARGET]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_TARGET], enableLight);
		modelStack.PopMatrix();
	}

	if (!TargetHit5)
	{
		modelStack.PushMatrix();
		modelStack.Translate(3, 0, -5);
		modelStack.Scale(0.015f, 0.015f, 0.015f);
		modelStack.Rotate(90, -1, 0, 0);

		meshList[GEO_TARGET]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_TARGET]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_TARGET], enableLight);
		modelStack.PopMatrix();
	}
	if (TargetHit5)
	{
		modelStack.PushMatrix();
		modelStack.Translate(3, 0, -5);
		modelStack.Scale(0.015f, 0.015f, 0.015f);
		modelStack.Rotate(150, -1, 0, 0);

		meshList[GEO_TARGET]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_TARGET]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_TARGET], enableLight);
		modelStack.PopMatrix();
	}

	if (!TargetHit6)
	{
		modelStack.PushMatrix();
		modelStack.Translate(5, 0, -8);
		modelStack.Scale(0.015f, 0.015f, 0.015f);
		modelStack.Rotate(90, -1, 0, 0);

		meshList[GEO_TARGET]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_TARGET]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_TARGET], enableLight);
		modelStack.PopMatrix();
	}
	if (TargetHit6)
	{
		modelStack.PushMatrix();
		modelStack.Translate(5, 0, -8);
		modelStack.Scale(0.015f, 0.015f, 0.015f);
		modelStack.Rotate(150, -1, 0, 0);

		meshList[GEO_TARGET]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TARGET]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_TARGET]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_TARGET], enableLight);
		modelStack.PopMatrix();
	}

	//Amongus NPC
	modelStack.PushMatrix();
	modelStack.Translate(2.5f, 0, 2.0f);
	modelStack.Scale(1.f, 1.f, 1.f);
	meshList[GEO_AMONGUS]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_AMONGUS]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_AMONGUS]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
	meshList[GEO_AMONGUS]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_AMONGUS], enableLight);
	modelStack.PopMatrix();

	//Amongus player
	modelStack.PushMatrix();
	modelStack.Translate(playerposition.x, 0, playerposition.z);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1.f, 1.3f, 1.f);
	meshList[GEO_AMONGUS2]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_AMONGUS2]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_AMONGUS2]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
	meshList[GEO_AMONGUS2]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_AMONGUS2], enableLight);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-5.f, 2.5f, 0.f);
	modelStack.Scale(0.6f, 0.6f, 0.6f);
	RenderText(meshList[GEO_TEXT], "Shoot The Targets!", Color(0, 1, 0));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-0.6f, 0.645f, -0.25f);
	{
		modelStack.PushMatrix();
		modelStack.Translate(0.f, 0.f, 0.05f);
		{
			modelStack.PushMatrix();
			modelStack.Translate(0.f, 0.f, 0.05f);
			{
				modelStack.PushMatrix();
				modelStack.Translate(-0.1f, 0.f, 0.05f);
				modelStack.Rotate(90, -1, 0, 0);
				modelStack.Scale(0.07f, 0.07f, 0.07f);
				RenderText(meshList[GEO_TEXT], "Targets", Color(0, 0, 0));
				modelStack.PopMatrix();
			}
			modelStack.Rotate(90, -1, 0, 0);
			modelStack.Scale(0.07f, 0.07f, 0.07f);
			RenderText(meshList[GEO_TEXT], "The", Color(0, 0, 0));
			modelStack.PopMatrix();
		}
		modelStack.Rotate(90, -1, 0, 0);
		modelStack.Scale(0.07f, 0.07f, 0.07f);
		RenderText(meshList[GEO_TEXT], "All", Color(0, 0, 0));
		modelStack.PopMatrix();
	}
	modelStack.Rotate(90, -1, 0, 0);
	modelStack.Scale(0.07f, 0.07f, 0.07f);
	RenderText(meshList[GEO_TEXT], "Shoot", Color(0, 0, 0));
	modelStack.PopMatrix();



	static bool isRightUp = false;
	if (!isRightUp && MouseController::GetInstance()->IsButtonDown(GLFW_MOUSE_BUTTON_LEFT) && BowEquip)
	{
		modelStack.PushMatrix();
		//No transform needed
		RenderMeshOnScreen(meshList[GEO_BOWSHOOT], 400, 300, 9, 6);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	//No transform needed
	std::string temp("FPS:" + std::to_string(fps));
	RenderTextOnScreen(meshList[GEO_TEXT], temp.substr(0, 9), Color(0, 1, 0), 40, 0, 0);
	modelStack.PopMatrix();

	/*modelStack.PushMatrix();
	//No transform needed
	RenderMeshOnScreen(meshList[GEO_GUI], 50, 50, 1, 1);
	modelStack.PopMatrix();*/

	modelStack.PushMatrix();
	//No transform needed
	RenderTextOnScreen(meshList[GEO_TEXT], interaction, Color(0, 0, 1), 35, 30, 70);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//No transform needed
	RenderTextOnScreen(meshList[GEO_TEXT], NPCinteraction, Color(0, 0, 1), 35, 30, 70);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//No transform needed
	RenderTextOnScreen(meshList[GEO_TEXT], NPCinteraction2, Color(0, 0, 1), 35, 30, 45);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//No transform needed
	RenderMeshOnScreen(meshList[GEO_CROSSHAIR], 400, 300, 1, 1);
	modelStack.PopMatrix();
}

void SceneAssignment2::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneAssignment2::RenderSkyBox()
{
	modelStack.PushMatrix();
	modelStack.Translate(0.f, 0.f, -50.f); // SO THAT THE QUAD IS OFF BY 50 UNITS INSIDE
	RenderMesh(meshList[GEO_FRONT], false); // SKYBOX SHOULD BE RENDERED WITHOUT LIGHT
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.f, 0.f, 50.f); // SO THAT THE QUAD IS OFF BY 50 UNITS INSIDE
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_BACK], false); // SKYBOX SHOULD BE RENDERED WITHOUT LIGHT
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(50.f, 0.f, 0.f); // SO THAT THE QUAD IS OFF BY 50 UNITS INSIDE
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_RIGHT], false); // SKYBOX SHOULD BE RENDERED WITHOUT LIGHT
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-50.f, 0.f, 0.f); // SO THAT THE QUAD IS OFF BY 50 UNITS INSIDE
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_LEFT], false); // SKYBOX SHOULD BE RENDERED WITHOUT LIGHT
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.f, 50.f, 0.f); // SO THAT THE QUAD IS OFF BY 50 UNITS INSIDE
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_TOP], false); // SKYBOX SHOULD BE RENDERED WITHOUT LIGHT
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.f, -50.f, 0.f); // SO THAT THE QUAD IS OFF BY 50 UNITS INSIDE
	modelStack.Rotate(-90, 1, 0, 0);
	RenderMesh(meshList[GEO_BOTTOM], false); // SKYBOX SHOULD BE RENDERED WITHOUT LIGHT
	modelStack.PopMatrix();
}

void SceneAssignment2::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
{
	glDisable(GL_DEPTH_TEST);
	glm::mat4 ortho = glm::ortho(0.f, 800.f, 0.f, 600.f, -1000.f, 1000.f); // dimension of screen UI
		projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0.f);
	modelStack.Scale(sizex, sizey, 0);
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneAssignment2::RenderText(Mesh* mesh, std::string text, Color color)
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
			glm::mat4(1.f),
			glm::vec3(i * 1.0f, 0, 0));
		glm::mat4 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_CULL_FACE);

}

void SceneAssignment2::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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
			glm::mat4(1.f),
			glm::vec3(0.5f + i * 1.0f, 0.5f, 0)
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

void SceneAssignment2::Exit()
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

void SceneAssignment2::HandleKeyPress(float dt)
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

	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_TAB))
	{
		if (light[0].type == Light::LIGHT_POINT) {
			light[0].type = Light::LIGHT_DIRECTIONAL;
		}
		else if (light[0].type == Light::LIGHT_DIRECTIONAL) {
			light[0].type = Light::LIGHT_SPOT;
		}
		else {
			light[0].type = Light::LIGHT_POINT;
		}

		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}
	if (Cameraswitch == false)
	{
		if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W))
		{
			glm::vec3 orientation = glm::vec4(camera.yawView.x, 0.f, camera.yawView.z, 0.f) * dt;
			camera.position += orientation;
			camera.target += orientation;
			playerposition += orientation;
		}
		if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S))
		{
			glm::vec3 orientation = glm::vec4(camera.yawView.x, 0.f, camera.yawView.z, 0.f) * dt;
			camera.position -= orientation;
			camera.target -= orientation;
			playerposition -= orientation;
		}
		if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A))
		{
			glm::vec3 vectorToRotate = glm::vec4(camera.yawView.x, 0.f, camera.yawView.z, 0.f) * dt;
			vectorToRotate = glm::rotate(
				glm::mat4(1.f),
				glm::radians(90.f),
				glm::vec3(0, 1, 0))
				* glm::vec4(vectorToRotate, 0.f);
			camera.position += vectorToRotate;
			camera.target += vectorToRotate;
			playerposition += vectorToRotate;

		}
		if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D))
		{
			glm::vec3 vectorToRotate = glm::vec4(camera.yawView.x, 0.f, camera.yawView.z, 0.f) * dt;
			vectorToRotate = glm::rotate(
				glm::mat4(1.f),
				glm::radians(90.f),
				glm::vec3(0, 1, 0))
				* glm::vec4(vectorToRotate, 0.f);
			camera.position -= vectorToRotate;
			camera.target -= vectorToRotate;
			playerposition -= vectorToRotate;
		}
	}

	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_C))
	{
		Cameraswitch = !Cameraswitch;
	}
	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_B))
	{
		PanCamera += static_cast<float>(dt) * 10;
	}
	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_V))
	{
		PanCamera -= static_cast<float>(dt) * 10;
	}
}

void SceneAssignment2::HandleMouseInput()
{
	static bool isLeftUp = false;
	static bool isRightUp = false;
	// Process Left button
	if (!isLeftUp && MouseController::GetInstance() -> IsButtonDown(GLFW_MOUSE_BUTTON_LEFT))
	{
		isLeftUp = true;
		std::cout << "LBUTTON DOWN" << std::endl;

		// transform into UI space
		double x = MouseController::GetInstance()->GetMousePositionX();
		double y = 600 - MouseController::GetInstance()->GetMousePositionY();
		// Check if mouse click position is within the GUI box
		// Change the boundaries as necessary
		if (x > 0 && x < 100 && y > 0 && y < 100) {
			std::cout << "GUI IS CLICKED" << std::endl;
		}
	}
	else if (isLeftUp && MouseController::GetInstance() -> IsButtonUp(GLFW_MOUSE_BUTTON_LEFT))
	{
		isLeftUp = false;
		std::cout << "LBUTTON UP" << std::endl;

		if (BowEquip && camera.HitDetect(-5, 0.7, -5, 0.7)) {
			TargetHit1 = true;
		}
		if (BowEquip && camera.HitDetect(-3, 0.7, -8, 0.7)) {
			TargetHit2 = true;
		}
		if (BowEquip && camera.HitDetect(-1, 0.7, -5, 0.7)) {
			TargetHit3 = true;
		}
		if (BowEquip && camera.HitDetect(1, 0.7, -8, 0.7)) {
			TargetHit4 = true;
		}
		if (BowEquip && camera.HitDetect(3, 0.7, -5, 0.7)) {
			TargetHit5 = true;
		}
		if (BowEquip && camera.HitDetect(5, 0.7, -8, 0.7)) {
			TargetHit6 = true;
		}
	}

	//Process left button
	if (!isRightUp && MouseController::GetInstance()->IsButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
	{
		isRightUp = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if (isRightUp && MouseController::GetInstance()->IsButtonUp(GLFW_MOUSE_BUTTON_RIGHT))
	{
		isRightUp = false;
		std::cout << "RBUTTON UP" << std::endl;
	}

	if (MouseController::GetInstance()->IsButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
		std::cout << "LMU PRESSED" << std::endl;
	}

}
