#ifndef MOVEGEN_H__
#define MOVEGEN_H__

#include "coordinates.h"
#include "position.h"

#include <set>
using namespace std;

class MoveGen {
public:
    MoveGen();
    ~MoveGen();

    set<Move> AllLegalMoves(Position *position);

private:
    Position *position;

    set<Move> AllPseudolegalMoves();
};

#endif
