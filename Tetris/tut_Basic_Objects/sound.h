#include <windows.h>
#include <iostream>
using namespace std;
//������� ������
void Sound()

{
	PlaySound("Sound.wav", NULL, SND_ASYNC);
}