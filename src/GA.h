#ifndef _GA_H_
#define _GA_H_

#include "datatype.h"
#include "cornerstitching.h"
using namespace std;

class INDIVIDUAL
{
public:    
    vector<PAD *> macro_vec;
    prec HPWL;
    prec fitness;

    bool operator<(const INDIVIDUAL &rhs) const
    {
        return HPWL < rhs.HPWL;
    }
};
class GA
{
public:
    void GA_main(CS &);

private:
    const size_t GENERATIONS = 0; 
    const size_t POPULATION_SIZE = 500;     
    const prec MUTATION_RATE = 0.1;  
    const prec CROSSOVER_RATE = 0.75;

    vector<INDIVIDUAL> InitializePopulation(CS &);
    INDIVIDUAL SelectParent(vector<INDIVIDUAL> &);
    INDIVIDUAL Crossover(const INDIVIDUAL &, const INDIVIDUAL &, CS &);
    void Mutation(INDIVIDUAL &);
    void Clean(CS &);
};

#endif