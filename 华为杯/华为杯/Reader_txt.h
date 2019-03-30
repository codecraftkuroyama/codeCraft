#pragma once

#include<string>
#include<math.h>
#include<iostream>
#include<fstream>
#include<ostream>
#include<vector>

using namespace std;

class reader_text {
public:
	int road_num;
	static int car_num, cross_num;
	int **road;
	int **car;
	int **cross;

	reader_text(char *text_road,char *text_car,char *text_cross);
	~reader_text();

};

int reader_text::car_num = 0;
int reader_text::cross_num = 0;

class count_most_cross
{
public:
	int *count_go_car_num_at_cross[2];
	int *count_arrive_car_num_at_cross[2];
	count_most_cross(int **cross, int **car);
	~count_most_cross();
};






void out_put(vector<vector<int>> car_route, char *anserPath, int car_num)
{
	ofstream write_file;
	write_file.open(anserPath);
	for (int car_order = 0; car_order < car_num; ++car_order)
	{
		write_file << '(';
		for (int road_order = 0; road_order < car_route[car_order].size(); ++road_order)
		{
			write_file << car_route[car_order][road_order];
			if (road_order != car_route[car_order].size() - 1)
				write_file << ", ";
		}
		write_file << ')' << endl;
	}
	write_file.close();
}








count_most_cross::count_most_cross(int **cross, int **car)
{
	count_go_car_num_at_cross[0] = new int[reader_text::cross_num];
	count_arrive_car_num_at_cross[0] = new int[reader_text::cross_num];
	count_go_car_num_at_cross[1] = new int[reader_text::cross_num];
	count_arrive_car_num_at_cross[1] = new int[reader_text::cross_num];
	for (int i = 0; i < reader_text::cross_num; ++i)
	{
		count_go_car_num_at_cross[0][i] = cross[i][0];
		count_arrive_car_num_at_cross[0][i] = cross[i][0];
		count_go_car_num_at_cross[1][i] = 0;
		count_arrive_car_num_at_cross[1][i] = 0;
	}
	for (int i = 0; i < reader_text::car_num; ++i)
	{
		++count_go_car_num_at_cross[1][car[i][1]-1];
		++count_arrive_car_num_at_cross[1][car[i][2]-1];
	}
}


count_most_cross::~count_most_cross()
{
	for (int i = 0; i < 2; ++i)
	{
		delete[]count_go_car_num_at_cross[i];
		delete[]count_arrive_car_num_at_cross[i];
	}
}










