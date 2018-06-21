#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "Shader.h"
#include "util.h"
#include "Texture.h"
#include "Camera.h"

namespace RoomRule
{
    enum wallType{
        _back, _left, _front, _right, _buttom, _top,
    };
    struct Rule{
        wallType type;
        std::vector<float> parameters;
        Rule()=default;
        Rule(wallType type, float p1, float p2, float cx, float cy, float cz):
            type(type), parameters({p1, p2, cx, cy, cz})
        {};
    };
};

struct Material{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    Material()=default;
    Material(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, const float shininess):
        ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
    {};
};

struct PointLight{
	glm::vec3 color;
    float ambient;
	float diffuse;
	float specular;
    PointLight()=default;
    PointLight(const glm::vec3 &color, const float ambient, const float diffuse, const float specular):
        color(color), ambient(ambient), diffuse(diffuse), specular(specular)
    {};
};

class Room
{
private:
    GLuint wall_VAO, wall_VBO, floor_VAO, floor_VBO;
    //GLuint wall_texID, floor_texID, ceil_texID;
    std::vector<float> wall_vertices;
    std::vector<float> floor_vertices;
    std::vector<glm::vec3> point_light_pos;
    Material wall_material;
    Material floor_material;
    Material ceil_material;
    PointLight point_light;
    std::string directory;

    glm::mat4 ModelMatrix;
    inline void bindVAO();
    inline void initRoom(Camera &camera);
    inline void setMaterial(Shader &shader, const std::string &name, Material &value);
    inline void setPointLight(Shader &shader, const std::string &name, PointLight &value);
    inline void setPointLightPos(Shader &shader, const std::string &name);
public:
    Room() = default;
    Room(Camera &camera,glm::mat4 &model);
    Room(const std::vector<float> &_wall, const std::vector<float> &_floor, 
        const Material &wall_material, const Material &floor_material, const Material &ceil_material, const std::vector<glm::vec3> &point_light_pos, const PointLight &point_light);
    ~Room() = default;

    inline const std::vector<glm::vec3>& GetLightPos();

    void Draw(Shader &materialShader);
};

void GetVertexByRules(std::vector<float> &vertices, Camera &camera, const std::vector<RoomRule::Rule> &rules, const glm::mat4& ModelMatrix=glm::mat4(1.0f));

// default setting
std::vector<float> GetFirstFloorDefaultGround(Camera &camera, const glm::mat4& ModelMatrix=glm::mat4(1.0f));
std::vector<float> GetFirstFloorDefaultWall(Camera &camera, const glm::mat4& ModelMatrix=glm::mat4(1.0f));
std::vector<glm::vec3> GetFirstFloorDefaultLightPos();
PointLight GetFirstFloorDefaultPointLight();
Material GetFirstFloorDefaultWallMaterial();
Material GetFirstFloorDefaultFloorMaterial();
Material GetFirstFloorDefaultCeilMaterial();
