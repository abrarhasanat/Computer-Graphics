#include<bits/stdc++.h>
#include "bitmap_image.hpp"
using namespace std;
#define pi acos(-1)
#define eps 1e-9
class Matrix {
public:
    vector<vector<double> > mat;
    int row, col;

    Matrix() {
        row = col = 0;
    }

    Matrix(int r, int c) {
        row = r;
        col = c;
        mat.resize(row);
        for (int i = 0; i < row; i++) {
            mat[i].resize(col);
        }
    }

    void print() {
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                cout << mat[i][j] << " ";
            }
            cout << endl;
        }
    }
    Matrix operator+(const Matrix& b) {
        Matrix res(row, col);
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                res.mat[i][j] = mat[i][j] + b.mat[i][j];
            }
        }
        return res;
    }

    Matrix operator-(const Matrix& b) {
        Matrix res(row, col);
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                res.mat[i][j] = mat[i][j] - b.mat[i][j];
            }
        }
        return res;
    }

    Matrix operator*(const Matrix& b) {
        Matrix res(row, b.col);
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < b.col; j++) {
                for (int k = 0; k < col; k++) {
                    res.mat[i][j] += mat[i][k] * b.mat[k][j];
                }
            }
        }
        return res;
    }
};

double eyex, eyey, eyez, lookx, looky, lookz, upx, upy, upz, fovy, aspectratio, znear, zfar;
double angle;
double degree_to_radian(double angle) {
    return angle * pi / 180.0;
}


void stage1(string fileName = "scene.txt") {


    stack<Matrix> S;
    Matrix M(4, 4);
    for (int i = 0; i < 4; i++) {
        M.mat[i][i] = 1;
    }

    ifstream file;
    file.open(fileName.c_str());
    string line;


    ofstream output_file;
    output_file.open("stage1.txt");


    /*
    Line 1: eyeX eyeY eyeZ
    Line 2: lookX lookY lookZ
    Line 3: upX upY upZ
    Line 4: fovY aspectRatio near far
    */


    getline(file, line);
    stringstream ss(line);
    //cout << line << endl;
    ss >> eyex >> eyey >> eyez;
    //cout << eyex << " " << eyey << " " << eyez << endl;
    getline(file, line);
    ss.clear();
    ss.str(line);
    ss >> lookx >> looky >> lookz;
    getline(file, line);
    ss.clear();
    ss.str(line);
    ss >> upx >> upy >> upz;
    getline(file, line);
    ss.clear();
    ss.str(line);
    ss >> fovy >> aspectratio >> znear >> zfar;



    while (true) {


        getline(file, line);
        ss.clear();
        ss.str(line);
        string command;
        ss >> command;
        if (command == "triangle") {
            // read 3 points
            vector<vector<double> > points(3, vector<double>(4));
            for (int i = 0; i < 3; i++) {
                getline(file, line);
                ss.clear();
                ss.str(line);
                ss >> points[i][0] >> points[i][1] >> points[i][2];
                points[i][3] = 1;
            }



            for (int i = 0; i < 3; i++) {
                Matrix P(4, 1);
                P.mat[0][0] = points[i][0];
                P.mat[1][0] = points[i][1];
                P.mat[2][0] = points[i][2];
                P.mat[3][0] = 1;
                P = M * P;
                points[i][0] = P.mat[0][0];
                points[i][1] = P.mat[1][0];
                points[i][2] = P.mat[2][0];
                points[i][3] = 1;
            }


            for (int i = 0; i < 3; i++) {
                // cout << points[i][0] << " " << points[i][1] << " " << points[i][2] << endl;

                output_file << fixed << setprecision(7) << points[i][0] << " " << points[i][1] << " " << points[i][2] << endl;
            }
            output_file << endl;
            //  cout << "triangle done" << endl;

        }
        else if (command == "translate") {

            double tx, ty, tz;
            getline(file, line);
            ss.clear();
            ss.str(line);
            ss >> tx >> ty >> tz;
            // create translation matrix
            Matrix T(4, 4);
            T.mat[0][0] = 1;
            T.mat[1][1] = 1;
            T.mat[2][2] = 1;
            T.mat[3][3] = 1;
            T.mat[0][3] = tx;
            T.mat[1][3] = ty;
            T.mat[2][3] = tz;
            // multiply T with top of stack
            M = M * T;
        }
        else if (command == "scale") {
            //read scaling amounts 
            double sx, sy, sz;
            getline(file, line);
            ss.clear();
            ss.str(line);
            ss >> sx >> sy >> sz;

            Matrix S(4, 4);
            S.mat[0][0] = sx;
            S.mat[1][1] = sy;
            S.mat[2][2] = sz;
            S.mat[3][3] = 1;

            M = M * S;
        }
        else if (command == "rotate") {
            // read rotation amount and axis 
            double angle, ax, ay, az;
            getline(file, line);
            ss.clear();
            ss.str(line);
            ss >> angle >> ax >> ay >> az;
            // create rotation matrix
            Matrix R(4, 4);
            double l = sqrt(ax * ax + ay * ay + az * az);
            ax /= l;
            ay /= l;
            az /= l;
            double c = cos(degree_to_radian(angle));
            double s = sin(degree_to_radian(angle));
            R.mat[0][0] = ax * ax * (1 - c) + c;
            R.mat[0][1] = ax * ay * (1 - c) - az * s;
            R.mat[0][2] = ax * az * (1 - c) + ay * s;
            R.mat[1][0] = ay * ax * (1 - c) + az * s;
            R.mat[1][1] = ay * ay * (1 - c) + c;
            R.mat[1][2] = ay * az * (1 - c) - ax * s;
            R.mat[2][0] = az * ax * (1 - c) - ay * s;
            R.mat[2][1] = az * ay * (1 - c) + ax * s;
            R.mat[2][2] = az * az * (1 - c) + c;
            R.mat[3][3] = 1;
            // multiply R with top of stack
            M = M * R;
        }
        else if (command == "push") {
            //cout << "push is done" << endl;
            S.push(M);
        }
        else if (command == "pop") {
            //cout << "pop is done" << endl;
            M = S.top();
            S.pop();
        }
        else if (command == "end") {
            break;
        }
        else {
            //cout << command << endl;
            cout << "Invalid command" << endl;
        }
    }

    file.close();
    output_file.close();
}


