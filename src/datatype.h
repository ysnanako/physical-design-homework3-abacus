////////////////////////////////////////////////////
// * brief
// * LAB       NCKU_SEDA 112
// * date      2024/7/11 update
// * version   1.0.0

// * author    PY , EasyMoney
////////////////////////////////////////////////////

#ifndef _DATATYPE_h_
#define _DATATYPE_h_

// ----------------------------------------------------------------------------

// ..... librarys ..... //
// - basic
#include <iostream>
#include <iomanip>
#include <sstream> /// For std::istringstream
#include <fstream>
// #include <cstdio> /// printf, fopen
#include <cstdlib> /// malloc, free
#include <algorithm>
#include <random>

// - data structure
#include <vector>
#include <cstring> /// for char array
#include <string>
// #include <map> /// unordered_map instead
// #include <set> /// unordered_set instead
#include <list> /// for POLY
// #include <iterator> /// for POLY

// - debug
// #include <cassert> /// assert
// #define NDEBUG
#include <exception>

// - Mathematics
#include <cmath>
#include <cfloat> /// set float precision
#include <climits>

// - multi thread
// #include <thread>
// #include <atomic>
#include <omp.h> /// replace thread // ! 只能用 index base
// #include <time.h>
#include <ctime>
#include <chrono> /// replace time.h

// 高精度計時 (ms)
// auto start = std::chrono::high_resolution_clock::now();
// auto end = std::chrono::high_resolution_clock::now();
// std::chrono::duration<double> duration;
// std::cout << duration.count() << " seconds" << std::endl;


/// exp double cal
// // #include <gmp.h>
// #include "gmp.h"    // C
// // #include "gmpxx.h"  // C++

// // #include "mpfr.h"

// ..... define ..... //

// - map, set
// #define USE_GOOGLE_HASH

#ifdef USE_GOOGLE_HASH
#include <google/dense_hash_map> /// better
#include <google/dense_hash_set>
#else
#include <unordered_map>
#include <unordered_set>
#endif

// ! 都用 HASH_MAP, HASH_SET
#ifdef USE_GOOGLE_HASH
#define HASH_MAP google::dense_hash_map
#define HASH_SET google::dense_hash_set
#else
#define HASH_MAP std::unordered_map
#define HASH_SET std::unordered_set
#endif

// - constant
#define PI 3.141592653589793238462L
#define SQRT2 1.414213562373095048801L

/// for CG
#define Epsilon 1.0E-15
#define MIN_LEN 25.0 /* 10.0 */ /* 5.0 */ /* 1.0 */

#define AMP_PARA 1000.0 // ! NCKU : 與chip相同大小的坐標系 (現在AMP_PARA只用在scale算線長)
// #define AMP_PARA 1.0 // ! replace : 比chip小了1000倍的坐標系

// - float precision
#define IS_FLOAT 0
#define IS_DOUBLE 1

#define PREC_MODE IS_FLOAT
#if PREC_MODE == IS_FLOAT
typedef float prec;
#define PREC_MAX FLT_MAX
#define PREC_MIN FLT_MIN
#define PREC_EPSILON numeric_limits<float>::epsilon()

#elif PREC_MODE == IS_DOUBLE
typedef double prec;
#define PREC_MAX DBL_MAX
#define PREC_MIN DBL_MIN
#define PREC_EPSILON numeric_limits<double>::epsilon()
#endif

#define INT_CONVERT(a) (int)(1.0 * (a) + 0.5f) /// 四捨五入
#define INT_DOWN(a) (int)(a)
#define INT_UP(a) (int)(a) + 1
#define UNSIGNED_CONVERT(a) (unsigned)(1.0 * (a) + 0.5f)

#define IS_PRECISION_EQUAL(a, b) (fabs((a) - (b)) <= PREC_EPSILON)

// ..... cout color ..... // by yychang
#define RESET "\033[0m"
#define BLACK "\033[30;47m"   // Black /
#define RED "\033[31;47m"     // Red /
#define GREEN "\033[32;47m"   // Green /
#define YELLOW "\033[33;47m"  // Yellow /
#define BLUE "\033[34;47m"    // Blue /
#define MAGENTA "\033[35;47m" // Magenta /
#define CYAN "\033[36;47m"    // Cyan /
#define WHITE "\033[37;47m"   // White /

using namespace std;

// ----------------------------------------------------------------------------

// ..... Data Structure Overveiw ..... //
// - Basic
struct POS;
struct FPOS;
struct RECT;

class MODULE;
class NET;
class PIN;
class PAD;
class BLOCKAGE;

