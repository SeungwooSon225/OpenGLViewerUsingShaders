// #version 140
// #extension GL_ARB_compatibility: enable
// 
// out vec3 normal;
// out vec3 n;
// out vec4 p;
// out float dist;
//  
//  void main()
//  {	
//      // object coordinate in H.C
//  	//vec4 P_obj = gl_Vertex;
//  	
//  	// Clip Coordinate
//  	gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex);
//  	normal = normalize(gl_NormalMatrix * gl_Normal);
//  	//p = gl_ModelViewMatrix * gl_Vertex;
//  	//n = vec3(gl_LightSource[0].position - p);
//  	//dist = length(dist);
//  
//  	gl_FrontColor = vec4(0,1,0,1);
//      gl_BackColor  = vec4(0,0,0,0); 
//  }

#version 140
#extension GL_ARB_compatibility: enable

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 N;
out vec3 v;

void main(void)
{

	v = vec3(gl_ModelViewMatrix * gl_Vertex);       
	N = normalize(aNormal);

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
