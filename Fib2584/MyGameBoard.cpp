#include "MyGameBoard.h"
#include <algorithm>

const int MyGameBoard::fibonacci_[32] = {0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309};
Random MyGameBoard::random_;

MyGameBoard::MyGameBoard():
board_(0)
{
}

MyGameBoard::MyGameBoard(int board[4][4]) {
    board_ = 0;
    for (int i = 0; i < 16; i++) {
        board_ <<= 5;
        board_ |= getIndex(board[i / 4][i % 4]);
    }
}

int MyGameBoard::getIndex(int fib) {
    return lower_bound(fibonacci_, fibonacci_ + 32, fib) - fibonacci_;
}

vector<int> MyGameBoard::getVec() {
    int b[4][4], tb[2][4][4], cb[2][4][4];
    getArrayBoard(b);
    vector<int> ret(48);
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            cb[0][i][j] = getIndex(b[i][j]);
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            cb[1][i][j] = cb[0][3-i][j];
    for (int r = 0; r < 4; ++r) {
        for (int m = 0; m < 2; ++m) {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    tb[m][i][j] = cb[m][j][3-i];
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    cb[m][i][j] = tb[m][i][j];
            int idx = 12 * r + 6 * m;
            for (int t = 0; t < 2; t++) {
                ret[idx + t] = t;
                ret[idx + t] = ret[idx + t] * 22 + cb[m][0][t];
                ret[idx + t] = ret[idx + t] * 22 + cb[m][1][t];
                ret[idx + t] = ret[idx + t] * 22 + cb[m][2][t];
                ret[idx + t] = ret[idx + t] * 22 + cb[m][3][t];
                ret[idx + t] = ret[idx + t] * 22 + cb[m][3][t+1];
                ret[idx + t] = ret[idx + t] * 22 + cb[m][2][t+1];
                ret[idx + t + 2] = t + 2;
                ret[idx + t + 2] = ret[idx + t + 2] * 22 + cb[m][0][t+1];
                ret[idx + t + 2] = ret[idx + t + 2] * 22 + cb[m][1][t+1];
                ret[idx + t + 2] = ret[idx + t + 2] * 22 + cb[m][2][t+1];
                ret[idx + t + 2] = ret[idx + t + 2] * 22 + cb[m][2][t+2];
                ret[idx + t + 2] = ret[idx + t + 2] * 22 + cb[m][1][t+2];
                ret[idx + t + 2] = ret[idx + t + 2] * 22 + cb[m][0][t+2];
                ret[idx + t + 4] = t + 4;
                ret[idx + t + 4] = ret[idx + t + 4] * 22 + cb[m][1-t][0+t];
                ret[idx + t + 4] = ret[idx + t + 4] * 22 + cb[m][2-t][0+t];
                ret[idx + t + 4] = ret[idx + t + 4] * 22 + cb[m][3-t][0+t];
                ret[idx + t + 4] = ret[idx + t + 4] * 22 + cb[m][2-t][1+t];
                ret[idx + t + 4] = ret[idx + t + 4] * 22 + cb[m][3-t][1+t];
                ret[idx + t + 4] = ret[idx + t + 4] * 22 + cb[m][3-t][2+t];
            }
        }
    }
    return ret;
}

void MyGameBoard::initialize()
{
	board_ = 0;
	addRandomTile();
	addRandomTile();
}

int MyGameBoard::move(MoveDirection moveDirection)
{
	BitBoard newBoard = 0;
	int score = 0;
	if(moveDirection == MOVE_UP || moveDirection == MOVE_DOWN) {
		for(int i = 0;i < 4;i++) {
			BitBoard tempColumn = 0;
			tempColumn = getColumn(i);
			newBoard |= restoreColumn(MoveTable::move_table.row_move_table_[moveDirection == MOVE_DOWN][tempColumn], i);
			score += MoveTable::move_table.row_move_score_table_[moveDirection == MOVE_DOWN][tempColumn];
		}
	}
	else {
		for(int i = 0;i < 4;i++) {
			BitBoard tempRow = 0;
			tempRow = getRow(i);
			newBoard |= restoreRow(MoveTable::move_table.row_move_table_[moveDirection == MOVE_RIGHT][tempRow], i);
			score += MoveTable::move_table.row_move_score_table_[moveDirection == MOVE_RIGHT][tempRow];
		}
	}
	board_ = newBoard;
	return score;
}

