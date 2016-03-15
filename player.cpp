#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    mySide = side;
    other = (side == BLACK) ? WHITE : BLACK;

    // create board
    b = new Board();
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
     std::vector<Move> moves = getOptions(mySide, b);

     if(moves.empty()) return NULL;

     // select move that leads to hightest score
     //Move best = getBestMoveImproved(moves);
     int maxlevel;
     if(testingMinimax) maxlevel = 2;
     else maxlevel = 7;
     Move best = getBestMoveNPly(moves, maxlevel);
     Move * bestp = new Move(best.getX(), best.getY());

     b->doMove(bestp, mySide);

     return bestp;
}

// returns all possible moves given a board and a side to move
std::vector<Move> Player::getOptions(Side side, Board * brd)
{
    return brd->getAllMoves(side);
}

// very quick analysis of moves to find best one
Move Player::getBestMove(std::vector<Move> moves)
{
    double maxh = -1e20;
    int maxIndex = 0;

    for(int i = 0; i < (int)moves.size(); i++){
        Board * newb = b->copy();
        newb->doMove(&moves[i], mySide);
        double h = heuristic(newb);
        if(h > maxh){
            maxh = h;
            maxIndex = i;
        }
    }

    return moves[maxIndex];
}

// preforms 2=ply minimax to find the optimal move
Move Player::getBestMoveImproved(std::vector<Move> moves)
{
    std::vector<Board*> myboards;
    for(int i = 0; i < (int)moves.size(); i++){

        Board * newb = b->copy();
        newb->doMove(&moves[i], mySide);
        
        // calculate all the moves the opp can make from this board
        std::vector<Move> opptMoves = getOptions(other, newb);
        std::vector<Board*> oppBoards;
        for(int j = 0; j < (int)opptMoves.size(); j++){
            Board * newbopp = newb->copy();
            newbopp->doMove(&opptMoves[j], other);
            if(testingMinimax){
                newbopp->score = simpleheurisitic(newbopp);
            }else{
                newbopp->score = heuristic(newbopp);
            }
            oppBoards.push_back(newbopp);
        }

        if(oppBoards.size() == 0) newb->score = -10000;
        else newb->score = oppBoards[getMinIndex(oppBoards)]->score;
        myboards.push_back(newb);
    }

    int index = getMaxIndex(myboards);
    return moves[index];
}

Move Player::getBestMoveNPly(std::vector<Move> moves, int maxlevel)
{
    int maxI = -1;
    double max = -1.e8;

    for(int i = 0; i < (int)moves.size(); i++){
        Board * newb = b->copy();
        newb->doMove(&moves[i], mySide);
        getScore(newb, maxlevel, 1, false, -1.e7, 1.e7);
        if(newb->score > max){
        	max = newb->score;
        	maxI = i;
        }
    }

    //eturn moves[getMaxIndex(boards)];
    return moves[maxI];
}

void Player::getScore(Board * brd, int maxlevel, int level, bool ourpick, double alpha, double beta)
{
    if(level == maxlevel){
        if(!testingMinimax){
            brd->score = heuristic(brd);
        }else{
            brd->score = simpleheurisitic(brd);
        }
        return;
    }

    if(ourpick){
        // we pick the next move, maximize from this list
        std::vector<Move> movs = getOptions(mySide, brd);
        //std::vector<Board*> nextBoards;

        for(int i = 0; i < (int)movs.size(); i++){
            if(beta <= alpha){
               break;
            }
            Board * newb = brd->copy();
            newb->doMove(&movs[i], mySide);
            ourpick = !ourpick;
            getScore(newb, maxlevel, level + 1, ourpick, alpha, beta);
            if(newb->score > alpha){
                alpha = newb->score;
            }
            //nextBoards.push_back(newb);
        }


        if(movs.size() == 0){
            if(!testingMinimax){
                brd->score = heuristic(brd);
            }else{
                brd->score = simpleheurisitic(brd);
            }

            return;
        }
        else{
            brd->score = alpha;
        }
    }else{
        // opp picks best move, mimimize from this list
        std::vector<Move> movs = getOptions(other, brd);
        //std::vector<Board*> nextBoards;

        for(int i = 0; i < (int)movs.size(); i++){
            if(beta <= alpha){
               break;
            }
            Board * newb = brd->copy();
            newb->doMove(&movs[i], other);
            ourpick = !ourpick;
            getScore(newb, maxlevel, level + 1, ourpick, alpha, beta);
            if(newb->score < beta){
                beta = newb->score;
            }
            //nextBoards.push_back(newb);
        }

        if(movs.size() == 0){
            if(!testingMinimax){
                brd->score = heuristic(brd);
            }else{
                brd->score = simpleheurisitic(brd);
            }

            return;
        }
        else{
            brd->score = beta;
        }
    }

}

// returns the min index of a set of boards
double Player::getMinIndex(std::vector<Board*> boards)
{
    double min = 1.e20;
    double minIndex = -1;

    for(int i = 0; i < (int)boards.size(); i++){
        double score = boards[i]->score;
        if(score < min){
            min = score;
            minIndex = i;
        }
    }

    return minIndex;
}

// returns the max index of a set of boards
double Player::getMaxIndex(std::vector<Board*> boards)
{
    double max = -1.e20;
    double maxIndex = -1;

    for(int i = 0; i < (int)boards.size(); i++){
        double score = boards[i]->score;
        if(score > max){
            max = score;
            maxIndex = i;
        }
    }

    return maxIndex;
}

// simple board scoring function 
double Player::simpleheurisitic(Board * b)
{
    if(mySide == WHITE){
        return (b->countWhite() - b->countBlack());
    }else{
        return (b->countBlack() - b->countWhite());
    }
}

// returns a score relating to how optimal a board is
double Player::heuristic(Board * b)
{
    return b->dynamic_heuristic_evaluation_function(mySide);
}