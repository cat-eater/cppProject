#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cubeState.h"
float a = 0.5f;			//cube piece side length
float e = a / 2;
float c = e + a;
extern bool sideRotating;
extern float HalfPi;
extern char cubeTurnKey;

float cubeVertices[108] = {
    // Right face (X+)
    -e,  e, -e,
    -e, -e, -e,
     e, -e, -e,
     e, -e, -e,
     e,  e, -e,
    -e,  e, -e,

    // Left face (X-)
    -e, -e,  e,
    -e,  e,  e,
     e,  e,  e,
     e,  e,  e,
     e, -e,  e,
    -e, -e,  e,

    // Top face (Y+)
   -e,  e, -e,
   -e,  e,  e,
   -e, -e,  e,
   -e, -e,  e,
   -e, -e, -e,
   -e,  e, -e,

   // Bottom face (Y-)
   e,  e,  e,
   e,  e, -e,
   e, -e, -e,
   e, -e, -e,
   e, -e,  e,
   e,  e,  e,

   // Front face (Z+)
    -e,  e,  e,
    -e,  e, -e,
     e,  e, -e,
     e,  e, -e,
     e,  e,  e,
    -e,  e,  e,

    // Back face (Z-)
     -e, -e, -e,
     -e, -e,  e,
      e, -e,  e,
      e, -e,  e,
      e, -e, -e,
     -e, -e, -e,
};


glm::vec3 cubePositions[27];

glm::mat4 cubeModelMat[27];
glm::mat4 cubeModelBuffMat[27];

float x, y, z;
void genCubePositonVecs()
{
    for (int i = 0;i < 27;i++)
    {
        if (i < 9) x = a;                       //right
        else if (i < 18) x = 0.0f;              
        else x = -a;                             //left

        if (i % 9 < 3) y = a;
        else if (i % 9 < 6) y = 0.0f;
        else y = -a;

        if (i % 3 == 0) z = a;
        else if (i % 3 == 1) z = 0.0f;
        else z = -a;

        cubePositions[i] = glm::vec3(x, y, z);


    };
}

void genCubeModelMat() {
    for (int i = 0;i < 27;i++)
    {
        cubeModelMat[i] = glm::mat4(1.0);
        cubeModelMat[i] = glm::translate(cubeModelMat[i], cubePositions[i]);

    }
}

void genCubeModelBuffMat() {
    for (int i = 0;i < 27;i++)
    {
        cubeModelBuffMat[i] = glm::mat4(1.0);
        cubeModelBuffMat[i] = glm::translate(cubeModelBuffMat[i], cubePositions[i]);

    }
}
/*
          if (i < 9) {
              model = glm::translate(model, -cubePositions[i]);
              model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0, 0.0, 0.0));
              model = glm::translate(model, cubePositions[i]);
              //model = glm::transpose(model);
          }*/

    
slice::slice( int a0,int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8) {
    posIndxArr[0] = a0;
    posIndxArr[1] = a1;
    posIndxArr[2] = a2;
    posIndxArr[3] = a3;
    posIndxArr[4] = a4;
    posIndxArr[5] = a5;
    posIndxArr[6] = a6;
    posIndxArr[7] = a7;
    posIndxArr[8] = a8;
}
    //glm::vec3 centerAxis;

void  slice::turn(int i, float time, float moveNo) {

    float angle = time;
    
    for (int j = 0;j < 9;j++) {
        if (posIndxArr[j] == i  && angle< HalfPi*moveNo+0.01 && angle >  HalfPi*(moveNo-1.0) )//add stoprotaion condition
        {
            cubeModelBuffMat[i] = glm::mat4(1.0);
            cubeModelBuffMat[i] = glm::translate(cubeModelMat[i], -cubePositions[i]);
            cubeModelBuffMat[i] = glm::rotate(cubeModelBuffMat[i], angle, centerAxis);
            cubeModelBuffMat[i] = glm::translate(cubeModelBuffMat[i], cubePositions[i]);
            //model = glm::translate(model, cubePositions[i]);
            if (angle >= HalfPi *moveNo) {
                
                for (int k :posIndxArr) {
                    cubeModelBuffMat[k] = glm::mat4(1.0);
                    //cubeModelMat[k]= glm::translate(cubeModelMat[k], cubePositions[k]);
                    cubeModelBuffMat[k] = cubeModelMat[k];
                    cubeModelMat[k] = glm::rotate(cubeModelMat[k],HalfPi, centerAxis);
                    //cubeModelMat[k] = glm::translate(cubeModelMat[k], -cubePositions[k]);
                   
                }
                //stopRotaion = true;
            }
        }
    }
    
}
    /*
std::vector<slice> genCubeSlices() {

int arrT[] = { 0,1,2,9,10,11,18,19,20 };
slice top(arrT);

int arrB[] = {6,7,8,15,16,17,24,25,26 };
slice bottom(arrB);

top.centerAxis = glm::vec3(0.0, 1.0, 0.0);
bottom.centerAxis = glm::vec3(0.0, 1.0, 0.0);



int arrR[] = { 0,1,2,3,4,5,6,7,8 };
slice right(arrR);

int arrL[] = { 18,19,20,22,23,24,25,26 };
slice left = (arrL);

right.centerAxis = glm::vec3( 1.0, 0.0, 0.0);
left.centerAxis = glm::vec3(1.0, 0.0, 0.0);



int arrF[] = { 6,3,0,15,12,9,24,21,18};
slice front(arrF);

int arrBA[] = { 2,5,8,11,14,17,20,23,26 };
slice back = (arrBA);

front.centerAxis = glm::vec3(0.0, 0.0, 1.0);
back.centerAxis = glm::vec3(0.0, 0.0, 1.0);

std::vector<slice> sliceVec = std::vector<slice>{ top,bottom,right,left,front,back };

return sliceVec;
}

std::vector<slice> cubeInit() {
    genCubePositonVecs();
    genCubeModelMat();
    std::vector<slice> genCubeSlices();
    return genCubeSlices();
}*/