#include "GA.h"
using namespace std;

void GA::GA_main(CS &CS_obj)
{
    vector<INDIVIDUAL> population = InitializePopulation(CS_obj);
    for(size_t i = 0; i < GENERATIONS; ++i)
    {
        vector<INDIVIDUAL> next_population;
        while(next_population.size() < POPULATION_SIZE)
        {
            INDIVIDUAL parent1 = SelectParent(population);
            INDIVIDUAL parent2 = SelectParent(population);
            INDIVIDUAL child = Crossover(parent1, parent2, CS_obj);
            Mutation(child);

            child.HPWL = CS_obj.HPWL(child.macro_vec);
            Clean(CS_obj);

            next_population.push_back(child);

            cout << "\r" << setw(2) << i + 1 << "/" << GENERATIONS
            << " (" << setw(3) << next_population.size() << "/" << POPULATION_SIZE << ")"
            << "  HPWL: " << setprecision(3) << child.HPWL << flush;
        }
        population = next_population;
    }
    sort(population.begin(), population.end());

    cout << "\n\nFINAL HPWL: " << CS_obj.HPWL(population[0].macro_vec) << endl;
}
vector<INDIVIDUAL> GA::InitializePopulation(CS & CS_obj)
{
    vector<INDIVIDUAL> population(POPULATION_SIZE);
    for(auto &individual : population)
    {
        individual.macro_vec.assign(PADS.begin(), PADS.end());
        random_shuffle(individual.macro_vec.begin(), individual.macro_vec.end());

        individual.HPWL = CS_obj.HPWL(individual.macro_vec);
        Clean(CS_obj);
        cout << "\r" << setw(2) << "0" << "/" << GENERATIONS
        << " (" << setw(3) << &individual - &population[0] + 1 << "/" << POPULATION_SIZE << ")"
        << "  HPWL: " << setprecision(3) << individual.HPWL << flush;
    }
    return population;
}
// roulette wheel selection
INDIVIDUAL GA::SelectParent(vector<INDIVIDUAL> &population)
{
    prec sum = 0;
    for(auto &individual : population)
    {
        individual.fitness = 1 / sqrt(individual.HPWL);
        sum += individual.fitness;
    }
    prec rand_num = (rand() % 10000) / 10000.0 * sum;
    for(auto const &individual : population)
    {
        rand_num -= individual.fitness;
        if(rand_num <= 0)
            return individual;
    }
    return population.back();
}
INDIVIDUAL GA::Crossover(const INDIVIDUAL &parent1, const INDIVIDUAL &parent2, CS &CS_obj)
{
    INDIVIDUAL child;
    child.macro_vec = parent1.macro_vec;
    for(auto const &macro : parent2.macro_vec)
    {
        if(find(child.macro_vec.begin(), child.macro_vec.end(), macro) == child.macro_vec.end())
            child.macro_vec.push_back(macro);
    }
    return child;
}
void GA::Mutation(INDIVIDUAL &individual)
{
    if((rand() % 10000) / 10000.0 < MUTATION_RATE)
    {
        size_t idx1 = rand() % individual.macro_vec.size();
        size_t idx2 = rand() % individual.macro_vec.size();
        swap(individual.macro_vec[idx1], individual.macro_vec[idx2]);
    }
}
void GA::Clean(CS &CS_obj)
{
    for(auto &tile : CS_obj.TILES)
        delete tile;
    CS_obj.TILES.clear();
    for(auto &macro : PADS)
        macro->placed = false;
}