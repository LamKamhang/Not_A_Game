#pragma once
#include <vector>
#include <glm/glm.hpp>

#define _min(x,y) ((x) < (y) ? (x) : (y))
#define _max(x,y) ((x) < (y) ? (y) : (x))
#define Infinity 100000.0f

// 按照背面，左侧面，右侧面，正面，上面及下面顺序排放, 中心在（0，0，0）
// length指正面的底边，width指侧面的底边
// 朝向面为逆时针，背向面为顺时针点，方便面剔除

std::vector<float> GenCubeBackVertices  (float length, float height, float cx = 0.0, float cy = 0.0, float cz = 0.0);
std::vector<float> GenCubeFrontVertices (float length, float height, float cx = 0.0, float cy = 0.0, float cz = 0.0);
std::vector<float> GenCubeLeftVertices  (float  width, float height, float cx = 0.0, float cy = 0.0, float cz = 0.0);
std::vector<float> GenCubeRightVertices (float  width, float height, float cx = 0.0, float cy = 0.0, float cz = 0.0);
std::vector<float> GenCubeTopVertices   (float length, float  width, float cx = 0.0, float cy = 0.0, float cz = 0.0);
std::vector<float> GenCubeButtomVertices(float length, float  width, float cx = 0.0, float cy = 0.0, float cz = 0.0);
std::vector<float> GenCubeVertices(float length = 1.0, float width = 1.0, float height = 1.0);

// 球体还有问题
std::vector<float> GenSectorVertices(float radius = 1.0, float degree = 360, int cw = false, float x = 0.0, float y = 0.0, float z = 0.0, float tx = 0.0, float ty = 1.0, float tz = 0.0);
std::vector<float> GenSphereVertices(float radius = 1.0);

// 椎体只提供顶点以及法向量，没有纹理坐标
std::vector<float> GenConeVertices(float radius = 1.0, float height = 1.0, float x = 0, float y = 0);

 std::vector<float> GenCylinderVertices(float radius = 1.0, float height = 1.0);

void ExternSphereVertexAttribute(std::vector<float> &vertices, std::vector<float> &position, int index, int totalPhiStep, int totalThetaStep, float texCoordX, float texCoordY);

			