#include<bits/stdc++.h>
#include <GL/glut.h>
#include <cmath>

// Global variables
int width = 800, height = 600; // window size
float angle = 45.0f;           // angle of the pendulum
float length = 300.0f;         // length of the pendulum
float dt = 0.1f;               // time step

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // set background color
}

void draw_pendulum()
{
    glColor3f(1.0f, 1.0f, 1.0f); // set pendulum color

    // Calculate the position of the bob
    float x = length * sin(angle);
    float y = -length * cos(angle);

    // Draw the pendulum rod
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(x, y);
    glEnd();

    // Draw the bob
    glTranslatef(x, y, 0.0f);
    glutSolidSphere(10, 20, 20);
    glTranslatef(-x, -y, 0.0f);
}

void update(int value)
{
    // Update the angle based on the laws of physics (simple harmonic motion)
    float angular_velocity = sqrt(9.8f / length) * sin(angle);
    angle += angular_velocity * dt;

    glutPostRedisplay(); // Trigger a window update

    glutTimerFunc(dt * 1000, update, 0); // Schedule the next update
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    gluOrtho2D(-width / 2, width / 2, -height / 2, height / 2);

    draw_pendulum();

    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Pendulum");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(dt * 1000, update, 0); // Schedule the first update
    glutMainLoop();
    return 0;
}
