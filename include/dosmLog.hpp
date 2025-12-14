#ifndef DOSM_LOG_HPP
#define DOSM_LOG_HPP

#include <iostream>
#include <string>

namespace dosm
{
	enum class logLVL
	{
		ERROR = 0,
		WARN,
		INFO,
		DEBUG
	};
#ifdef DOSM_ENABLE_LOG
	inline logLVL CURRENT_LOG_LVL = logLVL::INFO;
#endif
}

#ifdef DOSM_ENABLE_LOG
#define DOSM_LOG(LVL, msg)                                                                                         \
	do {                                                                                                            \
		if (static_cast<int>(LVL) <= static_cast<int>(dosm::CURRENT_LOG_LVL))                                        \
		{                                                                                                            \
			std::ostream& os = ((LVL == dosm::logLVL::ERROR) || (LVL == dosm::logLVL::WARN)) ? std::cerr : std::cout; \
			os << "[DOSM] ";                                                                                          \
			switch (LVL)                                                                                              \
			{                                                                                                         \
				case dosm::logLVL::ERROR: os << "ERROR "; break;                                                       \
				case dosm::logLVL::WARN:  os << "WARN  "; break;                                                       \
				case dosm::logLVL::INFO:  os << "INFO  "; break;                                                       \
				case dosm::logLVL::DEBUG: os << "DEBUG "; break;                                                       \
			}                                                                                                         \
			os << msg << std::endl;                                                                                   \
		}                                                                                                            \
	} while (0)
#else
#define DOSM_LOG(LVL, msg) do {} while (0)
#endif
#define DOSM_LOG_ERROR(msg) DOSM_LOG(dosm::logLVL::ERROR, msg)
#define DOSM_LOG_WARN(msg)  DOSM_LOG(dosm::logLVL::WARN,  msg)
#define DOSM_LOG_INFO(msg)  DOSM_LOG(dosm::logLVL::INFO,  msg)
#define DOSM_LOG_DEBUG(msg) DOSM_LOG(dosm::logLVL::DEBUG, msg)

#endif // DOSM_LOG_HPP
