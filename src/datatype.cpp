////////////////////////////////////////////////////
// * brief
// * LAB       NCKU_SEDA 112
// * date      2024/7/11 update
// * version   1.0.0

// * author    PY , EasyMoney
////////////////////////////////////////////////////

#include "datatype.h"
using namespace std;

// ----------------------------------------------------------------------------

// ..... FPOS ..... //

void FPOS::Set(prec a) {
    x = y = a;
}

// void FPOS::Set(FPOS a) {
//     x = a.x;
//     y = a.y;
// }

void FPOS::Set(prec xloc, prec yloc) {
    x = xloc;
    y = yloc;
}

void FPOS::SetZero()
{
    x = y = 0;
}

// prec FPOS::GetX() {
//     return x;
// };
// prec FPOS::GetY() {
//     return y;
// };

// void FPOS::Add(FPOS a) {
//     x += a.x;
//     y += a.y;
// }

// void FPOS::SetAdd(FPOS a, FPOS b)
// {
//     x = a.x + b.x;
//     y = a.y + b.y;
// }

void FPOS::Min(FPOS a)
{
    x = min(x, a.x);
    y = min(y, a.y);
}
void FPOS::SetMin(FPOS a, FPOS b)
{
    x = min(a.x, b.x);
    y = min(a.y, b.y);
}

void FPOS::Max(FPOS a)
{
    x = max(x, a.x);
    y = max(y, a.y);
}
void FPOS::SetMax(FPOS a, FPOS b)
{
    x = max(a.x, b.x);
    y = max(a.y, b.y);
}

void FPOS::Center(FPOS lb, FPOS W_H)
{
    x = lb.x + (W_H.x / 2.);
    y = lb.y + (W_H.y / 2.);
}
void FPOS::ur_Center(FPOS lb, FPOS ur)
{
    x = (lb.x + ur.x) / 2.;
    y = (lb.y + ur.y) / 2.;
}

void FPOS::update_lb(FPOS center, FPOS W_H)
{
    x = center.x - (W_H.x / 2.);
    y = center.y - (W_H.y / 2.);
}
void FPOS::update_ur(FPOS lb, FPOS W_H)
{
    x = lb.x + W_H.x;
    y = lb.y + W_H.y;
}
void FPOS::update_W_H(FPOS lb, FPOS ur)
{
    x = ur.x - lb.x;
    y = ur.y - lb.y;
}

// void FPOS::find_W_H(FPOS lb, FPOS ur) {
//     this = ur - lb;
// }

prec FPOS::Cal_area(FPOS ur) /// for lb
{
    return (ur.x - x) * (ur.y - y);
}
prec FPOS::Cal_area() /// for W_H
{
    return x * y;
}

prec FPOS::GetProduct()
{
    return x * y;
}
// void FPOS::Dump() {
//     cout << "(" << x << " " << y << ")" << endl;
// }

// void FPOS::Dump(string a) {
//     cout << a << ": (" << x << " " << y << ")" << endl;
// }

void FPOS::Swap()
{
    prec temp = x;
    x = y;
    y = temp;
}

FPOS::~FPOS() {}

// ..... POS ..... //

int POS::GetProduct()
{
    return x * y;
}

void POS::SetZero()
{
    x = y = 0;
}

// ..... RECT ..... //

RECT::~RECT() {}

// ..... MODULE ..... //

MODULE::MODULE()
{ /// initial
    name = "";
    id = 0;

    center = FPOS();
    lb = FPOS(); /// left-bottom corner
    ur = FPOS();

    best_center = FPOS();

    ori_W_H = FPOS(); // ori_W = ori_H = 0;
    ori_area = 0;

    // - inflat
    W_H = FPOS(); // W = H = 0;
    area = 0;
    area_Inv = 0;

    // global_inflation_ratio = 1; // ! global
    local_inflation_ratio = 1;

    is_std = 1; /// 1:standard cell | 0:macro | -1:filler
    fixed = false;

    den_scal = 1;

    nets.clear();
    pins.clear();

    // polys.clear(); /// 多邊形

    // - Power
    static_power = voltage = 0;
    crossnode = nullptr;

    // - Fence
    FENCE *fence = nullptr;
    REGION *region = nullptr;

    type = "";
    category = "";
    orientation = 0; /// 0(N), 1(W), 2(S), 3(E), 4(FN), 5(FW), 6(FS), 7(FE)

    total_grad = nullptr;
    wl_grad = nullptr;
    glo_d_grad = nullptr;
    loc_d_grad = nullptr;
    loc_d_lambda = nullptr;
}

