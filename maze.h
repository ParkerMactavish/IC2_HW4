#include <vector>
#include <cstdlib>
#include <fstream>
#include <stack>

#ifndef MAZE_H
#define MAZE_H

using namespace std;


class maze
{
	public:
		maze(int, int);//first is row 橫的數量, second is column直的數量
		maze read(fstream&);//for reading file
		maze generate();//for generate a maze
		bool go_through();//go to the next step, 走到end為0，還沒則1
		void print_map();//print the current map
	private:
		int visit(pair<int, int>);//visit function for the recursive solution to the routing
		void set_route();//set Start and End, and find the number of bonuses and the way to the end point
		pair<int, int> Size;//initialized after this->read
		pair<int, int> Start;//initialized after this->read
		pair<int, int> End;//initialized after this->read
		vector<vector<int>> MazeMap;//initialized after reading or generating
		vector<vector<int>> MazeMask;//initialized after set_route()

		vector<vector<int>> RouteMap;//used to let visit work
		stack<pair<int, int>> PathStack;//store the dfs gone through coordinates of path
		vector<pair<int, int>> Path;//store the coordinates of the path

		vector<pair<int, int>>::iterator PathIt;//Iterator for counting steps

		int CollectableCnt;//total bonuses
		int BonusCollected;//collected bonuses
};

#endif