void stage2() {

    // read the scene from stage1.txt and apply the view transformation
    /*

    Line 1: eyeX eyeY eyeZ
    Line 2: lookX lookY lookZ
    Line 3: upX upY upZ
    Line 4: fovY aspectRatio near far

    */



    ifstream file;
    file.open("stage1.txt");

    ofstream output_file;
    output_file.open("stage2.txt");

    string line;
    stringstream ss;

    // read the scene from stage1.txt and apply the view transformation
    // In the view transformation phase, the gluLookAt parameters in scene.txt is used to generate the 
    //view transformation matrix V, and the points in stage1.txt are transformed by V and written in 
    //stage2.txt. The process of generating V is given below. 

    // compute using the parameters eyex, eyey, eyez, lookx, looky, lookz, upx, upy, upz, fovy, aspectratio, znear, zfar; 
    // l = look - eyes
    double lx = lookx - eyex;
    double ly = looky - eyey;
    double lz = lookz - eyez;
    double l = sqrt(lx * lx + ly * ly + lz * lz);

    lx /= l;
    ly /= l;
    lz /= l;
    // r = l x up
    double rx = ly * upz - lz * upy;
    double ry = lz * upx - lx * upz;
    double rz = lx * upy - ly * upx;
    double r = sqrt(rx * rx + ry * ry + rz * rz);
    //  cout << "r: " << r << endl;
    rx /= r;
    ry /= r;
    rz /= r;
    // u = r x l
    double ux = ry * lz - rz * ly;
    double uy = rz * lx - rx * lz;
    double uz = rx * ly - ry * lx;
    double u = sqrt(ux * ux + uy * uy + uz * uz);
    // cout << " u: " << u << endl;
    ux /= u;
    uy /= u;
    uz /= u;
    // create V
    Matrix V(4, 4);
    V.mat[0][0] = rx;
    V.mat[0][1] = ry;
    V.mat[0][2] = rz;
    V.mat[0][3] = -rx * eyex - ry * eyey - rz * eyez;
    V.mat[1][0] = ux;
    V.mat[1][1] = uy;
    V.mat[1][2] = uz;
    V.mat[1][3] = -ux * eyex - uy * eyey - uz * eyez;
    V.mat[2][0] = -lx;
    V.mat[2][1] = -ly;
    V.mat[2][2] = -lz;
    V.mat[2][3] = lx * eyex + ly * eyey + lz * eyez;
    V.mat[3][3] = 1;
    // V.print();
     // now read the points from stage1.txt and apply the view transformation
    while (getline(file, line)) {
        if (line.size() == 0) {
            output_file << endl;
            continue;
        }
        ss.clear();
        ss.str(line);
        double x, y, z;
        ss >> x >> y >> z;
        // create point matrix
        Matrix P(4, 1);
        P.mat[0][0] = x;
        P.mat[1][0] = y;
        P.mat[2][0] = z;
        P.mat[3][0] = 1;
        // multiply V with P
        Matrix R = V * P;
        // write the transformed point in stage2.txt
        output_file << fixed << setprecision(7) << R.mat[0][0] << " " << R.mat[1][0] << " " << R.mat[2][0] << endl;

    }
    // close the files
    file.close();
    output_file.close();

}

