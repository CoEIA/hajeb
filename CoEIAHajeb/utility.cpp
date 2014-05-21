/*
* Copyright (C) 2014 - Center of Excellence in Information Assurance
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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
