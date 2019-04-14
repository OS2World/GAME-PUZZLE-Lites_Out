// class Block stores the state and information for each block in the grid

class Block
{
	 public:
		int state;    	//0-off 1-on1  2-on2
		int maxstate; 	// 1 or 2 depending on difficulty
		int direction; 	// -1 or 1 for direction of color change
		int x;					//block top corner pixel
		int y;

		Block(){maxstate = 1;
						state = x = y = 0;
						direction = DIRECTIONUP;
						};
		~Block(){};
		void SetX(int value) {x = value;};
		void SetY(int value) {y = value;};
		int  GetX(void) {return x;};
		int  GetY(void) {return y;};
		void SetMaxState(int value) {maxstate = value;};
		int  GetState(void){return state;};
		void SetState(int value);
		void SetDirection(int value){direction = value;};
		int ToggleState(int value);
	};

class GameGrid
{
	public:
		Block* Grid[BLOCKSACROSS][BLOCKSDOWN];    //Game grid of blocks
		int Width;			 //width of field in pixels
		int Height;			 //height of field in pixels
		int Across;			 //Grid Width in Blocks
		int Down;				 //Grid Height in Blocks

		GameGrid();
		~GameGrid();
		int CountLit();
		void SetState(int x, int y, int value){Grid[x][y]->SetState(value);};
		int  GetState(int x, int y) {return Grid[x][y]->GetState();};
		void SetMode(int x);
		int ToggleBlock(int x, int y);
};
