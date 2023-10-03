// using cylinders only as shapes and display lists and using OpenGL transformation functions. Draw a 3 - legged stool.
// For the legs, first create one in a display list and then draw it three times rotated appropriately using appropriate transformation.
// Handle user input at runtime as follows :
// • When user presses ‘x’ / ‘X’. the 3-legged stool should rotate around x-axis in CW/CCW manner respectively.
// • When user presses ‘y’ / ‘Y’.the 3 - legged stool should rotate around y - axis in CW / CCW manner respectively.
// • When user presses ‘z’ / ‘Z’.the 3 - legged stool should rotate around z - axis in CW / CCW manner respectively.
// • When user presses ‘space’.Toggle drawing mode of the 3 - legged between drawing a wireframe object or a solid one.


#define _USE_MATH_DEFINES 

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <glew.h>
#include <freeglut.h> 

using namespace std;


static unsigned int leg  /* List index */ , angleStep = 10;
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate
static bool isWire = true; // Is wireframe?


void setup(void);
void drawLeg(float radius, float height, int steps);
void drawSeat(float radius, float height, int steps);
void keyInput(unsigned char key, int x, int y);
void display();
void reshape(int width, int height);


void setup(void) {
    leg = glGenLists(3); // Return a list index.
    if (isWire) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glNewList(leg, GL_COMPILE);    
    drawLeg(0.1, 2.0, 32);
    glEndList();
}

void drawLeg(float radius, float height, int steps) {
    float angle, x, z;
    glPushMatrix();
    glTranslatef(0.0f, -height / 2.0f, 0.0f);
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= steps; i++) {
        angle = i * 2.0f * M_PI / steps;
        x = radius * cosf(angle); 
        z = radius * sinf(angle); 
        glVertex3f(x, 0.0f, z);
        glVertex3f(x, height, z);
    }
    glEnd();
    glPopMatrix();
}

void drawSeat(float radius, float height, int steps) {
    float angle, x, z;
    glPushMatrix();
    glTranslatef(0.0f, -height / 2.0f, 0.0f);
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= steps; i++) {
        angle = i * 2.0f * M_PI / steps;
        x = radius * cosf(angle);
        z = radius * sinf(angle);
        glVertex3f(x, 0.0f, z);
        glVertex3f(x, height, z);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i <= steps; i++) {
        angle = i * 2.0f * M_PI / steps;
        x = radius * cosf(angle);
        z = radius * sinf(angle);
        glVertex3f(x, 0.0f, z);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, height, 0.0f);
    for (int i = 0; i <= steps; i++) {
        angle = i * 2.0f * M_PI / steps;
        x = radius * cosf(angle);
        z = radius * sinf(angle);
        glVertex3f(x, height, z);
    }
    glEnd();
    glPopMatrix();
}

void keyInput(unsigned char key, int x, int y) {
    switch (key) {
        case 27:
            exit(0);
            break;
        case 'x':
            Xangle += angleStep;
            if (Xangle > 360.0) Xangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'X':
            Xangle -= angleStep;
            if (Xangle < 0.0) Xangle += 360.0;
            glutPostRedisplay();
            break;
        case 'y':
            Yangle += angleStep;
            if (Yangle > 360.0) Yangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'Y':
            Yangle -= angleStep;
            if (Yangle < 0.0) Yangle += 360.0;
            glutPostRedisplay();
            break;
        case 'z':
            Zangle += angleStep;
            if (Zangle > 360.0) Zangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'Z':
            Zangle -= angleStep;
            if (Zangle < 0.0) Zangle += 360.0;
            glutPostRedisplay();
            break;
        case ' ':
            isWire = !isWire;
            if (isWire) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glutPostRedisplay();
			break;
        default:
            break;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    glColor3f(0.0, 0.0, 0.0); // black

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glRotatef(Xangle, 1.0, 0.0, 0.0);
    glRotatef(Yangle, 0.0, 1.0, 0.0);
    glRotatef(Zangle, 0.0, 0.0, 1.0);

    // Draw the seat
    glPushMatrix();
    glTranslatef(0.0, 1.0, 0.0);
    drawSeat(1.0, 0.1, 32);
    glPopMatrix();
    
    // Draw the three legs
    glPushMatrix();
    glTranslatef(0.75, 0.0, 0.0);
    glRotatef(15.0, 0.0, 0.0, 1.0);
    glCallList(leg);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.75, 0.0, 0.0);
    glRotatef(-15.0, 0.0, 0.0, 1.0);
    glCallList(leg);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.75);
    glRotatef(-15.0, 1.0, 0.0, 0.0);
    glCallList(leg);
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (GLdouble)width / (GLdouble)height, 0.5, 20.0);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, width, height);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("3-Legged Stool");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyInput);
    glutReshapeFunc(reshape);
    setup();
    glutMainLoop();

    return 0;
}
