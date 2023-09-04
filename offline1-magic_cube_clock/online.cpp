#define _USE_MATH_DEFINES

// for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include<bits/stdc++.h>
#include <cmath>


using namespace std;

struct point {
    GLfloat x, y, z;
};

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

// Global variables
struct point pos;   // position of the eye
struct point l;     // look/forward direction
struct point r;     // right direction
struct point u;     // up direction

/* Draw axes: X in Red, Y in Green and Z in Blue */
void drawAxes() {
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);   // Red
    // X axis
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0);   // Green
    // Y axis
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1);   // Blue
    // Z axis
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);
    glEnd();
}

float scale = 0;
float scaleStep = 0.05f;
void drawOctahedron() {

    // Define the vertices of a triangle
    GLfloat vertices[] = {
        (1.0f * scale) / 3 , 1 - (2.0f * scale) / 3, (1.0f * scale) / 3 ,     // Top vertex
        1 - (2.0f * scale) / 3 ,(1.0f * scale) / 3  ,(1.0f * scale) / 3 ,    // Bottom left vertex
        (1.0f * scale) / 3, (1.0f * scale) / 3,  1 - (2.0f * scale) / 3    // Bottom right vertex
    };


    // Apply rotations and translations to create the octahedron

        // Draw the triangle
    for (int i = 1; i <= 4; ++i) {
        glPushMatrix();
        int j = max(i, i - 4);
        glRotatef((j - 1) * 90, 0.0f, 1.0f, 0.0f);  // Rotate 90 degrees around the Y-axis 
        if (i > 4) {
            // mirror it on xy plane
            glRotatef(180, 1.0f, 0.0f, 0.0f);

        }
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);  // Red
        glVertex3fv(vertices);  // Top vertex
        glColor3f(0.0f, 1.0f, 0.0f);  // Green
        glVertex3fv(vertices + 3);  // Bottom left vertex
        glColor3f(0.0f, 0.0f, 1.0f);  // Blue
        glVertex3fv(vertices + 6);  // Bottom right vertex
        glEnd();

        // glRotatef(i * 90.0f, 0.0f, 1.0f, 0.0f);  // Rotate 90 degrees around the Y-axis

        glPopMatrix();

    }


}




std::vector<float> buildUnitPositiveX(int subdivision)
{
    const float DEG2RAD = acos(-1) / 180.0f;

    std::vector<float> vertices;
    float n1[3];        // normal of longitudinal plane rotating along Y-axis
    float n2[3];        // normal of latitudinal plane rotating along Z-axis
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    float a1;           // longitudinal angle along Y-axis
    float a2;           // latitudinal angle along Z-axis

    // compute the number of vertices per row, 2^n + 1
    int pointsPerRow = (int)pow(2, subdivision) + 1;

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for (unsigned int i = 0; i < pointsPerRow; ++i)
    {
        // normal for latitudinal plane
        // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
        // therefore, it is rotating (0,1,0) vector by latitude angle a2
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
        for (unsigned int j = 0; j < pointsPerRow; ++j)
        {
            // normal for longitudinal plane
            // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
            // therefore, it is rotating (0,0,-1) vector by longitude angle a1
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            float scale = 1 / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            // add a vertex into array
            vertices.push_back(v[0]);
            vertices.push_back(v[1]);
            vertices.push_back(v[2]);
        }
    }


    return vertices;
}

