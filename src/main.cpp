#include "datatype.h"
#include "parser.h"
#include "cornerstitching.h"
#include "GA.h"
#include "abacus.h"
using namespace std;

int main(int argc, char *argv[])
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    PARSER PARSER_obj;
    CS CS_obj;
    GA GA_obj;
    ABACUS ABACUS_obj;

    PARSER_obj.GetFilename(argc, argv);

    cout << endl << "READ FILE" << endl;
    PARSER_obj.ReadAuxFile();
    PARSER_obj.ReadNodeFile();
    PARSER_obj.ReadNetFile();
    PARSER_obj.ReadPlFile();
    // PARSER_obj.ReadGpplFile();
    PARSER_obj.ReadSclFile();

    cout << endl << "MACRO PLACEMENT" << endl;
    GA_obj.GA_main(CS_obj);
    // CS_obj.HPWL(PADS);

    // cout << "\nLEGAL\n";
    // ABACUS_obj.Abacus();

    cout << endl << "WRITE FILE" << endl;
    PARSER_obj.WriteChipFile();
    PARSER_obj.WriteCellFile();
    PARSER_obj.WritePadFile();
    PARSER_obj.WritePadPinFile();
    PARSER_obj.WritePltFile();
    PARSER_obj.WritePloutFile();
    // PARSER_obj.WriteLegalPlFile();
    // PARSER_obj.WriteRowFile(ABACUS_obj);
    PARSER_obj.WriteMFile(CS_obj);

    cout << endl << "EXIT SUCCESS" << endl;
    return EXIT_SUCCESS;
}

// ./HW2_M16131056 ../benchmarks/adaptec1/adaptec1.aux
// ./HW2_M16131056 ../benchmarks/adaptec2/adaptec2.aux o254931
// ./HW2_M16131056 ../benchmarks/adaptec3/adaptec3.aux
// ./HW2_M16131056 ../benchmarks/adaptec4/adaptec4.aux

// gnuplot adaptec1/adaptec1.plt

// python3 tester.py --case adaptec1
// python3 tester.py --case adaptec2
// python3 tester.py --case adaptec3
// python3 tester.py --case adaptec4