void stage3() {
    // cout << "Stage 3" << endl;
    // cout << "------" << endl << endl;

     //fovy, aspectratio, znear, zfar;
     // read data from stage2.txt and apply the projection transformation and output the transformed scene to stage3.txt 
    ifstream file;
    file.open("stage2.txt");

    ofstream output_file;
    output_file.open("stage3.txt");

    //First compute the field of view along X (fovX) axis and determine rand t. 
    double fovx = fovy * aspectratio;
    double t = znear * tan(degree_to_radian(fovy / 2.0f));
    double r = znear * tan(degree_to_radian(fovx / 2.0f));
    //cout << "t: " << t << " r: " << r << endl;
    //Then compute the projection matrix P using the parameters t, r, znear, zfar.
    Matrix P(4, 4);
    // cout << fovy << " " << aspectratio << " " << znear << " " << zfar << endl;
    P.mat[0][0] = znear / r;
    P.mat[1][1] = znear / t;
    P.mat[2][2] = -(zfar + znear) / (zfar - znear);
    P.mat[2][3] = -(2 * zfar * znear) / (zfar - znear);
    P.mat[3][2] = -1;
    //Now read the points from stage2.txt and apply the projection transformation
    string line;
    stringstream ss;
    while (getline(file, line)) {
        if (line.size() == 0) {
            output_file << endl;
            continue;
        }
        ss.clear();
        ss.str(line);
        double x, y, z;
        ss >> x >> y >> z;
        // create point matrix
        Matrix Q(4, 1);
        Q.mat[0][0] = x;
        Q.mat[1][0] = y;
        Q.mat[2][0] = z;
        Q.mat[3][0] = 1;
        // multiply P with P
        Matrix R = P * Q;
        // write the transformed point in stage3.txt
        output_file << fixed << setprecision(7) << R.mat[0][0] / R.mat[3][0] << " " << R.mat[1][0] / R.mat[3][0] << " " << R.mat[2][0] / R.mat[3][0] << endl;
    }
    // close the files
    file.close();
    output_file.close();

}
static unsigned long int g_seed = 1;
inline int random_color() {
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}
class Point {
public:
    double x, y, z;
    Point() {}
    Point(double x, double y, double z) : x(x), y(y), z(z) {}
    Point operator+(const Point& p) {
        return Point(x + p.x, y + p.y, z + p.z);
    }
    Point operator-(const Point& p) {
        return Point(x - p.x, y - p.y, z - p.z);
    }
    Point operator*(const double& d) {
        return Point(x * d, y * d, z * d);
    }
    Point operator/(const double& d) {
        return Point(x / d, y / d, z / d);
    }
};

class Color {
public:
    int red, green, blue;
    Color() {}
    Color(int red, int green, int blue) : red(red), green(green), blue(blue) {}
    
};
class Triangle {
public:
    Point p1, p2, p3;
    Color color;
    Triangle() {}
    Triangle(Point p1, Point p2, Point p3) : p1(p1), p2(p2), p3(p3) {}
    // print 
    void print() {
        cout << "p1: " << p1.x << " " << p1.y << " " << p1.z << endl;
        cout << "p2: " << p2.x << " " << p2.y << " " << p2.z << endl;
        cout << "p3: " << p3.x << " " << p3.y << " " << p3.z << endl;
    }
    void setColor() {
        color.red = random_color() % 256;
        color.green = random_color() % 256;
        color.blue = random_color() % 256;
        
    }

};


