#include <trafficmodel/models/poisson.h>

#include <algorithm>
#include <cmath>
#include <stdexcept>

poisson_traffic_model::poisson_traffic_model(
	double mean_interval_seconds, double mean_packet_size_bytes, seed_type seed) :
	m_mean_interval_seconds(mean_interval_seconds), m_mean_packet_size_bytes(mean_packet_size_bytes), m_rng(seed) {
	if(m_mean_interval_seconds <= 0.0) {
		throw std::invalid_argument("poisson model mean interval must be positive");
	}

	if(m_mean_packet_size_bytes <= 0.0) {
		throw std::invalid_argument("poisson model mean packet size must be positive");
	}
}

std::vector<traffic_packet> poisson_traffic_model::generate(double simulation_time_seconds) {
	if(simulation_time_seconds < 0.0) {
		throw std::invalid_argument("simulation time must be non-negative");
	}

	const double mean_interval_ms = m_mean_interval_seconds / s_seconds_per_millisecond;

	std::poisson_distribution<std::uint64_t> interval_distribution{mean_interval_ms};
	std::exponential_distribution<double> size_distribution{1.0 / m_mean_packet_size_bytes};

	std::vector<traffic_packet> packets;

	double time = 0.0;

	while(time <= simulation_time_seconds) {
		const double raw_size = size_distribution(m_rng);
		const auto packet_size = static_cast<std::uint64_t>(std::max(1.0, std::round(raw_size)));

		packets.push_back(traffic_packet{.m_arrival_time_seconds = time, .m_size_bytes = packet_size});

		std::uint64_t interval_ms = 0;

		while(interval_ms == 0) {
			interval_ms = interval_distribution(m_rng);
		}

		time += static_cast<double>(interval_ms) * s_seconds_per_millisecond;
	}

	return packets;
}

double poisson_traffic_model::mean_interval_seconds() const noexcept { return m_mean_interval_seconds; }

double poisson_traffic_model::mean_packet_size_bytes() const noexcept { return m_mean_packet_size_bytes; }

void poisson_traffic_model::reseed(seed_type seed) { m_rng.seed(seed); }