MODULE &MODULE::operator=(const MODULE &module)
{
    if (&module != this)
    {
        name = module.name;
        id = module.id;

        center = module.center;
        lb = module.lb; /// left-bottom corner
        ur = module.ur;

        best_center = module.best_center;

        ori_W_H = module.ori_W_H;
        ori_area = module.ori_area;

        // - inflat
        W_H = module.W_H;
        area = module.area;
        area_Inv = module.area_Inv;

        // global_inflation_ratio = module.global_inflation_ratio;
        local_inflation_ratio = module.local_inflation_ratio;

        is_std = module.is_std; /// 1: standard cell, 0: macro
        fixed = module.fixed;

        den_scal = module.den_scal;

        nets.clear();
        nets = module.nets; // ? 深拷貝
        // for (vector< NET * >::const_iterator  iter = module.nets.begin(); iter != module.nets.end(); iter++)
        // 	nets.push_back(*iter);

        pins.clear();
        pins = module.pins;
        // for (vector< PIN * >::const_iterator  iter = module.pins.begin(); iter != module.pins.end(); iter++)
        // 	pins.push_back(*iter);

        // polys.clear(); /// 多邊形
        // polys = module.polys;
        // for (FPOS point : module.polys)
        // 	polys.push_back(point);

        // - Power
        static_power = module.static_power;
        voltage = module.voltage;
        crossnode = module.crossnode;

        // - Fence
        fence = module.fence;
        region = module.region;

        type = module.type;
        category = module.category;
        orientation = module.orientation;

        total_grad = module.total_grad, wl_grad = module.wl_grad, glo_d_grad = module.glo_d_grad, loc_d_grad = module.loc_d_grad;
        loc_d_lambda = module.loc_d_lambda;
    }

    return *this;
}

MODULE::~MODULE() {}

// ..... NET ..... //

NET::NET()
{
    name = "";
    id = 0;

    net_weight = 1;

    pins.clear();
    pin_lb = FPOS();
    pin_ur = FPOS();

    // macro placement (nanako)
    padpin_lb.Set(numeric_limits<double>::max());
    padpin_ur.Set(numeric_limits<double>::min());

    // - wirelength
    WL = 0;
    ExpSum_P = FPOS();
    ExpSum_N = FPOS();     /// < Sum(EXP(X)),Sum(EXP(Y)) > &  < Sum(EXP(-X)),Sum(EXP(-Y)) >
    ExpSum_P_Inv = FPOS();
    ExpSum_N_Inv = FPOS(); /// < 1/Sum(EXP(X)),1/Sum(EXP(Y)) > &  < 1/Sum(EXP(-X)),1/Sum(EXP(-Y)) >
    WA_ExpSum_P = FPOS();
    WA_ExpSum_N = FPOS();  /// < X*Sum(EXP(X)),Y*Sum(EXP(Y)) > &  < -X*Sum(EXP(-X)),-Y*Sum(EXP(-Y)) >
}

NET &NET::operator=(const NET &net)
{
    if (&net != this)
    {
        name = net.name;
        id = net.id;

        net_weight = net.net_weight;

        pins.clear();
        pins = net.pins;
        // for (vector<PIN *>::const_iterator iter = net.pins.begin(); iter != net.pins.end(); iter++)
        //     pins.push_back(*iter);
        pin_lb = net.pin_lb;
        pin_ur = net.pin_ur;

        // macro placement (nanako)
        pin_lb = net.pin_lb;
        pin_ur = net.pin_ur;

        // - wirelength
        WL = net.WL;
        ExpSum_P = net.ExpSum_P;
        ExpSum_N = net.ExpSum_N;         /// < Sum(EXP(X)),Sum(EXP(Y)) > &  < Sum(EXP(-X)),Sum(EXP(-Y)) >
        ExpSum_P_Inv = net.ExpSum_P_Inv;
        ExpSum_N_Inv = net.ExpSum_N_Inv; /// < 1/Sum(EXP(X)),1/Sum(EXP(Y)) > &  < 1/Sum(EXP(-X)),1/Sum(EXP(-Y)) >
        WA_ExpSum_P = net.WA_ExpSum_P;
        WA_ExpSum_N = net.WA_ExpSum_N;   /// < X*Sum(EXP(X)),Y*Sum(EXP(Y)) > &  < -X*Sum(EXP(-X)),-Y*Sum(EXP(-Y)) >
    }

    return *this;
}

prec NET::padHPWL()
{
    if(pads.empty()) return 0;
    else return (padpin_ur.x - padpin_lb.x) + (padpin_ur.y - padpin_lb.y);
}

void NET::updatePadPinPos()
{
    padpin_lb.Set(numeric_limits<double>::max());
    padpin_ur.Set(numeric_limits<double>::min());
    for(auto const &pin : pins)
    {
        PAD *macro = pin->pad;
        if(!macro)
            continue;
        if(!macro->placed)
            continue;
        padpin_lb.Min(macro->center + pin->offset);
        padpin_ur.Max(macro->center + pin->offset);
    }
}

NET::~NET() {}

// ..... PIN ..... //

PIN::PIN()
{
    name = "";
    // id = 0;

    module = nullptr;
    pad = nullptr;

    offset = FPOS();
}

PIN &PIN::operator=(const PIN &pin)
{
    if (&pin != this)
    {
        name = pin.name;
        // id = pin.id;

        module = pin.module;
        pad = pin.pad;

        offset = pin.offset;
    }

    return *this;
}

PIN::~PIN() {}

// ..... PAD ..... //

