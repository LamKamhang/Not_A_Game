#include "Header/Room.h"
using namespace RoomRule;

Room::Room(Camera &camera):
    directory("Resource/Texture")
{
    initRoom(camera);
    bindVAO();
}

Room::Room(const std::vector<float> &_wall, const std::vector<float> &_floor, 
        const Material &wall_material, const Material &floor_material, const Material &ceil_material,
        const std::vector<glm::vec3> &point_light_pos, const PointLight &point_light):
        wall_vertices(_wall), floor_vertices(_floor), point_light_pos(point_light_pos), 
        wall_material(wall_material), floor_material(floor_material), ceil_material(ceil_material),
         point_light(point_light), 
        directory("Resource/Texture")
{
    bindVAO();
}

void Room::Draw(Shader &materialShader)
{
    // set up point light
    setPointLight(materialShader, "point_light", point_light);
    setPointLightPos(materialShader, "point_light_pos");

    // first draw the floor
    materialShader.setBool("floor_wall", 0);
    // set floor_material
    setMaterial(materialShader, "floor_material", floor_material);
    // set ceil_material
    setMaterial(materialShader, "ceil_material", ceil_material);
    glBindVertexArray(floor_VAO);
        glDrawArrays(GL_TRIANGLES, 0, floor_vertices.size()>>3);
    
    // second, draw the wall
    materialShader.setBool("floor_wall", 1);
    // set wall_material
    setMaterial(materialShader, "wall_material", wall_material);
    glBindVertexArray(wall_VAO);
        glDrawArrays(GL_TRIANGLES, 0, wall_vertices.size()>>3);
    
    // unbind the VAO
    glBindVertexArray(0);
}

inline void Room::setMaterial(Shader &shader, const std::string &name, Material &value)
{
    shader.setVec3(name + ".ambient", value.ambient);
    shader.setVec3(name + ".diffuse", value.diffuse);
    shader.setVec3(name + ".specular", value.specular);
    shader.setFloat(name + ".shininess", value.shininess);
}

inline void Room::setPointLight(Shader &shader, const std::string &name, PointLight &value)
{
    shader.setVec3(name + ".color", value.color);
    shader.setFloat(name + ".ambient", value.ambient);
    shader.setFloat(name + ".diffuse", value.diffuse);
    shader.setFloat(name + ".specular", value.specular);
}

inline void Room::setPointLightPos(Shader &shader, const std::string &name)
{
    std::string num;
    for (size_t i = 0; i < point_light_pos.size(); ++i)
    {
        num = std::to_string(i);
        shader.setVec3(name + "[" + num + "]", point_light_pos[i]);
    }
}

inline const std::vector<glm::vec3>& Room::GetLightPos()
{
    return point_light_pos;
}

