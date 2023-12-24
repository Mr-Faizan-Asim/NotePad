
#include<iostream>
#include<conio.h>
#include<windows.h>
#include<list>
#include<vector>
#include<string>
#include<stdio.h>
#include <algorithm>

using namespace std;

template <typename T>
class Node {
public:
	T data;
	Node<T>* right;
	Node<T>* left;
	Node(T val){
		right = nullptr;
		left = nullptr;
		data = val;
	}
};

class Notepad {
public:
	list<char> temp;
	list<list<char>>* sheet;
	string tempString = "";
	Notepad(string data)
	{
		sheet = new list<list<char>>;
		load(data);
	}
	void load(string data)
	{
		int size = data.length();
		for(int i = 0 ; i < size ; i++)
		{
			if (data[i] == '\n' || i ==  size - 1)
			{
				sheet->push_back(temp);
				temp.clear();
			}
			else
			{
				temp.push_back(data[i]);
			}
		}
	}
	void insertAtIndex(int index,string data) {
		list<list<char>>* newSheet = new list<list<char>>();
		int x = 0;
		for (auto i : *sheet)
		{
			if (x == index)
			{
				stringToList(data);
				newSheet->push_back(temp);
			}
			else
			{
				newSheet->push_back(i);
			}
			x++;
		}
		sheet = newSheet;
	}
	void insert(string data) {
		stringToList(data);
		sheet->push_back(temp);
	}
	void edit(int index)
	{
		int x = 0;
		for (auto i : *sheet)
		{
			if (x == index)
			{
				temp = i;
				break;
			}
			x++;
		}
	}
	void stringToList(string data)
	{
		temp.clear();
		for (int i = 0; i < data.length(); i++)
		{
			temp.push_back(data[i]);
		}
	}
	string listToString()
	{
		string data = "";
		for (auto i : temp)
		{
			data = data + i;
		}
		tempString = data;
		return data;
	}
	string listToString(list<char> temp)
	{
		string data = "";
		for (auto i : temp)
		{
			data = data + i;
		}
		return data;
	}

};

