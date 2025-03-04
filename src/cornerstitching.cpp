#include "cornerstitching.h"
using namespace std;

// macro placement
void CS::MacroPlacement()
{
    // root
    root = new TILE(nullptr);
    root->lb = chip_lb;
    root->ur = chip_ur;
    root->W_H = chip_W_H;
    root->area = chip_area;
    root->center.Center(root->lb, root->W_H);
    tile_set.insert(root);
    // for(auto &macro : PADS)
    // {
    //     if(!macro->fixed && CanPlace(macro))
    //         CreatSolid(macro);      
    // }
    size_t count = 0;
    for(auto &macro : PADS)
    {
        if(!macro->fixed)
        {
            FPOS pos = minHPWLPos(macro);
            cout << "minHPWLPos: " << pos << endl;
            macro->lb = pos;
            macro->ur.update_ur(macro->lb, macro->W_H);
            macro->center.Center(macro->lb, macro->W_H);
            CreatSolid(macro);
        }
        if(++count > 10)
            break;
    }
}
FPOS CS::minHPWLPos(PAD *macro)
{
    FPOS pos;
    prec minHPWL = numeric_limits<double>::max();
    // place macro in empty tile
    for(auto tile : tile_set)
    {
        if(tile->macro)
            continue;
        macro->lb = tile->lb;
        macro->ur.update_ur(macro->lb, macro->W_H);
        macro->center.Center(macro->lb, macro->W_H);
        if(!CanPlace(macro))
            continue;
        // cal HPWL
        prec HPWL = 0;
        for(auto const &net : macro->nets)
        {
            net->updatePadPinPos();
            HPWL += net->padHPWL();
        }
        if(HPWL < minHPWL)
        {
            minHPWL = HPWL;
            pos = tile->lb;
        }
    }
    return pos;
}

