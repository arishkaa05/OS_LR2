
#include <iostream>
#include <string>
#include <windows.h>
#include <bitset>
#include <cstring>
#include <tchar.h>

using namespace std;

#define CLS system("cls")
#define DIV 1024        // Для преобразования байт в КБ
#define WIDTH 7         // Ширина поля, в котором печатаются числа.


void menu();
void computer_system_info(SYSTEM_INFO&);
void statusMemory();
void stateMemotyArea();
void reservRegion(SYSTEM_INFO&);
void reservTransfRegion(SYSTEM_INFO&);
void writeData();
void setProtection();
void changeProtection(DWORD&);
void returnMemory();


int main()
{
    SYSTEM_INFO SYSTEM_INFO;
    int c;
    menu();
    cin >> c;
    while (c) {
        switch (c)
        {
        case 0:
            cout << "\nGoodbye";
            break;
        case 1:
            cout << "\nYour choice: 1 - computer system information\n";
            computer_system_info(SYSTEM_INFO);
            break;
        case 2:
            cout << "\nYour choice: 2 - virtual memory status\n\n";
            statusMemory();
            break;
        case 3:
            cout << "\nYour choice: 3 - state of a particular memory area\n";
            stateMemotyArea();
            break;
        case 4:
            cout << "\nYour choice: 4 - reservation of the region in automatic mode and in the mode enter the address of the beginning of the region\n";
            reservRegion(SYSTEM_INFO); 
            break;
        case 5:
            cout << "\nYour choice: 5 - reserving a region and transferring physical memory to it in automatic mode and in the mode of entering the address of the beginning of the region\n";
            reservTransfRegion(SYSTEM_INFO);
            break;
        case 6:
            cout << "\nYour choice: 6 - writing data to memory cells\n";
            writeData();
            break;
        case 7:
            cout << "\nYour choice: 7 - setting access protection for a given region memory and checking\n";
            setProtection();
            break;
        case 8:
            cout << "\nYour choice: 8 - return of physical memory and release of the address region space\n";
            returnMemory();
            break;
        default:
            menu();
        }
        menu();
        cin >> c;
    }
}



void menu() {
    cout << "\n0 - exit";
    cout << "\n1 - computer system information";
    cout << "\n2 - virtual memory status";
    cout << "\n3 - state of a particular memory area";
    cout << "\n4 - reservation of the region";
    cout << "\n5 - reserving a region and transferring physical memory";
    cout << "\n6 - writing data to memory cells";
    cout << "\n7 - setting access protection for a given region memory and checking";
    cout << "\n8 - return of physical memory and release of the address region space";
    cout << "\nYour choice: ";
};

void computer_system_info(SYSTEM_INFO& SYSTEM_INFO) {
    WORD dwOemId;
   
    dwOemId = SYSTEM_INFO.wProcessorArchitecture; //Архитектура системы процессора.
    if (dwOemId == PROCESSOR_ARCHITECTURE_AMD64)
        cout << "x64 (AMD or Intel)";
    else if (dwOemId == PROCESSOR_ARCHITECTURE_IA32_ON_WIN64)
        cout << "WOW64";
    else if (dwOemId == PROCESSOR_ARCHITECTURE_IA64)
        cout << "Intel Itanium Processor Family (IPF)";
    else if (dwOemId == PROCESSOR_ARCHITECTURE_INTEL)
        cout << "x86";
    else if (dwOemId == PROCESSOR_ARCHITECTURE_UNKNOWN)
        cout << "Unknown";
    cout << "\narchitechture\n";
    cout << "Reserved for the future: " << SYSTEM_INFO.wReserved << endl; //Зарезервировано на будущее.
    cout << "Page Size : " << SYSTEM_INFO.dwPageSize << endl; //Размер страницы и гранулярность страничной защиты и обязательства. 
    cout << "Pointer to low memory address: " << SYSTEM_INFO.lpMinimumApplicationAddress << endl; //Указатель на младший адрес памяти, доступный приложениям и библиотекам динамической компоновки (DLL).
    cout << "Pointer to high memory address: " << SYSTEM_INFO.lpMaximumApplicationAddress << endl; //Указатель на старший адрес памяти, доступный приложениям и библиотекам динамической компоновки (DLL).
    cout << "The set of processors configured on the system: " << endl << std::bitset<32>(SYSTEM_INFO.dwActiveProcessorMask) << endl; //Маска, представляющая набор процессоров, сконфигурированных в системе.
    cout << "The number of processors in the system : " << SYSTEM_INFO.dwNumberOfProcessors << endl; //Количество процессоров в системе.
    cout << "Granularity for start address: "<< SYSTEM_INFO.dwAllocationGranularity << endl; //Гранулярность для начального адреса, в котором может быть выделена виртуальная память.
    cout << "Architecturally dependent system processor level: " << SYSTEM_INFO.wProcessorLevel << endl; //Уровень архитектурно-зависимого прицессора системы.
    cout << "Architecturally dependent processor revision: " << SYSTEM_INFO.wProcessorRevision << "\n\n"; //Ревизия архитектурно-зависимого процессора. 
    system("pause");
    CLS;
};

