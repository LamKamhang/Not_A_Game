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
    Material(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, const shininess):
        ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
    {};
};

struct Light{
	glm::vec3 color;
    float ambient;
	float diffuse;
	float specular;
    Light()=default;
    Light(const glm::vec3 &color, const ambient, const diffuse, const specular):
        color(color), ambient(ambient), diffuse(diffuse), specular(specular)
    {};
};

class Room
{
private:
    GLuint wall_VAO, wall_VBO, floor_VAO, floor_VBO;
    //GLuint wall_texID, floor_texID, ceil_texID;
    Material wall_material, floor_material, ceil_material;
    std::vector<float> wall_vertices, floor_vertices;
    std::vector<glm::vec3> light_pos;
    std::string directory;
    Light light;

    inline void bindVAO();
    inline void initRoom(Camera &camera);
    inline void setMaterial(Shader &shader, const std::string &name, Material &value);
    inline void setLight(Shader &shader, const std::string &name, Light &value);
    inline void setLightPos(Shader &shader, const std::string &name);
public:
    Room() = default;
    Room(Camera &camera);
    Room(const std::vector<float> &_wall, const std::vector<float> &_floor, 
        const Material &wall_material, const Material &floor_material, const Material &ceil_material);
    ~Room() = default;

    inline const std::vector<glm::vec3>& GetLightPos();
    void Draw(Shader &materialShader);
};

void GetVertexByRules(std::vector<float> &vertices, Camera &camera, const std::vector<RoomRule::Rule> &rules);

// default setting
std::vector<float> GetFirstFloorDefaultGround(Camera &camera);
std::vector<float> GetFirstFloorDefaultWall(Camera &camera);
std::vector<glm::vec3> GetFirstFloorDefaultLightPos();
Light GetFirstFloorDefaultLight();
Material GetFirstFloorDefaultWallMaterial();
Material GetFirstFloorDefaultFloorMaterial();
Material GetFirstFloorDefaultCeilMaterial();
