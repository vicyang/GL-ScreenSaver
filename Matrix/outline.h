#include <ft2build.h>
#include <freetype/ftbbox.h>
#include <freetype/ftoutln.h>
#include FT_FREETYPE_H

typedef struct { float x, y; } point;

void ft_init(void);
int LoadGlyph(long symbol);
void PointOnQuadBeizer(point cp[3], double t, double *vt );
void GetDatafromOutline(void);
