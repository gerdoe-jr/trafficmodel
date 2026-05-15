#include <trafficmodel/input_config.h>

#include <algorithm>
#include <cctype>
#include <fstream>
#include <stdexcept>
#include <string>

namespace {

std::string normalize_model_name(std::string value) {
	if(!value.empty() && value.back() == ':') {
		value.pop_back();
	}

	std::ranges::transform(value, value.begin(),
		[](unsigned char character) { return static_cast<char>(std::tolower(character)); });

	return value;
}

} // namespace

input_config read_input_config(const std::string &input_file_path) {
	std::ifstream input{input_file_path};

	if(!input) {
		throw std::runtime_error("cannot open input file: " + input_file_path);
	}

	input_config config;

	if(!(input >> config.m_simulation_time_seconds)) {
		throw std::invalid_argument("cannot read simulation time");
	}

	if(config.m_simulation_time_seconds < 0.0 || config.m_simulation_time_seconds > 3600.0) {
		throw std::invalid_argument("simulation time must be in range [0, 3600]");
	}

	if(!(input >> config.m_model_name)) {
		throw std::invalid_argument("cannot read model name");
	}

	config.m_model_name = normalize_model_name(config.m_model_name);

	double parameter = 0.0;

	while(input >> parameter) {
		config.m_parameters.push_back(parameter);
	}

	return config;
}
