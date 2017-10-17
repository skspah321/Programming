// 2014182037_LH.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
#include "stdafx.h"
using namespace std;

typedef struct Rectangle // 사각형
{
	float Right_up[2];
	float Right_down[2];
	float Left_up[2];
	float Left_down[2];


	float percent_plus; // 사각형이 들어오면 증가할 수치 !! ( 이건 물에서만 )

	bool Make_check; // 사각형 생성 여부 확인
	bool Direct_Rec; // 사각형 방향 확인
	bool Slice_check; // 사각형 잘렸는지 확인 -> 1이면 가던길 쭉감

}Rectangle;

typedef struct Basket // 바구니
{
	float Right_up[2];
	float Right_down[2];
	float Left_up[2];
	float Left_down[2];


	bool Make_Basket = 0;
	bool Move_flag = 0; // 0이면 왼쪽으로 1이면 오른쪽으로

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

GLvoid Make_Move_Rectangle(); // 움직이는 사각형 생성
GLvoid Make_Basket();        // 바구니 생성
GLvoid Make_Water_Rec();     // 바구니안의 물 생성

GLvoid Draw_Move_Rectangle(); // 움직이는 사각형 그리기
GLvoid Draw_Move_Basket();    // 바구니 그리기
GLvoid Draw_Water_Rec();    // 바구니 안의 물 그리기



GLvoid Check_Move_Basket(); // 바구니가 움직이는 함수
GLvoid Check_Move_Rec(); // 바구니가 움직이는 함수






Rectangle Move_Rec; //첫번째 사각형은 자르는 사각형
Rectangle Water_Rec; // 두번째 사각형은 바구니 안에 있는 물 
Basket Move_Basket; // 바구니

Point mouse_drag[2];

bool left_mouse_up = 0;
bool left_mouse_down = 0;
bool basket_check = 0;


float mx, my; // 클릭한 좌표값


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
	glClear(GL_COLOR_BUFFER_BIT);  //바탕 검은색으로 도배
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();  //1
	glTranslatef(400.0, 300.0, 0.0);   //월드 좌표 변환 행렬

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
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) // 마우스 눌렸을때
	{
		mx = x-400;
		my = 600 - y - 300;

		mouse_drag[0].x = mx;
		mouse_drag[0].y = my;
		mouse_drag[0].make_flag = 1;
		

		left_mouse_down = 1;
		left_mouse_up = 0;

	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) // 마우스 때졌을때
	{
		mx = x - 400;
		my = 600 - y - 300;

		mouse_drag[1].x = mx;
		mouse_drag[1].y = my;
		mouse_drag[1].make_flag = 1;


		left_mouse_down = 0;
		left_mouse_up = 1;
		
		//마우스 떄졌을때를 기준으로 도형자르기 시작
		

	}
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	if (key == 'a' || key == 'A') // 사각형 생성 
	{
		if (basket_check == 0)       // ( 한번만 작동 )
		{
			Make_Basket();        // 바구니 생성
			basket_check = 1;
		}

		Make_Move_Rectangle();          // a가 눌릴때 마다 사각형 생성
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
	Check_Move_Rec();     // 사각형이 움직인다.
	Check_Move_Basket();  // 바구니 움직임
	Make_Water_Rec();     // 바구니 안의 물 생성



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

	//물의 높이는 바구니의 위 점에 비례.
	Water_Rec.percent_plus = 20.0; // 처음 물 수치


	Water_Rec.Right_up[0] = Move_Basket.Right_down[0];
	Water_Rec.Right_up[1] = Move_Basket.Right_down[1] + Water_Rec.percent_plus;

	Water_Rec.Right_down[0] = Move_Basket.Right_down[0];
	Water_Rec.Right_down[1] = Move_Basket.Right_down[1];

	Water_Rec.Left_up[0] = Move_Basket.Left_up[0];
	Water_Rec.Left_up[1] = Move_Basket.Left_down[1] + Water_Rec.percent_plus;

	Water_Rec.Left_down[0] = Move_Basket.Left_down[0];
	Water_Rec.Left_down[1] = Move_Basket.Left_down[1];


	Water_Rec.Make_check = 1;

	if (Move_Basket.Right_down[0] > 400.0) // 왼쪽 벽면에 닿으면
	{
		//물의 파도 기능
		Water_Rec.Right_up[1] += Water_Rec.percent_plus*1.0;
		Water_Rec.Left_up[1] -= Water_Rec.percent_plus*1.0;
	}

	if (Move_Basket.Left_down[0] <= -400.0) // 왼쪽 벽면에 닿으면
	{
		//물의 파도 기능
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
	if (Move_Basket.Move_flag == 0) // 왼쪽으로
	{
		if (Move_Basket.Left_down[0] <= -400.0) // 왼쪽 벽면에 닿으면
		{
			Move_Basket.Move_flag = 1;

			//물의 파도 기능
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

	if (Move_Basket.Move_flag == 1) // 오른쪽으로
	{
		if (Move_Basket.Right_down[0] > 400.0) // 왼쪽 벽면에 닿으면
		{
			Move_Basket.Move_flag = 0;

			//물의 파도 기능
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
		if (Move_Rec.Direct_Rec == 0)  // 왼쪽으로 감
		{
			if ((Move_Rec.Left_up[0] <= -400.0 || Move_Rec.Left_down[0] <= -400.0) && Move_Rec.Slice_check == 0) // 나중에 도형이 잘리는 순간에 바꿔준다. 
				Move_Rec.Direct_Rec = 1;
			else
			{
				Move_Rec.Right_up[0] -= 10;

				Move_Rec.Right_down[0] -= 10;

				Move_Rec.Left_up[0] -= 10;

				Move_Rec.Left_down[0] -= 10;
			}
		}
		if (Move_Rec.Direct_Rec == 1) // 오른쪽으로 감
		{
			if ((Move_Rec.Right_up[0] > 400.0 || Move_Rec.Right_down[0] > 400.0) && Move_Rec.Slice_check == 0) 
				// 나중에 도형이 잘리는 순간에 바꿔준다.  [ 1. 사각형을 새로 만든다  2. flag 추가한다  3. 조건을 늘린다(잘린점으로)
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




