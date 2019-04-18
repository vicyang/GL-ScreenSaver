// CodeRain
// Code By: 523066680
//    Date: 2016-11

#include <GL/glut.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "outline.h"

#define SIZE_X 500
#define SIZE_Y 500
#define N_RAIN 50
#define N_TEXT 50

int winID;
GLUtesselator   *tobj;

static double vtx[5000][3];
static int    vtx_contours[100];
static int    Vi;
static int    vtx_ctsi;
static float  red = 0.0;
static int    PARTS = 3.0;  //曲线细分程度
static long   code;

extern FT_Library      library;
extern FT_Face         face;

extern FT_GlyphSlot    slot;
extern FT_Error        error;
extern FT_Outline      outline;
extern FT_BBox         box;

float angx = 0.0;
float angy = 0.0;
float angz = 0.0;
float progress[N_RAIN];
float speed[N_RAIN];
float bright[N_RAIN];
float RXlist[N_RAIN];
float RYlist[N_RAIN];
float RZlist[N_RAIN];
char str[] = "          abcdefghijklmnopqrstuvwxyz,ABCDEFGHIJKLMNOPQRSTUVWXYZ~!@#$%^&*()_+-={}|[];':<>,./\\?";
//char uni[] = "          abcdefghijklmnopqrstuvwxyz,ABCDEFGHIJKLMNOPQRSTUVWXYZ~!@#$%^&*()_+-={}|[];':<>,./\\?";

