#include<bits/stdc++.h> 
#include "1805023_bitmap_image.hpp"
#include <GL/glut.h>
#define pi acos(-1)
using namespace std;
const double eps = 1e-6;


bool isTextureEnabled = false;
class Point {
public:
    double x, y, z, n;
    Point() {
        x = y = z = 0;
        n = 1;
    }
    Point(double x, double y, double z) : x(x), y(y), z(z) {}
    Point operator+(const Point& p) const {
        return Point(x + p.x, y + p.y, z + p.z);
    }
    Point operator-(const Point& p) const {
        return Point(x - p.x, y - p.y, z - p.z);
    }
    Point operator*(const double& p) const {
        return Point(x * p, y * p, z * p);
    }
    Point operator/(const double& p) const {
        return Point(x / p, y / p, z / p);
    }
    Point operator*(const Point& p) const {
        return Point(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
    }


    double operator^(const Point& p) const {
        return x * p.x + y * p.y + z * p.z;
    }
    double len() {
        return sqrt(x * x + y * y + z * z);
    }
    Point unit() {
        return *this / len();
    }
    void normalize() {
        double l = len();
        x /= l;
        y /= l;
        z /= l;
    }
    void read() {
        cin >> x >> y >> z;
    }
    void print() {
        cout << x << " " << y << " " << z << endl;
    }

};

int zeroPhong = 0, nonZeroPhong = 0, zeroLambert = 0, nonZeroLambert = 0;


class Color {
public:
    double r, g, b;
    Color() {
        r = g = b = 0;
    }
    Color(double r, double g, double b) : r(r), g(g), b(b) {}
    Color operator+(const Color& p) const {
        return Color(r + p.r, g + p.g, b + p.b);
    }
    Color operator-(const Color& p) const {
        return Color(r - p.r, g - p.g, b - p.b);
    }
    Color operator*(const double& p) const {
        return Color(r * p, g * p, b * p);
    }
    Color operator/(const double& p) const {
        return Color(r / p, g / p, b / p);
    }
    Color operator*(const Color& p) const {
        return Color(r * p.r, g * p.g, b * p.b);
    }
    void read() {
        cin >> r >> g >> b;
    }
    void print() {
        cout << r << " " << g << " " << b << endl;
    }
};

class Light {
public:
    Point pos;
    Color color;
    double falloff;
    Light() {}
    Light(Point pos, Color color, double falloff) : pos(pos), color(color), falloff(falloff) {}
    void draw()
    {
        glPointSize(5);
        glBegin(GL_POINTS);
        glColor3f(color.r, color.g, color.b);
        glVertex3f(pos.x, pos.y, pos.z);
        glEnd();

    }
}
;

class SpotLight {
public:
    Point pos;
    Color color;
    Point dir;
    double angle;
    double falloff;
    SpotLight() {}
    SpotLight(Point pos, Color color, Point dir, double angle, double falloff) : pos(pos), color(color), dir(dir), angle(angle), falloff(falloff) {}
    void draw()
    {
        glPointSize(15);
        glBegin(GL_POINTS);
        glColor3f(color.r, color.g, color.b);
        glVertex3f(pos.x, pos.y, pos.z);
        glEnd();


    }
};

vector<Light*> lights;
vector<SpotLight*> spotLights;


class Ray {
public:
    Point origin, dir;
    Ray() {}
    Ray(Point _origin, Point _dir) {
        origin = _origin;
        dir = _dir;
        dir.normalize();
    }

};


double getDeterminant(double mat[3][3]) {
    double det = mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1])
        - mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0])
        + mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
    return det;
}



double intersectWithTriangle(Point a, Point b, Point c, Ray ray) {
    double bMat[3][3] = {
        {a.x - ray.origin.x, a.x - c.x,ray.dir.x},
        {a.y - ray.origin.y, a.y - c.y,ray.dir.y},
        {a.z - ray.origin.z, a.z - c.z,ray.dir.z}
    };
    double gMat[3][3] = {
        {a.x - b.x, a.x - ray.origin.x,ray.dir.x},
        {a.y - b.y, a.y - ray.origin.y,ray.dir.y},
        {a.z - b.z, a.z - ray.origin.z,ray.dir.z}
    };
    double tMat[3][3] = {
        {a.x - b.x, a.x - c.x,a.x - ray.origin.x},
        {a.y - b.y, a.y - c.y,a.y - ray.origin.y},
        {a.z - b.z, a.z - c.z,a.z - ray.origin.z}
    };
    double aMat[3][3] = {
        {a.x - b.x, a.x - c.x,ray.dir.x},
        {a.y - b.y, a.y - c.y,ray.dir.y},
        {a.z - b.z, a.z - c.z,ray.dir.z}
    };
    double aDet = getDeterminant(aMat);
    double beta = getDeterminant(bMat) / aDet;
    double gamma = getDeterminant(gMat) / aDet;
    double t = getDeterminant(tMat) / aDet;
    if (beta + gamma < 1 and beta > 0 and gamma > 0 and t > 0) {
        return t;
    }
    else return -1;
    return -1;
}



