/*
 * Skeleton code for AAA633 Visual Computing
 *
 * Won-Ki Jeong, wkjeong@kroea.ac.kr
 *
 */

#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <string>
#include <assert.h>
#include <vector>
#include <sstream>
#include <vector>
#include "Vector.h"
#include "Vertex.h"
#include "VertexIndex.h"
#include "Mesh.h"

#define TEST false

// Shader programs
GLuint p;
GLuint VAO;
GLuint VBO;
GLuint EBO;

Mesh mesh;

// Light position
float lpos[4] = {0.0f,1.0f,5.0f,0.0f};
GLfloat AmbientLightValue[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat DiffuseLightValue[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat SpecularLightValue[] = { 0.3f, 0.3f, 0.3f, 1.0f };

std::vector<Vertex> vertices;
std::vector<Vector3>* neighborNormal;
std::vector<VertexIndex> indices;

int numVertices;
int numFaces;

float cameraSpeed = 0.05f; // adjust accordingly
GLdouble cameraPos[] = { 0.0, 0.0, 1.0 };
GLdouble cameraFront[] = { 0.0, 0.0, -1.0 };
float eye[3], at[3], up[3];
bool isPerspective = true;
int width;
int height;


void SaveFaceNormal(std::vector<VertexIndex> indices, std::vector<Vertex>& vertices)
{
	for (int i = 0; i < indices.size(); i++)
	{
		// Compute FaceNormal
		Vector3 v0 = vertices[indices[i].vertex0].position;
		Vector3 v1 = vertices[indices[i].vertex1].position;
		Vector3 v2 = vertices[indices[i].vertex2].position;

		Vector3 va = v1 - v0;
		Vector3 vb = v2 - v0;

		Vector3 vc = va.CrossProduct(vb);
		vc.Normalize();	

		//std::cout << vc.x << " " << vc.y << " " <<vc.z << std::endl;

		// Save FaceNormal to each vertex
		//vertices[indices[i].a].neighborNormal.push_back(vc);
		//vertices[indices[i].b].neighborNormal.push_back(vc);
		//vertices[indices[i].c].neighborNormal.push_back(vc);

		neighborNormal[indices[i].vertex0].push_back(vc);
		neighborNormal[indices[i].vertex1].push_back(vc);
		neighborNormal[indices[i].vertex2].push_back(vc);
	}
}


void ComputeVertexNormal(std::vector<Vertex>& vertices)
{
	for (int i = 0; i < vertices.size(); i++)
	{
		Vector3 vertexNormal = Vector3(0.0f, 0.0f, 0.0f);

		for (int j = 0; j < neighborNormal[i].size(); j++)
		{
			vertexNormal += neighborNormal[i][j];
		}

		vertexNormal /= neighborNormal[i].size();
		vertexNormal.Normalize();

		vertices[i].Normal = vertexNormal;
	}
}


void reshape(int w, int h)
{
	width = w;
	height = h;

	if (!isPerspective)
	{
		glLoadIdentity();
		glViewport(0, 0, w, h);
		glOrtho(0, 100, 0, 100, 100, -100);
	}
	else
	{
		// 뷰포트 설정
		glViewport(0, 0, w, h);

		// 원근 투영 사용
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45, (float)w / h, 0.1, 500);

		// 모델뷰 매트릭스 초기화
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
}


void keyboard(unsigned char key, int x, int y)
{
	if(key == 'p') {
		isPerspective = !isPerspective;

		std::cout << isPerspective << std::endl;

		if (!isPerspective)
		{
			glViewport(0, 0, width, height);
			
			// 원근 투영 사용
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, 100, 0, 100, 100, -100);

			// 모델뷰 매트릭스 초기화
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}
		else
		{
			// 뷰포트 설정
			glViewport(0, 0, width, height);

			// 원근 투영 사용
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45, (float)width / height, 0.1, 500);

			// 모델뷰 매트릭스 초기화
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}
	}
	if (key == 'w')
	{
		std::cout << "W" << std::endl;
		cameraPos[0] += cameraSpeed * cameraFront[0];
		cameraPos[1] += cameraSpeed * cameraFront[1];
		cameraPos[2] += cameraSpeed * cameraFront[2];
	}
	if (key == 's')
	{
		std::cout << "S" << std::endl;
		cameraPos[0] -= cameraSpeed * cameraFront[0];
		cameraPos[1] -= cameraSpeed * cameraFront[1];
		cameraPos[2] -= cameraSpeed * cameraFront[2];
	}

	if (key == '=')
	{
		DiffuseLightValue[0] += 0.1f;
		DiffuseLightValue[1] += 0.1f;
		DiffuseLightValue[2] += 0.1f;

		glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLightValue); //Ambient 조명의 성질을 설정한다.
		glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLightValue); //Diffuse 조명의 성질을 설정한다.
		glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLightValue); //Specular 조명의 성질을 설정한다.
	}
	if (key == '-')
	{
		DiffuseLightValue[0] -= 0.1f;
		DiffuseLightValue[1] -= 0.1f;
		DiffuseLightValue[2] -= 0.1f;

		glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLightValue); //Ambient 조명의 성질을 설정한다.
		glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLightValue); //Diffuse 조명의 성질을 설정한다.
		glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLightValue); //Specular 조명의 성질을 설정한다.
	}
	//	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	//if (key == 'd')
	//	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	glutPostRedisplay();
}

