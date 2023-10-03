// Interaction:
// Press +/- to add/erase random point ---> good practice to add interaction as a comment at the top of source code

#include <glew.h>
#include <freeglut.h> 
#include <vector>
#include <glm/glm/vec3.hpp>
#include <random>
#include <iostream>

//points: datastructure to store current points
std::vector<glm::vec3> points;
// window dimension
float windowWidth = 100.0;
float windowHeight = 100.0;
// Drawing routine.
void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(1.0); // Default line width.
	glPointSize(5); // set point size to 5 units
//-------------------------------------
/*
----------write points drawing logic below------------
*/
	glBegin(GL_POINTS); // points mode to draw points
	glColor3f(0.0, 0.0, 0.0); // black color
	// draw points from points vector 
	for (int i = 0; i < points.size(); i++) {
		glVertex3f(points[i].x, points[i].y, 0.);
	}
	glEnd(); // end points mode
	//----------------------------------------------------
	/*
	----------write lines drawing logic below------------
	*/
	glBegin(GL_LINES); // lines mode to draw lines
	glColor3f(1, 0, 0); // red color
	// draw lines from points vector
	for (int i = 0; i < points.size(); i++) {
		// draw line only if i is odd (when the point has a previous pair)
		if (i % 2) {
			glVertex3f(points[i - 1].x, points[i - 1].y, 0.);
			glVertex3f(points[i].x, points[i].y, 0.);
		}
	}
	glEnd(); // end lines mode
	glFlush();
}
// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}
// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, windowWidth, 0.0, windowHeight, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
	case '+': {
		// add a random point
		glm::vec3 p;
		p.x = rand() % (int)windowWidth;
		p.y = rand() % (int)windowHeight;
		points.push_back(p);
		glutPostRedisplay();
		break;
	}
	case '-':
		// remove the last point
		if (!points.empty()) {
			points.pop_back();
			glutPostRedisplay();
		}
		break;
	default:
		break;
	}
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Press +/- to add/erase random point" << std::endl;
}

// Main routine.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	printInteraction(); // good practice to print how to interact
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("RandomLines.cpp");
	glutDisplayFunc(drawScene);//drawing function
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);//handle keyboard events

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}