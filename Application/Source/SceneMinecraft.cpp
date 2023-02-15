#include "SceneMinecraft.h"
#include "GL\glew.h"

//GLM Headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "KeyboardController.h"
#include <GLFW/glfw3.h>
#include <iostream>

SceneMinecraft::SceneMinecraft()
{
}

SceneMinecraft::~SceneMinecraft()
{
}	

void SceneMinecraft::Init()
{
	FBShoulderR = 0;
	LRShoulderR = 0;
	FBShoulderL = 0;
	LRShoulderL = 0;
	FBElbowR = 0;
	LRElbowR = 0;
	FBElbowL = 0;
	LRElbowL = 0;
	FBThighR = 0;
	LRThighR = 0;
	FBThighL = 0;
	LRThighL = 0;
	FBThigh = 0;
	FBKneeR = 0;
	LRKneeR = 0;
	FBKneeL = 0;
	LRKneeL = 0;
	FBKnee = 0;
	UDHead = 0;
	LFHead = 0;
	FBTorso = 0;
	FBTorso2 = 0;
	RTorso = 0;
	direction = 1;
	direction1 = 1;
	direction2 = -1;
	direction3 = 1;
	direction4 = 1;
	direction5 = 1;
	direction6 = 1;
	direction7 = 1;
	direction8 = 1;
	direction9 = 1;
	direction10 = 1;
	direction11 = 1;
	CharacterPos = glm::vec3(0,0,0);

	enableLight = true;

	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);

	// Load view matrix stack and set it with camera position, target position and up direction
	viewStack.LoadIdentity();

	// Load identity matrix into the model stack
	modelStack.LoadIdentity();

	// Initialise camera properties
	camera.Init(45.f, 45.f, 10.f);

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
	m_programID = LoadShaders("Shader//Shading.vertexshader", "Shader//Shading.fragmentshader");
	glUseProgram(m_programID);


	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");


	light[0].position = glm::vec3(0, 6, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 3;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;

	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);

	// Init VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("Axes", 10000.f, 10000.f, 10000.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Earth", Color(0.5f, 0.5f, 0.5f), 1.f, 16, 16);
	meshList[GEO_SPHERE_WHITE] = MeshBuilder::GenerateSphere("Sphere", Color(1.f, 1.f, 1.f), 1.f, 16, 16);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("Cube", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_PLAIN] = MeshBuilder::GenerateCube("Cube", Color(0.f, 1.f, 0.f), 1.f);
	meshList[GEO_CYLINDER] = MeshBuilder::GenerateCylinder("Cylinder", Color(0.5f, 0.5f, 0.5f), 1.f, 30.f, 1.f);
	meshList[GEO_CYLINDER_WHITE] = MeshBuilder::GenerateCylinder("Cylinder", Color(1.f, 1.f, 1.f), 1.f, 30.f, 1.f);
	meshList[GEO_HEMISPHERE] = MeshBuilder::GenerateHemisphere("Hemisphere", Color(0.5f, 0.5f, 0.5f), 1.f, 100.f, 1.f);
	meshList[GEO_HORN] = MeshBuilder::GenerateHalfTorus("Torus", Color(1.f, 1.f, 1.f), 1.f, 5.f, 12, 12);
	meshList[GEO_TOPHORN] = MeshBuilder::GenerateCone("Cone", Color(1.f, 1.f, 1.f), 30.f, 0.5f, 1.f);
	meshList[GEO_FEET] = MeshBuilder::GenerateCone("Cone", Color(0.f, 0.f, 0.f), 30.f, 0.5f, 1.f);
	meshList[GEO_SWORD] = MeshBuilder::GenerateCone("Cone", Color(0.5f, 0.5f, 0.5f), 30.f, 0.35f, 3.f);
	meshList[GEO_SWORDPT2] = MeshBuilder::GenerateCircle("Circle", Color(0.5f, 0.5f, 0.5f), 0.36f, 24);
	meshList[GEO_HANDLE] = MeshBuilder::GenerateCylinder("Cylinder", Color(0.5f, 0.5f, 0.5f), 1.f, 30.f, 1.f);

	ANIMATION currAnim = ANIMATION::RESET;
}

