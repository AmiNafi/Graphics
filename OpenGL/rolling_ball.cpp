#include <bits/stdc++.h>
using namespace std;


const float PI = acos(-1.0);
// #ifdef _linux_
#include<GL/glut.h>
// #elif _WIN32
//     #include<windows.h>
//     #include<glut.h>
// #endif
int subdivision;
float radius;
vector<float> buildUnitPositiveX() {
    const float DEG2RAD = acos(-1) / 180.0f;

    vector<float> vertices;
    float n1[3];  // normal of longitudinal plane rotating along Y-axis
    float n2[3];  // normal of latitudinal plane rotating along Z-axis
    float v[3];   // direction vector intersecting 2 planes, n1 x n2
    float a1;     // longitudinal angle along Y-axis
    float a2;     // latitudinal angle along Z-axis

    // compute the number of vertices per row, 2^n + 1
    int pointsPerRow = (int)pow(2, subdivision) + 1;

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for (unsigned int i = 0; i < pointsPerRow; ++i) {
        // normal for latitudinal plane
        // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
        // therefore, it is rotating (0,1,0) vector by latitude angle a2
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
        for (unsigned int j = 0; j < pointsPerRow; ++j) {
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
            v[0] *= (scale * radius);
            v[1] *= (scale * radius);
            v[2] *= (scale * radius);

            // add a vertex into array
            vertices.push_back(v[0]);
            vertices.push_back(v[1]);
            vertices.push_back(v[2]);
        }
    }
    // for (float x : vertices) cout << " inside = " << x << " " << radius << " " << subdivision << endl;
    return vertices;
}







struct Point {
    float x, y, z;
    Point operator =(const Point& pt) {
        x = pt.x;
        y = pt.y;
        z = pt.z;
        return *this;
    }
    Point operator+ (const Point& pt) const {
        Point ret;
        ret.x = this->x + pt.x;
        ret.y = this->y + pt.y;
        ret.z = this->z + pt.z;
        return ret;
    }

    Point operator- (const Point& pt) const {
        Point ret;
        ret.x = this->x - pt.x;
        ret.y = this->y - pt.y;
        ret.z = this->z - pt.z;
        return ret;
    } 
    void normalize() {
        float ln = sqrt(x * x + y * y + z * z);
        x /= ln;
        y /= ln;
        z /= ln;
    }
    Point operator* (const float val) const {
        Point ret;
        ret.x = this->x * val;
        ret.y = this->y * val;
        ret.z = this->z * val;
        return ret;
    }
    Point cross(Point pt) {
        Point ret;
        ret.x = y * pt.z - z * pt.y;
        ret.y = z * pt.x - x * pt.z;
        ret.z = x * pt.y - y * pt.x;
        return ret;
    }
    
    float getLength() {
        return sqrt(x * x + y * y + z * z);
    }
};
Point centroid, center;
Point camera, focus, forwrd, upwrd, rightwrd, spherefor, sphereup, sphereright;
float triangleConst, crad, cht;
int counter = 0 ;
int animate = 0 ;
vector < float > vertices;
void white() {
    glColor3f(1,1,1);
}
void black() {
    glColor3f(0,0,0);
}
void red() {
    glColor3f(1,0,0);
}
void green() {
    glColor3f(0,1,0);
}
void blue() {
    glColor3f(0,0,1);
}
void cyan() {
    glColor3f(0,1,1);
}
void magenta() {
    glColor3f(1,0,1);
}
void yellow() {
    glColor3f(1,1,0);
}



