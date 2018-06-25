#include "Header/Room.h"
using namespace RoomRule;

Room::Room(Camera &camera, glm::mat4 &model):
    directory("Resource/Texture")
{
    ModelMatrix = model;
    initRoom(camera);
    bindVAO();
}

Room::Room(const std::vector<float> &_wall, const std::vector<float> &_floor, 
        const Material &wall_material, const Material &floor_material, const Material &ceil_material,
        const std::vector<glm::vec3> &point_light_pos, const PointLight &point_light, const DirLight &dir_light):
        wall_vertices(_wall), floor_vertices(_floor), point_light_pos(point_light_pos), 
        wall_material(wall_material), floor_material(floor_material), ceil_material(ceil_material),
         point_light(point_light), dir_light(dir_light),
        directory("Resource/Texture")
{
    bindVAO();
}

void Room::Draw(Shader &materialShader)
{
    // set up point light
    setDirLight(materialShader, "dir_light", dir_light);
    setPointLight(materialShader, "point_light", point_light);
    setPointLightPos(materialShader, "point_light_pos");

    materialShader.setMat4("model",ModelMatrix);//set model matrix !!!

    // first draw the floor
    materialShader.setBool("floor_wall", 0);
    // set floor_material
    setMaterial(materialShader, "floor_material", floor_material);
    glBindVertexArray(floor_VAO);
        glDrawArrays(GL_TRIANGLES, 0, floor_vertices.size()>>4);
    // set ceil_material
    setMaterial(materialShader, "ceil_material", ceil_material);
        glDrawArrays(GL_TRIANGLES, floor_vertices.size()>>4, floor_vertices.size()>>4);

    
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
    if (value.diffuse_tex_color)
    {
        // bind diffuse map
        shader.setBool(name + ".diffuse_tex_color", true);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, value.diffuse_tex);
        shader.setInt(name + ".diffuse_tex", 0);
    }else
    {
        shader.setBool(name + ".diffuse_tex_color", false);
        shader.setVec3(name + ".ambient", value.ambient);
        shader.setVec3(name + ".diffuse", value.diffuse);
    }
    if (value.specular_tex_color)
    {
        // bind specular map
        shader.setBool(name + ".specular_tex_color", true);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, value.specular_tex);
        shader.setInt(name + ".specular_tex", 1);
    }else
    {
        shader.setBool(name + ".specular_tex_color", false);
        shader.setVec3(name + ".specular", value.specular);
    }
    shader.setFloat(name + ".shininess", value.shininess);
}

inline void Room::setPointLight(Shader &shader, const std::string &name, PointLight &value)
{
    shader.setVec3(name + ".color", value.color);
    shader.setFloat(name + ".ambient", value.ambient);
    shader.setFloat(name + ".diffuse", value.diffuse);
    shader.setFloat(name + ".specular", value.specular);
}

inline void Room::setDirLight(Shader &shader, const std::string &name, DirLight &value)
{
    shader.setVec3(name + ".dir", value.dir);
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
    floor_vertices = GetFirstFloorDefaultGround(camera, ModelMatrix);
    wall_vertices = GetFirstFloorDefaultWall(camera, ModelMatrix);
    point_light_pos = GetFirstFloorDefaultLightPos();
    point_light = GetFirstFloorDefaultPointLight();
    wall_material = GetFirstFloorDefaultWallMaterial();
    floor_material = GetFirstFloorDefaultFloorMaterial();
    ceil_material = GetFirstFloorDefaultCeilMaterial();
    dir_light = GetFirstFloorDefaultDirLight();
}

