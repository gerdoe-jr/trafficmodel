#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct input_config {
	double m_simulation_time_seconds = 0.0;
	std::string m_model_name;
	std::vector<double> m_parameters;
};

[[nodiscard]]
input_config read_input_config(const std::string &input_file_path);
