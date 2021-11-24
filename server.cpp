#include <iostream> 
#include <string.h>   //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>   //close 
#include <arpa/inet.h>    //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
     
#define DOMAIN AF_INET
#define TYPE SOCK_STREAM
#define PROTOCOL 0
#define BUF_SIZE 2048
#define MAX_NUM_OF_CLIENTS 10

using namespace std;
     
int main(int argc , char** argv){
    int opt = true;  
    int main_socket, addr_length, new_socket, client_socket[MAX_NUM_OF_CLIENTS] = {0};
    int i, sd, max_sd;

    int port = atoi(argv[1]);

    char buffer[BUF_SIZE];
         
    fd_set readfds;
         
    main_socket = socket(AF_INET , SOCK_STREAM , PROTOCOL);
     
    setsockopt(main_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
     
    struct sockaddr_in address;
    address.sin_family = AF_INET;  
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons(port);
    addr_length = sizeof(address);  
         
    bind(main_socket, (struct sockaddr *)&address, addr_length);
         
    listen(main_socket, 3);

    while(true){
        FD_ZERO(&readfds);  
        
        FD_SET(main_socket, &readfds);  
        max_sd = main_socket;  

        for(i = 0; i < MAX_NUM_OF_CLIENTS; i++){
            sd = client_socket[i];
            if(sd > 0){
                FD_SET(sd, &readfds);
            }
            if(sd > max_sd){
                max_sd = sd;
            }  
        }
     
        select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if(FD_ISSET(main_socket, &readfds)){  
            new_socket = accept(main_socket, (struct sockaddr*)&address, (socklen_t*)&addr_length);
            for(i=0; i<MAX_NUM_OF_CLIENTS; i++){  
                if(client_socket[i] == 0){  
                    client_socket[i] = new_socket;  
                    break;  
                }  
            }
        }

        for(i = 0; i < MAX_NUM_OF_CLIENTS; i++){  
            sd = client_socket[i];
            memset(buffer, '\0', BUF_SIZE);
            if(FD_ISSET(sd ,&readfds)){  
                if(recv(sd, buffer, BUF_SIZE, 0) <= 0){  
                    close(sd);
                    client_socket[i] = 0;
                }  
                else{
                    cout << buffer << endl;
                }
            }  
        }
    }
    return 0;  
}  