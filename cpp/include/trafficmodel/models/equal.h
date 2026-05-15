#pragma once

#include <cstdint>
#include <vector>

#include <trafficmodel/traffic_model.h>

class equal_traffic_model final : public traffic_model {
public:
	equal_traffic_model(double interval_seconds, std::uint64_t packet_size_bytes);

	[[nodiscard]]
	std::vector<traffic_packet> generate(double simulation_time_seconds) override;

	[[nodiscard]]
	double interval_seconds() const noexcept;

	[[nodiscard]]
	std::uint64_t packet_size_bytes() const noexcept;

private:
	double m_interval_seconds = 0.0;
	std::uint64_t m_packet_size_bytes = 0;
};
