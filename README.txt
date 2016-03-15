Done by both members together:
-Created the structure for the program
-Wrote pseudocode

Team member 1: Amine Boubezari
-  Wrote the getBestMove() function which finds the best move in one iteration
- Wrote the getBestMoveNply(int n) and getScore() methods which together recursively generate
ther search tree and finds the best move for any search depth n.
-debugged some issues relating to the search tree and the heuristic


Team member 2: Jonathan Willlett
- Wrote the comprehensive heuristic function, based on research done online
- Implemented alpha-beta pruning to optimize performance
- debugged issues relating to the heursitic



Improvments Implemented:

- Wrote a comprehensive heuristic function that weights each position on the board differently, as well as taking into
accout factors such as mobility, corner closeness, corner occupancy, and more. This gave us a huge improvement over 
the simple heuristic as difference in number of discs is actually a very poor measure of board ranking, especially in midgame.

- Implemented a general recursive function that allowed us to search to any depth. However depths greater than 7 ran out of memory
so our max depth is 7, even though larger depths still compute quickly enough. 

-We also made sure to stop at an odd depth because this allows all the scores of the tree to be based on our proactive maximization based on 
our own heurisitic. Stopping at an even depth bases the entire tree on the fact that the opponent will be using a similar heuristic to us, and assumes
the opponent will choose the move that minimizes the score based on our heurisitic.

- Implemented alpha-beta pruning to speed up computing time. This allowed us to go to larger depths without huge time costs.

- We tried modifying the heuristic function to place a huge cost on boards that result in losing positions for us, but that actually reduced performance
of our AI. We think this is because the large penalty is eliminating branches that have potentially good moves because the large penalty overwhelms it,
which is exarberbated by the fact that minimax assumes the opponent plays perfectly.