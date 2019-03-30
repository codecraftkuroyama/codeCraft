#pragma once
#include <vector>
#include <iostream>
#include<cstdlib>
#include<stack>
#include<queue>
#define INF 10000
#define INFINITY 65535

using namespace std;


////cross_to_road

int cross_to_road(int **road, int from_cross, int to_cross, int road_num)
{
	for (int i = 0; i < road_num; ++i)
	{
		if (road[i][4] == from_cross && road[i][5] == to_cross)
			return road[i][0];
		if (road[i][5] == from_cross && road[i][4] == to_cross)
			return road[i][0];
	}
}
//////////////////////////////////////输出路线


class Graph
{
public:
	vector<vector<int>> create_road;
	vector<int> loop_temp;
	vector<vector<int>> loop;
	int cross_num;
	int road_num;
	int **road;
	int **cross;
	vector<int>map;
	Graph(int cross_nu, int road_nu, int **roa, int **cros, vector<int>map);
	~Graph();
	void CreateGraph();//创建图
	void MiniSpanTree_Prim();//prim生成树
	void findCircuit(int k, int parent);//找一张图
	void findCircleRoad();//找所有图
private:
	vector<char> vexs; //顶点表
	vector<vector<float>> arc;//邻接矩阵
	vector<int>marked;//DFS遍历判断
	vector<vector<int>>prim;
	vector<int>path;
	int numVertexes, numEdges;
	int stackCount;

};
Graph::Graph(int cross_nu,int road_nu, int **roa, int **cros, vector<int> ma) {
	cross_num = cross_nu;
	road_num = road_nu;
	road = roa;
	cross = cros;
	map = ma;
}

Graph::~Graph() {

}

