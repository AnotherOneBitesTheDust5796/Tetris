#include <windows.h>
#include <iostream>
using namespace std;
//фоновая музыка
void Sound()

{
	PlaySound("Sound.wav", NULL, SND_ASYNC);
}