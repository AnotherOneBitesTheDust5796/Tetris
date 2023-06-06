#include "Square.h"
#include "Piece.h"
using namespace std;

// Строки и столбцы основного поля
const int ROWS = 20;
const int COLS = 10;

// Основной игровой класс
class Game {
public:

	// Конструктор
	Game();

	// Основные методы игрового класса
	bool canRotate(Piece p);
	bool moveCollision(int dir);
	bool rotationCollision();
	void clearMainGrid();
	void clearNextPieceGrid();
	void genNextPiece();
	void restart();
	void move(int dir);
	void rotateShape(int rot);
	void updateActivePiece();
	void updateNextPieceGrid();
	void fixActivePiece();
	void update();
	void updateActiveAfterCollision();
	void checkLine();
	void clearLine();
	//bool gameOver();

	// Игровые фигуры
	Piece activePiece;
	Piece nextPiece;
	Piece activePieceCopy;

	// Текущая игровая сетка
	Square mainGrid[ROWS][COLS];

	// Сетка со следующим фрагментом
	Square nextPieceGrid[5][5];

	// Игровые данные
	bool killed;
	bool paused;
	bool deleteLines;
	int linesCleared;
	//int shapesCount;
	int timer;
};
