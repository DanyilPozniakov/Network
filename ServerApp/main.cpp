#include <iostream>
#include <Server.h>
#include "windows.h"
#include <ctime>


#include <iostream>



int main(int argc, char* argv[])
{

#ifndef _DEBUG
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Error: GetStdHandle(STD_OUTPUT_HANDLE) returned INVALID_HANDLE_VALUE\n";
        return 1;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        std::cerr << "Error: GetConsoleMode() failed\n";
        return 1;
    }

    // Добавляем флаг ENABLE_VIRTUAL_TERMINAL_PROCESSING
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        std::cerr << "Error: SetConsoleMode() failed\n";
        return 1;
    }
#endif


    Server server("localhost", "2525");
    server.Run();


    return 0;
}
