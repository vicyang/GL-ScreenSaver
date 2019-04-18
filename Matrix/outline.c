#include <ft2build.h>
#include <freetype/ftbbox.h>
#include <freetype/ftoutln.h>
#include FT_FREETYPE_H

typedef struct { float x, y; } point;

extern double vtx[5000][3];
extern int    vtx_contours[100];
extern int    Vi;
extern int    vtx_ctsi;
extern float  red;
extern int    PARTS;  //曲线细分程度
extern long   code;

extern FT_Library      library;
extern FT_Face         face;
extern FT_GlyphSlot    slot;
extern FT_Error        error;
extern FT_Outline      outline;
extern FT_BBox         box;

void ft_init(void)
{
    char*   filename;
    filename = "C:/windows/fonts/Consola.ttf";

    error = FT_Init_FreeType( &library );
    error = FT_New_Face( library, filename, 0, &face );  /* create face object */

    printf("font-name: %s, style: %s\n", face->family_name, face->style_name);
    printf("n_faces:%d, face_index:%d, face_flags:%d, style_flags:%d\n",
            face->num_faces,
            face->face_index,
            face->face_flags,
            face->style_flags
        );

    printf("numglyphs :%d, fixed_sizes: %d, generic: %d\n", 
            face->num_glyphs, 
            face->num_fixed_sizes, 
            face->generic
        );

    printf("bbox x: %d - %d, y: %d - %d, ascender %d, descender %d, height %d \n", 
            face->bbox.xMin,face->bbox.xMax,face->bbox.yMin,face->bbox.yMax,
            face->ascender, 
            face->descender,
            face->height
        );

    printf("n_contours: %d \n n_points: %d \n flags: %d\n", 
                        outline.n_contours,
                        outline.n_points,
                        outline.flags
                    );
}

int LoadGlyph(char symbol)
{
    //这里可以是unicode编码值，字体必须支持才行
    FT_UInt index = FT_Get_Char_Index(face, symbol);

    if (index <= 0)
    {
        printf("Error in %d\n", symbol);
        return 1;
    }
    else
    {
        //printf("no problem in %d\n", symbol);
        FT_Error error = FT_Load_Glyph(face,
                                       index,
                                       FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP);
        slot = face->glyph;
        outline = slot->outline;
        FT_Outline_Get_BBox( &outline, &box );
        return 0;
    }
}

void PointOnQuadBeizer(point cp[3], double t, double *vt )
{
    double cx, bx, ax, cy, by, ay;

    ax = cp[1].x - cp[0].x;
    ay = cp[1].y - cp[0].y;

    bx = cp[2].x - cp[1].x;
    by = cp[2].y - cp[1].y;

    cx = (bx * t + cp[1].x) - (ax * t + cp[0].x);
    cy = (by * t + cp[1].y) - (ay * t + cp[0].y);

    vt[0] = cx*t + (ax * t + cp[0].x);
    vt[1] = cy*t + (ay * t + cp[0].y);
    vt[2] = 0.0;
}

void GetDatafromOutline(void)
{
    Vi = 0;
    vtx_ctsi = 0;
    int bgn;
    int next;
    point lastpt;
    point cp[3];

    for (int cts = 0; cts < outline.n_contours; cts++ )
    {
        bgn = ( cts == 0 ? 0 : outline.contours[cts-1] + 1 );

        for (int i = bgn; i <= outline.contours[cts] ; i++)
        {
            //i==终点时，next回到某个轮廓的起点
            next = ( i == outline.contours[cts] ? bgn : i+1 );

            if ( outline.tags[i] == 0 )
            {
                cp[1].x = (double)outline.points[i].x;
                cp[1].y = (double)outline.points[i].y;

                if ( outline.tags[i-1] == 1 )            //1, 0
                {
                    cp[0].x = (double)outline.points[i-1].x;
                    cp[0].y = (double)outline.points[i-1].y;

                    if (outline.tags[next] == 1)         //1, 0, 1      
                    {
                        cp[2].x = (double) outline.points[next].x;
                        cp[2].y = (double) outline.points[next].y;

                        for (float i = 0.0; i < PARTS; i+=1.0 )
                        {
                            PointOnQuadBeizer(cp, i/PARTS, vtx[Vi]);
                            Vi++;
                        }
                    }
                    else                                 //1, 0, 0
                    {
                        cp[2].x = (double)(outline.points[i].x + outline.points[next].x)/2;
                        cp[2].y = (double)(outline.points[i].y + outline.points[next].y)/2;

                        for (float i=0.0; i <PARTS; i+=1.0 )
                        {
                            PointOnQuadBeizer(cp, i/PARTS, vtx[Vi]);
                            Vi++;
                        }
                    }
                }
                else                                     //0, 0
                {
                    //起点为上一段曲线的终点 而上一段曲线终点可能是计算出来的
                    cp[0].x = lastpt.x;
                    cp[0].y = lastpt.y;

                    if ( outline.tags[next] == 0 )       //0, 0, 0
                    {
                        cp[2].x = (double)(outline.points[i].x + outline.points[next].x)/2 ;
                        cp[2].y = (double)(outline.points[i].y + outline.points[next].y)/2 ;

                        for (float i=0.0; i < PARTS; i+=1.0 )
                        {
                            PointOnQuadBeizer(cp, i/PARTS, vtx[Vi]);
                            Vi++;
                        }
                    }
                    else                                 //0, 0, 1
                    {
                        cp[2].x = (double)outline.points[next].x ;
                        cp[2].y = (double)outline.points[next].y ;

                        for (float i=0.0; i < PARTS; i+=1.0 )
                        {
                            PointOnQuadBeizer(cp, i/PARTS, vtx[Vi]);
                            Vi++;
                        }
                    }
                }
                lastpt.x = cp[2].x;
                lastpt.y = cp[2].y;
            }
            else
            {
                //直线线段  只记录当前点
                if ( outline.tags[next] == 1 )           //1, 1
                {   
                    vtx[Vi][0] = outline.points[i].x;
                    vtx[Vi][1] = outline.points[i].y;
                    vtx[Vi][2] = 0.0;
                    Vi++;
                }
            }
        }

        vtx_contours[vtx_ctsi] = Vi-1;
        vtx_ctsi++;
    }
    //printf("vtx_ctsi: %d, nvtx: %d\n", vtx_ctsi, Vi);
}
