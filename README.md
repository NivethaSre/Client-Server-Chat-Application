# Client-Server Chat Application

## Description
The **Client-Server Chat Application** is a text-based messaging system where multiple clients can connect to a central server to exchange text messages and share files securely. The application supports features like real-time messaging, file uploads/downloads, and encrypted communication, ensuring secure data transfers. It uses **pthread** for handling multiple client connections concurrently and **ncurses** for the client-side text interface. Communication between the client and server is secured with **OpenSSL**, and network communication is implemented using raw sockets.

This project aims to teach networking fundamentals, multithreading, file handling, and encryption in a simple client-server architecture.

---

## Features
- **Multithreaded Server**: Handles multiple clients simultaneously using **pthread**.
- **Client Interface**: Simple text-based UI built with **ncurses**.
- **Encrypted Communication**: All messages and file transfers are encrypted using **OpenSSL**.
- **File Sharing**: Clients can upload and download files via the server.
- **Cross-Platform**: Designed to work on POSIX-compliant systems (Linux, macOS, etc.).
- **CMake Build System**: Easy build and project management with **CMake**.
- **MIT License**: Open-source project that is free to modify and distribute.

---

## Learning Objectives
The primary goal of this project is to provide a deeper understanding of:
- **Network Programming**: Socket communication for real-time messaging and file sharing.
- **Multithreading**: Concurrent client handling using pthreads.
- **Encryption**: Using OpenSSL to secure communications.
- **File Handling**: Uploading, downloading, and removing files on the server.
  
It will help in learning how to manage file transfers, secure communication, and maintain multiple client connections in a client-server system.

---

## Getting Started

### Prerequisites
To build and run this application, you need the following tools installed:

- **A POSIX-compliant system** (Linux, macOS, etc.)
- **C++17 compatible compiler**
- **CMake**
- **OpenSSL** (`libssl-dev`)
- **ncurses** (`ncurses-dev`)

### Installation and Setup

1. Clone the repository:
   ```bash
   git clone --recursive https://github.com/YourRepo/Client-Server-Chat.git
   cd Client-Server-Chat
   ```

2. Create a build directory and navigate into it:
   ```bash
   mkdir build
   cd build
   ```

3. Run CMake to configure the project:
   ```bash
   cmake ..
   ```

4. Build the project:
   ```bash
   make
   ```

---

## Running the Application

### Server
1. Navigate to the server directory and run the server executable:
   ```bash
   cd build/server
   ./server
   ```

### Client
1. Navigate to the client directory and run the client executable:
   ```bash
   cd build/client
   ./client
   ```

---

## Commands

The client application supports the following commands:

- **`/help`**: Lists all available commands.
- **`/connect [server_ip]`**: Connects to the server using the provided IP address.
- **`/disconnect`**: Disconnects from the server and goes offline.
- **`/quit`**: Quits the client application.
- **`/users`**: Lists all users currently connected to the server.
- **`/files`**: Lists all available downloadable files on the server.
- **`/upload [file]`**: Uploads a file to the server.
- **`/download [file_id]`**: Downloads a file from the server.
- **`/remove [file_id/all]`**: Removes uploaded file(s) from the server.

---

## File Sharing

- **Uploading**: Clients can upload files to the server using the `/upload` command.
- **Downloading**: Clients can download files from the server by specifying the file name or unique file ID using the `/download` command.
- **Removing**: Clients can remove their uploaded files using `/remove [file_id/all]`.

Uploaded files are stored on the server, and clients can download them into their `resources/downloads` folder.

---

## Data Structures Used

- **Client List**: A dynamic array or linked list to manage client connections.
- **Message Queue**: A queue to handle incoming messages and ensure they are processed in the correct order.
- **File List**: A hash map to store and manage uploaded files, indexed by a unique file ID.
- **Client Struct**: A structure to store client-specific data such as socket descriptors, username, and connection status.
- **Mutex**: Used to ensure thread-safe operations on shared resources like the client list and files.

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## Acknowledgements

- **pthread**: For handling concurrent client connections.
- **OpenSSL**: For securing communication between the client and server.
- **ncurses**: For creating the simple text-based UI on the client-side.

---

