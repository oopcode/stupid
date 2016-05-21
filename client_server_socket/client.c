#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define DST_PORT 8888

int main() {
	// Holder for return values
	int ret;
	// File descriptor for client's socket (will be used for read/writes)
	int client_socket;
	client_socket = socket(AF_INET, SOCK_STREAM, 0); // do some error checking!
	if (client_socket == -1) {
		// Failed to create socket
		close(client_socket);
		return -1;
	}
	// This struct will hold data about who we want to talk to
	struct sockaddr_in dst_addr;
	// Host byte order
	dst_addr.sin_family = AF_INET;
	dst_addr.sin_port = htons(DST_PORT);
	dst_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	// No idea why this is needed
	memset(&(dst_addr.sin_zero), '\0', 8);
	// Connect to destination server. When we connect(), system binds our socket
	// at some free port itself (we don't need to bother about that)
	ret = connect(client_socket, (struct sockaddr *)&dst_addr, sizeof(struct sockaddr));
	if (ret == -1) {
		// Failed to connect
		close(client_socket);
		return -1;
	}
	// Send the message
	char *msg = "I'm Andrew!";
	int num_sent = send(client_socket, msg, strlen(msg), 0);
	// If everything went O.K.
	if (num_sent != -1) {
		// Print the answer
		char *answer = malloc(1000);
		int num_read = recv(client_socket, answer, 999, 0);
		// If everything's O.K.
		if (num_read != -1) {
			printf("Server response: %s\n", answer);
		}
		// Free memory
		free(answer);
	}
	// Close the socket, we're done
	close(client_socket);
	return 0;
}