void drawSphere() {

    float vertices[] = { 1 - scale , 0 , 0,
       0, 1 - scale, 0,
       0, 0, 1 - scale,
         -1 + scale, 0, 0,
            0, -1 + scale, 0,
            0, 0, -1 + scale
    };

    float radius = (1.0f * scale) / sqrt(3);
    //radius = radius * 2.0f;
    int subdivision = 5;
    float final_radius = radius; 
    for (int i = 0; i < 6; ++i) {

        if (i == 4) continue;
        
        glPushMatrix();
        glTranslatef(vertices[3 * i + 0], vertices[3 * i + 1], vertices[3 * i + 2]);
        glColor3f(1, 1, 0);
        if (i == 1) {
            // set the color to red
            glColor3f(1, 0, 0);
            glRotatef(90, 0, 0, 1);
        }
        if (i == 2) {
            // set the color to white
            glColor3f(1, 1, 1);
            glRotatef(-90, 0, 1, 0);
        }
        if (i == 3) {
            // set the color to blue
            glColor3f(0, 0, 1);
            glRotatef(180, 0, 1, 0);
        }
        if (i == 4) {
            // set the color to cyan
            glColor3f(0, 1, 1);
            glRotatef(-90, 0, 0, 1);
        }
        if (i == 5) {
            // set the color to magenta
            glColor3f(1, 0, 1);
            glRotatef(90, 0, 1, 0);
        }
        vector<float> vertices = buildUnitPositiveX(subdivision);

        for (int i = 0; i < vertices.size(); i++) {
            //  vertices[i] = vertices[i] * radius;
        }
        int pointsPerRow = (int)pow(2, subdivision) + 1;
        if (i != 1) {
            for (unsigned int i = 0; i < pointsPerRow / 2  - 1; ++i)
            {
                glBegin(GL_TRIANGLE_STRIP);

                for (unsigned int j = 0; j < pointsPerRow; ++j)
                {
                    // vertex 1
                    // yellow color

                    glNormal3f(vertices[3 * (i * pointsPerRow + j) + 0],
                        vertices[3 * (i * pointsPerRow + j) + 1],
                        vertices[3 * (i * pointsPerRow + j) + 2]);
                    glVertex3f(vertices[3 * (i * pointsPerRow + j) + 0] * radius,
                        vertices[3 * (i * pointsPerRow + j) + 1] * radius,
                        vertices[3 * (i * pointsPerRow + j) + 2] * radius);

                    // vertex 2
                    glNormal3f(vertices[3 * ((i + 1) * pointsPerRow + j) + 0],
                        vertices[3 * ((i + 1) * pointsPerRow + j) + 1],
                        vertices[3 * ((i + 1) * pointsPerRow + j) + 2]);
                    glVertex3f(vertices[3 * ((i + 1) * pointsPerRow + j) + 0] * radius,
                        vertices[3 * ((i + 1) * pointsPerRow + j) + 1] * radius,
                        vertices[3 * ((i + 1) * pointsPerRow + j) + 2] * radius);
                }
                glEnd();
            }
        }
        else {
            for (unsigned int i = 0; i < pointsPerRow - 1; ++i)
            {
                glBegin(GL_TRIANGLE_STRIP);

                for (unsigned int j = 0; j < pointsPerRow; ++j)
                {
                    // vertex 1
                    // yellow color

                    glNormal3f(vertices[3 * (i * pointsPerRow + j) + 0],
                        vertices[3 * (i * pointsPerRow + j) + 1],
                        vertices[3 * (i * pointsPerRow + j) + 2]);
                    glVertex3f(vertices[3 * (i * pointsPerRow + j) + 0] * radius,
                        vertices[3 * (i * pointsPerRow + j) + 1] * radius,
                        vertices[3 * (i * pointsPerRow + j) + 2] * radius);

                    // vertex 2
                    glNormal3f(vertices[3 * ((i + 1) * pointsPerRow + j) + 0],
                        vertices[3 * ((i + 1) * pointsPerRow + j) + 1],
                        vertices[3 * ((i + 1) * pointsPerRow + j) + 2]);
                    glVertex3f(vertices[3 * ((i + 1) * pointsPerRow + j) + 0] * radius,
                        vertices[3 * ((i + 1) * pointsPerRow + j) + 1] * radius,
                        vertices[3 * ((i + 1) * pointsPerRow + j) + 2] * radius);
                }
                glEnd();
            }
        }
        glPopMatrix();
    }

}


