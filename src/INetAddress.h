//
// Created by jhlod cui on 2024/12/23.
//

#ifndef MYMUDUO_INETADDRESS_H
#define MYMUDUO_INETADDRESS_H


#include <cstdint>
#include<string>
#include <netinet/in.h>
#include <arpa/inet.h>

class INetAddress {
public:
    explicit INetAddress(uint16_t port,const std::string& ip = "127.0.0.1");
    explicit INetAddress(const sockaddr_in &addr):addr_(addr){};

    [[nodiscard]] std::string toIp() const;
    [[nodiscard]] std::string toIpPort() const;

    [[nodiscard]] uint16_t toPort() const;

    [[nodiscard]] const sockaddr_in& getSockAddr() const {return addr_;};

private:
    sockaddr_in addr_{};
};


#endif //MYMUDUO_INETADDRESS_H
