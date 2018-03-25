#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <cstdlib>
#include <fstream>
#include <stack>

using namespace std;


class maze
{
	public:
		maze(int, int);//first is row 橫的數量, second is column直的數量
		maze read(fstream&);
		maze set_size(pair<int, int>);
	private:
		int visit(pair<int, int>);
		void go_through();
		pair<int, int> Size;//initialized after this->read
		pair<int, int> Start;//initialized after this->read
		pair<int, int> End;//initialized after this->read
		vector<vector<int>> MazeMap;//initialized after
		vector<vector<int>> MazeMask;
		
		vector<vector<int>> RouteMap;//used to let visit work
		stack<pair<int, int>> PathStack;//store the dfs gone through coordinates of path
		vector<pair<int, int>> Path;//store the coordinates of the path
};

#endif