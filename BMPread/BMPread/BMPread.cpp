#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

class BMPImage 
{
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    unsigned char* pixelData;
    ifstream inputFile;

public:
    bool openBMP(const string& fileName) 
    {
        if (!ifstream(fileName)) 
        {
            cerr << "The file does not exist " << endl;
            return false;
        }

        inputFile.open(fileName, ios::binary);
        if (!inputFile.is_open()) 
        {
            cerr << "File opening error: " << fileName << endl;
            return false;
        }
        inputFile.read((char*)&fileHeader, sizeof(BITMAPFILEHEADER));
        inputFile.read((char*)&infoHeader, sizeof(BITMAPINFOHEADER));

        // Проверка на тип BMP файла
        if (fileHeader.bfType != 0x4D42) 
        {
            cerr << "Invalid file format (not bmp): " << fileName << endl;
            return false;
        }

        // Проверка на 24/32-битный формат
        if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32) 
        {
            cerr << "Invalid file format (need only 24/32 bytes): " << fileName << endl;
            return false;
        }

        // Выделение памяти для пиксельных данных
        pixelData = new unsigned char[infoHeader.biSizeImage];
        if (!pixelData) 
        {
            cerr << "Memory allocation error" << endl;
            return false;
        }

        // Чтение пиксельных данных
        inputFile.seekg(fileHeader.bfOffBits, ios::beg);
        inputFile.read((char*)pixelData, infoHeader.biSizeImage);

        return true;
    }

    void displayBMP() 
    {
        int width = infoHeader.biWidth;
        int height = infoHeader.biHeight;
        int bytesPerPixel = infoHeader.biBitCount / 8;

        // Цикл по каждой строке изображения (в обратном порядке)
        for (int y = height - 1; y >= 0; --y) {
            // Цикл по каждому пикселю в строке
            for (int x = 0; x < width; ++x) {
                // Вычисление позиции пикселя в массиве данных
                int pixelOffset = (y * width + x) * bytesPerPixel;

                // Проверка цвета пикселя
                if (pixelData[pixelOffset] == 0 && pixelData[pixelOffset + 1] == 0 && pixelData[pixelOffset + 2] == 0) {
                    cout << "#"; // Черный цвет
                }
                else {
                    cout << "O"; // Белый цвет (фон)
                }
            }
            cout << endl;
        }
    }

    // Закрытие BMP файла
    void closeBMP() 
    {
        if (inputFile.is_open()) 
        {
            inputFile.close();
        }

        if (pixelData) 
        {
            delete[] pixelData;
            pixelData = nullptr;
        }
    }
};

int main() 
{
    BMPImage bmp;
    const string path = "BMP_ex2.bmp";

    if (bmp.openBMP(path))
    {
        bmp.displayBMP();
        bmp.closeBMP();
    }

    return 0;
}
