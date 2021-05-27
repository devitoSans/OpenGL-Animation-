#version 330 core

//resulting
out vec4 fragColor;

//struct
struct Material {
	sampler2D texture_diffuse1;
};

//in out shader
in VS_OUT {
	vec2 texCoord;
} vs_in;

//uniform 
uniform Material material;

void main() {
	//result
	fragColor = texture(material.texture_diffuse1, vs_in.texCoord);
}