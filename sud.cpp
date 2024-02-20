
#include <GL/freeglut.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include <unistd.h>

#define N 9 
#define IN 20

int board[N][N];
int tempBoard[N][N];
bool gameEnded = false;
int selectedRow = -1;
int selectedCol = -1;
bool errorOccurred = false;
bool showMessage = false;
std::string errorMessage = "";
int errorCount = 0;

void copyArray(int source[N][N], int destination[N][N]);
bool proverkaInit(int row, int col, int num);
bool proverka(int row, int col, int num);
bool endInit();
bool end();
bool perebor(int row, int col, int &solutionCount);
void printBoard();
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void initial();

bool proverkaInit(int row, int col, int num) {
    for (int x = 0; x < N; ++x)
        if (board[row][x] == num)
            return false;
    for (int x = 0; x < N; ++x)
        if (board[x][col] == num)
            return false;
    int startRow = row - row % (N / 3);
    int startCol = col - col % (N / 3);
    for (int i = 0; i < N / 3; ++i)
        for (int j = 0; j < N / 3; ++j)
            if (board[i + startRow][j + startCol] == num)
                return false;
    return true;
}
bool proverka(int row, int col, int num) {
    for (int x = 0; x < N; ++x)
        if (tempBoard[row][x] == num)
            return false;
    for (int x = 0; x < N; ++x)
        if (tempBoard[x][col] == num)
            return false;
    int startRow = row - row % (N / 3);
    int startCol = col - col % (N / 3);
    for (int i = 0; i < N / 3; ++i)
        for (int j = 0; j < N / 3; ++j)
            if (tempBoard[i + startRow][j + startCol] == num)
                return false;
    return true;
}
bool endInit() {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (board[i][j] == 0)
                return false;
    return true;
}
bool end() {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (tempBoard[i][j] == 0)
                return false;
    return true;
}
bool perebor(int row, int col, int &solutionCount) {
    if (row == N) {
        if (endInit()) {
            ++solutionCount;
            return true;
        }
        return false;
    }

    if (col == N) {
        return perebor(row + 1, 0, solutionCount);
    }

    if (board[row][col] != 0) {
        return perebor(row, col + 1, solutionCount);
    }

    for (int num = 1; num <= N; ++num) {
        if (proverka(row, col, num)) {
            board[row][col] = num;
            if (perebor(row, col + 1, solutionCount) && solutionCount < 2) {
                return true;
            }
            board[row][col] = 0;
        }
    }

    return false;
}


void keyboard(unsigned char key, int x, int y) {
    showMessage = false;
    switch (key) {
        case 27: // ESC 
            exit(0);
            break;
        case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':
            if (selectedRow != -1 && selectedCol != -1 && !gameEnded) {
                int num = key - '0';
                if (proverka(selectedRow, selectedCol, num)) {
                    tempBoard[selectedRow][selectedCol] = num;
                    selectedRow = -1;
                    selectedCol = -1;
                    showMessage = false;
                    printBoard();
                    if (end()) {
                        std::cout << "Вы решили судоку!" << std::endl;
                        gameEnded = true;
                    }
                } else {
                    errorOccurred = true;
                    showMessage = true;
                    errorMessage = "Ошибка!";
                    std::cout << "Ошибка!" << std::endl;
                    ++errorCount;
                    if (errorCount >= 5) {
                        std::cout << "Пять ошибок, конец игры!" << std::endl;
                        gameEnded = true;
                    }
                    printBoard();
                }
            }
            break;
    }
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int col = x / (600 / N); 
        int row = N - y / (600 / N) - 1; 
        selectedRow = row;
        selectedCol = col;
        glutPostRedisplay();
    }
}

void copyArray(int source[N][N], int destination[N][N]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            destination[i][j] = source[i][j];
        }
    }
}
void initial() {
    srand(time(0));
    memset(board, 0, sizeof(board));

    int numbersToFill = 0;

    while (numbersToFill < IN) {
        int row = rand() % N;
        int col = rand() % N;
        int num = (rand() % N) + 1;
        if (board[row][col] == 0 && proverkaInit(row, col, num)) {
            board[row][col] = num;
            ++numbersToFill;
        }
    }

    copyArray(board,tempBoard);

    int solutionCount = 0;
    if (!perebor(0, 0, solutionCount) || solutionCount != 1) {
       initial();
    }
}

void printBoard() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // сетка
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            bool isWhite = ((i / 3 + j / 3) % 2 == 0);
            if (isWhite) {
                glColor3f(1.0, 1.0, 1.0); // белый
            } else {
                glColor3f(0.85, 0.85, 0.85); // серый
            }
            glBegin(GL_POLYGON);
            glVertex2f(j, i);
            glVertex2f(j + 1, i);
            glVertex2f(j + 1, i + 1);
            glVertex2f(j, i + 1);
            glEnd();
        }
    }

    // границы ячеек
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    for (int i = 0; i <= N; ++i) {
        if (i % 3 == 0) {
            glLineWidth(2.5);
        } else {
            glLineWidth(1.0);
        }
        glVertex2f(i, 0);
        glVertex2f(i, N);
        glVertex2f(0, i);
        glVertex2f(N, i);
    }
    glEnd();

    // цифры 
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (tempBoard[i][j] != 0) {
                glRasterPos2f(j + 0.5 - 0.15, i + 0.5 - 0.15);
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0' + tempBoard[i][j]);
            }
        }
    }

    // подсветка выбранной
    if (selectedRow != -1 && selectedCol != -1) {
        glColor3f(0.0, 0.0, 1.0);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(selectedCol, selectedRow);
        glVertex2f(selectedCol + 1, selectedRow);
        glVertex2f(selectedCol + 1, selectedRow + 1);
        glVertex2f(selectedCol, selectedRow + 1);
        glEnd();
    }

   if (showMessage) {
        // Определите позицию для вывода сообщения
        int posX = 10; // Задайте нужное значение
        int posY = 10; // Задайте нужное значение

        // Установите цвет текста
        glColor3f(1.0, 0.0, 0.0); // Красный цвет текста

        // Установите позицию для вывода текста
        glRasterPos2f(posX, posY);

        // Выведите текст сообщения об ошибке
       for (char& c : errorMessage) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }
    

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Sudoku");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, N, 0, N);
    glMatrixMode(GL_MODELVIEW);

    initial();
    glutDisplayFunc(printBoard);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    glutMainLoop();

    return 0;
}
