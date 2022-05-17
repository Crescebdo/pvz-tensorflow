#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include<windows.h>
#include<winuser.h>
#include<iostream>
#include <string> 
#include "memory.h"
#include <time.h>
#include <filesystem> 
#include <chrono>
#include <thread>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "kernel32.lib")

using namespace std;

IZE::Memory m;
const int total_type = IZE::TOTAL_NUM + 1;
int type[total_type];
int rest[total_type];
int saved[total_type];

void saveToFile(BITMAP bmp, const char* path, HDC hMemoryDC, HBITMAP hBitmap, int height) {
	//打开输出文件
	FILE* fp;
	fopen_s(&fp, path, "w+b");

	//写入位图信息
	BITMAPFILEHEADER bfh = { 0 };
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;
	bfh.bfType = (WORD)0x4d42;
	BITMAPINFOHEADER bih = { 0 };
	bih.biBitCount = bmp.bmBitsPixel;
	bih.biCompression = BI_RGB;
	bih.biHeight = bmp.bmHeight;
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;
	bih.biWidth = bmp.bmWidth;
	fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);
	fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);
	BYTE* p = new BYTE[bmp.bmWidthBytes * bmp.bmHeight];
	GetDIBits(hMemoryDC, (HBITMAP)hBitmap, 0, height, p,
		(LPBITMAPINFO)&bih, DIB_RGB_COLORS);
	fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);

	//依次删除
	delete[] p;
	fclose(fp);
}

bool IsPathExist(const std::string& s) {
	struct stat buffer;
	return (stat(s.c_str(), &buffer) == 0);
}

std::string getPath(std::string basic_path, int plant) {
	int x = std::distance(type, std::find(type, type + total_type, plant));
	std::string path = basic_path + to_string(plant);
	if (!IsPathExist(path)) std::filesystem::create_directory(path);
	path += "\\" + to_string(saved[x]++) + ".bmp";
	cout << path << endl;
	return path;
}

//输入目标窗口的句柄、左上角的位置以及截图的长宽和保存地址
void Screenbmp(int** code, HWND hwnd, string path, int left, int top, int small_width, int small_height, int colN = 5, int rowN = 5) {
	// embattle from code
	m.embattleFromPlants(code);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));


	//获得指定窗口的dc（源dc）
	//注意GetWindowDC会把窗口的标题栏也同时截图，如果不需要窗口的标题就使用GetDC(hwnd)
	HDC hScreenDC = GetDC(hwnd);
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
	int width = 800;
	int height = 600;

	//画红线（仅作测试用）
	/*SelectObject(hScreenDC, GetStockObject(DC_PEN));
	SetDCPenColor(hScreenDC, RGB(255, 0, 0));
	int startX = 40;
	int startY = 70;
	MoveToEx(hScreenDC, startX, startY, NULL);
	LineTo(hScreenDC, startX + 400, startY);
	LineTo(hScreenDC, startX + 400, startY + 500);
	LineTo(hScreenDC, startX, startY + 500);
	LineTo(hScreenDC, startX, startY);
	for (int i = 1; i <= 4; i++) {
		MoveToEx(hScreenDC, startX, startY + 100 * i, NULL);
		LineTo(hScreenDC, startX + 400, startY + 100 * i);
	}
	for (int i = 1; i <= 4; i++) {
		MoveToEx(hScreenDC, startX + 80 * i, startY, NULL);
		LineTo(hScreenDC, startX + 80 * i, startY + 500);
	}*/

	//获得位图，存入内存
	//分割成小图
	int count = 0;
	for (int i = 0; i < colN; i++) {
		for (int j = 0; j < rowN; j++) {
			int curr_left = left + small_width * i;
			int curr_top = top + small_height * j;
			HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, small_width, small_height);
			SelectObject(hMemoryDC, hBitmap);
			//StretchBlt(hMemoryDC, 0, 0, small_width / 2, small_height / 2, hScreenDC, curr_left, curr_top, small_width, small_height, SRCCOPY);
			BitBlt(hMemoryDC, 0, 0, small_width, small_height, hScreenDC, curr_left, curr_top, SRCCOPY);
			BITMAP bmp;
			GetObject(hBitmap, sizeof(BITMAP), &bmp);
			string new_path = getPath(path, code[j][i]);
			//const char* p = new char[new_path.size() + 1];
			//strcpy(p, new_path.c_str());
			saveToFile(bmp, new_path.c_str(), hMemoryDC, hBitmap, small_height);
			DeleteObject(hBitmap);
			count++;
		}
	}

	DeleteDC(hMemoryDC);
	DeleteDC(hScreenDC);
}


HWND hwnd;
int main() {
	hwnd = FindWindowW(L"MainWindow", L"Plants vs. Zombies");
	if (hwnd == nullptr) return 1;
	LPRECT lprect = new tagRECT{};
	cout << GetWindowRect(hwnd, lprect) << endl;
	cout << lprect->left << " " << lprect->top << " " << lprect->right << " " << lprect->bottom;
	const char* p = "C:\\Users\\Chenz\\Desktop\\dataset\\";
	if (!IsPathExist(p))std::filesystem::create_directory(p);

	// set up type arrays

	int target_ratio = 15;	// configurable
	int target_num = 5 * target_ratio;	// do not change
	for (int i = 0; i < total_type - 1; i++) type[i] = m.type[i];
	type[total_type - 1] = -1;
	for (int i = 0; i < total_type; i++) {
		rest[i] = target_num;
		saved[i] = 0;
	}

	// generate plants
	srand(time(NULL));
	cout << "total repeat = " << target_num * total_type / 25;
	for (int l = 1; l <= target_num * total_type / 25; l++) {
		int** plants = new int* [5];
		for (int i = 0; i < 5; i++) {
			int* row = new int[5];
			for (int j = 0; j < 5; j++) {
				int rand_plant = rand() % total_type;
				while (rest[rand_plant] < 1)
					rand_plant = rand() % total_type;
				rest[rand_plant]--;
				row[j] = type[rand_plant];
			}
			plants[i] = row;
		}
		Screenbmp(plants, hwnd, p, 40, 70, 80, 100);
		for (int i = 0; i < 5; i++) delete[] plants[i];
		delete[] plants;
	}

	system("pause");
}