void MyGameBoard::addRandomTile()
{
	int oneTileRate = 6;
	int emptyTileNum = countEmptyTile();
	int randomTileLocation = random_.get_rand_num() % emptyTileNum;
	BitBoard randomTile = (random_.get_rand_num() % 8 < oneTileRate)?0x1:0x3;
	int count = 0;
	for(BitBoard tileMask = 0x1f;tileMask != 0;tileMask <<= 5, randomTile <<= 5) {
		if((board_ & tileMask) != 0)
			continue;
		if(count == randomTileLocation) {
			board_ |= randomTile;
			break;
		}
		count++;
	}
}

int MyGameBoard::countEmptyTile()
{
	int count = 0;
	BitBoard tileMask = 0x1f;
	for(;tileMask != 0;tileMask <<= 5) {
		if((board_ & tileMask) == 0)
			count++;
	}
	return count;
}

bool MyGameBoard::terminated()
{
	bool movable = false;
	BitBoard tempColumn;
	BitBoard tempRow;
	for(int i = 0;i < 4;i++) {
		tempColumn = getColumn(i);
		movable |= MoveTable::move_table.row_can_move_[0][tempColumn];
		movable |= MoveTable::move_table.row_can_move_[1][tempColumn];
		tempRow = getRow(i);
		movable |= MoveTable::move_table.row_can_move_[0][tempRow];
		movable |= MoveTable::move_table.row_can_move_[1][tempRow];
		if(movable)
			break;
	}
	return !movable;
}

void MyGameBoard::getArrayBoard(int board[4][4])
{
	BitBoard tempBoard = board_;
	for(int i = 0;i < 16;i++) {
		board[3 - (i / 4)][3 - (i % 4)] = fibonacci_[(int)(tempBoard & 0x1f)];
		tempBoard = tempBoard >> 5;
	}
}

int MyGameBoard::getMaxTile()
{
	BitBoard countBoard = board_;
	int max_tile = 0;
	for(int i = 0;i < 16;i++) {
		if((countBoard & 0x1f) > max_tile)
			max_tile = countBoard & 0x1f;
		countBoard >>= 5;
	}
	return fibonacci_[max_tile];
}

BitBoard MyGameBoard::getRow( int row )
{
	return board_ >> (row * 20) & 0xfffff;
}

BitBoard MyGameBoard::getColumn( int column )
{
	BitBoard tempBoard = board_ >> (column * 5) & BitBoard(0x0001, 0xf0001f0001f0001f);
	return (tempBoard & BitBoard(0x0001, 0xf000000000000000)) >> 45 |
      	   (tempBoard & BitBoard(0x0000, 0x00001f0000000000)) >> 30 |
      	   (tempBoard & BitBoard(0x0000, 0x0000000001f00000)) >> 15 |
       	   (tempBoard & BitBoard(0x0000, 0x000000000000001f));
}

BitBoard MyGameBoard::restoreRow( BitBoard rowBits, int row )
{
  	return rowBits << (row * 20);
}

BitBoard MyGameBoard::restoreColumn( BitBoard columnBits, int column )
{
  	return ((columnBits & BitBoard(0xf8000)) << 45 |
            (columnBits & BitBoard(0x07c00)) << 30 |
            (columnBits & BitBoard(0x003e0)) << 15 |
            (columnBits & BitBoard(0x0001f)) ) << (column * 5);
}

int MyGameBoard::getTile( int row, int column )
{
	int tile = ((board_ >> (row * 20) & 0xfffff) >> (column * 5)) & 0x1f;
	return fibonacci_[tile];
}

void MyGameBoard::showBoard()
{
  	for(int row = 3;row >= 0;row--) {
   		for(int column = 3;column >= 0;column--)
      		cout << dec << getTile(row, column) << "\t";
    	cout << "\n";
  	}
}

bool MyGameBoard::operator==(MyGameBoard gameBoard)
{
	return board_ == gameBoard.board_;
}
