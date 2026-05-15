#pragma once

#include <cstdint>
#include <random>
#include <vector>

#include <trafficmodel/traffic_model.h>

class poisson_traffic_model final : public traffic_model {
public:
	using seed_type = std::uint32_t;

	poisson_traffic_model(double mean_interval_seconds, double mean_packet_size_bytes, seed_type seed = 67);

	[[nodiscard]]
	std::vector<traffic_packet> generate(double simulation_time_seconds) override;

	[[nodiscard]]
	double mean_interval_seconds() const noexcept;

	[[nodiscard]]
	double mean_packet_size_bytes() const noexcept;

	void reseed(seed_type seed);

private:
	static constexpr double s_seconds_per_millisecond = 0.001;

	double m_mean_interval_seconds = 0.0;
	double m_mean_packet_size_bytes = 0.0;
	std::mt19937 m_rng;
};