PAD::PAD()
{
    name = "";
    id = 0;

    center = FPOS();
    lb = FPOS();
    ur = FPOS();

    W_H = FPOS();
    area = 0;

    pins.clear();
    obstructions.clear();
    polys.clear();
    rects.clear();

    type = "";
    category = "";
    orientation = 0;
    is_cover = false;
    fixed = false;

    // - Power
    power_type = 0; /// if it is power pad, set VDD = 1 or VSS = 0

    static_power = 0;
    voltage = 0;

    crossnodes.clear();

    // - Fence
    fence = nullptr;
}

PAD &PAD::operator=(const PAD &pad)
{
    if (&pad != this)
    {
        name = pad.name;
        id = pad.id;

        center = pad.lb;
        lb = pad.lb;
        ur = pad.ur;
        W_H = pad.W_H;
        area = pad.area;

        pins.clear();
        pins = pad.pins;
        // for (vector<PIN *>::const_iterator iter = pad.pins.begin(); iter != pad.pins.end(); iter++)
        //     pins.push_back(*iter);

        obstructions.clear();
        obstructions = pad.obstructions;
        // for (vector<BLOCKAGE *>::const_iterator iter = pad.obstructions.begin(); iter != pad.obstructions.end(); iter++)
        //     obstructions.push_back(*iter);

        polys.clear();
        polys = pad.polys;
        // for (vector<FPOS>::const_iterator iter = pad.polys.begin(); iter != pad.polys.end(); iter++)
        //     polys.push_back(*iter);

        rects.clear();
        rects = pad.rects;
        // for (vector<RECT>::const_iterator iter = pad.rects.begin(); iter != pad.rects.end(); iter++)
        //     rects.push_back(*iter);

        type = pad.type;
        category = pad.category;
        orientation = pad.orientation;
        is_cover = pad.is_cover;
        fixed = pad.fixed;

        // - Power
        power_type = pad.power_type; // if it is power pad, set VDD = 1 or VSS = 0

        static_power = pad.static_power; // if it is power pad, set VDD = 1 or VSS = 0
        voltage = pad.voltage;

        crossnodes.clear();
        crossnodes = pad.crossnodes;
        // for (vector<CROSSNODE *>::const_iterator iter = pad.crossnodes.begin(); iter != pad.crossnodes.end(); iter++)
        //     crossnodes.push_back(*iter);

        // - Fence
        fence = pad.fence;
    }

    return *this;
}

PAD::~PAD() {}

// ..... BLOCKAGE ..... //

BLOCKAGE::BLOCKAGE()
{
    id = 0;

    lb = FPOS();
    ur = FPOS();
    // W_H = FPOS();
    area = 0;

    // layer_id = -1;
    layer = nullptr;
}

BLOCKAGE &BLOCKAGE::operator=(const BLOCKAGE &blockage)
{
    if (&blockage != this)
    {
        id = blockage.id;

        lb = blockage.lb;
        ur = blockage.ur;
        // W_H = blockage.W_H;
        area = blockage.area;

        // layer_id = blockage.layer_id;
        layer = blockage.layer;
    }

    return *this;
}
BLOCKAGE::~BLOCKAGE() {}

// ..... BIN ..... //

BIN::BIN()
{
    id = 0;            // 1D vector index (i*dim_N + j)
    i_j_index = POS(); // 2D vector index (x:i , y:j)

    center = FPOS();
    lb = FPOS();
    ur = FPOS();
    W_H = FPOS();

    area = area_Inv = 0;
    cell_area = 0;
    filler_area = 0;
    pad_area = 0;
    blockage_area = 0;
    virt_blockage_area = 0;

    // - utilization
    fixed_area = avail_area = avail_area_Inv = 0;

    // - E_density model
    phi = 0;
    Efield = FPOS();
    density = 0;
    density_no_filler = 0;

    // - Power
    static_power = 0;
}

BIN &BIN::operator=(const BIN &bin)
{
    if (&bin != this)
    {
        id = bin.id;               // 1D vector index (i*dim_N + j)
        i_j_index = bin.i_j_index; // 2D vector index (x:i , y:j)

        center = bin.center;
        lb = bin.lb;
        ur = bin.ur;
        W_H = bin.W_H;

        area = bin.area;
        area_Inv = bin.area_Inv;
        cell_area = bin.cell_area;
        filler_area = bin.filler_area;
        pad_area = bin.pad_area;
        blockage_area = bin.blockage_area;
        virt_blockage_area = bin.virt_blockage_area;

        // - utilization
        fixed_area = bin.fixed_area;
        avail_area = bin.avail_area;
        avail_area_Inv = bin.avail_area_Inv;

        // - E_density model
        phi = bin.phi;
        Efield = bin.Efield;
        density = bin.density;
        density_no_filler = bin.density_no_filler;

        // - Power
        static_power = bin.static_power;
    }

    return *this;
}

BIN::~BIN() {}

// ..... LAYER ..... //

