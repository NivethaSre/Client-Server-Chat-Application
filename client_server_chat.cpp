#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <unistd.h>
#include <pthread.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>

#define MAX_CLIENTS 10
#define PORT 8080

// Client structure to manage connected clients
struct Client {
    int sock;
    SSL *ssl;
    std::string username;
};

std::vector<Client *> clients; // Store connected clients

// Function prototypes
void setup_openssl();
void cleanup_openssl();
SSL_CTX *create_context();
void configure_context(SSL_CTX *ctx);
void encrypt_message(std::string &message, SSL *ssl);
void decrypt_message(std::string &message, SSL *ssl);
void *client_thread(void *client_ptr);
void send_message_to_all(const std::string &message, Client *sender);
void *receive_messages(void *ssl_ptr);

void setup_openssl() {
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    SSL_library_init();
}

void cleanup_openssl() {
    EVP_cleanup();
    ERR_free_strings();
}

SSL_CTX *create_context() {
    const SSL_METHOD *method = TLS_server_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (!ctx) {
        std::cerr << "Unable to create SSL context" << std::endl;
        exit(1);
    }
    return ctx;
}

void configure_context(SSL_CTX *ctx) {
    SSL_CTX_set_ecdh_auto(ctx, 1);
    if (SSL_CTX_use_certificate_file(ctx, "server-cert.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, "server-key.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }
}

void encrypt_message(std::string &message, SSL *ssl) {
    // Simple encryption (can be expanded)
    for (auto &c : message) {
        c = c + 1; // Shift characters for encryption (Caesar Cipher example)
    }
}

void decrypt_message(std::string &message, SSL *ssl) {
    // Simple decryption (reverse of the encryption)
    for (auto &c : message) {
        c = c - 1; // Shift characters back
    }
}

void *client_thread(void *client_ptr) {
    Client *client = (Client *)client_ptr;
    char buffer[1024];
    int bytes;
    
    while (true) {
        bytes = SSL_read(client->ssl, buffer, sizeof(buffer) - 1);
        if (bytes <= 0) {
            break;
        }
        buffer[bytes] = '\0';
        decrypt_message(buffer, client->ssl);
        std::string message(buffer);
        std::cout << "Received message: " << message << std::endl;
        send_message_to_all(message, client);
    }

    close(client->sock);
    SSL_free(client->ssl);
    delete client;
    return nullptr;
}

void send_message_to_all(const std::string &message, Client *sender) {
    for (Client *client : clients) {
        if (client != sender) {
            SSL_write(client->ssl, message.c_str(), message.length());
        }
    }
}

void *receive_messages(void *ssl_ptr) {
    SSL *ssl = (SSL *)ssl_ptr;
    char buffer[1024];
    int bytes;

    while (true) {
        bytes = SSL_read(ssl, buffer, sizeof(buffer) - 1);
        if (bytes <= 0) {
            break;
        }
        buffer[bytes] = '\0';
        decrypt_message(buffer, ssl);
        std::cout << "Message from server: " << buffer << std::endl;
    }
    return nullptr;
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    setup_openssl();
    SSL_CTX *ctx = create_context();
    configure_context(ctx);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_sock, MAX_CLIENTS);

    std::cout << "Server started. Waiting for connections..." << std::endl;
    
    while (true) {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
        SSL *ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client_sock);
        if (SSL_accept(ssl) == -1) {
            ERR_print_errors_fp(stderr);
            continue;
        }

        Client *new_client = new Client{client_sock, ssl, "Unknown"};
        clients.push_back(new_client);
        pthread_t thread_id;
        pthread_create(&thread_id, nullptr, client_thread, (void *)new_client);
    }

    SSL_CTX_free(ctx);
    cleanup_openssl();
    return 0;
}
