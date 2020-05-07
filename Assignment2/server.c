// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <pwd.h>
#define PORT 80
int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    // char buffer[1024] = {0}; 
    char *hello = "Hello from server"; 

    // Struct passwd to get user nobody 
    struct passwd * nobody;
    int process_id;
    int child_id;
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
    //                                               &opt, sizeof(opt))) 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt)))
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 

    // Get nobody user information i.e. -2 for mac
    nobody = getpwnam("nobody");

    // Forking 
    process_id = fork();
    
    if (process_id > 0)
    {
        wait(NULL);
        send(new_socket , hello , strlen(hello) , 0 ); 
        printf("Hello message sent\n"); 
    }
    else if (process_id == 0)
    {
        child_id = setuid(nobody->pw_uid);
        if (child_id == -1)
        {
            printf("Set User ID error");
            exit(EXIT_FAILURE);
        }
        
        //Generating arguments to be sent to new exec'ed process e.g socket data
        char *args[] = {"./parentexec", &new_socket, NULL};

        //Execing a new process and sending arguments with it
        int e = execvp(args[0],args);

        //Check if exec got successfully and if not print the error with value
        if (e < 0) {
            printf("Exec Failed : %d\n", e);
        }
    }
    else
    {
        printf("Fork Error");
    }
    
    return 0; 
} 
