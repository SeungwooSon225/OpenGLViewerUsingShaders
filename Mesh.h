#pragma once

#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <string>
#include <sstream>
#include "textfile.h"
#include "VertexIndex.h"
#include "Vector.h"
#include "Vertex.h"


class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<VertexIndex> faces;
	std::vector<Vector3>* neighborNormal;

	int numVertices;
	int numFaces;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	void CreateMesh();
	std::vector<std::string> split(std::string str, char Delimiter);

	void SaveFaceNormal();
	void ComputeVertexNormal();
};