#pragma once

#ifdef NETWORK_PACKETS_USE_PCH
#include NETWORK_PACKETS_PCH_DIR
#else
#include <iostream>
#include <cstdint>
#include <HBuffer/HBuffer.hpp>
#endif

/// @brief follow type convention
/// char - 1 byte
/// short - 2 bytes
/// int - 4 bytes
/// long - 8 bytes