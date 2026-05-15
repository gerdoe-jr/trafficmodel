#pragma once

#include <cstdint>

struct traffic_packet {
	double m_arrival_time_seconds = 0.0;
	std::uint64_t m_size_bytes = 0;
};