void statusMemory() {
    MEMORYSTATUSEX statex;

    statex.dwLength = sizeof(statex);

    GlobalMemoryStatusEx(&statex);

    _tprintf(TEXT("There is  %*ld percent of memory in use.\n"),
        WIDTH, statex.dwMemoryLoad);
    _tprintf(TEXT("There are %*I64d total KB of physical memory.\n"),
        WIDTH, statex.ullTotalPhys / DIV);
    _tprintf(TEXT("There are %*I64d free  KB of physical memory.\n"),
        WIDTH, statex.ullAvailPhys / DIV);
    _tprintf(TEXT("There are %*I64d total KB of paging file.\n"),
        WIDTH, statex.ullTotalPageFile / DIV);
    _tprintf(TEXT("There are %*I64d free  KB of paging file.\n"),
        WIDTH, statex.ullAvailPageFile / DIV);
    _tprintf(TEXT("There are %*I64d total KB of virtual memory.\n"),
        WIDTH, statex.ullTotalVirtual / DIV);
    _tprintf(TEXT("There are %*I64d free  KB of virtual memory.\n"),
        WIDTH, statex.ullAvailVirtual / DIV);
    _tprintf(TEXT("There are %*I64d free  KB of extended memory.\n\n"),
        WIDTH, statex.ullAvailExtendedVirtual / DIV);
    system("pause");
    CLS;
};

void stateMemotyArea() {
    LPVOID lpAddress;       //Адрес точной памяти, который необходимо получить информацию
    MEMORY_BASIC_INFORMATION lpBuffer;
    DWORD state, type;


    cout << "Enter a memory location to determine the state: ";
    cin >> std::hex >> lpAddress;
    if (VirtualQuery(               // Функция VirtualQuery позволяет запрашивать определенную информацию об участке памяти по заданному адресу. 
        lpAddress,                  // адрес региона
        &lpBuffer,                  // адрес информационного буфера
        sizeof(lpBuffer)) == 0) {   // размер буфера
        cout << "Error 0x" << GetLastError() << "\n";
        system("pause");
        return;
    }
    cout << "\nThe exact memory address to get the information: " << lpAddress << endl;
    cout << "Base address of region: " << lpBuffer.BaseAddress << endl;             // базовый адрес региона
    cout << "Allocation base address: " << lpBuffer.AllocationBase << endl;         // базовый адрес выделения
    cout << "Initial access protection: " << lpBuffer.AllocationProtect << ((lpBuffer.AllocationProtect != 0) ? "" : " (caller does not have access)") << endl; // первоначальная защита доступа
    cout << "Size, in bytes, of region" << lpBuffer.RegionSize << endl;             // размер области в байтах
    state = lpBuffer.State;         
    if (state == MEM_COMMIT)
        cout << "Committed pages\n";        // выделено
    else if (state == MEM_FREE)
        cout << "Free pages\n";             // бесплатно
    else if (state == MEM_RESERVE)
        cout << "Reserved pages\n";         // зарезервировано
    else cout << "Unknown state\n";

    cout << "Current access protection: " << lpBuffer.Protect << endl;              // текущая защита доступа

    if (lpBuffer.Protect & PAGE_EXECUTE) cout << "PAGE_EXECUTE\n";
    if (lpBuffer.Protect & PAGE_EXECUTE_READ) cout << "PAGE_EXECUTE_READ\n";
    if (lpBuffer.Protect & PAGE_EXECUTE_READWRITE) cout << "PAGE_EXECUTE_READWRITE\n";
    if (lpBuffer.Protect & PAGE_EXECUTE_WRITECOPY) cout << "PAGE_EXECUTE_WRITECOPY\n";
    if (lpBuffer.Protect & PAGE_NOACCESS) cout << "PAGE_NOACCESS\n";
    if (lpBuffer.Protect & PAGE_READONLY) cout << "PAGE_READONLY\n";
    if (lpBuffer.Protect & PAGE_READWRITE) cout << "PAGE_READWRITE\n";
    if (lpBuffer.Protect & PAGE_WRITECOPY) cout << "PAGE_WRITECOPY\n";
    if (lpBuffer.Protect & PAGE_TARGETS_INVALID) cout << "PAGE_TARGETS_INVALID\n";
    if (lpBuffer.Protect & PAGE_TARGETS_NO_UPDATE) cout << "PAGE_TARGETS_NO_UPDATE\n";
    cout << endl;
    cout << "Type of pages\n";
    type = lpBuffer.Type;
    if (type == MEM_IMAGE)
        cout << "The memory pages within: image section\n";
    else if (type == MEM_MAPPED)
        cout << "The memory pages within: view of a section\n";
    else if (type == MEM_PRIVATE)
        cout << "The memory pages within the region are private\n";
    else cout << "Unknown type\n";
    cout << "\n";
    system("pause");
    CLS;
};

