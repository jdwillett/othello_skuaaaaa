#include "player.h"

Board * b;

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
	std::cerr << "started c\n";
    testingMinimax = false;

    mySide = side;
    other = (side == BLACK) ? WHITE : BLACK;

    // create board
    b = new Board();

    std::cerr << "finished constructing\n";
}

/*
 * Destructor for the player.
 */
Player::~Player() {
	delete b;
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
     b->doMove(opponentsMove, other);

     // find all possible moves
     std::cerr << "started domove\n";
     std::vector<Move> moves = getOptions();
     std::cerr << "got all the moves\n";

     std::cerr << "length of moves\n";

     if(moves.empty()) return NULL;

     for(int i = 0; i < (int)moves.size(); i++){
     	Move m = moves[i];
     	Move mov = m;
     	std::cerr << "(" << mov.getX() << ", " << mov.getY() << ")\n";
     }

     // select move that leads to hightest score
     Move best = getBestMove(moves);

     std::cerr << "best move: " << best.getX() << ", " << best.getY() << "\n";

     Move * bestp = new Move(best.getX(), best.getY());

     b->doMove(bestp, mySide);

     std::cerr << "ended domove\n";

     return bestp;
}


std::vector<Move> Player::getOptions()
{
	std::vector<Move> movs = b->getAllMoves(mySide);
	return movs;
}


Move Player::getBestMove(std::vector<Move> moves)
{
	double maxh = -1e20;
	int maxIndex = 0;

	for(int i = 0; i < (int)moves.size(); i++){
		Board * newb = b->copy();
		std::cerr << "processing move " << moves[i].getX() << ", " << moves[i].getY() << "\n";
		newb->doMove(&moves[i], mySide);
		double h = heuristic(newb);
		if(h > maxh){
			maxh = h;
			maxIndex = i;
		}
	}

	std::cerr << "chose " << moves[maxIndex].getX() << ", " << moves[maxIndex].getY() << "\n";

	return moves[maxIndex];

}

double Player::heuristic(Board * b)
{
	if(mySide == WHITE){
		return (double)(b->countWhite() - b->countBlack());
	}else{
		return (double)(b->countBlack() - b->countWhite());
	}

	cout << "XXXXXXXXXXXXXXXXXXXXXXX\n";
	return 0.0;
}

