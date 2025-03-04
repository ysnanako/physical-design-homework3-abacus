#include "datatype.h"
#include "parser.h"
#include "cornerstitching.h"
#include "abacus.h"
using namespace std;

int main(int argc, char *argv[])
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    PARSER PARSER_obj;
    CS CS_obj;
    ABACUS ABACUS_obj;

    PARSER_obj.GetFilename(argc, argv);

    cout << "\nREAD\n";
    PARSER_obj.ReadAuxFile();
    PARSER_obj.ReadNodeFile();
    PARSER_obj.ReadNetFile();
    // PARSER_obj.readPlFile();
    PARSER_obj.ReadGpplFile();
    PARSER_obj.ReadSclFile();

    // CS_obj.MacroPlacement();

    cout << "\nABACUS\n";
    // ABACUS_obj.Abacus();

    cout << "\nWRITE\n";
    PARSER_obj.WriteChipFile();
    PARSER_obj.WriteCellFile();
    PARSER_obj.WritePadFile();
    PARSER_obj.WritePadPinFile();
    PARSER_obj.WritePltFile();
    // PARSER_obj.WritePlFile();
    PARSER_obj.WriteLegalPlFile();
    // PARSER_obj.WriteRowFile(ABACUS_obj);
    // PARSER_obj.WriteMFile();

    cout << "\nINFO\n";
    PARSER_obj.CoutInfo();

    cout << "\nEXIT_SUCCESS\n";
    return EXIT_SUCCESS;
}

// ./HW3_M16131056 ../benchmarks/superblue1/superblue1.aux
// ./HW3_M16131056 ../benchmarks/superblue5/superblue5.aux
// ./HW3_M16131056 ../benchmarks/superblue19/superblue19.aux

// gnuplot superblue1/superblue1.plt
// gnuplot superblue5/superblue5.plt
// gnuplot superblue19/superblue19.plt

// ./iccad2013_check_legality superblue1.aux superblue1.legal.pl
// ./iccad2013_check_legality superblue5.aux superblue5.legal.pl
// ./iccad2013_check_legality superblue19.aux superblue19.legal.pl

// ./iccad2013_get_hpwl superblue1.aux superblue1.legal.pl
// ./iccad2013_get_hpwl superblue5.aux superblue5.legal.pl
// ./iccad2013_get_hpwl superblue19.aux superblue19.legal.pl