LAYER::LAYER()
{
    name = "";
    id = 0;

    direction = -1;

    // - Power
    metal_layer_id = -1; /// >= 0 為 metalLayer

    UnitNomResistance = default_layer_UnitNomResistance;
    width = default_layer_Width;
    spacing = default_layer_Spacing;
    pitch = 0;

    vdd_segment_num = 0;
    segment_num = 0;
    segment0_num = 0;

    vdd_seg_v_map.clear();
    vdd_seg_h_map.clear();
    vdd_seg_v.clear();
    vdd_seg_h.clear();

    crossnode_map.clear();
    crossnodes.clear();
}

LAYER &LAYER::operator=(const LAYER &layer)
{
    if (&layer != this)
    {
        name = layer.name;
        id = 0;

        direction = layer.direction;

        pitch = layer.pitch;

        metal_layer_id = layer.metal_layer_id;

        UnitNomResistance = layer.UnitNomResistance;
        width = default_layer_Width;
        spacing = default_layer_Spacing;

        vdd_segment_num = layer.vdd_segment_num;
        segment_num = layer.segment_num;
        segment0_num = layer.segment0_num;

        vdd_seg_v_map.clear();
        vdd_seg_v_map = layer.vdd_seg_v_map;
        vdd_seg_h_map.clear();
        vdd_seg_h_map = layer.vdd_seg_h_map;
        vdd_seg_v.clear();
        vdd_seg_v = layer.vdd_seg_v;
        vdd_seg_h.clear();
        vdd_seg_h = layer.vdd_seg_h;
        // for (vector<SEGMENT *>::const_iterator iter = layer.vdd_segemnts.begin(); iter != layer.vdd_segemnts.end(); iter++)
        //     vdd_segemnts.push_back(*iter);

        crossnode_map.clear();
        crossnode_map = layer.crossnode_map;
        crossnodes.clear();
        crossnodes = layer.crossnodes;
        // for (vector<SEGMENT *>::const_iterator iter = layer.crossnodes.begin(); iter != layer.crossnodes.end(); iter++)
        //     crossnodes.push_back(*iter);
    }

    return *this;
}

LAYER::~LAYER() {}

// ..... VIA ..... //

VIA::VIA()
{
    name = "";
    lower_name = upper_name = cut_name = "";
    // lower_name = upper_name = "";

    // LAYER* lower_layer, * upper_layer, * cut_layer;
    lower_layer = nullptr;
    upper_layer = nullptr;

    // is_rule = false;

    W_H = default_VIA_W_H; // prec W = 0, H = 0;

    unitNomResistance = default_VIA_UnitNomResistance;
    spacing = default_VIA_Spacing;

    // upper_layer_left_right = upper_layer_up_low = lower_layer_left_right = lower_layer_up_low = 0;

    upper_layer_W_H = FPOS();
    lower_layer_W_H = FPOS();
}

VIA &VIA::operator=(const VIA &via)
{
    if (&via != this)
    {
        name = via.name;
        lower_name = via.lower_name;
        upper_name = via.upper_name;
        cut_name = via.cut_name;

        lower_layer = via.lower_layer;
        upper_layer = via.upper_layer;

        // is_rule = via.is_rule;

        W_H = via.W_H;

        unitNomResistance = via.unitNomResistance;
        spacing = via.spacing;

        // upper_layer_left_right = via.upper_layer_left_right;
        // upper_layer_up_low = via.upper_layer_up_low;
        // lower_layer_left_right = via.lower_layer_left_right;
        // lower_layer_up_low = via.lower_layer_up_low;
        upper_layer_W_H = via.upper_layer_W_H;
        lower_layer_W_H = via.lower_layer_W_H;
    }

    return *this;
}

VIA::~VIA() {}

// ..... SEGMENT ..... //

SEGMENT::SEGMENT()
{
    lb = FPOS();
    ur = FPOS();
    W_H = FPOS();

    layer = nullptr;

    net_type = "stripe";
    power_type = "";

    direction = -1;
    left = nullptr;
    right = nullptr;
    powerline_tag = false;

    seg_width = 0;
}

SEGMENT &SEGMENT::operator=(const SEGMENT &segment)
{
    if (&segment != this)
    {
        lb = segment.lb;
        ur = segment.ur;
        W_H = segment.W_H;

        layer = segment.layer;

        net_type = segment.net_type;
        power_type = segment.power_type;

        direction = segment.direction;
        left = segment.left;
        right = segment.right;
        powerline_tag = segment.powerline_tag;

        seg_width = segment.seg_width;
    }

    return *this;
}

SEGMENT::~SEGMENT() {}

// ..... ROW ..... //

ROW::ROW()
{
    name = "";

    lb = FPOS();
    W_H = FPOS();

    // is_tall = false; /// 高列

    Sitespacing = 0;
    NumSites = 0;

    modules.clear();
}

ROW &ROW::operator=(const ROW &row)
{
    if (&row != this)
    {
        name = row.name;

        lb = row.lb;
        W_H = row.W_H;

        // is_tall = row.is_tall;

        Sitespacing = row.Sitespacing;
        NumSites = row.NumSites;

        modules.clear();
        modules = row.modules;
        //     for (vector<MODULE *>::const_iterator iter = row.modules.begin(); iter != row.modules.end(); iter++)
        //         modules.push_back(*iter);
    }

    return *this;
}

ROW::~ROW() {}