Ray getNormalOfTriangle(Point a, Point b, Point c, Point point, Ray incidentRay) {
    Point normal = (b - a) * (c - a);
    normal.normalize();
    if ((normal ^ incidentRay.dir) > 0) {
        normal = normal * -1;
    }
    return Ray(point, normal);
}

class Object;
class Sphere;
class Pyramid;
class Cube;
class CheckerBoard;

Color w_buff[1000][1000];
Color b_buff[1000][1000];
double  w_width, w_height, b_width, b_height;
vector<Object*> objects;
vector<Pyramid*> pyramids;
vector<Cube*> cubes;
vector<Sphere*> spheres;

class Object {
public:

    Point ref_point;
    Color color;
    double ambient, diffuse, specular, reflection, shininess, widthOfEachCell;
    bool isCheckerBoard = false;
    Object() {}

    virtual void draw() {}
    virtual double intersect(Ray ray, Color& color, int levelOfRecursion) {
        return -1;
    }
    virtual Ray getNormal(Point p, Ray incidentRay) {
        return Ray(Point(), Point());
    }
    virtual Color getColor(Point p) {
        return color;
    }
    Point getLowerLeftCorner(Point p) {
        int x = (p.x - ref_point.x) / widthOfEachCell;
        int y = (p.y - ref_point.y) / widthOfEachCell;
        return Point(ref_point.x + x * widthOfEachCell, ref_point.y + y * widthOfEachCell, 0);

    }
    virtual void read() {}
    bool isWhite(Point intersectionPoint) {
        int xx = (intersectionPoint.x - ref_point.x) / widthOfEachCell;
        int yy = (intersectionPoint.y - ref_point.y) / widthOfEachCell;
        if ((xx + yy) % 2 == 0) {
            // white cell  
            return true;
        }
        else {
            return false;
            // black cell
        }
    }

