#define _USE_MATH_DEFINES

#include <cstdlib>
#include <cmath>
#include <iostream>

#include <glew.h>
#include <freeglut.h>
using namespace std;

// Globals.
int lastMouseX;
int lastMouseY;
bool isDragging = false;
static int isAnimate = 0; // Animated?
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene.
static int animationPeriod = 25; // Time interval between frames.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
static int width, height; // Size of the OpenGL window.
static float space_craft_angle = 0.0; // Angle of the spacecraft.
static float xVal = 0, zVal = 50; // Co-ordinates of the spacecraft.
static int isCollision = 0; // Is there collision between the spacecraft and an asteroid?
static unsigned int spacecraft; // Display lists base index.
const int Planets = 8,
Mercury = 0,
Venus = 1,
Earth = 2,
Mars = 3,
Jupiter = 4,
Saturn = 5,
Uranus = 6,
Neptune = 7;

int angle_of_Moon = 0;

// define colors and light
float black[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // black color
float yellow[] = { 0.7f, 0.2f, 0.0f, 1.0f }; // yellow color

// Routine to draw a bitmap character string.
void writeBitmapString(void* font, char* string) {
	char* c;
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Asteroid class.
class Asteroid {
public:
	Asteroid();
	Asteroid(int i, float x, float y, float z, float r, float colorR, float colorG, float colorB, float a, float a_inc);
	float getCenterX() { return centerX; }
	float getCenterY() { return centerY; }
	float getCenterZ() { return centerZ; }
	float getRadius() { return radius; }
	void draw();

	float centerX, centerY, centerZ, radius, angle, angle_increment;
	float color[3];
	int index;
};

// Asteroid default constructor.
Asteroid::Asteroid() {
	index = 0;
	centerX = 0.0;
	centerY = 0.0;
	centerZ = 0.0;
	radius = 0.0; // Indicates no asteroid exists in the position.
	angle = 0.0;
	angle_increment = 0.0;
	color[0] = 0;
	color[1] = 0;
	color[2] = 0;
}

// Asteroid constructor.
Asteroid::Asteroid(int i, float x, float y, float z, float r, float colorR, float colorG, float colorB,
	float a, float a_inc) {
	index = i;
	centerX = x;
	centerY = y;
	centerZ = z;
	radius = r;
	color[0] = colorR;
	color[1] = colorG;
	color[2] = colorB;
	angle = a;
	angle_increment = a_inc;
}

// Function to draw asteroid.
void Asteroid::draw() {
	glPushMatrix();
	glRotatef(angle, 0, 1, 0);
	glTranslatef(centerX, centerY, centerZ);
	glColor3f(color[0], color[1], color[2]);
	glutSolidSphere(radius, 50, 50);
	if (index == Earth) {
		glPushMatrix();
		glRotatef(angle_of_Moon, 0.0, 0.1, 0.05);
		glTranslatef(1.75, 0.0, 0.0);
		glColor3f(0.76, 0.76, 0.76);
		glutSolidSphere(0.25, 50, 50);
		glPopMatrix(); // End Moon.
	}
	else if (index == Saturn) {
		// draw Saturn ring
		glPushMatrix();
		glRotatef(90, 1.0, 0.0, 0.0);
		glPointSize(4);
		glColor3f(0.95, 0.95, 0.95);
		glScalef(2.5,2.5,2.5);
		glBegin(GL_POINTS);
		double ring_angle = 0.0;
		for (int j = 0; j < 360; j++) {
			glVertex2d(cos(ring_angle), sin(ring_angle));
			ring_angle += 2 * M_PI / 180;
		}
		glEnd();

		glPointSize(1);
		glPopMatrix(); // ring done
	}
	glPopMatrix();
}

Asteroid planets_array[Planets];

// Initialization routine.
void setup(void) {
	// Spacecraft display list.
	spacecraft = glGenLists(1);
	glNewList(spacecraft, GL_COMPILE);
	glPushMatrix();
	glRotatef(180.0, 0.0, 1.0, 0.0); // To make the spacecraft point down the $z$-axis initially.
	glColor3f(1.0, 1.0, 1.0);
	glutWireCone(0.1, 0.5, 10, 10);
	glPopMatrix();
	glEndList();

	// Create Planets
	planets_array[Mercury] = Asteroid(Mercury, 7.5, 0.0, 0.0, 0.4, 0.64, 0.64, 0.64, 0, 2);
	planets_array[Venus] = Asteroid(Venus, 10.0, 0.0, 0.0, 0.5, 0.95, 0.95, 0.44, 0, 0.9);
	planets_array[Earth] = Asteroid(Earth, 12.5, 0.0, 0.0, 1.15, 0.0, 0.5, 1.0, 0, 0.7);
	planets_array[Mars] = Asteroid(Mars, -15.0, 0.0, 0.0, 0.85, 0.78, 0.29, 0.0, 0, 0.5);
	planets_array[Jupiter] = Asteroid(Jupiter, -20.0, 0.0, 0.0, 2.5, 0.78, 0.49, 0.24, 0, 0.2);
	planets_array[Saturn] = Asteroid(Saturn, -25.0, 0.0, 0.0, 2, 0.84, 0.72, 0.48, 0, 0.1);
	planets_array[Uranus] = Asteroid(Uranus, 26, 0.0, 0.0, 1.15, 0.53, 0.81, 0.98, 0, 0.05);
	planets_array[Neptune] = Asteroid(Neptune, -28.5, 0.0, 0.0, 1, 0.0, 0.31, 0.78, 0, 0.02);


	float lightAmb[] = { 0.1, 0.1, 0.1, 1.0 }; // ambient light
	float lightDif[] = { 1.0, 1.0, 1.0, 1.0 }; // diffuse light
	float lightSpec[] = { .5, .5, .5, .1 }; // specular light
	glPointSize(1.0);
	glLineWidth(2.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, yellow); // Set the ambient lighting
	glMaterialfv(GL_FRONT, GL_SPECULAR, yellow); // Set the specular lighting
	glEnable(GL_LIGHTING); // Enable lighting
	glEnable(GL_LIGHT7); // Enable the seventh light source

	glLightfv(GL_LIGHT7, GL_AMBIENT, lightAmb); // Ambient light
	glLightfv(GL_LIGHT7, GL_DIFFUSE, lightDif); // Diffuse light
	glLightfv(GL_LIGHT7, GL_SPECULAR, lightSpec); // Specular light
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow); // Set the diffuse light component
}


void animate(int value) {
	if (isAnimate) {
		for (int i = 0; i < Planets; i++) {
			planets_array[i].angle += planets_array[i].angle_increment;
			if (planets_array[i].angle > 360) planets_array[i].angle -= 360;
		}
		// Moon rotation
		angle_of_Moon += 2;
		if (angle_of_Moon > 360) angle_of_Moon -= 360;
		glutPostRedisplay();
		glutTimerFunc(animationPeriod, animate, 0);
	}
}


// Function to check if two spheres centered at (x1,y1,z1) and (x2,y2,z2) with
// radius r1 and r2 intersect.
int checkSpheresIntersection(float x1, float y1, float z1, float r1,
	float x2, float y2, float z2, float r2) {
	return ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2) <= (r1 + r2) * (r1 + r2));
}