class FrontEnd {
public:
	vector<FrontEnd>* RedoUndo;
	Notepad *note;
	string select = "- 1";
	int EditedIndex = -1;
	list<char> work;
	vector<string> menu = { "Insert a Line","Edit a Line","Delete a Line","Replace a Line", "Redo","Undo","Movement"};
	int state = 0;
	FrontEnd(string data)
	{
		note = new Notepad(data);
		RedoUndo->push_back(*this);
	}
	void Driver()
	{
		LinesPrint();
		while (true)
		{
			menuPrint(menu,5,5);
			if (select == "1")
			{

				note->insert(Lineget());
				LinesPrint();
				select = "- 1";
			}
			if (select == "2")
			{
				edit();
				LinesPrint();
				select = "- 1";
			}
			if (select == "3")
			{
				Delete();
				LinesPrint();
				select = "- 1";
			}
			if (select == "4")
			{
				replace();
				LinesPrint();
				select = "- 1";
			}
			if (select == "6")
			{
				
				cout << RedoUndo->size() << " Total States are these Chose One";
				cin >> state;
				*this = RedoUndo->front();
				LinesPrint();
				select = "- 1";
			}
		}
	}
	void insertionInUsed()
	{

	}
	// Edit 1st part 
	void edit()
	{

		cin.ignore();
		gotoxy(5, 25);
		cout << "SelectLine: ";
		gotoxy(5 + 13, 25);
		cin >> EditedIndex;
		EditedIndex = EditedIndex - 1;
		note->edit(EditedIndex);
		EditingMode();
	}
	// Edit second part 
	void EditingMode()
	{
	
		string tempData = note->listToString() + ' ';
		int position = 0;
		string dm = "";
		bool copierState = false;
		PrintList(tempData, position);
		while (true)
		{
			if (GetAsyncKeyState(VK_LEFT))
			{
				if (copierState)
				{
					gotoxy(1, 1);
					cout << "\033[35m" << "COPIED STATE ON";
					work.push_back(tempData[position]);
				}

				else
				{
					gotoxy(1, 1);
					cout << "                        ";
				}
				if (position > 0)
					position--;
				gotoxy(18 , 25);
				cout << "                                                                                                               ";
				gotoxy(18, 25);
				PrintList(tempData, position);
			}
			else if (GetAsyncKeyState(VK_RIGHT))
			{

				if (copierState)
				{
					gotoxy(1, 1);
					cout << "\033[35m" << "COPIED STATE ON";
					work.push_back(tempData[position]);
				}
				else
				{
					gotoxy(1, 1);
					cout << "                        ";
				}
				if (position < tempData.length() - 1)
					position++;

				gotoxy(25, 25);
				cout << "                                                                                                               ";

				gotoxy(18, 25);
				PrintList(tempData,position);
			}
			else if (GetAsyncKeyState(VK_CONTROL))
			{

				gotoxy(18, 24);
				getline(cin, dm);
				tempData.insert(position, dm);
				gotoxy(18, 25);
				PrintList(tempData, position);
				gotoxy(18, 24);
				cout << "                                                                                                                   ";
			}
			else if (GetAsyncKeyState(0x41))
			{
				copierState = !copierState;
			}
			else if (GetAsyncKeyState(VK_BACK))
			{
				if (position > 0)
				{
					tempData.erase(position - 1, 1);
					gotoxy(18, 25);
					PrintList(tempData, position);
				}
			}
			else if (GetAsyncKeyState(VK_HOME))
			{
				tempData.insert(position, note->listToString(work));
				gotoxy(18, 25);
				PrintList(tempData, position);
				gotoxy(18, 24);
				work.clear();
			}
			else if (GetAsyncKeyState(VK_ESCAPE))
			{
				note->insertAtIndex(EditedIndex, tempData);
				EditedIndex = -1;
				return;
			}
			Sleep(200);
		}
	}
	// Replace
	void replace()
	{
		int x;
		gotoxy(25, 25);
		cout << "ENTER THE LINE NUMBER 1: ";
		gotoxy(53, 25);
		cin >> x;
		int y;
		gotoxy(25, 26);
		cout << "ENTER THE LINE NUMBER 2: ";
		gotoxy(53, 26);
		cin >> y;
		x--; y--;
		if (x >= 0 && x < note->sheet->size() && y >= 0 && y < note->sheet->size())
		{
			iter_swap(next(note->sheet->begin(), x), next(note->sheet->begin(), y));
		}

	}
	// delete 
	void Delete()
	{
		int x;
		gotoxy(25, 25);
		cout << "ENTER THE LINE NUMBER: ";
		gotoxy(50, 25);
		cin >> x;
		x = x - 1;
		if (x >= 0 && x < note->sheet->size())
		{
			auto it = next(note->sheet->begin(), x);
			note->sheet->erase(it);
		}
	}
	void setter()
	{

	}
	void PrintList(string data,int p)
	{
		int x = 0;
		for (char a : data)
		{
			if (p == x)
			{
				cout << "\033[32m" << a;
			}
			else
			{
				cout << "\033[31m" << a;
			}
			x++;
		}
	}
	string Lineget()
	{
		cin.ignore();
		string temp;
		gotoxy(5, 25);
		cout << "Enter: ";
		gotoxy(5 + 9, 25);
		getline(cin, temp);
		return temp;
	}
	string menuPrint(vector<string> arr,int x,int y)
	{
		for (int i = 0 ; i < arr.size() ; i++)
		{
			gotoxy(x, y);
			cout << i + 1 <<". " << arr[i];
			y++;
		}
		int input;
		gotoxy(x, y++);
		cout<<"Select: ";
		gotoxy(x, y++);
		cin>>select;
		return select;
	}
	void LinesPrint()
	{
		system("cls");
		int x = 1;
		for (const auto a : *(note->sheet))
		{
			gotoxy(120,x);
			cout << x << ": " << merger(a)<<endl;
			x++;
		}

	}
	string merger(list<char> line)
	{
		string temp = "";
		for (auto a : line)
		{
			temp = temp + a;
		}
		return temp;
	}
	void gotoxy(int x, int y)
	{
		COORD coordinates;
		coordinates.X = x;
		coordinates.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
	}

};

int main()
{
	FrontEnd* f = new FrontEnd("ALi is \n is a good boy\n");
	f->Driver();

}
