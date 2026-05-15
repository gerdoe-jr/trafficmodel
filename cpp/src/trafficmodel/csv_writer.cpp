#include <fstream>
#include <iomanip>
#include <stdexcept>

#include <trafficmodel/csv_writer.h>

void write_traffic_csv(const std::filesystem::path &output_file_path, const std::vector<traffic_packet> &packets) {
	const std::filesystem::path parent_path = output_file_path.parent_path();

	if(!parent_path.empty()) {
		std::filesystem::create_directories(parent_path);
	}

	std::ofstream csv{output_file_path};

	if(!csv) {
		throw std::runtime_error("cannot open output CSV file: " + output_file_path.string());
	}

	csv << "arrival_time_seconds,size_bytes\n";

	for(const traffic_packet &packet : packets) {
		csv << std::setprecision(12) << packet.m_arrival_time_seconds << ',' << packet.m_size_bytes << '\n';
	}
}
