#version 140
#extension GL_ARB_compatibility: enable

#extension GL_ARB_separate_shader_objects : enable


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 normal;
out vec4 p;
out float dist0;
out float dist1;
out float specAlpha;
out vec4 k_a;
out vec4 k_d;
out vec4 k_s;

uniform float alpha;
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;

void main(void)
{
	gl_Position = gl_ModelViewProjectionMatrix * vec4(aPos, 1);
	//normal = (gl_ModelViewMatrix * vec4((gl_NormalMatrix * aNormal),0)).xyz;
	normal = normalize(gl_NormalMatrix * aNormal);

	p =	gl_ModelViewProjectionMatrix * vec4(aPos, 1);

	specAlpha = alpha;
	k_a = ambient;
	k_d = diffuse;
	k_s = specular;

	dist0 = length((gl_LightSource[0].position - p).xyz);
	dist1 = length((gl_LightSource[1].position - p).xyz);
}
