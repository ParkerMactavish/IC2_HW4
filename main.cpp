#include <iostream>
#include <string>
#include <fstream>
#include <winbase.h>
#include "maze.h"

using namespace std;

int main()
{
	string Command;
	char SkipChar;
	int RunFlag=1;
	fstream fs;
	while(RunFlag!=0)
	{
		if(RunFlag==1)
		{			
			cout<<"Please enter your command"<<endl;
			cin>>Command;
			
			if((Command[0]=='Q'||Command[0]=='q')&&Command[1]==0)
				RunFlag=0;
			
			else if(Command[0]=='R'||Command[0]=='r')
				RunFlag=2;
			
			else if(Command[0]=='G'||Command[0]=='g')
				RunFlag=3;
		}
		
		
		if(RunFlag==2)
		{
			cout<<"What path do you want to read the map from?"<<endl;
			cin>>Command;
			const char *Path = Command.c_str();
			if(INVALID_FILE_ATTRIBUTES == GetFileAttributes(Path))
			{
				cout<<"Your path is invalid"<<endl;
				RunFlag=2;
			}
			else
			{
				fs.open(Command, ios::in);
				RunFlag=21;
			}
		}
		
		if(RunFlag==21)
		{
			
			int row, col;
			fs>>row>>SkipChar>>col;
			//cout<<row<<col;
			maze *Maze = new maze(row, col);
			Maze->read(fs);
			delete Maze;
			fs.close();
			RunFlag=1;
		}
		
	}
	return 0;
}