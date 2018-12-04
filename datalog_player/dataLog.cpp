#include "stdafx.h"
#include "dataLog.h"
#include <sstream>
#include <iostream>


dataLog::dataLog() {}


int dataLog::load(std::string log_file)
{
	std::ifstream log_stream(log_file);

	// check for empty file
	if (log_stream.peek() == std::ifstream::traits_type::eof())
	{
		return 1;
	}

	load_field_list(log_stream);

	load_data(log_stream);

	return 0;
}

int dataLog::load_field_list(std::ifstream &log_stream)
{
	// the list of field in the log is just the first line
	std::string field_list_str;
	if (!std::getline(log_stream, field_list_str)) return 1;

	std::stringstream lineStream(field_list_str);
	std::string cell_str;

	while (std::getline(lineStream, cell_str, ','))
	{
		field_list.push_back(cell_str);
	}

	return 0;
}

int dataLog::load_data(std::ifstream &log_stream)
{
	std::string line_str;
	unsigned i = 0;
	while (std::getline(log_stream, line_str))
	{
		std::stringstream lineStream(line_str);
		std::string cell_str;

		std::getline(lineStream, cell_str, ',');
		timestemp_table.push_back(std::atof(cell_str.c_str()));

		std::vector<std::string> temp_vec;
		while (std::getline(lineStream, cell_str, ','))
		{
			temp_vec.push_back(cell_str);
		}

		log_table.push_back(temp_vec);
		i++;
	}
	return 0;
}


std::string dataLog::sec2hms(double time_in)
{
	unsigned hours = time_in / 3600;
	unsigned mins = (time_in - hours * 3600) / 60;
	unsigned secs = time_in - hours * 3600 - mins * 60;
	unsigned msecs = (time_in - hours * 3600 - mins * 60 - secs) * 1000;

	return std::to_string(hours) + ":" + std::to_string(mins) + ":" 
		+ std::to_string(secs) + "," + std::to_string(msecs);
}

int dataLog::to_srt(std::ofstream &srt_out)
{
	for (int i = 0; i < (timestemp_table.size()-1); i++)
	{
		srt_out << i + 1 << "\n";
		srt_out << sec2hms(timestemp_table[i]) << " --> " 
			<< sec2hms(timestemp_table[i + 1]) << "\n";
		for (int j = 0; j < (field_list.size()-1); j++)
		{
			srt_out << field_list[j+1] << ": " << log_table[i][j] << "\n";
		}
		srt_out << "\n";
	}
	return 0;
}
