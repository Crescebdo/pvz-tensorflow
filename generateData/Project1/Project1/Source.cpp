#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include<windows.h>
#include<winuser.h>
#include<iostream>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "kernel32.lib")

using namespace std;
//输入目标窗口的句柄、左上角的位置以及截图的长宽和保存地址
void Screenbmp(HWND hwnd, const char* path) {
	//获得指定窗口的dc（源dc）
	//注意GetWindowDC会把窗口的标题栏也同时截图，如果不需要窗口的标题就使用GetDC(hwnd)
	HDC hScreenDC = GetDC(hwnd);
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
	int width = 800;
	int height = 600;

	//获得位图，存入内存
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
	HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hBitmap));
	BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);
	hBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hOldBitmap));
	BITMAP bmp;
	GetObject(hBitmap, sizeof(BITMAP), &bmp);

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
	byte* p = new byte[bmp.bmWidthBytes * bmp.bmHeight];
	GetDIBits(hMemoryDC, (HBITMAP)hBitmap, 0, height, p,
		(LPBITMAPINFO)&bih, DIB_RGB_COLORS);
	fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);

	//依次删除
	delete[] p;
	fclose(fp);
	DeleteObject(hBitmap);
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
	const char* p = "C:\\Users\\Chenz\\Desktop\\11\\1.bmp";
	Screenbmp(hwnd, p);
	system("pause");
}