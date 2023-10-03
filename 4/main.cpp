// Draw "Gamal" using DDA and Bresenham line drawing algorithms
#include <cmath>
#include <iostream>
#include <glew.h>
#include <freeglut.h>
#include <glut.h>
#include <vector>
using namespace std;

const int width = 1440;
const int height = 540;

#define VERTICES 0
#define INDICES 1
#define SHAPE 0


static vector<float> vertices = {};
static vector<float> colors = {};

static unsigned int buffer[1];
static unsigned int vao[1];

void setup();
void drawScene();
void drawDDA();
void drawBresenham();
void animate(int someValue);


// function to initialize OpenGL
void setup() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    drawDDA();
    drawBresenham();

    glGenVertexArrays(1, vao); // Generate VAO ids.

    // draw GAMAL
    glBindVertexArray(vao[SHAPE]); // Bind the first VAO.
    glGenBuffers(1, buffer);
        
    glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) + colors.size() * sizeof(float), NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
    glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), colors.size() * sizeof(float), colors.data());

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, 0);    
    glColorPointer(3, GL_FLOAT, 0, (void*)(vertices.size() * sizeof(float)));
    glutTimerFunc(5, animate, 1);
}

void drawDDA() {
    // G points
    vector<pair<float, float>> points = {
        {100.0 * 2 / 15, 100.0 / 2}, {100.0 * 3 / 15, 100.0 / 2},
        {100.0 * 3 / 15, 100.0 / 2 - 100.0 / 15}, {100.0 * 1 / 15, 100.0 / 2 - 100.0 / 15},
        {100.0 * 1 / 15, 100.0 / 2 + 100.0 / 15}, {100.0 * 3 / 15, 100.0 / 2 + 100.0 / 15}
    };

    float x1, y1, x2, y2, dx, dy, steps, x, y, x_increment, y_increment;
    for (int i = 0; i < points.size() - 1; i++) {
        x1 = points[i].first; y1 = points[i].second;
        x2 = points[i + 1].first; y2 = points[i + 1].second;
        // DDA
        dx = x2 - x1;
        dy = y2 - y1;
        steps = max(abs(dx), abs(dy));
        x_increment = dx / steps;
        y_increment = dy / steps;
        x = x1;
        y = y1;

        for (int i = 0; i < steps; i++) {
            colors.push_back(i / steps); colors.push_back(0); colors.push_back(1 - i / steps);
            vertices.push_back(x); vertices.push_back(y); vertices.push_back(0.0);
            x += x_increment;
            y += y_increment;
        }
    }
    // A points
    points = {
        {100.0 * 10 / 15, 100.0 / 2 - 100.0 / 15}, {100.0 * 11 / 15, 100.0 / 2 + 100.0 / 15},
        {100.0 * 12 / 15, 100.0 / 2 - 100.0 / 15}
    };
    for (int i = 0; i < points.size() - 1; i++) {
        x1 = points[i].first; y1 = points[i].second;
        x2 = points[i + 1].first; y2 = points[i + 1].second;
        // DDA
        dx = x2 - x1;
        dy = y2 - y1;
        steps = max(abs(dx), abs(dy));
        x_increment = dx / steps;
        y_increment = dy / steps;
        x = x1;
        y = y1;

        for (int i = 0; i < steps; i++) {
            colors.push_back(i / steps); colors.push_back(0); colors.push_back(1 - i / steps);
            vertices.push_back(x); vertices.push_back(y); vertices.push_back(0.0);
            x += x_increment;
            y += y_increment;
        }
    }
    x1 = 100.0 * 10.5 / 15; y1 = 100.0 / 2;
    x2 = 100.0 * 11.5 / 15; y2 = 100.0 / 2;
    // DDA
    dx = x2 - x1;
    dy = y2 - y1;
    steps = max(abs(dx), abs(dy));
    x_increment = dx / steps;
    y_increment = dy / steps;
    x = x1;
    y = y1;

    for (int i = 0; i < steps; i++) {
        colors.push_back(i / steps); colors.push_back(0); colors.push_back(1 - i / steps);
        vertices.push_back(x); vertices.push_back(y); vertices.push_back(0.0);
        x += x_increment;
        y += y_increment;
    }
    // L points
    points = {
        {100.0 * 13 / 15, 100.0 / 2 + 100.0 / 15}, {100.0 * 13 / 15, 100.0 / 2 - 100.0 / 15},
        {100.0 * 14 / 15, 100.0 / 2 - 100.0 / 15}
    };

    for (int i = 0; i < points.size() - 1; i++) {
        x1 = points[i].first; y1 = points[i].second;
        x2 = points[i + 1].first; y2 = points[i + 1].second;
        // DDA
        dx = x2 - x1;
        dy = y2 - y1;
        steps = max(abs(dx), abs(dy));
        x_increment = dx / steps;
        y_increment = dy / steps;
        x = x1;
        y = y1;

        for (int i = 0; i < steps; i++) {
            colors.push_back(i / steps); colors.push_back(0); colors.push_back(1 - i / steps);
            vertices.push_back(x); vertices.push_back(y); vertices.push_back(0.0);
            x += x_increment;
            y += y_increment;
        }
    }
}