long uni[] = {
    32, 47,    47,    32,    85,   112,   100,    97,   116,   101,    32,
    101,    97,    99,   104,    32,   111,   112,   116,   105,   111,
    110,    13,    10,   112,   111,   108,   108,    46,   102,   105,
    110,   100,    40,    39,    91,   100,    97,   116,    97,    45,
    112,   111,   108,   108,    45,   111,   112,   116,   105,   111,
    110,    45,   105,   100,    93,    39,    41,    46,   101,    97,
     99,   104,    40,   102,   117,   110,    99,   116,   105,   111,
    110,    40,    41,    32,   123,    13,    10,     9,   118,    97,
    114,    32,    36,   116,   104,   105,   115,    32,    61,    32,
     36,    40,   116,   104,   105,   115,    41,    59,    13,    10,
      9,   118,    97,   114,    32,   111,   112,   116,   105,   111,
    110,    73,   100,    32,    61,    32,    36,   116,   104,   105,
    115,    46,    97,   116,   116,   114,    40,    39,   100,    97,
    116,    97,    45,   112,   111,   108,   108,    45,   111,   112,
    116,   105,   111,   110,    45,   105,   100,    39,    41,    59,
     13,    10,     9,   118,    97,   114,    32,   118,   111,   116,
    101,   100,    32,    61,    32,    40,   116,   121,   112,   101,
    111,   102,    32,   114,   101,   115,    46,   117,   115,   101,
    114,    95,   118,   111,   116,   101,   115,    91,   111,   112,
    116,   105,   111,   110,    73,   100,    93,    32,    33,    61,
     61,    32,    39,   117,   110,   100,   101,   102,   105,   110,
    101,   100,    39,    41,    59,    13,    10,     9,   118,    97,
    114,    32,   109,   111,   115,   116,    86,   111,   116,   101,
    100,    32,    61,    32,    40,   114,   101,   115,    46,   118,
    111,   116,   101,    95,    99,   111,   117,   110,   116,   115,
     91,   111,   112,   116,   105,   111,   110,    73,   100,    93,
     32,    61,    61,    61,    32,   109,   111,   115,   116,    86,
    111,   116,   101,   115,    41,    59,    13,    10,     9,   118,
     97,   114,    32,   112,   101,   114,    99,   101,   110,   116,
     32,    61,    32,    40,    33,   114,   101,   115,    46,   116,
    111,   116,    97,   108,    95,   118,   111,   116,   101,   115,
     41,    32,    63,    32,    48,    32,    58,    32,    77,    97,
    116,   104,    46,   114,   111,   117,   110,   100,    40,    40,
    114,   101,   115,    46,   118,   111,   116,   101,    95,    99,
    111,   117,   110,   116,   115,    91,   111,   112,   116,   105,
    111,   110,    73,   100,    93,    32,    47,    32,   114,   101,
    115,    46,   116,   111,   116,    97,   108,    95,   118,   111,
    116,   101,   115,    41,    32,    42,    32,    49,    48,    48,
     41,    59,    13,    10,     9,   118,    97,   114,    32,   112,
    101,   114,    99,   101,   110,   116,    82,   101,   108,    32,
     61,    32,    40,   109,   111,   115,   116,    86,   111,   116,
    101,   115,    32,    61,    61,    61,    32,    48,    41,    32,
     63,    32,    48,    32,    58,    32,    77,    97,   116,   104,
     46,   114,   111,   117,   110,   100,    40,    40,   114,   101,
    115,    46,   118,   111,   116,   101,    95,    99,   111,   117,
    110,   116,   115,    91,   111,   112,   116,   105,   111,   110,
     73,   100,    93,    32,    47,    32,   109,   111,   115,   116,
     86,   111,   116,   101,   115,    41,    32,    42,    32,    49,
     48,    48,    41,    59,    13,    10,     9,   118,    97,   114,
     32,    97,   108,   116,    84,   101,   120,   116,    59,    13,
     10,    13,    10,     9,    97,   108,   116,    84,   101,   120,
    116,    32,    61,    32,    36,   116,   104,   105,   115,    46,
     97,   116,   116,   114,    40,    39,   100,    97,   116,    97,
     45,    97,   108,   116,    45,   116,   101,   120,   116,    39,
     41,    59,    13,    10,     9,   105,   102,    32,    40,   118,
    111,   116,   101,   100,    41,    32,   123,    13,    10,     9,
      9,    36,   116,   104,   105,   115,    46,    97,   116,   116,
    114,    40,    39,   116,   105,   116,   108,   101,    39,    44,
     32,    36,    46,   116,   114,   105,   109,    40,    97,   108,
    116,    84,   101,   120,   116,    41,    41,    59,    13,    10,
      9,   125,    32,   101,   108,   115,   101,    32,   123,    13,
     10,     9,     9,    36,   116,   104,   105,   115,    46,    97,
    116,   116,   114,    40,    39,   116,   105,   116,   108,   101,
     39,    44,    32,    39,    39,    41,    59,    13,    10,     9,
    125,    59,    13,    10,     9,    36,   116,   104,   105,   115,
     46,   116,   111,   103,   103,   108,   101,    67,   108,    97,
    115,   115,    40,    39,   118,   111,   116,   101,   100,    39,
     44,    32,   118,   111,   116,   101,   100,    41,    59,    13,
     10,     9,    36,   116,   104,   105,   115,    46,   116,   111,
    103,   103,   108,   101,    67,   108,    97,   115,   115,    40,
     39,   109,   111,   115,   116,    45,   118,   111,   116,   101,
    115,    39,    44,    32,   109,   111,   115,   116,    86,   111,
    116,   101,   100,    41,    59,    13,    10,    13,    10,     9,
     47,    47,    32,    85,   112,   100,    97,   116,   101,    32,
    116,   104,   101,    32,    98,    97,   114,   115,    13,    10,
      9,   118,    97,   114,    32,    98,    97,   114,    32,    61,
     32,    36,   116,   104,   105,   115,    46,   102,   105,   110,
    100,    40,    39,    46,   114,   101,   115,   117,   108,   116,
     98,    97,   114,    32,   100,   105,   118,    39,    41,    59,
     13,    10,     9,   118,    97,   114,    32,    98,    97,   114,
     84,   105,   109,   101,    76,    97,   112,   115,   101,    32,
     61,    32,    40,   114,   101,   115,    46,    99,    97,   110,
     95,   118,   111,   116,   101,    41,    32,    63,    32,    53,
     48,    48,    32,    58,    32,    49,    53,    48,    48,    59,
     13,    10,     9,   118,    97,   114,    32,   110,   101,   119,
     66,    97,   114,    67,   108,    97,   115,   115,    32,    61,
     32,    40,   112,   101,   114,    99,   101,   110,   116,    32,
     61,    61,    61,    32,    49,    48,    48,    41,    32,    63,
     32,    39,   112,   111,   108,   108,    98,    97,   114,    53,
     39,    32,    58,    32,    39,   112,   111,   108,   108,    98,
     97,   114,    39,    32,    43,    32,    40,    77,    97,   116,
    104,    46,   102,   108,   111,   111,   114,    40,   112,   101,
    114,    99,   101,   110,   116,    32,    47,    32,    50,    48,
     41,    32,    43,    32,    49,    41,    59,    13,    10,    13,
     10,     9,   115,   101,   116,    84,   105,   109,   101,   111,
    117,   116,    40,   102,   117,   110,    99,   116,   105,   111,
    110,    32,    40,    41,    32,   123,    13,    10,     9,     9,
     98,    97,   114,    46,    97,   110,   105,   109,    97,   116,
    101,    40,   123,    32,   119,   105,   100,   116,   104,    58,
     32,   112,   101,   114,    99,   101,   110,   116,    82,   101,
    108,    32,    43,    32,    39,    37,    39,    32,   125,    44,
     32,    53,    48,    48,    41,    13,    10,     9,     9,     9,
     46,   114,   101,   109,   111,   118,   101,    67,   108,    97,
    115,   115,    40,    39,   112,   111,   108,   108,    98,    97,
    114,    49,    32,   112,   111,   108,   108,    98,    97,   114,
     50,    32,   112,   111,   108,   108,    98,    97,   114,    51,
     32,   112,   111,   108,   108,    98,    97,   114,    52,    32,
    112,   111,   108,   108,    98,    97,   114,    53,    39,    41,
     13,    10,     9,     9,     9,    46,    97,   100,   100,    67,
    108,    97,   115,   115,    40,   110,   101,   119,    66,    97,
    114,    67,   108,    97,   115,   115,    41,    13,    10,     9,
      9,     9,    46,   104,   116,   109,   108,    40,   114,   101,
    115,    46,   118,   111,   116,   101,    95,    99,   111,   117,
    110,   116,   115,    91,   111,   112,   116,   105,   111,   110,
     73,   100,    93,    41,    59,    13,    10,    13,    10,     9,
      9,   118,    97,   114,    32,   112,   101,   114,    99,   101,
    110,   116,    84,   101,   120,   116,    32,    61,    32,   112,
    101,   114,    99,   101,   110,   116,    32,    63,    32,   112,
    101,   114,    99,   101,   110,   116,    32,    43,    32,    39,
     37,    39,    32,    58,    32,   114,   101,   115,    46,    78,
     79,    95,    86,    79,    84,    69,    83,    59,    13,    10,
      9,     9,    36,   116,   104,   105,   115,    46,   102,   105,
    110,   100,    40,    39,    46,   112,   111,   108,   108,    95,
    111,   112,   116,   105,   111,   110,    95,   112,   101,   114,
     99,   101,   110,   116,    39,    41,    46,   104,   116,   109,
    108,    40,   112,   101,   114,    99,   101,   110,   116,    84,
    101,   120,   116,    41,    59,    13,    10,     9,   125,    44,
     32,    98,    97,   114,    84,   105,   109,   101,    76,    97,
    112,   115,   101,    41,    59,    13,    10,   125,    41,    59,
     13,    10,    13,    10,   105,   102,    32,    40,    33,   114,
    101,   115,    46,    99,    97,   110,    95,   118,   111,   116,
    101,    41,    32,   123,    13,    10,     9,   112,   111,   108,
    108,    46,   102,   105,   110,   100,    40,    39,    46,   112,
    111,   108,   108,   115,    39,    41,    46,   100,   101,   108,
     97,   121,    40,    52,    48,    48,    41,    46,   102,    97,
    100,   101,    73,   110,    40,    53,    48,    48,    41,    59,
     13,    10,   125,    13,    10,    13,    10,    47,    47,    32,
     68,   105,   115,   112,   108,    97,   121,    32,    34,    89,
    111,   117,   114,    32,   118,   111,   116,   101,    32,   104,
     97,   115,    32,    98,   101,   101,   110,    32,    99,    97,
    115,   116,    46,    34,    32,   109,   101,   115,   115,    97,
    103,   101,    46,    32,    68,   105,   115,    97,   112,   112,
    101,    97,   114,   115,    32,    97,   102,   116,   101,   114,
     32,    53,    32,   115,   101,    99,   111,   110,   100,   115,
     46,    13,    10,   118,    97,   114,    32,    99,   111,   110,
    102,   105,   114,   109,    97,   116,   105,   111,   110,    68,
    101,   108,    97,   121,    32,    61,    32,    40,   114,   101,
    115,    46,    99,    97,   110,    95,   118,   111,   116,   101,
     41,    32,    63,    32,    51,    48,    48,    32,    58,    32,
     57,    48,    48,    59,    13,    10,   112,   111,   108,   108,
     46,   102,   105,   110,   100,    40,    39,    46,   118,   111,
    116,   101,    45,   115,   117,    98,   109,   105,   116,   116,
    101,   100,    39,    41,    46,   100,   101,   108,    97,   121,
     40,    99,   111,   110,   102,   105,   114,   109,    97,   116,
    105,   111,   110,    68,   101,   108,    97,   121,    41,    46,
    115,   108,   105,   100,   101,    68,   111,   119,   110,    40,
     50,    48,    48,    44,    32,   102,   117,   110,    99,   116,
    105,   111,   110,    40,    41,    32,   123,    13,    10,     9,
    105,   102,    32,    40,   114,   101,   115,   117,   108,   116,
    115,    86,   105,   115,   105,    98,   108,   101,    41,    32,
    123,    13,    10,     9,     9,   117,   112,   100,    97,   116,
    101,    80,    97,   110,   101,   108,    72,   101,   105,   103,
    104,   116,    40,    41,    59,    13,    10,     9,   125,    13,
     10,    13,    10,     9,    36,    40,   116,   104,   105,   115,
     41,    46,   100,   101,   108,    97,   121,    40,    53,    48,
     48,    48,    41,    46,   102,    97,   100,   101,    79,   117,
    116,    40,    53,    48,    48,    44,    32,   102,   117,   110,
     99,   116,   105,   111,   110,    40,    41,    32,   123,    13,
     10,     9,     9,   114,   101,   115,   105,   122,   101,    80,
     97,   110,   101,   108,    40,    51,    48,    48,    41,    59,
     13,    10,     9,   125,    41,    59,    13,    10,   125,    41,
     59,    13,    10
};
static int n_char;
static int arr[N_RAIN][N_TEXT];