// Function to check if the spacecraft collides with an asteroid when the center of the base
// of the craft is at (x, 0, z) and it is aligned at an angle a to to the -z direction.
// Collision detection is approximate as instead of the spacecraft we use a bounding sphere.
int asteroidCraftCollision(float x, float z, float a) {
	// Check for collision with each asteroid.
	for (int i = 0; i < Planets; i++)
		if (checkSpheresIntersection(x - 0.5 * sin((M_PI / 180.0) * a), 0.0,
			z - 0.5 * cos((M_PI / 180.0) * a), 0.7072,
			planets_array[i].getCenterX(), planets_array[i].getCenterY(),
			planets_array[i].getCenterZ(), planets_array[i].getRadius()))
			return 1;
	// check for collision with the sun
	if (checkSpheresIntersection(x - 0.5 * sin((M_PI / 180.0) * a), 0.0,
		z - 0.5 * cos((M_PI / 180.0) * a), 0.7072, 0, 0, 0, 5)) return 1;
	return 0;
}

void drawOrbits() {
	float scale_of_planets[Planets] = { 7.5, 10, 12.5, 15, 20, 25, 26, 28.5 }; // scale of planets with respect to sun
	for (int i = 0; i < Planets; i++) {
		glPushMatrix();
		//glRotatef(63, 1.0, 0.0, 0.0);
		glScalef(scale_of_planets[i], scale_of_planets[i], scale_of_planets[i]);
		glBegin(GL_POINTS);
		double angle_of_rotation = 0.0;
		for (int j = 0; j < 360; j++) {
			glVertex3f(cos(angle_of_rotation), 0, sin(angle_of_rotation));
			angle_of_rotation += 2 * M_PI / 360;
		}
		glEnd();
		glPopMatrix();
	}
}


