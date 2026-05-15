#pragma once

#include <filesystem>
#include <vector>

#include <trafficmodel/traffic_packet.h>

void write_traffic_csv(const std::filesystem::path& output_file_path, const std::vector<traffic_packet> &packets);
