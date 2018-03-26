#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include "maze.h"

using namespace std;

int main()
{
	string Command;
	char SkipChar;
	int RunFlag = 1;
	fstream fs;
	while (RunFlag != 0)
	{
		if (RunFlag == 1)//�Ĥ@���q��O���O�A�nŪ�ɡB�H�������٬O���}
		{
			cout << "Please enter your command" << endl;
			cin >> Command;

			if ((Command[0] == 'Q' || Command[0] == 'q') && Command[1] == 0)
				RunFlag = 0;

			else if (Command[0] == 'R' || Command[0] == 'r')
				RunFlag = 2;

			else if (Command[0] == 'G' || Command[0] == 'g')
				RunFlag = 3;
		}


		if (RunFlag == 2)//Ū�ɹL�{
		{
			cout << "What path do you want to read the map from?" << endl;
			cin >> Command;
			const char *Path = Command.c_str();
			if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(Path))
			{
				cout << "Your path is invalid" << endl;
				RunFlag = 2;
			}
			else
			{
				fs.open(Command, ios::in);
				RunFlag = 21;
			}
		}

		if (RunFlag == 21)//���ɮ׼g�Jmaze��
		{
			int row, col;
			fs >> row >> SkipChar >> col;
			//cout<<row<<col;
			maze *Maze = new maze(row, col);
			Maze->read(fs);
			system("CLS");
			Maze->print_map();
			while (Maze->go_through())//�p�Ggothrough�٨S��̫�@�ӴN���|��
			{
				system("CLS");
				Maze->print_map();
				Sleep(100);
			}
			system("CLS");
			Maze->print_map();
			system("PAUSE");
			system("CLS");
			delete Maze;
			fs.close();
			RunFlag = 1;
		}


		if (RunFlag == 3)//�H�����Ͱg�c
		{
			int row = 0, col = 0;
			cout << "How many rows do you want?" << endl;
			cin >> row;
			while (row < 4)
			{
				cout << "The Maze is not big enough. How many rows do you want?" << endl;
				cin >> row;
			}
			
			cout << "How many columns do you want?" << endl;
			cin >> col;
			while (col < 4)
			{
				cout << "The Maze is not big enough. How many columns do you want?" << endl;
				cin >> col;
			}

			system("CLS");
			maze *Maze = new maze(row, col);
			Maze->generate().print_map();
			system("PAUSE");


			while (Maze->go_through())//�٨S���쩳�N���|��
			{
				system("CLS");
				Maze->print_map();
				Sleep(100);
			}
			system("CLS");
			Maze->print_map();
			system("PAUSE");
			system("CLS");

			delete Maze;
			RunFlag = 1;
		}

	}
	return 0;
}