class BIN;
class LAYER;
class VIA;
class SEGMENT;
class ROW;
// nanako
class TILE;
class SROW;
class CLUSTER;

// - Filler
// class FIller;

// - IR_drop
class CROSSNODE;

// - Fence
class REGION;
class FENCE;

// ? useless ?
// class ROUTING_EDGE;
// class VGRID;

// class NET_BOUNDING_BOX

// ----------------------------------------------------------------------------

// ..... default ..... //

// - Unit 單位
extern prec unitResistanceName; // ! def lef 皆為 ohm

// - Layer
extern prec default_layer_UnitNomResistance; // ! def lef 皆為 ohm
// todo : 0326 | 0.0001(MAX_IR 0.0002%) 0.001(MAX_IR 1.15%) 0.01(MAX_IR 19%) 0.1(MAX_IR 100%) 1(MAX_IR 100%)
extern prec default_layer_Spacing;
extern prec default_layer_Width;

// - VIA
extern prec default_VIA_UnitNomResistance; // ! def lef 皆為 ohm
extern prec default_VIA_Spacing;
extern FPOS default_VIA_W_H;

// - VIA unit
// ? 感覺就是rail layer 的 VIA
// todo : 0711
extern prec default_via_RDL_UnitNomResistance; // ! def lef 皆為 ohm
// todo : 0326|0.0001

// - Pin
extern FPOS default_Pin_W_H;

// - Bin info
extern unsigned int default_dim_M;
extern unsigned int default_dim_N;

// - Constant
extern prec default_dbu_per_micron; // ! all def coor 要 / dbu_per_micron , lef 不用
extern unsigned int default_thread_num;

// - Power
extern prec default_global_op_voltage;

// ..... Mode info ..... //

// - Benchmark
// extern bool benchmark_mode; /// 0:DEF/LEF | 1:aux
extern string benchmark_dir;
extern string benchmark;
extern string benchname; /// benchmark basename
extern string output_dir;
extern string input_def_pl; /// 指定intput def or pl
// extern string DATE;
extern bool read_tf;  /// 奇景case (PA_series)
extern bool read_aux; /// bookshelf // ! no FENCE or IR_drop
extern prec aux_scale;

// - GP mode
extern bool fence_mode; // ! 就算沒開也有DEAFULT_FENCE
extern bool IR_mode;
extern bool HRH_mode;
extern bool Routability_mode;

extern bool enable_filler;
extern prec stop_overflow;
extern unsigned int max_iter;
extern prec target_density; // ! 整體面積利用率 (filler會填到target_density)

// - opt_phi_coef (density weight) mode
extern bool local_density;
extern bool adaptive_opt_phi;
extern prec max_opt_phi;
extern prec lr;

// - GP cofficient
/// from Replace(standard cell) (default : UPPER_PCOF = 1.05, LOWER_PCOF = 0.95)
extern prec UPPER_PCOF; // 1.015, 1.012
extern prec LOWER_PCOF; // 0.70, 0.60

// -- NAG iter
extern unsigned int NAG_max_step;

// - Initial placement
extern int Init_P_mode;  /// -2:only_WL | -1:Skip | 0:CG iter | 1:QP | 2:random-center (NOISE) | 3:center
extern unsigned Init_CG_iter; /// for Init_P_mode = 0

// // - Detail Placement
// extern bool DP_mode;

// - Wirelength mode
#define mode_WA 0
#define mode_LSE 1
extern bool WL_mode; /// 0:WA | 1:LSE
extern prec Gamma;
extern prec Gamma_Inv;
extern prec base_wcof;

// - Parallel
extern const volatile unsigned int thread_num;
extern bool GPU;

// - Other
extern int random_seed;
extern bool plot_flag;
extern unsigned int plot_iter;

// ..... Variable Declaration ..... //

// - Constant
extern prec dbu_per_micron; // ! all def coor 要 / dbu_per_micron , lef 不用

// - Chip info
extern FPOS chip_lb, chip_ur;
extern FPOS chip_W_H;
extern prec chip_area;

// - Core info
extern FPOS core_lb, core_ur;
extern FPOS core_W_H;
extern prec core_area;

// - Floorplan

// - total
extern prec total_module_area;
extern prec total_pad_area;
extern prec total_blockage_area;
extern prec total_filler_area;
extern prec total_Instance_num;
extern prec total_WL; /// WA/LSE WL
extern prec wHPWL;    /// Real HPWL

// extern FPOS PR90_module_W_H; // ? useless

