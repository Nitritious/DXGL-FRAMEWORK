#ifndef SCENE_MINECRAFT_H
#define SCENE_MINECRAFT_H

#include "Scene.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "AltAzCamera.h"

class SceneMinecraft : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_QUAD,
		GEO_CIRCLE,
		GEO_SPHERE,
		GEO_TORUS,
		GEO_SPHERE_WHITE,	
		GEO_CUBE,
		GEO_PLAIN,
		GEO_CYLINDER,
		GEO_CYLINDER_WHITE,
		GEO_HEMISPHERE,
		GEO_HORN,
		GEO_TOPHORN,
		GEO_FEET,
		GEO_SWORD,
		GEO_HANDLE,
		GEO_SWORDPT2,
		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		U_TOTAL,
	};

	enum ANIMATION
	{
		RESET,
		IDLE,
		WAVE,
		HOP_FORWARD,
		FRONTFLIP_FORWARDATTACK,
		BACKFLIPnFRONTFLIP,
		RUN_SLASHnDASH,
		SUPERSAIYAN,
		NUM_ANIM
	};
	// Keep track of current animation
	ANIMATION currAnim;
	float delay = 0;

	SceneMinecraft();
	~SceneMinecraft();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	void HandleKeyPress();

	MatrixStack modelStack, viewStack, projectionStack;

	float FBShoulderR, LRShoulderR, FBShoulderL, LRShoulderL;
	float FBElbowR, LRElbowR, FBElbowL, LRElbowL;
	float FBHandL, LRHandL, FBHandR, LRHandR;
	float FBThighR, LRThighR, FBThighL,FBThighL2, LRThighL, FBThigh;
	float FBKneeR, LRKneeR, FBKneeL, LRKneeL, FBKnee;
	float UDHead, LFHead;
	float FBTorso, RTorso, FBTorso2;
	float direction, direction1, direction2, direction3, direction4, direction5, direction6, direction7, direction8, direction9, direction10, direction11;
	glm::vec3 CharacterPos;
	float timeelapsed;
	bool activate, reverse, Chargeinpos, flipAnim, running, BF1,BF2,BF3;

	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	AltAzCamera camera;
	int projType = 1; // fix to 0 for orthographic, 1 for projection

	void RenderMesh(Mesh* mesh, bool enableLight);

	Light light[1];
	bool enableLight;


};

#endif