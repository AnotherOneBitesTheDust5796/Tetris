#include <iostream>
#include <time.h>
#include <windows.h>
#include "Sound.h"

#include "GL/freeglut.h"
#include "Game.h"

using namespace std;

const int BLOCKSIZE = 40;
const int VPWIDTH = COLS * BLOCKSIZE;
const int VPHEIGHT = ROWS * BLOCKSIZE;


Game game;
GLvoid* font_style = GLUT_BITMAP_TIMES_ROMAN_24;

// Получение случайного числа в интервале [0, 1]
double random()
{
	int r = rand();
	return double(r) / RAND_MAX;
}

// Функция обратного вызова, таймер
void timer(int id)
{
	if (game.killed) {	// Если игра окончена

		game.paused = true;
		//glutTimerFunc(200, timer, id);
		game.clearMainGrid();
		game.clearNextPieceGrid();
		glutPostRedisplay();

	}
	else if (!game.paused) {	// Если игра продолжается и не поставлена на паузу
		game.update();
		if (game.killed) {
			glutTimerFunc(10, timer, 1);
		}
		else {
			glutPostRedisplay();
			glutTimerFunc(game.timer, timer, 0);
		}
	}
}

// Функция обратного вызова, обрабатывающая действия с клавиатуры
void keyboard(unsigned char key, int x, int y)
{
	if (game.paused && game.killed) {
		if (key == 13) { // 13 == ENTER
			game.killed = false;
			game.restart();
			glutTimerFunc(game.timer, timer, 0);
		}
	}
	else {
		if (key == 'p' || key == 27) { // 27 == ESCAPE
			game.paused = !game.paused;
			if (!game.paused)
				glutTimerFunc(game.timer, timer, 0);
		}
		else if (!game.paused && !game.killed && key == ' ') { // ' ' == SPACE
			game.rotateShape(-1);
			glutPostRedisplay();
		}
	}

	glutPostRedisplay();
}

// Функция обратного вызова, обрабатывающая действия со стрелками
void special(int key, int x, int y)
{
	if (!game.paused && !game.killed) {
		if (key == GLUT_KEY_LEFT) {
			game.move(-1);
			glutPostRedisplay();
		}
		else if (key == GLUT_KEY_RIGHT) {
			game.move(1);
			glutPostRedisplay();
		}
		else if (key == GLUT_KEY_DOWN) {
			game.update();
			glutPostRedisplay();
		}
	}
}

// Функция обратного вызова, отображающая окно игрового процесса
void display(void)
{
	const int N = 100;
	char msg[N + 1];

	glClearColor(0.2f, 0.2f, 0.2f, 0.72);
	glClear(GL_COLOR_BUFFER_BIT);

	// Сетка
	glViewport(0, 0, VPWIDTH, VPHEIGHT);
	glMatrixMode(GL_PROJECTION);

	if (!game.paused) {	// Если игра активна, то перемещение фигур осуществляется

		glLoadIdentity();
		gluOrtho2D(0, COLS, ROWS, 0);

		for (int r = 0; r < ROWS; r++) {
			for (int c = 0; c < COLS; c++) {
				Square& square = game.mainGrid[r][c];
				if (square.isFilled) {
					glColor3f(square.red, square.green, square.blue);
					glRectd(c + .1, r + .1, c + .9, r + .9);
				}
				else {
					glColor3f(0.2, 0.2, 0.2);
					glRectd(c, r, c + 1, r + 1);
				}
			}
		}
	}
	else {

		glLoadIdentity();
		gluOrtho2D(0, VPWIDTH, 0, VPHEIGHT);
	}

	// Вертикальная разделительная полоса
	glViewport(VPWIDTH, 0, VPWIDTH, VPHEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, VPWIDTH, 0, VPHEIGHT);

	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2d(1, 0);
	glVertex2d(1, glutGet(GLUT_WINDOW_HEIGHT));
	glEnd();

	// Сетка, отображающая следующий фрагмент
	glViewport(VPWIDTH + 50, -50, VPWIDTH, VPHEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, COLS, ROWS, 0);

	for (int r = 1; r < 5; r++) {
		for (int c = 0; c < 2; c++) {
			Square& square = game.nextPieceGrid[r][c];
			if (square.isFilled) {
				glColor3f(square.red, square.green, square.blue);
				glRectd(c + .1, r + .1, c + .9, r + .9);
			}
			else {
				glColor3f(0.2, 0.2, 0.2);
				glRectd(c, r, c + 1, r + 1);
			}
		}
	}

	glutSwapBuffers();
}

void soundtrack() {
	Sound();
}

void main(int argc, char* argv[])
{
	srand(time(0));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(550, 800);
	glutCreateWindow("Tetris");

	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(game.timer, timer, 0);
	soundtrack();

	glutMainLoop();
}
