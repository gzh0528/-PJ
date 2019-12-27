#include <GL/glut.h>
#include <cmath>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

/*定义颜色全局变量*/

GLfloat WHITE[] = { 1, 1, 1 };    
GLfloat RED[] = { 1, 0, 0 };    
GLfloat GREEN[] = { 0, 1, 0 };    
GLfloat MAGENTA[] = { 1, 0, 1 };  

/*定义辅助全局变量*/
GLint view = 0;
GLint flag;
GLuint drawcube, drawsphere, drawteapot;
const GLfloat Pi = 3.1415926536f; //圆周率
const int n = 1000;


/*
Camera 类： 实现真实场景的视角转换 水平移动半径为10，按上下键垂直移动
Ball 类：定义球型元件类 设计可在Y轴方向上下跳动
Cube 类：定义立方体元件类
Checkerboard：定义棋盘元件类 X,Z平面分布 并在上方设置点光源(4,3,7)
Pig 类：绘制一个小猪实体
*/
class Camera {
	/*利用图形学增量法更改坐标*/
public:
	double theta;      //确定x和z的位置
	double y;          //y位置
	double dTheta;     //角度增量
	double dy;         //上下y增量
public:
	//类构造函数—默认初始化用法
	Camera() : theta(0), y(3), dTheta(0.04), dy(0.2) {}
	//类方法
	double X_data() { return 10 * cos(theta); }
	double Y_data() { return y; }
	double Z_data() { return 10 * sin(theta); }
	void moveR() { theta += dTheta; }
	void moveL() { theta -= dTheta; }
	void moveU() { y += dy; }
	void moveD() { if (y > dy) y -= dy; }
};

class Ball {
	//类的属性
	double radius;
	GLfloat* color;
	double maximumHeight;
	double x;
	double y;
	double z;
	int direction;   //方向
public:
	//构造函数
	Ball(double r, GLfloat* c, double h, double x, double z) :
		radius(r), color(c), maximumHeight(h), direction(-1),
		y(h), x(x), z(z) {
	}

	//更新和绘制方法
	void update() {
		//正反运动
		y += direction * 0.05;
		if (y > maximumHeight) {
			y = maximumHeight;
			direction = -1;
		}
		else if (y < radius) {
			y = radius;
			direction = 1;
		}
		glPushMatrix();
		//单独设置每个球的材质参数
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		glTranslated(x, y, z);
		//创建球
		glutSolidSphere(radius, 30, 30);
		glPopMatrix();
	}
};

class Cube
{
	//类的属性
	double dsize;
	GLfloat* color;
	double maximumHeight;
	double x;
	double y;
	double z;
	int direction;   //方向
public:
	//构造函数
	Cube(double d, GLfloat* c, double h, double x, double z) :
		dsize(d),color(c), maximumHeight(h), direction(-1),
		y(h), x(x), z(z) {
	}

	//更新和绘制方法
	void update() {
		//上下正反运动
		y += direction * 0.05;
		if (y > maximumHeight) {
			y = maximumHeight;
			direction = -1;
		}
		else if (y < dsize) {
			y = dsize;
			direction = 1;
		}
		glPushMatrix();
		//单独设置每个立方体的材质参数
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		glTranslated(x, y, z);
		//创建立方体
		glutSolidCube(dsize);
		glPopMatrix();
	}
};

class Checkerboard {
	int displayListId;
	int width;
	int depth;
public:
	//构造函数
	Checkerboard(int width, int depth) : width(width), depth(depth) {}
	void create_board() {
		displayListId = glGenLists(1);     
		glNewList(displayListId, GL_COMPILE); 
		GLfloat lightPosition[] = { 4, 3, 7, 1 };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glBegin(GL_QUADS);
		glNormal3d(0, 1, 0);
		//保持x，z平面进行展开
		for (int x = 0; x < width - 1; x++) 
		{
			for (int z = 0; z < depth - 1; z++) 
			{
			//每个格子设置材料属性 
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
					(x + z) % 2 == 0 ? RED : MAGENTA);
				glVertex3d(x + 1, 0, z + 1);
				glVertex3d(x, 0, z + 1);
				glVertex3d(x, 0, z);
				glVertex3d(x + 1, 0, z);

			}
		}

