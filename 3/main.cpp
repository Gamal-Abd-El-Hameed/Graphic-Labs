///////////////////////////////////////////////////////////////////////////////////////          
// Helix and Sphere.cpp
//
// This program asks user to choose between two types of shapes (Helix and Sphere)
// The program approximates a (Helix and Sphere) with an array of latitudinal triangle strips.
//
// Interaction:
// 
// In case of sphere:
// When user presses Q / q, increase / decrease number of latitudinal slices.
// When user presses P / p, increase / decrease number of longitudinal slices.
// When user presses W / w, draw sphere in wireframe / draw filled sphere.
//
// In case of helix: Number of turns of helix that should be drawn is 5 turns
// When user presses R / r, increase / decrease radius of the helix
// When user presses H / h, increase / decrease pitch of helix.
// When user presses N / n, increase / decrease number of vertices used to draw the helix.
//
/////////////////////////////////////////////////////////////////////////////////////// 

#define _USE_MATH_DEFINES 

#include <cmath>
#include <iostream>
#include <random>
#include <glew.h>
#include <freeglut.h>
using namespace std;

// Globals.
static int userChoice; // User choice between helix and sphere
static float R = 5.0; // Radius of Sphere.
static int p = 6; // Number of longitudinal slices.
static int q = 4; // Number of latitudinal slices.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate hemisphere.
static float offset = -10; // Distance of Sphere from the viewer.
static float spinSpeed = 5; // Speed of rotation.
static float prev_time = 0; // Previous time.
static double numberOfVertices = 2 * M_PI * 5; // Number of vertices used to draw the helix.
static float pitch = 0.1; // Pitch of helix.
static bool isWireframe = true; // Flag to draw sphere in wireframe or filled

// Create a random number generator engine and distribution
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dis(0.0f, 1.0f);