void Graph::CreateGraph()
{
	stackCount = 0;

	numVertexes = cross_num;
	numEdges = road_num;
	marked.resize(numVertexes, 0);
	path.resize(numVertexes, 0);

	int i, j, w;
	char ch;
	for (i = 0; i < numVertexes; i++)
	{
		//cin >> ch;
		ch = cross[i][0];
		vexs.push_back(ch);
	}

	arc.resize(numVertexes, vector<float>(numVertexes));
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
	float length_weight = 0.1, speed_weight = -0.5, channel_weight = -0.4, cross_num_weight = -0.2;				/////////////////weight
	int cross_road_num[2];
	//cout << "输入边( vi ，vj)上的下标i,下标j,和权w :" << endl;
	for (int k = 0; k < numEdges; k++)
	{
		//cout << road[k][4] << " " << road[k][5] << " " << road[k][2] << endl;
		cross_road_num[0] = 0;
		cross_road_num[1] = 0;
		if (road[k][6] == 1)
		{
			i = map[road[k][4]];
			j = map[road[k][5]];
			for (int cross_road = 1; cross_road < 5; cross_road++)									//路口连接的road数
			{
				if (cross[i][cross_road] > 0)
					++cross_road_num[0];
				if (cross[j][cross_road]>0)
					++cross_road_num[1];
			}
			w = road[k][1] * length_weight + road[k][2] * speed_weight + road[k][3] * speed_weight + cross_num_weight*(cross_road_num[0] + cross_road_num[1]);

			arc[i][j] = arc[j][i] = w+20;
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
	for (int i = 1; i <numVertexes; i++)
	{
		lowcost.push_back(100);
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
		prim[adjvex[k]][k] = 1;
		prim[k][adjvex[k]] = 1;
		cout << "(" << adjvex[k] + 1  << "<->" << k + 1 << ")" << endl;
		for (int road_num = 0; road_num < numEdges; road_num++)
		{
			if ((map[road[road_num][4]] == adjvex[k] && map[road[road_num][5]] == k) || (map[road[road_num][5]] == adjvex[k] && map[road[road_num][4]] == k))
			{
				vector<int> temp;
				for (int i = 0; i < 7; ++i)
				{
					temp.push_back(road[road_num][i]);
				}
				create_road.push_back(temp);
			}
		}
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
					loop_temp.push_back(path[j]);
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

	for (int i = 0; i < numEdges; i++)
	{
		int n = map[road[i][4]];
		int m = map[road[i][5]];
		if (prim[n][m] == 1 || prim[m][n] == 1)
		{
			cout << i << " " << endl;
		}
		else
		{
			prim[n][m] = 1;
			prim[m][n] = 1;

			cout <<"loop : ";
			
			for (int j = 0; j < numVertexes; j++)
			{
				if (marked[j] != -1)
				{
					findCircuit(j, -1);
				}

			}
			for (int i = 0; i < loop_temp.size(); i++)
			{
				cout << loop_temp[i] << " ";
			}
			for (int k = 0; k < loop_temp.size(); ++k)
			{
				if (loop_temp[k] == m || loop_temp[k] == n)
				{
					vector<int> temp;
					temp.push_back(5000+i);
					if (k > 0)
					{
						temp.push_back(cross_to_road(road, loop_temp[k - 1] + 1, loop_temp[k] + 1, road_num));
						temp.push_back(cross_to_road(road, loop_temp[(k+1) % loop_temp.size()] + 1, loop_temp[(k + 2) % loop_temp.size()] + 1, road_num));
						//temp.push_back(loop_temp[k - 1] + 1);
					}
					else
					{
						if (loop_temp[loop_temp.size() - 1] == m || loop_temp[loop_temp.size() - 1] == n)
						{
							temp.push_back(cross_to_road(road, loop_temp[k] + 1, loop_temp[k + 1] + 1, road_num));
							temp.push_back(cross_to_road(road, loop_temp[loop_temp.size() - 2] + 1, loop_temp[loop_temp.size() - 1] + 1, road_num));
						}
						else
						{
							temp.push_back(cross_to_road(road, loop_temp[k + 1] + 1, loop_temp[k + 2] + 1, road_num));
							temp.push_back(cross_to_road(road, loop_temp[loop_temp.size() - 1] + 1, loop_temp[k] + 1, road_num));
							//temp.push_back(loop_temp[k + 1] + 1);
						}
					}
					loop.push_back(temp);
					break;
				}
			}
			
			loop_temp.clear();

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







int  min(int a, int b)
{
	return a < b ? a : b;
}


bool judge_is_ok(vector<int> shortest_route, vector<vector<int> >judge_circle)//判断生成的路线是否可行
{
	for (int i = 0; i < shortest_route.size(); i++)
	{
		for (int j = 0; j < judge_circle.size(); j++)
		{
			if (shortest_route[i] == judge_circle[j][0])
			{
				if (i == 0)
				{
					if (shortest_route[i + 1] == judge_circle[j][1] || shortest_route[i + 1] == judge_circle[j][2])
						return false;
				}
				if (i > 0 && i < shortest_route.size() - 1)
				{
					if (shortest_route[i - 1] == judge_circle[j][1] || shortest_route[i + 1] == judge_circle[j][1])
						return false;
					if (shortest_route[i + 1] == judge_circle[j][2] || shortest_route[i + 1] == judge_circle[j][2])
						return false;
				}
				if (i == shortest_route.size() - 1)
				{
					if (shortest_route[i - 1] == judge_circle[j][1] || shortest_route[i - 1] == judge_circle[j][2])
						return false;
				}

			}
		}
	}
	return true;
}



bool judge_is_same(vector<int> shortest_route1, vector<int> shortest_route2)//判断两条路线是否相同
{
	if (shortest_route1.size() != shortest_route2.size())
		return false;
	else
	{
		for (int i = 0; i < shortest_route1.size(); i++)
		{
			if (shortest_route1[i] != shortest_route2[i])
				return false;
		}
		return true;
	}
}



void short_route_former(int **road_weight, int beg, int en, int **d, int car_id, int **road, int road_num, vector<int> &shortest_route, vector<int>map,vector<int>comap)
{
	shortest_route.clear();
	int cross_num = reader_text::cross_num;
	int begining = map[beg];
	int end = map[en];
	int *route = new int[reader_text::cross_num];
	shortest_route.push_back(car_id);

	/*for (int i = 0; i < shortest_route.size(); i++)
	{
		cout << shortest_route[i] << endl;
	}
	getchar();*/
	for (int i = 0; i < cross_num; i++)
		route[i] = -1;
	int *v = new int[cross_num];
	for (int i = 0; i < cross_num; i++)
	{
		v[i] = 0;
	}
	for (int i = 0; i < cross_num; i++)
	{
		d[begining][i] = (i == begining ? 0 : INF);
	}
	for (int i = 0; i < cross_num; i++)
	{
		int x, m = INF;
		for (int y = 0; y < cross_num; y++)
		{

			if (!v[y] && d[begining][y] <= m)
				m = d[begining][x = y];
		}

		v[x] = 1;
		for (int y = 0; y < cross_num; y++)
		{

			d[begining][y] = min(d[begining][y], d[begining][x] + road_weight[x][y]);


		}

	}
	int k = 0;
	route[0] = comap[end];
	int n = end;
	for (; n != begining;)
	{
		for (int j = 0; j < cross_num; j++)
		{
			if (abs(d[begining][n] - (d[begining][j] + road_weight[j][n])) <= 1e-4)
			{
				route[++k] = comap[j];
				//cout << comap[j] << "   ";
				n = j;
				
				break;
			}
		}
	}
	//cout << endl<<endl;
	vector<int> temp(k + 1);

	//cout << endl;
	for (int j = 0; j <= k; j++)
	{
		temp[k - j] = route[j];
		//cout << temp[k - j] << "      ";
	}
	//cout << endl;

	for (int i = 0; i < k; i++)
	{
		int at_road = cross_to_road(road, temp[i], temp[i + 1], road_num);
		shortest_route.push_back(at_road);

	}
	//shortest_route.push_back(cross_to_road(road, temp[k], en, road_num));//push_back最后一条路
	/*for (int i = 0; i < shortest_route.size(); i++)
	{
	cout << shortest_route[i] << "    ";
	}*/
	//cout << endl;
	delete[]route;
	delete[]v;
}
//找寻原地图中的最短路径



void short_route_New(int **road_weight, int beg, int en, int **d, int car_id, int **road, int road_num, vector<int> &shortest_route, vector<int> map,vector<int>comap)
{
	shortest_route.clear();
	shortest_route.push_back(car_id);
	int begining = map[beg];
	int end = map[en];
	int **route = new int *[reader_text::cross_num];
	int cross_num = reader_text::cross_num;
	for (int i = 0; i < reader_text::cross_num; ++i)
	{
		route[i] = new int[reader_text::cross_num];
	}
	int *v = new int[cross_num];
	for (int i = 0; i < cross_num; i++)
	{
		v[i] = 0;
	}

	int number = 0;
	for (int i = 0; i < cross_num; i++)
		for (int j = 0; j < cross_num; j++)
			route[i][j] = -1;
	for (int i = 0; i < cross_num; i++)
	{
		v[i] = 0;
	}


	for (int i = 0; i < road_num; i++)
	{

		if (road[i][5] == en)
		{
			++number;

			route[number][0] = road[i][4];

		}
		else if (road[i][4] == en && road[i][6] == 1)
		{
			++number;

			route[number][0] = road[i][5];

		}
	}
	for (int i = 0; i < cross_num; i++)
	{
		d[begining][i] = (i == begining ? 0 : INF);

	}
	for (int i = 0; i < cross_num; i++)
	{
		int x, m = INF;
		for (int y = 0; y < cross_num; y++)
		{

			if (!v[y] && d[begining][y] <= m)
				m = d[begining][x = y];
		}

		v[x] = 1;
		for (int y = 0; y < cross_num; y++)
		{
			if (d[begining][y] > d[begining][x] + road_weight[x][y])
				d[begining][y] = d[begining][x] + road_weight[x][y];


		}

	}
	int min = 1;

	for (int i = 1; i <= number; i++)
	{
		if (d[begining][route[i][0]] < d[begining][route[min][0]])
			min = i;

	}

	int k = 0;
	int n = route[min][0];
	for (; n != begining;)
	{
		for (int j = 0; j < cross_num; j++)
		{
			if (abs(d[begining][n] - (d[begining][j] + road_weight[j][n])) <= 1e-4)
			{
				route[min][++k] = comap[j];

				n = j;
				break;
			}
		}
	}

	vector<int> temp(k + 1);


	for (int j = 0; j <= k; j++)
	{
		temp[k - j] = route[min][j];
	}
	for (int i = 0; i < k; i++)
	{
		int at_road = cross_to_road(road, temp[i], temp[i + 1], road_num);
		shortest_route.push_back(at_road);
	}
	shortest_route.push_back(cross_to_road(road, temp[k], en, road_num));//push_back最后一条路
	for (int i = 0; i < reader_text::cross_num; ++i)
	{
		delete[]route[i];
	}
	delete[]route;
	delete[]v;

}
//在生成的地图里找寻最短路径

void add_road_weight(int **road_weight, vector<int> shortest_route, int **road, int road_num, int add, vector<int>comap)
{
	int cross_num = reader_text::cross_num;
	for (int i = 0; i < cross_num; i++)
	{
		for (int j = 0; j < cross_num; j++)
		{
			int at_road = cross_to_road(road, comap[i] , comap[j], road_num);

			for (int k = 1; k < shortest_route.size(); k++)
			{
				if (at_road == shortest_route[k])
				{
					road_weight[i][j] = road_weight[i][j] + add / 2;
					if (road[i][6])
						road_weight[j][i] = road_weight[i][j] + add / 2;
				}
			}

		}

	}
}
//增加已有路的权重，使其生成的最短路径不相同
void inital_road_weight(int **road_weight, int **road, int road_num, int **count_road, int count_weight, vector<int>map)
{

	for (int i = 0; i < road_num; i++)
	{
		int b = map[road[i][4]];
		int e = map[road[i][5]];
		for (int j = 0; j < road_num; j++)
		{
			if (road[i][0] == count_road[j][0])
			{
				road_weight[b][e] = road[i][1] + count_road[j][1] / count_weight;
				if (road[i][6])
					road_weight[e][b] = road[i][1] + count_road[j][1] / count_weight;
			}
		}
	}

}
void shortRoute(int **New_road_weight, int beg, int en, int **d, vector<vector<int>> &car_route, int car_id, int **road, int road_num, vector<vector<int> >judge_circle, vector<vector<int> >create_road, int num_route, int **count_road, int count_weight, vector<int>map,vector<int>comap)
{

	int Graph_road_num = create_road.size();
	int cross_num = reader_text::cross_num;
	int **all_road_weight = new int *[cross_num];

	for (int i = 0; i < cross_num; ++i)
	{
		all_road_weight[i] = new int[cross_num];
	}
	for (int i = 0; i < cross_num; i++)
	{
		for (int j = 0; j < cross_num; j++)
		{
			all_road_weight[i][j] = INF;
		}

	}
	inital_road_weight(all_road_weight, road, road_num, count_road, count_weight,map);

	vector<vector<int> >short_route_all;
	vector<int>shortest_route;
	short_route_former(all_road_weight, beg, en, d, car_id, road, road_num, shortest_route, map,comap);

	/*for (int i = 0; i < shortest_route.size(); i++)
	{
	cout << shortest_route[i] << "      ";
	}
	cout << endl;*/

	short_route_all.push_back(shortest_route);
	for (int i = 1; i < num_route; i++)
	{
		add_road_weight(all_road_weight, shortest_route, road, road_num, 5, comap);
		//cout << beg << "     " << en<< endl;
		short_route_former(all_road_weight, beg, en, d, car_id, road, road_num, shortest_route, map,comap);
		/*	for (int i = 0; i < shortest_route.size(); i++)
		{
		cout << shortest_route[i] << "      ";
		}
		cout << endl;
		*/
		for (int j = 0; j < i; j++)
		{
			while (judge_is_same(short_route_all[j], shortest_route))
			{
				add_road_weight(all_road_weight, shortest_route, road, road_num, 5, comap);
				short_route_former(all_road_weight, beg, en, d, car_id, road, road_num, shortest_route, map,comap);
			}

		}
		short_route_all.push_back(shortest_route);
		inital_road_weight(all_road_weight, road, road_num, count_road, count_weight,map);
	}

	int number_ok = -1;//记录第几个是OK的
	for (int i = 0; i < num_route; i++)
	{
		if (judge_is_ok(short_route_all[i], judge_circle))
		{
			number_ok = i;
			//cout << i << endl;
			break;
		}
	}
	if (number_ok == -1)//即所有里面的都不能成功
	{
		short_route_New(New_road_weight, beg, en, d, car_id, road, road_num, shortest_route, map,comap);
		/*	for (int i = 0; i < shortest_route.size(); i++)
		{
		cout << shortest_route[i] << "        ";
		}
		cout << endl;
		*/
		car_route.push_back(shortest_route);
		for (int i = 0; i<shortest_route.size(); i++)
			for (int j = 0; j < road_num; j++)
			{
				if (count_road[j][0] == shortest_route[i])
					count_road[j][1]++;
			}
	}

	if (number_ok != -1)
	{
		car_route.push_back(short_route_all[number_ok]);
		for (int i = 0; i<short_route_all[number_ok].size(); i++)
			for (int j = 0; j < road_num; j++)
			{
				if (count_road[j][0] == short_route_all[number_ok][i])
					count_road[j][1]++;
			}
	}
	for (int i = 0; i < cross_num; ++i)
	{
		delete[]all_road_weight[i];

	}
	delete[]all_road_weight;



}


//////////////create time

void create_time(vector<vector<int>> &car_route, int car_num, int **car, vector<int> car_time, int road_num, int **road, int cross_num, int**cross)
{
	vector<vector<int>>car_flow;
	for (int i = 0; i < cross_num; i++)
	{
		vector<int> temp(2);
		temp[0] = cross[i][0];
		temp[1] = 0;
		car_flow.push_back(temp);
	}
	float car_flow_weight = 0.01;
	int max_car_num = 1100;
	int go_time;
	vector<int>car_temp_time(max_car_num);
	int car_order = 0;
	int temp_cross_order;
	for (int speed = 10; speed >= 1; speed--)
	{
		for (int i = 0; i < car_num; ++i)
		{
			if (car[i][3] == speed)
			{
				for (int route_it = 1; route_it < car_route[i].size(); route_it++)
				{
					for (int find_road = 0; find_road < road_num; ++find_road)
					{
						if (car_route[i][route_it] == road[find_road][0])
						{
							temp_cross_order = road[find_road][4];
							break;
						}
					}
					for (int find = 0; find < cross_num; find++)
					{
						if (car_flow[find][0] == temp_cross_order)
						{
							car_flow[find][1]++;
							if (route_it == 1)
								car_temp_time[car_order%max_car_num] += int(car_flow[find][1] * car_flow_weight);
							break;
						}
					}
				}
				car_order++;
				vector<int>::iterator it1 = car_route[i].begin();
				++it1;
				go_time = car_temp_time[car_order%max_car_num] > car[i][4] ? car_temp_time[car_order%max_car_num] : car[i][4];
				car_route[i].insert(it1, go_time);
				if (car_temp_time[car_order % max_car_num] < car[i][4])
					car_temp_time[car_order % max_car_num] += car[i][4];
				car_temp_time[car_order%max_car_num] += car_time[i];
			}
		}
	}
}


void time_cost(vector<int>&route_time, vector<vector<int>> &car_route, int **car, int **road, int road_num)
{
	int car_num = reader_text::car_num;
	for (int i = 0; i < car_num; i++)
	{
		route_time[i] = 0;
	}
	for (int i = 0; i < car_num; i++)
	{
		for (int j = 1; j < car_route[i].size(); j++)
		{
			for (int k = 0; k < road_num; k++)
			{
				if (car_route[i][j] == road[k][0])
				{
					int speed = min(car[i][3], road[k][2]);
					route_time[i] = route_time[i] + road[k][1] / speed;
					break;
				}
			}
		}
	}

}