// datalog_player.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "dataLog.h"
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	dataLog test_log;
	test_log.load("test.csv");

	std::ofstream srt_out;
	srt_out.open("test.srt");
	test_log.to_srt(srt_out);
	srt_out.close();

	while (true)
	{
		/*
		cout << "Enter a time:" << endl;
		double time;
		cin >> time;

		cout << "Data point: " 
			<< test_log.closest_last_datapoint(time) << endl;
			*/
		double time;
		string field;
		cout << "Enter data time: " << endl;
		cin >> time;
		cout << "Enter field name: " << endl;
		getline(cin >> ws, field);
		string value = test_log.get_value(field, time);
		cout << "Value is :" << value << endl;
	
	}

    return 0;
}

