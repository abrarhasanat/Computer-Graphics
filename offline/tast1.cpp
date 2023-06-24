#include <GL/glut.h>
#include <ctime>
#include <cmath>
#include<bits/stdc++.h>
#define pi acos(-1.0)
int hour, minute, second;

float theta = 1.0f; // Angle of the pendulum
float length = 0.4f; // Length of the pendulum arm
float gravity = 0.098f; // Acceleration due to gravity

void drawClock() {
    // Draw clock face
    glColor3f(0.9, 0.9, 0.9);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f); // Center of the clock
    for (int i = 0; i <= 360; i++) {
        float angle = 3.14159f * i / 180.0f;
        float x = 0.45 * cos(angle);
        float y = 0.45 * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    // Draw clock border
    glColor3f(0.3, 0.3, 0.3);
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= 360; i++) {
        float angle = 3.14159f * i / 180.0f;
        float x = 0.5 * cos(angle);
        float y = 0.5 * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    // Draw hour hand
    glColor3f(0.2, 0.2, 0.2);
    glPushMatrix();
    glRotatef(-(hour + minute / 60.0f) * 30.0f, 0.0f, 0.0f, 1.0f);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 0.2f);
    glEnd();
    glPopMatrix();

    // Draw minute hand
    glColor3f(0.2, 0.2, 0.2);
    glPushMatrix();
    glRotatef(-minute * 6.0f, 0.0f, 0.0f, 1.0f);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 0.3f);
    glEnd();
    glPopMatrix();

    // Draw second hand
    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glRotatef(-second * 6.0f, 0.0f, 0.0f, 1.0f);
    glLineWidth(1.0);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 0.4f);
    glEnd();
    glPopMatrix();

    // Draw number labels
    glColor3f(0.2, 0.2, 0.2);
    glPointSize(5.0);
    int j = -1;
    for (int i = 1; i <= 12; ++i) {
        float angle = 3.14159f * (j + 3) / 6.0f; // Adjust the angle offset to position numbers correctly
        float x = 0.38 * cos(angle);
        float y = 0.38 * sin(angle);

        // Convert integer to string
        std::string number = std::to_string(i);

        // Set position and render the number
        glRasterPos2f(x - 0.02, y - 0.02); // Adjust the position for better alignment
        for (char& c : number) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
        --j;
    }

    // Draw decorative elements
    glColor3f(0.5, 0.5, 0.5);
    glPointSize(3.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < 360; i += 6) {
        float angle = 3.14159f * i / 180.0f;
        float x = 0.42 * cos(angle);
        float y = 0.42 * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    glColor3f(0.5, 0.5, 0.5);
    glLineWidth(1.0);
    glBegin(GL_LINES);
    for (int i = 0; i < 360; i += 30) {
        float angle = 3.14159f * i / 180.0f;
        float x1 = 0.47 * cos(angle);
        float y1 = 0.47 * sin(angle);
        float x2 = 0.5 * cos(angle);
        float y2 = 0.5 * sin(angle);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    }
    glEnd();

    glFlush();
}



float ts = 0;
float A = 0.1;
float y = -length;
void drawPendulum() {
    // Draw pendulum arm 
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f);
    float th = ts * 2 * pi - M_1_PI / 2;
    float x = A * sin(th);
    y = -length * cos(asin(x / length));
    glVertex2f(x, y);
    glEnd();

    // Draw pendulum bob
    glTranslatef(x, y, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glutSolidSphere(0.05f, 20, 20);

    ts += 0.01f;

    glFlush();
}
// void drawPendulum() {
//     // Draw pendulum arm
//     glColor3f(0.0f, 0.0f, 0.0f);
//     glBegin(GL_LINES);
//     glVertex2f(0.0f, 0.0f);

//     glVertex2f(length * sin(theta), -length * cos(theta));
//     glEnd();

//     // Draw pendulum bob
//     glTranslatef(length * sin(theta), -length * cos(theta), 0.0f);
//     glColor3f(0.0f, 0.0f, 1.0f);
//     glutSolidSphere(0.05f, 20, 20);

//     glFlush();
// }

void updateClock(int value) {
    time_t currentTime = time(0);
    struct tm* localTime = localtime(&currentTime);

    // Get the current time
    hour = localTime->tm_hour;
    minute = localTime->tm_min;
    second = localTime->tm_sec;

    glutPostRedisplay(); // Mark the window for redrawing
    glutTimerFunc(1000, updateClock, 0); // Update clock every second
}

void updatePendulum(int value) {
    // Update the angle of the pendulum using the equation of motion
    float angularAcceleration = -(gravity / length) * sin(theta);
    float timeStep = 0.01f;
    theta += timeStep * angularAcceleration;

    glutPostRedisplay(); // Mark the window for redrawing
    glutTimerFunc(10, updatePendulum, 0); // Update pendulum position every 10 milliseconds
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw hexagonal frame
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
    float radius = 0.85f;
    for (int i = 0; i < 6; ++i) {
        float angle = 2.0f * pi * i / 6.0f;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    glColor3f(0.85, 0.85, 0.85);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 6; ++i) {
        float angle = 2.0f * pi * i / 6.0f;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    // Draw clock
    glPushMatrix();
    glTranslatef(0.0f, 0.23f, 0.0f); // Adjust the position of the clock
    drawClock();
    glPopMatrix();

    // Draw pendulum
    glPushMatrix();
    glTranslatef(0.0f, -0.27f, 0.0f); // Adjust the position of the pendulum
    drawPendulum();
    glPopMatrix();

    glutSwapBuffers();
}


void reshape(int width, int height) {
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Clock and Pendulum");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, updateClock, 0); // Start the clock update timer
    glutTimerFunc(0, updatePendulum, 0); // Start the pendulum swing

    glutMainLoop();
    return 0;
}