// conerstitching
bool CS::CanPlace(PAD *macro)
{
    vector<TILE *> found_vec = FindArea(macro->lb, macro->ur);
    for(auto const &found : found_vec)
    {
        if(found->macro)
        {
            cout << "Error: " << macro->name << " " << found->macro->name << " overlap. \n";
            return false;
        }
    }
    return true;
}
void CS::CreatSolid(PAD *macro)
{
    TILE *tile = FindPoint(root, macro->lb);
    TILE *solid = nullptr;
    if(tile->lb.x < macro->lb.x)
        tile = CutY(tile, macro->lb.y); // cut bot
    cout << "cut bot\n";
    while(tile->ur.y <= macro->ur.y)
    {
        solid = CutXJoinY(tile, solid, macro); // cut side
        tile = solid->top;
        cout << "cut side\n";
    }
    if(tile->lb.y < macro->ur.y && macro->ur.y < tile->ur.y)
    {
        CutY(tile, macro->ur.y); // cut top
        cout << "cut top\n";
        solid = CutXJoinY(tile, solid, macro); // cut side
    }
    solid->macro = macro;
    return;
}
TILE *CS::CutXJoinY(TILE *tile, TILE *solid, PAD *macro)
{
    TILE *empty;
    if(tile->lb.x < macro->lb.x)
    {
        empty = tile;
        tile = CutX(tile, macro->lb.x); // cut left
        if(CanJoinY(empty, empty->bot))
            JoinY(empty, empty->bot);
    }
    if(macro->ur.x < tile->ur.x)
    {
        empty = CutX(tile, macro->ur.x); // cut right
        if(CanJoinY(empty, empty->bot))
            JoinY(empty, empty->bot);
    }
    if(solid)
        JoinY(tile, solid);
    return tile;
}
TILE *CS::CutX(TILE *parent, prec x)
{
    // parent(left) child(right) nbr(neighbor)
    TILE *child = new TILE(nullptr);
    TILE *nbr;
    // child
    child->lb.x = x;            
    child->lb.y = parent->lb.y;
    child->ur.x = parent->ur.x;
    child->ur.y = parent->ur.y;

    child->center.ur_Center(child->lb, child->ur);
    child->W_H.update_W_H(child->lb, child->ur);
    child->area = child->W_H.Cal_area();

    child->right = parent->right;
    child->top = parent->top;
    child->left = parent;
    if(parent->bot)
        for(nbr = parent->bot; nbr->lb.x <= x; nbr = nbr->right)
        {
            child->bot = nbr;
            if(!nbr->right) break;
        }        
    else  
        child->bot = nullptr;
    // parent
    parent->ur.x = x;
    
    parent->center.ur_Center(parent->lb, parent->ur);
    parent->W_H.update_W_H(parent->lb, parent->ur);
    parent->area = parent->W_H.Cal_area();

    parent->right = child;
    if(parent->top)
        for(nbr = child->top; x <= nbr->ur.x; nbr = nbr->left)
        {
            parent->top = nbr;
            if(!nbr->left) break;
        }
    else
        parent->top = nullptr;
    // nbr
    if(child->right)
        for(nbr = child->right; nbr->left == parent; nbr = nbr->bot)
        {
            nbr->left = child;
            if(!nbr->bot) break;
        }
    if(child->top)
        for(nbr = child->top; x <= nbr->lb.x; nbr = nbr->left)
        {
            nbr->bot = child;
            if(!nbr->left) break;
        }
    if(child->bot)
        for(nbr = child->bot; nbr->top == parent; nbr = nbr->right)
        {
            nbr->top = child;
            if(!nbr->right) break;
        }
    // update set
    tile_set.insert(child);
    return child;
}
TILE *CS::CutY(TILE *parent, prec y)
{
    // parent(bot) child(top) nbr(neighbor)
    TILE *child = new TILE(nullptr);
    TILE *nbr;
    // child
    child->lb.x = parent->lb.x;
    child->lb.y = y;
    child->ur.x = parent->ur.x;
    child->ur.y = parent->ur.y;

    child->center.ur_Center(child->lb, child->ur);
    child->W_H.update_W_H(child->lb, child->ur);
    child->area = child->W_H.Cal_area();

    child->top = parent->top;
    child->right = parent->right;
    child->bot = parent;
    if(parent->left)
        for(nbr = parent->left; nbr->lb.y <= y; nbr = nbr->top)
        {
            child->left = nbr;
            if(!nbr->top) break;
        }
    else
        child->left = nullptr;
    // parent
    parent->ur.y = y;

    parent->center.ur_Center(parent->lb, parent->ur);
    parent->W_H.update_W_H(parent->lb, parent->ur);
    parent->area = parent->W_H.Cal_area();

    parent->top = child;
    if(parent->right)
        for(nbr = child->right; y <= nbr->ur.y; nbr = nbr->bot)
        {
            parent->right = nbr;
            if(!nbr->bot) break;
        }
    else
        parent->right = nullptr;
    // nbr
    if(child->top)
        for(nbr = child->top; nbr->bot == parent; nbr = nbr->left)
        {
            nbr->bot = child;
            if(!nbr->left) break;
        }
    if(child->right)
        for(nbr = child->right; y <= nbr->lb.y; nbr = nbr->bot)
        {
            nbr->left = child;
            if(!nbr->bot) break;
        }
    if(child->left)
        for(nbr = child->left; nbr->right == parent; nbr = nbr->top)
        {
            nbr->right = child;
            if(!nbr->top) break;
        }
    // update set
    tile_set.insert(child);
    return child;
}
void CS::JoinX(TILE *keep, TILE *lose)
{
    TILE *nbr;
    // keep(left) lose(right)
    if(keep->lb.x < lose->lb.x)
    {
        keep->ur.x = lose->ur.x;

        keep->center.ur_Center(keep->lb, keep->ur);
        keep->W_H.update_W_H(keep->lb, keep->ur);
        keep->area = keep->W_H.Cal_area();

        keep->right = lose->right;
        keep->top = lose->top;

        for(nbr = lose->right; nbr->left == lose; nbr = nbr->bot)
        {
            nbr->left = keep;
            if(!nbr->bot) break;
        }
    }
    else // lose(left) keep(right)
    {
        keep->lb.x = lose->lb.x;

        keep->center.ur_Center(keep->lb, keep->ur);
        keep->W_H.update_W_H(keep->lb, keep->ur);
        keep->area = keep->W_H.Cal_area();

        keep->left = lose->left;
        keep->bot = lose->bot;

        for(nbr = lose->left; nbr->right == lose; nbr = nbr->top)
        {
            nbr->right = keep;
            if(!nbr->top) break;
        }
    }
    // dont care
    if(lose->top)
        for(nbr = lose->top; nbr->bot == lose; nbr = nbr->left)
        {
            nbr->bot = keep;
            if(!nbr->left) break;
        }
    if(lose->bot)
        for(nbr = lose->bot; nbr->top == lose; nbr = nbr->right)
        {
            nbr->top = keep;
            if(!nbr->right) break;
        }
    // update set
    tile_set.erase(lose);
}
void CS::JoinY(TILE *keep, TILE *lose)
{
    TILE *nbr;
    // keep(bot) lose(top)
    if(keep->lb.y < lose->lb.y)
    {
        keep->ur.y = lose->ur.y;

        keep->center.ur_Center(keep->lb, keep->ur);
        keep->W_H.update_W_H(keep->lb, keep->ur);
        keep->area = keep->W_H.Cal_area();
        
        keep->top = lose->top;
        keep->right = lose->right;

        for(nbr = lose->top; nbr->bot == lose; nbr = nbr->left)
        {
            nbr->bot = keep;
            if(!nbr->left) break;
        }
    }
    else // lose(bot) keep(top)
    {
        keep->lb.y = lose->lb.y;

        keep->center.ur_Center(keep->lb, keep->ur);
        keep->W_H.update_W_H(keep->lb, keep->ur);
        keep->area = keep->W_H.Cal_area();
        
        keep->bot = lose->bot;
        keep->left = lose->left;

        for(nbr = lose->bot; nbr->top == lose; nbr = nbr->right)
        {
            nbr->top = keep;
            if(!nbr->right) break;
        }
    }
    // dont care
    if(lose->right)
        for(nbr = lose->right; nbr->left == lose; nbr = nbr->bot)
        {
            nbr->left = keep;
            if(!nbr->bot) break;
        }
    if(lose->left)
        for(nbr = lose->left; nbr->right == lose; nbr = nbr->top)
        {
            nbr->right = keep;
            if(!nbr->top) break;
        }
    // update set
    tile_set.erase(lose);
}
bool CS::CanJoinX(TILE *a, TILE *b)
{
    if(!a || !b)
        return false;
    if(a->lb.y != b->lb.y || a->ur.y != b->ur.y || a->macro != b->macro)
        return false;
    return true;
}
bool CS::CanJoinY(TILE *a, TILE *b)
{
    if(!a || !b)
        return false;
    if(a->lb.x != b->lb.x || a->ur.x != b->ur.x || a->macro != b->macro)
        return false;
    return true;
}
TILE *CS::FindPoint(TILE *tile, FPOS pos)
{
    if(!tile)
        return nullptr;
    else if(tile->ur.y <= pos.y)
        return FindPoint(tile->top, pos);
    else if(pos.x < tile->lb.x)
        return FindPoint(tile->left, pos);
    else if(tile->ur.x <= pos.x)
        return FindPoint(tile->right, pos);
    else
        return tile;
}
vector<TILE *> CS::FindArea(FPOS lb, FPOS ur)
{
    vector<TILE *> found_vec;
    TILE *head = FindPoint(root, lb);
    while(lb.y < head->ur.y)
    {
        ProbeRytThenBot(found_vec, head, head->lb, lb, ur);
        if(!head->bot)
            break;
        head = head->bot;
        while(head->ur.x <= lb.x)
            head = head->right;
    }
    return found_vec;
}
void CS::ProbeRytThenBot(vector<TILE *> &found_vec, TILE *tile, FPOS head_lb, FPOS lb, FPOS ur)
{
    if(tile->lb.y < head_lb.y && head_lb.y < lb.y) // tile above head and not last head
        return;
    if(tile->ur.x < ur.x && tile->right)
        ProbeRytThenBot(found_vec, tile->right, head_lb, lb, ur);
    if(head_lb.y < tile->lb.y && tile->bot)
        ProbeRytThenBot(found_vec, tile->bot, head_lb, lb, ur);
    if(lb.y < tile->ur.y && tile->lb.y < ur.y)
        found_vec.push_back(tile);
    return;
}
