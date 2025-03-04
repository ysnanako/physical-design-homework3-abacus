#include "abacus.h"
using namespace std;

void ABACUS::Abacus()
{
    // row
    for(auto const &row : ROWS)
    {
        SROW *srow = new SROW;
        srow->lb = row->lb;
        srow->W_H = row->W_H;
        srow->ur.update_ur(srow->lb, srow->W_H);
        vector<SROW *> srow_vec;
        srow_vec.push_back(srow);
        srow_vecvec.push_back(srow_vec);
    }
    // srow
    for(auto const &pad : PADS)
    {
        if(!pad->is_cover)
            CutRow(pad);
    }
    // sort 
    sort(MODULES.begin(), MODULES.end(), ABACUS::CompareCell);
    // count
    size_t count = 0;
    // place
    for(auto &cell : MODULES)
    {
        prec min_cost = PREC_MAX;
        SROW *min_srow = nullptr;
        unordered_set<SROW *> window;
        for(size_t window_num = 5; min_cost == PREC_MAX; window_num += 5)
        {
            GetWindow(window, window_num, cell);
            for(auto &srow : window)
            {
                if(GetVacancy(srow) < cell->W_H.x)
                    continue;
                prec cost = TempPlaceRow(srow, cell);
                if(cost < min_cost)
                {
                    min_cost = cost;
                    min_srow = srow;
                }
            }
        }
        PlaceRow(min_srow, cell);
        Count(count);
    }
    cout << endl;
}

// srow
void ABACUS::CutRow(PAD *pad)
{
    size_t end = RowIndex(pad->ur.y);
    for(size_t i = RowIndex(pad->lb.y); i < end; ++i)
    {
        for(size_t j = 0; j < srow_vecvec[i].size(); ++j)
        {
            SROW *srow = srow_vecvec[i][j];
            FPOS overlap; // start, end
            overlap.x = max(srow->lb.x, pad->lb.x);
            overlap.y = min(srow->ur.x, pad->ur.x);
            if(overlap.x >= overlap.y)
                continue;
            // if(overlap.x <= srow->lb.x && srow->ur.x <= overlap.y)
            if(srow->lb.x < overlap.x)
            {
                SROW *left = new SROW;
                left->lb = srow->lb;
                left->W_H.x = pad->lb.x - srow->lb.x;
                left->W_H.y = srow->W_H.y;
                left->ur.update_ur(left->lb, left->W_H);
                srow_vecvec[i].insert(srow_vecvec[i].begin() + j++, left);
            }
            if(overlap.y < srow->ur.x)
            {
                SROW *right = new SROW;
                right->lb.x = pad->ur.x;
                right->lb.y = srow->lb.y;
                right->W_H.x = srow->ur.x - pad->ur.x;
                right->W_H.y = srow->W_H.y;
                right->ur.update_ur(right->lb, right->W_H);
                srow_vecvec[i].insert(srow_vecvec[i].begin() + j++, right);
            }
            srow_vecvec[i].erase(srow_vecvec[i].begin() + j--);
        }
    }
}
size_t ABACUS::RowIndex(prec y)
{
    return (y - core_lb.y) / ROWS.front()->W_H.y;
}

// cell
bool ABACUS::CompareCell(MODULE *a, MODULE *b)
{
    return a->lb.x < b->lb.x;
}

// window
void ABACUS::GetWindow(unordered_set<SROW *> &window, size_t window_num, MODULE *cell)
{
    size_t center = RowIndex(cell->lb.y);
    for(size_t extend = 0; window.size() < window_num; ++extend)
    {
        size_t start = (extend > center) ? 0 : center - extend;
        size_t end = min(center + extend, srow_vecvec.size() - 1);

        for(size_t index = start; index <= end; ++index)
        {
            for(auto &srow : srow_vecvec[index])
            {
                FPOS overlap; // start, end
                overlap.x = max(srow->lb.x, cell->lb.x - srow->W_H.y * extend);
                overlap.y = min(srow->ur.x, cell->ur.x + srow->W_H.y * extend);
                if(overlap.x <= overlap.y)
                    window.insert(srow);
            }
        }
    }
}

// vacancy
prec ABACUS::GetVacancy(SROW *srow)
{
    prec vacancy = srow->W_H.x;
    for(auto const &cluster : srow->cluster_vec)
        vacancy -= cluster->w;
    return vacancy; 
}

// count
void ABACUS::Count(size_t &count)
{
    size_t total = MODULES.size();
    const size_t width = 50;
    float percent = static_cast<float>(++count) / total;

    cout << "[";
    size_t pos = static_cast<size_t>(percent * width);
    for (size_t i = 0; i < width; ++i)
    {
        if (i < pos)
            cout << "=";
        else if (i == pos)
            cout << ">";
        else
            cout << " ";
    }
    cout << "] " << fixed << setprecision(2) << (percent * 100) << "% (" << count << "/" << total << ")\r";
    cout.flush();
}