// extern int BIN_AREA;
// extern prec density_max;
// extern prec region_weight;

// ! handled in fence_init() (EasyMoney)
// extern FPOS std_avg_W_H; /// 5%-95% std avg W_H

// - Bin info
extern unsigned int dim_M;
extern unsigned int dim_N;
extern prec dim_M_Inv;
extern prec dim_N_Inv;
extern unsigned int total_bin_cnt;

extern FPOS BIN_W_H;
extern FPOS BIN_W_H_Inv;
extern prec BIN_area;
extern prec BIN_area_Inv;
extern prec bin_max_power;

// prec global_macro_area_scale; //- idk the funcionality yet(EasyMoney)
// extern unsigned int bin_cnt_per_fense;

// - Map
extern HASH_MAP<string, MODULE *> module_map;
extern HASH_MAP<string, PAD *> pad_map;
extern HASH_MAP<string, LAYER *> layer_map;
extern HASH_MAP<string, LAYER *> metal_layer_map;
extern HASH_MAP<string, VIA *> via_map;
extern HASH_MAP<string, FENCE *> fence_map;
extern HASH_MAP<string, ROW *> row_map;

// -- IR_drop
// extern HASH_MAP<FPOS, CROSSNODE *> crossnode_map; // todo : 0911

// -- Power Mesh
// extern HASH_MAP<prec, vector<SEGMENT *>> RingStrapV_VDD;
// extern HASH_MAP<prec, vector<SEGMENT *>> RingStrapH_VDD;
// extern HASH_MAP<prec, vector<SEGMENT *>> vdd_seg_v_map; /// RingStrapV_VDD // todo : 0911
// extern HASH_MAP<prec, vector<SEGMENT *>> vdd_seg_h_map; /// RingStrapH_VDD

// - Object
extern vector<LAYER *> LAYERS;
// vector<LAYER *> routing_LAYERS;
extern vector<LAYER *> metal_LAYERS;

extern vector<VIA *> VIAS;
extern vector<ROW *> ROWS;
extern vector<ROW *> TROWS;
extern vector<ROW *> SROWS;
extern prec TROW_H;
extern prec SROW_H;

// ! 內部都用HASH_SET 外部都用vector
extern vector<MODULE *> MODULES;
extern vector<MODULE *> FILLERS;
extern vector<MODULE *> INSTANCES;
extern vector<NET *> NETS;
extern vector<PIN *> PINS;
extern vector<PAD *> PADS;
extern vector<PAD *> VDD_PADS;
extern vector<BLOCKAGE *> BLOCKAGES;
extern vector<BLOCKAGE *> ROUTING_BLOCKAGES;

// - Fence
// todo : Edited by EasyMoney
extern vector<string> GroupMember;
extern vector<FENCE *> FENCES;

// - IR_drop
extern vector<CROSSNODE *> CROSSNODES;

// -- IRsolver Variables
extern prec global_op_voltage;
extern prec CROSSNODE_V_Max;
extern prec CROSSNODE_V_Min;
extern prec crossnode_max_IR_drop;

// -- Node Base Solver Variables
// -power_factor 65 -IR_tolerance 0.08 -error 0.000001
extern prec power_factor;   // @ default
extern prec IR_tolerance;   // @ default
extern prec error;          // @ default
extern prec min_op_voltage; // @ default

// - Power info
extern prec max_static_power;

extern prec UnitNomResistance_PDN_layer_V;
extern prec UnitNomResistance_PDN_layer_H;

extern prec num_power_macro;
extern prec total_module_power;
extern prec total_pad_power;

extern prec avg_via_resistance;

// - Power Mesh
extern VIA *via_RDL; // ? rail layer 的 某個VIA

extern LAYER *top_layer1, *top_layer2, *rail_layer;
// vector<SEGMENT *> VDD_SEGMENTS;
extern vector<SEGMENT *> InitRail_VDD;  // ? useless // todo : 0911
extern vector<SEGMENT *> InitRing_VDD;  // ? useless
extern vector<SEGMENT *> InitStrap_VDD; // ? useless

// extern vector<SEGMENT* > WOSndLayerStrap_VSS; // ? useless /// without second layer
// extern vector<SEGMENT* > WOSndLayerStrap_VDD;

// - Inflation
extern prec global_inflation_ratio;

// - Power Penalty Model

// - VGrid info

// - Debug
extern prec total_static_power; // ?

// - Routing track variables

