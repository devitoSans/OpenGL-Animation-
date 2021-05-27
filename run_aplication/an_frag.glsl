#version 330 core

//resulting
out vec4 fragColor;

//struct
struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 color;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

//in out shader
in VS_OUT {
	vec2 texCoord;
	vec3 normal;
	vec3 fragPos;
} vs_in;

//uniform
layout (std140) uniform Data {
	vec3 cameraPos;
	Light light;
};
uniform Material material;

//Lighting Result
vec4 lightingResult();


void main() {
	fragColor = lightingResult();
}


//Lighting Result
vec4 lightingResult() {
	vec4 mainTexture = texture(material.texture_diffuse1, vs_in.texCoord);
	if(mainTexture.a < 0.1)
		discard;
	vec4 diffuseMap = mainTexture;

	//properties for lighting
	vec3 diffuseMapVec3 = vec3(diffuseMap);
	vec3 specularMapVec3 = vec3(texture(material.texture_specular1, vs_in.texCoord));

	vec3 norm_normal = normalize(vs_in.normal);
	vec3 lightDir = normalize(light.position - vs_in.fragPos);

	vec3 cameraDir = normalize(cameraPos - vs_in.fragPos);
	vec3 reflectDir = reflect(-lightDir, norm_normal);

	//attenuation
	float distance = length(light.position - vs_in.fragPos);
	float distance_quadrat = distance * distance;
	float attenuation = 1.f / (light.constant + light.linear * distance + light.quadratic * distance_quadrat);

	//ambient
	vec3 ambient = light.ambient * diffuseMapVec3;

	//diffuse
	float diff = max(dot(norm_normal, lightDir), 0.f);
	vec3 diffuse = light.diffuse * diff * diffuseMapVec3;

	//specular
	float spec = pow(max(dot(cameraDir, reflectDir), 0.f), material.shininess);
	vec3 specular = light.specular * spec * diffuseMapVec3;

	//Lighting Result
	return vec4((ambient + diffuse) * light.color, 1.f);
}