//|||||||||||||||||||||||||||||||||||||||||||||||||||||
// public function
void GetVertexByRules(std::vector<float> &vertices, Camera &camera, const std::vector<Rule> &rules, const glm::mat4& ModelMatrix)
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
                camera.SetinnerBound(glm::vec3(cx-p1/2, cy-p2/2, cz), 
								    glm::vec3(cx+p1/2, cy+p2/2, cz),ModelMatrix);
				vertices.insert(vertices.end(), temp_wall.begin(), temp_wall.end());
				break;
			case _left:
				temp_wall = GenCubeLeftVertices(p1, p2, cx, cy, cz);
				camera.SetinnerBound(glm::vec3(cx, cy-p2/2, cz-p1/2), 
								    glm::vec3(cx, cy+p2/2, cz+p1/2),ModelMatrix);
				vertices.insert(vertices.end(), temp_wall.begin(), temp_wall.end());
				break;
			case _front:
				temp_wall = GenCubeFrontVertices(p1, p2, cx, cy, cz);
				camera.SetinnerBound(glm::vec3(cx-p1/2, cy-p2/2, cz),
								    glm::vec3(cx+p1/2, cy+p2/2, cz),ModelMatrix);
				vertices.insert(vertices.end(), temp_wall.begin(), temp_wall.end());
				break;
			case _right:
				temp_wall = GenCubeRightVertices(p1, p2, cx, cy, cz);
				camera.SetinnerBound(glm::vec3(cx, cy-p2/2, cz-p1/2), 
								    glm::vec3(cx, cy+p2/2, cz+p1/2),ModelMatrix);
				vertices.insert(vertices.end(), temp_wall.begin(), temp_wall.end());
				break;
            case _buttom:
                temp_wall = GenCubeButtomVertices(p1, p2, cx, cy, cz); 
                camera.SetinnerBound(glm::vec3(cx-p1/2, cy, cz-p2/2),
                                    glm::vec3(cx+p1/2, cy, cz+p2/2),ModelMatrix);
                vertices.insert(vertices.end(), temp_wall.begin(), temp_wall.end());
                break;
            case _top:
                temp_wall = GenCubeTopVertices(p1, p2, cx, cy, cz); 
                camera.SetinnerBound(glm::vec3(cx-p1/2, cy, cz-p2/2),
                                    glm::vec3(cx+p1/2, cy, cz+p2/2),ModelMatrix);
                vertices.insert(vertices.end(), temp_wall.begin(), temp_wall.end());
                break;
			default:break;
		}
	}
}
//||||||||||||||||||||||||||||||||||||||||||||||||||
// first floor
std::vector<float> GetFirstFloorDefaultGround(Camera &camera,const glm::mat4 &ModelMatrix)
{
    float cy = -0.11, ce = 10;
    std::vector<float> vertices;
    std::vector<Rule> rules{
        Rule(_top, 15, 25, 7.5, cy, 12.5),//1
        Rule(_top, 25, 75, 15+12.5, cy, 37.5),//2
        Rule(_top, 40, 60, 40+20, cy, 15+30),//3
        // Rule(_top, 10, 45, 80+5, cy, 15+22.5),//4
        Rule(_top, 500, 500, 0, -0.1, 0),

        //ceil
        Rule(_buttom, 15, 25, 7.5, ce, 12.5),//1
        Rule(_buttom, 25, 75, 15+12.5, ce, 37.5),//2
        Rule(_buttom, 40, 60, 40+20, ce, 15+30),//3
        Rule(_buttom, 10, 45, 80+5, ce, 15+22.5),//4
    };
    GetVertexByRules(vertices, camera, rules, ModelMatrix);
    return vertices;
}

