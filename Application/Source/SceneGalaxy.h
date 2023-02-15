#ifndef SCENE_GALAXY_H
#define SCENE_GALAXY_H

#include "Scene.h"
#include "Mesh.h"
#include "Camera.h"
#include "AltAzCamera.h"
#include "MatrixStack.h"

class SceneGalaxy : public Scene
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

	SceneGalaxy();
	~SceneGalaxy();

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

	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	AltAzCamera camera;
	int projType = 1; // fix to 0 for orthographic, 1 for projection
};

#endif