void zbuffer_algo() {
    // read data from stage3.txt and apply the z-buffer algorithm and output the transformed scene to stage4.txt
    ifstream file;
    file.open("stage3.txt");

    ofstream output_file;
    output_file.open("z_buffer.txt");

    // read the triangles from stage3.txt 
    string line;
    stringstream ss;
    vector<Triangle> triangles;
    while (getline(file, line)) {
        if (line.size() == 0) {
            continue;
        }
        ss.clear();
        // read the first point 
        ss.str(line);
        double x, y, z;
        ss >> x >> y >> z;
        Point p1(x, y, z);
        // read the second point
        getline(file, line);
        ss.clear();
        ss.str(line);
        ss >> x >> y >> z;
        Point p2(x, y, z);
        // read the third point
        getline(file, line);
        ss.clear();
        ss.str(line);
        ss >> x >> y >> z;
        Point p3(x, y, z);
        // create triangle
        Triangle t(p1, p2, p3);
        t.setColor();
        triangles.push_back(t);

    }
    // close the file
    file.close();

    //Read the config.txt file and store the values as Screen_Width, Screen_Height 

    ifstream config_file;
    config_file.open("config.txt");
    int Screen_Width, Screen_Height;
    config_file >> Screen_Width >> Screen_Height;
    //cout << Screen_Width << " " << Screen_Height << endl;
    // close the file
    config_file.close();




    // Initialize z-buffer and frame buffer 
    //Create a pixel mapping between the x-y range values and the Screen_Width X Screen_height range.

    double right_limit = 1.0, left_limit = -1.0, top_limit = 1.0, bottom_limit = -1.0;
    double x_range = right_limit - left_limit;
    double y_range = top_limit - bottom_limit;
    double dx = x_range /(double) Screen_Width;
    double dy = y_range / (double) Screen_Height;
    double top_Y = top_limit - dy / 2;
    double left_X = left_limit + dx / 2;
    double right_X = right_limit - dx / 2;
    double bottom_Y = bottom_limit + dy / 2;

    //Create a z - buffer, a two dimensional array of Screen Width X Screen Height dimension.
    //Initialize all the values in z - buffer with z max.In the aforementioned examples, z max = 2.0.
    //The memory for z - buffer should be dynamically allocated(using STL is allowed).

    double** z_buffer = new double* [Screen_Height];
    for (int i = 0; i < Screen_Height; i++) {
        z_buffer[i] = new double[Screen_Width];
        for (int j = 0; j < Screen_Width; j++) {
            z_buffer[i][j] = 1.0;
        }
    }


    //Create a bitmap_image object with Screen Width X Screen Height resolution and initialize its
    //background color with black.

    bitmap_image image(Screen_Width, Screen_Height);


    for (int i = 0; i < Screen_Height; i++) {
        for (int j = 0; j < Screen_Width; j++) {
            image.set_pixel(j, i, 0, 0, 0);
        }
    }

    for (Triangle triangle : triangles) {

        //Find top_scanline and bottom_scanline after necessary clipping

        double top_scanline = max(max(triangle.p1.y, triangle.p2.y), triangle.p3.y);
        double bottom_scanline = min(min(triangle.p1.y, triangle.p2.y), triangle.p3.y);


        //Compute the max_y for each triangle.If max_y > Top_Y, clip(i.e.ignore) the portion
        //above Top_Y and start scanning from Top_Y.Otherwise find the suitable mapping of
        //max_y to a top_scanline below Top_Y.Do a similar checking for min_y and Bottom Y.

        if (top_scanline > top_Y) {
            top_scanline = top_Y;
        }
        else {
            top_scanline = top_Y - (int)((top_Y - top_scanline) / dy) * dy;
        }

        if (bottom_scanline < bottom_limit) {
            bottom_scanline = bottom_limit;
        }
        else {
            bottom_scanline = bottom_Y + (int)((bottom_scanline - bottom_Y) / dy) * dy;
        }

        // for row_no from top_scanline to bottom_scanline 
        for (double row_no = top_scanline; row_no >= bottom_scanline; row_no -= dy) {
            // during scanning from top to bottom and left to right check for the middle 
            // values of each cell e.g top_Y - row_no*dy , left_X + col_no*dx 

            // compute min_x for each scanline for each triangle. If min_x < left_X, clip(i.e.ignore)
            // the portion left to left_X and start scanning from left_X.Otherwise find the suitable
            // mapping of min_x to a column to the right of left_X(left_intersecting column).Do a
            // similar checking for max_x and right_X.

            double min_x = min(min(triangle.p1.x, triangle.p2.x), triangle.p3.x);
            double max_x = max(max(triangle.p1.x, triangle.p2.x), triangle.p3.x);

            if (min_x < left_X) {
                min_x = left_X;
            }
            else {
                min_x = left_X + (int)((min_x - left_X) / dx) * dx;
            }

            if (max_x > right_X) {
                max_x = right_X;
            }
            else {
                max_x = left_X + (int)((max_x - left_X) / dx) * dx;
            }

            // find the left_intersecting column and right_intersecting column after necessary clipping

            double left_intersecting_column = min_x;
            double right_intersecting_column = max_x;

            // for col_no from left_intersecting_column to right_intersecting_column 
            for (double col_no = left_intersecting_column; col_no <= right_intersecting_column; col_no += dx) {

                // for each cell, compute the barycentric coordinates of the point corresponding to the
                // middle of the cell with respect to the triangle.

                double alpha, beta, gamma;
                alpha = ((triangle.p2.y - triangle.p3.y) * (col_no - triangle.p3.x) + (triangle.p3.x - triangle.p2.x) * (row_no - triangle.p3.y)) / ((triangle.p2.y - triangle.p3.y) * (triangle.p1.x - triangle.p3.x) + (triangle.p3.x - triangle.p2.x) * (triangle.p1.y - triangle.p3.y));
                beta = ((triangle.p3.y - triangle.p1.y) * (col_no - triangle.p3.x) + (triangle.p1.x - triangle.p3.x) * (row_no - triangle.p3.y)) / ((triangle.p2.y - triangle.p3.y) * (triangle.p1.x - triangle.p3.x) + (triangle.p3.x - triangle.p2.x) * (triangle.p1.y - triangle.p3.y));
                gamma = 1.0 - alpha - beta;

                // if the point is inside the triangle, compute the z value of the point and compare it
                // with the value in the z - buffer.If the z value of the point is less than the value in
                // the z - buffer, update the z - buffer with the z value of the point and update the
                // corresponding pixel in the frame buffer with the color of the triangle.

                if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                    double z = alpha * triangle.p1.z + beta * triangle.p2.z + gamma * triangle.p3.z;
                    if (z < z_buffer[(int)((top_Y - row_no) / dy)][(int)((col_no - left_X) / dx)] && z >= -1.0 && z <= 1.0) {
                        z_buffer[(int)((top_Y - row_no) / dy)][(int)((col_no - left_X) / dx)] = z;
                        image.set_pixel((int)((col_no - left_X) / dx), (int)((top_Y - row_no) / dy), triangle.color.red, triangle.color.green, triangle.color.blue);
                        //image.set_pixel((int)((col_no - left_X) / dx), (int)((top_Y - row_no) / dy), triangle.color.red, triangle.color.green, triangle.color.blue);

                    }
                }

            }
        }
    }

    // wriite the file to z_buffer.txt
    ofstream z_buffer_file;
    z_buffer_file.open("z_buffer.txt");
    for (int i = 0; i < Screen_Height; i++) {
        for (int j = 0; j < Screen_Width; j++) {
            if(z_buffer[i][j] < 1.0)
                z_buffer_file << z_buffer[i][j] << "\t";
        }
        z_buffer_file << endl;
    }

    //Save the image in a file

    image.save_image("output.bmp");
    // free the memory allocated for z_buffer
    for (int i = 0; i < Screen_Height; i++) {
        delete[] z_buffer[i];
    }
    delete[] z_buffer;
    

}
int32_t main() {
    stage1();
    stage2();
    stage3();
    zbuffer_algo();

    return 0;

}