void beginCallback(GLenum which)
{
    glBegin(which);
}

void endCallback(void)
{
    glEnd();
}

void errorCallback(GLenum errorCode)
{
    const GLubyte *estring;

    estring = gluErrorString(errorCode);
    fprintf(stderr, "Tessellation Error: %s\n", estring);
    exit(0);
}

void vertexCallback(GLdouble *vertex)
{
    //const GLdouble *pointer;
    //pointer = (GLdouble *) vertex;
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3dv(vertex);
}

void CALLBACK combineCallback(GLdouble coords[3], GLdouble *data[4],
                     GLfloat weight[4], GLdouble **dataOut )
{
   GLdouble *vertex;
   vertex = (GLdouble *) malloc(3 * sizeof(GLdouble));

   vertex[0] = coords[0];
   vertex[1] = coords[1];
   vertex[2] = coords[2];
   *dataOut = vertex;
}



void display(void)
{
    int errcode;
    int bgn;
    int i, j;
    int start;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 

    glPushMatrix();
    glRotatef(angy, 0.0, 0.5, 0.0);
    glRotatef(angx, 1.0, 0.0, 0.0);
    glRotatef(angz, 0.0, 0.0, 1.0);

    glColor3f(0.3, 0.8, 0.3);

    for (int m = 0; m < N_RAIN; m++)
    {
        glPushMatrix();
        
        //glTranslatef(-12000.0 + RXlist[m], 5000.0 + RYlist[m],  RZlist[m]);
        glTranslatef(RXlist[m], RYlist[m]+20000.0, RZlist[m]);
        glRotatef( (float)m/(float)N_RAIN * 10.0, 0.0, 1.0, 0.0 );
        for (int n = 0; n < N_TEXT ; n++ )
        {
            //向下移位
            glTranslatef(0.0, -1200.0, 0.0);

            glPushMatrix();
            glRotatef(-90.0, 0.0, 0.0, 1.0);
            if ( n == bright[m] )
            {
                glColor3f((float)n/(float)N_TEXT, (float)m/(float)N_RAIN+0.2, 0.5);
                glScalef(1.2, 1.2, 1.2);
            }
            else
            {
                glColor3f((float)n/(float)N_TEXT/1.5, (float)m/(float)N_RAIN, 0.3);
            }
            glCallList( arr[m][n] );
            glPopMatrix();
        }
        
        glPopMatrix();
    }
    glPopMatrix();
    
    glutSwapBuffers();
}

