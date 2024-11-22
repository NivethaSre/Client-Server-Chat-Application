#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define SERVER_MODE 1  // Set to 1 for server mode, 0 for client mode

// Global variables for server-side
int client_count = 0;
int client_sockets[MAX_CLIENTS];

// Structure for client data (used in multithreading)
typedef struct {
    int client_socket;
    SSL *ssl;
} client_data;

// Function to initialize OpenSSL
SSL_CTX *init_openssl() {
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    const SSL_METHOD *method = TLS_server_method();
    ctx = SSL_CTX_new(method);
    if (!ctx) {
        perror("SSL_CTX_new failed");
        exit(EXIT_FAILURE);
    }
    return ctx;
}

// Function to configure SSL certificates
void configure_certificates(SSL_CTX *ctx) {
    if (SSL_CTX_use_certificate_file(ctx, "server-cert.pem", SSL_FILETYPE_PEM) <= 0) {
        perror("Failed to load certificate");
        exit(EXIT_FAILURE);
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, "server-key.pem", SSL_FILETYPE_PEM) <= 0) {
        perror("Failed to load private key");
        exit(EXIT_FAILURE);
    }
}

// Function to handle client communication in server mode
void *client_handler(void *arg) {
    client_data *data = (client_data *)arg;
    char buffer[1024];
    int read_size;
    
    while ((read_size = SSL_read(data->ssl, buffer, sizeof(buffer))) > 0) {
        buffer[read_size] = '\0';  // Null terminate the message
        
        // Broadcast the message to all clients
        for (int i = 0; i < client_count; i++) {
            if (client_sockets[i] != data->client_socket) {
                SSL_write(data->ssl, buffer, strlen(buffer));
            }
        }
    }

    // Cleanup and close connection
    SSL_shutdown(data->ssl);
    SSL_free(data->ssl);
    close(data->client_socket);
    free(data);
    pthread_exit(NULL);
}

// Function to run the server
void start_server() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    SSL_CTX *ctx;
    pthread_t threads[MAX_CLIENTS];
    
    // Initialize OpenSSL
    SSL_library_init();
    ctx = init_openssl();
    configure_certificates(ctx);

    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept clients
    while ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len))) {
        if (client_count < MAX_CLIENTS) {
            SSL *ssl = SSL_new(ctx);
            SSL_set_fd(ssl, client_socket);
            if (SSL_accept(ssl) <= 0) {
                ERR_print_errors_fp(stderr);
            }

            client_data *data = malloc(sizeof(client_data));
            data->client_socket = client_socket;
            data->ssl = ssl;

            pthread_create(&threads[client_count], NULL, client_handler, (void *)data);
            client_sockets[client_count] = client_socket;
            client_count++;
        }
    }

    close(server_socket);
    SSL_CTX_free(ctx);
}

// Function to run the client
void start_client() {
    int sock;
    struct sockaddr_in server_addr;
    SSL_CTX *ctx;
    SSL *ssl;
    char message[1024];
    
    // Initialize OpenSSL
    SSL_library_init();
    ctx = init_openssl();
    
    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Create SSL object and connect
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sock);
    if (SSL_connect(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
    }

    printf("Connected to server...\n");

    // Communication loop
    while (1) {
        printf("Enter message: ");
        fgets(message, sizeof(message), stdin);

        // Send message to server
        SSL_write(ssl, message, strlen(message));

        // Receive response from server
        int bytes_received = SSL_read(ssl, message, sizeof(message) - 1);
        if (bytes_received > 0) {
            message[bytes_received] = '\0';
            printf("Server: %s\n", message);
        }
    }

    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sock);
    SSL_CTX_free(ctx);
}

int main() {
    #if SERVER_MODE
        start_server();  // Run as server
    #else
        start_client();  // Run as client
    #endif
    return 0;
}
