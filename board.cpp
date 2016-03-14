#include "board.h"

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}

 
/*
 * Returns true if the game is finished; false otherwise. The game is finished 
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == NULL) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }

    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A NULL move means pass.
    if (m == NULL) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->getX();
    int Y = m->getY();
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
}

std::vector<Move> Board::getAllMoves(Side side){
    std::vector<Move> moves;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)){
                moves.push_back(move);
            }else{
            }
        }
    }
    return moves;
}

int Board::numValidMoves(Side side){
    int count = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) count++;
        }
    }
    return count;
}

double Board::dynamic_heuristic_evaluation_function(Side side)  {

    if(side == WHITE){
        if(countWhite() == 0)
            return -1.e7;
        if(countBlack() == 0)
            return 1.e7;
    }else{
        if(countBlack() == 0)
            return -1.e7; 
        if(countWhite() == 0)
            return 1.e7;
    }




    int my_tiles = 0, opp_tiles = 0, i, j, k, my_front_tiles = 0, opp_front_tiles = 0, x, y;
    double p = 0, c = 0, l = 0, m = 0, f = 0, d = 0;

    int X1[] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int Y1[] = {0, 1, 1, 1, 0, -1, -1, -1};
    int V[8][8] = {
        {20, -3, 11, 8, 8, 11, -3, 20},
        {-3, -7, -4, 1, 1, -4, -7, -3},
        {11, -4, 2, 2, 2, 2, -4, 11},
        {8, 1, 2, -3, -3, 2, 1, 8},
        {8, 1, 2, -3, -3, 2, 1, 8},
        {11, -4, 2, 2, 2, 2, -4, 11},
        {-3, -7, -4, 1, 1, -4, -7, -3},
        {20, -3, 11, 8, 8, 11, -3, 20}
    };
    // Piece difference, frontier disks and disk squares
    for(i=0; i<8; i++){
        for(j=0; j<8; j++)  {
            if(get(side, i, j))  {
                d += V[i][j];
                my_tiles++;
            } else if(occupied(i, j))  {
                d -= V[i][j];
                opp_tiles++;
            }
            if(occupied(i, j))   {
                for(k=0; k<8; k++)  {
                    x = i + X1[k]; 
                    y = j + Y1[k];
                    if(x >= 0 && x < 8 && y >= 0 && y < 8 && occupied(x, y)) {
                        if(get(side, i, j))  my_front_tiles++;
                        else opp_front_tiles++;
                        break;
                    }
                }
            }
        }
    }

    if(my_tiles > opp_tiles)
        p = (100.0 * my_tiles)/(my_tiles + opp_tiles);
    else if(my_tiles < opp_tiles)
        p = -(100.0 * opp_tiles)/(my_tiles + opp_tiles);
    else p = 0;

    if(my_front_tiles > opp_front_tiles)
        f = -(100.0 * my_front_tiles)/(my_front_tiles + opp_front_tiles);
    else if(my_front_tiles < opp_front_tiles)
        f = (100.0 * opp_front_tiles)/(my_front_tiles + opp_front_tiles);
    else f = 0;

    // Corner occupancy
    my_tiles = opp_tiles = 0;
    if(get(side, 0, 0)) my_tiles++;
    else if(occupied(0, 0)) opp_tiles++;
    if(get(side, 0, 7)) my_tiles++;
    else if(occupied(0, 7)) opp_tiles++;
    if(get(side, 7, 0)) my_tiles++;
    else if(occupied(7, 0)) opp_tiles++;
    if(get(side, 7, 7)) my_tiles++;
    else if(occupied(7, 7)) opp_tiles++;
    c = 25 * (my_tiles - opp_tiles);

// Corner closeness
    my_tiles = opp_tiles = 0;
    if(!occupied(0, 0))   {
        if(get(side, 0, 1)) my_tiles++;
        else if(occupied(0,1)) opp_tiles++;
        if(get(side, 1, 1)) my_tiles++;
        else if(occupied(1, 1)) opp_tiles++;
        if(get(side, 1, 0)) my_tiles++;
        else if(occupied(1,0)) opp_tiles++;
    }
    if(!occupied(0, 7))   {
        if(get(side, 0, 6)) my_tiles++;
        else if(occupied(0, 6)) opp_tiles++;
        if(get(side, 1, 6)) my_tiles++;
        else if(occupied(1, 6)) opp_tiles++;
        if(get(side, 1, 7)) my_tiles++;
        else if(occupied(1, 7)) opp_tiles++;
    }
    if(!occupied(7, 0))   {
        if(get(side, 7, 1)) my_tiles++;
        else if(occupied(7, 1)) opp_tiles++;
        if(get(side, 6, 1)) my_tiles++;
        else if(occupied(6, 1)) opp_tiles++;
        if(get(side, 6, 0)) my_tiles++;
        else if(occupied(6, 0)) opp_tiles++;
    }
    if(!occupied(7, 7))   {
        if(get(side, 6, 7)) my_tiles++;
        else if(occupied(6, 7)) opp_tiles++;
        if(get(side, 6, 6)) my_tiles++;
        else if(occupied(6, 6)) opp_tiles++;
        if(get(side, 7, 6)) my_tiles++;
        else if(occupied(7, 6)) opp_tiles++;
    }
    l = -12.5 * (my_tiles - opp_tiles);

    // Mobility
    Side other;

    if(side == WHITE)
        other = BLACK;
    else
        other = WHITE;

    my_tiles = this->numValidMoves(side);
    opp_tiles = this->numValidMoves(other);
    if(my_tiles > opp_tiles)
        m = (100.0 * my_tiles)/(my_tiles + opp_tiles);
    else if(my_tiles < opp_tiles)
        m = -(100.0 * opp_tiles)/(my_tiles + opp_tiles);
    else m = 0;

    // final weighted score
    double score = (10 * p) + (801.724 * c) + (382.026 * l) + (78.922 * m) + (74.396 * f) + (10 * d);
    return score;
}