void drawCylinder() {

    float vertices[] = {
        (1.0f * scale) / 3 , 1 - (2.0f * scale) / 3, (1.0f * scale) / 3 ,     // Top vertex
        1 - (2.0f * scale) / 3 ,(1.0f * scale) / 3  ,(1.0f * scale) / 3 ,    // Bottom left vertex
        (1.0f * scale) / 3, (1.0f * scale) / 3,  1 - (2.0f * scale) / 3    // Bottom right vertex
    };
    float newposx = (vertices[0] + vertices[3]) / 2;
    float newposy = (vertices[1] + vertices[4]) / 2;
    float newposz = (vertices[2] + vertices[5]) / 2;
    float xheight = vertices[1] - vertices[0];
    float radius = vertices[0] * sqrt(3);
    float height = sqrt(2) * xheight;
    int segments = 50;
    double tempx = radius, tempy = 0;
    double currx, curry;
    float actual_radius = radius;
    for (int i = 0; i < 12; ++i) {
        glPushMatrix();
        if (i >= 4 and i <= 7) continue;
        if (i >= 8 and i <= 12) radius = actual_radius / 2;
        if (i == 8) height += 0.1f;
        glTranslatef((1 - scale) * 0.5f, (1 - scale) * 0.5f, 0);
        // here begins the transloation 
        if (i == 1) {
            glTranslatef(-(1 - scale) * 0.5f * 2.0f, 0, 0);
        }
        if (i == 2) {
            glTranslatef(-(1 - scale) * 0.5f, 0, -(1 - scale) * 0.5f);
        }

        if (i == 3) {
            glTranslatef(-(1 - scale) * 0.5f, 0, (1 - scale) * 0.5f);
        }

        if (i == 4) {
            glTranslatef(0, -(1 - scale) * 0.5f * 2.0f, 0);
        }

        if (i == 5) {
            glTranslatef(-(1 - scale) * 0.5f * 2.0f, -(1 - scale) * 0.5f * 2.0f, 0);
        }
        if (i == 6) {
            glTranslatef(-(1 - scale) * 0.5f, -(1 - scale) * 0.5f * 2.0f, (1 - scale) * 0.5f);
        }
        if (i == 7) {
            glTranslatef(-(1 - scale) * 0.5f, -(1 - scale) * 0.5f * 2.0f, -(1 - scale) * 0.5f);

        }
        if (i == 8) {
            glTranslatef(0, -(1 - scale) * 0.5f, (1 - scale) * 0.5f);
        }

        if (i == 9) {
            glTranslatef(0, -(1 - scale) * 0.5f, -(1 - scale) * 0.5f);
        }
        if (i == 10) {
            glTranslatef(-(1 - scale) * 0.5f * 2.0f, -(1 - scale) * 0.5f, -(1 - scale) * 0.5f);
        }

        if (i == 11) {
            glTranslatef(-(1 - scale) * 0.5f * 2.0f, -(1 - scale) * 0.5f, (1 - scale) * 0.5f);
        }

        glRotatef(90, 1, 0, 0);
        glRotatef(45, 0, 1, 0);
        if (i == 1) {
            glRotatef(180, 1, 0, 1);
        }
        if (i == 2 or i == 6) {
            glRotatef(-45, 0, 1, 0);
            glRotatef(45, 1, 0, 0);
        }
        if (i == 3 or i == 7) {
            glRotatef(-45, 0, 1, 0);
            glRotatef(-45, 1, 0, 0);
        }
        if (i == 4) {
            glRotatef(90, 0, 1, 0);
        }
        if (i == 8 or i == 10) {
            glRotatef(45, 0, 1, 0);
            glRotatef(45, 1, 0, 0);
        }
        if (i == 9 or i == 11) {
            glRotatef(45, 0, 1, 0);
            glRotatef(-45, 1, 0, 0);
        }

        glBegin(GL_QUADS);
        for (int i = 1; i <= segments; i++) {
            double theta = i * 2 * M_PI / segments;
            currx = radius * cos(theta);
            curry = radius * sin(theta);

            GLfloat c = (2 + cos(theta)) / 3;
            c = 1;
            glColor3f(c, c, c);
            glVertex3f(currx, curry, height / 2);
            glVertex3f(currx, curry, -height / 2);

            glVertex3f(tempx, tempy, -height / 2);
            glVertex3f(tempx, tempy, height / 2);

            tempx = currx;
            tempy = curry;
        }
        glEnd();

        glPopMatrix();
    }
}


/*  Handler for window-repaint event. Call back when the window first appears and
    whenever the window needs to be re-painted. */
GLfloat x = 0, y = 0, z = 0;
GLfloat rotationAngle = 0.0f;
GLfloat rotationRadius = 0.01f;
void display() {
    // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix




    // control viewing (or camera)
    gluLookAt(pos.x, pos.y, pos.z,
        pos.x + l.x, pos.y + l.y, pos.z + l.z,
        u.x, u.y, u.z);
    // draw
    //drawSphere(5,100,100);
    //drawCone(10, 5, 10);
    glPushMatrix();
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);
    // drawOctahedron();
    // drawAxes();
    // drawCylinder();
    drawSphere();
    glPopMatrix();
    glutSwapBuffers();  // Render now
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    /*if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }*/
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}


