
#include <iostream>
#include <windows.h>
#include <string>

using std::cout;
using std::wcin;
using std::cin;
using std::endl;

int main() {
    HANDLE file = NULL, map = NULL;
    LPVOID lpFileMap = NULL;
    std::string data;
    wchar_t lpFileName[250], fileMap[250];

    cout << "Enter file name: ";
    wcin.getline(lpFileName, MAX_PATH, L'\n');

    file = CreateFile(
        lpFileName,                     // имя файла
        GENERIC_WRITE | GENERIC_READ,   // режим доступа
        0,                              // совместный доступ
        NULL,                           // SD (дескр. защиты)
        CREATE_ALWAYS,                  // как действовать
        FILE_ATTRIBUTE_NORMAL,          // атрибуты файла
        NULL);                          // дескр.шаблона файла

    if (file == INVALID_HANDLE_VALUE) cout << "Error 0x" << GetLastError() << endl;
    else {
        cout << "Enter file mapping name: ";
        wcin.getline(fileMap, MAX_PATH, L'\n');
        map = CreateFileMapping(file, NULL, PAGE_READWRITE, 0, 128, (LPCTSTR)&fileMap[0]);
        lpFileMap = MapViewOfFile(      // Функция MapViewOfFile отображает представление проецируемого файла в адресное пространство вызывающего процесса.
            map,                        // дескр. объекта проецируемый файл
            FILE_MAP_ALL_ACCESS,        // режим доступа
            0,                          // старшее DWORD смещения
            0,                          // младшее DWORD смещения
            0);                         // число отображаемых байтов
        if (lpFileMap != NULL)
        {
            cout << "\n\nFile projected successfully.\n";
            cout << "Projection's address: " << lpFileMap << endl;
        }
        else cout << "File projection crushed :(\n";

        cout << "Enter data for input: \n> ";
        getchar();
        std::getline(cin, data);

        CopyMemory(lpFileMap, data.c_str(), data.length() * sizeof(char)); // CopyMemory копирует блок памяти из одного места в другой с использованием указателей

        cout << "Data entered.\nNow open reader.exe\nDon't close writer-file\n";
        system("pause");
    }

    if (lpFileMap != NULL && file != NULL) {
        UnmapViewOfFile(lpFileMap);     // Функция UnmapViewOfFile отменяет отображение  представления файла из адресного пространства вызывающего процесса.
        CloseHandle(file);              // Функция CloseHandle закрывает дескриптор открытого объекта.
    }
}