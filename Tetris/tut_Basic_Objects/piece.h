// �������� ���������
const int numPieces = 7;	// ���������� ��������� ������
const int numRotations = 4; // ���������� �������� ��� ������ ������
const int numSpaces = 8;	// ����� ������ ��� �������� ���������� � ������ �������

// ����� ������� �����
class Piece {

public:

	// ������������
	Piece();
	Piece(int newPiece);

	// ���������� ��������� (����������� �� �������� ������ ���� ���������, ���� ���� ���� ���� ����)
	int x;
	int y;

	// �������� �����
	float redVal, greenVal, blueVal;

	// ��� ������ � �� ��������
	int type;
	int rotation;

	// �������� ������
	void rotatePiece(int dir);

	// ��������� ����� �������� � ������� RGB
	void color(float r, float g, float b);

	// �������� ��� ����������� �������
	const int* rotations();
};

// ��� ������ � ������ ��������� �� �������� � ����� � ������� {x0, y0, x1, y1, x2, y2, x3, y3}
const int gamePieces[numPieces][numRotations][numSpaces] =
{
	{
		{0, 0, 1, 0, 0, 1, 1, 1}, // �������
		{0, 0, 1, 0, 0, 1, 1, 1},
		{0, 0, 1, 0, 0, 1, 1, 1},
		{0, 0, 1, 0, 0, 1, 1, 1},
	},
	{
		{0, 0, 0, 1, 0, 2, 0, 3}, // �����
		{0, 0, 1, 0, 2, 0, 3, 0},
		{0, 0, 0, 1, 0, 2, 0, 3},
		{0, 0, 1, 0, 2, 0, 3, 0},
	},
	{
		{0, 0, 0, 1, 1, 1, 0, 2}, // �
		{1, 0, 0, 1, 1, 1, 2, 1},
		{0, 1, 1, 0, 1, 1, 1, 2},
		{0, 0, 1, 0, 2, 0, 1, 1}
	},
	{	{0, 0, 1, 0, 0, 1, 0, 2}, // �
		{0, 0, 0, 1, 1, 1, 2, 1},
		{1, 0, 1, 1, 0, 2, 1, 2},
		{0, 0, 1, 0, 2, 0, 2, 1}
	},
	{	{0, 0, 1, 0, 1, 1, 1, 2}, // � �����.
		{0, 0, 1, 0, 2, 0, 0, 1},
		{0, 0, 0, 1, 0, 2, 1, 2},
		{2, 0, 0, 1, 1, 1, 2, 1}
	},
	{	{0, 0, 0, 1, 1, 1, 1, 2}, // �������
		{1, 0, 2, 0, 0, 1, 1, 1},
		{0, 0, 0, 1, 1, 1, 1, 2},
		{1, 0, 2, 0, 0, 1, 1, 1}
	},
	{	{1, 0, 0, 1, 1, 1, 0, 2}, // ������� �����.
		{0, 0, 1, 0, 1, 1, 2, 1},
		{1, 0, 0, 1, 1, 1, 0, 2},
		{0, 0, 1, 0, 1, 1, 2, 1}
	}
};