    pair<int, int> getXYForTexture(Point p, int isW) {
        Point lowerLeftPont = getLowerLeftCorner(p);
        double width = w_width;
        double height = w_height;
        if (!isW) {
            width = b_width;
            height = b_height;
        }
        double dx = p.x - lowerLeftPont.x;
        double dy = p.y - lowerLeftPont.y;
        if (dx < 0 or dy < 0) return make_pair(-1, -1);
        dx /= widthOfEachCell;
        dy /= widthOfEachCell;
        int x = dx * width;
        int y = dy * height;
        return make_pair(x, y);
    }
    virtual double Illuminate(Ray ray, Color& color, int level) {
        double result = this->intersect(ray, color, level);
        if (result < 0) return -1;
        if (level == 0) return result;

        Point intersectingPoint = ray.origin + ray.dir * result;
        Color colorAtIintersectingPoint = this->getColor(intersectingPoint);
        if (isTextureEnabled) {
            if (!isCheckerBoard) {
                color.r = colorAtIintersectingPoint.r * ambient;
                color.g = colorAtIintersectingPoint.g * ambient;
                color.b = colorAtIintersectingPoint.b * ambient;
            }
            else {
                if (isWhite(intersectingPoint)) {
                    pair<int, int> xy = getXYForTexture(intersectingPoint, 1);
                    if (xy.first != -1 and xy.second != -1) {
                        color.r = w_buff[xy.first][xy.second].r * ambient * colorAtIintersectingPoint.r;
                        color.g = w_buff[xy.first][xy.second].g * ambient * colorAtIintersectingPoint.g;
                        color.b = w_buff[xy.first][xy.second].b * ambient * colorAtIintersectingPoint.b;
                    }
                    else {
                        color.r = colorAtIintersectingPoint.r * ambient;
                        color.g = colorAtIintersectingPoint.g * ambient;
                        color.b = colorAtIintersectingPoint.b * ambient;

                    }
                }
                else {
                    pair<int, int>xy = getXYForTexture(intersectingPoint, 0);
                    if (xy.first != -1 and xy.second != -1) {
                        color.r = b_buff[xy.first][xy.second].r * ambient;
                        color.g = b_buff[xy.first][xy.second].g * ambient;
                        color.b = b_buff[xy.first][xy.second].b * ambient;
                    }
                    else {
                        color.r = colorAtIintersectingPoint.r * ambient;
                        color.g = colorAtIintersectingPoint.g * ambient;
                        color.b = colorAtIintersectingPoint.b * ambient;
                    }
                }
            }
        }
        else {
            color.r = colorAtIintersectingPoint.r * ambient;
            color.g = colorAtIintersectingPoint.g * ambient;
            color.b = colorAtIintersectingPoint.b * ambient;
        }




        double lambert = 0;
        double phong = 0;
        for (int i = 0; i < lights.size(); i++) {
            Point lightPos = lights[i]->pos;
            Point lightDir = intersectingPoint - lightPos;
            lightDir.normalize();
            Ray lightRay = Ray(lightPos, lightDir);
            Ray normal = this->getNormal(intersectingPoint, ray); // there might be some issues
            double tx = (intersectingPoint - lightPos).len();
            if (tx < eps) continue;

            bool isShadow = false;
            Color temp(0, 0, 0);
            for (int j = 0; j < objects.size(); j++) {
                double t = objects[j]->intersect(lightRay, temp, 0);
                if (t > 0 && t + eps < tx) {
                    isShadow = true;
                    break;
                }
            }
            if (isShadow) continue;
            Point toSource = lightPos - intersectingPoint;
            toSource.normalize();
            normal.dir.normalize();
            double distance = (lightPos - intersectingPoint).len();
            double scalingFactor = exp(-distance * distance * (double)(lights[i]->falloff));


            lambert += max(0.0, (normal.dir ^ toSource)) * scalingFactor;
            // if (lambert > 0) cout << lambert << endl;
             // find reflected Ray 
            Ray reflectedRay = Ray(intersectingPoint, lightDir - normal.dir * 2 * (lightDir ^ normal.dir));
            //phong += pow( R’.dot(toSource), shininess)*scaling_factor 
            phong += pow(max(0.0, reflectedRay.dir ^ toSource), shininess) * scalingFactor;

        }

        /*

        if S does not illuminate
intersecting point P: continue
vector toSource = PS;
toSource.normalize();
N = normal at
intersecting point P
N.normalize();
distance = distance between intersecting
point P and source S scaling_factor =
exp(-distance*distance*S.falloff);
lambert += toSource.dot(N)*scaling_factor;
R’ = reflected ray at point P (use N here as well) [here,
original ray is starting from camera, not from source]
R’.normalize()
phong += pow( R’.dot(toSource), shininess)*scaling_factor
        */

        for (int i = 0; i < spotLights.size(); ++i) {
            spotLights[i]->dir.normalize();
            Point lightPos = spotLights[i]->pos;
            Point lightDir = intersectingPoint - lightPos;
            lightDir.normalize();
            Ray lightRay(lightPos, lightDir);
            double dot = (lightDir) ^ spotLights[i]->dir;
            double angle = acos(dot);
            angle = (angle * 180.0) / pi;
            if (angle > spotLights[i]->angle) continue;
            lightRay = Ray(lightPos, lightDir);
            Ray normal = this->getNormal(intersectingPoint, lightRay);
            double tx = (intersectingPoint - lightPos).len();
            if (tx < eps) continue;
            bool isShadow = false;
            Color Temp(0, 0, 0);
            for (int j = 0; j < objects.size(); j++) {
                double t = objects[j]->intersect(lightRay, Temp, 0);
                if (t > 0 && t + eps < tx) {
                    isShadow = true;
                    break;
                }
            }
            if (isShadow) continue;
            Point toSource = lightPos - intersectingPoint;
            toSource.normalize();
            normal.dir.normalize();
            double distance = (lightPos - intersectingPoint).len();
            double scalingFactor = exp(-distance * distance * lights[i]->falloff);
            lambert += max(0.0, (normal.dir ^ toSource)) * scalingFactor;

            // find reflected Ray 
            Ray reflectedRay = Ray(intersectingPoint, lightDir - normal.dir * 2 * (lightDir ^ normal.dir));
            //phong += pow( R’.dot(toSource), shininess)*scaling_factor 
            phong += pow(max(0.0, reflectedRay.dir ^ toSource), shininess) * scalingFactor;

        }
        if (lambert > 0) nonZeroLambert++;
        else zeroLambert++;
        if (phong > 0) nonZeroPhong++;
        else zeroPhong++;

        if (isTextureEnabled) {
            if (!isCheckerBoard) {
                color.r += lambert * diffuse * colorAtIintersectingPoint.r;
                color.g += lambert * diffuse * colorAtIintersectingPoint.g;
                color.b += lambert * diffuse * colorAtIintersectingPoint.b;
            }
            else {
                if (isWhite(intersectingPoint)) {

                    pair<int, int>xy = getXYForTexture(intersectingPoint, 1);
                    if (xy.first != -1 and xy.second != -1) {
                        color.r += lambert * diffuse * w_buff[xy.first][xy.second].r * colorAtIintersectingPoint.r;
                        color.g += lambert * diffuse * w_buff[xy.first][xy.second].g * colorAtIintersectingPoint.g;
                        color.b += lambert * diffuse * w_buff[xy.first][xy.second].b * colorAtIintersectingPoint.b;
                    }
                    else {
                        color.r += lambert * diffuse * colorAtIintersectingPoint.r;
                        color.g += lambert * diffuse * colorAtIintersectingPoint.g;
                        color.b += lambert * diffuse * colorAtIintersectingPoint.b;
                    }
                }
                else {
                    pair<int, int>xy = getXYForTexture(intersectingPoint, 0);
                    if (xy.first != -1 and xy.second != -1) {

                        color.r += lambert * diffuse * b_buff[xy.first][xy.second].r;
                        color.g += lambert * diffuse * b_buff[xy.first][xy.second].g;
                        color.b += lambert * diffuse * b_buff[xy.first][xy.second].b;
                    }
                    else {
                        color.r += lambert * diffuse * colorAtIintersectingPoint.r;
                        color.g += lambert * diffuse * colorAtIintersectingPoint.g;
                        color.b += lambert * diffuse * colorAtIintersectingPoint.b;
                    }
                }
            }
        }
        else {
            color.r += lambert * diffuse * colorAtIintersectingPoint.r;
            color.g += lambert * diffuse * colorAtIintersectingPoint.g;
            color.b += lambert * diffuse * colorAtIintersectingPoint.b;
        }


        color.r += phong * specular * 1.0f;
        color.g += phong * specular * 1.0f;
        color.b += phong * specular * 1.0f;


        if (color.r > 1) color.r = 1;
        if (color.g > 1) color.g = 1;
        if (color.b > 1) color.b = 1;

        if (color.r < 0) color.r = 0;
        if (color.g < 0) color.g = 0;
        if (color.b < 0) color.b = 0;

        // reflection
        if (level > 0) {
            // if (level < 3) cout << level << endl;
            Ray normal = getNormal(intersectingPoint, ray);
            Ray reflectedRay = Ray(intersectingPoint, ray.dir - normal.dir * 2 * (ray.dir ^ normal.dir));
            reflectedRay.dir.normalize();
            reflectedRay.origin = reflectedRay.origin + reflectedRay.dir * eps;
            int nearest = -1;
            double minT = INT_MAX;
            Color temp(0, 0, 0);
            for (int k = 0; k < objects.size(); ++k) {
                double t = objects[k]->intersect(reflectedRay, temp, 0);
                if (t > 0 && t < minT) {
                    minT = t;
                    nearest = k;
                }
            }

            if (nearest != -1) {
                //   cout << "nearest " << nearest << endl;
                Color temp(0, 0, 0);
                double t = objects[nearest]->Illuminate(reflectedRay, temp, level - 1);
                color.r += temp.r * reflection;
                color.g += temp.g * reflection;
                color.b += temp.b * reflection;

            }
        }
        return result;
    }
};
class Sphere : public Object {
public:
    Point center;
    double radius;
    Sphere() {}
    Sphere(Point center, double radius) : center(center), radius(radius) {}
    Sphere(Point center, double radius, Color color, double ambient, double diffuse, double specular, double reflection, double shininess) : center(center), radius(radius) {
        this->color = color;
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->reflection = reflection;
        this->shininess = shininess;
    }



