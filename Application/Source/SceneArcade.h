#ifndef SCENE_ARCADE_H
#define SCENE_ARCADE_H
#define time_scale 5

#include "Scene.h"
#include "Mesh.h"
#include "CameraFPS.h"
#include "MatrixStack.h"
#include "Light.h"
#include "GameObject.h"
#include "CameraOC.h"
//#include "Vector3.h"

class SceneArcade : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_LIGHT,
		GEO_FRONT,
		GEO_BACK,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_BALL,
		GEO_SKEEBALL,
		GEO_SKEEBALL_INTRO_GUI,
		GEO_POWERMETER_GUI,
		GEO_BALL_COUNTER_GUI,
		GEO_ARROW,
		GEO_HIGHSTRIKER,
		GEO_HAMMER,
		GEO_PUCK,
		GEO_HIGHSTRIKER_INTRO_GUI,
		GEO_WALL,
		GEO_GOAL,
		GEO_TEXT,
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

		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_LIGHT2_TYPE,
		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,

		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_LIGHTENABLED,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

	SceneArcade();
	~SceneArcade();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
private:
	void HandleKeyPress();
	void HandleMouseInput();

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderGUI();
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);
	void RenderSkybox();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);

	bool CheckSphereCubeCollision(GameObject* sphere, GameObject* cube);

	void CollisionResponse(GameObject* go1, GameObject* go2);

	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	CameraFPS FPScamera;
	int projType = 1; // fix to 0 for orthographic, 1 for projection

	MatrixStack modelStack, viewStack, projectionStack;

	static const int NUM_LIGHTS = 3;
	Light light[NUM_LIGHTS];
	bool enableLight;

	bool moveCamera;
	bool playPrompt1;
	bool playPrompt2;
	bool playing1;
	bool playing2;
	bool skeeballIntro;
	bool highStrikerIntro;
	bool skeeballOutro;
	bool launch1;
	bool launch2;
	bool pressed;
	bool hit;
	bool retryPrompt;
	bool added;
	bool gameOver;
	bool exitPrompt;

	float m_worldWidth;
	float m_worldHeight;
	float power;
	float angle;
	float arrowAngle;
	float scoreX;
	float scoreY;

	int clicks;
	int timer;
	int height;
	int skeeballScore;
	int balls;

	GameObject* m_skeeball;
	GameObject* m_ball;
	GameObject* m_arrow;
	GameObject* m_highstriker;
	GameObject* m_hammer;
	GameObject* m_puck;
	GameObject* m_wall[5];
	GameObject* m_goal[5];

	glm::vec3 PrevCameraPosition;
	glm::vec3  PrevTargetPosition;
};

#endif