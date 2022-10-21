#version 140
#extension GL_ARB_compatibility: enable


in vec3 normal;
in vec4 p;
in float dist0;
in float dist1;
in float specAlpha;

void main() {
	vec3 v = normalize(-p.xyz);
	vec3 n = normal;
	vec3 l0 = normalize((gl_LightSource[0].position - p).xyz);
	vec3 l1 = normalize((gl_LightSource[1].position - p).xyz);
	vec3 r0 = normalize(reflect(-l0, n));
	vec3 r1 = normalize(reflect(-l1, n));

	float att0 = 1.0 / (gl_LightSource[0].constantAttenuation +
		gl_LightSource[0].linearAttenuation * dist0 +
		gl_LightSource[0].quadraticAttenuation * dist0 * dist0);

	float att1 = 1.0 / (gl_LightSource[1].constantAttenuation +
		gl_LightSource[1].linearAttenuation * dist1 +
		gl_LightSource[1].quadraticAttenuation * dist1 * dist1);
	
	vec4 loc_ambient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient + gl_LightSource[1].ambient * gl_FrontMaterial.ambient;

	vec4 glb_ambient = gl_LightModel.ambient * gl_FrontMaterial.ambient;

	vec4 diffuse0 = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	vec4 diffuse1 = gl_FrontMaterial.diffuse * gl_LightSource[1].diffuse;

	vec4 specular0 = gl_LightSource[0].specular;
	vec4 specular1 = gl_LightSource[1].specular;
	
	gl_FragColor = glb_ambient + loc_ambient + 

			att0*(max(dot(l0,n),0)*diffuse0 + 
			att1*(max(dot(l1,n),0)*diffuse1 + 
			
			att0 * pow(max(dot(r0,v),0.0), specAlpha) * specular0) +
			att1 * pow(max(dot(r1,v),0.0), specAlpha) * specular1);
}