// - Placement (EasyMoney)
// todo : extern varieables in .cpp
// todo : creat function to intialize placement varieables
// extern prec lamda;
// #define density_weight 8e-5
extern prec density_weight;
// #define HPWL_ref 3.5e5
#define HPWL_ref 3.46e5
extern prec WL_0; // last iter
extern prec WL_1; // current iter
extern prec total_ovf;
extern prec total_potn;
extern prec min_potn;

// - Gradient info (EasyMoney)
// #define ALPHAcGP 1e-12 // ? 1e-12 on paper but 1e-6 in implement // undo : 加到argument
// #define BetacGP 1e-13  // ? 1e-13 on paper but 1e-8 in implement
#define ALPHA_max 1e5
extern prec ALPHA;
extern prec BETA;
// cannot be pointer due to NAG function
extern vector<FPOS> Total_Grad;
extern vector<FPOS> WL_Grad;
extern vector<FPOS> Glo_D_Grad;
extern vector<FPOS> Loc_D_Grad;
extern vector<prec> Loc_D_Lambda;

// ----------------------------------------------------------------------------

struct FPOS
{
public:
    prec x = 0;
    prec y = 0;

    FPOS() : x(0), y(0) {}
    FPOS(prec xloc, prec yloc) : x(xloc), y(yloc) {}
    FPOS(double xloc, double yloc) : x(xloc), y(yloc) {}
    FPOS(int xloc, int yloc) : x(xloc), y(yloc) {}
    // FPOS(POS lb) : x(lb.x), y(lb.y) {}

    FPOS &operator=(const FPOS &f)
    {
        if (this != &f)
        {
            this->x = f.x;
            this->y = f.y;
        }
        return *this;
    }

    FPOS operator+(const FPOS &f) const
    {
        return FPOS(this->x + f.x, this->y + f.y);
    }

    FPOS operator-(const FPOS &f) const
    {
        return FPOS(this->x - f.x, this->y - f.y);
    }

    FPOS operator*(const FPOS &f) const
    {
        return FPOS(this->x * f.x, this->y * f.y);
    }

    FPOS operator*(prec a) const
    {
        return FPOS(this->x * a, this->y * a);
    }

    FPOS operator/(prec a) const
    {
        return FPOS(this->x / a, this->y / a);
    }

    FPOS operator+=(FPOS a) const
    {
        return FPOS(this->x + a.x, this->y + a.y);
    }

    bool operator==(const FPOS &f) const
    {
        // return this->x == f.x && this->y == f.y;
        // return fabs(this->x - f.x) < Epsilon && fabs(this->y - f.y) < Epsilon;
        // return (int)this->x*1000 - (int)f.x*1000 == 0 && (int)this->y*1000 - (int)f.y*1000 == 0;
        return abs(this->x - f.x) < 0.01 && abs(this->y - f.y) < 0.01;
        // return IS_PRECISION_EQUAL(this->x, f.x) && IS_PRECISION_EQUAL(this->y, f.y);
        // return (double)this->x - (double)f.x == 0 && (double)this->y - (double)f.y == 0;
    }

    bool operator!=(const FPOS &f) const
    {
        // return this->x == f.x && this->y == f.y;
        // return !(fabs(this->x - f.x) < Epsilon && fabs(this->y - f.y) < Epsilon);
        // return (int)this->x*1000 - (int)f.x*1000 != 0 || (int)this->y*1000 - (int)f.y*1000 != 0;
        return abs(this->x - f.x) > 0.01 || abs(this->y - f.y) > 0.01;
        // return !(IS_PRECISION_EQUAL(this->x, f.x) && IS_PRECISION_EQUAL(this->y, f.y));
        // return (double)this->x - (double)f.x == 0 && (double)this->y - (double)f.y == 0;
    }

    friend std::ostream &operator<<(std::ostream &os, const FPOS &p)
    {
        os << fixed << setprecision(3) << " (" << p.x << ", " << p.y << ") ";
        return os;
    }

    // bool operator<(const FPOS &f) const { /// for POLY
    //     return y < f.y || (y == f.y && x < f.x);
    // }

    ~FPOS();

    void Set(prec a);
    // void Set(FPOS a);
    void Set(prec xloc, prec yloc);
    void SetZero();
    // void Add(FPOS a);
    // void SetAdd(FPOS a, FPOS b);

    void Min(FPOS a);
    void SetMin(FPOS a, FPOS b);
    void Max(FPOS a);
    void SetMax(FPOS a, FPOS b);
    prec GetProduct();

    void Center(FPOS lb, FPOS W_H);
    void ur_Center(FPOS lb, FPOS ur);
    void update_lb(FPOS center, FPOS W_H);
    void update_ur(FPOS lb, FPOS W_H);
    void update_W_H(FPOS lb, FPOS ur);