void drawSpherePartial() {
    // draw the filled sphere from vertices
    // vector<float> vertices = verticesXPos;
    vertices = buildUnitPositiveX();
    // cout << " total vertices " << vertices.size() << endl;
    int totalPoints = vertices.size() / 3;
    int pointsPerRow = (int)pow(2, subdivision) + 1;
    int totalRows = totalPoints / pointsPerRow;
    
    glBegin(GL_QUADS);
    for (int i = 0; i < totalPoints - pointsPerRow - 1; i++) {
        Point p1, p2, p3, p4;
        p1.x = vertices[i * 3];
        p1.y = vertices[i * 3 + 1];
        p1.z = vertices[i * 3 + 2];
        // cout << " vertices " << vertices[i * 3] << endl;
        p2.x = vertices[(i + 1) * 3];
        p2.y = vertices[(i + 1) * 3 + 1];
        p2.z = vertices[(i + 1) * 3 + 2];

        p3.x = vertices[(i + pointsPerRow) * 3];
        p3.y = vertices[(i + pointsPerRow) * 3 + 1];
        p3.z = vertices[(i + pointsPerRow) * 3 + 2];

        p4.x = vertices[(i + pointsPerRow + 1) * 3];
        p4.y = vertices[(i + pointsPerRow + 1) * 3 + 1];
        p4.z = vertices[(i + pointsPerRow + 1) * 3 + 2];
        

        // cout << p1.x << " " << p1.y << " " << p1.z << endl;
        // cout << p2.x << " " << p2.y << " " << p2.z << endl;
        // cout << p4.x << " " << p4.y << " " << p4.z << endl;
        // cout << p3.x << " " << p3.y << " " << p3.z << endl;
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
        glVertex3f(p4.x, p4.y, p4.z);
        glVertex3f(p3.x, p3.y, p3.z);
        //cout << " inside drawing of vertices " << endl;
    }
    glEnd();
}
// void drawSphereFull() {

//     float sphereConst = 1.0 - triangleConst;
//     // cout << " sphere const = " << sphereConst << endl;
//     // x axis
//     green();
//     glPushMatrix();
//     glTranslated(triangleConst, 0, 0);
//     glScaled(sphereConst, sphereConst, sphereConst);
//     drawSpherePartial();
//     glPopMatrix();
//     glPushMatrix();
//     glRotated(180, 0, 1, 0);
//     glTranslated(triangleConst, 0, 0);
//     glScaled(sphereConst, sphereConst, sphereConst);
//     drawSpherePartial();
//     glPopMatrix();

//     // z axis
//     blue();
//     glPushMatrix();
//     glRotated(90, 0, 1, 0);
//     glTranslated(triangleConst, 0, 0);
//     glScaled(sphereConst, sphereConst, sphereConst);
//     drawSpherePartial();
//     glPopMatrix();
//     glPushMatrix();
//     glRotated(-90, 0, 1, 0);
//     glTranslated(triangleConst, 0, 0);
//     glScaled(sphereConst, sphereConst, sphereConst);
//     drawSpherePartial();
//     glPopMatrix();

//     // y axis
//     red();
//     glPushMatrix();
//     glRotated(90, 0, 0, 1);
//     glTranslated(triangleConst, 0, 0);
//     glScaled(sphereConst, sphereConst, sphereConst);
//     drawSpherePartial();
//     glPopMatrix();
//     glPushMatrix();
//     glRotated(-90, 0, 0, 1);
//     glTranslated(triangleConst, 0, 0);
//     glScaled(sphereConst, sphereConst, sphereConst);
//     drawSpherePartial();
//     glPopMatrix();
// }

void drawCylinderCurve() {
    float cylinderConst = 1 - triangleConst;
    crad = sqrt(1.0 / 3.0) * cylinderConst;
    // cout << " crad " << crad << endl;
    cht = sqrt(2) * triangleConst;
    glPushMatrix();
    glRotated(45, 0, 0, 1);
    glTranslated(triangleConst / sqrt(2), 0, 0);
    int thetaUnit = 5;
    yellow();
    for (int theta = -35; theta < 35; theta += thetaUnit) {
        Point pt1, pt2, pt3, pt4;
        pt1.x = pt4.x = crad * cos(theta * PI / 180.0);
        pt1.y = pt2.y = cht / 2;
        pt1.z = pt4.z = crad * sin(theta * PI / 180.0);
        pt2.x = pt3.x = crad * cos((theta + thetaUnit) * PI / 180.0);
        pt3.y = pt4.y = -cht / 2;
        pt2.z = pt3.z = crad * sin((theta + thetaUnit) * PI / 180.0);
        glBegin(GL_QUADS);
        glVertex3f(pt1.x, pt1.y, pt1.z);
        glVertex3f(pt2.x, pt2.y, pt2.z);
        glVertex3f(pt3.x, pt3.y, pt3.z);
        glVertex3f(pt4.x, pt4.y, pt4.z);
        glEnd();
    }
    glPopMatrix();
}

