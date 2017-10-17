// 2014182037_LH.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
#include "stdafx.h"
using namespace std;

typedef struct Rectangle // �簢��
{
	float Right_up[2];
	float Right_down[2];
	float Left_up[2];
	float Left_down[2];


	float percent_plus; // �簢���� ������ ������ ��ġ !! ( �̰� �������� )

	bool Make_check; // �簢�� ���� ���� Ȯ��
	bool Direct_Rec; // �簢�� ���� Ȯ��
	bool Slice_check; // �簢�� �߷ȴ��� Ȯ�� -> 1�̸� ������ �߰�

}Rectangle;

typedef struct Basket // �ٱ���
{
	float Right_up[2];
	float Right_down[2];
	float Left_up[2];
	float Left_down[2];


	bool Make_Basket = 0;
	bool Move_flag = 0; // 0�̸� �������� 1�̸� ����������

}Basket;

typedef struct Point
{
	float x;
	float y;
	bool make_flag;
}Point;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid MouseCallback(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char, int, int);
GLvoid Timer(int);
GLvoid Mouse_motion(int, int);

GLvoid Make_Move_Rectangle(); // �����̴� �簢�� ����
GLvoid Make_Basket();        // �ٱ��� ����
GLvoid Make_Water_Rec();     // �ٱ��Ͼ��� �� ����

GLvoid Draw_Move_Rectangle(); // �����̴� �簢�� �׸���
GLvoid Draw_Move_Basket();    // �ٱ��� �׸���
GLvoid Draw_Water_Rec();    // �ٱ��� ���� �� �׸���



GLvoid Check_Move_Basket(); // �ٱ��ϰ� �����̴� �Լ�
GLvoid Check_Move_Rec(); // �ٱ��ϰ� �����̴� �Լ�






Rectangle Move_Rec; //ù��° �簢���� �ڸ��� �簢��
Rectangle Water_Rec; // �ι�° �簢���� �ٱ��� �ȿ� �ִ� �� 
Basket Move_Basket; // �ٱ���

Point mouse_drag[2];

bool left_mouse_up = 0;
bool left_mouse_down = 0;
bool basket_check = 0;


float mx, my; // Ŭ���� ��ǥ��


default_random_engine dre;
uniform_int_distribution<int> uid(1, 5);
uniform_real_distribution<double> R_color(0.1, 1.0);
uniform_real_distribution<double> G_color(0.1, 1.0);
uniform_real_distribution<double> B_color(0.1, 1.0);

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("2014182037_LH");
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutReshapeFunc(Reshape);
	glutMouseFunc(MouseCallback);
	glutMotionFunc(Mouse_motion);
	glutTimerFunc(100, Timer, 1);
	glutMainLoop();
}





GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // RGB
	glClear(GL_COLOR_BUFFER_BIT);  //���� ���������� ����
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();  //1
	glTranslatef(400.0, 300.0, 0.0);   //���� ��ǥ ��ȯ ���

	Draw_Water_Rec();
	Draw_Move_Basket();
	Draw_Move_Rectangle();

	if (mouse_drag[0].make_flag == 1 && mouse_drag[1].make_flag == 1)
	{
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_LINE_STRIP);
		glVertex2f(mouse_drag[0].x, mouse_drag[0].y);
		glVertex2f(mouse_drag[1].x, mouse_drag[1].y);
		glEnd();
		glColor3f(1.0, 1.0, 1.0);
		mouse_drag[0].make_flag = 0;
		mouse_drag[1].make_flag = 0;
	}


	glPopMatrix();// 1


	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0.0, 800.0, 0.0, 600.0, -400.0, 400.0);
}

GLvoid MouseCallback(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) // ���콺 ��������
	{
		mx = x-400;
		my = 600 - y - 300;

		mouse_drag[0].x = mx;
		mouse_drag[0].y = my;
		mouse_drag[0].make_flag = 1;
		

		left_mouse_down = 1;
		left_mouse_up = 0;

	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) // ���콺 ��������
	{
		mx = x - 400;
		my = 600 - y - 300;

		mouse_drag[1].x = mx;
		mouse_drag[1].y = my;
		mouse_drag[1].make_flag = 1;


		left_mouse_down = 0;
		left_mouse_up = 1;
		
		//���콺 ���������� �������� �����ڸ��� ����
		

	}
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	if (key == 'a' || key == 'A') // �簢�� ���� 
	{
		if (basket_check == 0)       // ( �ѹ��� �۵� )
		{
			Make_Basket();        // �ٱ��� ����
			basket_check = 1;
		}

		Make_Move_Rectangle();          // a�� ������ ���� �簢�� ����
		Move_Rec.Make_check = 1;
	}
	else if (key == 'i' || key == 'I')
	{
		printf("(%f,%f)\n", mouse_drag[0].x, mouse_drag[0].y);
		printf("(%f,%f)\n", mouse_drag[1].x, mouse_drag[1].y);

	}
	else if (key == 'q' || key == 'Q')
	{
		exit(0);
	}
}