		glEnd();
		glEndList();
	}
	//两个坐标中心
	double centerx() { return width / 2; }
	double centerz() { return depth / 2; }
	void draw() {
		glCallList(displayListId);
	}
};

class Pig {
	GLfloat ax;
	GLfloat ay;
	GLfloat az;
	GLfloat mx;
	GLfloat my;
	GLfloat mz;
	GLfloat* theta;// [3] = { 0.0, -90.0, 0.0 };
public:
	//构造函数
	//Checkerboard(int width, int depth) : width(width), depth(depth) {}
	Pig(double axis_x,double axis_y,double axis_z,double size_x,double size_y,double size_z,GLfloat* theta)
		:ax(axis_x),ay(axis_y),az(axis_z),mx(size_x),my(size_y),mz(size_z),theta(theta){}
	//中心X
	void draw1() //画第一只猪
	{

		glTranslatef(ax, ay, az);
		glScalef(mx, my, mz);
		//glRotatef(theta[0], 1.0, 0.0, 0.0);
		glRotatef(theta[1], 0.0, 1.0, 0.0);

		glPushMatrix();
		glEnable(GL_COLOR_MATERIAL);
		//glTranslatef(0.15, -0.2, 0.125);
		glTranslatef(0.15, -0.2, 0.125);
		glColor3f(1.0, 0.5, 0.5);
		glutSolidSphere(0.05f, 100, 100);
		glDisable(GL_COLOR_MATERIAL);
		glPopMatrix();
		GLUquadricObj* pObj;
		glPushMatrix();
		glEnable(GL_COLOR_MATERIAL);
		pObj = gluNewQuadric();
		gluQuadricNormals(pObj, GLU_SMOOTH);
		glTranslatef(0, 0.55f, 0);
		glRotatef(90, 1, 0, 0);
		glColor3f(0.2, 0.2, 0.2);
		gluCylinder(pObj, 0.1f, 0.2f, 0.27f, 50, 50);
		glDisable(GL_COLOR_MATERIAL);
		glPopMatrix();

		glPushMatrix();
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(1.0, 0.5, 0.5);
		glTranslatef(-0.15, -0.2, 0.125);
		glutSolidSphere(0.05f, 100, 100);
		glDisable(GL_COLOR_MATERIAL);
		glPopMatrix();

		glPushMatrix();
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(1.0, 0.5, 0.5);
		glTranslatef(0.15, -0.2, -0.125);
		glutSolidSphere(0.05f, 100, 100);
		glDisable(GL_COLOR_MATERIAL);
		glPopMatrix();

		glPushMatrix();
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(1.0, 0.5, 0.5);
		glTranslatef(-0.15, -0.2, -0.125);
		glutSolidSphere(0.05f, 100, 100);
		glDisable(GL_COLOR_MATERIAL);
		glPopMatrix();

		glPushMatrix();
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(1.0, 0.5, 0.5);
		GLUquadricObj* quadratic;
		quadratic = gluNewQuadric();
		gluCylinder(quadratic, 0.05f, 0.05f, 0.27f, 50, 50);
		glDisable(GL_COLOR_MATERIAL);
		glPopMatrix();

		glPushMatrix();
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(1.0, 0.5, 0.5);
		glDisable(GL_COLOR_MATERIAL);
		glBegin(GL_TRIANGLES);
		glVertex3f(0.2, 0.28, 0.15);
		glVertex3f(0.19, 0.06, 0.15);
		glVertex3f(0.07, 0.21, 0.1);

		glVertex3f(0.2, 0.28, 0.15);
		glVertex3f(0.07, 0.21, 0.1);
		glVertex3f(0.12, 0.2, 0.08);

		glVertex3f(0.2, 0.28, 0.15);
		glVertex3f(0.19, 0.06, 0.15);
		glVertex3f(0.12, 0.2, 0.08);
		glEnd();
		glEnable(GL_COLOR_MATERIAL);
		glPopMatrix();

		glPushMatrix();

		glColor3f(1.0, 0.5, 0.5);
		glDisable(GL_COLOR_MATERIAL);
		glBegin(GL_TRIANGLES);
		glVertex3f(-0.2, 0.28, 0.15);
		glVertex3f(-0.19, 0.06, 0.15);
		glVertex3f(-0.07, 0.21, 0.1);

		glVertex3f(-0.2, 0.28, 0.15);
		glVertex3f(-0.07, 0.21, 0.1);
		glVertex3f(-0.12, 0.2, 0.08);

		glVertex3f(-0.2, 0.28, 0.15);
		glVertex3f(-0.19, 0.06, 0.15);
		glVertex3f(-0.12, 0.2, 0.08);
		glEnd();
		glEnable(GL_COLOR_MATERIAL);
		glPopMatrix();

		//球型身体
		glPushMatrix();
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(10, 0.8, 0.8);
		//glColor3f(0.0f, 1.0f, 0.0f);
		glutSolidSphere(0.25f, 100, 100);
		glDisable(GL_COLOR_MATERIAL);
		glPopMatrix();

		glPushMatrix();
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(1.0, 0.5, 0.5);
		glBegin(GL_POLYGON);
		for (int i = 0; i < n; ++i)
			glVertex3f(0.06 * cos(2 * Pi / n * i), 0.04 * sin(2 * Pi / n * i), 0.27f);
		glEnd();
		glDisable(GL_COLOR_MATERIAL);
		glPopMatrix();

		glPushMatrix();
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_POLYGON);
		for (int i = 0; i < n; ++i)
			glVertex3f(-0.02 + 0.01 * cos(2 * Pi / n * i), 0.01 * sin(2 * Pi / n * i), 0.27f);
		glEnd();
		glDisable(GL_COLOR_MATERIAL);
		glPopMatrix();

