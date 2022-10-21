#version 140
#extension GL_ARB_compatibility: enable

// in vec4 p;
// in vec3 normal, lightDir;
// 
// void main()
// {	
// 	//vec3 v = normalize(-vec3(p));
// 	vec3 n = normalize(normal);
// 	vec3 l = normalize(vec3(gl_LightSource[0].position));
// 	//vec3 r = normalize(-1 * n);
// 
// 	vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
// 	gl_FragColor = max(dot(l,n),0)*diffuse;
// 	//gl_FragColor = gl_Color;
// }


in vec3 N;
in vec3 v;    

void main (void)  
{ 
    vec3 L = normalize(gl_LightSource[0].position.xyz - v);   
    vec3 E = normalize(-v);       // we are in Eye Coordinates, so EyePos is (0,0,0)  
    vec3 R = normalize(-reflect(L,N));  

    //calculate Ambient Term:  
    vec4 Iamb = gl_FrontLightProduct[0].ambient;    

    //calculate Diffuse Term:  
    vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);    

    // calculate Specular Term:
    vec4 Ispec = gl_FrontLightProduct[0].specular 
                * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);

    // write Total Color:  
    gl_FragColor = vec4(1,0,0,1) + Iamb + Idiff + Ispec;   
}