#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
using namespace std;

class Player {

private:
    Side  mySide;
    Side other;
    
public:
    Board *b;
    Player(Side side);
    ~Player();
    
    Move *doMove(Move *opponentsMove, int msLeft);
    double heuristic(Board*board);
    std::vector<Move> getOptions(Side side, Board * brd);
    Move getBestMove(std::vector<Move> moves);
    Move getBestMoveImproved(std::vector<Move> moves);
    double getMinIndex(std::vector<Board*> boards);
    double getMaxIndex(std::vector<Board*> boards);
    double simpleheurisitic(Board * b);
    Move getBestMoveNPly(std::vector<Move> moves, int maxlevel);
    void getScore(Board * brd, int maxlevel, int level, bool ourpick);
    
    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
