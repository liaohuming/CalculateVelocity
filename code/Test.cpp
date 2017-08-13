// By: Huming Liao, 2016-9-25 17:34:38
// At: CASE, Cleveland, OHio, 44120

#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

struct RangeBox
{
	vector <double> v;
	double y_coordinate;
	double y_v;
	double x_lower;
	double x_upper;
	double y_lower;
	double y_upper;	
};

struct Point
{
	double x;
	double y;
	double v;
};

int main(int argc, char* argv[])
{
	int flag = 1;
	while (flag)
	{
		double xLower = 297;
		double xUpper = 303;
		double yLower = 0.0;
		double yUpper = 200.0;
		int nodeNum = 40;
		vector <double> y;
		vector <RangeBox> rangeBox_vector;

		cout << "Please input following informations!" << endl;
		cout << "(1)input_csv_file_name: ";
		char inputFileName[255] = { 0 };
		cin >> inputFileName;
		char outputFileName[255] = { 0 };
		strcpy(outputFileName, inputFileName);
		strcat(inputFileName, ".csv");
		strcat(outputFileName, "_out.csv");

		cout << "(2)xLower: ";
		cin >> xLower;

		cout << "(3)xUpper: ";
		cin >> xUpper;

		cout << "(4)yLower: ";
		cin >> yLower;

		cout << "(5)yUpper: ";
		cin >> yUpper;

		cout << "(6)nodeNum: ";
		cin >> nodeNum;

		double delta_y = (yUpper - yLower) / nodeNum;
		for (int i = 1; i <= nodeNum; i++)
		{
			RangeBox rb_i;
			rb_i.y_coordinate = yLower + ((2 * i - 1) / 2.0)*delta_y;
			rb_i.x_lower = xLower;
			rb_i.x_upper = xUpper;
			//rb_i.y_lower = yLower + (i - 1)*delta_y;
			//rb_i.y_upper = yLower + i*delta_y;
			rb_i.y_lower = rb_i.y_coordinate - 1.5*delta_y;
			rb_i.y_upper = rb_i.y_coordinate + 1.5*delta_y;
			rangeBox_vector.push_back(rb_i);
		}

		ifstream inputFile(inputFileName);
		string lineStr;
		getline(inputFile, lineStr); //jump first line which is the title of the data
		getline(inputFile, lineStr);
		vector <Point> pointSet;

		while (!inputFile.eof())
		{
			Point point;
			sscanf(lineStr.c_str(), "%*d,%lf,%*lf,%*lf,%*lf,%*lf,%*lf,%*lf,%*lf,%*lf,%lf,%lf,%*lf", &(point.v), &(point.x), &(point.y));
			pointSet.push_back(point);
			getline(inputFile, lineStr);
		}

		for (int i = 0; i < nodeNum; i++)
		{
			for (int j = 0; j < pointSet.size(); j++)
			{
				if (rangeBox_vector[i].x_lower <= pointSet[j].x && pointSet[j].x <= rangeBox_vector[i].x_upper)
				{
					if (rangeBox_vector[i].y_lower <= pointSet[j].y  && pointSet[j].y <= rangeBox_vector[i].y_upper)
					{
						rangeBox_vector[i].v.push_back(pointSet[j].v);
					}
				}
			}
		}

		for (int i = 0; i < nodeNum; i++)
		{
			double y_v_temp = 0.0;
			for (int j = 0; j < rangeBox_vector[i].v.size(); j++)
			{
				double temp = 0.0;
				temp = rangeBox_vector[i].v[j];
				y_v_temp += temp;
			}
			y_v_temp = y_v_temp / rangeBox_vector[i].v.size();
			rangeBox_vector[i].y_v = y_v_temp;
		}

		ofstream ofs;
		ofs.open(outputFileName, ios::out | ios::trunc);
		for (int i = 0; i < nodeNum; i++)
		{
			double x_coordinate = (rangeBox_vector[i].x_lower + rangeBox_vector[i].x_upper) / 2.0;
			ofs << x_coordinate << "," << rangeBox_vector[i].y_coordinate << "," << rangeBox_vector[i].y_v << endl;
			cout << "x = " << x_coordinate << " y = " << rangeBox_vector[i].y_coordinate << " v = " << rangeBox_vector[i].y_v << endl;
		}
		ofs.close();

		cout << "**************************************************" << endl;
		cout << "(7)Select an option:" << endl;
		cout << "   1 = Continue" << endl;
		cout << "   0 = Exit" << endl;
		cout << "Enter the number: ";
		cin >> flag;
	}

	return 0;
}