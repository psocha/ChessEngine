#ifndef MOVEGEN_H__
#define MOVEGEN_H__

#include <vector>
using namespace std;

class MoveGen {
public:
    MoveGen();
    ~MoveGen();

    vector<Move> AllLegalMoves(Position *position);

private:
    Position *position;

    vector<Move> AllPseudolegalMoves();
};

#endif
