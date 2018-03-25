#include <vector>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "maze.h"
#include <iomanip>
#include <set>

using namespace std;

maze:: maze(int row, int col)
{
	Size.first=row;
	Size.second=col;
	
	
	vector<int> TmpCol(col, 0);
	for(int i=0; i<row; i++)
	{
		MazeMask.push_back(TmpCol);
		MazeMap.push_back(TmpCol);
		RouteMap.push_back(TmpCol);
	}
	
}

maze maze::read(fstream& fs)
{
	char SkipChar;
	string DataLine;
	for(int col=0; col<Size.second; col++)
	{
		for(int row=0; row<Size.first-1; row++)
			fs>>MazeMap[row][col]>>SkipChar;
		fs>>MazeMap[Size.first-1][col];
	}
	
	
	//for testing
	for(int col=0; col<Size.second; col++)
	{	
		for(int row=0; row<Size.first; row++)
			cout<<setw(5)<<MazeMap[row][col];
		cout<<endl;
	}
	cout<<endl;
	
	set_route();
	
	return *this;
}


void maze::set_route()
{
	//cout<<Start.first<<' '<<Start.second<<' '<<End.first<<' '<<End.second<<' '<<endl<<endl;
	
	/*for(int col=0; col<Size.second; col++)
	{	
		for(int row=0; row<Size.first; row++)
			cout<<setw(5)<<RouteMap[row][col];
		cout<<endl;
	}
	
	cout<<endl;*/
	
	for(int col=0; col<Size.second; col++)
	{
		for(int row=0; row<Size.first; row++)
		{
			if(MazeMap[row][col]!=1) RouteMap[row][col]=1;
			
			if(MazeMap[row][col]==200)
			{
				Start.first=row;
				Start.second=col;
			}
			if(MazeMap[row][col]==201)
			{
				End.first=row;
				End.second=col;
			}
		}
	}
	
	pair<int, int> Now=Start;	
	visit(Now);
	
	for(int col=0; col<Size.second; col++)
	{	
		for(int row=0; row<Size.first; row++)
			cout<<setw(5)<<RouteMap[row][col];
		cout<<endl;
	}
	cout<<endl;
	
	while(PathStack.size()>0&&PathStack.top()!=End)
	{
		Path.push_back(PathStack.top());
		PathStack.pop();
	}	
	
	Path.push_back(End);
	
	
	for(int i=0; i<Path.size(); i++)
	{
		cout<<Path[i].first<<' '<<Path[i].second;
		cout<<endl;
	}
	
	return *this;
}

int maze::visit(pair<int, int> Now)
{
	if(RouteMap[Now.first][Now.second]==1)
	{
		RouteMap[Now.first][Now.second]=2;
		if(RouteMap[End.first][End.second]==1&&
		visit(make_pair(Now.first+1, Now.second))==1&&		
		visit(make_pair(Now.first, Now.second+1))==1&&
		visit(make_pair(Now.first-1, Now.second))==1&&
		visit(make_pair(Now.first, Now.second-1))==1)
			RouteMap[Now.first][Now.second]=1;
	}
	
	PathStack.push(Now);
	
	return RouteMap[End.first][End.second];
	
}

