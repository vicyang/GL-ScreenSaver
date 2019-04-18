#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int load_text(char **buff, int *len)
{
    FILE *fp = fopen("sample.txt", "r");
    fseek( fp, 0, SEEK_END  );
    int size = ftell(fp);
    *buff = (char *) malloc(size);
    memset(*buff, '1', size);
    fseek( fp, 0, SEEK_SET );
    fread( *buff, 1, size, fp);
    *len = size;
    printf("Text Length: %d\n", size);
    return 0;
}
