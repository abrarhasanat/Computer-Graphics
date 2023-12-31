// for MS Windows
#include<bits/stdc++.h>
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
using namespace std;
const float scaleStep = 0.05f;
const float moveForwardStep = 0.05f;
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



/* Draw a cube centered at the origin */
void drawCube() {
}

double xheight = 1.0f;
double radius = 0.01f;
float scale = 0;



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
    for (int i = 0; i < 6; ++i) {
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
    xheight = vertices[1] - vertices[0];
    radius = vertices[0] * sqrt(3);
    float height = sqrt(2) * xheight;
    int segments = 50;
    double tempx = radius, tempy = 0;
    double currx, curry;
    for (int i = 0; i < 12; ++i) {
        glPushMatrix();
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

    drawAxes();
    drawOctahedron();
    drawCylinder();
    drawSphere();

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
    //float v = 0.1;
    double v = 0.25;
    double lx = centerx - eyex;
    double lz = centerz - eyez;
    double s;
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
        eyex += v * (-upy * lz);
        eyez += v * (lx * upy);
        s = sqrt(eyex * eyex + eyez * eyez) / (2 * sqrt(2));
        eyex /= s;
        eyez /= s;
        break;
    case 'd':
        eyex -= v * (-upy * lz);
        eyez -= v * (lx * upy);
        s = sqrt(eyex * eyex + eyez * eyez) / (2 * sqrt(2));
        eyex /= s;
        eyez /= s;
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
        scale += scaleStep;
        if (scale > 1.0f)
            scale = 1.0f;
        break;
    case '.':
        triangleScale++;
        cylinderHeight++;
        cylinderRadius--;
        radius -= 0.015f;
        xheight += 0.022f;
        scale -= scaleStep;
        if (scale < 0.0f)
            scale = 0.0f;
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

    GLfloat speed = 0.1; // Adjust this value to control the speed of movement

    // Calculate the forward vector
    GLfloat forwardx = centerx - eyex;
    GLfloat forwardy = centery - eyey;
    GLfloat forwardz = centerz - eyez;

    // Normalize the forward vector
    GLfloat magnitude = sqrt(forwardx * forwardx + forwardy * forwardy + forwardz * forwardz);
    forwardx /= magnitude;
    forwardy /= magnitude;
    forwardz /= magnitude;




    GLfloat rightx = upy * centerz - upz * centery;
    GLfloat righty = upz * centerx - upx * centerz;
    GLfloat rightz = upx * centery - upy * centerx;

    
    // Update the eye and center positions

    switch (key) {
    case GLUT_KEY_LEFT:
        eyex += v * (-upy * lz);
        eyez += v * (lx * upy);
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
        break;\
    case GLUT_KEY_UP:
        // move forward
        eyex += forwardx * speed;
        eyey += forwardy * speed;
        eyez += forwardz * speed;
        centerx += forwardx * speed;
        centery += forwardy * speed;
        centerz += forwardz * speed;
        break;
    case GLUT_KEY_DOWN:
        eyex -= forwardx * speed;
        eyey -= forwardy * speed;
        eyez -= forwardz * speed;
        centerx -= forwardx * speed;
        centery -= forwardy * speed;
        centerz -= forwardz * speed;
        
        break;

        // case GLUT_KEY_UP:
    //     eyey += v;
    //     break;
    // case GLUT_KEY_DOWN:
    //     eyey -= v;
    //     break;

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