// Drawing routine..
void drawScene(void) {
	float lightPos[] = { 0, 0, 0, 0.1 }; // light position
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Begin plan viewport.
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
	glViewport(0, 0, width, height);
	glLoadIdentity();

	// Write text in isolated (i.e., before gluLookAt) translate block.
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos3f(-28.0, 25.0, -30.0);
	if (isCollision) writeBitmapString((void*)font, (char*)"Cannot - will crash!");
	glPopMatrix();

	// translate the scene
	glRotatef(Zangle, 0.0, 0.0, 1.0);
	glRotatef(Yangle, 0.0, 1.0, 0.0);
	glRotatef(Xangle, 1.0, 0.0, 0.0);

	// Locate the camera at the tip of the cone and pointing in the direction of the cone.	
	gluLookAt(xVal - 0.5 * sin((M_PI / 180.0) * space_craft_angle),
		0.0,
		zVal - 0.5 * cos((M_PI / 180.0) * space_craft_angle),
		xVal - 0.55 * sin((M_PI / 180.0) * space_craft_angle),
		0.0,
		zVal - 0.55 * cos((M_PI / 180.0) * space_craft_angle),
		0.0,
		1.0,
		0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	// Draw spacecraft.
	glPushMatrix();
	glTranslatef(xVal, 0.0, zVal);
	glRotatef(space_craft_angle, 0.0, 1.0, 0.0);
	glCallList(spacecraft);
	glPopMatrix();

	// Draw Sun.
	glPushMatrix();
	glColor3f(1.0, 0.96, 0.71);
	glLightfv(GL_LIGHT7, GL_POSITION, lightPos);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, yellow);
	glutSolidSphere(5, 50, 50);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
	glPopMatrix();

	for (int i = 0; i < Planets; i++)
		planets_array[i].draw();
	// End plan viewport.

	// Begin bottom right viewport.
	glEnable(GL_SCISSOR_TEST);
	glScissor(width * 2.0 / 3.0, 0, width / 3.0, height / 3.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
	glViewport(width * 2.0 / 3.0, 0, width / 3.0, height / 3.0);
	glLoadIdentity();

	// Write text in isolated (i.e., before gluLookAt) translate block.
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos3f(-28.0, 25.0, -30.0);
	if (isCollision) writeBitmapString((void*)font, (char*)"Cannot - will crash!");
	glPopMatrix();

	// Draw a vertical line on the left of the viewport to separate the two viewports
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glVertex3f(-5.0, -5.0, -5.0);
	glVertex3f(-5.0, 5.0, -5.0);
	glEnd();

	// Draw a horizontal line on the top of the viewport to separate the two viewports
	glBegin(GL_LINES);
	glVertex3f(-5.0, 5.0, -5.0);
	glVertex3f(5.0, 5.0, -5.0);
	glEnd();

	// Fixed camera.
	gluLookAt(0.0, 40.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	drawOrbits();

	// Draw spacecraft.
	glPushMatrix();
	glTranslatef(xVal, 0.0, zVal);
	glRotatef(space_craft_angle, 0.0, 1.0, 0.0);
	glCallList(spacecraft);
	glPopMatrix();

	// Draw Sun.
	glPushMatrix();
	glColor3f(1.0, 0.96, 0.71);
	glLightfv(GL_LIGHT7, GL_POSITION, lightPos);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, yellow);
	glutSolidSphere(5, 50, 50);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
	glPopMatrix();

	for (int i = 0; i < Planets; i++)
		planets_array[i].draw();

	// End bottom right viewport.
	glutSwapBuffers();
}


// OpenGL window reshape routine.
void resize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 250.0);
	glMatrixMode(GL_MODELVIEW);

	// Pass the size of the OpenGL window.
	width = w;
	height = h;
}

void handleMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			lastMouseX = x;
			lastMouseY = y;
			isDragging = true;
		}
		else if (state == GLUT_UP) {
			isDragging = false;
		}
	}
}

void handleMotion(int x, int y) {
	if (isDragging) {
		Xangle += (y - lastMouseY) * 0.2;
		Yangle += (x - lastMouseX) * 0.2;
		lastMouseX = x;
		lastMouseY = y;
		glutPostRedisplay();
	}
}


// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
	case ' ': // animation on and off.
		if (isAnimate) isAnimate = 0;
		else {
			isAnimate = 1;
			animate(1);
		}
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
	case '+': // increase animation speed
		if (animationPeriod > 5) animationPeriod -= 5;
		glutPostRedisplay();
		break;
	case '-': // decrease animation speed
		animationPeriod += 5;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}


// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y) {
	float tempxVal = xVal, tempzVal = zVal, tempAngle = space_craft_angle;

	// Compute next position.
	if (key == GLUT_KEY_LEFT) tempAngle = space_craft_angle + 5.0;
	if (key == GLUT_KEY_RIGHT) tempAngle = space_craft_angle - 5.0;
	if (key == GLUT_KEY_UP) {
		tempxVal = xVal - sin(space_craft_angle * M_PI / 180.0);
		tempzVal = zVal - cos(space_craft_angle * M_PI / 180.0);
	}
	if (key == GLUT_KEY_DOWN) {
		tempxVal = xVal + sin(space_craft_angle * M_PI / 180.0);
		tempzVal = zVal + cos(space_craft_angle * M_PI / 180.0);
	}

	// Angle correction.
	if (tempAngle > 360.0) tempAngle -= 360.0;
	if (tempAngle < 0.0) tempAngle += 360.0;

	// Move spacecraft to next position only if there will not be collision with an asteroid.
	if (!asteroidCraftCollision(tempxVal, tempzVal, tempAngle)) {
		isCollision = 0;
		xVal = tempxVal;
		zVal = tempzVal;
		space_craft_angle = tempAngle;
	}
	else isCollision = 1;

	glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void) {
	cout << "Interaction:\n"
		<< "Press space to toggle between animation on and off.\n"
		<< "Press the x, X, y, Y, z, Z keys to rotate the scene.\n"
		<< "Press the arrow keys to move the scene.\n"
		<< "Press + to increase animation speed and - to decrease animation speed.\n";
}

// Main routine.
int main(int argc, char** argv) {
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1480, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Course Project (Solar System Simulation)");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutMouseFunc(handleMouse);
	glutMotionFunc(handleMotion);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);

	glewExperimental = GL_TRUE;
	glewInit();
	setup();
	glutMainLoop();
}
