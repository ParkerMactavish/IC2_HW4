#include "maze.h"
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <set>

using namespace std;

maze::maze(int row, int col)
{
	Size.first = row;
	Size.second = col;


	vector<int> TmpCol(col, 0);//put the 2d vector with all 0
	for (int i = 0; i<row; i++)
	{
		MazeMask.push_back(TmpCol);
		MazeMap.push_back(TmpCol);
		RouteMap.push_back(TmpCol);
	}

	CollectableCnt = 0;//set bonuses as 0
	BonusCollected = 0;
}

maze maze::read(fstream& fs)
{
	char SkipChar;//because the numbers are separated by commas, so we need a character buffer
	for (int col = 0; col<Size.second; col++)
	{
		for (int row = 0; row<Size.first - 1; row++)
			fs >> MazeMap[row][col] >> SkipChar;
		fs >> MazeMap[Size.first - 1][col];
	}


	//for testing
	/*for (int col = 0; col<Size.second; col++)
	{
		for (int row = 0; row<Size.first; row++)
			cout << setw(5) << MazeMap[row][col];
		cout << endl;
	}
	cout << endl;*/

	set_route();//find the way from start to end

	return *this;
}


maze maze::generate()
{
	vector <int> BoundVec(Size.second, 1);//upper and lower boundary of the map
	vector<int>ContentVec(Size.second, 0); *ContentVec.begin() = 1; *(ContentVec.end() - 1) = 1;//the space inside the upper and lower bound
	*MazeMap.begin() = BoundVec; *(MazeMap.end() - 1) = BoundVec;
	cout << MazeMap.size() << ' ' << MazeMap[0].size() << ' ' << Size.first << ' '<<Size.second << endl;


	//we create a z-character like maze, you can see if you generate a barely random maze
	vector<int> FirstColVec = ContentVec; for (int i = 1; i * 7 < FirstColVec.size() - 1; i++) FirstColVec[i*7] = 202;
	vector<int>SecondColVec = BoundVec; *(SecondColVec.end() - 2) = 0;
	vector<int>ThirdColVec = ContentVec; for (int i = 1; i * 7 < ThirdColVec.size() - 1; i++) ThirdColVec[ThirdColVec.size()-1-i*7] = 203;
	vector<int>FourthColVec = BoundVec; *(FourthColVec.begin() + 1) = 0;
	for(int i=1; i<Size.first-1; i++)
	{
		if (i % 4 == 0) MazeMap[i] = FourthColVec;
		else if (i % 4 == 1) MazeMap[i] = FirstColVec;
		else if (i % 4 == 2)MazeMap[i] = SecondColVec;
		else MazeMap[i] = ThirdColVec;
	}
	
	MazeMap[1][1] = 200;
	if (Size.first % 4 == 0 || Size.first % 4 == 3)MazeMap[Size.first - 2][Size.second - 2] = 201;
	else if (Size.first% 4 == 2 || Size.first % 4 == 1)MazeMap[Size.first - 2][1] = 201;


	set_route();//find the way from start to end
	return *this;
}


void maze::set_route()
{
	for (int col = 0; col<Size.second; col++)
	{
		for (int row = 0; row<Size.first; row++)
		{
			if (MazeMap[row][col] != 1) RouteMap[row][col] = 1;//if it's not wall, then it's walkable(presented by 1)

			if (MazeMap[row][col] == 200)//set start
			{
				Start.first = row;
				Start.second = col;
			}
			if (MazeMap[row][col] == 201)//set end
			{
				End.first = row;
				End.second = col;
			}
			if (MazeMap[row][col] > 201)//set bonuses
			{
				CollectableCnt++;
			}
		}
	}

	pair<int, int> Now = Start;//initialize the recurssion
	visit(Now);//go through the recursive way of finding road to end

	/*for (int col = 0; col<Size.second; col++)
	{
		for (int row = 0; row<Size.first; row++)
			cout << setw(5) << RouteMap[row][col];
		cout << endl;
	}
	cout << endl;*/


	//get the result of the path
	while (PathStack.size()>0 && PathStack.top() != End)
	{
		Path.push_back(PathStack.top());
		PathStack.pop();
	}
	Path.push_back(End);


	/*for (int i = 0; i<Path.size(); i++)
	{
		cout << Path[i].first << ' ' << Path[i].second;
		cout << endl;
	}

	cout << (Path.end()-1)->first << ' ' << (Path.end()-1)->second << endl;
	system("PAUSE");*/


	PathIt = Path.begin();//set the iterator to the begin and wait to demonstrate the way to end
}

