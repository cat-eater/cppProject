#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

extern float a ;			//cube piece side length
extern float e;
extern float c ;

extern float cubeVertices[36*3];

void genCubePositonVecs();
extern glm::vec3 cubePositions[27];

void genCubeModelMat();
void genCubeModelBuffMat();
extern glm::mat4 cubeModelMat[27];
extern glm::mat4 cubeModelBuffMat[27];
class slice {
public:
	int posIndxArr[9];
	glm::vec3 centerAxis;
	slice( int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);
	void turn(int i, float angle, float moveNo);
};