reader_text::reader_text(char *text_road, char *text_car, char *text_cross)
{
	//read text
	road_num, cross_num = 0;

	ifstream reader;

	reader.open(text_road);
	int length = 1000;
	char buffer[256];
	while (!reader.eof())
	{
		reader.getline(buffer, 256, '\n');
		++road_num;
	}
	road_num--;
	road = new int *[road_num];
	for (int i = 0; i < road_num; ++i)
	{
		road[i] = new int[7];
	}

	reader.seekg(0, std::ios::end);
	length = reader.tellg();
	reader.seekg(0, std::ios::beg);
	char *data_road = new char[length];
	reader.read(data_road, length);
	reader.close();
	int i = 0;
	for (int road_order = 0; road_order < road_num; ++road_order)
	{
		for (int data_order = 0; data_order < 7; ++data_order)
		{
			while (i < length)
			{
				if (data_road[i] >= '0' && data_road[i] <= '9')
				{
					if (data_road[i + 1] < '0' || data_road[i + 1] > '9')
					{
						road[road_order][data_order] = data_road[i] - '0';
						++i;
						break;
					}
					else if (data_road[i + 2] < '0' || data_road[i + 2] > '9')
					{
						road[road_order][data_order] = (data_road[i] - '0') * 10 + data_road[i + 1] - '0';
						i += 2;
						break;
					}
					else if (data_road[i + 3] < '0' || data_road[i + 3] > '9')
					{
						road[road_order][data_order] = (data_road[i] - '0') * 100 + (data_road[i + 1] - '0') * 10 + data_road[i + 2] - '0';
						i += 3;
						break;
					}
					else if (data_road[i + 4] < '0' || data_road[i + 4] > '9')
					{
						road[road_order][data_order] = (data_road[i] - '0') * 1000 + (data_road[i + 1] - '0') * 100 + (data_road[i + 2] - '0') * 10 + data_road[i + 3] - '0';
						i += 4;
						break;
					}
					else if (data_road[i + 5] < '0' || data_road[i + 5] > '9')
					{
						road[road_order][data_order] = (data_road[i] - '0') * 10000 + (data_road[i + 1] - '0') * 1000 + (data_road[i + 2] - '0') * 100 + (data_road[i + 3] - '0')*10 + data_road[i + 4] - '0';
						i += 5;
						break;
					}
				}
				++i;
			}
		}
	}
	delete[]data_road;



	reader.open(text_car);
	while (!reader.eof())
	{
		reader.getline(buffer, 256, '\n');
		++car_num;
	}
	car_num--;
	car = new int *[car_num];
	for (int i = 0; i < car_num; ++i)
	{
		car[i] = new int[5];
	}

	reader.seekg(0, std::ios::end);
	length = reader.tellg();
	reader.seekg(0, std::ios::beg);
	char *data_car = new char[length];
	reader.read(data_car, length);
	reader.close();

	i = 0;
	for (int car_order = 0; car_order < car_num; ++car_order)
	{
		for (int data_order = 0; data_order < 5; ++data_order)
		{
			while (i < length)
			{
				if (data_car[i] >= '0' && data_car[i] <= '9')
				{
					if (data_car[i + 1] < '0' || data_car[i + 1] > '9')
					{
						car[car_order][data_order] = data_car[i] - '0';
						++i;
						break;
					}
					else if (data_car[i + 2] < '0' || data_car[i + 2] > '9')
					{
						car[car_order][data_order] = (data_car[i] - '0') * 10 + data_car[i + 1] - '0';
						i += 2;
						break;
					}
					else if (data_car[i + 3] < '0' || data_car[i + 3] > '9')
					{
						car[car_order][data_order] = (data_car[i] - '0') * 100 + (data_car[i + 1] - '0') * 10 + data_car[i + 2] - '0';
						i += 3;
						break;
					}
					else if (data_car[i + 4] < '0' || data_car[i + 4] > '9')
					{
						car[car_order][data_order] = (data_car[i] - '0') * 1000 + (data_car[i + 1] - '0') * 100 + (data_car[i + 2] - '0') * 10 + data_car[i + 3] - '0';
						i += 4;
						break;
					}
					else if (data_car[i + 5] < '0' || data_car[i + 5] > '9')
					{
						car[car_order][data_order] = (data_car[i] - '0') * 10000 + (data_car[i + 1] - '0') * 1000 + (data_car[i + 2] - '0') * 100 + ((data_car[i + 3])-'0') * 10 + data_car[i + 4] - '0';
						i += 5;
						break;
					}
					else if (data_car[i + 6] < '0' || data_car[i + 6] > '9')
					{
						car[car_order][data_order] = (data_car[i] - '0') * 100000 + (data_car[i + 1] - '0') * 10000 + (data_car[i + 2] - '0') * 1000 + ((data_car[i + 3]) - '0') * 100 + (data_car[i + 4] - '0') * 10 + data_car[i + 5] - '0';
						i += 6;
						break;
					}
				}
				++i;
			}
		}
	}

	delete[]data_car;

	/*for (int i = 0; i < car_num; i++)
	{
		cout << car[i][0] << endl;
		getchar();
	}*/


	reader.open(text_cross);
	while (!reader.eof())
	{
		reader.getline(buffer, 256, '\n');
		++cross_num;
	}
	cross_num--;
	cross = new int *[cross_num];
	for (int i = 0; i < cross_num; ++i)
	{
		cross[i] = new int[5];
	}

	reader.seekg(0, std::ios::end);
	length = reader.tellg();
	reader.seekg(0, std::ios::beg);
	char *data_cross = new char[length];
	reader.read(data_cross, length);

	reader.close();
	i = 0;
	for (int cross_order = 0; cross_order < cross_num; ++cross_order)
	{
		for (int data_order = 0; data_order < 5; ++data_order)
		{
			while (i < length)
			{
				if (data_cross[i] >= '0' && data_cross[i] <= '9')
				{
					if (data_cross[i + 1] < '0' || data_cross[i + 1] > '9')
					{
						cross[cross_order][data_order] = data_cross[i] - '0';
						++i;
						break;
					}
					else if (data_cross[i + 2] < '0' || data_cross[i + 2] > '9')
					{
						cross[cross_order][data_order] = (data_cross[i] - '0') * 10 + data_cross[i + 1] - '0';
						i += 2;
						break;
					}
					else if (data_cross[i + 3] < '0' || data_cross[i + 3] > '9')
					{
						cross[cross_order][data_order] = (data_cross[i] - '0') * 100 + (data_cross[i + 1] - '0') * 10 + data_cross[i + 2] - '0';
						i += 3;
						break;
					}
					else if (data_cross[i + 4] < '0' || data_cross[i + 4] > '9')
					{
						cross[cross_order][data_order] = (data_cross[i] - '0') * 1000 + (data_cross[i + 1] - '0') * 100 + (data_cross[i + 2] - '0') * 10 + data_cross[i + 3] - '0';
						i += 4;
						break;
					}
				}
				++i;
			}
		}
	}

	delete[]data_cross;

};

reader_text::~reader_text()
{
	for (int i = 0; i < road_num; ++i)
	{
		delete[]road[i];
	}
	delete[]road;

	for (int i = 0; i < car_num; ++i)
	{
		delete[]car[i];
	}
	delete[]car;

	for (int i = 0; i < cross_num; ++i)
	{
		delete[]cross[i];
	}
	delete[]cross;
}



