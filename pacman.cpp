#include <cstdio>

#include <cstdlib>

#include <cmath>



#ifdef __APPLE__

//openGL，全名是Open Graphics Library，是一個開放標準(open standard)的API，可以用來繪製2D與3D圖形。

//優點：1.跨平台。2.支援多種語言。3.硬體加速。

//缺點；1.只有繪圖的功能，所有其他東西都需要外部的輔助2.有許多進階用途需要對硬體有一定程度的了解

//在gl3.0之後許多舊版的函式如glBegin(),glPerspective(),glVertex3f()等都已經不被包含在核心(core)版本裡

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







struct Color{float r, g, b;};

struct Point{float x, y;};



bool trigger_left_key = false;

bool trigger_up_key = false;

bool trigger_right_key = false;

bool trigger_down_key = false;



bool trigger_A_key = false;

bool trigger_W_key = false;

bool trigger_D_key = false;

bool trigger_S_key = false;



/* 畫一個長方形

 參數：

 - mid: 長方形中心座標

 - width: 長方形誆度

 - height: 長方形高度

 - c: 長方形填充顏色

 */

void DrawRectangle(Point mid, float width, float height, Color c)

{

    

    glBegin(GL_QUADS);

    glColor3f(c.r, c.g, c.b);

    glVertex3f(mid.x - width/2, mid.y - height/2, 0.0);

    glVertex3f(mid.x + width/2, mid.y - height/2, 0.0);

    glVertex3f(mid.x + width/2, mid.y + height/2, 0.0);

    glVertex3f(mid.x - width/2, mid.y + height/2, 0.0);

    glEnd();

    return;

}



/* 畫一個圓形(哪裡有這個圓？？)哈囉??

 參數：

 - mid: 中心座標

 - radius: 半徑

 - c: 填充顏色

 */



void DrawCircle(Point mid, float radius, Color c)

{

    glBegin(GL_POLYGON);//POLYGON多邊形

    glColor3f(c.r, c.g, c.b);

    for(float tt = 0; tt < 2*M_PI;tt += 0.09)//從0畫到360一圈

        glVertex3f(mid.x + radius*cos(tt), mid.y + radius*sin(tt), 0);

    glEnd();

    return;

}



/*

 int n=100;

 for(int i=0;i<n;i++)

 {

 glVertex2f(R*cos(2*PI/n*i),R*sin(2*PI/n*i));

 }

 */



