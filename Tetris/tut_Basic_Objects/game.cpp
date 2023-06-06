#include <cstdlib>
#include <cstdio>
#include <iostream>

#include "GL/freeglut.h"
#include "Game.h"

using namespace std;

//Вызывается всякий раз, когда истекает таймер, но игра не заканчивается и не приостанавливается
void Game::update() {
	// Столкновение контрольных фигур
	if (moveCollision(0)) {					// Если произошло столкновение
		if (activePiece.y <= 2) {			// проверяем, завершает ли оно игру
			killed = true;
		}
		else {								// Если игра все еще активна
			updateActiveAfterCollision();	// Фигура фиксируется на месте столкновения
			checkLine();					// Проверка наличия заполненных строк
			if (deleteLines)				// Если было обнаружено, что строки были удалены,
				clearLine();				// заполненные строки удаляются
			genNextPiece();					// Создание нового фрагмента

			clearNextPieceGrid();
			updateNextPieceGrid();

			updateActivePiece();			// Его обновление в игровой сетке
		}
	}
	else {									// Если столкновения не было, фигура опускается ниже
		fixActivePiece();
		activePiece.y++;
		updateActivePiece();
	}
}

//Starting a new game and initializing the required elements
void Game::restart()
{
	clearMainGrid();		// Очистка основной решетки
	clearNextPieceGrid();	// Очистите сетку с помощью следующего рисунка
	linesCleared = 0;		// Счет игрока равен нулю
	//shapesCount = 1;		// Счетчик фигур за игру равен единице
	killed = false;
	paused = false;
	deleteLines = false;

	// Генерация текущего случайного фрагмента
	activePiece = Piece(rand() % numPieces);
	activePiece.x = COLS / 2;
	activePiece.y = 0;
	updateActivePiece();

	// Генерируем следующий фрагмент
	nextPiece = Piece(rand() % numPieces);
	nextPiece.x = COLS / 2;
	nextPiece.y = 0;
	updateNextPieceGrid();
}

//Обновление игровой сетки и корректное отображение активной фигуры при падении
void Game::fixActivePiece() {
	// Определение данных текущего фрагмента по его типу и положению
	const int* trans = activePiece.rotations();
	for (int i = 0; i < 8; i += 2) {
		Square& square = mainGrid[activePiece.y + trans[i + 1]][activePiece.x + trans[i]];
		// Установка активных и неактивных ячеек
		square.isFilled = false;
		square.isActive = false;
	}
}

//Создайте следующую игровую фигуру
void Game::genNextPiece() {
	activePiece = nextPiece;
	nextPiece = Piece(rand() % numPieces);
	nextPiece.x = COLS / 2;
	nextPiece.y = 0;
	// Увеличьте счетчик фигур за игру
	//shapesCount++;
}

//Перемещайте активную фигуру влево и вправо
void Game::move(int dir)
{
	if (moveCollision(dir))	// Если происходит столкновение с одной из границ,
		return;				// ничего не происходит
	fixActivePiece();
	activePiece.x += dir;
	updateActivePiece();
}

//Очистка основной решетки
void Game::clearMainGrid()
{
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			mainGrid[r][c].isFilled = false;
			mainGrid[r][c].isActive = false;
		}
	}
}

//Очищаем сетку следующим фрагментом
void Game::clearNextPieceGrid()
{
	for (int r = 0; r < 5; r++) {
		for (int c = 0; c < 5; c++) {
			nextPieceGrid[r][c].isFilled = false;
			nextPieceGrid[r][c].isActive = false;
		}
	}
}

//Изменение положения активной детали с помощью поворота
void Game::updateActivePiece() {
	// Указатель на массив, в котором хранятся все преобразования
	const int* trans = activePiece.rotations();
	for (int i = 0; i < 8; i += 2) {
		// Найдите активную фигуру в игровой сетке
		Square& square = mainGrid[activePiece.y + trans[i + 1]][activePiece.x + trans[i]];
		// Преобразуйте активный фрагмент в заполненные ячейки сетки
		square.isFilled = true;
		square.isActive = true;
		square.red = activePiece.redVal;
		square.green = activePiece.blueVal;
		square.blue = activePiece.greenVal;
	}
}

//Обновляем сетку следующим фрагментом
void Game::updateNextPieceGrid() {
	// Указатель на массив, в котором хранятся все преобразования
	const int* transNext = nextPiece.rotations();
	for (int i = 0; i < 8; i += 2) {
		// Найдите активную фигуру в игровой сетке
		Square& squareNext = nextPieceGrid[nextPiece.y + transNext[i + 1]][nextPiece.x + transNext[i]];
		// Преобразуйте активный фрагмент в заполненные ячейки сетки
		squareNext.isFilled = true;
		squareNext.isActive = true;
		squareNext.red = nextPiece.redVal;
		squareNext.green = nextPiece.blueVal;
		squareNext.blue = nextPiece.greenVal;
	}
}

//Конструктор
Game::Game()
{
	restart();
	timer = 500;
}

//Поверните текущую деталь и проверьте, можно ли ее поворачивать
void Game::rotateShape(int dir) {
	// Создайте копию активного элемента и проверьте, можно ли его повернуть
	activePieceCopy = Piece(rand() % numPieces);
	activePieceCopy.x = activePiece.x;
	activePieceCopy.y = activePiece.y;
	activePieceCopy.rotation = activePiece.rotation;
	activePieceCopy.type = activePiece.type;
	activePieceCopy.rotatePiece(dir);

	// Если активный элемент можно повернуть, он поворачивается и отображается
	if (canRotate(activePieceCopy)) {
		fixActivePiece();
		activePiece.rotatePiece(dir);
		updateActivePiece();
	}
}

//Проверка того, можно ли поворачивать деталь
bool Game::canRotate(Piece activeP) {
	if (rotationCollision()) {
		return false;
	}
	else
		return true;
}

//Проверка на наличие столкновений при вращении детали
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

//Проверка на наличие столкновений при перемещении элемента
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

//Проверка на наличие столкновений при перемещении элемента
void Game::updateActiveAfterCollision() {
	const int* trans = activePiece.rotations();
	for (int i = 0; i < 8; i += 2) {
		Square& square = mainGrid[activePiece.y + trans[i + 1]][activePiece.x + trans[i]];
		square.isActive = false;
	}
}

//Проверка строк на заполнение и установка заполненных строк для удаления
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

//Удалите заполненную строку и переместите все фигуры вверх на одну ячейку вниз
void Game::clearLine() {
	for (int r = ROWS - 1; r > 0; r--) { // Проверка каждой строки
		int linesDeleted = 0;
		if (mainGrid[r][0].toBeDeleted) {
			for (int r2 = r; r2 > 0; r2--) { // Переместите все строки вниз на одну ячейку
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