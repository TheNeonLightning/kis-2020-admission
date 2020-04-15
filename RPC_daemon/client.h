#include <unistd.h>
#include <sys/syscall.h>
#include <iostream>
#include <string>

using std::string;
using std::cin;
using std::cout;

// предполагается что соединение с сервером уже установлено
const int server_socket = 0;
const size_t ARGUMENT_MAX_SIZE = 4096;

long RPC_syscall(int syscall_num, int syscall_args_num) {

    write(server_socket, &syscall_num, sizeof(int));

    write(server_socket, &syscall_args_num, sizeof(int));

    char argument[ARGUMENT_MAX_SIZE];
    for (size_t index = 0; index < syscall_args_num; ++index) {
        scanf("%s", argument);
        write(server_socket, &argument, ARGUMENT_MAX_SIZE);
    }

    long syscall_result = -1;
    read(server_socket, &syscall_result, sizeof(long));

    return syscall_result;
}
