#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BACKLOG_SZ		5
#define LISTEN_PORT 	8888
#define MAX_READ_LEN 	999
#define INPUT_BUFF_LEN  1000

int main() {
	// Holder for return values
	int ret;
	// File descriptor for the socket we'll be listening on
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		// Failed to create server socket
		close(server_socket);
		return -1;
	}
	// The struct that will keep info about our local address
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(LISTEN_PORT);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	// No idea why this is needed
	memset(&(server_addr.sin_zero), '\0', 8);
	// We're a server, so we need to bind to some local port
	ret = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
	if (ret == -1) {
		// Some error
		close(server_socket);
		return -1;
	}
	// We're a server, so we're going to listen()
	ret = listen(server_socket, BACKLOG_SZ);
	if (ret == -1) {
		// Some error
		close(server_socket);
		return -1;
	}
	while (1) {
		// File descriptor for the socket that will be created when a client is accepted
		int client_socket;
		// This struct will keep info about the client's address
		struct sockaddr_in client_addr;
		int sin_size = sizeof(struct sockaddr_in);
		// Accept() the client, thus getting another socket for read/write communication
		client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &sin_size);
		if (client_socket == -1) {
			// Failed to accept
			close(client_socket);
			continue;
		}
		// Allocate space for client's message
		char * input_buff = malloc(INPUT_BUFF_LEN);
		// Read the client's message into buffer
		int num_read = recv(client_socket, (void *) input_buff, MAX_READ_LEN, 0);
		// If everything went O.K.
		if (num_read != -1) {
			// Print client's message
			printf("They said: %s\n", input_buff);
			// Be gentle and welcome the client
			char *resp = "You're welcome!";
			int num_sent = send(client_socket, resp, strlen(resp), 0);
			if (num_sent == -1) {
				// Maybe print the error
			}	
		}
		// Free client's message buffer like a good boy
		free(input_buff);
		// Close client's socket, we're done
		close(client_socket);
	}
	close(server_socket);
	return 0;
}