void reservRegion(SYSTEM_INFO& SYSTEM_INFO) {
    int c;
    void* address = nullptr;

    cout << "\n1 - Region reservation in automatic mode\n2 - Region reservation in mode enter the address of the beginning of the region";
    cout << "\nYour choice: ";
    cin >> c;
    if (c == 1 || c == 2) {
        if (c == 2) {
            cout << "Enter a memory location to determine the state: ";
            cin >> address;
        }

        address = VirtualAlloc(         // С помощью данной функции есть возможность выделить или зарезервировать страницы виртуальной памяти. 
            address,                    // базовые адрес 
            SYSTEM_INFO.dwPageSize,     // размер
            MEM_RESERVE,                // способ получения
            PAGE_READWRITE);            // тип доступа
        if (address) {
            if (address = VirtualAlloc(address, SYSTEM_INFO.dwPageSize, MEM_COMMIT, PAGE_READWRITE))
                cout << "Memory area allocated\nAddress: " << address << endl;
            else cout << "Error 0x" << GetLastError() << endl << "Memory hasn't commited";
        }
        else cout << "Error 0x" << GetLastError() << endl << "Memory hasn't reserved\n";

        system("pause");
        CLS;
    }
    else
        cout << "An error has occurred! Try again.\n";
}

void reservTransfRegion(SYSTEM_INFO& SYSTEM_INFO) {
    int c;
    void* address = nullptr;

    cout << "\n1 - Region reservation in automatic mode\n2 - Region reservation in mode enter the address of the beginning of the region";
    cout << "\nYour choice: ";
    cin >> c;
    if (c == 1 || c == 2)   {
        if (c == 2) {
            cout << "Enter a memory location to determine the state: ";
            cin >> address;
        }
        if (address = VirtualAlloc(address, SYSTEM_INFO.dwPageSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE))
            cout << "Memory area allocated\nAddress: " << address << endl;
        else cout << "Error 0x" << GetLastError() << endl << "Memory hasn't allocated\n";

        system("pause");
        CLS;
    }
    else 
        cout << "An error has occurred! Try again.\n";
    
};

void writeData() {
    std::string source;
    LPVOID address = NULL; //not const
    MEMORY_BASIC_INFORMATION info;
    size_t addr_t;

    cout << "Enter data for input: ";
    std::getline(cin, source);
    cin.ignore(INT_MAX, '\n');
    cout << "Enter the input address: ";
    cin >> addr_t;
    address = (LPVOID)addr_t;

    if (!VirtualQuery(address, &info, 256)) {
        cout << "An error has occurred! Try again.\n" << GetLastError() << "\n";
        system("pause");
        CLS;
        return;
    }

    if (info.Protect && (PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY | PAGE_READWRITE | PAGE_WRITECOPY)) {
        char* destination = (char*)address;
        CopyMemory(destination, source.c_str(), source.length() * sizeof(char));
        cout << "Memory area " << address << " filled\n";
    }
    else cout << "An error has occurred! Try again.\n";

    system("pause");
    CLS;
}

