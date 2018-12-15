#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>

class dataLog
{
private:
	// a lsit of fields of data, note that this first 'field' is "Times(s)"
	// witch is not a valid MP field, so please ignore it we use
	std::vector<std::string> field_list;
	std::vector<double> timestemp_table;
	std::vector<std::vector<std::string> > log_table;
	// the following hash maps are used to enable O1 data look up
	std::unordered_map<std::string, unsigned> fields_map;
	std::unordered_map<double, unsigned> time_map;
public:
	dataLog();

	// loading data structure
	int load(std::string log_file);

	// loading a list of fields that captured by the log
	int load_field_list(std::ifstream &log_stream);

	// load all log data
	int load_data(std::ifstream &log_stream);

	// load 2 look up maps
	int load_maps();

	// helper function to convert time in second
	// to hours:minuses:seconds,milliseconds
	std::string sec2hms(double time_in);


	// generating a .srt file 
	int to_srt(std::ofstream &srt_out);

	// accessing a list of fields in the log
	// they will ignore the time
	unsigned get_num_fields();
	std::string get_field_name(unsigned idx);

	// accessing datas ==================================================

	// helper function that determine
	// the closest last data point given a time in second
	// this operation runs in O[log(n)] time
	double closest_last_datapoint(double time);

	// get a value for a field at a times temp
	std::string get_value(std::string field, double time);
	
};