// ..... TILE ..... //  

TILE::TILE(PAD *macro) : macro(macro)
{ 
    if(!macro)
    {
        center = FPOS();
        lb = FPOS();
        ur = FPOS();
        
        W_H = FPOS();
        area = 0;
    }
    else
    {
        center = macro->center;
        lb = macro->lb;
        ur = macro->ur;

        W_H = macro->W_H;
        area = macro->area;
    }
    top = nullptr;
    bot = nullptr;
    left = nullptr;
    right = nullptr;
}

TILE &TILE::operator=(const TILE &tile)
{
    if(&tile != this)
    {
        macro = tile.macro;
        
        center = tile.center;
        lb = tile.lb;
        ur = tile.ur;

        W_H = tile.W_H;
        area = tile.area;

        top = tile.top;
        bot = tile.bot;
        left = tile.left;
        right = tile.right;
    }
    return *this;
}

TILE::~TILE() {}

// ..... CLUSTER ..... //

CLUSTER &CLUSTER::operator=(const CLUSTER &cluster)
{
    if (&cluster != this)
    {
        lb = cluster.lb;

        e = cluster.e;
        q = cluster.q;
        w = cluster.w;

        module_vec.clear();
        module_vec = cluster.module_vec;
        // for (vector<MODULE *>::const_iterator iter = cluster.modules.begin(); iter != cluster.modules.end(); iter++)
        //     modules.push_back(*iter);
    }

    return *this;
}

// // ..... FILLER ..... //

// FILLER::FILLER()
// {
//     center = FPOS();
//     lb = FPOS();

//     ori_W_H = FPOS();
//     ori_area = 0;

//     // - inflat
//     W_H = FPOS();
//     area = area_Inv = 0;

//     // global_inflation_ratio = 1; // ! global
//     local_inflation_ratio = 1;

//     den_scal = 1;
//     // - Gradient
//     total_grad = nullptr;
//     wl_grad = nullptr;
//     glo_d_grad = nullptr;
//     loc_d_grad = nullptr;
//     loc_d_lambda = nullptr;
// }

// FILLER &FILLER::operator=(const FILLER &filler)
// {
//     if (&filler != this)
//     {
//         center = filler.center;
//         lb = filler.lb;

//         ori_W_H = filler.ori_W_H;
//         ori_area = filler.ori_area;

//         W_H = filler.W_H;
//         area = filler.area;

//         local_inflation_ratio = filler.local_inflation_ratio;

//         den_scal = filler.den_scal;
//         // - Gradient
//         total_grad = filler.total_grad;
//         wl_grad = filler.wl_grad;
//         glo_d_grad = filler.glo_d_grad;
//         loc_d_grad = filler.loc_d_grad;
//         loc_d_lambda = filler.loc_d_lambda;
//     }
//     return *this;
// }

// FILLER::~FILLER() {}

// ..... CROSSNODE ..... //

CROSSNODE::CROSSNODE()
{
    id = 0;

    center = FPOS();
    // lb = FPOS();
    // ur = FPOS();

    top_layer = nullptr;
    bottom_layer = nullptr;

    total_conductor = 0;
    left_conductor = 0;
    right_conductor = 0;
    low_conductor = 0;
    up_conductor = 0;
    via_conductor = 0;

    voltage = 0;
    pad_current = 0;
    static_current = 0;
    violated = false;
    // distributed_V = 0;

    power_pad = false; /// 0=no connect to pads; 1=connect to power pad
    v_strap = nullptr; /// point to vertical strap segment
    h_strap = nullptr; /// point to horizontal strap segment

    /// point to adjacency node
    left_node = nullptr;
    right_node = nullptr;
    low_node = nullptr;
    up_node = nullptr;

    modules.clear();
}

CROSSNODE &CROSSNODE::operator=(const CROSSNODE &crossnode)
{
    if (&crossnode != this)
    {
        id = crossnode.id;

        center = crossnode.center;
        // lb = crossnode.lb;
        // ur = crossnode.ur;

        top_layer = crossnode.top_layer;
        bottom_layer = crossnode.bottom_layer;

        total_conductor = crossnode.total_conductor;
        left_conductor = crossnode.left_conductor;
        right_conductor = crossnode.right_conductor;
        low_conductor = crossnode.low_conductor;
        up_conductor = crossnode.up_conductor;
        via_conductor = crossnode.via_conductor;

        voltage = crossnode.voltage;
        pad_current = crossnode.pad_current;
        static_current = crossnode.static_current;
        // distributed_V = crossnode.distributed_V;
        violated = crossnode.violated;

        power_pad = crossnode.power_pad; /// 0=no connect to pads; 1=connect to power pad
        v_strap = crossnode.v_strap;     /// point to vertical strap segment
        h_strap = crossnode.h_strap;     /// point to horizontal strap segment

        /// point to adjacency node
        left_node = crossnode.left_node;
        right_node = crossnode.right_node;
        low_node = crossnode.low_node;
        up_node = crossnode.up_node;

        modules.clear();
        modules = crossnode.modules;
        // for (vector<MODULE *>::const_iterator iter = crossnode.modules.begin(); iter != crossnode.modules.end(); iter++)
        //     modules.push_back(*iter);
    }

    return *this;
}