    prec Cal_area(FPOS ur); /// for lb
    prec Cal_area(); /// for W_H

    // void Dump();
    // void Dump(std::string a);

    void Swap();
};

struct POS
{
    int x = 0;
    int y = 0;

    POS() : x(0), y(0) {}
    POS(prec xloc, prec yloc) : x(xloc), y(yloc) {}
    POS(double xloc, double yloc) : x(xloc), y(yloc) {}
    POS(int xloc, int yloc) : x(xloc), y(yloc) {}
    POS(FPOS lb) : x(lb.x), y(lb.y) {}

    int GetProduct();
    void SetZero();
    POS &operator=(const POS &p)
    {
        if (this != &p)
        {
            this->x = p.x;
            this->y = p.y;
        }
        return *this;
    }

    POS operator+(const POS &p) const
    {
        return POS(this->x + p.x, this->y + p.y);
    }

    POS operator-(const POS &p) const
    {
        return POS(this->x - p.x, this->y - p.y);
    }

    POS operator*(const POS &p) const
    {
        return POS(this->x * p.x, this->y * p.y);
    }
    POS operator*(int a) const
    {
        return POS(this->x * a, this->y * a);
    }

    bool operator==(const POS &p) const
    {
        return this->x == p.x && this->y == p.y;
    }

    friend std::ostream &operator<<(std::ostream &os, const POS &p)
    {
        os << " (" << p.x << ", " << p.y << ") ";
        return os;
    }
};

// * for crossnode_map
namespace std
{
    template <>
    struct hash<FPOS>
    {
        size_t operator()(const FPOS &fpos) const
        {
            return hash<prec>()(fpos.x) ^ (hash<prec>()(fpos.y) << 1);
        }
    };
}


class RECT /// 內部方形才用
{
public:
    RECT() : lb(FPOS()), ur(FPOS()) {}
    RECT(FPOS lb, FPOS ur) : lb(lb), ur(ur) {}
    // RECT(POS lb, POS ur) : lb(lb), ur(ur) {}

    RECT &operator=(const RECT &r)
    {
        if (this != &r)
        {
            this->lb = r.lb;
            this->ur = r.ur;
        }
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const RECT &p)
    {
        os << " <" << p.lb << ", " << p.ur << "> ";
        return os;
    }

    ~RECT();

    FPOS lb, ur;
};

// ..... Data Structure ..... //
//

class MODULE /// unfixed, movable
{
public:
    MODULE();
    MODULE &operator=(const MODULE &);
    ~MODULE();

    string name = "";
    unsigned int id = 0;

    FPOS center, lb, ur; /// left-bottom corner
    FPOS best_center;

    FPOS ori_W_H; // prec ori_W = 0, ori_H = 0;
    prec ori_area = 0;

    // - inflat
    FPOS W_H; // prec W = 0, H = 0;
    prec area = 0;
    prec area_Inv = 0;
    // prec global_inflation_ratio = 0; // ! global
    prec local_inflation_ratio = 0;

    int is_std = true; /// 1:standard cell | 0:macro | -1:filler
    bool fixed = false;

    // - smoothing
    // todo : INitial den_scal before GP
    prec den_scal = 1;

    // - connection
    vector<NET *> nets;
    vector<PIN *> pins;
    // vector<FPOS> polys; /// 多邊形

    // - Power
    prec static_power = 0, voltage = 0;
    CROSSNODE *crossnode = nullptr; /// 只掛一個crossnode

    // - Fence
    FENCE *fence = nullptr;
    REGION *region = nullptr;

    // ? useless ?
    string type = ""; // module type (MACRO XXX in .lef)
    string category = "";
    int orientation = 0; /// 0(N), 1(W), 2(S), 3(E), 4(FN), 5(FW), 6(FS), 7(FE)

    // set< unsigned int > nets;   // * ori nets index
    // vector< unsigned int > pins;    // X ori pins index
    // vector< unsigned int > net_bounding_boxes; // * net_bounding_boxes index
    // vector< unsigned int > con_to_fences; //number of nets connect to fences

    FPOS *total_grad, *wl_grad, *glo_d_grad, *loc_d_grad;
    prec *loc_d_lambda;

    unsigned int diverg_aware_precond_cof = 1;
};

class NET
{
public:
    NET();
    NET &operator=(const NET &);
    ~NET();

    string name = "";
    unsigned int id = 0;

    // prec x, y;
    prec net_weight = 1;

