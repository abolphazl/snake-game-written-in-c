#include <stdio.h>
#include <conio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>

#define SNAKE_SIZE 200

int score;
int MAP_X;
int MAP_Y;
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN};
enum Direction dir;

int X_x;
int X_y;


struct Head {
    int x;
    int y;
};

struct Tail {
    int x[SNAKE_SIZE];
    int y[SNAKE_SIZE];
};

struct Snake {
    struct Head head;
    struct Tail tail;
    int len;
};
struct Snake snake;


void random_x() {
    srand(time(0));
    X_x = (rand() % (MAP_X-3)) + 1;
    X_y = (rand() % (MAP_Y-3)) + 1;
}


void render() {
    for (int i=0; i<MAP_X-1; i++) {
        for (int j=0; j<MAP_Y-1; j++) {
            
            // draw walls; check snake hit to wall
            if ((i == 0 || i == MAP_X-2) || (j == 0 || j == MAP_Y-2)) {
                if (snake.head.x == i && snake.head.y == j) {
                    clrscr();
                    puts("You hit the wall.");
                    exit(0);
                }
                putchar('#');
            }
            
            // draw snake head; generate new X if snake eat that
            else if (snake.head.x == i && snake.head.y == j) {
                putchar('O');
                if (X_x == i && X_y == j) {
                    random_x();
                    score++;
                    snake.len++;
                }
            }

            // draw X
            else if (i == X_x && j == X_y) {
                putchar('X');
            }


            // draw tail, spaces; check snake hit to himself
            else {
                int its_tail = 0;
                for (int m=0; m<snake.len; m++) {
                    if (snake.tail.x[m] == i && snake.tail.y[m] == j) {
                        // if (snake.head.x == i && snake.head.y == j) {
                        //     clrscr();
                        //     puts("You hit yourself.");
                        //     exit(0);
                        // }
                        putchar('o');
                        its_tail = 1;
                    }
                }
            
                if (!its_tail)
                    putchar(' ');
            }
        }
        printf("\n");
    }
}


void set_direction(char key) {
    switch (key) {
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'a':
            dir = LEFT;
            break;
    }
}


void move() {

    int prev_head_x = snake.head.x;
    int prev_head_y = snake.head.y;
    int tmp_x, tmp_y;

    switch (dir) {
        case LEFT:
            snake.head.y -= 1;
            break;
        case RIGHT:
            snake.head.y += 1;
            break;
        case UP:
            snake.head.x -= 1;
            break;
        case DOWN:
            snake.head.x += 1;
            break;
        default:
            break;
    }

    for (int i=0; i<snake.len; i++) {
        tmp_x = snake.tail.x[i];
        tmp_y = snake.tail.y[i];

        snake.tail.x[i] = prev_head_x;
        snake.tail.y[i] = prev_head_y;

        prev_head_x = tmp_x;
        prev_head_y = tmp_y;
    }
}

    



int main() {

    struct winsize w;    
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    MAP_X = w.ws_row;
    MAP_Y = w.ws_col;

    snake.head.x = MAP_X / 2;
    snake.head.y = MAP_Y / 2;

    random_x();

    while (1) {
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        MAP_X = w.ws_row-1;
        MAP_Y = w.ws_col;

        clrscr();

        if (kbhit()) {
            char key = getch();
            if (key == 'q')
                break;
            set_direction(key);
        }

        move();
        render();
        printf("Score: %d\n", score);
        usleep(80000);
    }

    return 0;
}