    virtual Ray getNormal(Point p, Ray incidentRay) {
        Point dir = p - center;
        dir.normalize();
        return Ray(p, dir);
    }


    virtual double intersect(Ray ray, Color& color, int level) {
        ray.origin = ray.origin - center;
        double a = 1;
        double b = 2 * (ray.origin ^ ray.dir);
        double c = (ray.origin ^ ray.origin) - radius * radius;
        double d = b * b - 4 * a * c;
        double result = -1;
        if (d < 0) {
            return result;
        }
        else {
            double t1 = (-b + sqrt(d)) / (2 * a);
            double t2 = (-b - sqrt(d)) / (2 * a);
            if (t1 > 0 && t2 > 0) {
                result = min(t1, t2);
            }
            else if (t1 > 0) {
                result = t1;
            }
            else if (t2 > 0) {
                result = t2;
            }
            else {
                return result;
            }
        }

        return result;
    }

    // Point intersectingPoint = ray.origin + ray.dir * result;
    // Color colorAtIintersectingPoint = this->getColor(intersectingPoint);
    // color.r = colorAtIintersectingPoint.r * ambient;
    // color.g = colorAtIintersectingPoint.g * ambient;
    // color.b = colorAtIintersectingPoint.b * ambient;



    // Ray normal = this->normal(intersectingPoint, ray);