    vector<PIN *> pins;
    vector<MODULE *> modules;
    vector<PAD *> pads;
    FPOS pin_lb, pin_ur;

    // nanako
    prec padHPWL();
    void updatePadPinPos();
    FPOS padpin_lb, padpin_ur;
     
    // - wirelength
    prec WL;
    FPOS ExpSum_P, ExpSum_N;         /// < Sum(EXP(X)),Sum(EXP(Y)) > &  < Sum(EXP(-X)),Sum(EXP(-Y)) >
    FPOS ExpSum_P_Inv, ExpSum_N_Inv; /// < 1/Sum(EXP(X)),1/Sum(EXP(Y)) > &  < 1/Sum(EXP(-X)),1/Sum(EXP(-Y)) >
    FPOS WA_ExpSum_P, WA_ExpSum_N;   /// < X*Sum(EXP(X)),Y*Sum(EXP(Y)) > &  < -X*Sum(EXP(-X)),-Y*Sum(EXP(-Y)) >
};

class PIN
{
public:
    PIN();
    PIN &operator=(const PIN &);
    ~PIN();

    string name = "";
    // unsigned int id = 0; // ! useless

    MODULE *module = nullptr;
    PAD *pad = nullptr;

    // prec x, y;

    FPOS offset;
    // FPOS coor; // ? 需要嗎? 因為之後還要更新 (PY)
};

class PAD /// pre-placed macro & pad & cover
{
public:
    PAD();
    PAD &operator=(const PAD &);
    ~PAD();

    string name = "";
    unsigned int id = 0;

    FPOS center, lb, ur; /// out-bound
    FPOS W_H;            /// prec ori_W = 0, ori_H = 0;
    prec area = 0;

    // nanako
    bool placed = false;
    vector<NET *> nets;

    vector<PIN *> pins;
    vector<BLOCKAGE *> obstructions; /// default:MOS tripe | 多邊形:多個矩形
    /// 多邊形
    vector<FPOS> polys; /// coor
    vector<RECT> rects; /// 矩形

    // ? useless ?
    string type = ""; // pad type (MACRO XXX in .lef)
    string category = "";
    int orientation = 0;   /// 0(N), 1(W), 2(S), 3(E), 4(FN), 5(FW), 6(FS), 7(FE)
    bool is_cover = false; /// (terminal NI)
    bool fixed = false;    /// (FIXED) nanako

    // - Power
    // bool is_power_pad = 0;
    // int power_type;				// if it is power pad, set VDD = 1 or VSS = 0
    int power_type = -1; /// power pad : 1(VDD), 0(VSS) | default : -1

    prec static_power = 0, voltage = 0;
    // CROSSNODE* crossnode;
    // vector< vector< int > > crossnode_ids;
    vector<CROSSNODE *> crossnodes; /// MACRO : all covered crossnodes, std_cell : single crossnode:cell

    // - Fence
    FENCE *fence = nullptr;
};

class BLOCKAGE
{
public:
    BLOCKAGE();
    BLOCKAGE &operator=(const BLOCKAGE &);
    ~BLOCKAGE();

    unsigned int id = 0;

    FPOS lb, ur; /// BLOCKAGE : lb,ur | PAD.obstructions : offset
    // FPOS W_H; // prec W = 0, H = 0;
    prec area = 0;

    LAYER *layer = nullptr; /// blockage layer (0: placement blockage; 1~: routing blockage)
    // int layer_id = -1;
};

// @ mainly edited by EasyMoney
class BIN
{
public:
    BIN();
    BIN &operator=(const BIN &);
    ~BIN();
    unsigned int id = 0; // 1D vector index (i*dim_N + j)
    POS i_j_index;       // 2D vector index (x:i, y:j)

    FPOS center, lb, ur;
    FPOS W_H; // prec W = 0, H = 0;

    prec area = 0, area_Inv = 0;
    prec cell_area = 0;
    prec filler_area = 0;
    prec pad_area = 0;
    prec blockage_area = 0;
    prec virt_blockage_area = 0;

    prec fixed_area = 0;
    prec utilized_area = 0;
    prec utilized_area_no_filler = 0;

    // - utilization
    prec avail_area = 0, avail_area_Inv = 0;

    // - E_density model
    prec phi = 0; /// 電位
    FPOS Efield;  /// 電場
    prec density = 0;
    prec density_no_filler = 0;

    // - Power
    prec static_power = 0;
};

class LAYER
{
public:
    LAYER();
    LAYER &operator=(const LAYER &);
    ~LAYER();

