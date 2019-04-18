// CodeRain
// Code By: 523066680
//    Date: 2016-11

#include <GL/glut.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "tools.h"

#define SIZE_X 500
#define SIZE_Y 500
#define N_RAIN 50
#define N_TEXT 50

int winID;
GLUtesselator   *tobj;

double vtx[5000][3];
int    vtx_contours[100];
int    Vi;
int    vtx_ctsi;
float  red = 0.0;
int    PARTS = 3.0;  //曲线细分程度
long   code;

FT_Library      library;
FT_Face         face;

FT_GlyphSlot    slot;
FT_Error        error;
FT_Outline      outline;
FT_BBox         box;

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

char *uni;
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
    base = glGenLists( n_char );
    printf("nchar: %d\n", n_char);
    
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
    
    load_text(&uni, &n_char);

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
