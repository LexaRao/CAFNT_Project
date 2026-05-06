// Serial Deamon.cpp : A simple serial communication daemon that listens for commands and executes them.
// This code is intended for Windows and uses the WinAPI for serial communication.

#include <windows.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <algorithm>

// Define static variables for interacting with the systems components.
const string resourcesLoc = "Resources/";

static HANDLE openSerialPort(const std::wstring& portName)
{
    HANDLE handle = CreateFileW(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if (handle == INVALID_HANDLE_VALUE) {
        return INVALID_HANDLE_VALUE;
    }

    DCB dcb = {0};
    dcb.DCBlength = sizeof(dcb);
    if (!GetCommState(handle, &dcb)) {
        CloseHandle(handle);
        return INVALID_HANDLE_VALUE;
    }

    dcb.BaudRate = CBR_9600;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity = NOPARITY;

    if (!SetCommState(handle, &dcb)) {
        CloseHandle(handle);
        return INVALID_HANDLE_VALUE;
    }

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    SetCommTimeouts(handle, &timeouts);

    return handle;
}

static bool sendSerial(HANDLE port, const std::string& text)
{
    if (port == INVALID_HANDLE_VALUE) {
        return false;
    }
    DWORD written = 0;
    return WriteFile(port, text.c_str(), static_cast<DWORD>(text.size()), &written, nullptr) && written == text.size();
}

static bool receiveSerial(HANDLE port, std::string& output)
{
    output.clear();
    if (port == INVALID_HANDLE_VALUE) {
        return false;
    }

    char buffer = 0;
    DWORD bytesRead = 0;
    while (true) {
        if (!ReadFile(port, &buffer, 1, &bytesRead, nullptr)) {
            return false;
        }
        if (bytesRead == 0) {
            break;
        }
        if (buffer == '\r') {
            continue;
        }
        if (buffer == '\n') {
            break;
        }
        output.push_back(buffer);
        if (output.size() > 1024) {
            break;
        }
    }

    return !output.empty();
}

// Author: Lexa Hope.
// Note: Exit the commandline using the following requirements of the program.  Clean exit.  
static std::vector><std::string> exitCommand(const std::string& input) {
    // Successful exit when command is received.
    exit(0);
}

static std::vector<std::string> splitCommand(const std::string& input)
{
    std::vector<std::string> parts;
    std::istringstream iss(input);
    std::string token;
    while (iss >> token) {
        parts.push_back(token);
    }
    return parts;
}

static std::string commandHelp(const std::vector<std::string>& args)
{
    return "Available commands: help (ST/REPT), ping, echo <text>, add <a> <b>, led_on, led_off, exit (FUNC/STOP)";
}

static std::string commandPing(const std::vector<std::string>& args)
{
    return "PONG";
}

static std::string commandEcho(const std::vector<std::string>& args)
{
    if (args.size() < 2) {
        return "Usage: echo <text>";
    }
    std::string result;
    for (size_t i = 1; i < args.size(); ++i) {
        if (i > 1) {
            result += ' ';
        }
        result += args[i];
    }
    return result;
}

// Add commands to the system using a preconfigured json folder.
static std::string commandAdd(const std::vector<std::string>& args)
{
    if (args.size() != 3) {
        return "Usage: add <a> <b>";
    }
    try {
        int a = std::stoi(args[1]);
        int b = std::stoi(args[2]);
        return std::to_string(a + b);
    }
    catch (...) {
        return "Invalid numbers";
    }
}

static std::string commandLedOn(const std::vector<std::string>& args)
{
    // Local library call placeholder
    // Insert the actual local device control library function here.
    return "LED turned on";
}

static std::string commandLedOff(const std::vector<std::string>& args)
{
    // Local library call placeholder
    // Insert the actual local device control library function here.
    return "LED turned off";
}

int main()
{
    std::wstring portName = L"\\.\\COM3";
    HANDLE serialPort = openSerialPort(portName);
    bool useSerial = serialPort != INVALID_HANDLE_VALUE;

    if (useSerial) {
        sendSerial(serialPort, "Serial daemon started on COM3\r\n");
    }
    else {
        std::cout << "Could not open serial port COM3. Falling back to console mode.\n";
    }

    std::unordered_map<std::string, std::function<std::string(const std::vector<std::string>&)>> commands;
    commands["help"] = commandHelp;
    commands["ping"] = commandPing;
    commands["echo"] = commandEcho;
    commands["add"] = commandAdd;
    commands["list"] = commandList;
    commands["led_on"] = commandLedOn;
    commands["led_off"] = commandLedOff;

    // Define commands for backwards compatiblity with the prototype.
    commands["ST/REPT"] = commandHelp;
    commands["FUNC/STOP"] = commandExit;
    
    commands["exit"] = [](const std::vector<std::string>& args) { return std::string("exit"); };

    std::string input;
    while (true) {
        if (useSerial) {
            if (!receiveSerial(serialPort, input)) {
                continue;
            }
        }
        else {
            std::cout << "> ";
            if (!std::getline(std::cin, input)) {
                break;
            }
        }

        auto parts = splitCommand(input);
        if (parts.empty()) {
            continue;
        }

        std::string command = parts[0];
        std::transform(command.begin(), command.end(), command.begin(), ::tolower);

        auto it = commands.find(command);
        std::string response;
        if (it == commands.end()) {
            response = "Unknown command. Type help.";
        }
        else {
            response = it->second(parts);
        }

        if (useSerial) {
            sendSerial(serialPort, response + "\r\n");
        }
        else {
            std::cout << response << "\n";
        }

        // Break the loop if the command is exit.
        if (command == "exit") {
            break;
        }
    }

    if (useSerial) {
        CloseHandle(serialPort);
    }

    return 0;
}
