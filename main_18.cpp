#include <gl\glut.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <random>
#include <string>
using namespace std;

typedef struct shape
{
	float x_y_z[3]; // 클릭한 x y z 좌표
	float scale_size[3]; // 크기변화 x y z
	float roate_size[4]; // angle,x,y,z 
	float random_color[3];
	bool make_flag; // 도형이 만들어 졌는지 확인
	int random_shape;

}Shape;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid MouseCallback(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char, int, int);
GLvoid Timer(int);
GLvoid Menufunc(int);



GLvoid draw_center_line();
GLvoid init_local();
GLvoid init_shape(int);

bool left_mouse_flag = 0;
bool a_flag = 0;
bool r_flag = 0;
bool l_flag = 0;

float zoom_x = 1.0;
float zoom_y = 1.0;
float zoom_z = 1.0;
float move_x = 0.0;
float move_y = 0.0;
float move_z = 0.0;
float rotate_x = 0.0;
float rotate_y = 0.0;
float rotate_z = 0.0;
float rotate_angle = 0.0;

float Width, Height;
int shape_count = 0;

float local_1[16]; // 회전행렬 
float local_2[16]; // 이동행렬 


default_random_engine dre;
uniform_int_distribution<int> uid(0, 2);
uniform_real_distribution<double> R_color(0.1, 1.0);
uniform_real_distribution<double> G_color(0.1, 1.0);
uniform_real_distribution<double> B_color(0.1, 1.0);

Shape shape_arr[2];
GLUquadricObj* Obj_01;

void main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example18");
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutReshapeFunc(Reshape);
	glutMouseFunc(MouseCallback);

	glutTimerFunc(50, Timer, 1);
	glutMainLoop();
}




