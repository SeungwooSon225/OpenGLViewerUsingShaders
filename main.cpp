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
#include <GL/freeglut.h>


// Shader programs
GLuint p;

Mesh mesh;

// Light position
float lpos[2][4] = { {-1.0f,1.0f,3.0f,0.0f},
					 {1.0f,1.0f,3.0f,0.0f} };
//GLfloat AmbientLightValue[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//GLfloat DiffuseLightValue[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//GLfloat SpecularLightValue[] = { 1.0f, 1.0f, 1.0f, 1.0f };

float greenOffset = 0.2f;
float currentAlpha = 1.0f;
float currentAmbient[] = { 0.5f, greenOffset, 0.5f, 1.0f };
float currentDiffuse[] = { 0.5f, greenOffset, 0.5f, 1.0f };
float currentSpecular[] = { 0.5f, greenOffset, 0.5f, 1.0f };

float cameraSpeed = 0.05f;
GLdouble cameraPos[] = { 0.0, 0.0, 1.0 };
GLdouble cameraFront[] = { 0.0, 0.0, -1.0 };
bool isPerspective = true;
int width;
int height;

bool isLeftClicked = false;
bool isRightClicked = false;
bool isMiddleClicked = false;
float mouseDownPosition[2];
float tempCameraPosition[3];
float tempModelPosition[2];
float modelPosition[2];
float tempModelRotation[2];
float modelRotation[2];
float tempScale[3];
float scale[3] = {1.0f, 1.0f, 1.0f};


