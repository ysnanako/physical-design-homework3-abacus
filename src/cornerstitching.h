#ifndef _CS_H_
#define _CS_H_

#include "datatype.h"
using namespace std;

class CS
{
public:
    unordered_set<TILE *> TILES;
    TILE *root;

    prec HPWL(vector<PAD *> &);
private:
    FPOS minHPWLPos(PAD *);
    FPOS minAreaPos(PAD *);

    bool CanPlace(PAD *);
    void CreatSolid(PAD *);
    TILE *CutXJoinY(TILE *, TILE *, PAD *);

    TILE *CutX(TILE *, prec);
    TILE *CutY(TILE *, prec);
    void JoinX(TILE *, TILE *);
    void JoinY(TILE *, TILE *);
    bool CanJoinX(TILE *, TILE *);
    bool CanJoinY(TILE *, TILE *);

    TILE *FindPoint(TILE *, FPOS);
    vector<TILE *> FindArea(FPOS, FPOS);
    void ProbeRytThenBot(vector<TILE *> &, TILE *, FPOS, FPOS, FPOS);
};

#endif