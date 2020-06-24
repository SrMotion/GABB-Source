#pragma once
#include <Windows.h>
#include <fstream>
#include <string>

void cout(std::wstring);

namespace SCREENCAP {
	BYTE	*m_pLastImage;
	int		m_Width;
	int		m_Height;
	int		m_Bpp;

	void Get24BitBmp(const int &nWidth, const int &nHeight, const HBITMAP &hBitmap, BYTE *lpDesBits)
	{
		HDC hDC = GetDC(0);

		HDC memDC1 = CreateCompatibleDC(hDC);
		HDC memDC2 = CreateCompatibleDC(hDC);

		BYTE *lpBits = NULL;

		BITMAPINFO bmi;
		ZeroMemory(&bmi, sizeof(BITMAPINFO));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = nWidth;
		bmi.bmiHeader.biHeight = nHeight;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 24;
		bmi.bmiHeader.biCompression = BI_RGB;

		HBITMAP hDIBMemBM = ::CreateDIBSection(0, &bmi, DIB_RGB_COLORS, (void**)&lpBits, NULL, NULL);

		HBITMAP hOldBmp1 = (HBITMAP)::SelectObject(memDC1, hDIBMemBM);

		HBITMAP hOldBmp2 = (HBITMAP) ::SelectObject(memDC2, hBitmap);

		BitBlt(memDC1, 0, 0, nWidth, nHeight, memDC2, 0, 0, SRCCOPY);

		for (int i = 0; i < nHeight; i++)
			CopyMemory(&lpDesBits[i * 3 * nWidth], &lpBits[nWidth * 3 * (nHeight - 1 - i)], nWidth * 3);

		// clean up
		SelectObject(memDC1, hOldBmp1);
		SelectObject(memDC2, hOldBmp2);
		ReleaseDC(0, hDC);
		DeleteObject(hDIBMemBM);
		DeleteObject(hOldBmp1);
		DeleteObject(hOldBmp2);
		DeleteDC(memDC1);
		DeleteDC(memDC2);
	}

	void UpdateLastImage(int Width, int Height, int Size, int Bpp, BYTE* lpBits)
	{
		if (m_pLastImage)
		{
			delete[]m_pLastImage;
			m_pLastImage = 0;
		}
		if (m_pLastImage == 0) // first time
			m_pLastImage = new BYTE[Size];

		memcpy(m_pLastImage, lpBits, Size);
		m_Width = Width;
		m_Height = Height;
		m_Bpp = Bpp;
	}

	BYTE NumberScan(char nr) {
		int offset = nr - 48;
		if (offset < 0 || offset > 9) offset = 0;
		//cout(std::to_string(offset));
		return 0x60 + offset;
	}

	void SaveBmpToFile(LPCTSTR szFileName, int W, int H, int Bpp, int* lpBits)
	{
		BITMAPINFO Bmi = { 0 };
		Bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		Bmi.bmiHeader.biWidth = W;
		Bmi.bmiHeader.biHeight = H;
		Bmi.bmiHeader.biPlanes = 1;
		Bmi.bmiHeader.biBitCount = Bpp;
		Bmi.bmiHeader.biCompression = BI_RGB;
		Bmi.bmiHeader.biSizeImage = W * H*Bpp / 8;

		FILE * fp;
		if (_wfopen_s(&fp, szFileName, L"wb") != 0) return;
		int h = Bmi.bmiHeader.biHeight;
		int w = Bmi.bmiHeader.biWidth;
		Bmi.bmiHeader.biHeight = -h;
		Bmi.bmiHeader.biWidth = w;

		BITMAPFILEHEADER bfh = { 0 };
		bfh.bfType = ('M' << 8) + 'B';
		bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		bfh.bfSize = Bmi.bmiHeader.biSizeImage + bfh.bfOffBits;

		fwrite(&bfh, sizeof(bfh), 1, fp);
		fwrite(&Bmi.bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp);
		fwrite(lpBits, Bmi.bmiHeader.biSizeImage, 1, fp);
		fclose(fp);
	}

	bool isRGB(BYTE * p, int pos, int r, int g, int b) {
		return p[pos + 2] == r && p[pos + 1] == g && p[pos] == b;
	}

	bool isRGB(BYTE * p, int pos, int rgb) {
		return isRGB(p, pos, rgb, rgb, rgb);
	}

	void setRGB(BYTE * p, int pos, int r, int g, int b) {
		p[pos + 2] = r, p[pos + 1] = g, p[pos] = b;
	}