void drawUpperCylinder() {
    drawCylinderCurve();
    glRotated(90,0,1,0);
    drawCylinderCurve();
    glRotated(90,0,1,0);
    drawCylinderCurve();
    glRotated(90,0,1,0);
    drawCylinderCurve();
    glRotated(90,0,1,0);
}

void drawXaxisCylinder() {
    glPushMatrix();
    glRotated(90,1,0,0);
    drawCylinderCurve();
    glPopMatrix();
    glPushMatrix();
    glRotated(-90,1,0,0);
    drawCylinderCurve();
    glPopMatrix();
}
void drawCylinderFull() {
    drawUpperCylinder();
    glPushMatrix();
    glScaled(1,-1,1); // XZ mirror
    drawUpperCylinder();
    glPopMatrix();
    drawXaxisCylinder();
    glPushMatrix();
    glScaled(-1,1,1); // YZ mirror
    drawXaxisCylinder();
    glPopMatrix();
}

void drawGridUnit(float x, float y) {
    glBegin(GL_QUADS);
    glVertex3f(x, y, 0.0);
    glVertex3f(x + 1, y, 0.0);
    glVertex3f(x + 1, y + 1, 0.0);
    glVertex3f(x, y + 1, 0.0);
    glEnd();
}
void drawGridFull() {
    for (int x = -100; x <= 100; x++) {
        for (int y = -100; y <= 100; y++) {
            if ((x + y)&1) white();
            else black();
            drawGridUnit(x, y);
        }
    }
}
void drawBorderPartial(float x, float y, float dx, float dy, float h) {
    glBegin(GL_QUADS);
    glVertex3f(x, y, 0);
    glVertex3f(x + dx, y + dy, 0);
    glVertex3f(x + dx, y + dy, h);
    glVertex3f(x, y, h);
    glEnd();
}
void drawBorderFull() {
    magenta();
    drawBorderPartial(-5, -5, 10, 0, 1);
    drawBorderPartial(5, -5, 0, 10, 1);
    drawBorderPartial(5, 5, -10, 0, 1);
    drawBorderPartial(-5, 5, 0, -10, 1);
    // drawBorderPartial()
}
void drawTriangle() {

    glBegin(GL_TRIANGLES);
    Point p1, p2, p3;
    p1.x = p2.y = p3.z = 1;
    p1.y = p1.z = p2.x = p2.z = p3.x = p3.y = 0;
    p1 = centroid + (p1 - centroid) * triangleConst;
    p2 = centroid + (p2 - centroid) * triangleConst;
    p3 = centroid + (p3 - centroid) * triangleConst;
    glVertex3f(p1.x, p1.y, p1.z);
    glVertex3f(p2.x, p2.y, p2.z);
    glVertex3f(p3.x, p3.y, p3.z);

    glEnd();
}

void drawHalfOcta() {
    magenta();
    drawTriangle();
    glRotated(90,0,1,0);
    cyan();
    drawTriangle();
    glRotated(90,0,1,0);
    magenta();
    drawTriangle();
    glRotated(90,0,1,0);
    cyan();
    drawTriangle();
    glRotated(90,0,1,0);
}

void drawAxes() {
    glLineWidth(3);
    glBegin(GL_LINES);

    
    red();
    glVertex3f(0,0,0);
    glVertex3f(500,0,0);

    green();
    glVertex3f(0,0,0);
    glVertex3f(0,500,0);

    blue();
    glVertex3f(0,0,0);
    glVertex3f(0,0,500);

    
    glEnd();
    
}