void SceneMinecraft::Update(double dt)
{
	// Check for key press, you can add more for interaction
	HandleKeyPress();
	camera.Update(dt);
	timeelapsed += (dt);
	switch (currAnim)
	{
		//ANIMATIONS:
		//Press 2 to make character fly up and attacks diagonally down left (Complex)
		//2 = SUPERSAIYAN
		//Press 5 to make character run then slash & dash at the same time (Complex)
		//5 = RUN_SLASH&DASH
		//Press 6 to make character backflip the frontflip and slam weapon forward (Complex)
		//6 = BACKFLIPnFRONTFLIP
		//Press 7 to make character frontflip (Simple)
		//7 = FRONTFLIP_FORWARDATTACK
		//Press 8 to make character hop forwards (Simple)
		//8 = HOP_FORWARD
		//Press 9 to make chracter wave (Simple)
		//9 = WAVE

	case RESET:
		FBShoulderR = 0;
		LRShoulderR = 0;
		FBShoulderL = 0;
		LRShoulderL = 0;
		FBElbowR = 0;
		LRElbowR = 0;
		FBElbowL = 0;
		LRElbowL = 0;
		FBThighR = 0;
		LRThighR = 0;
		FBThighL = 0;
		LRThighL = 0;
		FBKneeR = 0;
		LRKneeR = 0;
		FBKneeL = 0;
		LRKneeL = 0;
		FBKnee;
		UDHead = 0;
		LFHead = 0;
		direction = 1;
		direction1 = 1;
		direction2 = -1;
		direction3 = 1;
		direction4 = 1;
		direction5 = 1;
		direction6 = 1;
		direction7 = 1;
		direction8 = 1;
		direction9 = 1;
		direction10 = 1;
		direction11 = 1;
		CharacterPos = glm::vec3(0, 0, 0);
		FBTorso = 0;
		RTorso = 0;
		FBTorso2 = 0;
		timeelapsed = 0;
		activate = 0;
		reverse = 0;
		FBHandL = 0;
		LRHandL = 0;
		FBHandR = 0;
		LRHandR = 0;
		flipAnim = 0;
		running = 0;
		BF1 = 0;
		BF2 = 0;
		BF3 = 0;
		Chargeinpos = false;
		break;
	case WAVE:
		LRShoulderL += 2.5f * direction;
		if (LRShoulderL == 110.f)
		{
			direction = 0;
			LRElbowL += 2.0f * direction2;
			if (LRElbowL == 80.f || LRElbowL == 10.f && direction2 < 0.f)
			{
				direction2 = -direction2;
			}
		}
		LRShoulderR -= 2.5f * direction3;
		if (LRShoulderR == -110.f)
		{
			direction3 = 0;
			LRElbowR -= 2.0f * direction1;
			if (LRElbowL == 80.f || LRElbowL == 10.f && direction1 < 0.f)
			{
				direction1 = -direction1;
			}
		}
		break;
	case HOP_FORWARD:
		CharacterPos.y = -1 * sinf(timeelapsed * -3.5);
		CharacterPos.z += 0.03;
		FBThighL += 2.5f * direction1;
		FBThighR += 2.5f * direction2;
		if (FBThighL == 35.f && FBKneeL > 0.f)
		{
			direction1 = 0;
		}
		if (FBThighL <= 35.f && FBKneeL <= 0.f)
		{
			direction1 = -1;
		}
		if (FBThighL == -85.f)
		{
			direction1 = -direction1;
		}
		if (FBThighR == 35.f && FBThighR > 0.f)
		{
			direction2 = 0;
		}
		if (FBThighR <= 35.f && FBKneeR <= 0.f)
		{
			direction2 = -1;
		}
		if (FBThighR == -85.f)
		{
			direction2 = -direction2;
		}
		if (FBThighL > -85.f && FBThighL < 20.f)
		{
			FBKneeL += 0.5f;
		}
		if (FBThighR > -85.f && FBThighR < 20.f)
		{
			FBKneeR += 0.5f;
		}
		if (FBThighL == 35.f && FBKneeL >= 0.f)
		{
			FBKneeL -= 4.5f;
		}
		if (FBThighR == 35.f && FBKneeR > 0.f)
		{
			FBKneeR -= 4.5f;
		}

		if (!reverse)
		{
			FBTorso += 0.7f * direction3;
		}
		else
		{
			FBTorso -= 0.39f * direction3;
		}
		if (FBTorso >= 25.f) //&& FBThighL >= -85.f && FBThighR >= -85.f)// && FBThighL >= -83.f && FBThighR >= -83.f)
		{
			reverse = true;
		}
		else if (FBTorso <= -0.8f)// && FBThighL >= 35.f && FBThighR >= 35.f)// && FBThighL >= 25.f && FBThighR >= 25.f)
		{
			reverse = false;
		}

		FBShoulderL -= 1.5 * direction4;
		if (FBShoulderL <= -55.f)
		{
			direction4 = -direction4;
		}
		else if (FBShoulderL >= 21.5f)
		{
			direction4 = -direction4;
		}
		FBShoulderR -= 1.5 * direction5;
		if (FBShoulderR <= -55.f)
		{
			direction5 = -direction5;
		}
		else if (FBShoulderR >= 21.5f)
		{
			direction5 = -direction5;
		}
		break;

	case BACKFLIPnFRONTFLIP:
		if (!Chargeinpos)
		{
			if (FBTorso <= 30)
			{
				FBTorso += static_cast<float>(dt) * 50;
			}
			if (FBThighL >= -80)
			{
				FBThighL += static_cast<float>(dt) * -130;
				FBKneeL += static_cast<float>(dt) * 130;
			}
			if (FBThighR >= -80)
			{
				FBThighR += static_cast<float>(dt) * -130;
				FBKneeR += static_cast<float>(dt) * 130;
			}
			if (CharacterPos.y >= -0.6)
			{
				CharacterPos.y += static_cast<float>(dt) * -1;
			}
			if (FBShoulderL <= 80)
			{
				FBShoulderL += static_cast<float>(dt) * 130;
				FBElbowL += static_cast<float>(dt) * -130;
			}
			if (FBShoulderR <= 80)
			{
				FBShoulderR += static_cast<float>(dt) * 130;
				FBElbowR += static_cast<float>(dt) * -130;
			}
			else
			{
				Chargeinpos = true;
			}
		}	
		
		else
		{
			if (Chargeinpos)
			{
				if (!flipAnim)
				{
					/*std::cout << timeelapsed << std::endl;
					delay += timeelapsed;
					if (timeelapsed == 1.45989234759);*/
					if (!BF1)
					{
						if (FBThighL <= 0)
						{
							FBThighL += static_cast<float>(dt) * 180;
							FBKneeL += static_cast<float>(dt) * -180;
						}
						if (FBThighR <= 0)
						{
							FBThighR += static_cast<float>(dt) * 180;
							FBKneeR += static_cast<float>(dt) * -180;
						}
						if (FBTorso >= 0)
						{
							FBTorso += static_cast<float>(dt) * -50;
						}
						if (FBShoulderL >= -50)
						{
							FBShoulderL += static_cast<float>(dt) * -130;
							FBElbowL += static_cast<float>(dt) * -10;
						}
						if (FBShoulderR >= -50)
						{
							FBShoulderR += static_cast<float>(dt) * -130;
							FBElbowR += static_cast<float>(dt) * -10;
						}
						if (!reverse)
						{
							CharacterPos.y += static_cast<float>(dt) * 3.5f;
							FBShoulderR += static_cast<float>(dt) * -130;
							FBElbowR += static_cast<float>(dt) * 130;

						}
						else
						{
							if (CharacterPos.y >= 0)
							{
								CharacterPos.y += static_cast<float>(dt) * -3.5f;
							}
						}
						if (FBTorso <= -180)
						{
							reverse = true;
						}
						else if (FBTorso <= -360)
						{
							reverse = false;
						}
						if (FBThighR >= 0)
						{
							BF1 = true;
						}
					}
					else
					{
						if (BF1)
						{
							if (FBTorso >= -30)
							{
								FBTorso += static_cast<float>(dt) * -60;
							}
							if (FBTorso <= -30 && FBTorso >= -300)
							{
								FBTorso += static_cast<float>(dt) * -150;
								if (FBThighL >= -90)
								{
									FBThighL += static_cast<float>(dt) * -250;
								}
								if (FBKneeL <= 45)
								{
									FBKneeL += static_cast<float>(dt) * 250;
								}
								if (FBThighR >= -90)
								{
									FBThighR += static_cast<float>(dt) * -250;
								}
								if (FBKneeR <= 45)
								{
									FBKneeR += static_cast<float>(dt) * 250;
								}

							}
							if (FBTorso <= -350)
							{
								if (FBThighL <= 0)
								{
									FBThighL += static_cast<float>(dt) * 250;
								}
								if (FBKneeL >= 0)
								{
									FBKneeL += static_cast<float>(dt) * -250;
								}
								if (FBThighR <= 0)
								{
									FBThighR += static_cast<float>(dt) * 250;
								}
								if (FBKneeR >= 0)
								{
									FBKneeR += static_cast<float>(dt) * -250;
								}
								if (FBShoulderR <= 0)
								{
									FBShoulderR += static_cast<float>(dt) * 230;
								}
								if (FBShoulderL >= 0)
								{
									FBShoulderL += static_cast<float>(dt) * -230;
								}
								if (FBElbowL <= 0)
								{
									FBElbowL += static_cast<float>(dt) * 230;
								}
								if (FBElbowR <= 0)
								{
									FBElbowR += static_cast<float>(dt) * 230;
								}
								if (CharacterPos.y >= 0)
								{
									CharacterPos.y += static_cast<float>(dt) * -2;
								}
								if (CharacterPos.y <= 0)
								{
									flipAnim = true;
								}
							}
							if (FBTorso >= -360)
							{
								FBTorso += static_cast<float>(dt) * -100;

							}

						}
					
					}

				}
				else {
					if (flipAnim)
					{
						if (!activate)
						{
							if (FBTorso <= 30)
							{
								FBTorso += static_cast<float>(dt) * 50;
							}
							if (FBThighL >= -80)
							{
								FBThighL += static_cast<float>(dt) * -130;
								FBKneeL += static_cast<float>(dt) * 130;
							}
							if (FBThighR >= -80)
							{
								FBThighR += static_cast<float>(dt) * -130;
								FBKneeR += static_cast<float>(dt) * 130;
							}
							if (CharacterPos.y >= -0.6)
							{
								CharacterPos.y += static_cast<float>(dt) * -1;
							}
							if (FBShoulderL >= -100)
							{
								FBShoulderL += static_cast<float>(dt) * -130;
								FBElbowL += static_cast<float>(dt) * -130;
							}
							if (FBShoulderR >= -100)
							{
								FBShoulderR += static_cast<float>(dt) * -130;
								FBElbowR += static_cast<float>(dt) * -130;
							}
							else
								activate = true;

						}
						else
						{

							if (activate)
							{
								std::cout << FBTorso << std::endl;
								//std::cout << timeelapsed << std::endl;
								delay += timeelapsed;
								if (timeelapsed == 1.45989234759);
								{
									if (FBThighL <= 0 && !activate)
									{
										FBThighL += static_cast<float>(dt) * 360;
										FBKneeL += static_cast<float>(dt) * -360;
									}
									if (FBThighR <= 0 && !activate)
									{
										FBThighR += static_cast<float>(dt) * 360;
										FBKneeR += static_cast<float>(dt) * -360;
									}
									else if (activate)
									{
										if (FBThighL >= -140)
										{
											FBThighL += static_cast<float>(dt) * -130;

										}
										if (FBKneeL >= 0)
										{
											FBKneeL += static_cast<float>(dt) * -500;
										}
										if (FBKneeR >= 0)
										{
											FBKneeR += static_cast<float>(dt) * -500;
										}
										if (FBThighR >= -140)
										{
											FBThighR += static_cast<float>(dt) * -130;
										}
										if (FBKneeL <= 0) {
											FBKneeR += static_cast<float>(dt) * 130;
										}
										if (FBShoulderL >= -130) {
											FBShoulderL += static_cast<float>(dt) * -130;
										}
										if (LRShoulderL <= 20)
										{
											LRShoulderL += static_cast<float>(dt) * 130;
										}
										if (FBElbowL <= 0)
										{
											FBElbowL += static_cast<float>(dt) * 130;
										}
										if (FBElbowR <= 0)
										{
											FBElbowR += static_cast<float>(dt) * 130;
										}
										if (LRElbowR <= 90)
										{
											LRElbowR += static_cast<float>(dt) * 130;
										}
										if (FBHandL <= 90)
										{
											FBHandL += static_cast<float>(dt) * 130;
										}

									}
									if (FBTorso <= 410)
									{
										FBTorso += static_cast<float>(dt) * 200;
									}

									if (!reverse)
									{
										CharacterPos.y += static_cast<float>(dt) * 3.5f;
									}
									else
									{
										if (CharacterPos.y >= -2)
										{
											CharacterPos.y += static_cast<float>(dt) * -9.5f;
										}
									}


									if (FBTorso >= 180)
									{
										reverse = true;

									}
									else if (FBTorso >= 410)
									{
										reverse = false;
									}
								}
							}
						}
					}
				}
			}
		}
		break;

	case FRONTFLIP_FORWARDATTACK:
		if (!activate)
		{
			if (FBTorso <= 30)
			{
				FBTorso += static_cast<float>(dt) * 50;
			}
			if (FBThighL >= -80)
			{
				FBThighL += static_cast<float>(dt) * -130;
				FBKneeL += static_cast<float>(dt) * 130;
			}
			if (FBThighR >= -80)
			{
				FBThighR += static_cast<float>(dt) * -130;
				FBKneeR += static_cast<float>(dt) * 130;
			}
			if (CharacterPos.y >= -0.6)
			{
				CharacterPos.y += static_cast<float>(dt) * -1;
			}
			if (FBShoulderL >= -100)
			{
				FBShoulderL += static_cast<float>(dt) * -130;
				FBElbowL += static_cast<float>(dt) * -130;
			}
			if (FBShoulderR >= -100)
			{
				FBShoulderR += static_cast<float>(dt) * -130;
				FBElbowR += static_cast<float>(dt) * -130;
			}
			else
				activate = true;
		
		}
		else
		{

			if (activate)
			{
				std::cout << FBTorso << std::endl;
				//std::cout << timeelapsed << std::endl;
				delay += timeelapsed;
				if (timeelapsed == 1.45989234759);
				{
					if (FBThighL <= 0 && !activate)
					{
						FBThighL += static_cast<float>(dt) * 360;
						FBKneeL += static_cast<float>(dt) * -360;
					}
					if (FBThighR <= 0 && !activate)
					{
						FBThighR += static_cast<float>(dt) * 360;
						FBKneeR += static_cast<float>(dt) * -360;
					}
					else if (activate)
					{
						if (FBThighL >= -140)
						{
							FBThighL += static_cast<float>(dt) * -130;
							
						}
						if (FBKneeL <= 0)
						{
							FBKneeL += static_cast<float>(dt) * 130;
						}
						if (FBThighR >= -140)
						{
							FBThighR += static_cast<float>(dt) * -130;
						}
						if (FBKneeL <= 0) {
							FBKneeR += static_cast<float>(dt) * 130;
						}
						if (FBShoulderL >= -130) {
							FBShoulderL += static_cast<float>(dt) * -130;
						}
						if (LRShoulderL <= 20) 
						{
							LRShoulderL += static_cast<float>(dt) * 130;
						}
						if (FBElbowL <= 0)
						{
							FBElbowL += static_cast<float>(dt) * 130;
						}
						if (FBElbowR <= 0)
						{
							FBElbowR += static_cast<float>(dt) * 130;
						}
						if (LRElbowR <= 90)
						{
							LRElbowR += static_cast<float>(dt) * 130;
						}
						if (FBHandL <= 90)
						{
							FBHandL += static_cast<float>(dt) * 130;
						}

					}
					if (FBTorso <= 410)
					{
						FBTorso += static_cast<float>(dt) * 200;
					}

					if (!reverse)
					{
						CharacterPos.y += static_cast<float>(dt) * 3.5f;
					}
					else
					{
						if (CharacterPos.y >= -2.4)
						{
							CharacterPos.y += static_cast<float>(dt) * -3.5f;
						}
					}


					if (FBTorso >= 180)
					{
						reverse = true;

					}
					else if (FBTorso >= 410)
					{
						reverse = false;
					}
				}
			}
		}
		break;

	case RUN_SLASHnDASH:
		if (!running) {
			CharacterPos.y = -0.1 * sinf(timeelapsed * 10);
			if (CharacterPos.z <= 0)
			{
				CharacterPos.z += 0.05;
			}
			if (FBTorso <= 10) {
				FBTorso += 2.5 * direction3;
			}
			FBShoulderL += 3.5f * direction;
			FBShoulderR -= 3.5f * direction;
			if (FBElbowL <= 0 || direction < 0)
			{
				FBElbowL += 1.f * direction;
			}
			if (FBElbowR <= 0 || direction > 0)
			{
				FBElbowR -= 1.f * direction;
			}
			FBThighR += 2.5f * direction;
			if (FBThighR <= -45.f && FBKneeR > 0.f)
			{
				direction = 0;
			}
			if (FBThighR <= -45.f && FBKneeR <= 0.f)
			{
				direction = 1;
			}
			if (FBThighR >= 45.f)
			{
				direction = -direction;
			}
			if (FBThighR <= 45.f && FBThighR > 20.f)
			{
				FBKneeR += 1.5f;
			}
			if (FBThighR <= -45.f && FBKneeR > 0.f)
			{
				FBKneeR -= 3.5f;
			}


			FBThighL += 2.5f * direction2;
			if (FBThighL <= -45.f && FBKneeL > 0.f)
			{
				direction2 = 0;
			}
			if (FBThighL <= -45.f && FBKneeL <= 0.f)
			{
				direction2 = 1;
			}
			if (FBThighL >= 45.f)
			{
				direction2 = -direction2;
			}
			if (FBThighL <= 45.f && FBThighL > 20.f)
			{
				FBKneeL += 1.5f;
			}
			if (FBThighL <= -45.f && FBKneeL > 0.f)
			{
				FBKneeL -= 3.5f;
			}
			if (CharacterPos.z >= 0.f)
			{
				running = true;
			}
		}
		else {
			if (running)
			{
				if (CharacterPos.z >= 0 && !activate)
				{
					direction = 0;
					direction2 = 0;
					if (FBTorso >= 0)
					{
						FBTorso += static_cast<float>(dt) * -20.f;
					}
					if (RTorso >= -20)
					{
						RTorso += static_cast<float>(dt) * -50.f;
					}
					if (FBThighL >= -30)
					{
						FBThighL += static_cast<float>(dt) * -50.f;
					}
					if (LRThighL >= 15)
					{
						LRThighL += static_cast<float>(dt) * 50.f;
					}
					if (FBKneeL <= 20)
					{
						FBKneeL += static_cast<float>(dt) * 50.f;
					}
					if (FBThighR >= -20)
					{
						FBThighR += static_cast<float>(dt) * -50.f;
					}
					if (LRThighR >= -35)
					{
						LRThighR += static_cast<float>(dt) * -50.f;
					}
					if (FBKneeR <= 20)
					{
						FBKneeR += static_cast<float>(dt) * 50.f;
					}
					if (LRKneeR <= 50)
					{
						LRKneeR += static_cast<float>(dt) * 50.f;
					}
					if (FBShoulderR <= 30) 
					{
						FBShoulderR += static_cast<float>(dt) * 50.f;
					}
					if (LRShoulderR >= -20)
					{
						LRShoulderR += static_cast<float>(dt) * -50.f;
					}
					if (FBShoulderL >= -70)
					{
						FBShoulderL += static_cast<float>(dt) * -50.f;
					}
					if (FBHandL <= 70)
					{
						FBHandL += static_cast<float>(dt) * 50.f;
					}
					if (LRElbowL >= -60)
					{
						LRElbowL += static_cast<float>(dt) * -50.f;
					}
					if (LFHead <= 20)
					{
						LFHead += static_cast<float>(dt) * 7.f;
					}
					if (LFHead >= 20)
					{
						activate = true;
					}
				}
				else
				{
					std::cout << timeelapsed << std::endl;
					delay += timeelapsed;
					if (timeelapsed == 15.34234);
					{
						if (RTorso <= 0)
						{
							RTorso += static_cast<float>(dt) * 50.f;
						}
						if (LFHead >= 0)
						{
							LFHead += static_cast<float>(dt) * -50.f;
						}
						if (LRElbowL <= 25)
						{
							LRElbowL += static_cast<float>(dt) * 370.f;
						}
						if (FBShoulderR >= -95)
						{
							FBShoulderR += static_cast<float>(dt) * -220.f;
						}
						if (FBShoulderL <= 0)
						{
							FBShoulderL += static_cast<float>(dt) * 220.f;
						}
						if (LRShoulderL <= 95)
						{
							LRShoulderL += static_cast<float>(dt) * 220.f;
						}
						if (LRElbowR <= 50)
						{
							LRElbowR += static_cast<float>(dt) * 220.f;
						}
						if (FBTorso < 45)
						{
							FBTorso += static_cast<float>(dt) * 200.f;
						}
						if (FBThighL <= 0)
						{
							FBThighL += static_cast<float>(dt) * 200.f;
						}
						if (UDHead >= -45)
						{
							UDHead += static_cast<float>(dt) * -150.f;
						}
						if (CharacterPos.z <= 6)
						{
							CharacterPos.z += static_cast<float>(dt) * 25.f;
						}
					}
				}
			}
		}
		break;

	case SUPERSAIYAN:
	{
		if (!activate)
		{
			if (FBTorso <= 25)
			{
				FBTorso += static_cast<float>(dt) * 12.f;
			}
			if (FBThigh >= -70)
			{
				FBThigh += static_cast<float>(dt) * -40.f;
			}
			if (FBKnee <= 70)
			{
				FBKnee += static_cast<float>(dt) * 40.f;
			}
			if (FBShoulderR >= -70)
			{
				FBShoulderR += static_cast<float>(dt) * -40.f;
			}
			if (FBShoulderL >= -70)
			{
				FBShoulderL += static_cast<float>(dt) * -40.f;
			}
			if (FBElbowR >= -70)
			{
				FBElbowR += static_cast<float>(dt) * -40.f;
			}
			if (FBElbowL >= -70)
			{
				FBElbowL += static_cast<float>(dt) * -40.f;
			}
			if (CharacterPos.y >= -0.55)
			{
				CharacterPos.y += static_cast<float>(dt) * -0.3f;
			}
			if (FBTorso >= 25)
			{
				activate = true;
			}
		}
		else
		{
			if (!BF1)
			{
				if (FBTorso >= 0)
				{
					FBTorso += static_cast<float>(dt) * -80.f;
				}
				if (FBThigh <= 0)
				{
					FBThigh += static_cast<float>(dt) * 120.f;
				}
				if (FBKnee >= 0)
				{
					FBKnee += static_cast<float>(dt) * -150.f;
				}
				if (FBShoulderR >= -170)
				{
					FBShoulderR += static_cast<float>(dt) * -300.f;
				}
				if (FBShoulderL >= -170)
				{
					FBShoulderL += static_cast<float>(dt) * -300.f;
				}
				if (FBElbowR <= 0)
				{
					FBElbowR += static_cast<float>(dt) * 300.f;
				}
				if (FBElbowL <= 0)
				{
					FBElbowL += static_cast<float>(dt) * 300.f;
				}
				if (CharacterPos.y <= 4)
				{
					CharacterPos.y += static_cast<float>(dt) * 10.f;
				}
				if (FBThigh >= 0)
				{
					BF1 = true;
				}
			}
			else
			{
				if (BF1)
				{
					if (!BF2)
					{
						if (CharacterPos.y >= 3)
						{
							CharacterPos.y += static_cast<float>(dt) * -1.f;
						}
						if (FBTorso <= 20)
						{
							FBTorso += static_cast<float>(dt) * 50.f;
						}
						if (FBShoulderL <= 0)
						{
							FBShoulderL += static_cast<float>(dt) * 300.f;
						}
						if (FBShoulderR <= -90)
						{
							FBShoulderR += static_cast<float>(dt) * 300.f;
						}
						if (FBThigh >= -45)
						{
							FBThigh += static_cast<float>(dt) * -300.f;
						}
						if (FBKnee <= 45)
						{
							FBKnee += static_cast<float>(dt) * 300.f;
						}
						if (CharacterPos.y <= 3)
						{
							BF2 = true;
						}
					}
					else 
					{
						if (BF2)
						{
							if (CharacterPos.y >= 0)
							{
								CharacterPos.y += static_cast<float>(dt) * -3.f;
							}
							if (CharacterPos.z <= 6)
							{
								CharacterPos.z += static_cast<float>(dt) * 6.f;;
							}
							if (FBTorso <= 115)
							{
								FBTorso += static_cast<float>(dt) * 300.f;
							}
							if (FBShoulderL >= -180)
							{
								FBShoulderL += static_cast<float>(dt) * -500.f;
							}
							if (FBShoulderR <= 0)
							{
								FBShoulderR += static_cast<float>(dt) * 300.f;
							}
							if (FBThigh <= 0)
							{
								FBThigh += static_cast<float>(dt) * 300.f;
							}
							if (FBKnee >= 0)
							{
								FBKnee += static_cast<float>(dt) * -300.f;
							}
							if (FBHandL <= 90)
							{
								FBHandL += static_cast<float>(dt) * 300.f;
							}
						}
					}
				}
			}

		}
	}

	}

	


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

void SceneMinecraft::Render()
{
	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Load view matrix stack and set it with camera position, target position and up direction
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);

	// Load identity matrix into the model stack
	modelStack.LoadIdentity();

	// Calculate the light position in camera space
	glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[0].position, 1);
	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));		

	modelStack.PushMatrix();
	// Render objects
	RenderMesh(meshList[GEO_AXES], false);
	modelStack.PopMatrix();

	/*modelStack.PushMatrix();
	{
		modelStack.Translate(0.f, -5.f, 0.f);
		meshList[GEO_CUBE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
		meshList[GEO_CUBE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_CUBE]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_CUBE]->material.kShininess = 5.0f;
		RenderMesh(meshList[GEO_CUBE], enableLight);
	}
	modelStack.PopMatrix();*/

	modelStack.Translate(0.f, 0.f, 0.f);
	modelStack.PushMatrix();
	{
	//torso (root)
		modelStack.PushMatrix();
		modelStack.Translate(0.f, 0.f, 0.f);
		modelStack.Translate(CharacterPos.x, CharacterPos.y, CharacterPos.z);
		modelStack.Rotate(FBTorso, 1.f, 0.f, 0.f);
		modelStack.Rotate(FBTorso2, 0.f, 0.f, 1.f);
		modelStack.Rotate(RTorso, 0.f, 1.f, 0.f);
		//(head)
		{
			//Neck
			modelStack.PushMatrix();
			modelStack.Translate(0.f, 1.2f, 0.f);
			modelStack.Rotate(LFHead, 0.f, 1.f, 0.f);
			modelStack.Rotate(UDHead, 1.f, 0.f, 0.f);
			{
				//Head
				modelStack.PushMatrix();
				modelStack.Translate(0.f,0.86f,0.f);

				modelStack.PushMatrix();
				modelStack.Translate(0.f, 0.5f, 0.f);

				modelStack.PushMatrix();
				modelStack.Translate(0.f, -0.99f, 0.f);
				{
					//Horn
					modelStack.PushMatrix();
					modelStack.Translate(0.f, 1.5f, 0.f);

					modelStack.PushMatrix();
					modelStack.Translate(0.f, 0.f, 0.f);
					{
						//Top of Horn
						modelStack.PushMatrix();
						modelStack.Translate(0.65f, 0.365f, 0.f);

						modelStack.PushMatrix();
						modelStack.Translate(-1.3f, 0.f, 0.f);
						modelStack.Rotate(-30.f, 0.f, 0.f, 1.f);
						modelStack.Scale(0.3f, 0.3f, 0.3f);
						meshList[GEO_TOPHORN]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
						meshList[GEO_TOPHORN]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
						meshList[GEO_TOPHORN]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
						meshList[GEO_TOPHORN]->material.kShininess = 2.0f;
						RenderMesh(meshList[GEO_TOPHORN], enableLight);
						modelStack.PopMatrix();

						modelStack.Rotate(30.f, 0.f, 0.f, 1.f);
						modelStack.Scale(0.3f, 0.3f, 0.3f);
						meshList[GEO_TOPHORN]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
						meshList[GEO_TOPHORN]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
						meshList[GEO_TOPHORN]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
						meshList[GEO_TOPHORN]->material.kShininess = 2.0f;
						RenderMesh(meshList[GEO_TOPHORN], enableLight);
						modelStack.PopMatrix();
					}
					modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
					modelStack.Rotate(240.f, 0.f, 1.f, 0.f);
					modelStack.Scale(0.15f, 0.15f, 0.15f);
					meshList[GEO_HORN]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
					meshList[GEO_HORN]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
					meshList[GEO_HORN]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
					meshList[GEO_HORN]->material.kShininess = 2.0f;
					RenderMesh(meshList[GEO_HORN], enableLight);
					modelStack.PopMatrix();

					modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
					modelStack.Rotate(-60.f, 0.f, 1.f, 0.f);
					modelStack.Scale(0.15f, 0.15f, 0.15f);
					meshList[GEO_HORN]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
					meshList[GEO_HORN]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
					meshList[GEO_HORN]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
					meshList[GEO_HORN]->material.kShininess = 2.0f;
					RenderMesh(meshList[GEO_HORN], enableLight);
					modelStack.PopMatrix();
				}
				{
				//Eyes
				modelStack.PushMatrix();
				modelStack.Translate(0.4f, 0.4f, 0.7f);

				modelStack.PushMatrix();
				modelStack.Translate(-0.8f, 0.f, 0.f);

				modelStack.Scale(0.2f, 0.36f, 0.1f);
				meshList[GEO_SPHERE]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
				meshList[GEO_SPHERE]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
				meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
				meshList[GEO_SPHERE]->material.kShininess = 4.0f;
				RenderMesh(meshList[GEO_SPHERE], enableLight);
				modelStack.PopMatrix();

				modelStack.Scale(0.2f, 0.36f, 0.1f);
				meshList[GEO_SPHERE]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
				meshList[GEO_SPHERE]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
				meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
				meshList[GEO_SPHERE]->material.kShininess = 4.0f;
				RenderMesh(meshList[GEO_SPHERE], enableLight);
				modelStack.PopMatrix();
				}
				modelStack.Scale(0.9f, 0.3f, 0.8f);
				meshList[GEO_SPHERE_WHITE]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
				meshList[GEO_SPHERE_WHITE]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
				meshList[GEO_SPHERE_WHITE]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
				meshList[GEO_SPHERE_WHITE]->material.kShininess = 2.0f;
				RenderMesh(meshList[GEO_SPHERE_WHITE], enableLight);
				modelStack.PopMatrix();

				modelStack.Scale(0.9f, 0.3f, 0.8f);
				meshList[GEO_SPHERE_WHITE]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
				meshList[GEO_SPHERE_WHITE]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
				meshList[GEO_SPHERE_WHITE]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
				meshList[GEO_SPHERE_WHITE]->material.kShininess = 2.0f;
				RenderMesh(meshList[GEO_SPHERE_WHITE], enableLight);
				modelStack.PopMatrix();

				modelStack.Scale(0.9f, 1.f, 0.8f);
				meshList[GEO_CYLINDER_WHITE]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
				meshList[GEO_CYLINDER_WHITE]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
				meshList[GEO_CYLINDER_WHITE]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
				meshList[GEO_CYLINDER_WHITE]->material.kShininess = 2.0f;
				RenderMesh(meshList[GEO_CYLINDER_WHITE], enableLight);
				modelStack.PopMatrix();
			}
			modelStack.Scale(0.2f, 0.2f, 0.2f);
			meshList[GEO_CYLINDER]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
			meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
			meshList[GEO_CYLINDER]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
			meshList[GEO_CYLINDER]->material.kShininess = 5.0f;
			RenderMesh(meshList[GEO_CYLINDER], enableLight);
			modelStack.PopMatrix();
		}
		modelStack.PushMatrix();
		//(Left hands)
		{
			//Left shoulder joint
			modelStack.PushMatrix();
			modelStack.Translate(0.8f, 0.7f, 0.f);
			modelStack.Rotate(LRShoulderL, 0.f, 0.f, 1.f);
			modelStack.Rotate(FBShoulderL, 1.f, 0.f, 0.f);
			{
				//arm
				modelStack.PushMatrix();
				modelStack.Translate(0.2f, -0.48f, 0.f);
				{
					//elbow joint
					modelStack.PushMatrix();
					modelStack.Translate(0.123f, -0.34f, 0.f);
					modelStack.Rotate(-10.f, 1.f, 0.f, 0.f);
					modelStack.Rotate(LRElbowL, 0.f, 0.f, 1.f);
					modelStack.Rotate(FBElbowL, 1.f, 0.f, 0.f);
					{
						//forearm
						modelStack.PushMatrix();
						modelStack.Translate(0.09f, -0.44f, 0.f);
						{
							//hand
							modelStack.PushMatrix();
							modelStack.Translate(0.06f, -0.34f, 0.f);
							modelStack.Rotate(FBHandL, 1.f, 0.f, 0.f);
							{
								
						
								modelStack.PushMatrix();
								modelStack.Translate(0.f, -0.1f, 0.25f);
								modelStack.Rotate(10, 1.f, 0.f, 0.f);
								modelStack.Rotate(90, 1.f, 0.f, 0.f);

								modelStack.PushMatrix();
								modelStack.Translate(0.f, 0.f, 0.f);
								modelStack.Rotate(180, 1.f, 0.f, 0.f);

								modelStack.PushMatrix();
								modelStack.Translate(0.f, 0.f, 0.f);
								modelStack.Scale(0.08f, 1.f, 0.08f);
								meshList[GEO_HANDLE]->material.kAmbient.Set(0.3f, 0.3f, 0.3);
								meshList[GEO_HANDLE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
								meshList[GEO_HANDLE]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
								meshList[GEO_HANDLE]->material.kShininess = 5.0f;
								RenderMesh(meshList[GEO_HANDLE], enableLight);
								modelStack.PopMatrix();
								
								meshList[GEO_SWORDPT2]->material.kAmbient.Set(0.3f, 0.3f, 0.3);
								meshList[GEO_SWORDPT2]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
								meshList[GEO_SWORDPT2]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
								meshList[GEO_SWORDPT2]->material.kShininess = 5.0f;
								RenderMesh(meshList[GEO_SWORDPT2], enableLight);
								modelStack.PopMatrix();

								meshList[GEO_SWORD]->material.kAmbient.Set(0.3f, 0.3f, 0.3);
								meshList[GEO_SWORD]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
								meshList[GEO_SWORD]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
								meshList[GEO_SWORD]->material.kShininess = 5.0f;
								RenderMesh(meshList[GEO_SWORD], enableLight);
								modelStack.Scale(1.f, 1.f, 1.f);
								
								modelStack.PopMatrix();
								
								
							}
							modelStack.Scale(0.2f, 0.2f, 0.2f);
							meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
							meshList[GEO_SPHERE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
							meshList[GEO_SPHERE]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
							meshList[GEO_SPHERE]->material.kShininess = 5.0f;
							RenderMesh(meshList[GEO_SPHERE], enableLight);
							modelStack.PopMatrix();
						}
						modelStack.Rotate(10.f, 0.f, 0.f, 1.f);
						modelStack.Scale(0.17f, 0.72f, 0.2f);
						meshList[GEO_CYLINDER]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
						meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
						meshList[GEO_CYLINDER]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
						meshList[GEO_CYLINDER]->material.kShininess = 5.0f;
						RenderMesh(meshList[GEO_CYLINDER], enableLight);
						modelStack.PopMatrix();
					}
					modelStack.Scale(0.28f, 0.28f, 0.28f);
					meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
					meshList[GEO_SPHERE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
					meshList[GEO_SPHERE]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
					meshList[GEO_SPHERE]->material.kShininess = 5.0f;
					RenderMesh(meshList[GEO_SPHERE], enableLight);
					modelStack.PopMatrix();
				}
				modelStack.Rotate(20.f, 0.f, 0.f, 1.f);
				modelStack.Scale(0.2f, 0.8f, 0.22f);
				meshList[GEO_CYLINDER]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
				meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
				meshList[GEO_CYLINDER]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
				meshList[GEO_CYLINDER]->material.kShininess = 5.0f;
				RenderMesh(meshList[GEO_CYLINDER], enableLight);
				modelStack.PopMatrix();
			}
			modelStack.Scale(0.3f, 0.3f, 0.3f);
			meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
			meshList[GEO_SPHERE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
			meshList[GEO_SPHERE]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
			meshList[GEO_SPHERE]->material.kShininess = 5.0f;
			RenderMesh(meshList[GEO_SPHERE], enableLight);
			modelStack.PopMatrix();
		}
		modelStack.PopMatrix();
		modelStack.PushMatrix(); 
		//(Right Hands)
		{
			//right shoulder joint
			modelStack.PushMatrix();
			modelStack.Translate(-0.8f, 0.7f, 0.f);
			modelStack.Rotate(LRShoulderR, 0.f, 0.f, 1.f);
			modelStack.Rotate(FBShoulderR, 1.f, 0.f, 0.f);
			{
				//arm
				modelStack.PushMatrix();
				modelStack.Translate(-0.2f, -0.48f, 0.f);
				{
					//elbow joint
					modelStack.PushMatrix();
					modelStack.Translate(-0.123f, -0.34f, 0.f);
					modelStack.Rotate(-10.f, 1.f, 0.f, 0.f);
					modelStack.Rotate(LRElbowR, 0.f, 0.f, 1.f);
					modelStack.Rotate(FBElbowR, 1.f, 0.f, 0.f);
					{
						//forearm
						modelStack.PushMatrix();
						modelStack.Translate(-0.09f, -0.44f, 0.f);
						{
							//hand
							modelStack.PushMatrix();
							modelStack.Translate(-0.06f, -0.34f, 0.f);
							modelStack.Scale(0.2f, 0.2f, 0.2f);
							meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
							meshList[GEO_SPHERE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
							meshList[GEO_SPHERE]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
							meshList[GEO_SPHERE]->material.kShininess = 5.0f;
							RenderMesh(meshList[GEO_SPHERE], enableLight);
							modelStack.PopMatrix();
						}
						modelStack.Rotate(-10.f, 0.f, 0.f, 1.f);
						modelStack.Scale(0.17f, 0.72f, 0.2f);
						meshList[GEO_CYLINDER]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
						meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
						meshList[GEO_CYLINDER]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
						meshList[GEO_CYLINDER]->material.kShininess = 5.0f;
						RenderMesh(meshList[GEO_CYLINDER], enableLight);
						modelStack.PopMatrix();
					}
					modelStack.Scale(0.28f, 0.28f, 0.28f);
					meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
					meshList[GEO_SPHERE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
					meshList[GEO_SPHERE]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
					meshList[GEO_SPHERE]->material.kShininess = 5.0f;
					RenderMesh(meshList[GEO_SPHERE], enableLight);
					modelStack.PopMatrix();
				}
				modelStack.Rotate(-20.f, 0.f, 0.f, 1.f);
				modelStack.Scale(0.2f, 0.8f, 0.22f);
				meshList[GEO_CYLINDER]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
				meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
				meshList[GEO_CYLINDER]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
				meshList[GEO_CYLINDER]->material.kShininess = 5.0f;
				RenderMesh(meshList[GEO_CYLINDER], enableLight);
				modelStack.PopMatrix();
			}
			modelStack.Scale(0.3f, 0.3f, 0.3f);
			meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
			meshList[GEO_SPHERE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
			meshList[GEO_SPHERE]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
			meshList[GEO_SPHERE]->material.kShininess = 5.0f;
			RenderMesh(meshList[GEO_SPHERE], enableLight);
			modelStack.PopMatrix();
		}
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		//(Left Legs)
		{
			//Left Thigh joint
			modelStack.PushMatrix();
			modelStack.Translate(0.4f, -1.f, 0.f);
			modelStack.Rotate(LRThighL, 0.f, 0.f, 1.f);
			modelStack.Rotate(FBThighL, 1.f, 0.f, 0.f);
			modelStack.Rotate(-FBThighL2, 1.f, 0.f, 0.f);
			modelStack.Rotate(FBThigh, 1.f, 0.f, 0.f);
			{
				//thigh
				modelStack.PushMatrix();
				modelStack.Translate(0.f, -0.55f, 0.f);
				{
					//knee gear nigger
					modelStack.PushMatrix();
					modelStack.Translate(0.f,-0.5f,0.f);
					modelStack.Rotate(LRKneeL, 0.f, 0.f, 1.f);
					modelStack.Rotate(FBKneeL , 1.f, 0.f, 0.f);
					modelStack.Rotate(FBKnee, 1.f, 0.f, 0.f);
					{
						//leg
						modelStack.PushMatrix();
						modelStack.Translate(0.f, -0.5f, 0.f);
						{
							//feet
							modelStack.PushMatrix();
							modelStack.Translate(0.f, -0.35f, 0.f);
							modelStack.Rotate(180.f, 1.f, 0.f, 0.f);
							modelStack.Scale(0.4f, 0.3f, 0.42f);
							meshList[GEO_FEET]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
							meshList[GEO_FEET]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
							meshList[GEO_FEET]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
							meshList[GEO_FEET]->material.kShininess = 5.0f;
							RenderMesh(meshList[GEO_FEET], enableLight);
							modelStack.PopMatrix();
						}
						modelStack.Scale(0.2f, 0.7f, 0.22f);
						meshList[GEO_CYLINDER]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
						meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
						meshList[GEO_CYLINDER]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
						meshList[GEO_CYLINDER]->material.kShininess = 5.0f;
						RenderMesh(meshList[GEO_CYLINDER], enableLight);
						modelStack.PopMatrix();
					}
					modelStack.Scale(0.28f, 0.28f, 0.28f);
					meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
					meshList[GEO_SPHERE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
					meshList[GEO_SPHERE]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
					meshList[GEO_SPHERE]->material.kShininess = 5.0f;
					RenderMesh(meshList[GEO_SPHERE], enableLight);
					modelStack.PopMatrix();
				}
				modelStack.Scale(0.23f, 0.8f, 0.25f);
				meshList[GEO_CYLINDER]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
				meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
				meshList[GEO_CYLINDER]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
				meshList[GEO_CYLINDER]->material.kShininess = 5.0f;
				RenderMesh(meshList[GEO_CYLINDER], enableLight);
				modelStack.PopMatrix();
			}
			modelStack.Scale(0.3f, 0.3f, 0.3f);
			meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
			meshList[GEO_SPHERE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
			meshList[GEO_SPHERE]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
			meshList[GEO_SPHERE]->material.kShininess = 5.0f;
			RenderMesh(meshList[GEO_SPHERE], enableLight);
			modelStack.PopMatrix();
		}
		//(Right Legs)
		{
			//Right Thigh joint
			modelStack.PushMatrix();
			modelStack.Translate(-0.4f, -1.f, 0.f);
			modelStack.Rotate(LRThighR, 0.f, 0.f, 1.f);
			modelStack.Rotate(FBThighR, 1.f, 0.f, 0.f);
			modelStack.Rotate(FBThigh, 1.f, 0.f, 0.f);
			{
				//thigh
				modelStack.PushMatrix();
				modelStack.Translate(0.f, -0.55f, 0.f);
				{
					//knee gear
					modelStack.PushMatrix();
					modelStack.Translate(0.f, -0.5f, 0.f);
					modelStack.Rotate(LRKneeR, 0.f, 0.f, 1.f);
					modelStack.Rotate(FBKneeR, 1.f, 0.f, 0.f);
					modelStack.Rotate(FBKnee, 1.f, 0.f, 0.f);
					{
						//leg
						modelStack.PushMatrix();
						modelStack.Translate(0.f, -0.5f, 0.f);
						{
							//feet
							modelStack.PushMatrix();
							modelStack.Translate(0.f, -0.35f, 0.f);
							modelStack.Rotate(180.f, 1.f, 0.f, 0.f);
							modelStack.Scale(0.4f, 0.3f, 0.42f);
							meshList[GEO_FEET]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
							meshList[GEO_FEET]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
							meshList[GEO_FEET]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
							meshList[GEO_FEET]->material.kShininess = 5.0f;
							RenderMesh(meshList[GEO_FEET], enableLight);
							modelStack.PopMatrix();
						}
						modelStack.Scale(0.2f, 0.7f, 0.22f);
						meshList[GEO_CYLINDER]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
						meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
						meshList[GEO_CYLINDER]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
						meshList[GEO_CYLINDER]->material.kShininess = 5.0f;
						RenderMesh(meshList[GEO_CYLINDER], enableLight);
						modelStack.PopMatrix();
					}
					modelStack.Scale(0.28f, 0.28f, 0.28f);
					meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
					meshList[GEO_SPHERE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
					meshList[GEO_SPHERE]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
					meshList[GEO_SPHERE]->material.kShininess = 5.0f;
					RenderMesh(meshList[GEO_SPHERE], enableLight);
					modelStack.PopMatrix();
				}
				modelStack.Scale(0.23f, 0.8f, 0.25f);
				meshList[GEO_CYLINDER]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
				meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
				meshList[GEO_CYLINDER]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
				meshList[GEO_CYLINDER]->material.kShininess = 5.0f;
				RenderMesh(meshList[GEO_CYLINDER], enableLight);
				modelStack.PopMatrix();
			}
			modelStack.Scale(0.3f, 0.3f, 0.3f);
			meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
			meshList[GEO_SPHERE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
			meshList[GEO_SPHERE]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
			meshList[GEO_SPHERE]->material.kShininess = 5.0f;
			RenderMesh(meshList[GEO_SPHERE], enableLight);
			modelStack.PopMatrix();
		}

		modelStack.PopMatrix();
		modelStack.Scale(0.95f, 1.2f, 0.5f);
		meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_SPHERE]->material.kShininess = 5.0f;
		RenderMesh(meshList[GEO_SPHERE], enableLight);
		modelStack.PopMatrix();

	}
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	// Render light
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	modelStack.Scale(0.1f, 0.1f, 0.1f);
	RenderMesh(meshList[GEO_SPHERE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.f,-3.7f,0.f);
	modelStack.Scale(30.f,1.f,30.f);
	RenderMesh(meshList[GEO_PLAIN], false);
	modelStack.PopMatrix();
}

void SceneMinecraft::Exit()
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

void SceneMinecraft::HandleKeyPress()
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

	if (KeyboardController::GetInstance()->IsKeyPressed('9'))
	{
		currAnim = WAVE;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('8'))
	{
		currAnim = HOP_FORWARD;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('7'))
	{
		currAnim = FRONTFLIP_FORWARDATTACK;
	}


	if (KeyboardController::GetInstance()->IsKeyPressed('6'))
	{
		currAnim = BACKFLIPnFRONTFLIP;
	} 

	if (KeyboardController::GetInstance()->IsKeyPressed('5'))
	{
		CharacterPos.z = -10;
		currAnim = RUN_SLASHnDASH;
	
	} 

	if (KeyboardController::GetInstance()->IsKeyPressed('2'))
	{
		currAnim = SUPERSAIYAN;
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('C')) // activate chainsaws (only when chainsaw)
	{
	
	}
	if (Application::IsKeyPressed('P')) {
		if (projType == 0)
			projType = 1;
		else
			projType = 0;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('R'))
	{
		currAnim = IDLE;
		FBShoulderR = 0;
		LRShoulderR = 0;
		FBShoulderL = 0;
		LRShoulderL = 0;
		FBElbowR = 0;
		LRElbowR = 0;
		FBElbowL = 0;
		LRElbowL = 0;
		FBThighR = 0;
		LRThighR = 0;
		FBThighL = 0;
		LRThighL = 0;
		FBThigh = 0;
		FBKneeR = 0;
		LRKneeR = 0;
		FBKneeL = 0;
		LRKneeL = 0;
		UDHead = 0;
		LFHead = 0;
		direction = 1;
		direction1 = 1;
		direction2 = -1;
		direction3 = 1;
		direction4 = 1;
		direction5 = 1;
		direction6 = 1;
		direction7 = 1;
		direction8 = 1;
		direction9 = 1;
		direction10 = 1;
		direction11 = 1;
		CharacterPos = glm::vec3(0, 0, 0);
		FBTorso = 0;
		RTorso = 0;
		FBTorso2 = 0;
		timeelapsed = 0;
		activate = 0;
		reverse = 0; 
		FBHandL = 0;
		LRHandL = 0;
		FBHandR = 0;
		LRHandR = 0;
		FBKnee = 0;
		flipAnim = 0;
		running = 0;
		BF1 = 0;
		BF2 = 0;
		BF3 = 0;
		Chargeinpos = false;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_ENTER))
	{
		// Only animate the sun
		
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_0))
	{
		// Toggle light on or off
		if (light[0].power < 1)
		{
			light[0].power = 3;
		}
		else
		{
			light[0].power = 0.3;
		}
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	}
}

void SceneMinecraft::RenderMesh(Mesh* mesh, bool enableLight)
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
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE,
			glm::value_ptr(modelView_inverse_transpose));

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
	mesh->Render();
}