    virtual void draw() {
        int stacks = 30;
        int slices = 20;
        Point points[100][100];
        double h, r;
        for (int i = 0;i < stacks; ++i) {
            h = radius * sin(((double)i / (double)stacks) * (pi / 2));
            r = radius * cos(((double)i / (double)stacks) * (pi / 2));
            for (int j = 0;j < slices; ++j) {
                points[i][j].x = r * cos(((double)j / (double)slices) * 2 * pi);
                points[i][j].y = r * sin(((double)j / (double)slices) * 2 * pi);
                points[i][j].z = h;
            }
        }
        for (int i = 0;i < stacks; ++i) {
            glPushMatrix();
            glTranslated(center.x, center.y, center.z);
            glColor3f(this->color.r, this->color.g, this->color.b);
            for (int j = 0;j < slices; ++j) {
                glBegin(GL_QUADS);
                // upper 
                glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                glVertex3f(points[i][(j + 1) % slices].x, points[i][(j + 1) % slices].y, points[i][(j + 1) % slices].z);
                glVertex3f(points[i + 1][(j + 1) % slices].x, points[i + 1][(j + 1) % slices].y, points[i + 1][(j + 1) % slices].z);
                glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                // lower 
                glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
                glVertex3f(points[i][(j + 1) % slices].x, points[i][(j + 1) % slices].y, -points[i][(j + 1) % slices].z);
                glVertex3f(points[i + 1][(j + 1) % slices].x, points[i + 1][(j + 1) % slices].y, -points[i + 1][(j + 1) % slices].z);
                glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);

                glEnd();
            }
            glPopMatrix();
        }
    }

    void read() {
        center.read();
        cin >> radius;
        color.read();
        cin >> ambient >> diffuse >> specular >> reflection >> shininess;
    }
};


class CheckerBoard : public Object {
public:

    CheckerBoard() {}
    CheckerBoard(int _widthOfEachCell) {
        this->widthOfEachCell = _widthOfEachCell;
        ref_point = Point(-250, -250, 0); // bottom left corner of the checkerboard
    }
    CheckerBoard(int _checkerBoard, double ambientForCheckerBoard, double diffuseForCheckerBoard, double reflectionForCheckerBoard) {
        this->widthOfEachCell = _checkerBoard;
        this->color = Color(0, 0, 0);
        this->ambient = ambientForCheckerBoard;
        this->diffuse = diffuseForCheckerBoard;
        this->specular = 0;
        this->reflection = reflectionForCheckerBoard;
        this->shininess = 0;
        ref_point = Point(-250, -250, 0); // bottom left corner of the checkerboard
    }
    CheckerBoard(int _widthOfEachCell, Color color, double ambient, double diffuse, double specular, double reflection, double shininess) {
        this->widthOfEachCell = _widthOfEachCell;
        this->color = color;
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->reflection = reflection;
        this->shininess = shininess;
        ref_point = Point(-250, -250, 0); // bottom left corner of the checkerboard
    }
    virtual Ray getNormal(Point p, Ray incidentRay) {
        if (incidentRay.dir.z < 0) {
            return Ray(p, Point(0, 0, 1));
        }
        else {
            return Ray(p, Point(0, 0, -1));
        }
    }

    // assignment operator overloading 
    CheckerBoard& operator=(const CheckerBoard& c) {
        this->widthOfEachCell = c.widthOfEachCell;
        this->color = c.color;
        this->ambient = c.ambient;
        this->diffuse = c.diffuse;
        this->specular = c.specular;
        this->reflection = c.reflection;
        this->shininess = c.shininess;
        this->ref_point = c.ref_point;
        return *this;
    }

    virtual double intersect(Ray ray, Color& color, int level) {
        Point normal = Point(0, 0, 1);
        double dot = ray.dir ^ normal;
        if ((dot * 100) == 0)

            return -1;
        double result = -1 * (ray.origin ^ normal) / dot;
        Point P = ray.origin + ray.dir * result;
        return result;
    }
    virtual Color getColor(Point p) {
        int x = (p.x - ref_point.x) / widthOfEachCell;
        int y = (p.y - ref_point.y) / widthOfEachCell;
        if ((x + y) % 2 == 0) {
            return Color(1, 1, 1);
        }
        else {
            return Color(0, 0, 0);
        }
    }


};

class Pyramid : public Object {
public:
    Point a, b, c, d, e;
    Ray calculatedNormal;
    Pyramid() {}

    Pyramid(Point a, Point b, Point c, Point d, Point e, Color color, double ambient, double diffuse, double specular, double reflection, double shininess) : a(a), b(b), c(c), d(d), e(e) {
        this->color = color;
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->reflection = reflection;
        this->shininess = shininess;
    }


    Pyramid(Point lowestPointCoordinate, double width, double height, Color colr, double ambient, double diffuse, double specular, double reflection, double shininess) {

        this->a = lowestPointCoordinate;
        this->b = lowestPointCoordinate + Point(width, 0, 0);
        this->c = lowestPointCoordinate + Point(width, width, 0);
        this->d = lowestPointCoordinate + Point(0, width, 0);
        this->e = lowestPointCoordinate + Point(width / 2, width / 2, height);
        this->color = colr;
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->reflection = reflection;
        this->shininess = shininess;
    }





    virtual Ray getNormal(Point p, Ray incidentRay) {
        double t = this->intersect(incidentRay, this->color, 0);
        if (t == -1) return Ray(Point(), Point());
        return this->calculatedNormal;
    }



