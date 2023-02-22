#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>
//#include "Vector3.h"

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_SKEEBALL,
		GO_BALL,
		GO_ARROW,
		GO_HIGHSTRIKER,
		GO_HAMMER,
		GO_PUCK,
		GO_WALL,
		GO_CYLINDER,
		GO_TOTAL,
	};
	GAMEOBJECT_TYPE type;
	glm::vec3 pos;
	glm::vec3 vel;
	glm::vec3 scale;
	glm::vec4 rotate;
	bool active;
	float mass;
	glm::vec3 force;
	glm::vec3 temp;
	glm::vec3 acc;
	glm::vec3 dir;
	glm::vec3 frictionalForce;

	float multiplier;
	float distX;
	float distY;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_SKEEBALL);
	~GameObject();
};
#endif

