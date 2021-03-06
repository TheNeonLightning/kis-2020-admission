#include <string>
#include <cstdio>
#include <cstdint>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

using std::string;

const size_t MAX_PENDING_CALLS = SOMAXCONN;
const size_t MAX_ARGUMENT_SIZE = 4096;

class Server {
public:

    Server(uint16_t port_number): port_number(port_number) {
        server_socket = socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in address = {
                .sin_family = AF_INET,
                .sin_port = htons(port_number),
                .sin_addr = inet_addr("0.0.0.0")
        };

        bind(server_socket, (const struct sockaddr*) &address, sizeof(address));
    }



    void ProcessCall(struct epoll_event* rp_call) {

        int fd = rp_call->data.fd;

        if (fd == server_socket) {

            int new_fd = accept(server_socket, nullptr, nullptr);

            AddConnection(new_fd);

        } else {

            int syscall_num = -1, syscall_args_num = -1;
            long syscall_result = -1;

            read(fd, &syscall_num, sizeof(int));
            read(fd, &syscall_args_num, sizeof(int));

             char arguments[syscall_args_num][MAX_ARGUMENT_SIZE];


            for (size_t index = 0; index < syscall_args_num; ++index) {
                read(fd, &arguments[index], MAX_ARGUMENT_SIZE);
            }



            switch (syscall_args_num) {
                case 0:
                    syscall_result = syscall(syscall_num);
                    break;
                case 1:
                    syscall_result = syscall(syscall_num, arguments[0]);
                    break;
                case 2:
                    syscall_result = syscall(syscall_num, arguments[0], arguments[1]);
                    break;
                case 3:
                    syscall_result = syscall(syscall_num, arguments[0], arguments[1], arguments[2]);
                    break;
                case 4:
                    syscall_result = syscall(syscall_num, arguments[0], arguments[1], arguments[2], arguments[3]);
                    break;
            }

            write(fd, &syscall_result, sizeof(long));

        }
    }

    [[noreturn]] void Run() {
        listen(server_socket, SOMAXCONN);

        epoll_fd = epoll_create1(0);

        AddConnection(server_socket);

        struct epoll_event pending_calls[MAX_PENDING_CALLS];

        while (true) {

            int pending_calls_number = epoll_wait(epoll_fd, pending_calls, MAX_PENDING_CALLS, -1);
            for (size_t index = 0; index < pending_calls_number; ++index) {
                ProcessCall(&pending_calls[index]);
            }

        }

    }

    void AddConnection(int fd) {

        int flags = fcntl(fd, F_GETFL);
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);

        struct epoll_event rp_call;
        rp_call.events = EPOLLIN;
        rp_call.data.fd = fd;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &rp_call);

    }

private:
    uint16_t port_number = 0;
    int server_socket = 0;
    int epoll_fd = 0;
};