#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <math.h>

#define WHITE 7
#define GREEN 2
#define RED 4
#define GREEN_BRIGHT 10
#define RED_BRIGHT 12

typedef struct
{
    unsigned short x;
    unsigned short y;
    unsigned short speed;
    unsigned char c;
} govno;

int fill_frame(char* buffer,govno* objects, int columns, int rows);

int clear_frame(HANDLE hConsole,int columns, int rows, CONSOLE_SCREEN_BUFFER_INFO csbi)
{
    COORD coordCur = csbi.dwCursorPosition;
    coordCur.X = 0;
    coordCur.Y = 0;
    SetConsoleCursorPosition(hConsole, coordCur);
    return 0;
}

void free_govno(govno* govno)
{
    free(govno);
}

govno* create_govno(int columns)
{
    govno* govnishe = malloc(columns*sizeof(govno));
    char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@";

    for (size_t i = 0; i < columns; i++)
    {
        govnishe[i].x = i;
        govnishe[i].y = 0;
        govnishe[i].speed = 1+rand() % 5;
        govnishe[i].c = chars[rand()%64];
    }
    return govnishe;
}

int print_frame(HANDLE hConsole, govno* objects, char* buffer, int columns, int rows)
{
    fill_frame(buffer,objects, columns, rows);

    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < columns; j++)
        {
            if (buffer[i*columns+j] == ' ')
            {
                putchar(' ');
                continue;
            }
            if (rand()%2)
            {
                SetConsoleTextAttribute(hConsole, GREEN);
            }
            else {
                SetConsoleTextAttribute(hConsole, GREEN_BRIGHT);
            }
            putchar(buffer[i*columns+j]);
        }
    }
    return 0;
}

int _console()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = 0;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    printf("columns: %d\nrows: %d\n", columns, rows);
    Sleep(1000);

    char* buffer = alloca(columns*rows);

    govno* objects = create_govno(columns);

    while (1)
    {
        print_frame(hConsole, objects, buffer, columns, rows);
        clear_frame(hConsole, columns, rows, csbi);
    }
    free_govno(objects);
    return 0;
}



int fill_frame(char* buffer, govno* objects, int columns, int rows)
{
    memset(buffer, ' ', columns*rows);
    int r1;
    char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@";
    char* chars_bright = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'.";

    // int i, j; // i = x line & j = y line
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        { 
            r1 = rand()%3;
            for (size_t k = 0; k < columns; k++)
            {
                if (objects[k].x == j && objects[k].y == i && r1 == 0)
                {
                    buffer[i*columns + k] = chars[k%64];
                }
                for (size_t a = 1; a < rows; a++)
                {
                    if (objects[k].x == j && (objects[k].y-a) == i && r1 == 0)
                    {
                        buffer[i*columns + k] = chars_bright[rand()%69];
                    }
                }
            }
        }
    }

    for (size_t k = 0; k < columns; k++)
    {
        objects[k].y+= objects[k].speed;
        if (objects[k].y == rows*2)
        {
            objects[k].c = chars[rand()%64];
            objects[k].y %= 60;
        } else if (objects[k].y > rows*2)
        {
            objects[k].c = chars[rand()%64];
            objects[k].y = 1+rand()%5+rand()%10;
        }
        
    }

    return 0;
}

int main()
{
    srand(time(NULL));
    system("cls");
    _console();
    return 0;
}