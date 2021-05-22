#include <iostream>

#define NOMINMAX
#include <thread>
#include <vector>
#include <Windows.h>

std::vector<int> xPos;
std::vector<int> yPos;

int id = 0;
static void CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
	int MsgBox_X = xPos[id];
	int MsgBox_Y = yPos[id];
	
	SetWindowPos(hwnd, nullptr, MsgBox_X, MsgBox_Y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	id++;
}

void error(HWND hwnd, LPCWSTR msg, int X, int Y)
{
	HWINEVENTHOOK hHook = SetWinEventHook(EVENT_OBJECT_CREATE, EVENT_OBJECT_CREATE, nullptr,
		&WinEventProc, GetCurrentProcessId(), GetCurrentThreadId(), WINEVENT_INCONTEXT);

	// set position
	xPos.push_back(X);
	yPos.push_back(Y);
	
	MessageBox(hwnd, msg,
		L"Microsoft Windows", MB_OK | MB_ICONERROR);

	if (hHook) UnhookWinEvent(hHook);
}

int main(int argc, const char* argv)
{
	std::cout << "Gathering system info..." << std::endl;
	HWND hwnd = GetDesktopWindow();

	RECT rect;
	GetClientRect(hwnd, &rect);
	int width = (rect.right - rect.left);
	int height = (rect.bottom - rect.top);
	std::cout << "monitor width:" << width << std::endl;
	std::cout << "monitor height:" << height << std::endl;

	int center_x = width / 2;
	int center_y = height / 2;

	std::vector<std::thread> threads;
	
	std::cout << "Starting..." << std::endl;
	for (int i = 0; i < 5; i++) {
		//std::stringstream ss(); ss << i;
		
		threads.emplace_back(std::thread(error, hwnd, L"Windows was not installed correctly. Please reinstall Windows.\nError 4 (Windows error 096)", center_x + 30*i, center_y + 25*i));

		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}

	for(auto& t : threads)
	{
		t.join();
	}

	std::cout << "Done!";
	getchar();
	
	return 0;
}
