#include "Piece.h"

Piece::Piece() {
	Piece(0);
}

//Создание нового элемента, настройка его типа, цвета и поворота основания
Piece::Piece(int numType) {
	type = numType;
	switch (type) {
	case 0: color(1.0, 0.0, 1.0); break;
	case 1: color(0.0, 1.0, 1.0); break;
	case 2: color(0.7, 0.7, 0.0); break;
	case 3: color(0.0, 1.0, 0.0); break;
	case 4: color(1.0, 0.0, 0.5); break;
	case 5: color(0.0, 0.0, 1.0); break;
	case 6: color(1.0, 0.0, 0.0); break;
	}
	rotation = 0;
}

//Получение массива с информацией о конкретном элементе на основе его вращения
const int* Piece::rotations() {
	return gamePieces[type][rotation];
}

//Настройка цвета элемента
void Piece::color(float r, float g, float b) {
	redVal = r;
	greenVal = g;
	blueVal = b;
}

//Увеличивайте или уменьшайте индекс вращения игровых фигур
void Piece::rotatePiece(int dir) {
	if (dir > 0) {
		if (rotation == 3)
			rotation = 0;
		else
			rotation += dir;
	}
	else {
		if (rotation == 0)
			rotation = 3;
		else
			rotation += dir;
	}
}
