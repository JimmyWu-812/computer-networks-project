#include "util.cpp"

int main(int argc, char** argv){
    char buffer[BUF_SIZE], ch;
    char *ip_address;
    const char* delimiter = ":";
    int addr_length, i, port, client_socket;
    fstream file;
    string operation, username;

    create_directory("client_dir");
    
    client_socket = socket(DOMAIN, TYPE, PROTOCOL);

    ip_address = strtok(argv[1], delimiter);
    port = atoi(strtok(NULL, delimiter));

    struct sockaddr_in address = destination_setup(1, ip_address, port);
    addr_length = sizeof(address);

    connect(client_socket, (struct sockaddr*)&address, addr_length);

    cout << "input your username:" << endl;
    while(true){
		memset(buffer, '\0', BUF_SIZE);

        cin >> username;
        username = "usr " + username;
        send(client_socket, username.c_str(), username.size(), 0);

        recv(client_socket, buffer, BUF_SIZE, 0);

        if(buffer[0] == '0'){
            cout << "username is in used, please try another:" << endl;
        }
        else{
            cout << "connect successfully" << endl;
            break;
        }
    }

    while(true){
        getline(cin, operation);
        send(client_socket, operation.c_str(), operation.size(), 0);
    }

    close(client_socket);

    return 0;
}