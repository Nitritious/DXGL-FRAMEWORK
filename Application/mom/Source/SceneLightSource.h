#ifndef SCENE_GALAXY_H
#define SCENE_GALAXY_H

#include "Scene.h"
#include "Mesh.h"
#include "Camera.h"
#include "AltAzCamera.h"
#include "MatrixStack.h"
#include "light.h"

class SceneLightSource : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_QUAD,
		GEO_CIRCLE,
		GEO_SPHERE,
		GEO_TORUS,
		GEO_SPHERE_ORANGE,
		GEO_SPHERE_BLUE,
		GEO_SPHERE_GREY,	
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
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHTENABLED,
		U_TOTAL,
	};

	enum ANIMATION
	{
		ANIM_MOON,
		ANIM_EARTH,
		ANIM_SUN,
		NUM_ANIM
	};
	// Keep track of current animation
	ANIMATION currAnim;

	SceneLightSource();
	~SceneLightSource();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	void HandleKeyPress();

	MatrixStack modelStack, viewStack, projectionStack;

	float moonRotation;
	float earthRotation;
	float sunRotation;

	void RenderMesh(Mesh* mesh, bool enableLight);
	Light light[1];
	bool enableLight;

	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	AltAzCamera camera;
	int projType = 1; // fix to 0 for orthographic, 1 for projection
};

#endif