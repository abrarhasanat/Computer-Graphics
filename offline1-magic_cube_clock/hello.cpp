#include <GL/glut.h>
#include <cmath>
#include <ctime>

// Constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const float CLOCK_RADIUS = 0.4f;
const float HOUR_HAND_LENGTH = 0.3f;
const float MINUTE_HAND_LENGTH = 0.4f;
const float SECOND_HAND_LENGTH = 0.45f;

// Camera parameters
GLfloat cameraPositionX = 0.0f;
GLfloat cameraPositionY = 0.0f;
GLfloat cameraPositionZ = 2.0f;

GLfloat lookAtX = 0.0f;
GLfloat lookAtY = 1.0f;
GLfloat lookAtZ = 0.0f;

// Function to draw the clock
void drawClock() {
    // Get the current time
    time_t rawTime;
    struct tm* timeInfo;
    time(&rawTime);
    timeInfo = localtime(&rawTime);

    // Clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPositionX, cameraPositionY, cameraPositionZ, lookAtX, lookAtY, lookAtZ, 0.0f, 1.0f, 0.0f);

    // Draw the clock face
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glutSolidSphere(CLOCK_RADIUS, 50, 50);
    glPopMatrix();

    // Draw the hour hand
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glRotatef(-timeInfo->tm_hour * 30.0f - timeInfo->tm_min * 0.5f, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 0.0f, -0.05f);
    glScalef(0.03f, HOUR_HAND_LENGTH, 0.03f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Draw the minute hand
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glRotatef(-timeInfo->tm_min * 6.0f - timeInfo->tm_sec * 0.1f, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 0.0f, -0.08f);
    glScalef(0.02f, MINUTE_HAND_LENGTH, 0.02f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Draw the second hand
    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glRotatef(-timeInfo->tm_sec * 6.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 0.0f, -0.1f);
    glScalef(0.01f, SECOND_HAND_LENGTH, 0.01f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Draw clock markings
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    for (int i = 0; i < 12; ++i) {
        glPushMatrix();
        glRotatef(i * 30.0f, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0f, CLOCK_RADIUS - 0.02f, 0.0f);
        glScalef(0.01f, 0.04f, 0.01f);
        glutSolidCube(1.0f);
        glPopMatrix();
    }
    glPopMatrix();

    // Draw clock numbers
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.3f, 0.0f);
    for (int i = 1; i <= 12; ++i) {
        glPushMatrix();
        glRotatef(i * 30.0f, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0f, 0.0f, -0.05f);
        glScalef(0.1f, 0.1f, 0.1f);
        glutStrokeCharacter(GLUT_STROKE_ROMAN, i + '0');
        glPopMatrix();
    }
    glPopMatrix();

    // Swap buffers to display the clock
    glutSwapBuffers();
}

// Function to handle window resizing
void handleResize(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
}

int main(int argc, char** argv) {
    // Initialize OpenGL and GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("3D Clock");

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Register callback functions
    glutDisplayFunc(drawClock);
    glutReshapeFunc(handleResize);

    // Enter the main loop
    glutMainLoop();

    return 0;
}