	void setRGB(BYTE * p, int pos, int rgb) {
		setRGB(p, pos, rgb, rgb, rgb);
	}
	std::string numbers[10] = {
		"01111100111111110001111000110111110", //0
		"01100000111111101111100000000000000", //1
		"01100111100011110011111111110111011", //2
		"01001101100011110101111110110111110", //3
		"00011000011100011010011111111111111", //4
		"00000100111011111101111010111101110", //5
		"00011100011111011101101011110001110", //6
		"11000001100111110111011110001100000", //7
		"01101100111111110101111110110111110", //8
		"01100001111000110111011011000111000"  //9
	};

	int getPseudoNumber(std::string &ctx, int w, int h) {
		if(ctx.find('1') == std::string::npos) return -1;
		while (ctx.substr(0, 6).find('1') == std::string::npos) ctx = ctx.substr(6) + ctx.substr(0, 6);
		int res[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		for (unsigned int i = 0; i < ctx.length(); i++) for (int j = 0; j < 10; j++) if (ctx[i] == numbers[j][i] && ctx[i] == '1') res[j]++; else if(numbers[j][i] == '0' && ctx[i] == '1') res[j]--;
		int max = 0, maxn = -1;
		for (int i = 0; i < 10; i++) if (res[i] > max) max = res[i], maxn = i;
		return maxn;
	}

	std::vector<int> GtCaptcha(std::wstring path, HWND target) {
		std::vector<int> res;

		int base = (m_Width / 2) * 3 + m_Width * 300;
		for (int i = 0; i < (m_Height/2) * m_Width * 3; i += m_Width * 3) {
			if (isRGB(m_pLastImage, base + i, 134, 183, 202)) {
				res.push_back(i / (m_Width * 3) + 100);
				break;
			}
			else setRGB(m_pLastImage, base + i, (i / m_Width * 3) % 256);
		}
		base = m_Width * m_Height * 3 - (m_Width / 2) * 3 - m_Width * 300;
		for (int i = 0; i > (m_Height / 2) * m_Width * -3; i -= m_Width * 3) {
			if (isRGB(m_pLastImage, base + i, 134, 183, 202)) {
				res.push_back(m_Height + (i / (m_Width * 3)) - 100);
				break;
			}
			else setRGB(m_pLastImage, base + i, (i / m_Width * 3) % 256);
		}

		base = (m_Width * 3) * (m_Height / 2);
		for (int i = 150; i < (m_Width / 2) * 3; i += 3) {
			if (isRGB(m_pLastImage, base + i, 134, 183, 202)) {
				res.push_back(i / 3);
				break;
			}
			else setRGB(m_pLastImage, base + i, i % 256);
		}
		for (int i = -153; i > (m_Width / -2) * 3; i -= 3) {
			if (isRGB(m_pLastImage, base + i, 134, 183, 202)) {
				res.push_back(m_Width + (i / 3) - 1);
				break;
			}
			else setRGB(m_pLastImage, base + i, i % 256); 
		}
		if (res.size() < 4) return res;


		base = 3 * m_Width * (res[0] + (res[1] - res[0]) / 2) +res[2] * 3;
		int length = 0;
		int maxlen = 0;
		int pos = 0;
		int maxpos = base;
		for (unsigned int i = 0; i < 300; i+=3) {
			if (isRGB(m_pLastImage, base + i, 252, 230, 186)) {
				if (!length) pos = i / 3;
				length++;
				if (length > maxlen) maxpos = pos, maxlen = length;
			}
			else setRGB(m_pLastImage, base + i, 255, 0, 255), length = 0;
		}
		if (maxlen % 2 == 0) {
			maxlen += 1;
			maxpos -= 1;
		}

		if (maxlen < 10) return res;

		res.push_back(maxpos);
		res.push_back(maxlen + 4);


		base += 3 * (maxpos + maxlen / 2);

		int plusheight = 0;
		for (int i = -15; i < 15; i++) {
			if (isRGB(m_pLastImage, base + (i * m_Width * 3), 252, 230, 186) || i == 0) {
				setRGB(m_pLastImage, base + (i * m_Width * 3), 255, 255, 0);
				plusheight++;
			}
			else {
				if (i < 0) plusheight = 0;
				else break;
				setRGB(m_pLastImage, base + (i * m_Width * 3), 255, 0, 255);
			}
		}

		int charheight = int((double)plusheight * 1.6);
		int charwidth = maxlen + 4;

		if(charheight < 5 || isRGB(m_pLastImage, base - (charwidth / 3) * 6, 252, 230, 186) ||
			isRGB(m_pLastImage, base + (charwidth / 3) * 6, 252, 230, 186)) return res;

		std::string num1 = "", num2 = "", num3 = "", num4 = "";

		int subbase, subsubbase;
		for (int i = 1; i < 6; i++) {
			subbase = base - ((charheight / 2) * m_Width * 3) + 3 * (charwidth + (charwidth * i) / 6);
			for (int j = 0; j <= 6; j++) {
				subsubbase = subbase + (j * (charheight / 5)) * m_Width * 3;
				if (isRGB(m_pLastImage, subsubbase, 252, 230, 186) ||
					isRGB(m_pLastImage, subsubbase + 3, 252, 230, 186) ||
					isRGB(m_pLastImage, subsubbase - 3, 252, 230, 186) ||
					isRGB(m_pLastImage, subsubbase - 3 * m_Width, 252, 230, 186) ||
					isRGB(m_pLastImage, subsubbase + 3 * m_Width, 252, 230, 186)) {

					setRGB(m_pLastImage, subsubbase, 0, 255, 0);
					num3 += "1";
				}
				else {
					setRGB(m_pLastImage, subsubbase, 255, 0, 0);
					num3 += "0";
				}

				
			}
		}

		for (int i = 1; i < 6; i++) {
			subbase = base - ((charheight / 2) * m_Width * 3) + 3 * (2 * charwidth + (charwidth * i) / 6);
			for (int j = 0; j <= 6; j++) {
				subsubbase = subbase + (j * (charheight / 5)) * m_Width * 3;
				if (isRGB(m_pLastImage, subsubbase, 252, 230, 186) ||
					isRGB(m_pLastImage, subsubbase + 3, 252, 230, 186) ||
					isRGB(m_pLastImage, subsubbase - 3, 252, 230, 186) ||
					isRGB(m_pLastImage, subsubbase - 3 * m_Width, 252, 230, 186) ||
					isRGB(m_pLastImage, subsubbase + 3 * m_Width, 252, 230, 186)) {

					setRGB(m_pLastImage, subsubbase, 0, 255, 0);
					num4 += "1";
				}
				else {
					setRGB(m_pLastImage, subsubbase, 255, 0, 0);
					num4 += "0";
				}

			}
		}
		for (int i = 1; i < 6; i++) {
			subbase = base - ((charheight / 2) * m_Width * 3) - 3 * (charwidth + (charwidth * i) / 6);
			for (int j = 0; j <= 6; j++) {
				subsubbase = subbase + (j * (charheight / 5)) * m_Width * 3;
				if (isRGB(m_pLastImage, subsubbase, 252, 230, 186) ||
					isRGB(m_pLastImage, subsubbase + 3, 252, 230, 186) ||
					isRGB(m_pLastImage, subsubbase - 3, 252, 230, 186) ||
					isRGB(m_pLastImage, subsubbase - 3 * m_Width, 252, 230, 186) ||
					isRGB(m_pLastImage, subsubbase + 3 * m_Width, 252, 230, 186)) {

					setRGB(m_pLastImage, subsubbase, 0, 255, 0);
					num2 += "1";
				}
				else {
					setRGB(m_pLastImage, subsubbase, 255, 0, 0);
					num2 += "0";
				}

			}
		}

		for (int i = 1; i < 6; i++) {
			subbase = base - ((charheight / 2) * m_Width * 3) - 3 * (2 * charwidth + (charwidth * i) / 6);
			for (int j = 0; j <= 6; j++) {
				subsubbase = subbase + (j * (charheight / 5)) * m_Width * 3;
				if (isRGB(m_pLastImage, subsubbase, 252, 230, 186) ||
					isRGB(m_pLastImage, subsubbase + 3, 252, 230, 186) ||
					isRGB(m_pLastImage, subsubbase - 3, 252, 230, 186) ||
					isRGB(m_pLastImage, subsubbase - 3 * m_Width, 252, 230, 186) ||
					isRGB(m_pLastImage, subsubbase + 3 * m_Width, 252, 230, 186)) {

					setRGB(m_pLastImage, subsubbase, 0, 255, 0);
					num1 += "1";
				}
				else {
					setRGB(m_pLastImage, subsubbase, 255, 0, 0);
					num1 += "0";
				}

			}
		}
		std::string buf = num1;
		num1 = "";
		while (buf.length()) {
			num1 = buf.substr(0, 7) + num1;
			buf = buf.substr(7);
		}
		buf = num2;
		num2 = "";
		while (buf.length()) {
			num2 = buf.substr(0, 7) + num2;
			buf = buf.substr(7);
		}

		int d0 = getPseudoNumber(num1, 5, 7);
		int d1 = getPseudoNumber(num2, 5, 7);
		int d2 = getPseudoNumber(num3, 5, 7);
		int d3 = num4.find('1') != std::string::npos ? 0 : -1;

		cout(num1 + ": " + std::to_string(d0));
		cout(num2 + ": " + std::to_string(d1));
		cout(num3 + ": " + std::to_string(d2));
		cout(num4 + ": " + std::to_string(d3));

		if (num2.find('1') == std::string::npos || num3.find('1') == std::string::npos) return res;

		if (d0 != -1) d1 = 10 * d0 + d1;
		if (d3 != -1) d2 = 10 * d2 + d3;
		int r = d1 + d2;

		setRGB(m_pLastImage, 3 * m_Width * int(res[0] + (res[1] - res[0]) * 0.66) + (res[2] + (res[3] - res[2]) / 2) * 3, 255, 0, 0);
		setRGB(m_pLastImage, (3 * m_Width * (res[1] - 30)) + ((res[2] + 40) * 3), 255, 50, 255);

		cout(std::to_string(d1) + " + " + std::to_string(d2) + " = " + std::to_string(r));

		PostMessage(target, WM_RBUTTONDOWN, 0, MAKELPARAM(res[0] + (res[1] - res[0]) * 0.66, res[2] + (res[3] - res[2]) / 2));
		PostMessage(target, WM_RBUTTONUP, 0, MAKELPARAM(res[0] + (res[1] - res[0]) * 0.66, res[2] + (res[3] - res[2]) / 2));

		std::string lt = std::to_string(r);
		BYTE zz;
		for (unsigned int i = 0; i < lt.length(); i++) {
			zz = NumberScan(lt[i]);
			PostMessage(target, WM_KEYDOWN, LOBYTE(zz), 0);
			PostMessage(target, WM_KEYUP, LOBYTE(zz), 0);
		}

		PostMessage(target, WM_RBUTTONDOWN, 0, MAKELPARAM(res[2] + 40, res[1] - 30));
		PostMessage(target, WM_RBUTTONUP, 0, MAKELPARAM(res[2] + 40, res[1] - 30));

		SaveBmpToFile(path.c_str(), m_Width, m_Height, 24, (int*)m_pLastImage);
		return res;
	}

	void Capture(HWND target, std::wstring filename) {
		RECT r = { 0 };
		GetClientRect(target, &r);
		int Width = r.right - r.left;
		int Height = r.bottom - r.top;
		if (Width < 10 && Height < 10) return;


		HDC		hdc = GetDC(target);
		HDC		memDC = CreateCompatibleDC(hdc);
		HBITMAP memBM = CreateCompatibleBitmap(hdc, Width, Height);
		HBITMAP hOld = (HBITMAP)SelectObject(memDC, memBM);


		if (memBM == NULL) {
			cout(L"Error code: " + std::to_wstring(GetLastError()));
			return;
		}

		BOOL bRet = BitBlt(memDC, 0, 0, Width, Height, hdc, 0, 0, SRCCOPY | CAPTUREBLT);


		if (memBM != 0) {


			int Bpp = GetDeviceCaps(hdc, BITSPIXEL);
			int size = (Bpp / 8) * (Width * Height);
			BYTE *lpBits1 = new BYTE[size];
			BYTE *lpBits2 = new BYTE[Width * Height * 3];
			BYTE *lpBits3 = new BYTE[Width * Height * 3];

			BOOL Ret = TRUE;
			HBITMAP hBmp = 0;
			int MinBlackPixels = Width * Height * 10;
			int Count = 0;
			int Size24 = Width * Height * 3;
			while (Count < 5)
			{

				//Ret = PrintWindow(target, memDC, 0);
				GetBitmapBits(memBM, size, lpBits1);
				hBmp = CreateBitmap(Width, Height, 1, Bpp, lpBits1);
				Get24BitBmp(Width, Height, hBmp, lpBits2);
				int BlackPixels = 0;
				for (int i = 0; i < Size24; i += 3)
				{
					if (lpBits2[i + 2] == 0 && lpBits2[i + 1] == 0 && lpBits2[i + 0] == 0)
						BlackPixels++;
				}

				if (BlackPixels < MinBlackPixels)
				{
					MinBlackPixels = BlackPixels;
					memcpy(lpBits3, lpBits2, Size24);
					Count = 0;
				}
				Count++;
				DeleteObject(hBmp);
			}
			memcpy(lpBits2, lpBits3, Size24);

			UpdateLastImage(Width, Height, Size24, 24, lpBits2);

			//SaveBmpToFile(filename.c_str(), Width, Height, 24, (int*)lpBits2);


			delete[] lpBits1;
			delete[] lpBits2;
			delete[] lpBits3;
			DeleteObject(hBmp);
		}
		SelectObject(memDC, hOld);
		DeleteObject(memBM);
		DeleteDC(memDC);
		ReleaseDC(0, hdc);
	}
}