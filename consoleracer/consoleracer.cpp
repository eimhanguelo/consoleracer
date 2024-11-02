#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70

using namespace std;

class ConsoleUtils {
public:
    static HANDLE console;
    static COORD CursorPosition;

    static void gotoxy(int x, int y) {
        CursorPosition.X = x;
        CursorPosition.Y = y;
        SetConsoleCursorPosition(console, CursorPosition);
    }

    static void setcursor(bool visible, DWORD size) {
        if (size == 0) size = 20;
        CONSOLE_CURSOR_INFO lpCursor;
        lpCursor.bVisible = visible;
        lpCursor.dwSize = size;
        SetConsoleCursorInfo(console, &lpCursor);
    }

    static void setColor(int color) {
        SetConsoleTextAttribute(console, color);
    }
};

HANDLE ConsoleUtils::console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD ConsoleUtils::CursorPosition;

class Car {
private:
    int carPos;
    int carY;
    char car[4][4] = { {' ', '-', '-', ' '},
                       {'|', '-', '-', '|'},
                       {' ', '-', '-', ' '},
                       {'|', '-', '-', '|'} }; // Character representation of the car

public:
    Car() : carPos(WIN_WIDTH / 2), carY(22) {}

    void draw() const {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                ConsoleUtils::gotoxy(j + carPos, i + carY);
                cout << car[i][j]; // Erases the car's previous position
            }
        }
    }

    // Clears the car's position to avoid screen overlap
    void erase() const {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                ConsoleUtils::gotoxy(j + carPos, i + carY);
                cout << " ";
            }
        }
    }

    void moveLeft() {
        if (carPos > 18) carPos -= 4;     // Moves the car to the left within screen boundaries

    }

    void moveRight() {
        if (carPos < 50) carPos += 4;     // Moves the car to the right within screen boundaries

    }

    void moveUp() {
        if (carY > 1) carY--;     // Moves the car upwards within screen boundaries

    }

    void moveDown() {
        if (carY < SCREEN_HEIGHT - 5) carY++;     // Moves the car downwards within screen boundaries

    }

    int getX() const {
        return carPos;
    }

    int getY() const {
        return carY;
    }
};

class Enemy {
private:
    int x, y;
    bool active;
    int color;

public:
    Enemy() : x(0), y(1), active(false), color(12) {} // 12 is red in Windows console

    void spawn() {
        x = 17 + rand() % 33;
        y = 1;
        active = true;
    }

    void draw() const {
        if (active) {
            ConsoleUtils::setColor(color); // Set enemy color
            ConsoleUtils::gotoxy(x, y); cout << "****";
            ConsoleUtils::gotoxy(x, y + 1); cout << " ** ";
            ConsoleUtils::gotoxy(x, y + 2); cout << "****";
            ConsoleUtils::gotoxy(x, y + 3); cout << " ** ";
            ConsoleUtils::setColor(15); // Reset to default color (white)
        }
    }

    void erase() const {
        if (active) {
            ConsoleUtils::gotoxy(x, y); cout << "    ";
            ConsoleUtils::gotoxy(x, y + 1); cout << "    ";
            ConsoleUtils::gotoxy(x, y + 2); cout << "    ";
            ConsoleUtils::gotoxy(x, y + 3); cout << "    ";
        }
    }

    void moveDown() {
        if (active) y++;
    }

    bool isOutOfScreen() const {
        return y > SCREEN_HEIGHT - 4;
    }

    void deactivate() {
        active = false;
    }

    bool isActive() const {
        return active;
    }

    int getY() const {
        return y;
    }

    int getX() const {
        return x;
    }
};

class Game {
private:
    Car car;
    Enemy enemies[3];
    int score;

public:
    Game() : score(0) {}

    void drawBorder() {
        for (int i = 0; i < SCREEN_HEIGHT; i++) {
            for (int j = 0; j < 17; j++) {
                ConsoleUtils::gotoxy(0 + j, i); cout << "|";
                ConsoleUtils::gotoxy(WIN_WIDTH - j, i); cout << "|";
            }
        }
        for (int i = 0; i < SCREEN_HEIGHT; i++) {
            ConsoleUtils::gotoxy(SCREEN_WIDTH, i); cout << "|";
        }
    }

    void updateScore() const {
        ConsoleUtils::gotoxy(WIN_WIDTH + 7, 5);
        cout << "Score: " << score;
    }

    void gameover() const {
        system("cls");
        cout << "\n\n\tGame Over\n";
        cout << "\tPress Enter to return to the menu.";
        cin.ignore();
    }

    bool collision() {
        for (const auto& enemy : enemies) {
            if (enemy.isActive()) {
                if (enemy.getY() + 3 >= car.getY() && enemy.getY() <= car.getY() + 3) { // Check Y-axis overlap
                    if (enemy.getX() + 3 >= car.getX() && enemy.getX() <= car.getX() + 3) { // Check X-axis overlap
                        return true;
                    }
                }
            }
        }
        return false;
    }

    void play() {
        score = 0;
        enemies[0].spawn();  // Spawn the first enemy initially
        drawBorder();
        updateScore();

        while (1) {
            if (_kbhit()) {
                char ch = _getch();
                if (ch == 'a' || ch == 'A') car.moveLeft();
                else if (ch == 'd' || ch == 'D') car.moveRight();
                else if (ch == 'w' || ch == 'W') car.moveUp();
                else if (ch == 's' || ch == 'S') car.moveDown();
                else if (ch == 27) break;  // Exit the game loop
            }

            car.draw();

            // Handle each enemy car
            for (auto& enemy : enemies) {
                // Erase enemy from its current position before moving it down
                enemy.erase();

                if (enemy.isActive()) {
                    enemy.moveDown();  // Move the enemy down if it's active
                }

                // Check if enemy is out of screen
                if (enemy.isOutOfScreen()) {
                    enemy.deactivate();  // Deactivate and erase the enemy
                    score++;  // Increase the score for a successfully dodged car
                    updateScore();
                    enemy.spawn();  // Spawn a new enemy from the top
                }

                // Draw the enemy in its new position
                enemy.draw();
            }

            // Check for collision after moving enemies
            if (collision()) {
                gameover();
                return;
            }

            // Control game speed
            Sleep(50);

            // Erase the car after each iteration to prevent overlapping drawing
            car.erase();
        }
    }


    void instructions() const {
        system("cls");
        cout << "Instructions\n----------------\nAvoid cars by moving left, right, up, or down.\n";
        cout << "Press 'a' to move left\nPress 'd' to move right\nPress 'w' to move up\nPress 's' to move down\nPress 'escape' to exit\n";
        cout << "Press Enter to return to menu";
        cin.ignore();
    }

    void menu() {
        while (1) {
            system("cls");
            ConsoleUtils::gotoxy(10, 5); cout << "Car Game Menu";
            ConsoleUtils::gotoxy(10, 9); cout << "1. Start Game\n2. Instructions\n3. Quit\nSelect option: ";
            char op = cin.get();
            cin.ignore();

            if (op == '1') play();
            else if (op == '2') instructions();
            else if (op == '3') exit(0);
        }
    }
};

int main() {
    ConsoleUtils::setcursor(0, 0);
    srand(time(0));
    Game game;
    game.menu();
    return 0;
}