void drawBresenham() {    
    // draw A
    vector<pair<float, float>> points = {
        {100.0 * 4 / 15, 100.0 / 2 - 100.0 / 15}, {100.0 * 5 / 15, 100.0 / 2 + 100.0 / 15},
        {100.0 * 6 / 15, 100.0 / 2 - 100.0 / 15}
    };
    
    int x1, y1, x2, y2, dx, dy, sx, sy, err, e2;
    float x, y;
    for (int i = 0; i < points.size() - 1; i++) {
        x1 = points[i].first; y1 = points[i].second;
        x2 = points[i + 1].first; y2 = points[i + 1].second;
        // Bresenham
        dx = abs(x2 - x1); dy = abs(y2 - y1);
        sx = (x1 < x2) ? 1 : -1; sy = (y1 < y2) ? 1 : -1;
        err = dx - dy;
        x = x1; y = y1;
        while (true) {
            colors.push_back((x - x1) / (x2 - x1)); colors.push_back(0); colors.push_back(1 - (x - x1) / (x2 - x1));
            vertices.push_back(x); vertices.push_back(y); vertices.push_back(0.0);
            if (x == x2 && y == y2)
                break;
            e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x += sx;
            }
            if (e2 < dx) {
                err += dx;
                y += sy;
            }
        }
    }        
    x1 = 100.0 * 4.5 / 15;
    y1 = 100.0 / 2;
    x2 = 100.0 * 5.5 / 15;
    y2 = 100.0 / 2;
    // Bresenham
    dx = abs(x2 - x1); dy = abs(y2 - y1);
    sx = (x1 < x2) ? 1 : -1; sy = (y1 < y2) ? 1 : -1;
    err = dx - dy;
    x = x1; y = y1;
    while (true) {
        colors.push_back((x - x1) / (x2 - x1)); colors.push_back(0); colors.push_back(1 - (x - x1) / (x2 - x1));
        vertices.push_back(x); vertices.push_back(y); vertices.push_back(0.0);
        if (x == x2 && y == y2)
            break;
        e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }    

    // draw M
    points = {
        {100.0 * 7 / 15, 100.0 / 2 - 100.0 / 15}, {100.0 * 7 / 15, 100.0 / 2 + 100.0 / 15},
        {100.0 * 8 / 15, 100.0 / 2}, {100.0 * 9 / 15, 100.0 / 2 + 100.0 / 15},
        {100.0 * 9 / 15, 100.0 / 2 - 100.0 / 15}
    };
    for (int i = 0; i < points.size() - 1; i++) {
        x1 = points[i].first; y1 = points[i].second;
        x2 = points[i + 1].first; y2 = points[i + 1].second;
        // Bresenham
        dx = abs(x2 - x1); dy = abs(y2 - y1);
        sx = (x1 < x2) ? 1 : -1; sy = (y1 < y2) ? 1 : -1;
        err = dx - dy;
        x = x1; y = y1;
        while (true) {
            colors.push_back((x - x1) / (x2 - x1)); colors.push_back(0); colors.push_back(1 - (x - x1) / (x2 - x1));
            vertices.push_back(x); vertices.push_back(y); vertices.push_back(0.0);
            if (x == x2 && y == y2)
                break;
            e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x += sx;
            }
            if (e2 < dx) {
                err += dx;
                y += sy;
            }
        }
    }
}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    float* bufferData = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    for (int i = 0; i < colors.size(); i++)
        bufferData[vertices.size() + i] = (float)rand() / (float)RAND_MAX;
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glPointSize(5);   
    glBindVertexArray(vao[SHAPE]);
    glDrawArrays(GL_POINTS, 0, vertices.size() / 3);
    glutSwapBuffers();
    glFlush();
}

void animate(int someValue) {
    glutPostRedisplay();
    glutTimerFunc(500, animate, 1);
}

// OpenGL window reshape routine.
void resize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {
    switch (key) {
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

// Main routine.
int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 100);
    glutCreateWindow("GAMAL");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);

    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
}
