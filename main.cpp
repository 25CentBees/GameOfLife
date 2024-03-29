#include <iostream>
#include <stdlib.h>
#include <vector>
#include "windows.h"

const int width = 60;
const int height = 60;
const int boardArea = width * height;

const int mooreNeighbors = 8;
int directions[mooreNeighbors] = { -width, -width + 1, 1, width + 1, width, width - 1, -1, -width - 1 };

int overflowPosition = boardArea - width;

void GoToXY(int column, int row) {

	COORD coord{};
	coord.X = column;
	coord.Y = row;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void SetTextColor(short color_num) {

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_num);
}
void ColorAssigning(unsigned short color_num, char color_ch) {

	SetTextColor(color_num);
	std::cout << color_ch;
}
int ReturnDirectionModifier(int index, int dirIndex) {

	//Complicated way to make sure things don't break.
	if (index <= width) {

		return index + overflowPosition;
	}
	else if (index >= overflowPosition) {

		return index - overflowPosition;
	}
	return index + directions[dirIndex];
}
int main() {

	srand(static_cast<int>(time(NULL)));

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	COORD position{};

	std::vector<int*> board;
	std::vector<int> board2;

	int neighbors[boardArea] = {};

	for (int i = 0; i <= boardArea; ++i) {

		board.push_back(new int(rand() % 2));
		board2.push_back(*board[i]);
	}

	while (true) {

		for (int i = 0; i < boardArea; ++i) {

			if (i < overflowPosition) {

				for (int j = 0; j < mooreNeighbors; ++j) {

					//Checking whether neighbors are alive or dead.
					int directionModifier = ReturnDirectionModifier(i, j);

					if (*board[i] == 1) {

						++neighbors[directionModifier];
					}
				}
			}
		}
		for (int i = 0; i < boardArea; ++i) {

			//Setting board2 as previous board state.
			board2[i] = *board[i];

			//Getting cursor position through this horrendous if statement.
			if (GetConsoleScreenBufferInfo(hConsole, &cbsi)) {

				position = cbsi.dwCursorPosition;
			}
			//Here are the rules for Conway's game of life.
			bool rule =
				(*board[i] == 1 && (neighbors[i] == 2 || neighbors[i] == 3)) ||
				(*board[i] == 0 && neighbors[i] == 3);

			*board[i] = rule ? 1 : 0;

			neighbors[i] = 0;
			if (*board[i] != board2[i]) {
				
				//Assigning off to 0 to no color and on to 1 to blank.
				switch (*board[i]) {

				case 0:
					ColorAssigning(0, '0');
					break;
				case 1:
					ColorAssigning(255, '1');
					break;
				}
				SetTextColor(0);
			}
			else if ((i + 1) % height != height) {
			
				//Going to next line if the current entry matches the last entry that was here.
				GoToXY(position.X + 1, position.Y);
			}
			if (i == boardArea - 1) {
				
				//Why am I doing this? because it is faster and less flickery than clearing the console.
				GoToXY(0, 0);
			}
			else if (i % width == 0) {
				
				//The less inputs into console the better.
				GoToXY(0, position.Y + 1);
			}
		}
	}

	for(auto& b : board){

		delete b;
	}

	return 0;
}