		glPushMatrix();
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINE_STRIP);
		glLineWidth(5.0);
		for (int i = 4000; i < n + 4000; i++)
		{
			glVertex3f(0.08 * cos(i * Pi / n), -0.15 + 0.06 * sin(i * Pi / n), 0.12f);
		}
		glEnd();
		glDisable(GL_COLOR_MATERIAL);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0, 0, 0);
		glBegin(GL_POLYGON);
		for (int i = 0; i < n; ++i)
			glVertex3f(0.02 + 0.01 * cos(2 * Pi / n * i), 0.01 * sin(2 * Pi / n * i), 0.27f);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(0.0, 0.0, 0.0);
		glTranslatef(0.05, 0.08, 0.23);
		glutSolidSphere(0.013f, 1000, 1000);
		glDisable(GL_COLOR_MATERIAL);
		glPopMatrix();

		glPushMatrix();
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(0.0, 0.0, 0.0);
		glTranslatef(-0.05, 0.08, 0.23);
		glutSolidSphere(0.013f, 1000, 1000);
		glDisable(GL_COLOR_MATERIAL);

		glPopMatrix();


	}

};


/*猪的初始坐标+缩放倍数*/
GLfloat ax = 7.0f;
GLfloat ay = 0.5f;
GLfloat az = 11.0f;
GLfloat mx = 3.0f;
GLfloat my = 3.0f;
GLfloat mz = 3.0f;
GLfloat theta[3] = { 0.0, -90.0, 0.0 };
/*
全局变量：棋盘格、猪、相机和球、立方体的类型实例化
*/
Checkerboard checkerboard(15, 15);
Pig pig(ax, ay, az, mx, my, mz, theta);
Camera camera;
Ball balls[] = {
	Ball(1, GREEN, 7, 6, 1),
	Ball(0.5, MAGENTA, 6, 3, 4),
	Ball(0.4, RED, 5, 1, 7)
};

Cube cubes[] = {
	Cube(0.7,MAGENTA,4,5,3)
};


