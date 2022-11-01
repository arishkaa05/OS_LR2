
#include <iostream>
#include <windows.h>

using std::cout;
using std::wcin;
using std::cin;
using std::endl;

int main() {
    wchar_t lpName[250];
    HANDLE map = NULL;
    LPVOID lpFileMap = NULL;

    cout << "Enter the projected file name: ";
    wcin.getline(lpName, 250, L'\n');

    map = OpenFileMapping(              //Функция OpenFileMapping открывает именованный объект "проецируемый файл"
        FILE_MAP_READ | FILE_MAP_WRITE, // режим доступа
        FALSE,                          // флажок наследования
        (LPCTSTR)lpName);               // имя объекта
    if (map == INVALID_HANDLE_VALUE) cout << "An error has occurred! Try again.\n";
    else {
        lpFileMap = MapViewOfFile(  //Функция MapViewOfFile отображает представление проецируемого файла в адресное пространство вызывающего процесса.
            map,                    // дескр. объекта проецируемый файл
            FILE_MAP_ALL_ACCESS,    // режим доступа
            0,                      // старшее DWORD смещения
            0,                      // младшее DWORD смещения
            0);                     // число отображаемых байтов
        if (!lpFileMap) {
            cout << "There's nothing in file projection\n";
            system("pause");
            return 2;
        }
        cout << "Address: " << lpFileMap << "\nProjection's data: \n";
        cout << (char*)lpFileMap << endl;
        system("pause");
    }
    if (!UnmapViewOfFile(lpFileMap)) cout << "Unable to ummap view of file\n";  // Функция UnmapViewOfFile отменяет отображение  представления файла из адресного пространства вызывающего процесса.
    if (!CloseHandle(map)) cout << "Unable to close map file\n";                //Функция CloseHandle закрывает дескриптор открытого объекта.
    delete[] lpName;
}