void reshape(int w, int h)
{
	width = w;
	height = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)width / height, 0.1, 500);

	// 모델뷰 매트릭스 초기화
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '3':
		if (currentDiffuse[0] >= 1.0f)
		{
			std::cout << "Current diffuse light is maximum" << std::endl;
			break;
		}

		currentDiffuse[0] += 0.1f;
		currentDiffuse[1] += 0.1f;
		currentDiffuse[2] += 0.1f;

		if (currentDiffuse[0] > 1.0f)
		{
			currentDiffuse[0] = 1.0f;
			currentDiffuse[1] = 0.5f + greenOffset;
			currentDiffuse[2] = 1.0f;
		}

		//glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLightValue); //Diffuse 조명의 성질을 설정한다.
		//glLightfv(GL_LIGHT1, GL_DIFFUSE, DiffuseLightValue); //Diffuse 조명의 성질을 설정한다.

		std::cout << "Increase diffuse light, (current diffuse light: " << currentDiffuse[0] << ", " << currentDiffuse[1] << ", " << currentDiffuse[2] << ")" << std::endl;

		break;

	case '1':
		if (currentDiffuse[1] <= 0.0f)
		{
			std::cout << "Current diffuse light is minimum" << std::endl;
			break;
		}

		currentDiffuse[0] -= 0.1f;
		currentDiffuse[1] -= 0.1f;
		currentDiffuse[2] -= 0.1f;


		if (currentDiffuse[1] < 0.01f)
		{
			currentDiffuse[0] = 0.5f - greenOffset;
			currentDiffuse[1] = 0.0f;
			currentDiffuse[2] = 0.5f - greenOffset;
		}

		std::cout << "Decrease diffuse light, (current diffuse light: " << currentDiffuse[0] << ", " << currentDiffuse[1] << ", " << currentDiffuse[2] << ")" << std::endl;

		//glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLightValue); //Diffuse 조명의 성질을 설정한다.
		//glLightfv(GL_LIGHT1, GL_DIFFUSE, DiffuseLightValue); //Diffuse 조명의 성질을 설정한다.

		break;

	case '6':
		if (currentAmbient[0] >= 1.0f)
		{
			std::cout << "Current ambient light is maximum" << std::endl;
			break;
		}

		currentAmbient[0] += 0.1f;
		currentAmbient[1] += 0.1f;
		currentAmbient[2] += 0.1f;

		if (currentAmbient[0] > 1.0f)
		{
			currentAmbient[0] = 1.0f;
			currentAmbient[1] = 0.5f + greenOffset;
			currentAmbient[2] = 1.0f;
		}

		//glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLightValue); //Ambient 조명의 성질을 설정한다.
		//glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientLightValue); //Ambient 조명의 성질을 설정한다.

		std::cout << "Increase ambient light, (current diffuse light: " << currentAmbient[0] << ", " << currentAmbient[1] << ", " << currentAmbient[2] << ")" << std::endl;

		break;

	case '4':
		if (currentAmbient[1] <= 0.0f)
		{
			std::cout << "Current ambient light is minimum" << std::endl;
			break;
		}

		currentAmbient[0] -= 0.1f;
		currentAmbient[1] -= 0.1f;
		currentAmbient[2] -= 0.1f;

		if (currentAmbient[1] < 0.01f)
		{
			currentAmbient[0] = 0.5f - greenOffset;
			currentAmbient[1] = 0.0f;
			currentAmbient[2] = 0.5f - greenOffset;
		}

		//glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLightValue); //Ambient 조명의 성질을 설정한다.
		//glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientLightValue); //Ambient 조명의 성질을 설정한다.

		std::cout << "Decrease ambient light, (current diffuse light: " << currentAmbient[0] << ", " << currentAmbient[1] << ", " << currentAmbient[2] << ")" << std::endl;

		break;

	case '9':
		if (currentSpecular[0] >= 1.0f)
		{
			std::cout << "Current specular light is maximum" << std::endl;
			break;
		}

		currentSpecular[0] += 0.1f;
		currentSpecular[1] += 0.1f;
		currentSpecular[2] += 0.1f;

		if (currentSpecular[0] > 1.0f)
		{
			currentSpecular[0] = 1.0f;
			currentSpecular[1] = 0.5f + greenOffset;
			currentSpecular[2] = 1.0f;
		}

		//glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLightValue); //Specular 조명의 성질을 설정한다.
		//glLightfv(GL_LIGHT1, GL_SPECULAR, SpecularLightValue); //Specular 조명의 성질을 설정한다.

		std::cout << "Increase specular light, (current diffuse light: " << currentSpecular[0] << ", " << currentSpecular[1] << ", " << currentSpecular[2] << ")" << std::endl;

		break;

	case '7':
		if (currentSpecular[1] <= 0.0f)
		{
			std::cout << "Current specular light is minimum" << std::endl;
			break;
		}

		currentSpecular[0] -= 0.1f;
		currentSpecular[1] -= 0.1f;
		currentSpecular[2] -= 0.1f;

		if (currentSpecular[1] < 0.01f)
		{
			currentSpecular[0] = 0.5f - greenOffset;
			currentSpecular[1] = 0.0f;
			currentSpecular[2] = 0.5f - greenOffset;
		}
		//glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLightValue); //Specular 조명의 성질을 설정한다.
		//glLightfv(GL_LIGHT1, GL_SPECULAR, SpecularLightValue); //Specular 조명의 성질을 설정한다.

		std::cout << "Decrease specular light, (current diffuse light: " << currentSpecular[0] << ", " << currentSpecular[1] << ", " << currentSpecular[2] << ")" << std::endl;

		break;

	case 'p':
		std::cout << "Set projection mode as perspective" << std::endl;

		isPerspective = true;
		// 원근 투영 사용
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45, (float)width / height, 0.1, 500);

		// 모델뷰 매트릭스 초기화
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		break;

	case 'o':
		std::cout << "Set projection mode as orthogonal" << std::endl;

		isPerspective = false;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-3.0f, 3.0f, -3.0f, 3.0, -100.0f, 100.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		break;

	case '-':
		currentAlpha -= 0.1f;

		if (currentAlpha < 0) currentAlpha = 0.0f;

		std::cout << "Alpha: " << currentAlpha << std::endl;
		break;

	case '=':
		currentAlpha += 0.1f;
		std::cout << "Alpha: " << currentAlpha << std::endl;
		break;

	default:
		break;
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

	glutPostRedisplay();
}


void processMouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			if (isLeftClicked == false)
			{
				mouseDownPosition[0] = x;
				mouseDownPosition[1] = y;

				tempModelRotation[0] = modelRotation[0];
				tempModelRotation[1] = modelRotation[1];

				isLeftClicked = true;
			}
		}

		else if (button == GLUT_RIGHT_BUTTON)
		{
			if (isRightClicked == false)
			{
				mouseDownPosition[0] = x;
				mouseDownPosition[1] = y;

				tempCameraPosition[0] = cameraPos[0];
				tempCameraPosition[1] = cameraPos[1];
				tempCameraPosition[2] = cameraPos[2];

				tempScale[0] = scale[0];
				tempScale[1] = scale[1];
				tempScale[2] = scale[2];

				isRightClicked = true;
			}
		}

		else if (button == GLUT_MIDDLE_BUTTON)
		{
			if (isMiddleClicked == false)
			{
				mouseDownPosition[0] = x;
				mouseDownPosition[1] = y;

				tempModelPosition[0] = modelPosition[0];
				tempModelPosition[1] = modelPosition[1];

				isMiddleClicked = true;
			}
		}
	}
	else if (state == GLUT_UP)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			isLeftClicked = false;
		}

		else if (button == GLUT_RIGHT_BUTTON)
		{
			isRightClicked = false;
		}

		else if (button == GLUT_MIDDLE_BUTTON)
		{
			isMiddleClicked = false;
		}
	}
}

