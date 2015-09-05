/* Chris Patton 20/07/09
 * mill
. 2 players, 9 pieces each to start
. a mill is 3 in a row. once a mill is made, take an opponents piece
. move pieces to adjacent vertices 
. . starts out by placing 9 pieces anywhere
. . if you only have 3 pieces¸ then you can jump to any vertice. 
. cannot take piece that is in a mill 
. . unless no other available pieces
. game ends when a mill can't be made, ie 2 pieces left

  C -> B -> A (inside)
  1 2 3 
  8   4
  7 6 5 
*/
#include <stdio.h>
#include <stdlib.h>
#define MAX 2 +1
#define VERT '.' 
#define BLACK 'B' 
#define WHITE 'W' 

typedef char Error;
char ErrHandle [] = " >%s: "; 
Error errInput [] = "not valid input";
Error	errOccupied [] = "that spot is taken";
Error	errEmpty [] = "that cell is empty";
Error	errNotEmpty [] = "that cell is not empty";
Error	errYourStone [] = "that's your stone";
Error	errFrom [] = "you can only move your stone";
Error	errNoJump	[] = "you can't jump yet";
Error	errMilled	[] = "you can't take a milled stone";

char Welcome [] = "\n\
\t    Mill! \n\
\t  White = %c\n\
\t  Black = %c\n\
\tFirst place 9 stones each. White starts. To place,\n\
\ttype in the name of the cell where you want to place\n\
\ta stone. EX: A1 (top left corner). To move a stone,\n\
\ttype first the cell of the stone you want to move\n\
\tand then the destination. EX: A1 A8\n"; 

typedef enum { 
	NW=0, N, NE, E, SE, S, SW, W }
cell_t;
typedef char layer_t; 
typedef struct {
	layer_t A [8]; 
	layer_t B [8]; 
	layer_t C [8]; 	
	int white, black; // # of stones 
} board_t; 
typedef struct ref { // reference to a cell (layer, cell)
	layer_t * layer; // layer A, B, or C
	cell_t cell; 
	char lay; // 'A', 'B', or 'C'
} ref_t; 

void disp_board (board_t b);
Error* input_cell (char *com); 
Error* chk_is_cell (char *com); 
int chk_mill (char player, ref_t vicinity, board_t board); 
int chk_is_adj (ref_t from, ref_t to);
int chk_milled (char player, board_t *board); 
ref_t get_ref (char *com, board_t *board); 
Error* place (char player, board_t *board); 
Error* move (char player, board_t *board); 
Error* take (char player, board_t *board); 

void scan_nl (void) 
{ char c; 
	while ((c=getchar())!='\n')
		; }

int main () 
{
	board_t game;
	int n;
	// initialize board
	char player = WHITE;
	game.black = game.white = 9; // # of stones to start
	for (n=0; n<8; n++) 
		game.A[n] = game.B[n] = game.C[n] = VERT;
	
	// Welcome
	printf (Welcome, WHITE, BLACK); 
	disp_board (game); 

	char *err; 
	// place 9 pieces each
	for (n=17; n>=0; n--) 
	{
		printf("%s, place %d more stone%c: ", 
		 (player==WHITE) ? "White" : "Black", 
		 (n/2)+1, (n<=1) ? '\0' : 's');
		while ((err = place(player, &game)) != 0) 
			printf(ErrHandle, err);
		disp_board (game); 
		player = (player==WHITE) ? BLACK : WHITE; 
	}
/*// --- sample scenario
	game.A[8] = game.B[2] = game.C[1] = game.A[3]= 'W';
	game.C[4] = game.C[5] = game.C[6] = game.C[7]='B';
	game.black = 4; game.white = 4; 
	disp_board (game); 
*/
	// move 'till one player has 2 pieces
	while (game.black > 2 && game.white > 2) 
	{	
		printf("%s's move: ", (player==WHITE) ? "White" : "Black");
		while ((err = move(player, &game)) != 0)
			printf(ErrHandle, err); 
		disp_board (game); 
		player = (player==WHITE) ? BLACK : WHITE; 
	}
	
	// decleration of victory!!!!!
	{
	printf("\n\t%s wins!!!! Schade... Thank you for playing Mill.\n", 
		(game.black<=2) ? "White" : "Black");
	//printf("\n\tPress [Enter] to close..."); char c=getchar(); 
	}
	return 0;
}

void disp_board (board_t b) 
{ 
 // later: [B] [W] [ ]
printf("\t  C\n");
printf("\t(%c)---------------(%c)---------------(%c)    1 2 3\n",
	b.C[0],b.C[1],b.C[2]);
printf("\t |      B          |                 |     8   4\n");
printf("\t |    (%c)---------(%c)---------(%c)    |     7 6 5\n",
	b.B[0],b.B[1],b.B[2]); 
printf("\t |     |      A    |           |     |\n"); 
printf("\t |     |    (%c)---(%c)---(%c)    |     |     Stones:\n",
	b.A[0],b.A[1],b.A[2]);
printf("\t |     |     |           |     |     |     White: %d\n", b.white); 
printf("\t(%c)---(%c)---(%c)         (%c)---(%c)---(%c)    Black: %d\n",
	b.C[7], b.B[7], b.A[7], b.A[3],b.B[3],b.C[3], b.black); 
printf("\t |     |     |           |     |     |\n"); 
printf("\t |     |    (%c)---(%c)---(%c)    |     |\n",b.A[6],b.A[5],b.A[4]);
printf("\t |     |           |           |     |\n");
printf("\t |    (%c)---------(%c)---------(%c)    |\n",b.B[6],b.B[5],b.B[4]); 
printf("\t |                 |                 |\n");
printf("\t(%c)---------------(%c)---------------(%c)\n",b.C[6],b.C[5],b.C[4]);
}