void idle(void)
{
    usleep(30000);
    angx += 0.1;
    if (angz < 90.0) 
    {
        angz += 1.0;
    }
    else
    {
        if (angy < 50.0)
            angy += 0.2;
    }

    int r;
    for (int i = 0; i < N_RAIN; i++ )
    {
        if ( progress[i] < n_char  )
        {
            progress[i] += speed[i];
            arr[i][ (int)progress[i] % N_TEXT ] = (int)progress[i];
        }
        else
        {
            progress[i] = 1.0;
            RYlist[i] = (float)(rand() % 20 - 10) * 1000.0;
        }
        bright[i] = (int)progress[i] % N_TEXT;
    }


    glutPostRedisplay();
}

void reshape(int Width, int Height)
{
    const float fa = 20000.0;
    const float half = 8000.0;

    float w = (float)Width;
    float h = (float)Height;

    glViewport(0, 0, Width, Height);     //视口范围
    glMatrixMode(GL_PROJECTION);              // 投影视图矩阵
    glLoadIdentity();
    //glOrtho(-half, half, -half, half, 0.0, 40000.0);
    gluPerspective(100.0, w/h, 10.0, 80000.0);
    glMatrixMode(GL_MODELVIEW);               // 模型视图矩阵
    glLoadIdentity();
    gluLookAt(0.0,0.0,fa, 0.0,0.0,0.0, 0.0,1.0,fa);  //设置观察点
            // 观察点，   朝向的坐标， 观察点向上坐标
}