void processDragMouse(int x, int y)
{
	float mouseMoveX = (mouseDownPosition[0] - x);
	float mouseMoveY = (mouseDownPosition[1] - y);

	if (isMiddleClicked)	// translate model
	{
		modelPosition[0] = tempModelPosition[0] - mouseMoveX / 1500.0f;
		modelPosition[1] = tempModelPosition[1] + mouseMoveY / 1500.0f;
	}
	else if (isLeftClicked)	// rotate model
	{
		modelRotation[0] = tempModelRotation[0] - mouseMoveX / 10.0f;
		modelRotation[1] = tempModelRotation[1] - mouseMoveY / 10.0f;
	}
	else if (isRightClicked)	// zoom camera
	{
		if (isPerspective)
		{
			cameraPos[2] = tempCameraPosition[2] + mouseMoveY / 1500.0f;
		}
		else
		{
			scale[0] = tempScale[0] + mouseMoveY / 500.0f;
			scale[1] = tempScale[1] + mouseMoveY / 500.0f;
			scale[2] = tempScale[2] + mouseMoveY / 500.0f;
		}
	}
}


void renderScene(void) 
{
	//glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2],
		cameraFront[0], cameraFront[1], cameraFront[2],
		0.0f, 1.0f, 0.0f);

	glTranslatef(modelPosition[0], modelPosition[1], 0.0f);
	glRotatef(modelRotation[0], 0.0f, 1.0f, 0.0f);
	glRotatef(modelRotation[1], 1.0f, 0.0f, 0.0f);

	if (isPerspective)
	{
		glScalef(1.0, 1.0f, 1.0f);
	}
	else
	{
		glScalef(scale[0], scale[1], scale[2]);
	}

	glUseProgram(p);

	float alpha = glGetUniformLocation(p, "alpha");
	glUniform1f(alpha, currentAlpha);

	float ambient = glGetUniformLocation(p, "ambient");
	glUniform4f(ambient, currentAmbient[0], currentAmbient[1], currentAmbient[2], currentAmbient[3]);

	float diffuse = glGetUniformLocation(p, "diffuse");
	glUniform4f(diffuse, currentDiffuse[0], currentDiffuse[1], currentDiffuse[2], currentDiffuse[3]);

	float specular = glGetUniformLocation(p, "specular");
	glUniform4f(specular, currentSpecular[0], currentSpecular[1], currentSpecular[2], currentSpecular[3]);

	// Draw something here!
	glBindVertexArray(mesh.VAO);
	glDrawElements(GL_TRIANGLES, mesh.numFaces * 3, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, mesh.numVertices);
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
	glutMouseFunc(processMouse);
	glutMotionFunc(processDragMouse);


	glEnable(GL_DEPTH_TEST);
    glClearColor(1.0,1.0,1.0,1.0);


	//glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLightValue); //Ambient 조명의 성질을 설정한다.
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLightValue); //Diffuse 조명의 성질을 설정한다.
	//glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLightValue); //Specular 조명의 성질을 설정한다.
	glLightfv(GL_LIGHT0, GL_POSITION, lpos[0]);

	//glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientLightValue); //Ambient 조명의 성질을 설정한다.
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, DiffuseLightValue); //Diffuse 조명의 성질을 설정한다.
	//glLightfv(GL_LIGHT1, GL_SPECULAR, SpecularLightValue); //Specular 조명의 성질을 설정한다.
	glLightfv(GL_LIGHT1, GL_POSITION, lpos[1]);

	glewInit();
	if (glewIsSupported("GL_VERSION_3_3"))
		printf("Ready for OpenGL 3.3\n");
	else {
		printf("OpenGL 3.3 is not supported\n");
		exit(1);
	}

	// Create shader program
	p = createGLSLProgram( "../phong.vert", NULL, "../phong.frag" ); // Phong
	glUseProgram(p);

	mesh.CreateMesh("bunny.off");

	//CreateTriangle();

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}