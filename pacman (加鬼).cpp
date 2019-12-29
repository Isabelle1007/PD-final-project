#include <cstdio>
#include <cstdlib>
#define _USE_MATH_DEFINES 
#include <cmath>
#ifdef __APPLE__
#include <OpenGL/gl.h> 
#include <OpenGL/glu.h> 
#include <GLUT/glut.h>
#elif __FreeBSD__ || __linux
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#elif _WIN32
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL\glut.h>
#include <windows.h>
#endif

struct Color
{
	float r, g, b;
};
struct Point
{
	float x, y;
};

bool trigger_left_key = false;
bool trigger_up_key = false;
bool trigger_right_key = false;
bool trigger_down_key = false;

bool trigger_A_key = false;
bool trigger_W_key = false;
bool trigger_D_key = false;
bool trigger_S_key = false;

/* �e�@�Ӫ����
   �ѼơG
   - mid: ����Τ��߮y��
   - width: ������F��
   - height: ����ΰ���
   - c: ����ζ�R�C��
*/
void DrawRectangle(Point mid, float width, float height, Color c)
{
    glBegin(GL_QUADS); //GL_QUADS �h�տW�߶�R�|���
        glColor3f(c.r, c.g, c.b);
        glVertex3f(mid.x - width/2, mid.y - height/2, 0.0);
        glVertex3f(mid.x + width/2, mid.y - height/2, 0.0);
        glVertex3f(mid.x + width/2, mid.y + height/2, 0.0);
        glVertex3f(mid.x - width/2, mid.y + height/2, 0.0);
    glEnd();
    return;
}

/* �e�@�Ӷ��
   �ѼơG
   - mid: ���߮y��
   - radius: �b�|
   - c: ��R�C��
*/

void DrawCircle(Point mid, float radius, Color c)
{
    glBegin(GL_POLYGON); //GL_POLYGON ���²���R�Y�h���
        glColor3f(c.r, c.g, c.b);
        for(float tt = 0; tt < 2*M_PI; tt += 0.09)
            glVertex3f(mid.x + radius*cos(tt), mid.y + radius*sin(tt), 0);
    glEnd();
    return;
}

/* �e�@��Pacman
   �ѼơG
   - mid: ���߮y��
   - radius: �b�|
   - theta: �L�ڨ��פj�p(0~359)
   - orient: �L�ڴ¦V(0~359)
*/
void DrawPacman(Point mid, float radius, float theta, float orient, Color c)
{
    float ftheta = theta*M_PI/360.0;
    float tt_mid = orient*2*M_PI/360.0;
    glBegin(GL_POLYGON);
        glColor3f(c.r, c.g, c.b);
        glVertex3f(mid.x, mid.y, 0);
        for(float tt = ftheta; tt < 2*M_PI-ftheta;tt += 0.09)
        glVertex3f(mid.x + radius*cos(tt+tt_mid), mid.y + radius*sin(tt+tt_mid), 0);
    glEnd();
    return;
}

void DrawGhost(Point mid, float radius, float theta, float orient, Color c)
{
    float ftheta = theta*M_PI/360.0;
    float tt_mid = orient*2*M_PI/360.0;
    glBegin(GL_POLYGON);
        glColor3f(c.r, c.g, c.b);
        glVertex3f(mid.x, mid.y, 0);
        int times = 0;
        for(float tt = ftheta; tt < 2*M_PI-ftheta; tt += 0.09)
        {
        	times += 1 ;
        	if(times % 2 == 0)
        	glVertex3f(mid.x + radius*cos(tt+tt_mid), mid.y + radius*sin(tt+tt_mid), 0);
        	else
        	glVertex3f(mid.x + radius*0.8*cos(tt+tt_mid), mid.y + radius*0.8*sin(tt+tt_mid), 0);
		}
    glEnd();
    return;
}