void clockWise() {

    rotationAngle = 10.0f;
    GLfloat rotationAngleRad = rotationAngle * (M_PI / 180.0);

    // Calculate the distance between the camera position and the origin
    GLfloat distanceToOrigin = sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);

    // Rotate the camera position around the origin
    GLfloat newX = cos(rotationAngleRad) * pos.x + sin(rotationAngleRad) * pos.z;
    GLfloat newZ = -sin(rotationAngleRad) * pos.x + cos(rotationAngleRad) * pos.z;
    pos.x = newX;
    pos.z = newZ;

    // Recalculate the camera's forward direction
    l.x = -pos.x / distanceToOrigin;
    l.y = -pos.y / distanceToOrigin;
    l.z = -pos.z / distanceToOrigin;

    // Recalculate the camera's up direction
    // u.x = l.y * r.z - l.z * r.y;
    // u.y = l.z * r.x - l.x * r.z;
    // u.z = l.x * r.y - l.y * r.x;

    // // Recalculate the camera's right direction
    // r.x = u.y * l.z - u.z * l.y;
    // r.y = u.z * l.x - u.x * l.z;
    // r.z = u.x * l.y - u.y * l.x;

    // // Normalize the right direction vector
    // GLfloat rLength = sqrt(r.x * r.x + r.y * r.y + r.z * r.z);
    // r.x /= rLength;
    // r.y /= rLength;
    // r.z /= rLength;

    // r.x = 0.7;
    // r.y = 0;
    // r.z = 0.7;





}


void counterClockwise() {
    rotationAngle = 10.0f;
    GLfloat rotationAngleRad = rotationAngle * (M_PI / 180.0);

    // Calculate the distance between the camera position and the origin
    GLfloat distanceToOrigin = sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);

    // Rotate the camera position around the origin
    GLfloat newX = cos(rotationAngleRad) * pos.x - sin(rotationAngleRad) * pos.z;
    GLfloat newZ = sin(rotationAngleRad) * pos.x + cos(rotationAngleRad) * pos.z;
    pos.x = newX;
    pos.z = newZ;
    // Recalculate the camera's forward direction
    l.x = -pos.x / distanceToOrigin;
    l.y = -pos.y / distanceToOrigin;
    l.z = -pos.z / distanceToOrigin;

    // Recalculate the camera's up direction
    // u.x = l.y * r.z - l.z * r.y;
    // u.y = l.z * r.x - l.x * r.z;
    // u.z = l.x * r.y - l.y * r.x;

    // // Recalculate the camera's right direction
    // r.x = u.y * l.z - u.z * l.y;
    // r.y = u.z * l.x - u.x * l.z;
    // r.z = u.x * l.y - u.y * l.x;

    // // Normalize the right direction vector
    // GLfloat rLength = sqrt(r.x * r.x + r.y * r.y + r.z * r.z);
    // r.x /= rLength;
    // r.y /= rLength;
    // r.z /= rLength;

    // r.x = 0.7;
    // r.y = 0;
    // r.z = 0.7;


}
void keyboardListener(unsigned char key, int xx, int yy) {
    double rate = 0.01;
    switch (key) {

    case '1':
        r.x = r.x * cos(rate) + l.x * sin(rate);
        r.y = r.y * cos(rate) + l.y * sin(rate);
        r.z = r.z * cos(rate) + l.z * sin(rate);

        l.x = l.x * cos(rate) - r.x * sin(rate);
        l.y = l.y * cos(rate) - r.y * sin(rate);
        l.z = l.z * cos(rate) - r.z * sin(rate);
        break;

    case '2':
        r.x = r.x * cos(-rate) + l.x * sin(-rate);
        r.y = r.y * cos(-rate) + l.y * sin(-rate);
        r.z = r.z * cos(-rate) + l.z * sin(-rate);

        l.x = l.x * cos(-rate) - r.x * sin(-rate);
        l.y = l.y * cos(-rate) - r.y * sin(-rate);
        l.z = l.z * cos(-rate) - r.z * sin(-rate);
        break;

    case '3':
        l.x = l.x * cos(rate) + u.x * sin(rate);
        l.y = l.y * cos(rate) + u.y * sin(rate);
        l.z = l.z * cos(rate) + u.z * sin(rate);

        u.x = u.x * cos(rate) - l.x * sin(rate);
        u.y = u.y * cos(rate) - l.y * sin(rate);
        u.z = u.z * cos(rate) - l.z * sin(rate);
        break;

    case '4':
        l.x = l.x * cos(-rate) + u.x * sin(-rate);
        l.y = l.y * cos(-rate) + u.y * sin(-rate);
        l.z = l.z * cos(-rate) + u.z * sin(-rate);

        u.x = u.x * cos(-rate) - l.x * sin(-rate);
        u.y = u.y * cos(-rate) - l.y * sin(-rate);
        u.z = u.z * cos(-rate) - l.z * sin(-rate);
        break;

    case '5':
        u.x = u.x * cos(rate) + r.x * sin(rate);
        u.y = u.y * cos(rate) + r.y * sin(rate);
        u.z = u.z * cos(rate) + r.z * sin(rate);

        r.x = r.x * cos(rate) - u.x * sin(rate);
        r.y = r.y * cos(rate) - u.y * sin(rate);
        r.z = r.z * cos(rate) - u.z * sin(rate);
        break;

    case '6':
        u.x = u.x * cos(-rate) + r.x * sin(-rate);
        u.y = u.y * cos(-rate) + r.y * sin(-rate);
        u.z = u.z * cos(-rate) + r.z * sin(-rate);

        r.x = r.x * cos(-rate) - u.x * sin(-rate);
        r.y = r.y * cos(-rate) - u.y * sin(-rate);
        r.z = r.z * cos(-rate) - u.z * sin(-rate);
        break;
    case 'a':
        //rotate the camera clockwise
        rotationAngle += 10;
        //clockWise();


        break;
    case 'd':
        rotationAngle -= 10;
        //counterClockwise();
        break;
    case '7':

        break;
    case ',':
        scale += scaleStep;
        if (scale > 1.0f)
            scale = 1.0f;
        break;
    case '.':
        scale -= scaleStep;
        if (scale < 0.0f)
            scale = 0.0f;
        break;

    default:
        break;
    }
    glutPostRedisplay();
}


