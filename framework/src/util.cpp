#include "Header/util.h"

// 按照背面，正面，左面，右面，顶面，底面顺序排放
std::vector<float> GenCubeBackVertices(float length, float height, float cx, float cy, float cz)
{
    return std::vector<float>{
        // back（cannot see, cw)
         cx +  length/2,  cy + -height/2,  cz,     0, 0, 1,        0,       0,   // G
         cx + -length/2,  cy + -height/2,  cz,     0, 0, 1,        length,  0,   // F
         cx + -length/2,  cy +  height/2,  cz,     0, 0, 1,        length,  height,   // E
         cx + -length/2,  cy +  height/2,  cz,     0, 0, 1,        length,  height,   // E
         cx +  length/2,  cy +  height/2,  cz,     0, 0, 1,        0,       height,   // H
         cx +  length/2,  cy + -height/2,  cz,     0, 0, 1,        0,       0,   // G
    };
}
std::vector<float> GenCubeFrontVertices(float length, float height, float cx, float cy, float cz)
{
    return std::vector<float>{
        // front(can see, ccw)
         cx + -length/2,  cy +  height/2,  cz,     0, 0, -1,       0,       height,   // A
         cx + -length/2,  cy + -height/2,  cz,     0, 0, -1,       0,       0,   // B
         cx +  length/2,  cy + -height/2,  cz,     0, 0, -1,       length,  0,   // C
         cx +  length/2,  cy + -height/2,  cz,     0, 0, -1,       length,  0,   // C
         cx +  length/2,  cy +  height/2,  cz,     0, 0, -1,       length,  height,   // D
         cx + -length/2,  cy +  height/2,  cz,     0, 0, -1,       0,       height,   // A
    };
}
std::vector<float> GenCubeLeftVertices(float width, float height, float cx, float cy, float cz)
{
    return std::vector<float>{
     // left(cannot see, cw)
        cx,  cy +  height/2,  cz +   width/2,     -1, 0, 0,       width,    height,   // A
        cx,  cy +  height/2,  cz +  -width/2,     -1, 0, 0,       0,        height,   // E
        cx,  cy + -height/2,  cz +  -width/2,     -1, 0, 0,       0,        0,   // F
        cx,  cy + -height/2,  cz +  -width/2,     -1, 0, 0,       0,        0,   // F
        cx,  cy + -height/2,  cz +   width/2,     -1, 0, 0,       width,    0,   // B
        cx,  cy +  height/2,  cz +   width/2,     -1, 0, 0,       width,    height,   // A
    };
}
std::vector<float> GenCubeRightVertices(float width, float height, float cx, float cy, float cz)
{
    return std::vector<float>{
        // right(can see, ccw)
         cx,  cy +  height/2,  cz +   width/2,     1, 0, 0,        0, height,    // D
         cx,  cy + -height/2,  cz +   width/2,     1, 0, 0,        0, 0,    // C
         cx,  cy + -height/2,  cz +  -width/2,     1, 0, 0,        width, 0,    // G
         cx,  cy + -height/2,  cz +  -width/2,     1, 0, 0,        width, 0,    // G
         cx,  cy +  height/2,  cz +  -width/2,     1, 0, 0,        width, height,    // H
         cx,  cy +  height/2,  cz +   width/2,     1, 0, 0,        0, height,    // D
    };
}
std::vector<float> GenCubeTopVertices(float length, float width, float cx, float cy, float cz)
{
    return std::vector<float>{
        // top(can see, ccw)
        cx + -length/2,  cy,  cz +  -width/2,     0, 1, 0,        0, width,    // E
        cx + -length/2,  cy,  cz +   width/2,     0, 1, 0,        0, 0,    // A
        cx +  length/2,  cy,  cz +   width/2,     0, 1, 0,        length, 0,    // D
        cx +  length/2,  cy,  cz +   width/2,     0, 1, 0,        length, 0,    // D
        cx +  length/2,  cy,  cz +  -width/2,     0, 1, 0,        length, width,    // H
        cx + -length/2,  cy,  cz +  -width/2,     0, 1, 0,        0, width,    // E
    };
}
std::vector<float> GenCubeButtomVertices(float length, float width, float cx, float cy, float cz)
{
    return std::vector<float>{
        // buttom(cannot see, cw)
        cx + -length/2, cy,  cz +  -width/2,     0, -1, 0,       0, 0,    // F
        cx +  length/2, cy,  cz +  -width/2,     0, -1, 0,       length, 0,    // G
        cx +  length/2, cy,  cz +   width/2,     0, -1, 0,       length, width,    // C
        cx +  length/2, cy,  cz +   width/2,     0, -1, 0,       length, width,    // C
        cx + -length/2, cy,  cz +   width/2,     0, -1, 0,       0, width,    // B
        cx + -length/2, cy,  cz +  -width/2,     0, -1, 0,       0, 0,    // F
    };
}
std::vector<float> GenCubeVertices(float length, float width, float height)
{
    return std::vector<float> {        
        // position                      // normal      // texCoord
        // x    y   z  (0,0)-->button_left, (0,1)-->top_left, (1,0)-->button_right, (1,1)-->top_right
        // back（cannot see, cw)
         length/2, -height/2, -width/2,     0, 0, 1,        0, 0,   // G
        -length/2, -height/2, -width/2,     0, 0, 1,        1, 0,   // F
        -length/2,  height/2, -width/2,     0, 0, 1,        1, 1,   // E
        -length/2,  height/2, -width/2,     0, 0, 1,        1, 1,   // E
         length/2,  height/2, -width/2,     0, 0, 1,        0, 1,   // H
         length/2, -height/2, -width/2,     0, 0, 1,        0, 0,   // G

        // front(can see, ccw)
        -length/2,  height/2,  width/2,     0, 0, -1,       0, 1,   // A
        -length/2, -height/2,  width/2,     0, 0, -1,       0, 0,   // B
         length/2, -height/2,  width/2,     0, 0, -1,       1, 0,   // C
         length/2, -height/2,  width/2,     0, 0, -1,       1, 0,   // C
         length/2,  height/2,  width/2,     0, 0, -1,       1, 1,   // D
        -length/2,  height/2,  width/2,     0, 0, -1,       0, 1,   // A

        // left(cannot see, cw)
        -length/2,  height/2,  width/2,     -1, 0, 0,       1, 1,   // A
        -length/2,  height/2, -width/2,     -1, 0, 0,       0, 1,   // E
        -length/2, -height/2, -width/2,     -1, 0, 0,       0, 0,   // F
        -length/2, -height/2, -width/2,     -1, 0, 0,       0, 0,   // F
        -length/2, -height/2,  width/2,     -1, 0, 0,       1, 0,   // B
        -length/2,  height/2,  width/2,     -1, 0, 0,       1, 1,   // A

        // right(can see, ccw)
         length/2,  height/2,  width/2,     1, 0, 0,        0, 1,    // D
         length/2, -height/2,  width/2,     1, 0, 0,        0, 0,    // C
         length/2, -height/2, -width/2,     1, 0, 0,        1, 0,    // G
         length/2, -height/2, -width/2,     1, 0, 0,        1, 0,    // G
         length/2,  height/2, -width/2,     1, 0, 0,        1, 1,    // H
         length/2,  height/2,  width/2,     1, 0, 0,        0, 1,    // D

        // top(can see, ccw)
        -length/2,  height/2, -width/2,     0, 1, 0,        0, 1,    // E
        -length/2,  height/2,  width/2,     0, 1, 0,        0, 0,    // A
         length/2,  height/2,  width/2,     0, 1, 0,        1, 0,    // D
         length/2,  height/2,  width/2,     0, 1, 0,        1, 0,    // D
         length/2,  height/2, -width/2,     0, 1, 0,        1, 1,    // H
        -length/2,  height/2, -width/2,     0, 1, 0,        0, 1,    // E

        // buttom(cannot see, cw)
        -length/2, -height/2, -width/2,     0, -1, 0,       0, 0,    // F
         length/2, -height/2, -width/2,     0, -1, 0,       1, 0,    // G
         length/2, -height/2,  width/2,     0, -1, 0,       1, 1,    // C
         length/2, -height/2,  width/2,     0, -1, 0,       1, 1,    // C
        -length/2, -height/2,  width/2,     0, -1, 0,       0, 1,    // B
        -length/2, -height/2, -width/2,     0, -1, 0,       0, 0,    // F
    };
}

