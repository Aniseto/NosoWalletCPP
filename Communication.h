#include <string>
#pragma once
#include <boost/asio.hpp> //Boost Library //vcpkg install boost:x64-windows-static https://www.boost.org/
#include <iostream>
#include <fstream>
#include <vector>


std::string SendStringToNode(const std::string& ip, int port, const std::string& message) {
    WSADATA wsaData; // Wsdata Variable
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error Initializing Winsock." << std::endl;
        return "Error Initializing Winsock.";
    }

    // Create TCP socket
    SOCKET socketCliente = socket(AF_INET, SOCK_STREAM, 0);
    if (socketCliente == INVALID_SOCKET) {
        std::cerr << "Error creating socket." << std::endl;
        WSACleanup();
        return "Error creating socket.";
    }

    // Specify Address and port to connect
    sockaddr_in direccionServidor{};
    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &(direccionServidor.sin_addr)) <= 0) {
        std::cerr << "Error converting IP address." << std::endl;
        closesocket(socketCliente);
        WSACleanup();
        return "Error converting IP address.";
    }

    //*! Connect to server
    if (connect(socketCliente, reinterpret_cast<struct sockaddr*>(&direccionServidor), sizeof(direccionServidor)) == SOCKET_ERROR) {
        std::cerr << "Error connecting to server." << std::endl;
        closesocket(socketCliente);
        WSACleanup();
        return "Error connecting to server.";
    }

    // Send message to server
    if (send(socketCliente, message.c_str(), message.size(), 0) == SOCKET_ERROR) {
        std::cerr << "Error sendind message." << std::endl;
        closesocket(socketCliente);
        WSACleanup();
        return "Error sending message.";
    }

    if (message == "GETZIPSUMARY\n")
    {
        std::string response = "Sumary Downloaded\n";
        std::remove("summary.zip");
        std::ofstream outputFile("summary.zip", std::ios::binary);
        char buffer[1024];
        std::string respuesta;
        int bytesRecibidos;

        while ((bytesRecibidos = recv(socketCliente, buffer, sizeof(buffer) - 1, 0)) > 0)
        {
            // Write Bytes to file
            outputFile.write(buffer, bytesRecibidos);
        }

        outputFile.close();

        return response;
    }

    char buffer[1024];
    std::string respuesta;
    int bytesRecibidos;
    while ((bytesRecibidos = recv(socketCliente, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesRecibidos] = '\0';
        respuesta += buffer;
    }

    // Close Connection and return answer
    closesocket(socketCliente);
    WSACleanup();
    return respuesta;

}

int GetMainetTimeStamp()
{
    std::string ip = "20.199.50.27";
    int port = 8080;
    std::string command = "NSLTIME\n";
    std::string TimeString = SendStringToNode(ip, port, command);
    int TimeInt = stoi(TimeString);
    //cout << "Time Integer" << TimeInt << endl; // Control String to check answer.
    return TimeInt;
}
