#include "Square.h"
#include "Piece.h"
using namespace std;

// ������ � ������� ��������� ����
const int ROWS = 20;
const int COLS = 10;

// �������� ������� �����
class Game {
public:

	// �����������
	Game();

	// �������� ������ �������� ������
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

	// ������� ������
	Piece activePiece;
	Piece nextPiece;
	Piece activePieceCopy;

	// ������� ������� �����
	Square mainGrid[ROWS][COLS];

	// ����� �� ��������� ����������
	Square nextPieceGrid[5][5];

	// ������� ������
	bool killed;
	bool paused;
	bool deleteLines;
	int linesCleared;
	//int shapesCount;
	int timer;
};