void renderScene(void) 
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2],
		cameraFront[0], cameraFront[1], cameraFront[2],
		0.0f, 1.0f, 0.0f);

	// Draw something here!
	glUseProgram(p);
	glBindVertexArray(mesh.VAO);
	glDrawElements(GL_TRIANGLES, mesh.numFaces * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

    glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}

void changeSize(int x, int y)
{

}


std::vector<std::string> split(std::string str, char Delimiter) {
	std::istringstream iss(str);             // istringstream에 str을 담는다.
	std::string buffer;                      // 구분자를 기준으로 절삭된 문자열이 담겨지는 버퍼

	std::vector<std::string> result;

	// istringstream은 istream을 상속받으므로 getline을 사용할 수 있다.
	while (getline(iss, buffer, Delimiter)) {
		result.push_back(buffer);               // 절삭된 문자열을 vector에 저장
	}

	return result;
}


void CreateTriangle()
{
	char* meshFilePath = "../mesh-data/bunny.off";
	char* meshFileData = textFileRead(meshFilePath);

	if (meshFileData == NULL)
		std::cout << "Fail to read mesh data" << std::endl;
	else
		std::cout << "Success to read mesh data" << std::endl;

	std::string meshData = meshFileData;
	std::vector<std::string> result = split(meshData, '\n');
	std::vector<std::string> metaData = split(result[1], ' ');

	numVertices = stoi(metaData[0]);
	numFaces = stoi(metaData[1]);

	std::cout << numVertices << std::endl;
	std::cout << numFaces << std::endl;

	neighborNormal = new std::vector<Vector3>[numVertices];

	for (int i = 0; i < numVertices; i++)
	{
		std::vector<std::string> triangle = split(result[2 + i], ' ');

		//std::cout << triangle[0] << ' ' << triangle[1] << ' ' << triangle[2] << std::endl;

		Vertex vertex;
		vertex.position = Vector3(stof(triangle[0]), stof(triangle[1]), stof(triangle[2]));

		vertices.push_back(vertex);
	}


	//indices = new unsigned int[numFaces * 3];

	int aa = 0;
	for (int i = 0; i < numFaces; i++)
	{
		std::vector<std::string> vertexIndex = split(result[2 + numVertices + i], ' ');

		//std::cout << vertexIndex[1] << ' ' << vertexIndex[2] << ' ' << vertexIndex[3] << std::endl;

		VertexIndex index = VertexIndex(stoul(vertexIndex[1]), stoul(vertexIndex[2]), stoul(vertexIndex[3]));
		indices.push_back(index);

		if (vertexIndex[1] == "0" || vertexIndex[2] == "0" || vertexIndex[3] == "0")
		{
			aa++;
		}
	}

	SaveFaceNormal(indices, vertices);
	ComputeVertexNormal(vertices);

	std::cout << neighborNormal[0].size() << std::endl;
	std::cout << aa << std::endl;
		

	std::wcout << neighborNormal[0].size() << std::endl;
	float tt = 0.0f;
	for (int i = 0; i < 6; i++)
	{
		std::wcout << neighborNormal[0][i].x << std::endl;
		tt += neighborNormal[0][i].x;

		//std::wcout << << std::endl;

		std::wcout << std::endl;

		std::wcout << vertices[i].Normal.x << std::endl;
		std::wcout << vertices[i].Normal.y << std::endl;
		std::wcout << vertices[i].Normal.z << std::endl;
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

#if TEST
	std::vector<Vertex> vertices;

	Vertex vertex1;
	vertex1.position = Vector3(0.5f, 0.5f, 0.0f);
	vertices.push_back(vertex1);

	Vertex vertex2;
	vertex2.position = Vector3(0.5f, -0.5f, 0.5f);
	vertices.push_back(vertex2);

	Vertex vertex3;
	vertex3.position = Vector3(-0.5f, -0.5f, 0.5f);
	vertices.push_back(vertex3);

	Vertex vertex4;
	vertex4.position = Vector3(-0.5f, 0.5f, 0.5f);
	vertices.push_back(vertex4);


	//float vts[] = {
	//	0.5f,  0.5f, 0.0f,  // 우측 상단
	//	0.5f, -0.5f, 0.5f,  // 우측 하단
	//	-0.5f, -0.5f, 0.5f,  // 좌측 하단
	//	-0.5f,  0.5f, 0.5f   // 좌측 상단
	//};

	std::vector<VertexIndex> indices;

	VertexIndex index1 = VertexIndex(0, 1, 3);
	indices.push_back(index1);

	VertexIndex index2 = VertexIndex(1, 2, 3);
	indices.push_back(index2);

	//unsigned int ids[] = {  // 0부터 시작한다는 것을 명심하세요!
	//	0, 1, 3,   // 첫 번째 삼각형
	//	1, 2, 3    // 두 번째 삼각형
	//};

	SaveFaceNormal(indices, vertices);
	ComputeVertexNormal(vertices);

	std::cout << "d" << vertices[1].neighborNormal.size() << std::endl;
	std::cout << "d" << vertices[1].Normal.x << std::endl;
	std::cout << "d" << vertices[1].Normal.y << std::endl;
	std::cout << "d" << vertices[1].Normal.z << std::endl;

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vts), vts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(VertexIndex), &indices[0], GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ids), ids, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
#else
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(VertexIndex), &indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
#endif // TEST

	
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}


//
// Main
//
int main(int argc, char** argv)
{
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("AAA633 - Assignment 1");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glEnable(GL_DEPTH_TEST);
    glClearColor(1.0,1.0,1.0,1.0);


	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLightValue); //Ambient 조명의 성질을 설정한다.
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLightValue); //Diffuse 조명의 성질을 설정한다.
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLightValue); //Specular 조명의 성질을 설정한다.

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

	glewInit();
	if (glewIsSupported("GL_VERSION_3_3"))
		printf("Ready for OpenGL 3.3\n");
	else {
		printf("OpenGL 3.3 is not supported\n");
		exit(1);
	}

	// Create shader program
	p = createGLSLProgram( "../phong.vert", NULL, "../phong.frag" ); // Phong

	mesh.CreateMesh();

	//CreateTriangle();

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}