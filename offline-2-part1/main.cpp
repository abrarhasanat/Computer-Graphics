#include<bits/stdc++.h>
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


void readFile(string fileName) {

    // initialize empty stack S of matrices
    stack<Matrix> S;
    // initialize identity matrix M 
    Matrix M(4, 4);
    for (int i = 0; i < 4; i++) {
        M.mat[i][i] = 1;
    }

    ifstream file;
    file.open(fileName.c_str());
    string line;

    // create a new file output_file named as "stage1.txt" in output mode
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
    cout << line << endl;
    ss >> eyex >> eyey >> eyez;
    cout << eyex << " " << eyey << " " << eyez << endl;
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
        // here begins stage 1 : 

        // read command 
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


            // apply transformation matrix M to each point from points 
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
            // output the transformed points to output_file

            for (int i = 0; i < 3; i++) {
                cout << points[i][0] << " " << points[i][1] << " " << points[i][2] << endl;

                output_file << fixed << setprecision(7) << points[i][0] << " " << points[i][1] << " " << points[i][2] << endl;
            }
            output_file << endl;
            cout << "triangle done" << endl;

        }
        else if (command == "translate") {
            //read translation amounts
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
            cout << command << " " << sx << " " << sy << " " << sz << endl;
            // create scaling matrix
            Matrix S(4, 4);
            S.mat[0][0] = sx;
            S.mat[1][1] = sy;
            S.mat[2][2] = sz;
            S.mat[3][3] = 1;
            // multiply S with top of stack
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
            cout << "push is done" << endl;
            S.push(M);
        }
        else if (command == "pop") {
            cout << "pop is done" << endl;
            M = S.top();
            S.pop();
        }
        else if (command == "end") {
            break;
        }
        else {
            cout << command << endl;
            cout << "Invalid command" << endl;
        }
    }

    file.close();
    output_file.close();
}


void stage2() {

    cout << "Stage 2" << endl;
    cout << "------" << endl << endl;
    // in this stage, we will read the scene from stage1.txt and apply the view transformation
    // and then output the transformed scene to stage2.txt
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
    cout << "lx " << lx << " ly " << ly << " lz " << lz << " l " << l << endl;
    cout << "l: " << l << endl;
    lx /= l;
    ly /= l;
    lz /= l;
    // r = l x up
    double rx = ly * upz - lz * upy;
    double ry = lz * upx - lx * upz;
    double rz = lx * upy - ly * upx;
    double r = sqrt(rx * rx + ry * ry + rz * rz);
    cout << "r: " << r << endl;
    rx /= r;
    ry /= r;
    rz /= r;
    // u = r x l
    double ux = ry * lz - rz * ly;
    double uy = rz * lx - rx * lz;
    double uz = rx * ly - ry * lx;
    double u = sqrt(ux * ux + uy * uy + uz * uz);
    cout << " u: " << u << endl;
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
    V.print();
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
    cout << "Stage 3" << endl;
    cout << "------" << endl << endl;

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
    cout << "t: " << t << " r: " << r << endl;
    //Then compute the projection matrix P using the parameters t, r, znear, zfar.
    Matrix P(4, 4);
    cout << fovy << " " << aspectratio << " " << znear << " " << zfar << endl;
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

int32_t main() {
    readFile("scene.txt");
    stage2();
    stage3();
}