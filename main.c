#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>


char* extract_path(char *buffer) {
	int start_idx = 4; // 4 because you are extracting the requested path in the response which is in this format "GET /requested/path HTTP/1.1" 4th index is '/'.
	int end_idx = start_idx;
	while(buffer[end_idx + 1] != ' '){
		end_idx++;
	}
	int len = end_idx - start_idx + 1;
	char* substr = malloc(len + 1); // one byte more for null terminator. Also no need for explicit casting of void* in C.
	if(substr == NULL) {
		perror("Malloc failed\n");
	}
	strncpy(substr, buffer + start_idx, len);
	substr[len] = '\0'; // Null terminate the string
	return substr;
}

int main() {
	// Disable output buffering
	setbuf(stdout, NULL);
 	setbuf(stderr, NULL);

	int server_fd, client_addr_len;
	struct sockaddr_in client_addr;
	
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		printf("Socket creation failed: %s...\n", strerror(errno));
		return 1;
	}
	
    // setting SO_REUSEADDR ensures that we don't run into 'Address already in use' errors
	int reuse = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		printf("SO_REUSEADDR failed: %s \n", strerror(errno));
		return 1;
	}
	
	struct sockaddr_in serv_addr = { .sin_family = AF_INET ,
									 .sin_port = htons(4221),
									 .sin_addr = { htonl(INADDR_ANY) },
									};
	
	if (bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0) {
		printf("Bind failed: %s \n", strerror(errno));
		return 1;
	}
	
	int connection_backlog = 5;
	if (listen(server_fd, connection_backlog) != 0) {
		printf("Listen failed: %s \n", strerror(errno));
		return 1;
	}
	
	printf("Waiting for a client to connect...\n");
	client_addr_len = sizeof(client_addr);
	
	int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
	printf("Client connected\n");

	char buffer[1024];
	size_t size = read(client_fd, buffer, sizeof(buffer) - 1); // leaving one byte for null termination
	char* requested_path = extract_path(buffer);

	char message[1024] = "HTTP/1.1 200 OK\r\n\r\nRequested path: ";
	int fixed_message_len = strlen(message);
	strncpy(message + fixed_message_len, requested_path, strlen(requested_path));
	if(send(client_fd, message, strlen(message), 0) == -1){
		printf("Send failed: %s \n", strerror(errno));
		return 1;
	}
	close(server_fd);
	close(client_fd);

	return 0;
}