GLvoid drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // RGB
	glClear(GL_COLOR_BUFFER_BIT);  //바탕 검은색으로 도배
	glMatrixMode(GL_MODELVIEW);
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glRotatef(-10.0, 1.0, 0.0, 0.0);

	glPushMatrix();			//1
	glTranslatef(move_x, move_y, move_z);
	glRotatef(rotate_angle, rotate_x, rotate_y, rotate_z);
	glScalef(zoom_x, zoom_y, zoom_z);

	//glMultMatrixf(local_1);  // 회전 행렬을 한번에 적용하겠다.
	for (int i = 0; i < 16; ++i)
		printf("%f", local_1[i]);
	printf("\n");
	
	

	glPushMatrix();  //2
	glColor3f(0.85, 0.75, 0.0);   // 색깔 노랑 설정
	glTranslatef(0.0, -150 * Height / Width, 0.0);  // 맨 밑에 깔았음
	glScalef(45.0, 10.0, 1.0);

	Obj_01 = gluNewQuadric();
	gluQuadricDrawStyle(Obj_01, GLU_FILL);
	gluSphere(Obj_01, 10.0, 10, 10);
	glPopMatrix();   //2

	


	draw_center_line();
	glPopMatrix();			//1
	glPopMatrix();

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	Width = w;
	Height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(60.0f, w / h, 1.0, 1500.0);
	glTranslatef(0.0, 0.0, -500.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

GLvoid MouseCallback(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{


		left_mouse_flag = 1;
	}
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	if (key == 'x' || key == 'X') // x축으로 회전
	{
		if (key == 'x')            // 양의 방향
		{
			if (rotate_x == 0.0)
				rotate_angle = 0.0;
			rotate_angle += 30.0;
			rotate_x = 1.0;
			rotate_y = 0.0;
			rotate_z = 0.0;
		}
		else if (key == 'X')    // 음의 방향
		{
			if (rotate_x == 0.0)
				rotate_angle = 0.0;
			rotate_angle -= 30.0;
			rotate_x = 1.0;
			rotate_y = 0.0;
			rotate_z = 0.0;
		}
	}

	else if (key == 'y' || key == 'Y') // y축으로 회전
	{
		if (key == 'y')
		{
			if (rotate_y == 0.0)         // 양의 방향
				rotate_angle = 0.0;
			rotate_angle += 30.0;
			rotate_x = 0.0;
			rotate_y = 1.0;
			rotate_z = 0.0;
		}
		else if (key == 'Y')
		{
			if (rotate_y == 0.0)		  // 음의 방향
				rotate_angle = 0.0;
			rotate_angle -= 30.0;
			rotate_x = 0.0;
			rotate_y = 1.0;
			rotate_z = 0.0;
		}
	}
	else if (key == 'z' || key == 'Z') // z축으로 회전
	{
		if (key == 'z')
		{
			if (rotate_z == 0.0)		// 양의 방향
				rotate_angle = 0.0;
			rotate_angle += 30.0;
			rotate_x = 0.0;
			rotate_y = 0.0;
			rotate_z = 1.0;
		}
		else if (key == 'Z')
		{
			if (rotate_z == 0.0)		   // 음의 방향
				rotate_angle = 0.0;
			rotate_angle -= 30.0;
			rotate_x = 0.0;
			rotate_y = 0.0;
			rotate_z = 1.0;
		}
	}
	else if (key == '+')  // 제자리에서 확대
	{
		zoom_x += 0.1;
		zoom_y += 0.1;
		zoom_z += 0.1;
	}
	else if (key == '-') // 제자리에서 축소
	{
		zoom_x -= 0.1;
		zoom_y -= 0.1;
		zoom_z -= 0.1;
	}
	else if (key == 'w' || key == 'W')  // y축 양의 방향 이동
	{
		move_y += 20.0;
	}
	else if (key == 'a' || key == 'A') // x축 음의 방향 이동
	{
		move_x -= 20.0;
	}
	else if (key == 's' || key == 'S') // y축 음의 방향 이동
	{
		move_y -= 20.0;
	}
	else if (key == 'd' || key == 'D') // x축 양의 방향 이동
	{
		move_x += 20.0;
	}

	else if (key == 'm')  // y축 방향 애니메이션
	{
		if (a_flag == 0)
			a_flag = 1;
		else
			a_flag = 0;
	}
	else if (key == 'i' || key == 'I') // 제자리로 리셋
	{
		glLoadIdentity();

		zoom_x = 1.0;
		zoom_y = 1.0;
		zoom_z = 1.0;

		rotate_x = 0.0;
		rotate_y = 0.0;
		rotate_z = 0.0;
		rotate_angle = 0.0;

		move_x = 0.0;
		move_y = 0.0;
		move_z = 0.0;

		init_shape(0);

	}
	

	else if (key == 'q' || key == 'Q') // 프로그램 종료
	{
		exit(0);
	}

	glutPostRedisplay();
}

GLvoid draw_center_line()
{

	glLineWidth(3.0);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0); // x축은 빨강
	glVertex3f(-10.0, 0.0, 0.0);
	glVertex3f(10.0, 0.0, 0.0);
	glColor3f(0.0, 1.0, 0.0); // y축은 초록
	glVertex3f(0.0, -10.0, 0.0);
	glVertex3f(0.0, 10.0, 0.0);
	glColor3f(0.0, 0.0, 1.0); // z축은 파랑
	glVertex3f(0.0, 0.0, 10.0);
	glVertex3f(0.0, 0.0, -10.0);
	glEnd();

	//	glPushMatrix();
	//	{
	//		glColor3f(1.0f, 0.0f, 0.0f);
	//		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	//		glScalef(0.1f, 10.0f, 0.1f);
	//		glutSolidCube(1.0f);
	//	}
	//	glPopMatrix();

	//	glPushMatrix();
	//	{
	//		glColor3f(0.0f, 1.0f, 0.0f);
	//		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	//		glScalef(0.1f, 10.0f, 0.1f);
	//		glutSolidCube(1.0f);
	//	}
	//	glPopMatrix();

	//	glPushMatrix();
	//	{
	//		glColor3f(0.0f, 0.0f, 1.0f);
	//		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	//		glScalef(0.1f, 10.0f, 0.1f);
	//		glutSolidCube(1.0f);
	//	}
	//	glPopMatrix();

}

GLvoid init_local()
{
	memset(local_1, 0, sizeof(local_1));
	local_1[0] = local_1[5] = local_1[10] = local_1[15] = 1;  // 처음엔 단위행렬을 적용해주겠다.

	memset(local_2, 0, sizeof(local_2));
	local_2[0] = local_2[5] = local_2[10] = local_2[15] = 1;  // 처음엔 단위행렬을 적용해주겠다.
}



GLvoid Timer(int value)
{
	
	glutPostRedisplay();
	glutTimerFunc(50, Timer, 1);
}







GLvoid init_shape(int check)
{
	

}
