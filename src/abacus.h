#ifndef _ABACUS_H_
#define _ABACUS_H_

#include "datatype.h"
using namespace std;

class ABACUS
{
public:
    void Abacus();
    vector<vector<SROW *>> srow_vecvec;
private:
    // srow
    void CutRow(PAD *);
    size_t RowIndex(prec);
    // sort
    static bool CompareCell(MODULE *, MODULE *);
    // window
    void GetWindow(unordered_set<SROW *> &, size_t,  MODULE *);
    // vacancy
    prec GetVacancy(SROW *);
    // count
    void Count(size_t &);
    // place    
    void PlaceRow(SROW *, MODULE *);
    void Collapse(SROW *, CLUSTER *);
    void AddCell(CLUSTER *, MODULE *);
    void AddCluster(CLUSTER *, CLUSTER *);
    // temp
    prec TempPlaceRow(SROW *, MODULE *);
    void TempCollapse(SROW *, vector<CLUSTER> &, CLUSTER &);
};

#endif