    virtual double intersect(Ray ray, Color& color, int level) {
        double t1 = intersectWithTriangle(a, b, e, ray);
        double t2 = intersectWithTriangle(b, c, e, ray);
        double t3 = intersectWithTriangle(c, d, e, ray);
        double t4 = intersectWithTriangle(d, a, e, ray);
        double t5 = intersectWithTriangle(a, b, c, ray);
        double t6 = intersectWithTriangle(a, c, d, ray);
        // now find the closest intersection point FROM the ray origin
        double t = INT_MAX;
        if (t1 != -1) {
            t = t1;
            this->calculatedNormal = getNormalOfTriangle(a, b, e, ray.origin + ray.dir * t, ray);
        }
        if (t2 != -1) {
            if (t2 < t) {
                t = t2;
                this->calculatedNormal = getNormalOfTriangle(b, c, e, ray.origin + ray.dir * t, ray);
            }
        }
        if (t3 != -1) {
            if (t3 < t) {
                t = t3;
                this->calculatedNormal = getNormalOfTriangle(c, d, e, ray.origin + ray.dir * t, ray);
            }
        }
        if (t4 != -1) {
            if (t4 < t) {
                t = t4;
                this->calculatedNormal = getNormalOfTriangle(d, a, e, ray.origin + ray.dir * t, ray);
            }
        }
        if (t5 != -1) {
            if (t5 < t) {
                t = t5;
                this->calculatedNormal = getNormalOfTriangle(a, b, c, ray.origin + ray.dir * t, ray);
            }
        }
        if (t6 != -1) {
            if (t6 < t) {
                t = t6;
                this->calculatedNormal = getNormalOfTriangle(a, c, d, ray.origin + ray.dir * t, ray);
            }
        }
        if (t == INT_MAX) return -1;
        return t;
    }





    virtual void draw() {
        // draw the pyramid 
        // remember my floor is on the xy plane 
        // my up vector is z axis
        glBegin(GL_QUADS);
        glColor3f(this->color.r, this->color.g, this->color.b);
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
        glVertex3f(c.x, c.y, c.z);
        glVertex3f(d.x, d.y, d.z);
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(this->color.r, this->color.g, this->color.b);
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
        glVertex3f(e.x, e.y, e.z);
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(this->color.r, this->color.g, this->color.b);
        glVertex3f(b.x, b.y, b.z);
        glVertex3f(c.x, c.y, c.z);
        glVertex3f(e.x, e.y, e.z);
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(this->color.r, this->color.g, this->color.b);
        glVertex3f(c.x, c.y, c.z);
        glVertex3f(d.x, d.y, d.z);
        glVertex3f(e.x, e.y, e.z);
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(this->color.r, this->color.g, this->color.b);
        glVertex3f(d.x, d.y, d.z);
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(e.x, e.y, e.z);
        glEnd();


    }

    void read() {
        a.read();
        b.read();
        c.read();
        d.read();
        e.read();
        color.read();
        cin >> ambient >> diffuse >> specular >> reflection >> shininess;
    }
};

class Cube : public Object {
public:
    Point a, b, c, d, e, f, g, h;
    Ray calculatedNormal;
    Cube() {}

    Cube(Point a, Point b, Point c, Point d, Point e, Point f, Point g, Point h, Color color, double ambient, double diffuse, double specular, double reflection, double shininess) : a(a), b(b), c(c), d(d), e(e), f(f), g(g), h(h) {
        this->color = color;
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->reflection = reflection;
        this->shininess = shininess;
    }
    Cube(Point bottomLowerLeft, double side, Color color, double ambient, double diffuse, double specular, double reflection, double shininess) {
        this->a = bottomLowerLeft;
        this->b = bottomLowerLeft + Point(side, 0, 0);
        this->c = bottomLowerLeft + Point(side, side, 0);
        this->d = bottomLowerLeft + Point(0, side, 0);
        this->e = bottomLowerLeft + Point(0, 0, side);
        this->f = bottomLowerLeft + Point(side, 0, side);
        this->g = bottomLowerLeft + Point(side, side, side);
        this->h = bottomLowerLeft + Point(0, side, side);
        this->color = color;
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->reflection = reflection;
        this->shininess = shininess;
    }


    virtual Ray getNormal(Point p, Ray incidentRay) {
        double t = this->intersect(incidentRay, this->color, 0);
        if (t == -1) return Ray(Point(), Point());
        return this->calculatedNormal;
    }


