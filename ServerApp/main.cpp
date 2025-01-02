#include <Server.h>

int main(int argc, char* argv[])
{
    Server server("localhost", "2525");
    server.Run();


    return 0;
}