void drawOctahedron() {
    drawHalfOcta();
    glPushMatrix();
    glRotated(90,0,1,0);
    glScalef(1,-1,1);
    drawHalfOcta();
    glPopMatrix();
}

void drawSphere()
{
    
    glPushMatrix();
    radius = 1;
    glTranslatef(center.x, center.y, center.z);
    int slices = 8;
    int stacks = 16;
	Point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(PI/2));
		r=radius*cos(((double)i/(double)stacks)*(PI/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*PI);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*PI);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        // glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
                if (j % 2) red();
                else green();
                // cout << "point = " << points[i][j].x << " " << points[i][j].y << " " << points[i][j].z << endl;
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                if (j % 2) green();
                else red();
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
    glPopMatrix();
}

void drawSpherePointer() {
    blue();
    glBegin(GL_QUADS);
    glVertex3f(center.x, center.y, center.z);
    glVertex3f(center.x, center.y, center.z - 0.2);
    Point centerfor = center + spherefor * 2;
    glVertex3f(centerfor.x, centerfor.y, centerfor.z - 0.2);
    glVertex3f(centerfor.x, centerfor.y, centerfor.z);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // cout << camera.x << "," << camera.y << " ==== " << focus.x << "," << focus.y << endl;
    gluLookAt(camera.x,camera.y,camera.z,focus.x,focus.y,focus.z,upwrd.x,upwrd.y,upwrd.z);
    // gluLookAt(5,5,5,0,0,0,0,1,0);
    // drawAxes();
    // glPushMatrix();
    // glTranslatef(0, 0, -5);
    // drawOctahedron();
    // drawSphereFull();
    // drawCylinderFull();
    drawGridFull();
    drawBorderFull();
    drawSphere();
    drawSpherePointer();
    // glPopMatrix();
    // cout << " camera.x = " << camera.x << endl;
    
    //,camera.y,camera.z,focus.x,focus.y,focus.z,upwrd.x,upwrd.y,upwrd.z);
    glutSwapBuffers();
}

void initialize(){
    center.x = 0;
    center.y = 0;
    center.z = 1;
    centroid.x = 1.0 / 3.0;
    centroid.y = 1.0 / 3.0;
    centroid.z = 1.0 / 3.0;
    triangleConst = 1.0;
    camera.x = camera.y = camera.z = 5;
    focus.x = focus.y = focus.z = 0;
    forwrd = focus - camera;
    forwrd.normalize();
    upwrd.x = 0;
    upwrd.y = 0;
    upwrd.z = 1;
    rightwrd = forwrd.cross(upwrd);
    rightwrd.normalize();
    subdivision = 4;
    crad = radius = centroid.getLength();   
    cht = sqrt(2);
    sphereup = upwrd;
    spherefor.x = spherefor.z = 0;
    spherefor.y = 1;
    sphereright = spherefor.cross(sphereup);
}