CROSSNODE::~CROSSNODE() {}

// ..... REGION ..... //

REGION::REGION()
{
    fence = nullptr;

    center = FPOS();
    lb = FPOS();
    ur = FPOS();
    W_H = FPOS();

    area = 0;
    module_area = 0;

    modules.clear();
}

REGION &REGION::operator=(const REGION &region)
{
    if (&region != this)
    {
        fence = region.fence;

        center = region.center;
        lb = region.lb;
        ur = region.ur;
        W_H = region.W_H;

        area = region.area;
        module_area = region.module_area;

        modules.clear();
    }

    return *this;
}

REGION::~REGION() {}

// ..... FENCE ..... //

FENCE::FENCE()
{
    name = "";
    id = 0;

    // - bounding-box
    center = FPOS();
    lb = FPOS();
    ur = FPOS();
    W_H = FPOS();

    area = 0;
    module_area = 0;
    module_area_Inv = 0;
    blockage_area = 0;
    virt_blockage_area = 0;
    pad_area = 0;
    avail_area = 0;

    // - filler
    filler_size = FPOS();
    filler_area = 0;
    filler_cnt = 0;

    regions.clear();
    modules.clear();
    pads.clear();
    fillers.clear();
    instances.clear();
    BINs.clear();

    // - utilization
    // gsum_ovf_area = 0;
    local_target_density = 0;
    gsum_phi = 0;
    loc_gsum_ovfl = 0;
    glo_gsum_ovfl = 0;
    // - weighting factor
    opt_phi_cof = 0;

    // - historical
    prec last_loc_gsum_ovfl;
    bool lock = 0;
}

FENCE &FENCE::operator=(const FENCE &fence)
{
    if (&fence != this)
    {
        name = fence.name;
        id = fence.id;

        // - bounding-box
        center = fence.center;
        lb = fence.lb;
        ur = fence.ur;
        W_H = fence.W_H;

        area = fence.area;
        module_area = fence.module_area;
        module_area_Inv = fence.module_area_Inv;
        blockage_area = fence.blockage_area;
        pad_area = fence.pad_area;

        // - filler
        filler_size = fence.filler_size;
        filler_area = fence.filler_area;
        filler_cnt = fence.filler_cnt;

        regions.clear();
        regions = fence.regions;
        // for (vector<REGION *>::const_iterator iter = fence.regions.begin(); iter != fence.regions.end(); iter++)
        //     regions.push_back(*iter);

        modules.clear();
        modules = fence.modules;
        // for (vector<REGION *>::const_iterator iter = fence.modules.begin(); iter != fence.modules.end(); iter++)
        //     modules.push_back(*iter);

        pads.clear();
        pads = fence.pads;
        // for (vector<REGION *>::const_iterator iter = fence.pads.begin(); iter != fence.pads.end(); iter++)
        //     pads.push_back(*iter);

        fillers.clear();
        fillers = fence.fillers;
        // for (vector<REGION *>::const_iterator iter = fence.fillers.begin(); iter != fence.fillers.end(); iter++)
        //     fillers.push_back(*iter);

        instances.clear();
        instances = fence.instances;

        BINs.clear();
        BINs = fence.BINs;
        // for (vector<REGION *>::const_iterator iter = fence.BINs.begin(); iter != fence.BINs.end(); iter++)
        //     BINs.push_back(*iter);

        // - utilization
        // gsum_ovf_area = fence.gsum_ovf_area;
        gsum_phi = fence.gsum_phi;
        loc_gsum_ovfl = fence.loc_gsum_ovfl;
        glo_gsum_ovfl = fence.glo_gsum_ovfl;
        // - weighting factor
        opt_phi_cof = fence.opt_phi_cof;

        // - historical
        min_loc_gsum_ovfl = fence.min_loc_gsum_ovfl;
        lock = fence.lock;
        stuck = fence.stuck;
    }

    return *this;
}

FENCE::~FENCE() {}

// ----------------------------------------------------------------------------

// // - Basic
// class FPOS;

// class MODULE;
// class NET;
// class PIN;
// class PAD;
// class BLOCKAGE;

// class BIN;
// class LAYER;
// class VIA;
// class SEGMENT;
// class ROW;

// // - Filler
// class FIller;

// // - IR_drop
// class CROSSNODE;

// // - Fence
// class REGION;
// class FENCE;

// ----------------------------------------------------------------------------

// ..... default ..... //

// - Unit 單位
prec unitResistanceName = 1; // ! def lef 皆為 ohm

// - Layer
prec default_layer_UnitNomResistance = 0.01; // ! def lef 皆為 ohm
// todo : 0326 | 0.0001(MAX_IR 0.0002%) 0.001(MAX_IR 1.15%) 0.01(MAX_IR 19%) 0.1(MAX_IR 100%) 1(MAX_IR 100%)
prec default_layer_Spacing = 0.1;
prec default_layer_Width = 0.1;