// input syntactical cell [A-C][1-8]
Error* input_cell (char *com)
{
	while ((com[0]=getchar())==' ')
		; 
	com[0] = (com[0]>'Z') ? (com[0]-'a')+'A' : com[0]; 
	com[1] = getchar();
	// enforce input syntax
 	if ( (com[0] < 'A' || com[0] > 'C') ||
			 ((int)com[1]-'0' < 1 || (int)com[1]-'0' > 8) ) {
		scan_nl (); 
		return errInput; }
	return 0;
}

// returns a reference to a point on the board
ref_t get_ref (char *com, board_t *board) 
{	
	ref_t ref;
	com[0] = (com[0]>'Z') ? (com[0]-'a')+'A' : com[0]; 
	if (com[0]=='A') ref.layer = board->A;
	else if (com[0]=='B') ref.layer = board->B; 
	else if (com[0]=='C') ref.layer = board->C; 
	ref.cell = ((cell_t)com[1]-'0')-1; 
	ref.lay = com[0]; 
	return ref;
}

// check for a mill in the vecinity of a given reference
int chk_mill (char player, ref_t vicinity, board_t board)
{ 
	// assume that vicinity = player
	cell_t cell = vicinity.cell;
	layer_t *layer = vicinity.layer; 
	// across 
	int n, i[] = {N, W, E, S}; 
	for (n=0; n<4; n++) {
		if (cell==i[n] && 
			  board.A[i[n]]==player &&
			  board.B[i[n]]==player &&
			  board.C[i[n]]==player )
			return 1;
	}
	// ccrners
	// NW, NE, SE, SW even 
	// N, W, S, E odd
	int m; 
	if ((cell/2.0)==(int)(cell/2.0)) { // is even
		n = cell + 1; 								// look right
		m = (cell==6) ? 0 : cell + 2; 
		if (layer[m]==player && layer[n]==player) 
			return 1; 
		n = (cell==0) ? 7 : cell - 1; // look left
		m = (cell==0) ? 6 : cell - 2; 
		if (layer[m]==player && layer[n]==player)
			return 1; }
	else { // is odd
		n = cell - 1; 								// left
		m = (cell==7) ? 0 : cell + 1; // right;
		if (layer[m]==player && layer[n]==player)
			return 1; }
	// corners
	// ##  Michael sieht mich an 
	// shittier method WAS here
	return 0; 
}

// check whether from and to are adjacent
int chk_is_adj (ref_t from, ref_t to)
{
	if (from.cell==to.cell &&
			abs(to.lay-from.lay) == 1) // 'B' - 'A' = 1
		return 1;
	else if (from.lay==to.lay) { 
		if ((from.cell == 0 && to.cell == 7) ||
				(from.cell == 7 && to.cell == 0))
			return 1;
		else if (abs(to.cell-from.cell)==1)
			return 1; }
	return 0;
}	

// check whter all stones are milled
int chk_milled (char player, board_t *board) 
{ 
	int stones = (player==WHITE) ? board->white : board->black;
	char com [] = "A0";
	ref_t t; 
	while (com[0] <= 'C') {
		t = get_ref(com, board); 
 		for (t.cell=0; t.cell<=7; t.cell++) {
			if (t.layer[t.cell] != player) continue;
			if (chk_mill(player, t, *board)) stones -= 1;
		} com[0] += 1; }
	if (stones) return 0; 
	else return 1;
}

Error* place (char player, board_t *board)
{
	char *err, com [MAX]; 
	if ((err = input_cell (com)) != 0)
		return err;
	scan_nl(); 
	ref_t target = get_ref (com, board);
	if (target.layer[target.cell] == VERT)
		target.layer[target.cell] = player;
	else
		return errOccupied;

	if (chk_mill(player, target, *board)) {
		disp_board (*board); 
		printf(" Mill! Take a stone: "); 
		while ((err = take(player, board)) !=0 )
			printf(ErrHandle, err); }
	return 0;
}
	
Error* move (char player, board_t *board)
{
	char *err;
	char a [MAX], b [MAX];
	if ((err = input_cell(a)) != 0) 
		return err;
	if ((err = input_cell(b)) != 0) 
		return err;
	scan_nl(); 

	ref_t from = get_ref(a, board),
				to = get_ref(b, board); 
	if (from.layer[from.cell] != player) 
		return errFrom; 
	if (to.layer[to.cell] != VERT) 
		return errNotEmpty;
	int stones = (player==WHITE) ? board->white : board->black;
	if (stones > 3 && chk_is_adj(from, to) == 0)
		return errNoJump;

	from.layer[from.cell] = VERT; 
	to.layer[to.cell] = player;
	if (chk_mill(player, to, *board)) {
		disp_board (*board); 
		printf(" Mill! Take a stone: "); 
		while ((err = take(player, board)) !=0 )
			printf(ErrHandle, err); }
	return 0;
}

Error* take (char player, board_t *board)
{
	char *err, com [MAX];
	if ((err = input_cell(com)) != 0)
		return err;
	scan_nl(); 
	ref_t target = get_ref (com, board);
	if (target.layer[target.cell] == player)
		return errYourStone;
	else if (target.layer[target.cell] == VERT)
		return errEmpty;
	else {
		// can't take a milled stone unless all pieces are milled
		char opp = (player==WHITE) ? BLACK : WHITE;
		if (chk_mill(opp, target, *board)) { 
			if (chk_milled(opp, board)==0)
				return errMilled; 
			} 
		target.layer[target.cell] = VERT;
		int *opponent = (player==WHITE) ? &(board->black) : &(board->white); 
		(*opponent) --;
	}
	return 0;
}
