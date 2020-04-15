// Задача C004
//
// Реализуйте многопоточный RPC-демон и библиотеку RPC-клиента поверх системных
// вызовов.

#include <cstdlib>

#include "server.h"

int main(int argc, char* argv[]) {

    uint16_t port_number = strtol(argv[1], nullptr, 10);

    Server RPCdaemon(port_number);
    RPCdaemon.Run();

    return 0;
}