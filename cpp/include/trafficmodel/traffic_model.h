#pragma once

#include <vector>

#include <trafficmodel/traffic_packet.h>

class traffic_model {
public:
	virtual ~traffic_model() = default;

	[[nodiscard]]
	virtual std::vector<traffic_packet> generate(double simulation_time_seconds) = 0;
};
