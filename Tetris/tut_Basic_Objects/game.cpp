#include <cstdlib>
#include <cstdio>
#include <iostream>

#include "GL/freeglut.h"
#include "Game.h"

using namespace std;

//���������� ������ ���, ����� �������� ������, �� ���� �� ������������� � �� ������������������
void Game::update() {
	// ������������ ����������� �����
	if (moveCollision(0)) {					// ���� ��������� ������������
		if (activePiece.y <= 2) {			// ���������, ��������� �� ��� ����
			killed = true;
		}
		else {								// ���� ���� ��� ��� �������
			updateActiveAfterCollision();	// ������ ����������� �� ����� ������������
			checkLine();					// �������� ������� ����������� �����
			if (deleteLines)				// ���� ���� ����������, ��� ������ ���� �������,
				clearLine();				// ����������� ������ ���������
			genNextPiece();					// �������� ������ ���������

			clearNextPieceGrid();
			updateNextPieceGrid();

			updateActivePiece();			// ��� ���������� � ������� �����
		}
	}
	else {									// ���� ������������ �� ����, ������ ���������� ����
		fixActivePiece();
		activePiece.y++;
		updateActivePiece();
	}
}

//Starting a new game and initializing the required elements
void Game::restart()
{
	clearMainGrid();		// ������� �������� �������
	clearNextPieceGrid();	// �������� ����� � ������� ���������� �������
	linesCleared = 0;		// ���� ������ ����� ����
	//shapesCount = 1;		// ������� ����� �� ���� ����� �������
	killed = false;
	paused = false;
	deleteLines = false;

	// ��������� �������� ���������� ���������
	activePiece = Piece(rand() % numPieces);
	activePiece.x = COLS / 2;
	activePiece.y = 0;
	updateActivePiece();

	// ���������� ��������� ��������
	nextPiece = Piece(rand() % numPieces);
	nextPiece.x = COLS / 2;
	nextPiece.y = 0;
	updateNextPieceGrid();
}

//���������� ������� ����� � ���������� ����������� �������� ������ ��� �������
void Game::fixActivePiece() {
	// ����������� ������ �������� ��������� �� ��� ���� � ���������
	const int* trans = activePiece.rotations();
	for (int i = 0; i < 8; i += 2) {
		Square& square = mainGrid[activePiece.y + trans[i + 1]][activePiece.x + trans[i]];
		// ��������� �������� � ���������� �����
		square.isFilled = false;
		square.isActive = false;
	}
}

//�������� ��������� ������� ������
void Game::genNextPiece() {
	activePiece = nextPiece;
	nextPiece = Piece(rand() % numPieces);
	nextPiece.x = COLS / 2;
	nextPiece.y = 0;
	// ��������� ������� ����� �� ����
	//shapesCount++;
}

//����������� �������� ������ ����� � ������
void Game::move(int dir)
{
	if (moveCollision(dir))	// ���� ���������� ������������ � ����� �� ������,
		return;				// ������ �� ����������
	fixActivePiece();
	activePiece.x += dir;
	updateActivePiece();
}

//������� �������� �������
void Game::clearMainGrid()
{
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			mainGrid[r][c].isFilled = false;
			mainGrid[r][c].isActive = false;
		}
	}
}

//������� ����� ��������� ����������
void Game::clearNextPieceGrid()
{
	for (int r = 0; r < 5; r++) {
		for (int c = 0; c < 5; c++) {
			nextPieceGrid[r][c].isFilled = false;
			nextPieceGrid[r][c].isActive = false;
		}
	}
}

//��������� ��������� �������� ������ � ������� ��������
void Game::updateActivePiece() {
	// ��������� �� ������, � ������� �������� ��� ��������������
	const int* trans = activePiece.rotations();
	for (int i = 0; i < 8; i += 2) {
		// ������� �������� ������ � ������� �����
		Square& square = mainGrid[activePiece.y + trans[i + 1]][activePiece.x + trans[i]];
		// ������������ �������� �������� � ����������� ������ �����
		square.isFilled = true;
		square.isActive = true;
		square.red = activePiece.redVal;
		square.green = activePiece.blueVal;
		square.blue = activePiece.greenVal;
	}
}

