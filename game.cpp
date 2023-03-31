#include <stdio.h>
#include <conio.h>
#include <thread>
#include <chrono>
#include <random>

const size_t field_width  = 30;
const size_t field_height = 20;
const size_t MAX_LENGTH = 100;

int snakeX[MAX_LENGTH], snakeY[MAX_LENGTH];
int length = 3;
int direction = 0; // 0: up, 1: right, 2: down, 3: left
int fruitX, fruitY;
int score = 0;
int gameOver = 0;

void init() {
    int i;

    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_int_distribution<int> rand_width(0, field_width-1);
    std::uniform_int_distribution<int> rand_height(0, field_height-1);

    // Initialize snake's starting position
    snakeX[0] = field_width >> 1;
    snakeY[0] = field_width >> 1;
    for (i = 1; i < length; i++) {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1] + 1;
    }

    // Initialize fruit's position
    fruitX = rand_width(mt);
    fruitY = rand_height(mt);
}

void draw() {
    int i, j;

    // Clear the screen
    system("cls");

    // Draw the border
    for (i = 0; i < field_width + 2; i++) {
        printf("#");
    }
    printf("\n");

    // Draw the game board
    for (j = 0; j < field_height; j++) {
        printf("#");
        for (i = 0; i < field_width; i++) {
            if (i == snakeX[0] && j == snakeY[0]) {
                printf("O");
            }
            else if (i == fruitX && j == fruitY) {
                printf("F");
            }
            else {
                int k;
                int isBody = 0;
                for (k = 1; k < length; k++) {
                    if (i == snakeX[k] && j == snakeY[k]) {
                        printf("o");
                        isBody = 1;
                        break;
                    }
                }
                if (!isBody) {
                    printf(" ");
                }
            }
        }
        printf("#\n");
    }

    // Draw the border
    for (i = 0; i < field_width + 2; i++) {
        printf("#");
    }
    printf("\n");

    // Draw the score
    printf("Score: %d\n", score);
}

void input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'w':
            direction = 0;
            break;
        case 'd':
            direction = 1;
            break;
        case 's':
            direction = 2;
            break;
        case 'a':
            direction = 3;
            break;
        }
    }
}

void update() {
    int i;

    // Move the snake
    for (i = length - 1; i > 0; i--) {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }
    switch (direction) {
    case 0:
        snakeY[0]--;
        break;
    case 1:
        snakeX[0]++;
        break;
    case 2:
        snakeY[0]++;
        break;
    case 3:
        snakeX[0]--;
        break;
    }

    // Check for collision with the fruit
    if (snakeX[0] == fruitX && snakeY[0] == fruitY) {
        score++;
        length++;
        fruitX = rand() % field_width;
        fruitY = rand() % field_height;
    }

    // Check for collision with the walls
    if (snakeX[0] < 0 || snakeX[0] >= field_width || snakeY[0] < 0 || snakeY[0] >= field_height) {
        gameOver = 1;
    }

    // Check for collision with the body
    for (i = 1; i < length; i++) {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
            gameOver = 1;
            break;
        }
    }
}

int main() {
    // Initialize the game
    init();

    // Loop until the game is over
    while (!gameOver) {
        // Draw the game
        draw();

        // Handle input
        input();

        // Update the game
        update();

        // Sleep for a short time to control the game speed
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Game over
    printf("Game over!\n");

    return 0;
}