    virtual double intersect(Ray ray, Color& color, int level) {
        //  cout << ray.origin.x << " " << ray.origin.y << " " << ray.origin.z << endl;
        double t = -1;
        // a,b,c,d square -> two trinagle 
        double t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12;
        t1 = t2 = t3 = t4 = t5 = t6 = t7 = t8 = t9 = t10 = t11 = t12 = -1;
        t1 = intersectWithTriangle(a, b, c, ray);
        t2 = intersectWithTriangle(d, c, a, ray);

        //e,f,g,h square -> two trinagle
        t3 = intersectWithTriangle(e, f, g, ray);
        t4 = intersectWithTriangle(e, g, h, ray);

        // a,e,f,b square -> two trinagle
        t5 = intersectWithTriangle(a, e, f, ray);
        t6 = intersectWithTriangle(a, f, b, ray);

        // d,h,g,c square -> two trinagle
        t7 = intersectWithTriangle(d, h, g, ray);
        t8 = intersectWithTriangle(d, g, c, ray);

        // a,d,h,e square -> two trinagle
        t9 = intersectWithTriangle(a, d, h, ray);
        t10 = intersectWithTriangle(a, h, e, ray);

        // b,c,g,f square -> two trinagle
        t11 = intersectWithTriangle(b, c, g, ray);
        t12 = intersectWithTriangle(b, g, f, ray);

        // now find the closest intersection point FROM the ray origin
        t = INT_MAX;
        if (t1 != -1) {
            t = t1;
            this->calculatedNormal = getNormalOfTriangle(a, b, c, ray.origin + ray.dir * t, ray);
        }
        if (t2 != -1) {
            if (t2 < t) {
                t = t2;
                this->calculatedNormal = getNormalOfTriangle(d, c, a, ray.origin + ray.dir * t, ray);
            }
        }
        if (t3 != -1) {
            if (t3 < t) {
                t = t3;
                this->calculatedNormal = getNormalOfTriangle(e, f, g, ray.origin + ray.dir * t, ray);
            }
        }
        if (t4 != -1) {
            if (t4 < t) {
                t = t4;
                this->calculatedNormal = getNormalOfTriangle(e, g, h, ray.origin + ray.dir * t, ray);
            }
        }
        if (t5 != -1) {
            if (t5 < t) {
                t = t5;
                this->calculatedNormal = getNormalOfTriangle(a, e, f, ray.origin + ray.dir * t, ray);
            }
        }
        if (t6 != -1) {
            if (t6 < t) {
                t = t6;
                this->calculatedNormal = getNormalOfTriangle(a, f, b, ray.origin + ray.dir * t, ray);
            }
        }
        if (t7 != -1) {
            if (t7 < t) {
                t = t7;
                this->calculatedNormal = getNormalOfTriangle(d, h, g, ray.origin + ray.dir * t, ray);
            }
        }
        if (t8 != -1) {
            if (t8 < t) {
                t = t8;
                this->calculatedNormal = getNormalOfTriangle(d, g, c, ray.origin + ray.dir * t, ray);
            }
        }
        if (t9 != -1) {
            if (t9 < t) {
                t = t9;
                this->calculatedNormal = getNormalOfTriangle(a, d, h, ray.origin + ray.dir * t, ray);
            }
        }
        if (t10 != -1) {
            if (t10 < t) {
                t = t10;
                this->calculatedNormal = getNormalOfTriangle(a, h, e, ray.origin + ray.dir * t, ray);
            }
        }
        if (t11 != -1) {
            if (t11 < t) {
                t = t11;
                this->calculatedNormal = getNormalOfTriangle(b, c, g, ray.origin + ray.dir * t, ray);
            }
        }
        if (t12 != -1) {
            if (t12 < t) {
                t = t12;
                this->calculatedNormal = getNormalOfTriangle(b, g, f, ray.origin + ray.dir * t, ray);
            }
        }
        if (t == INT_MAX) return -1;
        return t;
    }
    virtual void draw() {
        // draw the cube 
        glBegin(GL_QUADS);
        glColor3f(this->color.r, this->color.g, this->color.b);
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
        glVertex3f(c.x, c.y, c.z);
        glVertex3f(d.x, d.y, d.z);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(this->color.r, this->color.g, this->color.b);
        glVertex3f(e.x, e.y, e.z);
        glVertex3f(f.x, f.y, f.z);
        glVertex3f(g.x, g.y, g.z);
        glVertex3f(h.x, h.y, h.z);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(this->color.r, this->color.g, this->color.b);
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
        glVertex3f(f.x, f.y, f.z);
        glVertex3f(e.x, e.y, e.z);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(this->color.r, this->color.g, this->color.b);
        glVertex3f(d.x, d.y, d.z);
        glVertex3f(c.x, c.y, c.z);
        glVertex3f(g.x, g.y, g.z);
        glVertex3f(h.x, h.y, h.z);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(this->color.r, this->color.g, this->color.b);
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(d.x, d.y, d.z);
        glVertex3f(h.x, h.y, h.z);
        glVertex3f(e.x, e.y, e.z);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(this->color.r, this->color.g, this->color.b);
        glVertex3f(b.x, b.y, b.z);
        glVertex3f(c.x, c.y, c.z);
        glVertex3f(g.x, g.y, g.z);
        glVertex3f(f.x, f.y, f.z);
        glEnd();


    }


