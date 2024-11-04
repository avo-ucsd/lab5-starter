#include "http-server.h"
#include <string.h>

char const HTTP_404_NOT_FOUND[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";
char const HTTP_404_MESSAGE[] = "Page not found.\r\n";
int num = 0;

void handle_404(int client_sock, char *path)  {
    printf("SERVER LOG: Got request for unrecognized path \"%s\"\n", path);

    char response_buff[BUFFER_SIZE];
    snprintf(response_buff, BUFFER_SIZE, "Error 404:\r\nUnrecognized path \"%s\"\r\n", path);
    // snprintf includes a null-terminator

    // TODO: send response back to client?
    write(client_sock, HTTP_404_NOT_FOUND, sizeof(HTTP_404_NOT_FOUND));
    write(client_sock, response_buff, strlen(response_buff));
}

char const HTTP_200_SUCCESS[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
char const HTTP_200_MESSAGE[] = "Successful request.\r\n";

void handle_200(int client_sock)
{
	write(client_sock, HTTP_200_SUCCESS, sizeof(HTTP_200_SUCCESS));
}


void handle_response(char *request, int client_sock) {
    char path[256];

    printf("\nSERVER LOG: Got request: \"%s\"\n", request);

    // Parse the path out of the request line (limit buffer size; sscanf null-terminates)
    if (sscanf(request, "GET %255s", path) != 1) {
        printf("Invalid request line\n");
        return;
    }

    if (strstr(request, "/shownum") || strcmp(path, "/") == 0)
    {
	    handle_200(client_sock);

	    // Consider malloc/calloc for dynamic length 	    
	    char your_num_message[50] = "Your number is ";
	    sprintf(your_num_message + strlen(your_num_message), "%d\r\n", num);
	    write(client_sock, your_num_message, strlen(your_num_message));
    }
    else if (strstr(request, "/increment"))
    {
	    handle_200(client_sock);

	    num++;
	    char increment_message[] = "Incremented num. See /shownum path to see update.\r\n";
	    write(client_sock, increment_message, sizeof(increment_message));
    }
    else if (strstr(request, "/add"))
    {
	    handle_200(client_sock);
    }
    else
    {
	    handle_404(client_sock, path);
    }
}

int main(int argc, char *argv[]) {
    int port = 0;
    if(argc >= 2) { // if called with a port number, use that
        port = atoi(argv[1]);
    }

    start_server(&handle_response, port);
}
