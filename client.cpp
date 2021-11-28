#include "util.cpp"

int main(int argc, char** argv){
/* declaration */
    char buffer[BUF_SIZE];
    char *ip_address;
    const char* delimiter = ":";

    int addr_length, port, client_socket, space_pos, bytes_read;
    int received_bytes;
    unsigned long long int size_of_file, current_bytes;

    string operation, argument, command, dir_name = "client_dir";

    fstream file;

/* pre-processing */
    create_directory(dir_name);
    
    client_socket = socket(DOMAIN, TYPE, PROTOCOL);

    ip_address = strtok(argv[1], delimiter);
    port = atoi(strtok(NULL, delimiter));

    struct sockaddr_in address = destination_setup(1, ip_address, port);
    addr_length = sizeof(address);

    connect(client_socket, (struct sockaddr*)&address, addr_length);

/* setup username */
    cout << "input your username:" << endl;

    while(true){
		clear_buffer(buffer);

        getline(cin, argument);
        // cout << "argument: " << argument << endl;

        strcat(buffer, "usr");
        send(client_socket, buffer, BUF_SIZE, 0);
        send(client_socket, argument.c_str(), BUF_SIZE, 0);

        recv(client_socket, buffer, BUF_SIZE, 0);
        // cout << "buffer in usr: " << buffer << endl;

        if(buffer[0] == '0'){
            cout << "username is in used, please try another:" << endl;
        }
        else if(buffer[0] == '1'){
            cout << "connect successfully" << endl;
            break;
        }
        // else{
        //     cout << "error" << endl;
        // }
    }

/* execute operations */
    while(true){
		clear_buffer(buffer);
        
        getline(cin, operation);
        // cout << "operation: " << operation << endl;

        space_pos = operation.find(" ");
        command = operation.substr(0, space_pos);
        // cout << "command: " << command << endl;

        if(command == "ls"){
            if(space_pos == string::npos){
                // cout << "Hi, I'm ls" << endl;
                send(client_socket, command.c_str(), BUF_SIZE, 0);
                recv(client_socket, buffer, BUF_SIZE, 0);
                cout << buffer;
            }
            else{
                cout << "Command format error" << endl;
            }
        }
        else if(command == "put"){
            argument = operation.substr(space_pos + 1);
            // cout << "argument: " << argument << endl;
            space_pos = argument.find(" ");

            if(space_pos == string::npos){
                // cout << "Hi, I'm put" << endl;
                file.open(dir_name + "/" + argument, fstream::in|fstream::binary);

                if(!file.fail()){
                    while(file.peek() != EOF){
                        send(client_socket, command.c_str(), BUF_SIZE, 0);
                        send(client_socket, argument.c_str(), BUF_SIZE, 0);
                        file.read(buffer, BUF_SIZE);
                        // cout << buffer << endl;
                        // cout << "gcount: " << file.gcount() << endl;
                        bytes_read = file.gcount();

                        send(client_socket, to_string(bytes_read).c_str(), BUF_SIZE, 0);
                        send(client_socket, buffer, BUF_SIZE, 0);

                        clear_buffer(buffer);
                    }
                    file.close();
                    
                    strcat(buffer, "eof");
                    send(client_socket, buffer, BUF_SIZE, 0);
                    clear_buffer(buffer);
                    send(client_socket, argument.c_str(), BUF_SIZE, 0);
                    recv(client_socket, buffer, BUF_SIZE, 0);
                    cout << buffer << endl;
                }
                else{
                    cout << "The " << argument << " doesn’t exist" << endl;
                }
            }
            else{
                cout << "Command format error" << endl;
            }
        }
        else if(command == "get"){
            argument = operation.substr(space_pos + 1);
            // cout << "argument: " << argument << endl;
            space_pos = argument.find(" ");

            if(space_pos == string::npos){
                // cout << "Hi, I'm get" << endl;
                strcat(buffer, "get_req");
                send(client_socket, buffer, BUF_SIZE, 0);
                send(client_socket, argument.c_str(), BUF_SIZE, 0);
                // cout << "argument: " << argument << endl;
                clear_buffer(buffer);
                recv(client_socket, buffer, BUF_SIZE, 0);
                // cout << "buffer after get_req: " << buffer << endl;

                if(buffer[0] == '1'){
                    recv(client_socket, buffer, BUF_SIZE, 0);
                    // cout << "size of file: " << buffer << endl;
                    size_of_file = atoi(buffer);
                    clear_buffer(buffer);

                    file.open(dir_name + "/" + argument, fstream::out|fstream::binary|fstream::app);

                    current_bytes = 0;
                    while(current_bytes != size_of_file){
                        // cout << "current bytes: " << current_bytes << endl;
                        send(client_socket, command.c_str(), BUF_SIZE, 0);
                        send(client_socket, argument.c_str(), BUF_SIZE, 0);

                        received_bytes = recv(client_socket, buffer, BUF_SIZE, 0);
                        // cout << buffer << endl;
                        // cout << "received_bytes: " << received_bytes << endl;
                        current_bytes += received_bytes;
                        file.write(buffer, received_bytes);
                        clear_buffer(buffer);
                    }
                    file.close();

                    strcat(buffer, "get_fin");
                    send(client_socket, buffer, BUF_SIZE, 0);
                    cout << "get " << argument << " successfully" << endl;
                }
                else{
                    cout << "The " << argument << " doesn’t exist" << endl;
                }
            }
            else{
                cout << "Command format error" << endl;
            }
        }
        else{
            cout << "Command not found" << endl;
        }
    }

    close(client_socket);

    return 0;
}