int map[10][10] = 
{
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

const int DIR_UP = 0;
const int DIR_DOWN = 1;
const int DIR_LEFT = 2;
const int DIR_RIGHT = 3;

struct
{
    int step;
    int dir;
    int x, y;
    Color c;
}pacman;

struct
{
    int step;
    int dir;
    int x, y;
    Color c;
}ghost;

Color wall_Color = {0.5, 0.5, 0.5};

Point GetRealPoint(int x, int y) //��X���߮y�� �������߬�(0, 0) 
{
    Point ret;
    ret.x = -0.9+0.2*x;
    ret.y =  0.9-0.2*y;
    return ret; 
}

void SystemTimer(int _value)
{
    // �o��ƨC25ms�N�|�Q�I�s�@��
    // TODO

    int dx[] = {0, 0, -1, 1}, dy[] = {-1, 1, 0, 0};
    
    if(pacman.step == 0)
	{
        if(trigger_left_key) 
		pacman.dir = DIR_LEFT;    //DIR_LEFT = 2;
        if(trigger_right_key) 
		pacman.dir = DIR_RIGHT;   //DIR_RIGHT = 3;
        if(trigger_down_key) 
		pacman.dir = DIR_DOWN;    //DIR_DOWN = 1;
        if(trigger_up_key) 
		pacman.dir = DIR_UP;      //DIR_UP = 0;
    }
    
    if(ghost.step == 0)
	{
        if(trigger_A_key) 
		ghost.dir = DIR_LEFT;    //DIR_LEFT = 2;
        if(trigger_D_key) 
		ghost.dir = DIR_RIGHT;   //DIR_RIGHT = 3;
        if(trigger_S_key) 
		ghost.dir = DIR_DOWN;    //DIR_DOWN = 1;
        if(trigger_W_key) 
		ghost.dir = DIR_UP;      //DIR_UP = 0;
    }
    
    int next_x = pacman.x + dx[pacman.dir]; //dx[] = {0, 0, -1, 1} 
    int next_y = pacman.y + dy[pacman.dir]; //dy[] = {-1, 1, 0, 0} �W�U�O�Ϫ� 
    if(map[next_x][next_y] != 1) //�p�G���O��� 
        pacman.step++;
        
    int next_x2 = ghost.x + dx[ghost.dir]; //dx[] = {0, 0, -1, 1} 
    int next_y2 = ghost.y + dy[ghost.dir]; //dy[] = {-1, 1, 0, 0} �W�U�O�Ϫ� 
    if(map[next_x2][next_y2] != 1) //�p�G���O��� 
        ghost.step++;

    if(pacman.step == 4)
	{
        pacman.x += dx[pacman.dir];
        pacman.y += dy[pacman.dir];
        pacman.step = 0;
    }
    
    if(ghost.step == 4)
	{
        ghost.x += dx[ghost.dir];
        ghost.y += dy[ghost.dir];
        ghost.step = 0;
    }
    
    glutPostRedisplay();
    glutTimerFunc(25, SystemTimer, 1);
    return;
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    // ��e���ݭn�Q��s�ɡA�o��ƴN�|�Q�I�s
    // TODO
    for(int lx = 0; lx < 10; lx++)
        for(int ly = 0; ly < 10; ly++)
            if(map[lx][ly] == 1) //�p�G�o�Ӧ�m���ӬO��� 
                DrawRectangle(GetRealPoint(lx, ly), 0.18, 0.18, wall_Color);  //�e����� 
   
    float dt = pacman.step*0.25;
    float dt2 = ghost.step*0.25;
    Point pt = GetRealPoint(pacman.x, pacman.y);
    Point pt2 = GetRealPoint(ghost.x, ghost.y);
    
    if(pacman.dir == DIR_UP)  //DIR_UP = 0 
	pt.y += dt*0.2; 
    else if(pacman.dir == DIR_DOWN)  //DIR_DOWN = 1 
	pt.y -= dt*0.2; 
    else if(pacman.dir == DIR_LEFT) //DIR_LEFT = 2
	pt.x -= dt*0.2; 
    else if(pacman.dir == DIR_RIGHT)  //DIR_RIGHT = 3 
	pt.x += dt*0.2;
	
	if(ghost.dir == DIR_UP)  //DIR_UP = 0 
	pt2.y += dt2*0.2; 
    else if(ghost.dir == DIR_DOWN)  //DIR_DOWN = 1 
	pt2.y -= dt2*0.2; 
    else if(ghost.dir == DIR_LEFT) //DIR_LEFT = 2
	pt2.x -= dt2*0.2; 
    else if(ghost.dir == DIR_RIGHT)  //DIR_RIGHT = 3 
	pt2.x += dt2*0.2;
    
    DrawPacman(pt, 0.1,  15, 0, pacman.c); //���I �b�| �i�L���� �}�L��V
	DrawGhost(pt2, 0.1,  60, 180, ghost.c); //���I �b�| �i�L���� �}�L��V 
 
    glFlush();
    return;
}

void Init()
{
    // �o��Ʒ|�b�@�}�l�I�s
    // TODO 
    pacman.dir = DIR_DOWN; //�p���F�_�l���ʤ�V (���Ϋ�������N�|��) 
    pacman.step = 3;
    pacman.x = 3, pacman.y = 4; //�p���F�_�l��m �Y��b����]���|�d�� 
    pacman.c = {0.7, 0.3, 0}; //�p���F�C�� 
    
    ghost.dir = DIR_RIGHT; //�p���F�_�l���ʤ�V (���Ϋ�������N�|��) 
    ghost.step = 0;
    ghost.x = 4, ghost.y = 1; //�p���F�_�l��m �Y��b����]���|�d�� 
    ghost.c = {0.1, 0.5, 0}; //�����C�� 
    
    return;
}

void SpecialKeyDown(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) 
	trigger_left_key = true; 
    if(key == GLUT_KEY_RIGHT) 
	trigger_right_key = true; 
    if(key == GLUT_KEY_UP) 
	trigger_up_key = true; 
    if(key == GLUT_KEY_DOWN) 
	trigger_down_key = true;

    glutPostRedisplay();
    return;
}

void NormalKeyDown(unsigned char key, int x, int y)
{
	if(key == 'a') 
	trigger_A_key = true; 
    if(key == 'd') 
	trigger_D_key = true; 
    if(key == 'w') 
	trigger_W_key = true; 
    if(key == 's') 
	trigger_S_key = true; 
	
	glutPostRedisplay();
    return;
	
 } 

void SpecialKeyUp(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) 
	trigger_left_key = false; 
    if(key == GLUT_KEY_RIGHT) 
	trigger_right_key = false; 
    if(key == GLUT_KEY_UP) 
	trigger_up_key = false; 
    if(key == GLUT_KEY_DOWN) 
	trigger_down_key = false; 
	
    glutPostRedisplay();
    return;
}

void NormalKeyUp(unsigned char key, int x, int y)
{	
	if(key == 'a') 
	trigger_A_key = false; 
    if(key == 'd') 
	trigger_D_key = false; 
    if(key == 'w') 
	trigger_W_key = false; 
    if(key == 's') 
	trigger_S_key = false; 
	
    glutPostRedisplay();
    return;
}

int main(int argc, char* argv[])
{ 
    Init();
    
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Pacman");
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA); 
    glutDisplayFunc(Display);

    glutTimerFunc(25, SystemTimer, 1);

    glutSpecialFunc(SpecialKeyDown);
    glutSpecialUpFunc(SpecialKeyUp);
    glutKeyboardFunc(NormalKeyDown);
    glutKeyboardUpFunc (NormalKeyUp);

    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutMainLoop();
    
    return 0;
}

