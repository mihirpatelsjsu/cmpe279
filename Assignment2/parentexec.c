#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <pwd.h>
#define PORT 80

int main(int argc, char const *argv[]){

	int valread;
	char buffer[1024] = {0}; 

	//Reading the value from socket and storing it to buffer
	valread = read( *argv[1], buffer, 1024); 
    printf("%s\n",buffer ); 

    return 0;
}