    void read() {
        a.read();
        b.read();
        c.read();
        d.read();
        e.read();
        f.read();
        g.read();
        h.read();
        color.read();
        cin >> ambient >> diffuse >> specular >> reflection >> shininess;
    }
};



// class Light {
// public:
//     Point p;
//     Color color;
//     Light() {}
//     Light(Point p, Color color) : p(p), color(color) {}
// };
// class Camera {
// public:
//     Point p, l, r, u;
//     double fovY, aspectRatio, near, far;
//     Camera() {}
//     Camera(Point p, Point l, Point r, Point u, double fovY, double aspectRatio, double near, double far) : p(p), l(l), r(r), u(u), fovY(fovY), aspectRatio(aspectRatio), near(near), far(far) {}
// };
// class Scene {
// public:
//     Camera camera;
//     vector<Object*> objects;
//     vector<Light> lights;
//     Color ambient;
//     Scene() {}
//     void read() {
//         camera.p.read();
//         camera.l.read();
//         camera.r.read();
//         camera.u.read();
//         cin >> camera.fovY >> camera.aspectRatio >> camera.near >> camera.far;
//         int n;
//         cin >> n;
//         for (int i = 0; i < n; i++) {
//             string type;
//             cin >> type;
//             if (type == "sphere") {
//                 Sphere* sphere = new Sphere();
//                 sphere->read();
//                 objects.push_back(sphere);
//             }
//         }
//         cin >> n;
//         for (int i = 0; i < n; i++) {
//             Light light;
//             light.p.read();
//             light.color.read();
//             lights.push_back(light);
//         }
//         ambient.read();
//     }
// };



















// Scene scene;
// Point getPointOnScreen(int x, int y) {
//     Point p = scene.camera.l + scene.camera.r * (2 * (x + 0.5) / 512 - 1) + scene.camera.u * (2 * (y + 0.5) / 512 - 1);
//     return p;
// }
// Point getPointOnNearPlane(int x, int y) {
//     Point p = scene.camera.p + scene.camera.l * scene.camera.near + scene.camera.r * (2 * (x + 0.5) / 512 - 1) * scene.camera.near + scene.camera.u * (2 * (y + 0.5) / 512 - 1) * scene.camera.near;
//     return p;
// }
// Point getPointOnFarPlane(int x, int y) {
//     Point p = scene.camera.p + scene.camera.l * scene.camera.far + scene.camera.r * (2 * (x + 0.5) / 512 - 1) * scene.camera.far + scene.camera.u * (2 * (y + 0.5) / 512 - 1) * scene.camera.far;
//     return p;
// }
// Color getColor(Ray ray, int depth) {
//     if (depth == 0) return Color();
//     double t = 1e9;
//     Object* object = NULL;
//     for (int i = 0; i < scene.objects.size(); i++) {
//         double t1 = scene.objects[i]->intersect(ray, scene.objects[i]->color, depth);
//         if (t1 > 0 && t1 < t) {
//             t = t1;
//             object = scene.objects[i];
//         }
//     }
//     if (object == NULL) return Color();
//     Point p = ray.origin + ray.dir * t;
//     Color color = object->getColor(p);
//     Point n = object->normal(p ,ray);
//     Point v = (scene.camera.p - p).unit();
//     Color ambient = scene.ambient * color * object->ambient;
//     Color diffuse, specular;
//     for (int i = 0; i < scene.lights.size(); i++) {
//         Point l = (scene.lights[i].p - p).unit();
//         Ray ray(p, l);
//         bool flag = true;
//         for (int j = 0; j < scene.objects.size(); j++) {
//             double t1 = scene.objects[j]->intersect(ray);
//             if (t1 > 0 && t1 < 1) {
//                 flag = false;
//                 break;
//             }
//         }
//         if (!flag) continue;
//         double d = max(0.0, l ^ n);
//         diffuse = diffuse + scene.lights[i].color * color * object->diffuse * d;
//         Point r = (n * (l ^ n) * 2 - l).unit();
//         double s = max(0.0, r ^ v);
//         specular = specular + scene.lights[i].color * object->specular * pow(s, object->shininess);
//     }
//     Color reflection = getColor(Ray(p, ray.d - n * (ray.d ^ n) * 2), depth - 1) * object->reflection;
//     return ambient + diffuse + specular + reflection;
// }
// void draw() {
//     bitmap_image image(512, 512);
//     for (int i = 0; i < 512; i++) {
//         for (int j = 0; j < 512; j++) {
//             Point p = getPointOnScreen(i, j);
//             Point d = (p - scene.camera.p).unit();
//             Ray ray(scene.camera.p, d);
//             Color color = getColor(ray, 5);
//             image.set_pixel(i, j, color.r * 255, color.g * 255, color.b * 255);
//         }
//     }
//     image.save_image("1805023.bmp");
// }