void setProtection() {
    DWORD* dwPointer;
    dwPointer = new DWORD;
    DWORD old;

    cout << "Enter the address: ";
    cin >> *dwPointer;
    if (!dwPointer) {
        cout << "Address is null\n";
        system("pause");
        return;
    }
    cout << "Choose new protection level:\n";
    changeProtection(old);

    if (VirtualProtect(     // Эта функция устанавливает атрибуты на блоки памяти.
        dwPointer,          // адрес региона для установки флага
        sizeof(DWORD),      // размер региона
        old,                // флаг
        &old)) {            // адрес для сохранения старых флагов
        cout << "Old protection level:\n";
        if (old & PAGE_EXECUTE) cout << "PAGE_EXECUTE\n";
        if (old & PAGE_EXECUTE_READ) cout << "PAGE_EXECUTE_READ\n";
        if (old & PAGE_EXECUTE_READWRITE) cout << "PAGE_EXECUTE_READWRITE\n";
        if (old & PAGE_EXECUTE_WRITECOPY) cout << "PAGE_EXECUTE_WRITECOPY\n";
        if (old & PAGE_NOACCESS) cout << "PAGE_NOACCESS\n";
        if (old & PAGE_READONLY) cout << "PAGE_READONLY\n";
        if (old & PAGE_READWRITE) cout << "PAGE_READWRITE\n";
        if (old & PAGE_WRITECOPY) cout << "PAGE_WRITECOPY\n";
        if (old & PAGE_TARGETS_INVALID) cout << "PAGE_TARGETS_INVALID\n";
        if (old & PAGE_TARGETS_NO_UPDATE) cout << "PAGE_TARGETS_NO_UPDATE\n";
    }
    else cout << "Error" << GetLastError() << "\n Try again.\n";

    system("pause");
    CLS;
}

void changeProtection(DWORD& newLevel) {
    newLevel = -1;
    cout << "1 - PAGE_EXECUTE\n";
    cout << "2 - PAGE_EXECUTE_READ\n";
    cout << "3 - PAGE_EXECUTE_READWRITE\n";
    cout << "4 - PAGE_EXECUTE_WRITECOPY\n";
    cout << "5 - PAGE_NOACCESS\n";
    cout << "6 - PAGE_READONLY\n";
    cout << "7 - PAGE_READWRITE\n";
    cout << "8 - PAGE_WRITECOPY\n";
    cout << "9 - PAGE_TARGETS_INVALID\n";
    cout << "10- PAGE_TARGETS_NO_UPDATE\n";
    cout << "Enter your choice: ";
    cin >> newLevel;

    if (newLevel == 1) newLevel = PAGE_EXECUTE;
    else if (newLevel == 2) newLevel = PAGE_EXECUTE_READ;
    else if (newLevel == 3) newLevel = PAGE_EXECUTE_READWRITE;
    else if (newLevel == 4) newLevel = PAGE_EXECUTE_WRITECOPY;
    else if (newLevel == 5) newLevel = PAGE_NOACCESS;
    else if (newLevel == 6) newLevel = PAGE_READONLY;
    else if (newLevel == 7) newLevel = PAGE_READWRITE;
    else if (newLevel == 8) newLevel = PAGE_WRITECOPY;
    else if (newLevel == 9) newLevel = PAGE_TARGETS_INVALID;
    else newLevel = PAGE_TARGETS_NO_UPDATE;
}

void returnMemory() {       
    LPVOID address = NULL;
    cout << "Enter the memory location to free:";
    cin >> address;

    if (VirtualFree(    // Функция VirtualFree освобождает выделенную память.
        address,        // адресс памяти   
        0,              // рамер памяти
        MEM_RELEASE))   // операция
        cout << "Address region freed space\n";
    else std::cerr << "An error has occurred! Try again.\n" << GetLastError() << endl;

    system("pause");
    CLS;
};