std::vector<float> GenSectorVertices(float radius, float degree, int cw, float x, float y, float z, float tx, float ty, float tz)
{    
    std::vector<float> vertices; 
    std::vector<float> position;
    
    float c_x, c_y, c_z;
    int angle_setp = 6;
    for (float theta = 0; theta <= degree; theta += angle_setp)
    {
        c_x = x + radius * glm::cos(glm::radians(theta));
        c_z = z + radius * glm::sin(glm::radians(theta));
        c_y = y;
        position.push_back(c_x);
        position.push_back(c_y);
        position.push_back(c_z);
    }

    int size = position.size();
    if (cw)
    {
        for (int i = 0; i + 3 < size; i += 3)
        {
            vertices.push_back(position[i]);
            vertices.push_back(position[i + 1]);
            vertices.push_back(position[i + 2]);
            vertices.push_back(tx);
            vertices.push_back(ty);
            vertices.push_back(tz);

            vertices.push_back(position[i + 3]);
            vertices.push_back(position[i + 4]);
            vertices.push_back(position[i + 5]);
            vertices.push_back(tx);
            vertices.push_back(ty);
            vertices.push_back(tz);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(tx);
            vertices.push_back(ty);
            vertices.push_back(tz);
        }
    }
    else
    {
        for (int i = 0; i + 3 < size; i += 3)
        {
            vertices.push_back(position[i]);
            vertices.push_back(position[i + 1]);
            vertices.push_back(position[i + 2]);
            vertices.push_back(tx);
            vertices.push_back(ty);
            vertices.push_back(tz);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(tx);
            vertices.push_back(ty);
            vertices.push_back(tz);

            vertices.push_back(position[i + 3]);
            vertices.push_back(position[i + 4]);
            vertices.push_back(position[i + 5]);
            vertices.push_back(tx);
            vertices.push_back(ty);
            vertices.push_back(tz);
        }
    }
    return vertices;
}

