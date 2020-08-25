#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int main()
{
    int server_fd, new_socket, nBytes, port, opt = 1;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char return_msg_time[1024] = {0};
    char return_msg[1024] = {0};
    time_t now = time(0);
    char* dt = ctime(&now);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0))  == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    
    cout << "Listen to port 12345 \n";
    address.sin_port = htons(12345);
    
   
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    cout << "Listening..."<<endl;
    
    if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
    
    bool done =false;
    while(!done)
    {
        int len; 
        len=read(new_socket, buffer, 1024);
        cout << "Request received: " << buffer << endl;
        cout << "Last 4 character:";
        printf("%d, %d, %d, %d\n", buffer[len-4],buffer[len-3],buffer[len-2],buffer[len-1]);
        char last4 = {buffer[len-4]};
        char last3 = {buffer[len-3]};
        char lasttwo = {buffer[len-2]};
        char lastone = {buffer[len-1]};
        
        if (last4 ==13 && lasttwo ==13 && last3 ==10 && lastone ==10) 
	            {done=true;}
    }
    
    strcpy(buffer,"HTTP/1.1 200 OK\nDate:");
    strcpy(return_msg_time, dt);
    strcpy(return_msg,"Server: Apache/1.3.0 (Unix) \nLast-Modified: Mon, 07 Oct 2019\nContent-Length: 336\nContent-Type: text/html\n\n<!DOCTYPE html>\n <html lang=\"en\">\n <head>\n <meta charset=\"UTF-8\" />\n  <meta name=\"viewport\" content=\"width=device-width\", initial-scale=1.0\" />\n  <meta http-equiv=\"X-UA-Compatible\" content=\"ie-edge\" />\n <title>Document</title>\n </head>\n  <body>\n <h1>[CHEN SIN-ROU]</h1>\n<h2>[200586]</h2>\n <h2>[2019 Oct 12 10:08]</h2>\n  </body>\n </html>\n");
    send(new_socket, buffer, strlen(buffer), 0);
    send(new_socket, return_msg_time, strlen(return_msg_time), 0);
    send(new_socket, return_msg, strlen(return_msg), 0);
    cout << "response: " << buffer << return_msg_time << return_msg << endl;
    close(new_socket);
    return 0;
}
        