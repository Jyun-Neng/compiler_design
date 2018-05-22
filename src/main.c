#include "global.h"
#include "scanner.h"
#include "parser.h"

FILE *source;
int line_no = 1;

int main(int argc, const char *argv[])
{
    char c;

    if (argc != 2) {
      fprintf(stderr, "No source file.\n");
      exit(1);
    }
    source = fopen(argv[1], "r");
    if (source == NULL) {
      fprintf(stderr, "File %s not found\n", argv[1]);
      exit(1);
    }
    fprintf(stdout, "\n File %s read\n", argv[1]);

    parser();

    fprintf(stdout, "\n Parsing end. Line %d\n", line_no);

    return 0;
}
