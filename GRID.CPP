#include "litesout.h"
#include "grid.h"


// ----Block definitions -------------------

void Block::SetState(int value)
{
	value>=GRIDSTATEOFF ?value<=maxstate ?state=value:state=maxstate:state=GRIDSTATEOFF;
}

int Block::ToggleState(int value)
{
	if ((state < value) && (direction == DIRECTIONDOWN))
		direction = DIRECTIONUP;
	if ((state > value) && (direction == DIRECTIONUP))
		direction = DIRECTIONDOWN;
	if (state == maxstate)
		 direction = DIRECTIONDOWN;
	if (state == 0)
		direction = DIRECTIONUP;

	state+=direction;
	return state;
}

// ----- GameGrid definitions -----------

int GameGrid::ToggleBlock(int x, int y)
{
	int temp = 0;
	temp = Grid[x][y]->ToggleState(Grid[x][y]->GetState());

	if (x != 0) 				Grid[x-1][y]->ToggleState(temp);
	if (x + 1 < Across) Grid[x+1][y]->ToggleState(temp);
	if (y != 0 ) 				Grid[x][y-1]->ToggleState(temp);
	if (y + 1 < Down) 	Grid[x][y+1]->ToggleState(temp);
	return CountLit();
}

int GameGrid::CountLit()
{
	int count = 0;
	for (int i=0; i< Across; i++)
		for (int j=0; j< Down; j++)
		{
			if(Grid[i][j]->GetState() > 0) count++;
		};

	return count;
}

GameGrid::GameGrid()
	{
		int i;
		int j;

		Across = BLOCKSACROSS;
		Down = BLOCKSDOWN;
		Width = BLOCKWIDTH * Across;
		Height = BLOCKHEIGHT * Down;
		for (i=0; i< Across; i++)
			for (j=0; j< Down; j++)
				{
					Grid[i][j] = new Block;
					Grid[i][j]->x = i * BLOCKWIDTH;
					Grid[i][j]->y = j * BLOCKHEIGHT;
					Grid[i][j]->SetMaxState(MODEEASY);
					Grid[i][j]->SetState(GRIDSTATEOFF);
					Grid[i][j]->SetDirection(DIRECTIONUP);
				}
	}

GameGrid::~GameGrid()
	{
		for (int i; i < Across; i++)
			for (int j=0; j< Down; j++)
				delete Grid[i][j];
	};

void GameGrid::SetMode(int x)
	{
		for (int i; i < Across; i++)
			for (int j=0; j< Down; j++)
				Grid[i][j]->SetMaxState(x);
	};