GLvoid Timer(int value)
{
	Check_Move_Rec();     // �簢���� �����δ�.
	Check_Move_Basket();  // �ٱ��� ������
	Make_Water_Rec();     // �ٱ��� ���� �� ����



	glutPostRedisplay();
	glutTimerFunc(100, Timer, 1);
}

GLvoid Mouse_motion(int x, int y)
{
	x = x - 400;
	y = 600-y - 300;
	


}

GLvoid Make_Move_Rectangle()
{
	Move_Rec.Right_up[0] = 500.0;
	Move_Rec.Right_up[1] = 250.0;

	Move_Rec.Right_down[0] = 500.0;
	Move_Rec.Right_down[1] = 150.0;

	Move_Rec.Left_up[0] = 300.0;
	Move_Rec.Left_up[1] = 250.0;

	Move_Rec.Left_down[0] = 300.0;
	Move_Rec.Left_down[1] = 150.0;

	Move_Rec.Make_check = 0;
	Move_Rec.Direct_Rec = 0;
}

GLvoid Make_Basket()
{
	Move_Basket.Right_up[0] = 400.0;
	Move_Basket.Right_up[1] = -200.0;

	Move_Basket.Right_down[0] = 400.0;
	Move_Basket.Right_down[1] = -300.0;

	Move_Basket.Left_up[0] = 200.0;
	Move_Basket.Left_up[1] = -200.0;

	Move_Basket.Left_down[0] = 200.0;
	Move_Basket.Left_down[1] = -300.0;

	Move_Basket.Make_Basket = 1;
}

GLvoid Make_Water_Rec()
{

	//���� ���̴� �ٱ����� �� ���� ���.
	Water_Rec.percent_plus = 20.0; // ó�� �� ��ġ


	Water_Rec.Right_up[0] = Move_Basket.Right_down[0];
	Water_Rec.Right_up[1] = Move_Basket.Right_down[1] + Water_Rec.percent_plus;

	Water_Rec.Right_down[0] = Move_Basket.Right_down[0];
	Water_Rec.Right_down[1] = Move_Basket.Right_down[1];

	Water_Rec.Left_up[0] = Move_Basket.Left_up[0];
	Water_Rec.Left_up[1] = Move_Basket.Left_down[1] + Water_Rec.percent_plus;

	Water_Rec.Left_down[0] = Move_Basket.Left_down[0];
	Water_Rec.Left_down[1] = Move_Basket.Left_down[1];


	Water_Rec.Make_check = 1;

	if (Move_Basket.Right_down[0] > 400.0) // ���� ���鿡 ������
	{
		//���� �ĵ� ���
		Water_Rec.Right_up[1] += Water_Rec.percent_plus*1.0;
		Water_Rec.Left_up[1] -= Water_Rec.percent_plus*1.0;
	}

	if (Move_Basket.Left_down[0] <= -400.0) // ���� ���鿡 ������
	{
		//���� �ĵ� ���
		Water_Rec.Right_up[1] -= Water_Rec.percent_plus*1.0;
		Water_Rec.Left_up[1] += Water_Rec.percent_plus*1.0;
	}
}

GLvoid Draw_Move_Rectangle()
{
	if (Move_Rec.Make_check == 1)
	{
		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_POLYGON);
		glVertex2f(Move_Rec.Right_up[0], Move_Rec.Right_up[1]);
		glVertex2f(Move_Rec.Left_up[0], Move_Rec.Left_up[1]);
		glVertex2f(Move_Rec.Left_down[0], Move_Rec.Left_down[1]);
		glVertex2f(Move_Rec.Right_down[0], Move_Rec.Right_down[1]);
		glEnd();
		glColor3f(1.0, 1.0, 1.0);
		glPopMatrix();
	}
}


