#include <stdio.h>
#include <string.h>

void processSymbols(char *symbols, const char *dataType, int *address)
{
    char *symbol = strtok(symbols, ",;\n");
    int size;
    if (strcmp(dataType, "char") == 0){
        size = 1;
    }
    else if (strcmp(dataType, "int") == 0){
        size = 4;
    }
    else if (strcmp(dataType, "float") == 0){
        size = 4;
    }
    else if (strcmp(dataType, "long") == 0){
        size = 8;
    }
    else{
        size = 8;
    }

    while (symbol != NULL)
    {
        printf("%s\t%s\t\t%d\t%d\n", symbol, dataType, size, *address);
        *address += size;
        symbol = strtok(NULL, ",;\n"); 
    }
}

int main()
{
    FILE *file;
    file = fopen("main.c", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    char line[256];
    int address = 1000;

    printf("Symbol\tType\t\tSize\tAddress\n");

    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, "int main()") != NULL) 
        {
            continue;
        }

        char *token = strtok(line, " \t;,\n}");

        while (token != NULL)
        {
            if (strcmp(token, "int") == 0 || strcmp(token, "char") == 0 || strcmp(token, "long") == 0 || strcmp(token, "float") == 0 || strcmp(token, "double") == 0) // implies datatype found
            {
                
                char *symbols = strtok(NULL, " \t;\n"); 
                char *arrayBracket = strchr(symbols, '['); 
                if (arrayBracket != NULL) 
                {
                    *arrayBracket = '\0';  
                    int arraySize;
                    sscanf(arrayBracket + 1, "%d", &arraySize);
                    int elementSize;

                    if (strcmp(token, "float") == 0){
                        elementSize = 4;
                    }
                    else if (strcmp(token, "int") == 0){
                        elementSize = 4;
                    } 
                    else if (strcmp(token, "char") == 0){  
                        elementSize = 1;
                    }
                    else if (strcmp(token, "long") == 0){
                        elementSize = 8;
                    }
                    else{
                        elementSize = 8;
                    }

                    int totalSize = elementSize * arraySize;
                    printf("%s\t%s array\t%d\t%d\n", symbols, token, totalSize, address);

                    address += totalSize;
                }

                else
                {
                    processSymbols(symbols, token, &address);
                }
            }
            token = strtok(NULL, " \t\n"); 
        }
    }

    fclose(file);
    return 0;
}