void init() {
	/*
	1.深度测试
	2.一套不解释连招 设置散射反射镜面反射的光照
	3. 在init里面创建好棋盘
	*/
	glEnable(GL_DEPTH_TEST);
	glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
	glMaterialf(GL_FRONT, GL_SHININESS, 30);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	checkerboard.create_board();
}

void display() {
	//清除上一帧的结果
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//将Camera类的坐标作为观察坐标
	gluLookAt(camera.X_data(), camera.Y_data(), camera.Z_data(),    
		checkerboard.centerx(), 0.0, checkerboard.centerz(),   //焦点坐标
		0.0, 1.0, 0.0);   //摄像机机顶方向矢量
	checkerboard.draw();
	//画个小猪
	glPushMatrix();
	pig.draw1();
	glPopMatrix();
	//绘制小球
	for (int i = 0; i < sizeof balls / sizeof(Ball); i++) {
		//更新位置并绘图
		balls[i].update();
	}
	// 绘制 立方体
	for (int i = 0; i < sizeof cubes / sizeof(Cube); i++) {
		//更新位置并绘图
		cubes[i].update();
	}

	drawcube = glGenLists(1);
	glNewList(drawcube, GL_COMPILE);
	glutSolidCube(1);
	glEndList();

	drawteapot = glGenLists(1);
	glNewList(drawteapot, GL_COMPILE);
	glutSolidTeapot(1);
	glEndList();

	glPushMatrix();
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColor3f(0, 0, 1);
	glRotatef(1, 0, 0, 180);
	glTranslatef(10, 0.3, 10);
	//加个圆环
	glutSolidTorus(0.25, 0.4, 20, 20);
	//glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();

	/*四个桌子腿*/
	glPushMatrix();
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glColor4f(0, 1, 0.0, 0.0);
	glTranslatef(8, 0.5, 6);
	glScalef(0.5, 1.0, 0.5);
	glCallList(drawcube);
	glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glColor4f(0,1.0, 0.0, 0.0);
	glTranslatef(8, 0.5, 8);
	glScalef(0.5, 1.0, 0.5);
	glCallList(drawcube);
	glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glColor4f(0, 1, 0.0, 0.0);
	glTranslatef(6,0.5,8);
	glScalef(0.5, 1.0, 0.5);
	glCallList(drawcube);
	glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glColor4f(0, 1, 0.0, 0.0);
	glTranslatef(6, 0.5, 6);
	glScalef(0.5, 1.0, 0.5);
	glCallList(drawcube);
	glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();
	/*桌面*/
	glPushMatrix();
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glColor4f(0.1, 1.0, 0.1, 0.0);
	glScalef(2.50, 0.5, 2.50);
	glTranslatef(2.8, 1.5, 2.8);
	glCallList(drawcube);
	glColor4f(0.39, 0.30, 0.1, 0.0);
	glutWireCube(1);
	glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();


	glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();
	//桌面上摆茶壶
	glPushMatrix();//茶壶
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(7, 2, 7);
	glScalef(0.625, 1.25, 0.625);
	glCallList(drawteapot);
	glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();
	//glFlush();
	glutSwapBuffers();
}


void Key_operation(int key, int, int) {
	switch (key) {
	case GLUT_KEY_LEFT: camera.moveL(); break;
	case GLUT_KEY_RIGHT: camera.moveR(); break;
	case GLUT_KEY_UP: camera.moveU(); break;
	case GLUT_KEY_DOWN: camera.moveD(); break;
	}
	glutPostRedisplay();
}
//窗口调整大小时调用的函数
void reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, GLfloat(w) / GLfloat(h), 1.0, 150.0);
	glMatrixMode(GL_MODELVIEW);
}

void timer(int v) {
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, v);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(800, 600);
	glutCreateWindow("PJ2图形学");
	glutDisplayFunc(display);   
	glutReshapeFunc(reshape);   
	glutSpecialFunc(Key_operation);   
	glutTimerFunc(100, timer, 0); 
	init();
	glutMainLoop();

	return 0;
}