// - VIA
prec default_VIA_UnitNomResistance = 1; // ! def lef 皆為 ohm
prec default_VIA_Spacing = 0.2;
FPOS default_VIA_W_H = FPOS(100., 100.);

// - VIA unit
// ? 感覺就是rail layer 的 VIA
// todo : 0711
prec default_via_RDL_UnitNomResistance = 0.0001; // ! def lef 皆為 ohm
// todo : 0326|0.0001

// - Pin
// FPOS default_Pin_W_H = FPOS(3000., 3000.); /// For plot(計算時當成一個點)
// FPOS default_Pin_W_H = FPOS(1, 1) * AMP_PARA; /// For plot(計算時當成一個點)
FPOS default_Pin_W_H = FPOS(3, 3);

// - Bin info
unsigned int default_dim_M = 512;
unsigned int default_dim_N = 512;

// - Constant
prec default_dbu_per_micron = 1000; // ! all def coor 要 / dbu_per_micron , lef 不用
unsigned int default_thread_num = 1000; // 16 40 1000

// - Power
prec default_global_op_voltage = 1.5;

// ..... Mode info ..... //

// - Benchmark
// bool benchmark_mode = 0; /// 0:DEF/LEF | 1:aux
string benchmark_dir = ""; /// benchmark folder
string benchmark = "";     /// benchmark folder + basename
string benchname = "";     /// benchmark basename
string output_dir = "";
string input_def_pl = ""; /// 指定intput def or pl
// string DATE = "";
bool read_tf = false;  /// 奇景case (PA_series)
bool read_aux = false; /// bookshelf
prec aux_scale = 1.;

// - GP mode
bool fence_mode = false; // ! 就算沒開也有 DEAFULT FENCE
bool IR_mode = true;
bool HRH_mode = false;
bool Routability_mode = false;

bool enable_filler = true;
prec stop_overflow = 0.1; // 0.1 0.11
unsigned int max_iter = 1000; // ? Replace : 6000, DREAMPlace : 1000
prec target_density = 1; // 1 0.65 0.429 0.98 // ! 整體面積利用率 (filler會填到target_density)

// - opt_phi_coef (density weight) mode
bool local_density = false;
bool adaptive_opt_phi = true;
prec max_opt_phi = 20.;
prec lr = 0.001; /// 0.01 0.001 /// learning rate from DREAMPlace

// - GP cofficient
/// from Replace(standard cell) (default : UPPER_PCOF = 1.05, LOWER_PCOF = 0.95)
prec UPPER_PCOF = 1.04; // 1.03 1.05 1.015 1.012
prec LOWER_PCOF = 0.9;  // 0.9 0.70 0.60

// -- NAG iter
unsigned int NAG_max_step = 10;

// - Initial placement
int Init_P_mode = 2;   /// -2:only_WL | -1:Skip | 0:CG iter | 1:QP | 2:random-center (NOISE) | 3:center
unsigned int Init_CG_iter = 100; /// for Init_P_mode = 0

// // - Detail Placement
// bool DP_mode = true; // ! 目前沒作用

// - Wirelength mode
bool WL_mode = 0; /// 0:WA | 1:LSE
prec Gamma = 0;
prec Gamma_Inv = 0;
prec base_wcof = 0;

// - Parallel
const volatile unsigned int thread_num = default_thread_num;
bool GPU = false;

// - Other
int random_seed = 1000;
bool plot_flag = true;
unsigned int plot_iter = 10;

// ..... Variable Declaration ..... //

// - Constant
prec dbu_per_micron = default_dbu_per_micron; // ! all def coor 要 / dbu_per_micron , lef 不用

// - Chip info
FPOS chip_lb = FPOS(PREC_MAX, PREC_MAX), chip_ur = FPOS(PREC_MIN, PREC_MIN); // @ default
FPOS chip_W_H;
prec chip_area = 0;

// - Core info
FPOS core_lb = FPOS(PREC_MAX, PREC_MAX), core_ur = FPOS(PREC_MIN, PREC_MIN); // @ default
FPOS core_W_H;
prec core_area = 0;

// - total
prec total_module_area = 0;
prec total_pad_area = 0;
prec total_blockage_area = 0;
prec total_filler_area = 0;
prec total_Instance_num = 0;
prec total_WL = 0; /// WA/LSE
prec wHPWL = 0;    /// Real HPWL

// FPOS PR90_module_W_H = FPOS(); // ! 5% - 95% MODULE H // ? useless

// int BIN_AREA;
// prec density_max;
// prec region_weight;

// FPOS std_avg_W_H; /// 5%-95% std avg W_H // in fence.cpp

// - Bin info
unsigned int dim_M = default_dim_M;  /// # bin X
unsigned int dim_N = default_dim_N;  /// # bin Y
prec dim_M_Inv = 1. / default_dim_M; // ! 一定要加1.
prec dim_N_Inv = 1. / default_dim_N;
unsigned int total_bin_cnt = dim_M * dim_N;

FPOS BIN_W_H = FPOS();
FPOS BIN_W_H_Inv = FPOS();
prec BIN_area = 0;
prec BIN_area_Inv = 0;
prec bin_max_power = 0;

