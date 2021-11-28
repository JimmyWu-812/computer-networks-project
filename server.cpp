#include "util.cpp"
#include <set>

int main(int argc , char** argv){
    /* declaration */
    bool flag;

    int option = true, main_socket, addr_length, new_socket;
    int child_socket[MAX_NUM_OF_CLIENTS] = {0};
    int fd_max, port = atoi(argv[1]), bytes_read;

    char buffer[BUF_SIZE];

    string command, argument, username[MAX_NUM_OF_CLIENTS] = {""};
    string dir_name = "server_dir";

    fstream file;

    set<path> filenames;

    const path root_dir{dir_name};
    fd_set readfds;

    /* pre-processing */
    create_directory(dir_name);
         
    main_socket = socket(DOMAIN, TYPE, PROTOCOL);
     
    setsockopt(main_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&option, sizeof(option));
     
    struct sockaddr_in address = destination_setup(0, 0, port);
    addr_length = sizeof(address);
         
    bind(main_socket, (struct sockaddr *)&address, addr_length);
         
    listen(main_socket, MAX_NUM_OF_CLIENTS);

    while(true){
        fd_max = FD_setup(main_socket, child_socket, readfds);
     
        select(fd_max + 1, &readfds, NULL, NULL, NULL);

        if(FD_ISSET(main_socket, &readfds)){  
            new_socket = accept(main_socket, (struct sockaddr*)&address, (socklen_t*)&addr_length);
            // cout << "new_socket: " << new_socket << endl;
            for(int i=0; i<MAX_NUM_OF_CLIENTS; i++){  
                if(child_socket[i] == 0){  
                    child_socket[i] = new_socket;  
                    break;  
                }  
            }
        }

        for(int i=0; i<MAX_NUM_OF_CLIENTS; i++){
            filenames.clear();
            for(const auto& entry : directory_iterator(root_dir)){
                // cout << entry.path().filename() << endl;
                filenames.insert(entry.path().filename());
            }
            
            if(FD_ISSET(child_socket[i], &readfds)){
                clear_buffer(buffer);

                if(recv(child_socket[i], buffer, BUF_SIZE, 0) <= 0){
                    close(child_socket[i]);
                    child_socket[i] = 0;
                    username[i] = "";
                }
                else{
                    command = buffer;
                    clear_buffer(buffer);
                    // cout << "command: " << command << endl;

                    if(command == "usr"){
                        recv(child_socket[i], buffer, BUF_SIZE, 0);
                        // cout << "buffer in usr: " << buffer << endl;
                        argument = buffer;
                        clear_buffer(buffer);

                        flag = true;
                        for(int j=0; j<MAX_NUM_OF_CLIENTS; j++){
                            if(username[j] == argument){
                                strcat(buffer, "0");
                                send(child_socket[i], buffer, BUF_SIZE, 0);
                                flag = false;
                                break;
                            }
                        }

                        if(flag){
                            username[i] = argument;
                            // cout << "connect successfully: ";
                            // cout << child_socket[i] << " " << username[i] << endl;
                            strcat(buffer, "1");
                            send(child_socket[i], buffer, BUF_SIZE, 0);
                        }
                    }
                    else if(command == "ls"){
                        for (auto it=filenames.begin(); it!=filenames.end(); ++it){
                            strcat(buffer, it->c_str());
                            strcat(buffer, "\n");
                        }
                        // cout << "buffer in ls: " << buffer;

                        send(child_socket[i], buffer, BUF_SIZE, 0);
                    }
                    else if(command == "put"){
                        recv(child_socket[i], buffer, BUF_SIZE, 0);
                        // cout << "buffer: " << buffer << endl;
                        argument = buffer;
                        // cout << "argument: " << argument << endl;

                        clear_buffer(buffer);

                        recv(child_socket[i], buffer, BUF_SIZE, 0);
                        // cout << "buffer: " << buffer << endl;
                        bytes_read = atoi(buffer);
                        // cout << "bytes_read: " << bytes_read << endl;

                        clear_buffer(buffer);

                        file.open(dir_name + "/" + argument, fstream::out|fstream::binary|fstream::app);
                        recv(child_socket[i], buffer, BUF_SIZE, 0);
                        // cout << buffer << endl;
                        file.write(buffer, bytes_read);

                        file.close();
                    }
                    else if(command == "eof"){
                        recv(child_socket[i], buffer, BUF_SIZE, 0);
                        argument = buffer;
                        argument = "put " + argument + " successfully";
                        send(child_socket[i], argument.c_str(), BUF_SIZE, 0);
                    }
                    else if(command == "get"){

                    }
                }
            }  
        }
    }

    return 0;  
}  