//core of the algorithm of finding the road
int maze::visit(pair<int, int> Now)
{
	if (RouteMap[Now.first][Now.second] == 1)//如果他是可以走的話(牆是0，路是1)
	{
		RouteMap[Now.first][Now.second] = 2;//把這格先暫時設為路徑之一(以2代表)
		if (RouteMap[End.first][End.second] == 1 &&//如果這格本身不是終點
			visit(make_pair(Now.first + 1, Now.second)) == 1 &&//他的右邊那一個也找不到終點
			visit(make_pair(Now.first, Now.second + 1)) == 1 &&//他的下面那一個也找不到終點
			visit(make_pair(Now.first - 1, Now.second)) == 1 &&//他的左邊那一個也找不到終點
			visit(make_pair(Now.first, Now.second - 1)) == 1)//他的右邊那一個也找不到終點
			RouteMap[Now.first][Now.second] = 1;//他就不是路徑，所以改回1
	}
	if(RouteMap[Now.first][Now.second] == 2)
		PathStack.push(Now);//如果他在路徑上，就push到路徑的stack裡

	return RouteMap[End.first][End.second];//看終點的值有沒有變成2來決定有沒有找到

}


bool maze::go_through()
{
	//這部分其實是要做出戰爭迷霧的效果，但最後沒有用到@@
	if (PathIt->first > 0 && MazeMask[PathIt->first - 1][PathIt->second] == 0) MazeMask[PathIt->first - 1][PathIt->second] = 1;
	if (PathIt->second > 0 && MazeMask[PathIt->first][PathIt->second - 1] == 0) MazeMask[PathIt->first][PathIt->second - 1] = 1;
	if (PathIt->first < Size.first-1 && MazeMask[PathIt->first + 1][PathIt->second] == 0) MazeMask[PathIt->first + 1][PathIt->second] = 1;
	if (PathIt->second < Size.second - 1 && MazeMask[PathIt->first][PathIt->second + 1] == 0) MazeMask[PathIt->first][PathIt->second + 1] = 1;
	
	//如果還沒到終點就在mask上面畫箭頭
	if (PathIt != Path.end()-1)
	{
		if (PathIt->second == (PathIt + 1)->second - 1)MazeMask[PathIt->first][PathIt->second] = 2;
		else if (PathIt->second == (PathIt + 1)->second + 1)MazeMask[PathIt->first][PathIt->second] = 3;
		else if (PathIt->first == (PathIt + 1)->first - 1)MazeMask[PathIt->first][PathIt->second] = 4;
		else if (PathIt->first == (PathIt + 1)->first + 1)MazeMask[PathIt->first][PathIt->second] = 5;
	}
	//到終點要畫圓圈圈
	else MazeMask[PathIt->first][PathIt->second] = 6;

	//如果有踩到寶物要幫自己加一個寶物
	if (MazeMap[PathIt->first][PathIt->second] > 201) BonusCollected++;

	PathIt++;

	//如果走完了就回傳0，否則回傳1
	if (PathIt == Path.end())
	{
		return 0;
	}
	else return 1;
}


void maze::print_map()
{
	for (int col = 0; col<Size.second; col++)
	{
		for (int row = 0; row<Size.first; row++)
		{
			if (MazeMask[row][col] == 1)//如果可以直接顯示數字的話
			{
				cout << setw(3) << MazeMap[row][col];
			}
			else if(MazeMask[row][col]==0) cout << setw(3) << MazeMap[row][col];//如果不能顯示數字的話，還是顯示數字吧(原本應該是要用方塊蓋起來啦
			else if (MazeMask[row][col] == 2) cout << setw(3) << "↓";//下一步往下
			else if (MazeMask[row][col] == 3) cout << setw(3) << "↑";//下一步往上
			else if (MazeMask[row][col] == 4) cout << setw(3) << "→";//下一步往右
			else if (MazeMask[row][col] == 5) cout << setw(3) << "←";//下一步往左
			else if (MazeMask[row][col] == 6)cout << setw(3) << "⊙";//終點
		}
		cout << endl;
	}
	cout << endl << "Bonuses:" <<BonusCollected<<'/'<< CollectableCnt<<endl;//看自己蒐集到幾個寶物
}