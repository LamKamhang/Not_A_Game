#version 330 core
out vec4 FragColor;

struct Material{
	// use texture to replace color
	//vec3 ambient;
	//vec3 diffuse;
	//vec3 specular;
	sampler2D diffuse;	
	sampler2D specular;
	float shininess;
};

struct Light{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
	// ambient
	//vec3 ambient = light.ambient * material.ambient;
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

	// diffuse 
	vec3 norm = normalize(Normal);
	// corresponding to world coordinate, not camera coordinate
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	//vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

	
	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	// lightDir from FragPos to lightPos, so here need -lightDir
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir),0.0f), material.shininess);
	//vec3 specular = light.specular * spec * material.specular;
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0f);
}