//��������� ����� ��������� ����������
void Game::updateNextPieceGrid() {
	// ��������� �� ������, � ������� �������� ��� ��������������
	const int* transNext = nextPiece.rotations();
	for (int i = 0; i < 8; i += 2) {
		// ������� �������� ������ � ������� �����
		Square& squareNext = nextPieceGrid[nextPiece.y + transNext[i + 1]][nextPiece.x + transNext[i]];
		// ������������ �������� �������� � ����������� ������ �����
		squareNext.isFilled = true;
		squareNext.isActive = true;
		squareNext.red = nextPiece.redVal;
		squareNext.green = nextPiece.blueVal;
		squareNext.blue = nextPiece.greenVal;
	}
}

//�����������
Game::Game()
{
	restart();
	timer = 500;
}

//��������� ������� ������ � ���������, ����� �� �� ������������
void Game::rotateShape(int dir) {
	// �������� ����� ��������� �������� � ���������, ����� �� ��� ���������
	activePieceCopy = Piece(rand() % numPieces);
	activePieceCopy.x = activePiece.x;
	activePieceCopy.y = activePiece.y;
	activePieceCopy.rotation = activePiece.rotation;
	activePieceCopy.type = activePiece.type;
	activePieceCopy.rotatePiece(dir);

	// ���� �������� ������� ����� ���������, �� �������������� � ������������
	if (canRotate(activePieceCopy)) {
		fixActivePiece();
		activePiece.rotatePiece(dir);
		updateActivePiece();
	}
}

//�������� ����, ����� �� ������������ ������
bool Game::canRotate(Piece activeP) {
	if (rotationCollision()) {
		return false;
	}
	else
		return true;
}

//�������� �� ������� ������������ ��� �������� ������
bool Game::rotationCollision() {
	int x, y;
	const int* trans = activePieceCopy.rotations();
	for (int i = 0; i < 8; i += 2) {
		x = activePieceCopy.x + trans[i];
		y = activePieceCopy.y + trans[i + 1];

		if (x >= COLS || y >= ROWS || x < 0 || (mainGrid[y][x].isFilled && !mainGrid[y][x].isActive))
			return true;
	}
	return false;
}

//�������� �� ������� ������������ ��� ����������� ��������
bool Game::moveCollision(int dir) {
	int x, y;
	const int* trans = activePiece.rotations();
	for (int i = 0; i < 8; i += 2) {
		x = activePiece.x + trans[i];
		y = activePiece.y + trans[i + 1];
		if (dir == 0)
			y += 1;
		else
			x += dir;
		if (x >= COLS || y >= ROWS || x < 0 || (mainGrid[y][x].isFilled && !mainGrid[y][x].isActive))
			return true;
	}
	return false;
}

//�������� �� ������� ������������ ��� ����������� ��������
void Game::updateActiveAfterCollision() {
	const int* trans = activePiece.rotations();
	for (int i = 0; i < 8; i += 2) {
		Square& square = mainGrid[activePiece.y + trans[i + 1]][activePiece.x + trans[i]];
		square.isActive = false;
	}
}

//�������� ����� �� ���������� � ��������� ����������� ����� ��� ��������
void Game::checkLine() {
	int fullRows = 0;
	for (int r = 0; r < ROWS; r++) {
		bool fullRow = false;
		for (int c = 0; c < COLS; c++) {
			Square& square = mainGrid[r][c];
			if (square.isFilled) {
				fullRow = true;
			}
			else {
				fullRow = false;
				break;
			}
		}
		if (fullRow) {
			for (int c = 0; c < COLS; c++) {
				mainGrid[r][c].toBeDeleted = true;
			}
			deleteLines = true;
			linesCleared++;
		}
	}
}

//������� ����������� ������ � ����������� ��� ������ ����� �� ���� ������ ����
void Game::clearLine() {
	for (int r = ROWS - 1; r > 0; r--) { // �������� ������ ������
		int linesDeleted = 0;
		if (mainGrid[r][0].toBeDeleted) {
			for (int r2 = r; r2 > 0; r2--) { // ����������� ��� ������ ���� �� ���� ������
				for (int c = 0; c < COLS; c++) {
					mainGrid[r2][c].isFilled = mainGrid[r2 - 1][c].isFilled;
					mainGrid[r2][c].isActive = mainGrid[r2 - 1][c].isActive;
					mainGrid[r2][c].toBeDeleted = mainGrid[r2 - 1][c].toBeDeleted;
					mainGrid[r2][c].red = mainGrid[r2 - 1][c].red;
					mainGrid[r2][c].green = mainGrid[r2 - 1][c].green;
					mainGrid[r2][c].blue = mainGrid[r2 - 1][c].blue;
				}
			}
			r++;
		}
	}
	deleteLines = false;

}