std::vector<float> GenSphereVertices(float radius)
{
    std::vector<float> vertices;

    std::vector<float> position;
    
    float c_x, c_y, c_z;
    float angle_setp = 6;
    for (float phi = -90; phi <= 90; phi += angle_setp)
    {
        for (float theta = 0; theta < 360; theta += angle_setp)
        {
            c_x = radius * glm::cos(glm::radians(phi)) * glm::cos(glm::radians(theta));
            c_y =  radius * glm::sin(glm::radians(phi));
			c_z = radius * glm::cos(glm::radians(phi)) * glm::sin(glm::radians(theta));
            position.push_back(c_x);
			position.push_back(c_y);
			position.push_back(c_z);
        }
    }

    // extern vertex attributes
    int row = 180 / angle_setp + 1;
	int col = 360 / angle_setp;
	int index;
	for (int i = 1; i < row - 1; i++)
	{
		for (int j = 0; j < col; j++)
		{
			index = i * col + j;
			ExternSphereVertexAttribute(vertices, position, index + col, col, row, j, i + 1);
			index++;
			if (j == col - 1)
				index -= col;
			ExternSphereVertexAttribute(vertices, position, index, col, row, j + 1, i);
			ExternSphereVertexAttribute(vertices, position, index - 1, col, row, j, i);				
		}
		for (int j = 0; j < col; j++)
		{
			index = i * col + j;
			ExternSphereVertexAttribute(vertices, position, index - col, col, row, j, i - 1);				
			index--;
			if (j == 0)
				index += col;
			ExternSphereVertexAttribute(vertices, position, index, col, row, j - 1, i);
			ExternSphereVertexAttribute(vertices, position, index + 1, col, row, j, i);
		}
	}

    return vertices;
}

std::vector<float> GenConeVertices(float radius, float height, float x, float z)
{
    std::vector<float> vertices;

    std::vector<float> position;
    
    float c_x, c_y, c_z;
    int angle_setp = 6;
    for (float theta = 0; theta <= 360; theta += angle_setp)
    {
        c_x = radius * glm::cos(glm::radians(theta));
        c_z = radius * glm::sin(glm::radians(theta));
        c_y = 0;
        position.push_back(c_x);
        position.push_back(c_y);
        position.push_back(c_z);
    }

    int size = position.size();
    // 底面的圆(cannot see, cw)
    for (int i = 0; i + 3< size; i += 3)
    {
        vertices.push_back(position[i]);
        vertices.push_back(position[i + 1]);
        vertices.push_back(position[i + 2]);
        vertices.push_back(0);
        vertices.push_back(-1);
        vertices.push_back(0);

        vertices.push_back(position[i + 3]);
        vertices.push_back(position[i + 4]);
        vertices.push_back(position[i + 5]);
        vertices.push_back(0);
        vertices.push_back(-1);
        vertices.push_back(0);

        vertices.push_back(0);
        vertices.push_back(0);
        vertices.push_back(0);
        vertices.push_back(0);
        vertices.push_back(-1);
        vertices.push_back(0);
    }

    // 侧面(can see ccw)
    for (int i = 0; i + 3 < size; i += 3)
    {
        glm::vec3 base(position[i+3] - position[i],
                    position[i+4] - position[i+1],
                    position[i+5] - position[i+2]);
        glm::vec3 waist(x - position[i],
                    height - position[i+1],
                    z - position[i+2]);
        glm::vec3 norm = glm::normalize(glm::cross(waist, base));

        vertices.push_back(position[i]);
        vertices.push_back(position[i + 1]);
        vertices.push_back(position[i + 2]);
        vertices.push_back(norm.x);
        vertices.push_back(norm.y);
        vertices.push_back(norm.z);

        vertices.push_back(x);
        vertices.push_back(height);
        vertices.push_back(z);
        vertices.push_back(norm.x);
        vertices.push_back(norm.y);
        vertices.push_back(norm.z);

        vertices.push_back(position[i + 3]);
        vertices.push_back(position[i + 4]);
        vertices.push_back(position[i + 5]);
        vertices.push_back(norm.x);
        vertices.push_back(norm.y);
        vertices.push_back(norm.z);
    }

    return vertices;
}

