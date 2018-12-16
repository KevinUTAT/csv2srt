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

	load_maps();

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


int dataLog::load_maps()
{
	// load fields_map
	for (unsigned i = 1; i < field_list.size(); i++)
	{
		fields_map[field_list[i]] = i - 1;
	}

	// load time map
	for (unsigned i = 0; i < timestemp_table.size(); i ++)
	{
		time_map[timestemp_table[i]] = i;
	}

	return 0;
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


unsigned dataLog::get_num_fields()
{
	return field_list.size() - 1;
}


std::string dataLog::get_field_name(unsigned idx)
{
	if (idx > get_num_fields()) return "";
	return field_list[idx + 1];
}


double dataLog::closest_last_datapoint(double time)
{
	double data_point;
	auto iter = time_map.find(time);
	if (iter != time_map.end())
	{
		data_point = time_map[time];
	}
	else
	{
		unsigned start_point = 0;
		unsigned end_point = timestemp_table.size();
		while (true)
		{
			unsigned mid_point = (end_point + start_point) / 2;
			if (mid_point == start_point)  // the closet last point is found
			{
				data_point = timestemp_table[mid_point];
				break;
			}
			if (time < timestemp_table[mid_point])
			{
				end_point = mid_point;
			}
			else if (time > timestemp_table[mid_point])
			{
				start_point = mid_point;
			}
			else
			{
				data_point = timestemp_table[mid_point];
			}
		}
	}
	return data_point;
}


std::string dataLog::get_value(std::string field, double time)
{
	std::string value_0;
	// first find the closest last data point
	double datapoint_time = closest_last_datapoint(time);

	unsigned time_idx = time_map[datapoint_time];
	unsigned field_idx = fields_map[field];

	value_0 = log_table[time_idx][field_idx];

	return value_0;
}