#include <cstdio>
#include <cstdlib>
#define _USE_MATH_DEFINES 
#include <cmath>
//test //ya 
//test
//jida
//test 2.0
//jkjkj
//yumi
//user
//hallo
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

struct Color{float r, g, b;};
struct Point{float x, y;};

bool trigger_left_key = false;
bool trigger_up_key = false;
bool trigger_right_key = false;
bool trigger_down_key = false;

/* 礶よ
   把计
   - mid: よいみ畒夹
   - width: よ郌
   - height: よ蔼
   - c: よ恶肅︹
*/
void DrawRectangle(Point mid, float width, float height, Color c){
    glBegin(GL_QUADS);
        glColor3f(c.r, c.g, c.b);
        glVertex3f(mid.x - width/2, mid.y - height/2, 0.0);
        glVertex3f(mid.x + width/2, mid.y - height/2, 0.0);
        glVertex3f(mid.x + width/2, mid.y + height/2, 0.0);
        glVertex3f(mid.x - width/2, mid.y + height/2, 0.0);
    glEnd();
    return;
}

/* 礶蛾
   把计
   - mid: いみ畒夹
   - radius: 畖
   - c: 恶肅︹
*/

void DrawCircle(Point mid, float radius, Color c){
    glBegin(GL_POLYGON);
        glColor3f(c.r, c.g, c.b);
        for(float tt = 0; tt < 2*M_PI;tt += 0.09)
            glVertex3f(mid.x + radius*cos(tt), mid.y + radius*sin(tt), 0);
    glEnd();
    return;
}

/* 礶Pacman
   把计
   - mid: いみ畒夹
   - radius: 畖
   - theta: 糒ぺà(0~359)
   - orient: 糒ぺ绰(0~359)
*/
void DrawPacman(Point mid, float radius, float theta, float orient, Color c){
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

struct{
    int step;
    int dir;
    int x, y;
    Color c;
}pacman;

Color wall_Color = {0.5, 0.5, 0.5};


Point GetRealPoint(int x, int y){
    Point ret;
    ret.x = -0.9+0.2*x;
    ret.y =  0.9-0.2*y;
    return ret; 
}

void SystemTimer(int _value){
    // 硂ㄧ计–25ms碞穦砆㊣Ω
    // TODO

    int dx[] = {0, 0, -1, 1}, dy[] = {-1, 1, 0, 0};
    
    if(pacman.step == 0){
        if(trigger_left_key) pacman.dir = DIR_LEFT;
        if(trigger_right_key) pacman.dir = DIR_RIGHT;
        if(trigger_down_key) pacman.dir = DIR_DOWN;
        if(trigger_up_key) pacman.dir = DIR_UP;
    }
    
    int next_x = pacman.x + dx[pacman.dir];
    int next_y = pacman.y + dy[pacman.dir];
    if(map[next_x][next_y] != 1)
        pacman.step++;

    if(pacman.step == 4){
        pacman.x += dx[pacman.dir];
        pacman.y += dy[pacman.dir];
        pacman.step = 0;
    }
    
    glutPostRedisplay();
    glutTimerFunc(25, SystemTimer, 1);
    return;
}

void Display(){
    glClear(GL_COLOR_BUFFER_BIT);
    // 讽礶惠璶砆穝硂ㄧ计碞穦砆㊣
    // TODO
    for(int lx = 0;lx < 10;lx++)
        for(int ly = 0;ly < 10;ly++)
            if(map[lx][ly] == 1)
                DrawRectangle(GetRealPoint(lx, ly), 0.18, 0.18, wall_Color);
   
    float dt = pacman.step*0.25;
    Point pt = GetRealPoint(pacman.x, pacman.y);
    if(pacman.dir == DIR_UP) pt.y += dt*0.2; 
    else if(pacman.dir == DIR_DOWN) pt.y -= dt*0.2; 
    else if(pacman.dir == DIR_LEFT) pt.x -= dt*0.2; 
    else if(pacman.dir == DIR_RIGHT) pt.x += dt*0.2;
    
    DrawPacman(pt, 0.1,  45, 90, pacman.c);
 
    glFlush();
    return;
}

void Init(){
    // 硂ㄧ计穦秨﹍㊣
    // TODO 
    pacman.dir = DIR_RIGHT;
    pacman.step = 0;
    pacman.x = 1, pacman.y = 8;
    pacman.c = {0.7, 0.3, 0};
    return;
}

void SpecialKeyDown(int key, int x, int y){
    if(key == GLUT_KEY_LEFT) trigger_left_key = true; 
    if(key == GLUT_KEY_RIGHT) trigger_right_key = true; 
    if(key == GLUT_KEY_UP) trigger_up_key = true; 
    if(key == GLUT_KEY_DOWN) trigger_down_key = true; 
    glutPostRedisplay();
    return;
}

void SpecialKeyUp(int key, int x, int y){
    if(key == GLUT_KEY_LEFT) trigger_left_key = false; 
    if(key == GLUT_KEY_RIGHT) trigger_right_key = false; 
    if(key == GLUT_KEY_UP) trigger_up_key = false; 
    if(key == GLUT_KEY_DOWN) trigger_down_key = false; 
    glutPostRedisplay();
    return;
}

int main(int argc, char* argv[]){ 
    Init();
    
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Pacman");
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA); 
    glutDisplayFunc(Display);

    glutTimerFunc(25, SystemTimer, 1);

    glutSpecialFunc(SpecialKeyDown);
    glutSpecialUpFunc(SpecialKeyUp);

    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutMainLoop();
    
    return 0;
}

