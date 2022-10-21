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


// Shader programs
GLuint p;

Mesh mesh;

// Light position
float lpos[4] = {0.0f,1.0f,5.0f,0.0f};
GLfloat AmbientLightValue[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat DiffuseLightValue[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat SpecularLightValue[] = { 0.3f, 0.3f, 0.3f, 1.0f };

float cameraSpeed = 0.05f; // adjust accordingly
GLdouble cameraPos[] = { 0.0, 0.0, 1.0 };
GLdouble cameraFront[] = { 0.0, 0.0, -1.0 };
float eye[3], at[3], up[3];
bool isPerspective = true;
int width;
int height;


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