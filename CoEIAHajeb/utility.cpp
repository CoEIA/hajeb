#include <string>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "utility.h"

std::string Utility::IpFromHost(std::string host) {

    std::string address ;
    struct hostent *phe = gethostbyname(host.c_str());

    if (phe == 0) {
        std::cerr << "Yow! Bad host lookup." << std::endl;
        return "";
    }

    for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
        struct in_addr addr;
        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
        address = inet_ntoa(addr);
        break;
    }

    return address;
}
