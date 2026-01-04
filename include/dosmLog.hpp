#ifndef DOSM_LOG_HPP
#define DOSM_LOG_HPP

#include "dosmBasic.hpp"
#include <iostream>

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
		if (static_cast<ui8_t>(LVL) <= static_cast<ui8_t>(dosm::CURRENT_LOG_LVL))                                    \
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
	} while (0)                                                                                                     \

#define DOSM_PROGRESS(label, done, total, elapsed)                                                                                        \
	do {                                                                                                                                   \
		if ((total) == 0) break;                                                                                                            \
		dosm::ui8_t pct = static_cast<dosm::ui8_t>((100 * (done)) / (total));                                                               \
		std::cout << "\r[DOSM] " << dosm::str_t(label) << ": " << dosm::str_t(((done) == (total)) ? "100" : std::to_string(pct)) << "%";    \
		if ((elapsed) >= 0)                                                                                                                 \
		{                                                                                                                                   \
			dosm::ui64_t t = static_cast<dosm::ui64_t>(elapsed);                                                                             \
			dosm::ui64_t display = 0;                                                                                                        \
			if ((done) > 0 && (done) < (total)) display = t * static_cast<dosm::ui64_t>((total) - (done)) / static_cast<dosm::ui64_t>(done); \
			else display = t;                                                                                                                \
			dosm::ui64_t h = display / 3600;                                                                                                 \
			dosm::ui64_t m = (display % 3600) / 60;                                                                                          \
			dosm::ui64_t s = display % 60;                                                                                                   \
			std::cout << " | "                                                                                                               \
			<< (h < 10 ? "0" : "") << h << ":"                                                                                               \
			<< (m < 10 ? "0" : "") << m << ":"                                                                                               \
			<< (s < 10 ? "0" : "") << s;                                                                                                     \
		}                                                                                                                                   \
		std::cout << std::flush;                                                                                                            \
		if ((done) == (total)) std::cout << std::endl;                                                                                      \
	} while (0)                                                                                                                            \

#else
#define DOSM_LOG(LVL, msg) do {} while (0)
#define DOSM_PROGRESS(label, done, total) do {} while (0)
#endif
#define DOSM_LOG_ERROR(msg) DOSM_LOG(dosm::logLVL::ERROR, msg)
#define DOSM_LOG_WARN(msg)  DOSM_LOG(dosm::logLVL::WARN,  msg)
#define DOSM_LOG_INFO(msg)  DOSM_LOG(dosm::logLVL::INFO,  msg)
#define DOSM_LOG_DEBUG(msg) DOSM_LOG(dosm::logLVL::DEBUG, msg)

#endif // DOSM_LOG_HPP


