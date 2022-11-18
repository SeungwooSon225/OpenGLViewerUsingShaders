#version 140
#extension GL_ARB_compatibility: enable


in vec3 normal;
in vec4 p;
in float dist0;
in float dist1;
in float specAlpha;
in vec4 k_a;
in vec4 k_d;
in vec4 k_s;

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
	
	vec4 loc_ambient0 = gl_LightSource[0].ambient * k_a;
	vec4 loc_ambient1 = gl_LightSource[1].ambient * k_a;

	vec4 glb_ambient = gl_LightModel.ambient * k_a;

	vec4 diffuse0 = k_d * gl_LightSource[0].diffuse;
	vec4 diffuse1 = k_d * gl_LightSource[1].diffuse;

	vec4 specular0 = gl_LightSource[0].specular * k_s;
	vec4 specular1 = gl_LightSource[1].specular * k_s;

	vec4 phong0 = glb_ambient + loc_ambient0 + 

			att0*(max(dot(l0,n),0)*diffuse0 + 
			
			att0 * pow(max(dot(r0,v),0.0), 0.01 + specAlpha) * specular0);

	
	vec4 phong1 = glb_ambient + loc_ambient1 + 

			att1*(max(dot(l1,n),0)*diffuse1 + 
			
			att1 * pow(max(dot(r1,v),0.0), 0.01 + specAlpha) * specular1);
	
	gl_FragColor = phong0 + phong1;
}