GLvoid Draw_Move_Basket()
{
	if (Move_Basket.Make_Basket == 1)
	{
		glPushMatrix();
		glColor3f(1.0, 0.0, 0.0);
		glLineWidth(5.0);
		glBegin(GL_LINE_STRIP);
		glVertex2f(Move_Basket.Right_up[0], Move_Basket.Right_up[1]);
		glVertex2f(Move_Basket.Right_down[0], Move_Basket.Right_down[1]);
		glVertex2f(Move_Basket.Left_down[0], Move_Basket.Left_down[1]);
		glVertex2f(Move_Basket.Left_up[0], Move_Basket.Left_up[1]);
		glEnd();
		glColor3f(1.0, 1.0, 1.0);
		glPopMatrix();
	}
}

GLvoid Draw_Water_Rec()
{
	if (Move_Rec.Make_check == 1)
	{
		glPushMatrix();
		glColor3f(0.0, 0.0, 0.6);
		glBegin(GL_POLYGON);
		glVertex2f(Water_Rec.Right_up[0], Water_Rec.Right_up[1]);
		glVertex2f(Water_Rec.Left_up[0], Water_Rec.Left_up[1]);
		glVertex2f(Water_Rec.Left_down[0], Water_Rec.Left_down[1]);
		glVertex2f(Water_Rec.Right_down[0], Water_Rec.Right_down[1]);
		glEnd();
		glColor3f(1.0, 1.0, 1.0);
		glPopMatrix();
	}
}

GLvoid Check_Move_Basket()
{
	if (Move_Basket.Move_flag == 0) // ��������
	{
		if (Move_Basket.Left_down[0] <= -400.0) // ���� ���鿡 ������
		{
			Move_Basket.Move_flag = 1;

			//���� �ĵ� ���
			Water_Rec.Right_up[1] -= Water_Rec.percent_plus*1.5;
			Water_Rec.Left_up[1] += Water_Rec.percent_plus*1.5;
		}
		else
		{

			Move_Basket.Right_up[0] -= 20.0;

			Move_Basket.Right_down[0] -= 20.0;

			Move_Basket.Left_up[0] -= 20.0;

			Move_Basket.Left_down[0] -= 20.0;

		}
	}

	if (Move_Basket.Move_flag == 1) // ����������
	{
		if (Move_Basket.Right_down[0] > 400.0) // ���� ���鿡 ������
		{
			Move_Basket.Move_flag = 0;

			//���� �ĵ� ���
			Water_Rec.Right_up[1] += Water_Rec.percent_plus*1.5;
			Water_Rec.Left_up[1] -= Water_Rec.percent_plus*1.5;
		}
		else
		{
			Move_Basket.Right_up[0] += 20.0;

			Move_Basket.Right_down[0] += 20.0;

			Move_Basket.Left_up[0] += 20.0;

			Move_Basket.Left_down[0] += 20.0;
		}
	}

}

GLvoid Check_Move_Rec()
{
	if (Move_Rec.Make_check == 1)
	{
		if (Move_Rec.Direct_Rec == 0)  // �������� ��
		{
			if ((Move_Rec.Left_up[0] <= -400.0 || Move_Rec.Left_down[0] <= -400.0) && Move_Rec.Slice_check == 0) // ���߿� ������ �߸��� ������ �ٲ��ش�. 
				Move_Rec.Direct_Rec = 1;
			else
			{
				Move_Rec.Right_up[0] -= 10;

				Move_Rec.Right_down[0] -= 10;

				Move_Rec.Left_up[0] -= 10;

				Move_Rec.Left_down[0] -= 10;
			}
		}
		if (Move_Rec.Direct_Rec == 1) // ���������� ��
		{
			if ((Move_Rec.Right_up[0] > 400.0 || Move_Rec.Right_down[0] > 400.0) && Move_Rec.Slice_check == 0) 
				// ���߿� ������ �߸��� ������ �ٲ��ش�.  [ 1. �簢���� ���� �����  2. flag �߰��Ѵ�  3. ������ �ø���(�߸�������)
				Move_Rec.Direct_Rec = 0;
			else
			{
				Move_Rec.Right_up[0] += 10;

				Move_Rec.Right_down[0] += 10;

				Move_Rec.Left_up[0] += 10;

				Move_Rec.Left_down[0] += 10;
			}
		}

	}
}




