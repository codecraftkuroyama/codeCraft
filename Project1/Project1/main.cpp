#include <iostream>

#include"Class.h"
#include"Graph.h"

using namespace std;



int main()
{
	/*char road_path[] = "C:/Users/dell/Desktop/huawei/1-map-training-1/road.txt";
	char car_path[] = "C:/Users/dell/Desktop/huawei/1-map-training-1/car.txt";
	char cross_path[] = "C:/Users/dell/Desktop/huawei/1-map-training-1/cross.txt";
	reader_text reader(road_path, car_path, cross_path);*/

	Graph G;
	G.CreateGraph();
	cout << "最小生成树：" << endl;
	G.MiniSpanTree_Prim();
	
	//out_put(reader.road);
	//getchar();
	system("pause");
	return 0;
}
