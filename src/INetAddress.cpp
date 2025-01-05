//
// Created by jhlod cui on 2024/12/23.
//
// sin是 “socket internet” 的缩写
// hs 主机字节序 ns 网络字节序
#include "INetAddress.h"

INetAddress::INetAddress(uint16_t port, const std::string &ip)
{
    bzero(&addr_, sizeof &addr_);
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    // 用于将主机字节序（host - endian）的无符号短整型（16 位）数据转换为网络字节序（network - endian）
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());
}

std::string INetAddress::toIp() const
{
    char buffer[64] = {0};
    ::inet_ntop(AF_INET, &addr_.sin_addr, buffer, sizeof buffer);
    return {buffer};
}

uint16_t INetAddress::toPort() const
{
    return ntohs(addr_.sin_port);
}

std::string INetAddress::toIpPort() const
{
    char buffer[64] = {0};
    ::inet_ntop(AF_INET, &addr_.sin_addr, buffer, sizeof buffer);
    size_t end = strlen(buffer);
    uint16_t port = ntohs(addr_.sin_port);
    sprintf(buffer + end, ":%u", port);
    return buffer;
}