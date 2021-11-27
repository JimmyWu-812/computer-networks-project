#include "util.cpp"
#include <iostream>
#include <fstream>

int main(int argc, char** argv){
    /* declaration */
    char buffer[BUF_SIZE], ch;
    char *ip_address;
    const char* delimiter = ":";
    int addr_length, i, port, client_socket, space_pos;
    fstream file;
    string operation, username, command;

    /* pre-processing */
    create_directory("client_dir");
    
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

        getline(cin, username);
        send(client_socket, "usr", BUF_SIZE, 0);
        send(client_socket, username.c_str(), BUF_SIZE, 0);

        recv(client_socket, buffer, BUF_SIZE, 0);
        // cout << "buffer in usr: " << buffer << endl;

        if(buffer[0] == '0'){
            cout << "username is in used, please try another:" << endl;
        }
        else if(buffer[0] == '1'){
            cout << "connect successfully" << endl;
            break;
        }
    }

    /* execute operations */
    while(true){
		clear_buffer(buffer);
        
        getline(cin, operation);
        // cout << "operation: " << operation << endl;

        space_pos = operation.find(" ");
        command = operation.substr(0, space_pos);
        // cout << "command: " << command << endl;
        
        send(client_socket, command.c_str(), command.size(), 0);

        if(command == "ls"){
            if(space_pos == string::npos){
                // cout << "I'm ls" << endl;
                recv(client_socket, buffer, BUF_SIZE, 0);
                cout << buffer;
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