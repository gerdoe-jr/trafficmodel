#include <cstdint>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include <trafficmodel/csv_writer.h>
#include <trafficmodel/input_config.h>
#include <trafficmodel/models/equal.h>
#include <trafficmodel/models/poisson.h>

namespace {

std::unique_ptr<traffic_model> create_model(const input_config &config, poisson_traffic_model::seed_type seed) {
	if(config.m_model_name == "equal" || config.m_model_name == "uniform") {
		if(config.m_parameters.size() != 2) {
			throw std::invalid_argument(
				"equal model requires 2 parameters: interval_seconds packet_size_bytes");
		}

		return std::make_unique<equal_traffic_model>(
			config.m_parameters[0], static_cast<std::uint64_t>(config.m_parameters[1]));
	}

	if(config.m_model_name == "poisson") {
		if(config.m_parameters.size() != 2) {
			throw std::invalid_argument(
				"poisson model requires 2 parameters: mean_interval_seconds mean_packet_size_bytes");
		}

		return std::make_unique<poisson_traffic_model>(config.m_parameters[0], config.m_parameters[1], seed);
	}

	throw std::invalid_argument("unknown traffic model: " + config.m_model_name);
}

std::string default_output_path(const std::string &input_file_path) {
	const std::filesystem::path input_path{input_file_path};
	const std::filesystem::path stem = input_path.stem();

	return (stem.string() + "_traffic.csv");
}

} // namespace

int main(int argc, char *argv[]) {
	try {
		if(argc < 2 || argc > 4) {
			throw std::invalid_argument("usage: traffic_model <input_file> [output_csv] [seed]");
		}

		const std::string input_file_path = argv[1];
		const std::string output_file_path = argc >= 3 ? argv[2] : default_output_path(input_file_path);

		const auto seed = argc >= 4 ? static_cast<poisson_traffic_model::seed_type>(std::stoul(argv[3])) :
					      poisson_traffic_model::seed_type{42};

		const input_config config = read_input_config(input_file_path);
		auto model = create_model(config, seed);

		const auto packets = model->generate(config.m_simulation_time_seconds);

		write_traffic_csv(output_file_path, packets);

		std::cout << "model: " << config.m_model_name << '\n';
		std::cout << "packets: " << packets.size() << '\n';
		std::cout << "saved: " << output_file_path << '\n';

		return 0;
	} catch(const std::exception &exception) {
		std::cerr << "error: " << exception.what() << '\n';
		return 1;
	}
}