// place
void ABACUS::PlaceRow(SROW *srow , MODULE *cell)
{
    // shift
    if(cell->lb.x < srow->lb.x)
        cell->lb.x = srow->lb.x;
    if(srow->ur.x < cell->ur.x)
        cell->lb.x = srow->ur.x - cell->W_H.x;
    cell->ur.update_ur(cell->lb, cell->W_H);
    cell->center.Center(cell->lb, cell->W_H);
    // first cell or does not overlap
    if(!srow->cluster_vec.size() || srow->cluster_vec.back()->lb.x + srow->cluster_vec.back()->w < cell->lb.x)
    {
        CLUSTER *new_cluster = new CLUSTER;
        new_cluster->lb.x = cell->lb.x;
        new_cluster->lb.y = srow->lb.y;
        AddCell(new_cluster, cell);
        srow->cluster_vec.push_back(new_cluster);
    }
    else // overlap
    {
        AddCell(srow->cluster_vec.back(), cell);
        Collapse(srow, srow->cluster_vec.back());
    }
    // cell pos
    for(auto &cluster : srow->cluster_vec)
    {
        FPOS pos = cluster->lb;
        for(auto &cell : cluster->module_vec)
        {
            cell->lb = pos;
            cell->ur.update_ur(cell->lb, cell->W_H);
            cell->center.Center(cell->lb, cell->W_H);
            pos.x += cell->W_H.x;
        }
    }
}
void ABACUS::Collapse(SROW *srow, CLUSTER *cluster)
{
    // pos
    cluster->lb.x = static_cast<int>(cluster->q) / static_cast<int>(cluster->e);
    // boundary
    if(cluster->lb.x < srow->lb.x)
        cluster->lb.x = srow->lb.x;
    if(cluster->lb.x > srow->ur.x - cluster->w)
        cluster->lb.x = srow->ur.x - cluster->w;
    // check prev cluster
    // auto it = find(srow->cluster_vec.begin(), srow->cluster_vec.end(), cluster);
    if(1 < srow->cluster_vec.size()) //it != srow->cluster_vec.begin()
    {
        CLUSTER *prev_cluster = srow->cluster_vec.at(srow->cluster_vec.size() - 2); // *(prev(it))
        if(cluster->lb.x < prev_cluster->lb.x + prev_cluster->w)
        {
            // merge
            AddCluster(prev_cluster, cluster);
            srow->cluster_vec.pop_back();
            Collapse(srow, prev_cluster);
        }
    }
}
void ABACUS::AddCell(CLUSTER *cluster, MODULE *cell)
{
    cluster->module_vec.push_back(cell);
    cluster->e += 1;
    cluster->q += cell->lb.x - cluster->w;
    cluster->w += cell->W_H.x;
}
void ABACUS::AddCluster(CLUSTER *cluster, CLUSTER *add_cluster)
{
    cluster->module_vec.insert(cluster->module_vec.end(), add_cluster->module_vec.begin(), add_cluster->module_vec.end());
    cluster->e += add_cluster->e;
    cluster->q += add_cluster->q - add_cluster->e * cluster->w;
    cluster->w += add_cluster->w;
}

// temp
prec ABACUS::TempPlaceRow(SROW *srow, MODULE *cell)
{
    // copy
    vector<CLUSTER> temp_cluster_vec;
    for(auto &cluster : srow->cluster_vec)
    {
        CLUSTER temp_cluster = *cluster;
        temp_cluster_vec.push_back(temp_cluster);
    }
    MODULE temp_cell = (*cell);
    // shift
    if(cell->lb.x < srow->lb.x)
        temp_cell.lb.x = srow->lb.x;
    if(srow->ur.x < cell->ur.x)
        temp_cell.lb.x = srow->ur.x - cell->W_H.x;
    temp_cell.ur.update_ur(temp_cell.lb, temp_cell.W_H);
    temp_cell.center.Center(temp_cell.lb, temp_cell.W_H);
    // first cell or does not overlap
    if(!temp_cluster_vec.size() || temp_cluster_vec.back().lb.x + temp_cluster_vec.back().w < temp_cell.lb.x)
    {
        CLUSTER new_cluster;
        new_cluster.lb.x = temp_cell.lb.x;
        new_cluster.lb.y = srow->lb.y;
        AddCell(&new_cluster, &temp_cell);
        temp_cluster_vec.push_back(new_cluster);
    }
    else // overlap
    {
        AddCell(&temp_cluster_vec.back(), &temp_cell);
        TempCollapse(srow, temp_cluster_vec, temp_cluster_vec.back());
    }
    // cost
    prec x = temp_cluster_vec.back().lb.x + temp_cluster_vec.back().w - cell->W_H.x;
    return abs(x - cell->lb.x) + abs(temp_cluster_vec.back().lb.y - cell->lb.y);
}
void ABACUS::TempCollapse(SROW *srow, vector<CLUSTER> &cluster_vec, CLUSTER &cluster)
{
    // pos
    cluster.lb.x = static_cast<int>(cluster.q) / static_cast<int>(cluster.e);
    // boundary
    if(cluster.lb.x < srow->lb.x)
        cluster.lb.x = srow->lb.x;
    if(cluster.lb.x > srow->ur.x - cluster.w)
        cluster.lb.x = srow->ur.x - cluster.w;
    // check prev cluster
    // auto it = find(cluster_vec.begin(), cluster_vec.end(), cluster);
    if(1 < cluster_vec.size()) // it != cluster_vec.begin() 
    {
        // auto prev_it = prev(it);
        CLUSTER &prev_cluster = cluster_vec.at(cluster_vec.size() - 2); // *(prev_it);
        if(cluster.lb.x < prev_cluster.lb.x + prev_cluster.w)
        {
            // merge
            AddCluster(&prev_cluster, &cluster);
            cluster_vec.pop_back();
            TempCollapse(srow, cluster_vec, prev_cluster);
        }
    }
}