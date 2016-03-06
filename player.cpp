#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    Side mySide = side;
    Side other = (side == BLACK) ? WHITE : BLACK;

    // create board
    Board * b = &Board();




    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */

     // process opponents moves
     b.doMove(opponentsMove, other);

     // find all possible moves
     std::vector<Move*> moves = getOptions();

     // select move that leads to hightest score
     Move * best = getBestMove(moves);

     b.doMove(best, mySide);

    return *best;
}


std::vector<Move*> getOptions()
{
	return b.getAllMoves(mySide);
}


Move* getBestMove(std::vector<Move*> moves)
{
	double minh = 1e20;
	int minIndex = 0;

	for(int i = 0; i < (int)moves.size(); i++){
		Board * newb = b.copy();
		newb.doMove(moves[i], mySide);
		double h = heursitic(newb);
		if(h < minh){
			h = minh;
			minIndex = i;
		}
	}

	return moves[minIndex];

}

double heursitic(Board * b)
{

}