// - Map
HASH_MAP<string, MODULE *> module_map;
HASH_MAP<string, PAD *> pad_map;
HASH_MAP<string, LAYER *> layer_map;
HASH_MAP<string, LAYER *> metal_layer_map;
HASH_MAP<string, VIA *> via_map;
HASH_MAP<string, FENCE *> fence_map;
HASH_MAP<string, ROW *> row_map;

// -- IR_drop
// HASH_MAP<FPOS, CROSSNODE *> crossnode_map; // todo : 0911

// -- Power Mesh // 改成vector
// extern HASH_MAP<prec, vector<SEGMENT *>> RingStrapV_VDD;
// extern HASH_MAP<prec, vector<SEGMENT *>> RingStrapH_VDD;
// HASH_MAP<prec, vector<SEGMENT *>> vdd_seg_v_map; /// RingStrapV_VDD // todo : 0911
// HASH_MAP<prec, vector<SEGMENT *>> vdd_seg_h_map; /// RingStrapH_VDD // todo : 0911

// - Object
vector<LAYER *> LAYERS;
// vector<LAYER *> routing_LAYERS;
vector<LAYER *> metal_LAYERS;

vector<VIA *> VIAS;
vector<ROW *> ROWS; /// total ROW ptr
vector<ROW *> TROWS;
vector<ROW *> SROWS; // @ default
prec TROW_H = PREC_MIN;
prec SROW_H = PREC_MAX;

// ! 內部都用HASH_SET 外部都用vector
vector<MODULE *> MODULES;
vector<MODULE *> FILLERS;
vector<MODULE *> INSTANCES; /// MODULES + FILLERS
vector<NET *> NETS;
vector<PIN *> PINS;
vector<PAD *> PADS;
vector<PAD *> VDD_PADS;
vector<BLOCKAGE *> BLOCKAGES; /// PLACEMENT BLOCKAGE
vector<BLOCKAGE *> ROUTING_BLOCKAGES;

// - Fence
// todo : Edited by EasyMoney
vector<string> GroupMember;
vector<FENCE *> FENCES;

// - IR_drop
vector<CROSSNODE *> CROSSNODES;

// -- IRsolver Variables
prec global_op_voltage = default_global_op_voltage;
prec CROSSNODE_V_Max = 0, CROSSNODE_V_Min = global_op_voltage;
prec crossnode_max_IR_drop = PREC_MIN;

// -- Node Base Solver Variables
// -power_factor 65 -IRtolerance 0.08 -error 0.000001
prec power_factor = 65;   // @ default
prec IR_tolerance = 0.08; // @ default
prec error = 0.000001;    // @ default
prec min_op_voltage = global_op_voltage * (1 - IR_tolerance);

// - Power info
prec UnitNomResistance_PDN_layer_V = default_layer_UnitNomResistance; /// PA_series
prec UnitNomResistance_PDN_layer_H = default_layer_UnitNomResistance; /// PA_series

prec num_power_macro = 0;
prec total_module_power = 0;
prec total_pad_power = 0;

prec avg_via_resistance = 0;

// - Power Mesh
VIA *via_RDL = nullptr; // ? rail layer 的 某個VIA /// PA_series

LAYER *top_layer1 = nullptr, *top_layer2 = nullptr, *rail_layer = nullptr;
// vector<SEGMENT *> VDD_SEGMENTS;
vector<SEGMENT *> InitRail_VDD; // ? useless // todo : 0911
vector<SEGMENT *> InitRing_VDD;
vector<SEGMENT *> InitStrap_VDD;

// vector<SEGMENT* > WOSndLayerStrap_VSS; // ? useless /// without second layer
// vector<SEGMENT* > WOSndLayerStrap_VDD;

// - Inflation
prec global_inflation_ratio = 0;

// - Power Penalty Model

// - VGrid info

// - Debug
prec total_static_power = 0; // ?

// - Routing track variables

// - placement
// todo : extern varieables in .cpp
// todo : creat function to intialize placement varieables
prec density_weight = 8e-5;
prec WL_0 = 0;      /// last iter
prec WL_1 = 0;      /// cur iter
prec total_ovf = 0; /// total overflow
prec total_potn = 0;
prec min_potn = 0;

// prec OBJ_0,OBJ_1;

// - Gradient info (EasyMoney)
// #define ALPHAcGP 1e-12 // ? 1e-12 on paper but 1e-6 in implement // undo : 加到argument
// #define BetacGP 1e-13  // ? 1e-13 on paper but 1e-8 in implement
// prec ALPHA = pow(10, -12); /// local density of total density
// prec BETA = pow(10, -13); /// balance betwwen local and global density
// prec ALPHA = 1e-12;
// prec BETA = 1e-13;
prec ALPHA = 1e-6;
prec BETA = 1e-8;

vector<FPOS> Total_Grad;
vector<FPOS> WL_Grad;
vector<FPOS> Glo_D_Grad;
vector<FPOS> Loc_D_Grad;
vector<prec> Loc_D_Lambda;
// ----------------------------------------------------------------------------

// ! 留言板：(大問題留這邊)

// ! 空指標 = nullptr