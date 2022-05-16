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
//����Ŀ�괰�ڵľ�������Ͻǵ�λ���Լ���ͼ�ĳ���ͱ����ַ
void Screenbmp(HWND hwnd, const char* path) {
	//���ָ�����ڵ�dc��Դdc��
	//ע��GetWindowDC��Ѵ��ڵı�����Ҳͬʱ��ͼ���������Ҫ���ڵı����ʹ��GetDC(hwnd)
	HDC hScreenDC = GetDC(hwnd);
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
	int width = 800;
	int height = 600;

	//���λͼ�������ڴ�
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
	HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hBitmap));
	BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);
	hBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hOldBitmap));
	BITMAP bmp;
	GetObject(hBitmap, sizeof(BITMAP), &bmp);

	//������ļ�
	FILE* fp;
	fopen_s(&fp, path, "w+b");

	//д��λͼ��Ϣ
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

	//����ɾ��
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