std::vector<float> GetFirstFloorDefaultWall(Camera &camera,const glm::mat4 &ModelMatrix)
{
    
    float wall_height = 10, door_height = 7.5, wall_width = 1.5, cy = wall_height/2, dy = (door_height+wall_height)/2;
    float e = 0.1;
    std::vector<float> vertices;
    std::vector<Rule> rules{
       Rule(_front, 15, wall_height, 7.5, cy, 25),//1
       Rule(_front, wall_width, wall_height,  15+wall_width/2, cy, 10),//2
       Rule(_front, 25, wall_height,  15+12.5, cy, 40),//3
       Rule(_back, 25-wall_width, wall_height,  15+12.5-wall_width/2, cy, 40+e),//3
       Rule(_back, 28, wall_height,  15+14, cy, 60),//4
       Rule(_front, 25-wall_width, wall_height,  15+14-wall_width/2, cy, 60-e),//4
       Rule(_front, 65, wall_height,  15+32.5, cy, 75),//5
       Rule(_back, 22, wall_height,  69-11, cy, 60),//6
       Rule(_front, 15, wall_height,  50+7.5, cy, 60-e),//6
       Rule(_front, 3, wall_height,  40+1.5, cy, 60-wall_width),//7
       Rule(_front, 3, wall_height,  47+1.5, cy, 60-wall_width),//8
       Rule(_back, 17, wall_height,  90-8.5, cy, 60),//9
       Rule(_front, 4, wall_height,  65+2, cy, 60-wall_width),//10
       Rule(_front, 17, wall_height,  90-8.5, cy, 60-wall_width),//11
       Rule(_back, 4, wall_height,  65+2, cy, 60-3-wall_width),//12
       Rule(_back, 7, wall_height,  73+3.5, cy, 60-3-wall_width),//13
       Rule(_front, 4, wall_height,  65+2, cy, 60-3-2*wall_width),//14
       Rule(_front, 7, wall_height,  73+3.5, cy, 60-3-2*wall_width),//15
       Rule(_front, wall_width, wall_height,  40-wall_width/2, cy, 55),//16
       Rule(_back, wall_width, wall_height,  40-wall_width/2, cy, 52),//17
       Rule(_back, 6, wall_height,  50+3, cy, 40+wall_width),//18
       Rule(_back, 6, wall_height,  65-3, cy, 40+wall_width),//19
       Rule(_front, 6, wall_height,  50+3, cy, 40),//20
       Rule(_front, 31, wall_height,  90-15.5, cy, 40),//21
       Rule(_back, 25, wall_height,  90-12.5, cy, 40+e),//21
       Rule(_back, wall_width, wall_height,  65+wall_width/2, cy, 30),//22
       Rule(_back, 50, wall_height,  40+25, cy, 15),//23
       Rule(_back, 31, wall_height,  9+15.5, cy, wall_width),//24
       Rule(_back, 31, wall_height,  9+15.5, cy, 0),//25
       Rule(_back, 6, wall_height,  3, cy, wall_width),//26
       Rule(_back, 6, wall_height,  3, cy, 0),//27
       Rule(_back, wall_width, wall_height, 15+wall_width/2, cy, 7),//28

       Rule(_right, 25, wall_height,  0, cy, 12.5),//1
       Rule(_right, wall_width, wall_height,  6, cy, wall_width/2),//2
       Rule(_left, wall_width, wall_height,  9, cy, wall_width/2),//3
       Rule(_left, 65, wall_height,  15, cy, 10+32.5),//4
       Rule(_left, 7, wall_height,  15, cy, 3.5),//5
       Rule(_right, 65, wall_height,  15+wall_width, cy, 10+32.5),//6
       Rule(_right, 7, wall_height,  15+wall_width, cy, 3.5),//7
       Rule(_left, 5, wall_height,  40-wall_width, cy, 60-2.5),//8
       Rule(_left, 12, wall_height,  40-wall_width, cy, 40+6),//9
       Rule(_right, 5, wall_height,  40, cy, 60-2.5),//10
       Rule(_right, 12, wall_height,  40, cy, 40+6),//11
       Rule(_left, 15, wall_height,  40, cy, 7.5),//12
       Rule(_right, wall_width, wall_height,  43, cy, 60-wall_width/2),//13
       Rule(_left, wall_width, wall_height,  47, cy, 60-wall_width/2),//14
       Rule(_left, 20, wall_height,  50, cy, 40+10),//15
       Rule(_right, 20, wall_height,  50+e, cy, 40+10),//15
       Rule(_right, wall_width, wall_height,  56, cy, 40+wall_width/2),//16
       Rule(_left, wall_width, wall_height,  59, cy, 40+wall_width/2),//17
       Rule(_left, 15, wall_height,  65, cy, 15+7.5),//18
       Rule(_right, 15, wall_height,  65+wall_width, cy, 15+7.5),//19
       Rule(_left, 17-wall_width, wall_height,  65, cy, 40+(17-wall_width)/2),//20
       Rule(_right, 17-wall_width, wall_height,  65+e, cy, 40+(17-wall_width)/2),//20
       Rule(_right, wall_width, wall_height,  69, cy, 60-wall_width-3-wall_width/2),//21
       Rule(_left, wall_width, wall_height,  73, cy, 60-wall_width-3-wall_width/2),//22
       Rule(_right, wall_width, wall_height,  69, cy, 60-wall_width/2),//23
       Rule(_left, wall_width, wall_height,  73, cy, 60-wall_width/2),//24
       Rule(_left, 15+wall_width, wall_height,  65, cy, 75-(15+wall_width)/2),//25
       Rule(_right, 15+wall_width, wall_height,  65+e, cy, 75-(15+wall_width)/2),//25
       Rule(_left, 15, wall_height,  80, cy, 75-7.5),//26
       Rule(_right, 17-wall_width, wall_height,  80, cy, 40+(17-wall_width)/2),//27
       Rule(_left, 17-wall_width, wall_height,  80-e, cy, 40+(17-wall_width)/2),//27
       Rule(_left, 45, wall_height,  90, cy, 15+22.5),//28

       //door
       Rule(_back, 3, wall_height-door_height, 6+1.5, dy, wall_width), //1
       Rule(_front, 3, wall_height-door_height, 6+1.5, dy, 0),//1
       Rule(_buttom, 3, wall_width, 6+1.5, door_height, wall_width/2),//1
       Rule(_back, 4, wall_height-door_height, 43+2, dy, 60),//2
       Rule(_front, 4, wall_height-door_height, 43+2, dy, 60-wall_width),//2
       Rule(_buttom, 4, wall_width, 43+2, door_height, 60 - wall_width/2),//2
       Rule(_back, 3, wall_height-door_height, 56+1.5, dy, 40+wall_width),//3
       Rule(_front, 3, wall_height-door_height, 56+1.5, dy, 40),//3
       Rule(_buttom, 3, wall_width, 56+1.5, door_height, 40 + wall_width/2),//3
       Rule(_back, 4, wall_height-door_height, 69+2, dy, 60-3-wall_width),//4
       Rule(_front, 4, wall_height-door_height,69+2, dy, 60-3-2*wall_width),//4
       Rule(_buttom, 4, wall_width, 69+2, door_height, 60-3-1.5*wall_width),//4
       Rule(_back, 4, wall_height-door_height, 69+2, dy, 60),//5
       Rule(_front, 4, wall_height-door_height, 69+2, dy, 60-wall_width),//5
       Rule(_buttom, 4, wall_width, 69+2, door_height, 60-wall_width/2),//5
       Rule(_left, 3, wall_height-door_height, 15, dy, 7+1.5),//6
       Rule(_right, 3, wall_height-door_height, 15+wall_width, dy, 7+1.5),//6
       Rule(_buttom, wall_width, 3, 15+wall_width/2, door_height, 7+1.5),//6
       Rule(_right, 3, wall_height-door_height, 40-wall_width, dy, 52+1.5),//7
       Rule(_right, 3, wall_height-door_height, 40, dy, 52+1.5),//7
       Rule(_buttom, wall_width, 3, 40-wall_width/2, door_height, 52+1.5),//7
    };
    GetVertexByRules(vertices, camera, rules, ModelMatrix);
    return vertices;
}

