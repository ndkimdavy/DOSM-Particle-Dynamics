#ifndef DOSM_SOCKET_PUBLISHER_HPP
#define DOSM_SOCKET_PUBLISHER_HPP

#include "idosmSocket.hpp"
#include <netinet/in.h>

namespace dosm
{
    class DosmSocketPublisher : public IDosmSocket
    {
        public:
            DosmSocketPublisher(const str_t& ip, ui16_t port);
            ~DosmSocketPublisher(void) = default;

            void init(void) override;
            void release(void) override;

            i32_t send(const buf_t buf, idx_t len) override;

        private:
            str_t ip;
            ui16_t port;
            i32_t fd;
            sockaddr_in addr;
    };

} // namespace dosm

#endif // DOSM_SOCKET_PUBLISHER_HPP
