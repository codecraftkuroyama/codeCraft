
#include"Reader_txt.h"
#include"Graph.h"

using namespace std;



int main()
{
	char road_path[] = "E:/huawei_code/exam_1/road.txt";
	char car_path[] = "E:/huawei_code/exam_1/car.txt";
	char cross_path[] = "E:/huawei_code/exam_1/cross.txt";
	char anserPath[] = "E:/huawei_code/anser.txt";

	reader_text reader(road_path, car_path, cross_path);
	cout << reader.car_num << "   " << reader.cross_num << "  " << reader.road_num << endl;
	vector<vector<int>> car_route;								//路

	int **New_road_weight = new int *[reader_text::cross_num];							//路径邻接矩阵
	for (int i = 0; i < reader_text::cross_num; ++i)
	{
		New_road_weight[i] = new int[reader_text::cross_num];
	}
	int **d = new int *[reader_text::cross_num];							//中间矩阵
	for (int i = 0; i < reader_text::cross_num; ++i)
	{
		d[i] = new int[reader_text::cross_num];
	}
	int **route = new int *[reader_text::cross_num];									//同一点的多条路径
	for (int i = 0; i < reader_text::cross_num; ++i)
	{
		route[i] = new int[reader_text::cross_num];
	}
	int max_cross=0;
	for (int i = 0; i < reader.cross_num; ++i)
	{
		if (max_cross < reader.cross[i][0])
			max_cross = reader.cross[i][0];
	}
	vector<int>map(max_cross+1);
	vector<int>comap(reader.cross_num);
	for (int i = 0; i < reader.cross_num; ++i)									//////////////////////////////////////
	{
		map[reader.cross[i][0]] = i;
		comap[i] = reader.cross[i][0];
	}
	//count_most_cross count(reader.cross, reader.car);

	

	Graph G(reader.cross_num, reader.road_num, reader.road, reader.cross, map);
	G.CreateGraph();
	cout<<"mini_trss: "<<endl;
	G.MiniSpanTree_Prim();
	/*for (int i = 0; i < G.create_road.size(); ++i)
	{
		for (int j = 0; j < 7; ++j)
			cout << G.create_road[i][j] << " ";
		cout << endl;
	}
	//out_put(reader.road);*/
	//getchar();
	

	for (int i = 0; i < reader.cross_num; i++)
	{
		for (int j = 0; j < reader.cross_num; j++)
			New_road_weight[i][j] = INF;
	}
	for (int i = 0; i < G.create_road.size(); i++)										/////////////////
	{
		int begining = G.create_road[i][4];
		int end = G.create_road[i][5];
		int a = G.create_road[i][1];
		New_road_weight[map[begining]][map[end]] = a;
		if (G.create_road[i][6])
			New_road_weight[map[end]][map[begining]] = a;
	}
	
	G.findCircleRoad();
	int **count_road = new int *[reader.road_num];
	for (int i = 0; i < reader.road_num; i++)
	{
		count_road[i] = new int[2];
	}
	for (int i = 0; i < reader.road_num; i++)
	{
		count_road[i][0] = reader.road[i][0];
		count_road[i][1] = 0;
	}

	for (int car_order = 0; car_order < reader.car_num; ++car_order)
	{
		cout << car_order << "  ";
		int num_route = 1;//输入你要找多少条路线
		int count_weight = 6000;//输入你想要次数占的权重，最后是拿次数除以它

		//cout << reader.car[car_order][1] << "|" << reader.car[car_order][2] << endl;
		//cout << reader.car[car_order][0] << endl;
		shortRoute(New_road_weight, reader.car[car_order][1], reader.car[car_order][2], d, car_route, reader.car[car_order][0], reader.road, reader.road_num, G.loop, G.create_road, num_route, count_road, count_weight, map,comap);
		//getchar();
	}
	

	vector<int> car_route_num(reader.road_num);
	/*for (int car_order = 0; car_order < reader.car_num; ++car_order)
	{
		for (int data_order = 1; data_order < car_route[car_order].size(); data_order++)
			car_route_num[car_route[car_order][data_order]-5000]++;
	}
	for (int i = 0; i < reader.road_num; ++i)
	{
		cout << i+5000<<"  "<< car_route_num[i]<<endl;
	}*/

	
	

	vector<int> car_time(reader.car_num);
	time_cost(car_time, car_route, reader.car, reader.road, reader.road_num);
	create_time(car_route, reader.car_num, reader.car, car_time, reader.road_num,reader.road, reader.cross_num, reader.cross);
	/*for (int car_order = 0; car_order < reader.car_num; ++car_order)
	{
		for (int data_order = 0; data_order < car_route[car_order].size(); data_order++)
			cout << car_route[car_order][data_order] << " ";
		cout << endl;
	}*/

	out_put(car_route, anserPath, reader.car_num);


	for (int i = 0; i < reader.cross_num; ++i)
	{
		delete[]New_road_weight[i];
		delete[]d[i];
	}
	delete[]New_road_weight;
	delete[]d;
	for (int i = 0; i < reader.cross_num; ++i)
	{
		delete[]route[i];
	}
	delete[]route;
	system("pause");
	return 0;
}





