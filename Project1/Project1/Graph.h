#pragma once
#include <vector>
#include <iostream>

#include"Class.h"
using namespace std;
#define INFINITY 65535
class Graph
{
public:
	Graph();
	~Graph();
	void CreateGraph();//创建图
	void MiniSpanTree_Prim();//prim生成树
private:
	vector<char> vexs; //顶点表
	vector<vector<int>>arc;//邻接矩阵
	int numVertexes, numEdges;
};
Graph::Graph() {

}

Graph::~Graph() {

}

void Graph::CreateGraph()
{
	char road_path[] = "C:/Users/dell/Desktop/huawei/1-map-training-1/road.txt";
	char car_path[] = "C:/Users/dell/Desktop/huawei/1-map-training-1/car.txt";
	char cross_path[] = "C:/Users/dell/Desktop/huawei/1-map-training-1/cross.txt";
	reader_text reader(road_path, car_path, cross_path);
	
	numVertexes = reader.cross_num;
	numEdges = reader.road_num;
	int i, j, w;
	char ch;
	for (i = 0; i < numVertexes; i++)
	{
		//cin >> ch;
		ch = reader.cross[i][0];
		vexs.push_back(ch);
	}
	arc.resize(numVertexes, vector<int>(numVertexes));
	for (i = 0; i < numVertexes; i++) //初始化邻接矩阵
		for (j = 0; j < numVertexes; j++)
		{
			if (i != j)arc[i][j] = INFINITY;
			else arc[i][j] = 0;
		}

	//cout << "输入边( vi ，vj)上的下标i,下标j,和权w :" << endl;
	for (int k = 0; k < numEdges; k++)
	{
		cout << reader.road[k][4] << " " << reader.road[k][5] << " " << reader.road[k][2] << endl;
		if (reader.road[k][6] == 1)
		{
			i = reader.road[k][4] - 1;
			j = reader.road[k][5] - 1;
			w = reader.road[k][2] - 1;
			
			cout << i << "|" << j << "|" << w << endl;
			arc[i][j] = arc[j][i] = w;
		}
		
	}
}

void Graph::MiniSpanTree_Prim()//Prim 算法生成最小生成树
{
	vector<int> adjvex;//保存顶点下标
	vector<int> lowcost;//保存顶点间边的权值
	int min, j, k;
	lowcost.push_back(0);
	adjvex.push_back(0);
	for (int i = 1; i < numVertexes; i++)
	{
		lowcost.push_back(arc[0][i]);
		adjvex.push_back(0);
	}
	for (int i = 1; i < numVertexes; i++)
	{
		min = INFINITY;
		j = 1; k = 0;
		while (j < numVertexes)
		{
			if (lowcost[j] != 0 && lowcost[j] < min)
			{
				min = lowcost[j];
				k = j;
			}
			j++;
		}
		cout << "(" << adjvex[k] + 1  << "<->" << k + 1 << ")" << endl;
		lowcost[k] = 0;
		for (j = 1; j < numVertexes; j++)
		{
			if (lowcost[j] != 0 && arc[k][j] < lowcost[j])
			{
				lowcost[j] = arc[k][j];
				adjvex[j] = k;
			}
		}
	}
}