inline void Room::bindVAO()
{
    glGenVertexArrays(1, &floor_VAO);
	glBindVertexArray(floor_VAO);
		glGenBuffers(1, &floor_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, floor_VBO);
			glBufferData(GL_ARRAY_BUFFER, floor_vertices.size() * sizeof(float), &floor_vertices[0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(0));
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *)(6*sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

    glGenVertexArrays(1, &wall_VAO);
	glBindVertexArray(wall_VAO);
		glGenBuffers(1, &wall_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, wall_VBO);
			glBufferData(GL_ARRAY_BUFFER, wall_vertices.size() * sizeof(float), &wall_vertices[0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(0));
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *)(6*sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

inline void Room::initRoom(Camera &camera)
{
    floor_vertices = GetFirstFloorDefaultGround(camera);
    wall_vertices = GetFirstFloorDefaultWall(camera);
    point_light_pos = GetFirstFloorDefaultLightPos();
    point_light = GetFirstFloorDefaultPointLight();
    wall_material = GetFirstFloorDefaultWallMaterial();
    floor_material = GetFirstFloorDefaultFloorMaterial();
    ceil_material = GetFirstFloorDefaultCeilMaterial();
}

//|||||||||||||||||||||||||||||||||||||||||||||||||||||
// public function
void GetVertexByRules(std::vector<float> &vertices, Camera &camera, const std::vector<Rule> &rules)
{
    auto size = rules.size();
    std::vector<float> temp_wall;
    float p1, p2, cx, cy, cz;
    for (size_t i = 0; i < size; ++i)
	{
        p1 = rules[i].parameters[0];
        p2 = rules[i].parameters[1];
        cx = rules[i].parameters[2];
        cy = rules[i].parameters[3];
        cz = rules[i].parameters[4];
		switch(rules[i].type)
		{
			case _back: 
				temp_wall = GenCubeBackVertices(p1, p2, cx, cy, cz);
				//camera.SetinnerBound(glm::vec3(cx-p1/2, cy-p2/2, cz), 
				//				    glm::vec3(cx+p1/2, cy+p2/2, cz));
				vertices.insert(vertices.end(), temp_wall.begin(), temp_wall.end());
				break;
			case _left:
				temp_wall = GenCubeLeftVertices(p1, p2, cx, cy, cz);
				//camera.SetinnerBound(glm::vec3(cx, cy-p2/2, cz-p1/2), 
				//				    glm::vec3(cx, cy+p2/2, cz+p1/2));
				vertices.insert(vertices.end(), temp_wall.begin(), temp_wall.end());
				break;
			case _front:
				temp_wall = GenCubeFrontVertices(p1, p2, cx, cy, cz);
				//camera.SetinnerBound(glm::vec3(cx-p1/2, cy-p2/2, cz), 
				//				    glm::vec3(cx+p1/2, cy+p2/2, cz));
				vertices.insert(vertices.end(), temp_wall.begin(), temp_wall.end());
				break;
			case _right:
				temp_wall = GenCubeRightVertices(p1, p2, cx, cy, cz);
				//camera.SetinnerBound(glm::vec3(cx, cy-p2/2, cz-p1/2), 
				//				    glm::vec3(cx, cy+p2/2, cz+p1/2));
				vertices.insert(vertices.end(), temp_wall.begin(), temp_wall.end());
				break;
            case _buttom:
                temp_wall = GenCubeButtomVertices(p1, p2, cx, cy, cz); 
                //camera.SetinnerBound(glm::vec3(cx-p1/2, cy, cz-p2/2),
                //                    glm::vec3(cx+p1/2, cy, cz+p2/2));
                vertices.insert(vertices.end(), temp_wall.begin(), temp_wall.end());
                break;
            case _top:
                temp_wall = GenCubeTopVertices(p1, p2, cx, cy, cz); 
                //camera.SetinnerBound(glm::vec3(cx-p1/2, cy, cz-p2/2),
                //                    glm::vec3(cx+p1/2, cy, cz+p2/2));
                vertices.insert(vertices.end(), temp_wall.begin(), temp_wall.end());
                break;
			default:break;
		}
	}
}
//||||||||||||||||||||||||||||||||||||||||||||||||||
// first floor
std::vector<float> GetFirstFloorDefaultGround(Camera &camera)
{
    float cy = 0, ce = 10;
    std::vector<float> vertices;
    std::vector<Rule> rules{
        Rule(_buttom, 15, 25, 7.5, cy, 12.5),//1
        Rule(_buttom, 25, 75, 15+12.5, cy, 37.5),//2
        Rule(_buttom, 40, 60, 40+20, cy, 15+30),//3
        Rule(_buttom, 10, 45, 80+5, cy, 15+22.5),//4

        //ceil
        Rule(_buttom, 15, 25, 7.5, ce, 12.5),//1
        Rule(_buttom, 25, 75, 15+12.5, ce, 37.5),//2
        Rule(_buttom, 40, 60, 40+20, ce, 15+30),//3
        Rule(_buttom, 10, 45, 80+5, ce, 15+22.5),//4
    };
    GetVertexByRules(vertices, camera, rules);
    return vertices;
}

std::vector<float> GetFirstFloorDefaultWall(Camera &camera)
{
    
    float wall_height = 10, door_height = 6, wall_width = 1.5, cy = wall_height/2;
    std::vector<float> vertices;
    std::vector<Rule> rules{
       Rule(_back, 15, wall_height, 7.5, cy, 25),//1
       Rule(_back, wall_width, wall_height,  15+wall_width/2, cy, 10),//2
       Rule(_back, 25, wall_height,  15+12.5, cy, 40),//3
       Rule(_back, 28, wall_height,  15+14, cy, 60),//4
       Rule(_back, 65, wall_height,  15+32.5, cy, 75),//5
       Rule(_back, 18, wall_height,  65-9, cy, 60),//6
       Rule(_back, 3, wall_height,  40+1.5, cy, 60-wall_width),//7
       Rule(_back, 3, wall_height,  47+1.5, cy, 60-wall_width),//8
       Rule(_back, 17, wall_height,  90-8.5, cy, 60),//9
       Rule(_back, 4, wall_height,  65+2, cy, 60-wall_width),//10
       Rule(_back, 17, wall_height,  90-8.5, cy, 60-wall_width),//11
       Rule(_back, 4, wall_height,  65+2, cy, 60-3-wall_width),//12
       Rule(_back, 7, wall_height,  73+3.5, cy, 60-3-wall_width),//13
       Rule(_back, 4, wall_height,  65+2, cy, 60-3-2*wall_width),//14
       Rule(_back, 7, wall_height,  73+3.5, cy, 60-3-2*wall_width),//15
       Rule(_back, wall_width, wall_height,  40-wall_width/2, cy, 55),//16
       Rule(_back, wall_width, wall_height,  40-wall_width/2, cy, 52),//17
       Rule(_back, 6, wall_height,  50+3, cy, 40+wall_width),//18
       Rule(_back, 6, wall_height,  65-3, cy, 40+wall_width),//19
       Rule(_back, 6, wall_height,  50+3, cy, 40),//20
       Rule(_back, 31, wall_height,  90-15.5, cy, 40),//21
       Rule(_back, wall_width, wall_height,  65+wall_width/2, cy, 30),//22
       Rule(_back, 50, wall_height,  40+25, cy, 15),//23
       Rule(_back, 31, wall_height,  9+15.5, cy, wall_width),//24
       Rule(_back, 31, wall_height,  9+15.5, cy, 0),//25
       Rule(_back, 6, wall_height,  3, cy, wall_width),//26
       Rule(_back, 6, wall_height,  3, cy, 0),//27
       Rule(_back, wall_width, wall_height, 15+wall_width/2, cy, 7),//28

       Rule(_left, 25, wall_height,  0, cy, 12.5),//1
       Rule(_left, wall_width, wall_height,  6, cy, wall_width/2),//2
       Rule(_left, wall_width, wall_height,  9, cy, wall_width/2),//3
       Rule(_left, 65, wall_height,  15, cy, 10+32.5),//4
       Rule(_left, 7, wall_height,  15, cy, 3.5),//5
       Rule(_left, 30, wall_height,  15+wall_width, cy, 10+15),//6
       Rule(_left, 7, wall_height,  15+wall_width, cy, 3.5),//7
       Rule(_left, 5, wall_height,  40-wall_width, cy, 60-2.5),//8
       Rule(_left, 12, wall_height,  40-wall_width, cy, 40+6),//9
       Rule(_left, 5, wall_height,  40, cy, 60-2.5),//10
       Rule(_left, 12, wall_height,  40, cy, 40+6),//11
       Rule(_left, 15, wall_height,  40, cy, 7.5),//12
       Rule(_left, wall_width, wall_height,  43, cy, 60-wall_width/2),//13
       Rule(_left, wall_width, wall_height,  47, cy, 60-wall_width/2),//14
       Rule(_left, 20, wall_height,  50, cy, 40+10),//15
       Rule(_left, wall_width, wall_height,  56, cy, 40+wall_width/2),//16
       Rule(_left, wall_width, wall_height,  59, cy, 40+wall_width/2),//17
       Rule(_left, 15, wall_height,  65, cy, 15+7.5),//18
       Rule(_left, 15, wall_height,  65+wall_width, cy, 15+7.5),//19
       Rule(_left, 17-wall_width, wall_height,  65, cy, 40+(17-wall_width)/2),//20
       Rule(_left, wall_width, wall_height,  69, cy, 60-wall_width-3-wall_width/2),//21
       Rule(_left, wall_width, wall_height,  73, cy, 60-wall_width-3-wall_width/2),//22
       Rule(_left, wall_width, wall_height,  69, cy, 60-wall_width/2),//23
       Rule(_left, wall_width, wall_height,  73, cy, 60-wall_width/2),//24
       Rule(_left, 15+wall_width, wall_height,  65, cy, 75-(15+wall_width)/2),//25
       Rule(_left, 15, wall_height,  80, cy, 75-7.5),//26
       Rule(_left, 17-wall_width, wall_height,  80, cy, 40+(17-wall_width)/2),//27
       Rule(_left, 45, wall_height,  90, cy, 15+22.5),//28
    };
    GetVertexByRules(vertices, camera, rules);
    return vertices;
}

std::vector<glm::vec3> GetFirstFloorDefaultLightPos()
{
    float wall_height = 10, delta = -0.5;
    float y = wall_height-delta;
    return std::vector<glm::vec3>{
        glm::vec3(7.5, y, 20),//1
        glm::vec3(40, y, 25),//2
        glm::vec3(20, y, 50),//3
        glm::vec3(40, y, 67.5),//4
        glm::vec3(57.5, y, 55),//5
        glm::vec3(72.5, y, 68),//6
        glm::vec3(73, y, 50),//7
        glm::vec3(75, y, 25),//8
        glm::vec3(85, y, 60),//9
    };
}

/*
struct PointLight{
	glm::vec3 color;
    float ambient;
	float diffuse;
	float specular;
};
*/
PointLight GetFirstFloorDefaultPointLight()
{
    return PointLight(glm::vec3(1.0), 0.5, 0.8, 0.7);
}

/*
struct Material{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};
*/
Material GetFirstFloorDefaultWallMaterial()
{
    return Material(
        glm::vec3(1, 0.933, 0.894),//#ffeee4
        glm::vec3(1, 0.933, 0.894),//#ffeee4
        glm::vec3(1, 1, 0.953),//#fffff3
        16
    );
}

Material GetFirstFloorDefaultFloorMaterial()
{
    return Material(
        glm::vec3(0.329, 0.365, 0.384),//#d6ecfa
        glm::vec3(0.329, 0.365, 0.384),//#d6ecfa
        glm::vec3(0.384, 0.392, 0.286),//#fbffb9
        16
    );
}

Material GetFirstFloorDefaultCeilMaterial()
{
    return Material(
        glm::vec3(0.376, 0.361, 0.216),//#f6ea8c
        glm::vec3(0.376, 0.361, 0.216),//#f6ea8c
        glm::vec3(0.3, 0.3, 0.3),//
        1
    );
}