/* 畫一個Pacman

 參數：

 - mid: 中心座標

 - radius: 半徑

 - theta: 嘴巴角度大小(0~359)

 - orient: 嘴巴朝向(0~359)

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



int map[34][18] = {

    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},

    {1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1},

    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1},

    {1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1},

    {1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1},

    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},

    {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1},

    {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1},

    {1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},

    {1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1},

    {1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},

    {1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1},

    {1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},

    {1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1},

    {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},

    {1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1},

    {1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1},

    {1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1},

    {1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1},

    {1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1},

    {1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1},

    {1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1},

    {1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1},

    {1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1},

    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1},

    {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1},

    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1},

    {1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1},

    {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1},

    {1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},

    {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1},

    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},

    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}

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



struct

{

    int x , y;

    Color c;

}poop;



Color wall_Color = {0.7, 0.7, 0.7};

Color poop_Color = {0.54, 0.27, 0.07};



Point GetRealPoint(int x, int y)

{

    Point ret;

    ret.y =  -0.9+0.1*y;

    ret.x =  -0.9+0.055*x;

    return ret;

}



void SystemTimer(int _value){

    // 這函數每25ms就會被呼叫一次

    // TODO

    

    int dx[] = {0, 0, -1, 1}, dy[] = {1, -1, 0, 0};

    

    if(pacman.step == 0)

    {

        if(trigger_left_key)

            pacman.dir = DIR_LEFT;

        if(trigger_right_key)

            pacman.dir = DIR_RIGHT;

        if(trigger_down_key)

            pacman.dir = DIR_DOWN;

        if(trigger_up_key)

            pacman.dir = DIR_UP;

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

    

    int next_x = pacman.x + dx[pacman.dir];

    int next_y = pacman.y + dy[pacman.dir];

    if(map[next_x][next_y] != 1)

        pacman.step++;

    

    int next_x2 = ghost.x + dx[ghost.dir];

    int next_y2 = ghost.y + dy[ghost.dir];

    if(map[next_x2][next_y2] != 1)

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



void Display(){

    glClear(GL_COLOR_BUFFER_BIT);

    // 當畫面需要被更新時，這函數就會被呼叫

    // TODO

    

    for(int lx = 0;lx < 34;lx++)

        for(int ly = 0;ly < 18;ly++)

            if(map[lx][ly] == 1)

                DrawRectangle(GetRealPoint(lx, ly), 0.055, 0.1, wall_Color);

    DrawRectangle(GetRealPoint(2, 9),  0.055, 0.1, { 1, 0, 0});//red

    DrawRectangle(GetRealPoint(2, 10), 0.055, 0.1, { 1, 0, 0});

    DrawRectangle(GetRealPoint(3, 9),  0.055, 0.1, { 1, 0, 0});

    DrawRectangle(GetRealPoint(3, 10), 0.055, 0.1, { 1, 0, 0});

    DrawRectangle(GetRealPoint(4, 9),  0.055, 0.1, { 0, 1, 0.2});//green

    DrawRectangle(GetRealPoint(4, 10), 0.055, 0.1, { 0, 1, 0.2});

    DrawRectangle(GetRealPoint(5, 9),  0.055, 0.1, { 0, 1, 0.2});

    DrawRectangle(GetRealPoint(5, 10), 0.055, 0.1, { 0, 1, 0.2});

    DrawRectangle(GetRealPoint(4, 7),  0.055, 0.1, { 1, 0.8, 0});//yellow

    DrawRectangle(GetRealPoint(4, 8),  0.055, 0.1, { 1, 0.8, 0});

    DrawRectangle(GetRealPoint(5, 7),  0.055, 0.1, { 1, 0.8, 0});

    DrawRectangle(GetRealPoint(5, 8),  0.055, 0.1, { 1, 0.8, 0});

    DrawRectangle(GetRealPoint(2, 7),  0.055, 0.1, { 0.2, 0.2, 1});//blue

    DrawRectangle(GetRealPoint(2, 8),  0.055, 0.1, { 0.2, 0.2, 1});

    DrawRectangle(GetRealPoint(3, 7),  0.055, 0.1, { 0.2, 0.2, 1});

    DrawRectangle(GetRealPoint(3, 8),  0.055, 0.1, { 0.2, 0.2, 1});

    for(int x = 7;x <= 32;x++)//clean的顏色

        for(int i=6;i<=11;i++)

            if(map[x][i] == 1)

                DrawRectangle(GetRealPoint(x, i), 0.055, 0.1, { 1, 1, 1});

    

    //畫大便

    for(int lx = 0;lx < 34;lx++)

        for(int ly = 0;ly < 18;ly++)

            if(map[lx][ly] == 0)

                DrawCircle(GetRealPoint(lx, ly), 0.02, poop_Color);

    

    float dt = pacman.step*0.25;

    float dt2 = ghost.step*0.25;

    Point pt = GetRealPoint(pacman.x, pacman.y);

    Point pt2 = GetRealPoint(ghost.x, ghost.y);

    

    if(pacman.dir == DIR_UP)

        pt.y += dt*0.05;

    else if(pacman.dir == DIR_DOWN)

        pt.y -= dt*0.05;

    else if(pacman.dir == DIR_LEFT)

        pt.x -= dt*0.05;

    else if(pacman.dir == DIR_RIGHT)

        pt.x += dt*0.05;

    

    if(ghost.dir == DIR_UP)

        pt2.y += dt2*0.05;

    else if(ghost.dir == DIR_DOWN)

        pt2.y -= dt2*0.05;

    else if(ghost.dir == DIR_LEFT)

        pt2.x -= dt2*0.05;

    else if(ghost.dir == DIR_RIGHT)

        pt2.x += dt2*0.05;

    

    DrawPacman(pt, 0.03, 40, 50, pacman.c);

    DrawGhost(pt2, 0.03,  60, 180, ghost.c);

    

    map[pacman.x][pacman.y] = 2;

    map[ghost.x][ghost.y] = 2;

    

    glFlush();

    return;

}



void Init()

{

    // 這函數會在一開始呼叫

    // TODO

    pacman.dir = DIR_UP;

    pacman.step = 0;

    pacman.x = 1, pacman.y = 1;

    pacman.c = { 0.94, 0.64, 0.80};

    

    ghost.dir = DIR_RIGHT;

    ghost.step = 0;

    ghost.x = 4, ghost.y = 1;

    ghost.c = {0.06, 0.93, 0.93};

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

    glutInitWindowSize(1511, 800);

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
