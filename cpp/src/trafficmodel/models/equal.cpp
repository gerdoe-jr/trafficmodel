#include <stdexcept>

#include <trafficmodel/models/equal.h>

equal_traffic_model::equal_traffic_model(double interval_seconds, std::uint64_t packet_size_bytes) :
	m_interval_seconds(interval_seconds), m_packet_size_bytes(packet_size_bytes) {
	if(m_interval_seconds <= 0.0) {
		throw std::invalid_argument("equal model interval must be positive");
	}

	if(m_packet_size_bytes == 0) {
		throw std::invalid_argument("equal model packet size must be positive");
	}
}

std::vector<traffic_packet> equal_traffic_model::generate(double simulation_time_seconds) {
	if(simulation_time_seconds < 0.0) {
		throw std::invalid_argument("simulation time must be non-negative");
	}

	std::vector<traffic_packet> packets;

	for(double time = 0.0; time <= simulation_time_seconds; time += m_interval_seconds) {
		packets.push_back(traffic_packet{.m_arrival_time_seconds = time, .m_size_bytes = m_packet_size_bytes});
	}

	return packets;
}

double equal_traffic_model::interval_seconds() const noexcept { return m_interval_seconds; }

std::uint64_t equal_traffic_model::packet_size_bytes() const noexcept { return m_packet_size_bytes; }
