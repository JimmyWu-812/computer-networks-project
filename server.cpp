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
    struct sockaddr_in address;  
         
    //set of socket descriptors 
    fd_set readfds;
         
    //create a main socket 
    main_socket = socket(AF_INET , SOCK_STREAM , PROTOCOL);
     
    //set main socket to allow multiple connections , 
    //this is just a good habit, it will work without this 
    setsockopt(main_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
     
    //type of socket created 
    address.sin_family = AF_INET;  
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons(port);

    addr_length = sizeof(address);  
         
    //bind the socket to localhost port
    bind(main_socket, (struct sockaddr *)&address, addr_length);
         
    //try to specify maximum of 3 pending connections for the master socket 
    listen(main_socket, 3);

    while(true){
        //clear the socket set
        FD_ZERO(&readfds);  
        
        //add main socket to set 
        FD_SET(main_socket, &readfds);  
        max_sd = main_socket;  

        //add child sockets to set 
        for(i=0; i<MAX_NUM_OF_CLIENTS; i++){  
            //socket descriptor 
            sd = client_socket[i];  
            //if valid socket descriptor then add to read list 
            if(sd > 0){
                FD_SET(sd, &readfds);
            }
            //highest file descriptor number, need it for the select function 
            if(sd > max_sd){
                max_sd = sd;
            }  
        }
     
        //wait for an activity on one of the sockets , timeout is NULL , 
        //so wait indefinitely 
        select(max_sd + 1, &readfds, NULL, NULL, NULL);

        //If something happened on the master socket , 
        //then its an incoming connection 
        if(FD_ISSET(main_socket, &readfds)){  
            new_socket = accept(main_socket, (struct sockaddr*)&address, (socklen_t*)&addr_length);
            //add new socket to array of sockets 
            for(i=0; i<MAX_NUM_OF_CLIENTS; i++){  
                //if position is empty 
                if(client_socket[i] == 0){  
                    client_socket[i] = new_socket;  
                    break;  
                }  
            }
        }
        sleep(1);

        //else its some IO operation on some other socket
        for(i=0; i<MAX_NUM_OF_CLIENTS; i++){  
            sd = client_socket[i];
            memset(buffer, '\0', BUF_SIZE);
            if(FD_ISSET(sd ,&readfds)){  
                //Check if it was for closing, and also read the incoming message
                if(recv(sd, buffer, BUF_SIZE, 0) < 0){  
                    //Close the socket and mark as 0 in list for reuse
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