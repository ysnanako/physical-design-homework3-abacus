#ifndef _PARSER_H_
#define _PARSER_H_

#include "datatype.h"
#include "cornerstitching.h"
#include "abacus.h"
using namespace std;

class PARSER
{
public:
    string node_filename, net_filename, wts_filename, pl_filename, scl_filename;
    void GetFilename(const int& argc, char *argv[]);
    // read
    void ReadAuxFile();
    void ReadNodeFile();
    void ReadNetFile();
    void ReadWtsFile();
    void ReadPlFile();
    void ReadGpplFile();
    void ReadSclFile();
    // write
    void WriteChipFile();
    void WriteCellFile();
    void WritePadFile();
    void WritePadPinFile();
    void WritePltFile();
    // hw2
    void WritePloutFile();
    // hw3
    void WriteLegalPlFile();
    void WriteRowFile(ABACUS &);
    // matlab
    void WriteMFile(CS &);

private:
    ifstream fin;
    ofstream fout;
    // node
    void CreatModule(string);
    void CreatPad(string);
    // net
    void CreatNet(string);
    void CreatPin(string);
    // pl
    void SetNodePos(string);
    // scl
    double GetNum(string);
    // plt
    void CalTotal();
};

#endif