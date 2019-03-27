#pragma once
#include <vector>
#include <iostream>
#include<stack>
#include<queue>

#include"Class.h"
using namespace std;
#define INF 65535
class Graph
{
public:
	Graph();
	~Graph();
	void CreateGraph();//创建图
	void MiniSpanTree_Prim();//prim生成树
	void findCircuit(int k, int parent);//找一张图
	void findCircleRoad();//找所有图
	
private:
	vector<char> vexs; //顶点表
	vector<vector<int>>arc;//邻接矩阵
	vector<int>marked;//DFS遍历判断
	vector<vector<int>>prim;
	vector<int>path;
	int numVertexes, numEdges;
	int stackCount;
};
Graph::Graph() {

}

Graph::~Graph() {

}

void Graph::CreateGraph()
{
	stackCount = 0;

	char road_path[] = "C:/Users/dell/Desktop/huawei/1-map-training-1/road.txt";
	char car_path[] = "C:/Users/dell/Desktop/huawei/1-map-training-1/car.txt";
	char cross_path[] = "C:/Users/dell/Desktop/huawei/1-map-training-1/cross.txt";
	reader_text reader(road_path, car_path, cross_path);
	
	numVertexes = reader.cross_num;
	numEdges = reader.road_num;

	marked.resize(numVertexes, 0);
	path.resize(numVertexes, 0);

	int i, j, w;
	char ch;
	for (i = 0; i < numVertexes; i++)
	{
		//cin >> ch;
		ch = reader.cross[i][0];
		vexs.push_back(ch);
	}
	arc.resize(numVertexes, vector<int>(numVertexes));
	prim.resize(numVertexes, vector<int>(numVertexes));
	for (i = 0; i < numVertexes; i++) //初始化邻接矩阵
		for (j = 0; j < numVertexes; j++)
		{
			if (i != j) 
			{ 
				arc[i][j] = INF;
				prim[i][j] = 0;
			}
			else 
			{ 
				arc[i][j] = 0; 
				prim[i][j] = 0;
			}
		}

	//cout << "输入边( vi ，vj)上的下标i,下标j,和权w :" << endl;
	for (int k = 0; k < numEdges; k++)
	{
		cout << reader.road[k][0] << " "<<reader.road[k][4] << " " << reader.road[k][5] << " " << reader.road[k][2] << endl;
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
		min = INF;
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
		prim[adjvex[k]][k] = 1;
		prim[k][adjvex[k]] = 1;
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
	for (int i = 0; i < numVertexes; i++)
	{
		for (int j = 0; j < numVertexes; j++)
		{
			cout << prim[i][j] << "|";
		}
		cout << endl;
	}
};

void Graph::findCircuit(int k, int parent)
{
	path[stackCount] = k;
	stackCount++;
	marked[k] = 1;  //正在访问 
	for (int i = 0; i < numVertexes; i++) 
	{
		if (prim[k][i] == 1 || prim[i][k] == 1) 
		{
			if (marked[i] == 1 && parent != i) 
			{
				for (int j = stackCount - 1;; j--) 
				{
					cout << path[j] << "->";
					if (path[j] == i)
						break;

				}
				cout << endl;
			}
			if (marked[i] == 0)
				findCircuit(i, k);
		}


	}
	marked[k] = -1;  //访问结束 
	stackCount--;
}

void Graph::findCircleRoad()
{
	char road_path[] = "C:/Users/dell/Desktop/huawei/1-map-training-1/road.txt";
	char car_path[] = "C:/Users/dell/Desktop/huawei/1-map-training-1/car.txt";
	char cross_path[] = "C:/Users/dell/Desktop/huawei/1-map-training-1/cross.txt";
	reader_text reader(road_path, car_path, cross_path);
	
	for (int i = 0; i < numEdges; i++)
	{
		int n = reader.road[i][4] - 1;
		int m = reader.road[i][5] - 1;
		if (prim[n][m] == 1 || prim[m][n] == 1)
		{
			cout << i << " " << endl;
		}
		else
		{
			prim[n][m] = 1;
			prim[m][n] = 1;

			cout << "loop : " ;
			for (int j = 0; j < numVertexes; j++)
			{
				if (marked[j] != -1)
				{
					findCircuit(j, -1);
				}
				
			}
			stackCount = 0;
			for (int j = 0; j < numVertexes; j++)
			{
				marked[j] = 0;
				path[j] = 0;
			}
			//marked.resize(numVertexes, 0);
			//path.resize(numVertexes, 0);
			prim[n][m] = 0;
			prim[m][n] = 0;
		}
	}
};

