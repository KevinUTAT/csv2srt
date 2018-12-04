#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>

class dataLog
{
private:
	std::vector<std::string> field_list;
	std::vector<double> timestemp_table;
	std::vector<std::vector<std::string> > log_table;
public:
	dataLog();

	int load(std::string log_file);

	int load_field_list(std::ifstream &log_stream);

	int load_data(std::ifstream &log_stream);

	std::string sec2hms(double time_in);

	int to_srt(std::ofstream &srt_out);
};

