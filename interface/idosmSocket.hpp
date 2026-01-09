#ifndef IDOSM_SOCKET_HPP
#define IDOSM_SOCKET_HPP

#include "dosmBasic.hpp"

namespace dosm
{
    class IDosmSocket
    {
        public:
            virtual ~IDosmSocket(void) = default;

            virtual void init(void) = 0;
            virtual void release(void) = 0;

            virtual i32_t send(const buf_t buf, idx_t len) 
            {
                (void)buf;
                (void)len;
                return -1;
            }

            virtual i32_t recv(buf_t buf, idx_t maxlen)
            {
                (void)buf;
                (void)maxlen;
                return -1;
            }
    };
} // namespace dosm

#endif // IDOSM_SOCKET_HPP
