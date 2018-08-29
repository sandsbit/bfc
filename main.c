#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s [source file]\n", argv[0]);
        return 1;
    }

    FILE *srcf = fopen(argv[1], "r");

    if (srcf == NULL) {
        printf("File not found\n");
        fclose(srcf);
        return 2;
    }

    char* source = malloc(sizeof(char) * 100000);
    fscanf(srcf, "%s", source);
    fclose(srcf);

    FILE* ccodef = fopen("bfctmp.c", "w");
    fprintf(ccodef, "#include <string.h>\n"
                    "#include <stdio.h>\n"
                    "int main() {"
                    "int i = 0;"
                    "char arr[30000];"
                    "memset(arr, 0, sizeof(arr));");
    for (int i = 0; i < strlen(source); i++) {
        switch (source[i]) {
            case '>':
                fprintf(ccodef, "i++;");
                break;
            case '<':
                fprintf(ccodef, "i--;");
                break;
            case '+':
                fprintf(ccodef, "arr[i]++;");
                break;
            case '-':
                fprintf(ccodef, "arr[i]--;");
                break;
            case '.':
                fprintf(ccodef, "putchar(arr[i]);");
                break;
            case ',':
                fprintf(ccodef, "arr[i] = getchar();");
                break;
            case '[':
                fprintf(ccodef, "while(arr[i]){");
                break;
            case ']':
                fprintf(ccodef, "}");
                break;
            default:
                printf("Invalid command: %c", source[i]);
                return 3;
        }
    }
    fprintf(ccodef, "}");
    fclose(ccodef);

    char * cmd = "gcc bfctmp.c -O3 -o bfapp.out";

    system(cmd);

    remove("bfctmp.c");
    return 0;
}