void initGL() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set background color to black and opaque
    glEnable(GL_DEPTH_TEST);
}
void reshape(int w , int h){
    glViewport(0,0,(GLsizei)w , (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,1,2.0,1000.0);
    glMatrixMode(GL_MODELVIEW);
}

// void idle(){
//     printf("Currently we don't have anything to do!!!.\n");

//     glutPostRedisplay();
// }

void Timer(int value){

    // if(animate){
    //     printf("Inside if(Animate)");
    //     printf("Counter: %d\n", counter);

    //     counter++;
    // }
    
    glutPostRedisplay();
    glutTimerFunc(1000/60, Timer, 0);
}
void keyboard_handler(unsigned char key, int x, int y) {
    float rate = 0.02, shrink = 0.05, sprate = 0.2;
    // float sprate = 0.8, sptheta = 0.02;
    switch(key){
        case '1':
            // printf("1 Pressed\n");
            rightwrd = rightwrd * cos(rate) + forwrd * sin(rate);
            forwrd = forwrd * cos(rate) - rightwrd * sin(rate);
            break;
        case '2':
            rightwrd = rightwrd * cos(-rate) + forwrd * sin(-rate);
            forwrd = forwrd * cos(-rate) - rightwrd * sin(-rate);
            break;

        case '3':
            forwrd = forwrd * cos(rate) + upwrd * sin(rate);
            upwrd = upwrd * cos(rate) - forwrd * sin(rate);
            break;

        case '4':
            forwrd = forwrd * cos(-rate) + upwrd * sin(-rate);
            upwrd = upwrd * cos(-rate) - forwrd * sin(-rate);
            break;

        case '5':
            upwrd = upwrd * cos(rate) + rightwrd * sin(rate);
            rightwrd = rightwrd * cos(rate) - upwrd * sin(rate);
            break;

        case '6':
            upwrd = upwrd * cos(-rate) + rightwrd * sin(-rate);
            rightwrd = rightwrd * cos(-rate) - upwrd * sin(-rate);
            break;

        case 'i':
            if (center.x + spherefor.x * sprate + radius > 5 || center.x + spherefor.x * sprate - radius < -5) {
                spherefor.x *= -1;
            }
            if (center.y + spherefor.y * sprate + radius > 5 || center.y + spherefor.y * sprate - radius < -5) {
                    spherefor.y *= -1;
                }
            center = center + spherefor * sprate;
            break;
        case 'k':
            if (center.x - spherefor.x * sprate + radius > 5 || center.x - spherefor.x * sprate - radius < -5) {
                spherefor.x *= -1;
            }
            if (center.y - spherefor.y * sprate + radius > 5 || center.y - spherefor.y * sprate - radius < -5) {
                    spherefor.y *= -1;
                }
            center = center - spherefor * sprate;
            break;
        case 'j':
            sphereright = sphereright * cos(rate) + spherefor * sin(rate);
            spherefor = spherefor * cos(rate) - sphereright * sin(rate);
            break;
        case 'l':
            sphereright = sphereright * cos(-rate) + spherefor * sin(-rate);
            spherefor = spherefor * cos(-rate) - sphereright * sin(-rate);
            break;
        // case ',':
        //     if (triangleConst > 0.0) triangleConst -= shrink;
        //     break;

        // case '.':
        //     if (triangleConst < 1.0) triangleConst += shrink;
        //     break;

        default:
            printf("Unknown Key Pressed\n");
            break;
    }
    focus = camera + forwrd;
    sphereright = spherefor.cross(sphereup);
}
void special_handler(int key, int x, int y) { 
    float rate = 0.8;
    switch(key) {
        case GLUT_KEY_UP: // forwrd
            printf("Up Pressed\n");
            camera = camera + forwrd * rate;
            focus = focus + forwrd * rate;
            break;
        case GLUT_KEY_DOWN: // backward
            printf("Down Pressed\n");
            camera = camera - forwrd * rate;
            focus = focus - forwrd * rate;
            break;
        case GLUT_KEY_LEFT: // left
            printf("Left Pressed\n");
            camera = camera - rightwrd * rate;
            focus = focus - rightwrd * rate;
            break;
        case GLUT_KEY_RIGHT: // left
            printf("Right Pressed\n");
            camera = camera + rightwrd * rate;
            focus = focus + rightwrd * rate;
            break;
        case GLUT_KEY_PAGE_UP: // pageup 9
            printf("Page Up Pressed\n");
            camera = camera + upwrd * rate;
            focus = focus + upwrd * rate;
            break;
        case GLUT_KEY_PAGE_DOWN: // pagedown 3
            printf("Page Down Pressed\n");
            camera = camera - upwrd * rate;
            focus = focus - upwrd * rate;
            break;
        default:
            printf("Unknown Key Pressed\n");
            break;
    }
}

int main(int argc, char** argv){
    printf("Hello World\n");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    initialize();
    
    glutCreateWindow("OpenGL Setup Test");
    

    glutDisplayFunc(display);
    
    glutKeyboardFunc(keyboard_handler);
    glutSpecialFunc(special_handler);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, Timer, 0);
    initGL();
    
    glutMainLoop();

    return 0;
}