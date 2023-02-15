#ifndef SCENE_1_H
#define SCENE_1_H

#include "Scene.h"
#include "Mesh.h"
#include "Camera.h"

class Scene1 : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_QUAD,
		GEO_AXES,
		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_TOTAL,
	};

	Scene1();
	~Scene1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	void HandleKeyPress();

	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	Camera camera;
	int projType = 1; // fix to 0 for orthographic, 1 for projection
};

#endif