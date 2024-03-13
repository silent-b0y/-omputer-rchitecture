#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

void main()
{
  setlocale(LC_ALL, "rus");
  BITMAPFILEHEADER bmpFileHeader;
  BITMAPINFOHEADER bmpInfoHeader;
  int Width, Height;
  RGBQUAD Palette[256];
  SHORT* inBuf;
  RGBTRIPLE* outBuf;
  HANDLE hInputFile, hOutFile;
  DWORD RW;

  hInputFile = CreateFile(L"test.bmp", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
  if (hInputFile == INVALID_HANDLE_VALUE) return;
  hOutFile = CreateFile(L"Result.bmp", GENERIC_WRITE, 0, NULL, CREATE_NEW, 0, NULL);
  if (hOutFile == INVALID_HANDLE_VALUE)
  {
    CloseHandle(hInputFile);
    return;
  }

  // Считываем инфу
  ReadFile(hInputFile, &bmpFileHeader, sizeof(bmpFileHeader), &RW, NULL);
  ReadFile(hInputFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &RW, NULL);

  // Установим указатель на начало растра
  SetFilePointer(hInputFile, bmpFileHeader.bfOffBits, NULL, FILE_BEGIN);
  Width = bmpInfoHeader.biWidth;
  Height = bmpInfoHeader.biHeight;

  // Выделим память
  inBuf = new SHORT[Width];
  outBuf = new RGBTRIPLE[Width];
  cout << "Разрешение изображения: ";
  cout << Width << "x" << Height << "\n";
  cout << "Количество битов на пиксел: " << bmpInfoHeader.biBitCount << "\n";
  cout << "Размер изображения: " << bmpFileHeader.bfSize << " байт\n";

  // Заполним заголовки
  bmpFileHeader.bfOffBits = sizeof(bmpFileHeader) + sizeof(bmpInfoHeader) + 1024;
  bmpInfoHeader.biBitCount = 24;
  bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + Width * Height * 3 + Height * (3 * Width % 4);

  // Запишем заголовки
  WriteFile(hOutFile, &bmpFileHeader, sizeof(bmpFileHeader), &RW, NULL);
  WriteFile(hOutFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &RW, NULL);

  // Палитра
  WriteFile(hOutFile, Palette, 256 * sizeof(RGBQUAD), &RW, NULL);

  // Начнем преобразовывать
  for (int i = 0; i < Height; i++)
  {
    ReadFile(hInputFile, inBuf, sizeof(SHORT) * Width, &RW, NULL);
    for (int j = 0; j < Width; j++)
    {
      outBuf[j].rgbtRed = ((inBuf[j] & 0x7C00) / 1024) * 8;
      outBuf[j].rgbtGreen = ((inBuf[j] & 0x03E0) / 32) * 8;
      outBuf[j].rgbtBlue = (inBuf[j] & 0x001F) * 8;
    }
    WriteFile(hOutFile, outBuf, sizeof(RGBTRIPLE) * Width, &RW, NULL);

    // Пишем мусор для выравнивания
    WriteFile(hOutFile, Palette, (3 * Width) % 4, &RW, NULL);
    SetFilePointer(hInputFile, Width % 4, NULL, FILE_CURRENT);
  }

  delete[] inBuf;
  delete[] outBuf;
  CloseHandle(hInputFile);
  CloseHandle(hOutFile);

  cout << "Updating has come to the end successfully!\n";
  system("pause");
}