// Initialization routine.
void setup(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

// Drawing routine.
void drawScene(void) {
	float r, g, b;
    int  i, j;
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, offset);
    glRotatef(Zangle, 0.0, 0.0, 1.0);
    glRotatef(Yangle, 0.0, 1.0, 0.0);
    glRotatef(Xangle, 1.0, 0.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    if (userChoice == 1) { // Sphere
		// If user presses W / w, draw sphere in wireframe / draw filled sphere.
		if (isWireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        for (j = 0; j < q; j++) {
            glBegin(GL_TRIANGLE_STRIP); 
            for (i = 0; i <= p; i++)
            {
				r = dis(gen);
				g = dis(gen);
				b = dis(gen);

				glColor3f(r, g, b);
                glVertex3f(R * cos((float)(j + 1) / q * M_PI / 2.0) * cos(2.0 * (float)i / p * M_PI),
                    R * sin((float)(j + 1) / q * M_PI / 2.0),
                    -R * cos((float)(j + 1) / q * M_PI / 2.0) * sin(2.0 * (float)i / p * M_PI));

				r = dis(gen);
				g = dis(gen);
				b = dis(gen);

				glColor3f(r, g, b);
                glVertex3f(R * cos((float)j / q * M_PI / 2.0) * cos(2.0 * (float)i / p * M_PI),
                    R * sin((float)j / q * M_PI / 2.0),
                    -R * cos((float)j / q * M_PI / 2.0) * sin(2.0 * (float)i / p * M_PI));
            }
            glBegin(GL_TRIANGLE_STRIP);
            for (i = 0; i <= p; i++)
            {
				r = dis(gen);
				g = dis(gen);
				b = dis(gen);

				glColor3f(r, g, b);
                glVertex3f(-R * cos((float)(j + 1) / q * M_PI / 2.0) * cos(2.0 * (float)i / p * M_PI),
                    -R * sin((float)(j + 1) / q * M_PI / 2.0),
                    R * cos((float)(j + 1) / q * M_PI / 2.0) * sin(2.0 * (float)i / p * M_PI));

				r = dis(gen);
				g = dis(gen);
				b = dis(gen);

				glColor3f(r, g, b);
                glVertex3f(-R * cos((float)j / q * M_PI / 2.0) * cos(2.0 * (float)i / p * M_PI),
                    -R * sin((float)j / q * M_PI / 2.0),
                    R * cos((float)j / q * M_PI / 2.0) * sin(2.0 * (float)i / p * M_PI));
            }
            glEnd();
        }        
    }
    else if (userChoice == 2) { // Helix
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        float xc = 0.0f;
        float yc = 0.0f;

        glBegin(GL_LINE_STRIP);
        for (float i = 0; i <= numberOfVertices; i += 0.1) {
			r = dis(gen);
			g = dis(gen);
			b = dis(gen);

			glColor3f(r, g, b);
            glVertex3f(xc + R * cos(i), yc + R * sin(i), pitch * i);
        }
        glEnd();
    }
    glFlush();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void spinDisplay() {
	Yangle += spinSpeed * (glutGet(GLUT_ELAPSED_TIME) - prev_time) / 1000;
	prev_time = glutGet(GLUT_ELAPSED_TIME); // to indicate refresh rate
	if (Yangle > 360.0) Yangle -= 360.0;
	glutPostRedisplay();
}

void spinDisplayReverse() {

	Yangle -= spinSpeed * (glutGet(GLUT_ELAPSED_TIME) - prev_time) / 1000;
	prev_time = glutGet(GLUT_ELAPSED_TIME); // to indicate refresh rate
	std::cout << Yangle << std::endl;
	if (Yangle > 360.0) Yangle -= 360.0;
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(spinDisplay);
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(spinDisplayReverse);
		break;
	default:
		break;
	}
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'x':
		Xangle += 5.0;
		if (Xangle > 360.0) Xangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'X':
		Xangle -= 5.0;
		if (Xangle < 0.0) Xangle += 360.0;
		glutPostRedisplay();
		break;
	case 'y':
		Yangle += 5.0;
		if (Yangle > 360.0) Yangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'Y':
		Yangle -= 5.0;
		if (Yangle < 0.0) Yangle += 360.0;
		glutPostRedisplay();
		break;
	case 'z':
		Zangle += 5.0;
		if (Zangle > 360.0) Zangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'Z':
		Zangle -= 5.0;
		if (Zangle < 0.0) Zangle += 360.0;
		glutPostRedisplay();
		break;
	case 'O':
		offset += 1;
		glutPostRedisplay();
		break;
	case 'o':
		offset -= 1;
		glutPostRedisplay();
		break;
	case ' ':
		glutIdleFunc(NULL);
		break;
	case 'P':
		p += 1;
		glutPostRedisplay();
		break;
	case 'p':
		if (p > 3) p -= 1;
		glutPostRedisplay();
		break;
	case 'Q':
		q += 1;
		glutPostRedisplay();
		break;
	case 'q':
		if (q > 3) q -= 1;
		glutPostRedisplay();
		break;
	case 'W':
		isWireframe = true;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glutPostRedisplay();
		break;
	case 'w':
		isWireframe = false;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glutPostRedisplay();
		break;
	case 'R':
		R += 0.1;
		glutPostRedisplay();
		break;
	case 'r':
		if (R > 0.1) R -= 0.1;
		glutPostRedisplay();
		break;
	case 'H':
		pitch += 0.1;
		glutPostRedisplay();
		break;
	case 'h':
		if (pitch > 0.1) pitch -= 0.1;
		glutPostRedisplay();
		break;
	case 'N':
		numberOfVertices += 1;
		glutPostRedisplay();
		break;
	case 'n':
		if (numberOfVertices > 3) numberOfVertices -= 1;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void) {
	cout << "Interaction:\n" <<
		"In case of sphere:\n" <<
		"Press Q / q to increase / decrease number of latitudinal slices.\n" <<
		"Press P / p to increase / decrease number of longitudinal slices.\n" <<
		"Press W / w to draw sphere in wireframe / draw filled sphere.\n" <<
		"In case of helix : \n" <<
		"Press R / r to increase / decrease radius of the helix\n" <<
		"Press H / h to increase / decrease pitch of helix.\n" <<
		"Press N / n to increase / decrease number of vertices used to draw the helix.";
}

// Main routine.
int main(int argc, char** argv) {
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	printInteraction();
	cout << "Which shape do you want to draw?\n"
		<< "1. Sphere\n"
		<< "2. Helix\n";
	cin >> userChoice;

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Helix and Sphere.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutMouseFunc(mouse);
	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}
