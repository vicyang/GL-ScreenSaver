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

int read_lines(char (*line)[], int *lines)
{
    FILE *fp = fopen("sample.txt", "r");

    *lines = 0;
    while ( fp != NULL )
    {
        if ( ! fgets( line[ *lines++ ], 1024, fp ) ) { break; }
    }

    printf("lines: %d\n", *lines);

    for (int n = 0; n < *lines; n++)
    {
        printf("%s", line[n]);
    }

    return 0;
}
