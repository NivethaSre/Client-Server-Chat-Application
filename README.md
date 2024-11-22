# Client-Server Chat Application

 Description

The Client-Server Chat Application is a text-based messaging system where multiple clients can connect to a central server to exchange text messages and share files securely. The application supports features like real-time messaging, file uploads/downloads, and encrypted communication, ensuring secure data transfers. It uses **pthreads** for handling multiple client connections concurrently and **ncurses** for the client-side text interface. The communication between the client and the server is secured with **OpenSSL**, and the network communication is implemented using raw sockets.

This project aims to teach networking fundamentals, multithreading, file handling, and encryption in a simple client-server architecture.



 Features

- Multithreaded Server: Handles multiple clients simultaneously using pthreads.
- Client Interface: Simple text-based UI built with ncurses.
- Encrypted Communication: All messages and file transfers are encrypted using OpenSSL.
- File Sharing: Clients can upload and download files via the server.
- Cross-Platform: Designed to work on POSIX-compliant systems (Linux, macOS, etc.).
- CMake Build System: Easy build and project management with CMake.
- MIT License:Open-source project that is free to modify and distribute.

---

## Learning Objective

The primary goal of this project is to provide a deeper understanding of **network programming**, **socket communication**, **multithreading**, and **encryption** by building a chat application from scratch. It will also help in learning how to manage file transfers in a client-server system.



## Getting Started

### Prerequisites

To build and run this application, you need the following tools installed:

- A POSIX-compliant system (Linux, macOS, etc.)
- A C 17 compatible compiler**
- CMake
- OpenSSL (Install `libssl-dev`)
- ncurses(Install `ncurses`)

Installation and Setup

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



 Running the Application

 Server

Navigate to the `server` directory and run the server executable:
```bash
cd build/server
./server
```

Client

Navigate to the `client` directory and run the client executable:
```bash
cd build/client
./client
```

---

Commands

The client application supports the following commands:

- /help: Lists all available commands.
- /connect [server_ip]: Connects to the server using the provided IP address.
- /disconnect: Disconnects from the server and goes offline.
- /quit: Quits the client application.
- /users: Lists all users currently connected to the server.
- /files: Lists all available downloadable files on the server.
- /upload [file]: Uploads a file to the server.
- /download [file_id]: Downloads a file from the server.
- /remove [file_id/all]: Removes uploaded file(s) from the server.



File Sharing

Clients can upload files to the server, making them available for download by other clients. Files are stored on the server, and clients can download them by either the file name or the unique file ID. Once downloaded, files are saved in the `resources/downloads` folder on the client system.