std::vector<glm::vec3> GetFirstFloorDefaultLightPos()
{
    float wall_height = 10, delta = 0.1;
    float y = wall_height-delta;
    return std::vector<glm::vec3>{
        glm::vec3(7.5, y, 15),//1
        glm::vec3(35, y, 25),//2
        glm::vec3(25, y, 50),//3
        glm::vec3(25, y, 67.5),//4
        glm::vec3(55, y, 67.5),//4
        glm::vec3(57.5, y, 50),//5
        glm::vec3(72.5, y, 68),//6
        //glm::vec3(73, y, 50),//7
        glm::vec3(75, y, 25),//8
        glm::vec3(85, y, 50),//9
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
    return PointLight(glm::vec3(1.0), 0.5, 1.5, 1.0);
}

DirLight GetFirstFloorDefaultDirLight()
{
    return DirLight(glm::vec3(0, 0, 1), glm::vec3(1), 0.2, 0.8, 0.5);
}
/*
struct Material{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    GLuint diffuse_tex;
    GLuint specular_tex;
    bool diffuse_tex_color;
    bool specular_tex_color;
};
*/
Material GetFirstFloorDefaultWallMaterial()
{
    return Material(
        "matrix.jpg",
        glm::vec3(1, 1, 0.953),//#fffff3
        16
    );
    // return Material(
    //     glm::vec3(1, 0.933, 0.894),//#ffeee4
    //     glm::vec3(1, 0.933, 0.894),//#ffeee4
    //     glm::vec3(1, 1, 0.953),//#fffff3
    //     16
    // );
}

Material GetFirstFloorDefaultFloorMaterial()
{
    return Material(
        "floor.jpg",
        glm::vec3(0.384, 0.392, 0.286),//#fbffb9
        16
    );
    // return Material(
    //     glm::vec3(0.329, 0.365, 0.384),//#d6ecfa
    //     glm::vec3(0.329, 0.365, 0.384),//#d6ecfa
    //     glm::vec3(0.384, 0.392, 0.286),//#fbffb9
    //     16
    // );
}

Material GetFirstFloorDefaultCeilMaterial()
{
    return Material(
        "ceil.jpg",
        glm::vec3(0.3, 0.3, 0.3),//
        2
    );
    // return Material(
    //     glm::vec3(0.376, 0.361, 0.216),//#f6ea8c
    //     glm::vec3(0.376, 0.361, 0.216),//#f6ea8c
    //     glm::vec3(0.3, 0.3, 0.3),//
    //     2
    // );
}