    string name = "";
    unsigned int id = 0;

    // string direction; /// routing layer default direction (VERTICAL / HORIZONTAL) (不一定只能單一方向)
    int direction = -1; /// default -1 | 0:VERTICAL | 1:HORIZONTAL

    // - Power
    int metal_layer_id = -1; /// >= 0 為 metalLayer

    prec UnitNomResistance = default_layer_UnitNomResistance;
    prec width = default_layer_Width;
    prec spacing = default_layer_Spacing;
    prec pitch = 0; // * only use pitch for congestion map

    int vdd_segment_num = 0, segment_num = 0;
    int segment0_num = 0;

    HASH_MAP<prec, vector<SEGMENT *>> vdd_seg_v_map; /// RingStrapV_VDD
    HASH_MAP<prec, vector<SEGMENT *>> vdd_seg_h_map; /// RingStrapH_VDD
    vector<SEGMENT *> vdd_seg_v;
    vector<SEGMENT *> vdd_seg_h;

    HASH_MAP<FPOS, CROSSNODE *> crossnode_map;
    vector<CROSSNODE *> crossnodes;
};

class VIA
{
public:
    VIA();
    VIA &operator=(const VIA &);
    ~VIA();

    string name = "";
    string lower_name = "", upper_name = "", cut_name = ""; // * parser 查找方便還是需要
    /// 在 LEFViaCB, DEFViaCB 中只會先存 name
    /// 之後在 CheckVia 才給 *layer

    // LAYER* lower_layer, * upper_layer, * cut_layer;
    LAYER *lower_layer, *upper_layer;

    // bool is_rule = 0; /// VIARULE

    FPOS W_H; // prec W = 0, H = 0;

    prec unitNomResistance = default_VIA_UnitNomResistance;
    prec spacing = default_VIA_Spacing;

    // prec upper_layer_left_right = 0, upper_layer_up_low = 0, lower_layer_left_right = 0, lower_layer_up_low = 0;
    /// VIA 上下金屬層的長寬
    /// upper_layer_left_right：上層W
    /// upper_layer_up_low：上層H
    /// lower_layer_left_right：下層W
    /// lower_layer_up_low：下層H

    FPOS upper_layer_W_H;
    FPOS lower_layer_W_H;
};

class SEGMENT
{
public:
    SEGMENT();
    SEGMENT &operator=(const SEGMENT &);
    ~SEGMENT();

    // double ori_x, ori_y, end_x, end_y;
    // double width, length;
    FPOS lb, ur;
    FPOS W_H; // prec W = 0, H = 0;  // ? SEGMENT 厚度

    LAYER *layer = nullptr;
    // string layer;				// which metal layer this segment use
    // LAYER* layer_ptr;

    string net_type = "stripe";   /// ring/stripe (default)/rail
    string power_type = ""; // VDD/VSS/etc...

    int direction = -1; /// default -1 | 1:VERTICAL | 0:HORIZONTAL
    SEGMENT *left = nullptr, *right = nullptr;
    bool powerline_tag = false;

    prec seg_width = 0; // todo : + default
};

class ROW
{
public:
    ROW();
    ROW &operator=(const ROW &);
    ~ROW();

    string name = "";

    FPOS lb;
    FPOS W_H;

    // bool is_tall = false; /// 高列

    prec Sitespacing = 0; /// DO (X) BY Y
    prec NumSites = 0;    /// STEP (X_num) Y_num

    vector<MODULE *> modules;
};

// cornerstitching(nanako) 

class TILE 
{
public:
    TILE(PAD *);
    TILE &operator=(const TILE &);
    ~TILE();

    PAD *macro = nullptr;

    FPOS center, lb, ur;
    FPOS W_H;
    prec area = 0;

    TILE *top, *bot, *left, *right;
};

// abacus(nanako)

class SROW 
{
public:
    FPOS lb;
    FPOS ur;
    FPOS W_H;

    vector<CLUSTER *> cluster_vec;
};

class CLUSTER
{
public:
    CLUSTER &operator=(const CLUSTER &);
    
    FPOS lb;

    prec e = 0; // weight
    prec q = 0; // function
    prec w = 0; // width

    vector<MODULE *> module_vec;
};

// ----------------------------------------------------------------------------

// class FILLER
// {
// public:
//     FILLER();
//     FILLER &operator=(const FILLER &);
//     ~FILLER();

//     FPOS center, lb, ur; /// left-bottom corner

//     FPOS ori_W_H; // prec ori_W = 0, ori_H = 0;
//     prec ori_area = 0;

