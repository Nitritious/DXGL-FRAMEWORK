#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string& meshName, Color color, float length = 1.f);
	static Mesh* GenerateCircle(const std::string& meshName,Color color,float radius = 1.f,int numSlice = 360);
	static Mesh* GenerateCube(const std::string& meshName, Color color, float length);
	static Mesh* GenerateSphere(const std::string& meshName,
		Color color,
		float radius = 1.f,
		int numSlice = 360,
		int numStack = 360);
	static Mesh* GenerateTorus(const std::string& meshName,Color color,float innerR = 1.f,float outerR = 1.f,int numSlice = 360,int numStack = 360);
	static Mesh* GenerateCylinder(const std::string& meshName, Color color, float height, unsigned sliceCount, float radius);
	static Mesh* GenerateHemisphere(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius);
	static Mesh* GenerateHalfTorus(const std::string& meshName, Color color, float innerR, float outerR, int numSlice, int numStack);
	static Mesh* GenerateCone(const std::string& meshName, Color color, unsigned numSlices, float radius, float height);
	static Mesh* GenerateOBJ(const std::string& meshName, const std::string& file_path);
	static Mesh* GenerateOBJMTL(const std::string& meshName, const std::string& file_path, const std::string& mtl_path);

	static Mesh* GenerateText(const std::string& meshName, unsigned numRow, unsigned numCol);
};

#endif