std::vector<float> GenCylinderVertices(float radius, float height)
{
    std::vector<float> vertices;

    std::vector<float> position;
    
    float c_x, c_y, c_z;
    int angle_setp = 6;
    for (float theta = 0; theta <= 360; theta += angle_setp)
    {
        c_x = radius * glm::cos(glm::radians(theta));
        c_z = radius * glm::sin(glm::radians(theta));
        c_y = 0;
        position.push_back(c_x);
        position.push_back(c_y);
        position.push_back(c_z);
    }

    int size = position.size();

    // 底面和顶面的圆(cannot see, cw)
    for (int i = 0; i + 3< size; i += 3)
    {
        // buttom
        vertices.push_back(position[i]);
        vertices.push_back(position[i + 1]);
        vertices.push_back(position[i + 2]);
        vertices.push_back(0);
        vertices.push_back(-1);
        vertices.push_back(0);

        vertices.push_back(position[i + 3]);
        vertices.push_back(position[i + 4]);
        vertices.push_back(position[i + 5]);
        vertices.push_back(0);
        vertices.push_back(-1);
        vertices.push_back(0);

        vertices.push_back(0);
        vertices.push_back(0);
        vertices.push_back(0);
        vertices.push_back(0);
        vertices.push_back(-1);
        vertices.push_back(0);

        // top
        vertices.push_back(position[i]);
        vertices.push_back(height);
        vertices.push_back(position[i + 2]);
        vertices.push_back(0);
        vertices.push_back(1);
        vertices.push_back(0);

        vertices.push_back(0);
        vertices.push_back(height);
        vertices.push_back(0);
        vertices.push_back(0);
        vertices.push_back(1);
        vertices.push_back(0);

        vertices.push_back(position[i + 3]);
        vertices.push_back(height);
        vertices.push_back(position[i + 5]);
        vertices.push_back(0);
        vertices.push_back(1);
        vertices.push_back(0);
    }
    
    // 侧面
    for (int i = 0; i + 3 < size; i += 3)
    {
        float nx = (position[i] + position[i+3])/2;
        float ny = 0;
        float nz = (position[i+2] + position[i+5])/2;

        vertices.push_back(position[i]);
        vertices.push_back(position[i + 1]);
        vertices.push_back(position[i + 2]);
        vertices.push_back(nx);
        vertices.push_back(ny);
        vertices.push_back(nz);

        vertices.push_back(position[i]);
        vertices.push_back(height);
        vertices.push_back(position[i + 2]);
        vertices.push_back(nx);
        vertices.push_back(ny);
        vertices.push_back(nz);

        vertices.push_back(position[i + 3]);
        vertices.push_back(position[i + 4]);
        vertices.push_back(position[i + 5]);
        vertices.push_back(nx);
        vertices.push_back(ny);
        vertices.push_back(nz);

        vertices.push_back(position[i + 3]);
        vertices.push_back(position[i + 4]);
        vertices.push_back(position[i + 5]);
        vertices.push_back(nx);
        vertices.push_back(ny);
        vertices.push_back(nz);

        vertices.push_back(position[i]);
        vertices.push_back(height);
        vertices.push_back(position[i + 2]);
        vertices.push_back(nx);
        vertices.push_back(ny);
        vertices.push_back(nz);

        vertices.push_back(position[i + 3]);
        vertices.push_back(height);
        vertices.push_back(position[i + 5]);
        vertices.push_back(nx);
        vertices.push_back(ny);
        vertices.push_back(nz);
    }
    return vertices;
}

void ExternSphereVertexAttribute(std::vector<float> &vertices, std::vector<float> &position, int index, int totalPhiStep, int totalThetaStep, float texCoordX, float texCoordY)
{
	glm::vec3 normal = glm::normalize(glm::vec3(position[index * 3], position[index * 3 + 1], position[index * 3 + 2]));
	vertices.push_back(position[index * 3]);
	vertices.push_back(position[index * 3 + 1]);
	vertices.push_back(position[index * 3 + 2]);
	vertices.push_back(normal.x);
	vertices.push_back(normal.y);
	vertices.push_back(normal.z);
	vertices.push_back(texCoordX / totalPhiStep);
	vertices.push_back(texCoordY / totalThetaStep);
}