//     // - inflat
//     FPOS W_H; // prec W = 0, H = 0;
//     prec area = 0;
//     prec area_Inv = 0;
//     // prec global_inflation_ratio = 1; // ! global
//     prec local_inflation_ratio = 1;

//     prec den_scal = 1;
//     // - Gradient
//     FPOS *total_grad, *wl_grad, *glo_d_grad, *loc_d_grad;
//     prec *loc_d_lambda;
// };

// ----------------------------------------------------------------------------

class CROSSNODE
{
public:
    CROSSNODE();
    CROSSNODE &operator=(const CROSSNODE &);
    ~CROSSNODE();

    int id = 0;

    // FPOS center, lb, ur;
    FPOS center;
    LAYER *top_layer = nullptr, *bottom_layer = nullptr;

    // double left_x, right_x, up_y, low_y;
    // int neighbor_num;
    // double center_x, center_y;

    prec total_conductor = 0, left_conductor = 0, right_conductor = 0, low_conductor = 0, up_conductor = 0, via_conductor = 0;
    prec voltage = 0;
    prec pad_current = 0, static_current = 0; /// static_current = std_current + pad_currernt (只需要更新std_current)
    // prec distributed_V = 0;
    // double total_drop, total_I, Accumulate_DynamicI, total_drop_con, total_drop_nor;
    // double cost;  //cost for RW
    bool violated = false;
    bool power_pad = false;      /// 0=no connect to pads; 1=connect to power pad
    SEGMENT *v_strap = nullptr;  /// point to vertical strap segment
    SEGMENT *h_strap = nullptr;  /// point to horizontal strap segment
    CROSSNODE *left_node = nullptr, *right_node = nullptr, *low_node = nullptr, *up_node = nullptr; /// point to adjacency node
    // bool covered;

    // todo : 0325
    // int top_metal_layer_index = -1;
    // int bottom_metal_layer_index = -1;

    // Add for voltage violation map
    //  bool pseudo;

    // double Pg;

    // double Qg;
    // int history_factor;

    // Add for power aware inflation
    //  vector<MODULE*> node_modules;
    //  double max_module_power_in_node;
    //  double base_inflate_ratio;
    //  bool inflation_tag;

    vector<MODULE *> modules;
};

class REGION
{
public:
    REGION();
    REGION &operator=(const REGION &);
    ~REGION();

    FENCE *fence;

    FPOS center, lb, ur;
    FPOS W_H; // prec W = 0, H = 0;

    prec area = 0;
    prec module_area = 0;

    vector<MODULE *> modules;
};

// @ mainly edited by EasyMoney
class FENCE
{
public:
    FENCE();
    FENCE &operator=(const FENCE &);
    ~FENCE();

    string name = "";
    unsigned int id = 0;

    // - bounding-box
    FPOS center, lb, ur;
    FPOS W_H; // prec W = 0, H = 0;

    prec area = 0; /// total reogion area
    prec module_area = 0;
    prec module_area_Inv = 0;
    prec blockage_area = 0;
    prec virt_blockage_area = 0;
    prec pad_area = 0;
    prec avail_area = 0;

    // - filler
    FPOS filler_size;
    prec filler_area = 0;
    unsigned int filler_cnt = 0;

    // int layer_id; // ! useless
    vector<REGION *> regions;
    vector<MODULE *> modules;
    vector<PAD *> pads; // ! 但pad不一定在上面
    // vector<BLOCKAGE *> virt_blockage;
    vector<MODULE *> fillers;
    vector<MODULE *> instances;
    vector<BIN> BINs; // 1D vector for CUDA parelellization
                      // index : i*dim_M + j

    // - utilization
    prec local_target_density = 0;
    prec gsum_phi = 0;
    prec loc_gsum_ovfl = 0;
    prec glo_gsum_ovfl = 0;

    // - weighting factor
    // ! maybe modify opt_phi_cof in my research (EasyMoney)
    prec opt_phi_cof = 0; // lambda in papers

    // - historical
    prec min_loc_gsum_ovfl;
    prec min_gsum_phi ;

    // - convergency
    bool lock = 0;
    unsigned int stuck = 0;
};

#endif

// ----------------------------------------------------------------------------
// ! 留言板：(大問題留這邊)

// ? todo : CUDA , not sure where to apply it, another file? (EasyMoney)
// * 我覺得可以獨立出來 在其他有平行化計算的地方在宣告 PY

// ? 梯度全部獨立出來，不要寫在資料結構裡對吧? (PY)
// * Yeah (Easymoney)