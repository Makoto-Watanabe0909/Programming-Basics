#include <iostream>
#include <cmath>
#ifdef WIN32
#include <windows.h>
#include <gl/GL.h>
#include "GLUT.h"
#else
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#endif

using namespace std;

double getTime();      // 開始時からの経過時間を秒単位で取得する関数
double mousePosX = 0;  // マウスカーソル位置X
double mousePosY = 0;  // マウスカーソル位置Y
unsigned char key = 0; // キーボードからの入力値
void predisplay();     // 描画開始時に必ず呼ぶ関数
void postdisplay();    // 描画終了時に必ず呼ぶ関数

////////////////////////////////////////////////////////////////////////////////

void display()
{
    predisplay(); // 描画開始時に必ず呼ぶ関数。消してはならない
    /***** 以降を編集する *****/

    /////
    ///// 2Dパート開始
    glDisable(GL_LIGHTING);  // 消してはいけない
    float WHEN=1;//全てを統べる変数(1~24で)
    
    if(WHEN<1||WHEN>24){
        exit(0);
    }
    
    float NIGHT[3]={0.2,0.2,0.2};
    float TWIGHT[3]={1,0.7,0.6};
    float BEFORENOON[3]={0.8,0.9,1};
    float AFTERNOON[3]={0.6,0.7,1};
    
    
    //「プログラムで図形描く講義に大体登場するあれ」で囲いを作る。
    //WHENの値で色と密度が変わる。
    int i;
    glLineWidth(2);
    for(i=-100;i<200;i=i+WHEN){
        //WHENによって色が変わる
        if(WHEN>=4&&WHEN<=6){glColor3d(TWIGHT[0],TWIGHT[1],TWIGHT[2]);}
        else if(WHEN>=7&&WHEN<=10){glColor3d(BEFORENOON[0],BEFORENOON[1],BEFORENOON[2]);}
        else if(WHEN>=11&&WHEN<=15){glColor3d(AFTERNOON[0],AFTERNOON[1],AFTERNOON[2]);}
        else if(WHEN>=16&&WHEN<=18){glColor3d(TWIGHT[0],TWIGHT[1]-0.2,TWIGHT[2]-0.2);}
        else {glColor3d(NIGHT[0],NIGHT[1],NIGHT[2]);}
        
        glBegin(GL_LINES);
        glVertex3d(-100+i,100,0);
        glVertex3d(100,100-i,0);
        glVertex3d(-100,100-i,0);
        glVertex3d(-100+i,-100,0);
        glVertex3d(-100,100-i,0);
        glVertex3d(100-i,100,0);
        glVertex3d(-100+i,-100,0);
        glVertex3d(100,-100+i,0);
        glEnd();
    }

    //長針
    glBegin(GL_LINES);
    glColor3d(1.0,1.0,1.0);
    glVertex3d(0,0,0);
    glVertex3d(0,50,0);
    glEnd();
    
    //短針 WHENの値で角度が変わる
    float CLOCK=WHEN*15*2*3.14/180;
    glBegin(GL_LINES);
    glColor3d(1.0,1.0,1.0);
    glVertex3d(0,0,0);
    glVertex3d(sin(CLOCK)*30,cos(CLOCK)*30,0);
    glEnd();
    
    /***** ここまで編集する *****/
    postdisplay(); // 描画終了時に必ず呼ぶ関数。消してはならない
}

////////////////////////////////////////////////////////////////////////////////

#pragma region not have to edit
void predisplay()
{
    // 背景色の設定
    glClearColor(0.0, 0.0, 0.0, 1.0); // 背景色 [Red, Green, Blue, Alpha] Alphaは1.0固定
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 100.0,
              0, 0, 0,
              0, 1.0, 0.0);
}

void postdisplay()
{
    glutSwapBuffers();
    key = 0;
}

void keyboard(unsigned char k, int x, int y)
{
    key = k;
}

static double startTime;
static double prevTime;
double getTime()
{
#ifdef WIN32
    LARGE_INTEGER freq, t;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t);
    return (double)t.QuadPart / freq.QuadPart - startTime;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1.0e9 - startTime;
#endif
}

void idle()
{
    double time = getTime();
    if (time - prevTime > 1.0 / 60.0)
    {
        display();
        prevTime = time;
    }
}

int screenWidth = 1;
int screenHeight = 1;
void resize(int w, int h)
{
    screenWidth = w;
    screenHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100.0, 100.0, -100.0, 100.0, 0.1, 200.0);
}

void mouseMotion(int x, int y)
{
    mousePosX =  (x - screenWidth  / 2) * 200.0 / screenWidth;
    mousePosY = -(y - screenHeight / 2) * 200.0 / screenHeight;
    glutIdleFunc(idle);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 800); // ウィンドウサイズ [幅, 高さ]
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("graphics"); // ウィンドウ名
    
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouseMotion);
    glutReshapeFunc(resize);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LIGHT0);
    float gray[4]  = { 0.5, 0.5, 0.5, 1.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gray);
    float black[4] = { 0.1, 0.1, 0.1, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, black);

    // ライトの位置 [x, y, z, 1]
    float lightPos[4] = { 0, 80.0, 100.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    // ライトの色 [Red, Green, Blue, 1]
    float lightColor[4] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightColor);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);

#ifdef WIN32
    LARGE_INTEGER freq, t;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t);
    startTime = (double)t.QuadPart / freq.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    startTime = ts.tv_sec + ts.tv_nsec / 1.0e9;
#endif
    prevTime = getTime();
    glutMainLoop();
    return 0;
}
#pragma endregion
