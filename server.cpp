#include "util.cpp"

int main(int argc , char** argv){
    bool flag;
    int option = true, main_socket, addr_length, new_socket;
    int child_socket[MAX_NUM_OF_CLIENTS] = {0};
    int fd_max, port = atoi(argv[1]), space_pos;
    char** buffer = new char*[MAX_NUM_OF_CLIENTS];
    for(int i=0; i<MAX_NUM_OF_CLIENTS; i++){
        buffer[i] = new char[BUF_SIZE];
    }
    string operation, command, argument, username[MAX_NUM_OF_CLIENTS] = {""};
    fd_set readfds;

    create_directory("server_dir");
         
    main_socket = socket(DOMAIN, TYPE, PROTOCOL);
     
    setsockopt(main_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&option, sizeof(option));
     
    struct sockaddr_in address = destination_setup(0, 0, port);
    addr_length = sizeof(address);
         
    bind(main_socket, (struct sockaddr *)&address, addr_length);
         
    listen(main_socket, 3);

    while(true){
        fd_max = FD_setup(main_socket, child_socket, readfds);
     
        select(fd_max + 1, &readfds, NULL, NULL, 0);

        check_new_client(main_socket, child_socket, readfds, address, addr_length);

        for(int i=0; i<MAX_NUM_OF_CLIENTS; i++){
            if(FD_ISSET(child_socket[i], &readfds)){
                memset(buffer[i], '\0', BUF_SIZE);

                if(recv(child_socket[i], buffer[i], BUF_SIZE, 0) <= 0){
                    close(child_socket[i]);
                    child_socket[i] = 0;
                    username[i] = "";
                }
                else{
                    operation = buffer[i];
                    // cout << "operation: " << operation << endl;
                    space_pos = operation.find(" ");
                    command = operation.substr(0, space_pos);
                    // cout << "command: " << command << endl;

                    if(command == "usr"){
                        argument = operation.substr(space_pos + 1);
                        // cout << "argument: " << argument << endl;
                        flag = true;
                        for(int j=0; j<MAX_NUM_OF_CLIENTS; j++){
                            if(strcmp(username[j].c_str(), buffer[i]) == 0){
                                send(child_socket[i], "0", BUF_SIZE, 0);
                                flag = false;
                                break;
                            }
                        }
                        if(flag){
                            username[i] = buffer[i];
                            send(child_socket[i], "1", BUF_SIZE, 0);
                        }
                    }
                    else if(operation == "ls"){
                        if(space_pos != string::npos){
                            cout << "Command format error" << endl;
                            continue;
                        }
                        for (const auto & entry : directory_iterator("server_dir")){
                            std::cout << entry.path() << std::endl;
                        }
                    }
                    else if(operation == "put"){

                    }
                    else if(operation == "get"){

                    }
                    else{
                        cout << "Command not found" << endl;
                    }
                }
            }  
        }
    }

    for(int i=0; i<MAX_NUM_OF_CLIENTS; i++){
        delete[] buffer[i];
    }
    delete[] buffer;

    return 0;  
}  