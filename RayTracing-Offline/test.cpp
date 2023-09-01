#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// Global variables
double nearDistance = 1, farDistance = 1000, fovY = 80, aspectRatio = 1;
double widthOfEachCell = 50;
double ambientForCheckerBoard = 0.1, diffuseForCheckerBoard = 0.1, reflectionForCheckerBoard = 0.8;
int levelOfRecursion = 3, pixelsAlongX = 768, pixelsAlongY = 768;
int numberOfObjects = 0;

struct point {
    GLfloat x, y, z;
};

point pos = { 100, 100, 100 };
point l = { -1, -1, -1 };
point r, u;

void readInputFile() {
    // Your readInputFile code here
    // ...
}

void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovY, aspectRatio, nearDistance, farDistance);
    glMatrixMode(GL_MODELVIEW);
}

void drawCheckerBoard() {
    for (int z = -5; z <= 5; ++z) {
        for (int x = -5; x <= 5; ++x) {
            bool isWhite = (z + x) % 2 == 0;
            if (isWhite) {
                glColor3f(1, 1, 1); // White
            }
            else {
                glColor3f(0, 0, 0); // Black
            }

            glBegin(GL_QUADS);
            glVertex3f(x * widthOfEachCell, 0, z * widthOfEachCell);
            glVertex3f((x + 1) * widthOfEachCell, 0, z * widthOfEachCell);
            glVertex3f((x + 1) * widthOfEachCell, 0, (z + 1) * widthOfEachCell);
            glVertex3f(x * widthOfEachCell, 0, (z + 1) * widthOfEachCell);
            glEnd();
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(pos.x, pos.y, pos.z, pos.x + l.x, pos.y + l.y, pos.z + l.z, 0, 1, 0);

    drawCheckerBoard();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    readInputFile();

    glutInit(&argc, argv);
    glutInitWindowSize(pixelsAlongX, pixelsAlongY);
    glutInitWindowPosition(50, 50);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("OpenGL 3D Drawing");
    glutDisplayFunc(display);

    initGL();

    glutMainLoop();
    return 0;
}
