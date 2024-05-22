#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_VARIABLES 100
#define MAX_NAME_LENGTH 50
#define MAX_LINE_LENGTH 100

struct Variable
{
    char name[MAX_NAME_LENGTH];
    char type[MAX_NAME_LENGTH];
    int length;
    int address;
};

struct Variable symbolTable[MAX_VARIABLES];
int symbolCount = 0;

void addToSymbolTable(const char *name, const char *type, int length, int address)
{
    if (symbolCount < MAX_VARIABLES)
    {
        strcpy(symbolTable[symbolCount].name, name);
        strcpy(symbolTable[symbolCount].type, type);
        symbolTable[symbolCount].length = length;
        symbolTable[symbolCount].address = address;
        symbolCount++;
    }
}

void processArrayDeclaration(char *token, int *address)
{
    char *name = strtok(token, "[");
    int length = atoi(strtok(NULL, "]"));
    addToSymbolTable(name, "array", length, *address);
    *address += length * sizeof(char); // Corrected for correct size calculation
}

int main()
{
    FILE *file = fopen("read.c", "r");

    char line[MAX_LINE_LENGTH];
    int address = 1000;

    while (fgets(line, sizeof(line), file))
    {
        char *token = strtok(line, " ();,\n\t");

        while (token != NULL)
        {
            if (strcmp(token, "int") == 0 || strcmp(token, "char") == 0 || strcmp(token, "float") == 0 || strcmp(token, "double") == 0)
            {
                size_t size = -1;
                const char *type;

                if (strcmp(token, "int") == 0)
                {
                    size = sizeof(int);
                    type = "int";
                }
                else if (strcmp(token, "char") == 0)
                {
                    size = sizeof(char);
                    type = "char";
                }
                else if (strcmp(token, "float") == 0)
                {
                    size = sizeof(float);
                    type = "float";
                }
                else if (strcmp(token, "double") == 0)
                {
                    size = sizeof(double);
                    type = "double";
                }

                token = strtok(NULL, " ();,\n\t");

                while (token != NULL && token[0] != ';')
                {
                    if (token[0] == '[')
                    {
                        processArrayDeclaration(token, &address);
                    }
                    else if (strchr(token, '[') != NULL)
                    {
                        processArrayDeclaration(token, &address);
                    }
                    else if (isalpha(token[0]) || token[0] == '_')
                    {
                        if (strcmp(token, "main") != 0)
                        {
                            addToSymbolTable(token, type, size, address);
                            address += size;
                        }
                    }
                    token = strtok(NULL, " ();,\n\t");
                }
            }
            else
            {
                char *temp = strdup(token);
                token = strtok(NULL, " ();,\n\t");
            }
            token = strtok(NULL, " ();,\n\t");
        }
    }

    fclose(file);

    printf("Symbol Table:\n");
    printf("-------------------------------------------------\n");
    printf("| %-10s | %-10s | %-10s | %-10s |\n", "Symbol", "Type", "Length", "Address");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < symbolCount; i++)
    {
        printf("| %-10s | %-10s | %-10d | %-10d |\n", symbolTable[i].name, symbolTable[i].type, symbolTable[i].length, symbolTable[i].address);
    }
    printf("-------------------------------------------------\n");

    return 0;
}