void keypress(unsigned char key, int mousex, int mousey)
{
    int errcode;
    switch (key)
    {
        case 'q':
        case 'Q':
            glutDestroyWindow(winID);
            exit(0);
            break;            
        case 'w':
        case 'W':
            angx += 1.0;
            glutPostRedisplay();
            break;
        case 's':
        case 'S':
            angx -= 1.0;
            glutPostRedisplay();
            break;
        case 'a':
        case 'A':
            angz += 1.0;
            glutPostRedisplay();
            break;
        case 'd':
        case 'D':
            angz -= 1.0;
            glutPostRedisplay();
            break;
        case 'j':
        case 'J':
            angy += 2.0;
            glutPostRedisplay();
            break;
        case 'k':
        case 'K':
            angy -= 2.0;
            glutPostRedisplay();
            break;
    }
}

void init(void)
{
    int errcode;
    Vi = 0;
    vtx_ctsi = 0;
    int bgn;
    int next;
    int base;  //for Display List

    //glutFullScreen();
    srand( time(NULL) );
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glLineWidth( 1.0 );
    glPointSize( 2.0 );
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_MAP1_VERTEX_3);
    glEnable(GL_LINE_SMOOTH);

    // glShadeModel(GL_SMOOTH);

    tobj = gluNewTess();
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    gluTessCallback(tobj, GLU_TESS_VERTEX, glVertex3dv);
    gluTessCallback(tobj, GLU_TESS_BEGIN,  beginCallback);
    gluTessCallback(tobj, GLU_TESS_END,    endCallback);
    gluTessCallback(tobj, GLU_TESS_ERROR,  errorCallback);
    gluTessCallback(tobj, GLU_TESS_COMBINE,combineCallback);
    //gluTessProperty(tobj, GLU_TESS_BOUNDARY_ONLY, GL_TRUE);
    gluTessProperty(tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);
    
    for (int i = 0; i < N_RAIN; i++)
    {
        //制造参差效果
        RXlist[i] = (float)(rand() % 20 - 10) * 3000.0;
        RYlist[i] = (float)(rand() % 20 - 10) * 2000.0;
        RZlist[i] = (float)(rand() % 20 - 10) * 2000.0;

        //随机进度、递进速度
        progress[i] = (float) ((rand()%50 + 1) * 1);
        speed[i]    = (float) ( rand()%5 + 1) / 8.0;

        //填入空白；显示列表的索引，从1开始
        for (int j = 0; j < N_TEXT; j++)
        {
            arr[i][j] = 1;
        }
    }

    // 创建显示列表
    n_char = sizeof(uni)/sizeof(uni[0]);
    base = glGenLists( n_char );
    
    //每个字符的点数据（已经对曲线求值了的）整理到 glyph 数组
    for (int code = 0; code < n_char; code++ )
    {
        errcode = LoadGlyph( uni[code] );
        //处理坐标、曲线数据到vtx数组
        GetDatafromOutline();

        glNewList(base+code, GL_COMPILE);
        gluTessBeginPolygon(tobj, NULL);
        for (int cti = 0; cti < vtx_ctsi ; cti++ )
        {
            gluTessBeginContour(tobj);
            int pti = (cti == 0 ? 0 : vtx_contours[cti-1] + 1 );
            for (; pti <= vtx_contours[cti]; pti++ )
            {
                gluTessVertex(tobj, vtx[pti], vtx[pti] );
            }
            gluTessEndContour(tobj);
        }
        gluTessEndPolygon(tobj);

        //侧轮廓加厚
        for (int cti = 0; cti < vtx_ctsi; cti++ )
        {
            glBegin(GL_QUAD_STRIP);
            int pti = (cti == 0 ? 0 : vtx_contours[cti-1] + 1 );
            for (; pti <= vtx_contours[cti]; pti++ )
            {
                glVertex3f( vtx[pti][0], vtx[pti][1], 0.0 );
                glVertex3f( vtx[pti][0], vtx[pti][1], -20.0 );
            }
            glEnd();
        }

        glEndList();
    }

}

int main( int argc, char** argv )
{
    ft_init();
    LoadGlyph(code);
    GetDatafromOutline();
    
    for (int i=0; i<outline.n_contours; i++  )
    {
        printf("Contour %d: %d\n", i, outline.contours[i]);
    }
    printf("\n");

    glutInit(&argc, argv);
                    //显示模式     双缓冲          RGBA
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA |GLUT_DEPTH | GLUT_MULTISAMPLE );
    glutInitWindowSize(SIZE_X, SIZE_Y);
    glutInitWindowPosition(200, 200);
    winID = glutCreateWindow("Fonts");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keypress);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMainLoop();

    FT_Done_Face( face );
    FT_Done_FreeType( library );

    return 0;
}
