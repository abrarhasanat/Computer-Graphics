// for MS Windows
#include<bits/stdc++.h>
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
using namespace std;
/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

// Global variables
GLfloat eyex = 2, eyey = 2, eyez = 2;
GLfloat centerx = 0, centery = 0, centerz = 0;
GLfloat upx = 0, upy = 1, upz = 0;
bool isAxes = true, isCube = false, isPyramid = false;
bool isOctahedron = false;
/* Draw axes: X in Red, Y in Green and Z in Blue */



double triangleScale = 100;
double cylinderHeight = 100;
double cylinderRadius = 0;

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

/* Draw a cube centered at the origin */
void drawCube() {
    glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
    // Top face (y = 1.0f)
    // Define vertices in counter-clockwise (CCW) order with normal pointing out
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    // Bottom face (y = -1.0f)
    glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Front face  (z = 1.0f)
    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Back face (z = -1.0f)
    glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Left face (x = -1.0f)
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Right face (x = 1.0f)
    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();  // End of drawing color-cube
}

double xheight = 1.0f;
double radius = 0.01f;
float scale = 0;
void drawCylinder() {

    float vertices[] = {
        (1.0f * scale) / 3 , 1 - (2.0f * scale) / 3, (1.0f * scale) / 3 ,     // Top vertex
        1 - (2.0f * scale) / 3 ,(1.0f * scale) / 3  ,(1.0f * scale) / 3 ,    // Bottom left vertex
        (1.0f * scale) / 3, (1.0f * scale) / 3,  1 - (2.0f * scale) / 3    // Bottom right vertex
    };
    float newposx = (vertices[0] + vertices[3]) / 2;
    float newposy = (vertices[1] + vertices[4]) / 2;
    float newposz = (vertices[2] + vertices[5]) / 2;
    xheight = vertices[1] - vertices[0];
    radius = vertices[0] * sqrt(3);
    float height = sqrt(2) * xheight;
    int segments = 50;
    double tempx = radius, tempy = 0;
    double currx, curry;
    for (int i = 0; i < 1; ++i) {
        glPushMatrix();
        glTranslatef( (1- scale) * 0.5f , (1- scale) * 0.5f , 0 );
        glRotatef(90, 1, 0, 0);
        glRotatef(45, 0, 1, 0);
        if (i != 0) glRotatef(90 * i, 0, 1, 0);
        glBegin(GL_QUADS);
        for (int i = 1; i <= segments; i++) {
            double theta = i * 2.0 * M_PI / segments;
            currx = radius * cos(theta);
            curry = radius * sin(theta);

            GLfloat c = (2 + cos(theta)) / 3;
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
struct point {
    GLfloat x, y, z;
};

void drawSphere() {
    
}


void drawOctahedron() {
   
    // Define the vertices of a triangle
    GLfloat vertices[] = {
        (1.0f * scale) / 3 , 1 - (2.0f * scale) / 3, (1.0f * scale) / 3 ,     // Top vertex
        1 - (2.0f * scale) / 3 ,(1.0f * scale) / 3  ,(1.0f * scale) / 3 ,    // Bottom left vertex
        (1.0f * scale) / 3, (1.0f * scale) / 3,  1 - (2.0f * scale) / 3    // Bottom right vertex
    };
    

    // Apply rotations and translations to create the octahedron

        // Draw the triangle
    for (int i = 1; i <= 8; ++i) {
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




/* Draw a pyramid centered at the origin */
void drawPyramid() {
   
}

/*  Handler for window-repaint event. Call back when the window first appears and
    whenever the window needs to be re-painted. */
void display() {
    // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix

    // default arguments of gluLookAt
    // gluLookAt(0,0,0, 0,0,-100, 0,1,0);

    // control viewing (or camera)
    gluLookAt(eyex, eyey, eyez,
        centerx, centery, centerz,
        upx, upy, upz);
    // draw

    if (isAxes) drawAxes();
    if (isCube) drawCube();
    if (isPyramid) drawPyramid();
    if (isOctahedron) drawOctahedron();
    drawCylinder();
    //drawSphere();

    glutSwapBuffers();  // Render now
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshapeListener(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();
    
    // Reset the projection matrix
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

/* Callback handler for normal-key event */
void keyboardListener(unsigned char key, int x, int y) {
    float v = 0.1;
    switch (key) {
        // Control eye (location of the eye)
        // control eyex
    case '1':
        eyex += v;
        break;
    case '2':
        eyex -= v;
        break;
        // control eyey
    case '3':
        eyey += v;
        break;
    case '4':
        eyey -= v;
        break;
        // control eyez
    case '5':
        eyez += v;
        break;
    case '6':
        eyez -= v;
        break;

        // Control center (location where the eye is looking at)
        // control centerx
    case 'q':
        centerx += v;
        break;
    case 'w':
        centerx -= v;
        break;
        // control centery
    case 'e':
        centery += v;
        break;
    case 'r':
        centery -= v;
        break;
        // control centerz
    case 't':
        centerz += v;
        break;
    case 'y':
        centerz -= v;
        break;

        // Control what is shown
    case 'a':
        isAxes = !isAxes;   // show/hide Axes if 'a' is pressed
        break;
    case 'c':
        isCube = !isCube;   // show/hide Cube if 'c' is pressed
        break;
    case 'p':
        isPyramid = !isPyramid; // show/hide Pyramid if 'p' is pressed
        break;
    case 'o':
        isOctahedron = !isOctahedron;   // show/hide Octahedron if 'o' is pressed
        break;
        // comma key 
    case ',':
        triangleScale--;
        cylinderHeight--;
        cylinderRadius++;
        radius += 0.015f;
        xheight -= 0.022f;
        scale += 0.1f;
        break;
    case '.':
        triangleScale++;
        cylinderHeight++;
        cylinderRadius--;
        radius -= 0.015f;
        xheight += 0.022f;
        scale -= 0.1f;
        break;
        // Control exit
    case 27:    // ESC key
        exit(0);    // Exit window
        break;
        // rotate the cube around the y-axis if space is pressed 
    }


    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Callback handler for special-key event */
void specialKeyListener(int key, int x, int y) {
    double v = 0.25;
    double lx = centerx - eyex;
    double lz = centerz - eyez;
    double s;
    switch (key) {
    case GLUT_KEY_LEFT:
        eyex += v * (upy * lz);
        eyez += v * (-lx * upy);
        s = sqrt(eyex * eyex + eyez * eyez) / (2 * sqrt(2));
        eyex /= s;
        eyez /= s;
        break;
    case GLUT_KEY_RIGHT:
        eyex += v * (-upy * lz);
        eyez += v * (lx * upy);
        s = sqrt(eyex * eyex + eyez * eyez) / (2 * sqrt(2));
        eyex /= s;
        eyez /= s;
        break;
    case GLUT_KEY_UP:
        eyey += v;
        break;
    case GLUT_KEY_DOWN:
        eyey -= v;
        break;

    default:
        return;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    glutInit(&argc, argv);                      // Initialize GLUT
    glutInitWindowSize(640, 640);               // Set the window's initial width & height
    glutInitWindowPosition(50, 50);             // Position the window's initial top-left corner
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("OpenGL 3D Drawing");      // Create a window with the given title
    glutDisplayFunc(display);                   // Register display callback handler for window re-paint
    glutReshapeFunc(reshapeListener);           // Register callback handler for window re-shape
    glutKeyboardFunc(keyboardListener);         // Register callback handler for normal-key event
    glutSpecialFunc(specialKeyListener);        // Register callback handler for special-key event
    initGL();                                   // Our own OpenGL initialization
    glutMainLoop();                             // Enter the event-processing loop
    return 0;
}
