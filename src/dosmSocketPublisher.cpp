#include "dosmSocketPublisher.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace dosm
{
    DosmSocketPublisher::DosmSocketPublisher(const str_t& ip, ui16_t port)
    {
        this->ip = ip;
        this->port = port;
        this->fd = -1;
        this->addr = sockaddr_in{};
    }

    void DosmSocketPublisher::init(void)
    {
        this->fd = (i32_t)::socket(AF_INET, SOCK_DGRAM, 0);
        if (this->fd < 0)
            return;

        this->addr = sockaddr_in{};
        this->addr.sin_family = AF_INET;
        this->addr.sin_port = htons(this->port);
        this->addr.sin_addr.s_addr = ::inet_addr(this->ip.c_str());
    }

    void DosmSocketPublisher::release(void)
    {
        if (this->fd >= 0)
        {
            ::close(this->fd);
            this->fd = -1;
        }
    }

    i32_t DosmSocketPublisher::send(const buf_t buf, idx_t len)
    {
        if (this->fd < 0)
            return -1;

        if (buf == nullptr)
            return -1;

        if (len == 0)
            return 0;

        i32_t n = (i32_t)::sendto(
                this->fd,
                (const void*)buf,
                (idx_t)len,
                0,
                (const sockaddr*)&this->addr,
                (socklen_t)sizeof(this->addr)
                );

        return n;
    }

} // namespace dosm





