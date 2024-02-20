#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#define N 9 //лучше оставить 9... ну можно 15
#define IN 30 //сколько заполнено ячеек изначально
using namespace std;

void printBoard(int board[N][N]) {
    for (int i = 0; i < N; i++) {
        if (i % (N/3) == 0 && i != 0)
            cout << "_ _ _ _ _ _ _ _ _ _ _" <<endl;
        for (int j = 0; j < N; j++) {
            if (j %(N/3)== 0 && j != 0)
                cout << "| ";
            if (N<10)
                cout << board[i][j] << " ";
            else if ( board[i][j] > 9)
                cout << board[i][j] << " ";
            else
                cout << board[i][j] << "  ";
        }
        cout << endl;
    }
}

bool proverka(int board[N][N], int row, int col, int num) {
    for (int x = 0; x < N; x++)
        if (board[row][x] == num)
            return false;
    for (int x = 0; x < N; x++)
        if (board[x][col] == num)
            return false;
    int startRow = row - row % (N/3);
    int startCol = col - col %(N/3);
    for (int i = 0; i < (N/3); i++)
        for (int j = 0; j < (N/3); j++)
            if (board[i + startRow][j + startCol] == num)
                return false;
    return true;
}

void initial(int board[N][N]) {
    srand(time(0)); 
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            board[i][j] = 0; // занулить поле

    for (int i=0; i < IN; ) {
        int row = rand() % N;
        int col = rand() % N;
        int num = (rand() % N) + 1;
        if (board[row][col] == 0 && proverka(board, row, col, num))
            {board[row][col] = num;
            i++;}
        
    }
}

bool perebor(int board[N][N]) {
    int row, col;
    bool isEmpty = true;
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            if (board[row][col] == 0) {
                isEmpty = false;
                break;
            }
        }
        if (!isEmpty)
            break;
    }
    if (isEmpty)
        return true;

    for (int num = 1; num <= N; num++) {
        if (proverka(board, row, col, num)) {
            board[row][col] = num;
            if (perebor(board))
                return true;
            board[row][col] = 0;
        }
    }
    return false;
}

bool end(int board[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (board[i][j] == 0)
                return false;
    return true;
}

int main() {
    int board[N][N];
    int cnt=0;
    cout << "изначальное поле:" << endl;
    initial(board);
    printBoard(board);
    
    while (!end(board)) {
        int row, col, num;
        cout << "введите строку, столбец и число: ";
        cin >> row >> col >> num;
        if (row < 1 || row > N || col < 1 || col > N || num < 1 || num > N) {
            cout << "ошибка ввода" << endl;
            continue;
        }
        row--, col--;
        if (board[row][col] != 0) {
            cout << "эта ячейка уже занята" << endl;
            continue;
        }
        if (!proverka(board, row, col, num)) {
            cout << "ошибка" << endl;
            cnt++;
            if (cnt==5) 
                {cout << "5 ошибок, конец" << endl;
                    return 0;}
            continue;
        }
        board[row][col] = num;
        cout << "текущее поле:" << endl;
        printBoard(board);
    }
    cout << "ура" << endl;
    return 0;
}
