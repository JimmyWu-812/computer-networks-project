#include "util.cpp"

int main(int argc , char** argv){
    int option = true, main_socket, addr_length, new_socket;
    int child_socket[MAX_NUM_OF_CLIENTS] = {0};
    int fd, fd_max, port = atoi(argv[1]);
    char buffer[BUF_SIZE];  
    fd_set readfds;
         
    main_socket = socket(DOMAIN, TYPE, PROTOCOL);
     
    setsockopt(main_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&option, sizeof(option));
     
    struct sockaddr_in address = destination_setup(0, 0, port);
    addr_length = sizeof(address);
         
    bind(main_socket, (struct sockaddr *)&address, addr_length);
         
    listen(main_socket, 3);

    while(true){
        fd_max = FD_setup(main_socket, child_socket, readfds);
     
        select(fd_max + 1, &readfds, NULL, NULL, 0);

        check_new_connection(main_socket, child_socket, readfds, address, addr_length);

        receive_command(child_socket, readfds, buffer);

        handle_put();
    }
    return 0;  
}  