void specialKeyListener(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_UP:		//down arrow key
        pos.x += l.x;
        pos.y += l.y;
        pos.z += l.z;
        break;
    case GLUT_KEY_DOWN:		// up arrow key
        pos.x -= l.x;
        pos.y -= l.y;
        pos.z -= l.z;
        break;

    case GLUT_KEY_RIGHT:
        pos.x += r.x;
        pos.y += r.y;
        pos.z += r.z;
        break;
    case GLUT_KEY_LEFT:
        pos.x -= r.x;
        pos.y -= r.y;
        pos.z -= r.z;
        break;

    case GLUT_KEY_PAGE_UP:
        pos.x += u.x;
        pos.y += u.y;
        pos.z += u.z;
        break;
    case GLUT_KEY_PAGE_DOWN:
        pos.x -= u.x;
        pos.y -= u.y;
        pos.z -= u.z;
        break;

    case GLUT_KEY_INSERT:
        break;

    case GLUT_KEY_HOME:
        break;
    case GLUT_KEY_END:
        pos.x = 3;pos.y = 3; pos.z = 3;
        l.x = -1;l.y = -1;l.z = -1;
        u.x = 0;u.y = 1;u.z = 0;
        r.x = 0.3;r.y = 0;r.z = -0.3;

    default:
        break;
    }
    glutPostRedisplay();
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    pos.x = 3;pos.y = 3; pos.z = 3;

    l.x = -1;l.y = -1;l.z = -1;
    u.x = 0;u.y = 1;u.z = 0;
    r.x = 0.707;r.y = 0;r.z = -0.707;

    glutInit(&argc, argv);                  // Initialize GLUT
    glutInitWindowSize(640, 640);           // Set the window's initial width & height
    glutInitWindowPosition(50, 50);         // Position the window's initial top-left corner
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("OpenGL 3D Drawing 2");          // Create a window with the given title
    glutDisplayFunc(display);               // Register display callback handler for window re-paint
    glutReshapeFunc(reshape);               // Register callback handler for window re-shape

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);

    initGL();                               // Our own OpenGL initialization
    glutMainLoop();                         // Enter the event-processing loop
    return 0;
}
