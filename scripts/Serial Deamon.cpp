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
#include <filesystem>
#include <fstream>
#include <filesystem>

// Define static variables for interacting with the systems components.
const std::string resourcesLoc = "Resources/";
int receiverSetting = 0;
bool poweredOn = false;
bool curColor = 0;
int powerLevel = 0;

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
static std::string exitCommand(const std::vector<std::string>& args) {
    // Successful exit when command is received.
    exit(0);
    return "Exiting...";
}

// Author: Lexa Hope.
// Note: Account for repeated commands.
static std::string repeatCommand(const std::vector<std::string>& args) {
    // Account for repeated commands.
    // std::cout << "This command is a repeat of prior commands.\n";
    return "Repeat acknowledged";
}

// Author: Lexa Hope.
// Note: Command for changing the power level.
static std::string zeroPowerLevelCommand(const std::vector<std::string>& args) {
    // Change the power level internally to zero.
    powerLevel = 0;
    // std::cout << "Power level set to 0.\n";
    return "Power level set to 0";
}

// Author: Lexa Hope.
// Note: Command for changing the power level.
static std::string onePowerLevelCommand(const std::vector<std::string>& args) {
    // Change the power level internally to one.
    powerLevel = 1;
    // std::cout << "Power level set to 1.\n";
    return "Power level set to 1";
}

// Author: Lexa Hope.
// Note: Command for changing the power level.
static std::string twoPowerLevelCommand(const std::vector<std::string>& args) {
    // Change the power level internally to two.
    powerLevel = 2;
    // std::cout << "Power level set to 2.\n";
    return "Power level set to 2";
}

// Author: Lexa Hope.
// Note: Command for changing the power level.
static std::string threePowerLevelCommand(const std::vector<std::string>& args) {
    // Change the power level internally to three.
    powerLevel = 3;
    // std::cout << "Power level set to 3.\n";
    return "Power level set to 3";
}

// Author: Lexa Hope.
// Note: Command for changing the power level.
static std::string fourPowerLevelCommand(const std::vector<std::string>& args) {
    // Change the power level internally to four.
    powerLevel = 4;
    // std::cout << "Power level set to 4.\n";
    return "Power level set to 4";
}

// Author: Lexa Hope.
// Note: Command for changing the power level.
static std::string fivePowerLevelCommand(const std::vector<std::string>& args) {
    // Change the power level internally to five.
    powerLevel = 5;
    // std::cout << "Power level set to 5.\n";
    return "Power level set to 5";
}

// Author: Lexa Hope.
// Note: Command for changing the power level.
static std::string sixPowerLevelCommand(const std::vector<std::string>& args) {
    // Change the power level internally to six.
    powerLevel = 6;
    // std::cout << "Power level set to 6.\n";
    return "Power level set to 6";
}

// Author: Lexa Hope.
// Note: Command for changing the power level.
static std::string sevenPowerLevelCommand(const std::vector<std::string>& args) {
    // Change the power level internally to seven.
    powerLevel = 7;
    // std::cout << "Power level set to 7.\n";
    return "Power level set to 7";
}

// Author: Lexa Hope.
// Note: Command for changing the power level.
static std::string eightPowerLevelCommand(const std::vector<std::string>& args) {
    // Change the power level internally to eight.
    powerLevel = 8;
    // std::cout << "Power level set to 8.\n";
    return "Power level set to 8";
}

// Author: Lexa Hope.
// Note: Command for changing the power level.
static std::string ninePowerLevelCommand(const std::vector<std::string>& args) {
    // Change the power level internally to nine.
    powerLevel = 9;
    // std::cout << "Power level set to 9.\n";
    return "Power level set to 9";
}

// Author: Lexa Hope.
// Note: Command for changing state of receiver which will be used in the diagram.
static std::string changeStateCommand(const std::vector<std::string>& args) {
    // If the code is in the correct state, change the state of the receiver.
    if (poweredOn && powerLevel >= 5) {  
        // Output for file has a change to it.
        const std::string stateChangeOutput = "Pressed";

        // Create file name.
        std::filesystem::path stateFile = resourcesLoc + "keyTracker.text";

        // Create users input that can be picked up by another program which will use the data to change it state.
        std::ofstream file;
        file.open(stateFile.string(), std::ios::out);

        // Check if file is open.
        if (!file.is_open()) {
            // std::cout << "Error opening file for writing: " << stateFile.string() << "\n";
            return "Error opening file";
        } else { // Write the state change output to the file.
            file << stateChangeOutput;
            file.close();
            // std::cout << "State changed and written to file.\n";
            return "State changed";
        }
    }
    return "Conditions not met for state change";
}

// Author: Lexa Hope.
// Note: Command for powering on receiver.
static std::string powerCommand(const std::vector<std::string>& args) {
    // Change power settings.
    if (!poweredOn) {
        poweredOn = true;
        // std::cout << "Receiver powered on.\n";
        return "Powered on";
    } else {
        poweredOn = false;
        // std::cout << "Receiver powered off.\n";
        return "Powered off";
    }
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
    commands["led_on"] = commandLedOn;
    commands["led_off"] = commandLedOff;

    // Define commands for compatiblity with the CAFNT project. (Author: Lexa Hope)
    // Comands for help.
    commands["st/rept"] = commandHelp;
    commands["func/stop"] = exitCommand;

    // Commands for repeating commands.
    commands["repeat!"] = repeatCommand;

    // Commands for adjusting the power levels.
    commands["0"] = zeroPowerLevelCommand;
    commands["1"] = onePowerLevelCommand;
    commands["2"] = twoPowerLevelCommand;
    commands["3"] = threePowerLevelCommand;
    commands["4"] = fourPowerLevelCommand;
    commands["5"] = fivePowerLevelCommand;
    commands["6"] = sixPowerLevelCommand;
    commands["7"] = sevenPowerLevelCommand;
    commands["8"] = eightPowerLevelCommand;
    commands["9"] = ninePowerLevelCommand;

    // Commands for powering on receiver from standby mode.
    commands["power"] = powerCommand;

    // Commands for changing the state of the receiver.
    commands["vol+"] = changeStateCommand;
    commands["vol-"] = changeStateCommand;
    commands["fast back"] = changeStateCommand;
    commands["fast forward"] = changeStateCommand;
    commands["pause"] = changeStateCommand;
    commands["down"] = changeStateCommand;
    commands["up"] = changeStateCommand;
    commands["eq"] = changeStateCommand;
    
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
