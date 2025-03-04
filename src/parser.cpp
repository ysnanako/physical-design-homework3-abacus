#include "parser.h"
using namespace std;

#define N 0
#define S 1
#define W 2
#define E 3
#define F 4

// public
// argv
void PARSER::GetFilename(const int& argc, char *argv[])
{
    if(argc == 2)
        benchmark = argv[1];
    else
    {
        cerr << "Usage: ./exe <circuit.aux>. \n";
        exit(EXIT_FAILURE);
    }
    for(size_t cnt = 0, i = 0; cnt < 3; ++i)
    {
        benchmark_dir += argv[1][i];
        if(cnt == 2 && argv[1][i] != '/')
            benchname += argv[1][i];
        if(argv[1][i] == '/')
            cnt++;
    }
    output_dir = "dat/" + benchname + "/";
}
// read
void PARSER::ReadAuxFile()
{
    fin.open(benchmark);
    if(!fin)
    {
        cerr << left << setw(10) << benchmark << " failed\n";
        exit(EXIT_FAILURE);
    }
    string temp;
    fin >> temp >> temp >> node_filename >> net_filename >> wts_filename >> pl_filename >> scl_filename;
    fin.close();
    node_filename = benchmark_dir + node_filename;
    net_filename = benchmark_dir + net_filename;
    wts_filename = benchmark_dir + wts_filename;
    pl_filename = benchmark_dir + pl_filename;
    scl_filename = benchmark_dir + scl_filename;
    fin.close();
    cout << left << setw(10) << ".aux" << " complete\n";
}
void PARSER::ReadNodeFile()
{
    fin.open(node_filename);
    if(!fin)
    {
        cerr << left << setw(10) << ".node" << " failed\n";
        exit(EXIT_FAILURE);
    }
    string line;
    getline(fin, line);
    getline(fin, line);
    getline(fin, line);
    getline(fin, line);
    getline(fin, line);
    getline(fin, line);
    while(getline(fin, line))
    {
        if(line.size() < 1)
            continue;
        else if(line.back() != 'l' && line.back() != 'I')
            CreatModule(line);
        else if(line.back() == 'l' || line.back() == 'I')
            CreatPad(line);
    }
    fin.close();
    cout << left << setw(10) << ".node" << " complete\n";
}
void PARSER::ReadNetFile()
{
    fin.open(net_filename);
    if(!fin)
    {
        cerr << left << setw(10) << ".net" << " failed\n";
        exit(EXIT_FAILURE);
    }
    string line;
    getline(fin, line);
    getline(fin, line);
    getline(fin, line);
    getline(fin, line);
    getline(fin, line);
    getline(fin, line);
    getline(fin, line);
    while(getline(fin, line))
    {
        if(line.size() < 1)
            continue;
        else if(line.substr(0, 1) == "N")
            CreatNet(line);
        else if(line.substr(0, 1) == "\t")
            CreatPin(line);
    }
    default_Pin_W_H.Set(3);
    fin.close();
    cout << left << setw(10) << ".net" << " complete\n";
}
void PARSER::ReadWtsFile()
{
    
}
void PARSER::ReadPlFile()
{
    fin.open(pl_filename);
    if(!fin)
    {
        cerr << left << setw(10) << ".pl" << " failed\n";
        exit(EXIT_FAILURE);
    }
    string line;
    getline(fin, line);
    getline(fin, line);
    getline(fin, line);
    getline(fin, line);
    while(getline(fin, line))
    {
        if(line.size() < 1)
            continue;
        SetNodePos(line);
    }
    chip_W_H.update_W_H(chip_lb, chip_ur);
    chip_area = chip_W_H.GetProduct();
    fin.close();
    cout << left << setw(10) << ".pl" << " complete\n";
}
void PARSER::ReadGpplFile()
{
    fin.open(benchmark_dir + benchname + ".gp.pl");
    if(!fin)
    {
        cerr << left << setw(10) << ".gp.pl" << " failed\n";
        exit(EXIT_FAILURE);
    }
    string line;
    getline(fin, line);
    getline(fin, line);
    getline(fin, line);
    getline(fin, line);
    while(getline(fin, line))
    {
        if(line.size() < 1)
            continue;
        SetNodePos(line);
    }
    chip_W_H.update_W_H(chip_lb, chip_ur);
    chip_area = chip_W_H.GetProduct();
    fin.close();
    cout << left << setw(10) << ".gp.pl" << " complete\n";
}
void PARSER::ReadSclFile()
{
    fin.open(scl_filename);
    if(!fin)
    {
        cerr << left << setw(10) << ".scl" << " failed\n";
        exit(EXIT_FAILURE);
    }
    string line;
    getline(fin, line);
    getline(fin, line);
    getline(fin, line);
    getline(fin, line);
    double num_rows = 0, num_sites = 0, height = 0, site_width = 0;
    while(getline(fin, line))
    {
        if(line.size() < 8)
            continue;
        else if(line.substr(0, 7) == "NumRows" && !num_rows)
        {
            // core
            num_rows = GetNum(line);
        } 
        else if(line.substr(0, 8) == "   Coord")
        {
            // row
            ROW *new_ptr = new ROW;
            new_ptr->lb.y = GetNum(line);
            ROWS.push_back(new_ptr);
            // core
            if(core_lb.y == PREC_MAX)
                core_lb.y = new_ptr->lb.y;
        }
        else if(line.substr(0, 9) == "   Height")
        {
            // row
            ROWS.back()->W_H.y = GetNum(line);
            // core
            if(!height)
                height = ROWS.back()->W_H.y;
        }
        else if(line.substr(0, 12) == "   Sitewidth" && !site_width)
        {
            // core
            site_width = GetNum(line);
        }
        else if(line.substr(0, 14) == "   Sitespacing")
        {
            // row
            ROWS.back()->Sitespacing = GetNum(line);
        }
        else if(line.substr(0, 9) == "   Subrow")
        {
            // row
            ROWS.back()->lb.x = GetNum(line);
            ROWS.back()->NumSites = GetNum(line.substr(24));
            ROWS.back()->W_H.x = site_width * ROWS.back()->NumSites;
            // core
            if(ROWS.back()->lb.x < core_lb.x)
                core_lb.x = ROWS.back()->lb.x;
            if(num_sites < ROWS.back()->NumSites)
                num_sites = ROWS.back()->NumSites;
        }
    }
    core_ur.x = core_lb.x + num_sites * site_width;
    core_ur.y = core_lb.y + num_rows * height;
    core_W_H.update_W_H(core_lb, core_ur);
    core_area = core_W_H.GetProduct();
    fin.close();
    cout << left << setw(10) << ".scl" << " complete\n";
}
// write
void PARSER::WriteChipFile()
{
    fout.open(output_dir + benchname + "_chip.dat");
    if(!fout)
    {
        cerr << left << setw(10) << "_chip.dat" << " failed\n";
        exit(EXIT_FAILURE);
    }
    fout << "\n# core region\n";
    fout << core_lb.x << ", " << core_lb.y << "\n";
    fout << core_ur.x << ", " << core_lb.y << "\n";
    fout << core_ur.x << ", " << core_ur.y << "\n";
    fout << core_lb.x << ", " << core_ur.y << "\n";
    fout << core_lb.x << ", " << core_lb.y << "\n\n";
    fout << "\n# die area\n";
    fout << chip_lb.x << ", " << chip_lb.y << "\n";
    fout << chip_ur.x << ", " << chip_lb.y << "\n";
    fout << chip_ur.x << ", " << chip_ur.y << "\n";
    fout << chip_lb.x << ", " << chip_ur.y << "\n";
    fout << chip_lb.x << ", " << chip_lb.y << "\n\n";
    fout.close();
    cout << left << setw(10) << "_chip.dat" << " complete\n";
}
void PARSER::WriteCellFile()
{
    fout.open(output_dir + benchname + "_cell.dat");
    if(!fout)
    {
        cerr << left << setw(10) << "_cell.dat" << " failed\n";
        exit(EXIT_FAILURE);
    }
    fout << "\n# FENCE DEFAULT\n";
    fout << "0, 0\n\n";
    for(auto const &module : MODULES)
    {
        // relative to core_lb
        module->lb = module->lb - core_lb; 
        module->ur.update_ur(module->lb, module->W_H);
        // write cell file
        fout << "# " << module->name << "\n";
        fout << module->lb.x << ", " << module->lb.y << "\n";
        fout << module->ur.x << ", " << module->lb.y << "\n";
        fout << module->ur.x << ", " << module->ur.y << "\n";
        fout << module->lb.x << ", " << module->ur.y << "\n";
        fout << module->lb.x << ", " << module->lb.y << "\n\n";
        // relative to (0, 0)
        module->lb = module->lb + core_lb; 
        module->ur.update_ur(module->lb, module->W_H);
    }
    fout.close();
    cout << left << setw(10) << "_cell.dat" << " complete\n";
}
void PARSER::WritePadFile()
{
    fout.open(output_dir + benchname + "_pad.dat");
    if(!fout)
    {
        cerr << left << setw(10) << "_pad.dat" << " failed\n";
        exit(EXIT_FAILURE);
    }
    fout << "\n# PADS\n";
    fout << "0, 0\n\n";
    for(auto const &pad : PADS)
    {
        if(pad->is_cover)
            continue;
        // relative to core_lb
        pad->lb = pad->lb - core_lb;
        pad->ur.update_ur(pad->lb, pad->W_H);
        // write pad file
        fout << "# " << pad->name << "\n";
        fout << pad->lb.x << ", " << pad->lb.y << "\n";
        fout << pad->ur.x << ", " << pad->lb.y << "\n";
        fout << pad->ur.x << ", " << pad->ur.y << "\n";
        fout << pad->lb.x << ", " << pad->ur.y << "\n";
        fout << pad->lb.x << ", " << pad->lb.y << "\n\n";
        // arrow /
        fout << pad->lb.x + pad->W_H.x * 0.65 << ", " << pad->lb.y + pad->W_H.y * 0.85 << "\n";
        fout << pad->lb.x + pad->W_H.x * 0.75 << ", " << pad->ur.y << "\n\n";
        // arrow |
        fout << pad->lb.x + pad->W_H.x * 0.75 << ", " << pad->lb.y + pad->W_H.y * 0.6 << "\n";
        fout << pad->lb.x + pad->W_H.x * 0.75 << ", " << pad->ur.y << "\n\n";
        // arrow
        fout << pad->lb.x + pad->W_H.x * 0.75 << ", " << pad->ur.y << "\n";
        fout << pad->lb.x + pad->W_H.x * 0.85 << ", " << pad->lb.y + pad->W_H.y * 0.85 << "\n\n";
        // relative to (0, 0)
        pad->lb = pad->lb + core_lb;
        pad->ur.update_ur(pad->lb, pad->W_H);
    }
    fout.close();
    cout << left << setw(10) << "_pad.dat" << " complete\n";
}
void PARSER::WritePadPinFile()
{
    fout.open(output_dir + benchname + "_pad_pin.dat");
    if(!fout)
    {
        cerr << left << setw(10) << "_pad_pin.dat" << " failed\n";
        exit(EXIT_FAILURE);
    }
    fout << "\n# PAD_PIN\n";\
    fout << "0, 0\n\n";
    for(auto const &net : NETS)
    {
        fout << "# " << net->name << "\n";
        for(auto const &pin : net->pins)
        {
            if(pin->pad)
            {
                // relative to core_lb
                FPOS pos = pin->pad->center + pin->offset - core_lb;
                fout << pos.x << ", " << pos.y << "\n";
                fout << pos.x + default_Pin_W_H.x << ", " << pos.y << "\n";
                fout << pos.x + default_Pin_W_H.x << ", " << pos.y + default_Pin_W_H.y << "\n";
                fout << pos.x << ", " << pos.y + default_Pin_W_H.y << "\n";
                fout << pos.x << ", " << pos.y << "\n\n";
            }
        }
    }
    fout.close();
    cout << left << setw(10) << "_pad_pin.dat" << " complete\n";
}
void PARSER::WritePltFile()
{
    fout.open("plt/" + benchname + "/" + benchname + ".plt");
    if(!fout)
    {
        cerr << left << setw(10) << ".plt" << " failed\n";
        exit(EXIT_FAILURE);
    }
    CalTotal();
    fout << "set nokey\n";
    fout << "set term pngcairo font 'qt'\n";
    fout << "set output '" << benchname + "/" + benchname  << ".png'\n\n";

    fout << "set size ratio -1\n";
    fout << "set xrange [0:" << chip_ur.x << "]\n";
    fout << "set yrange [0:" << chip_ur.y << "]\n";
    fout << "plot[:][:] ";
    fout << "'../" << output_dir + benchname << "_chip.dat' w l lc -1, ";
    fout << "'../" << output_dir + benchname << "_cell.dat' w l lc 6, ";
    fout << "'../" << output_dir + benchname << "_pad.dat' w l lc 4, ";
    fout << "'../" << output_dir + benchname << "_pad_pin.dat' w l lc -1";
    // fout << "'../" << output_dir + benchname << "_row.dat' w l lc 4, "; 
    fout << "\n\n\n";
    
    fout << "set title '" << benchname << ", MODULE= " << MODULES.size() << ", NET= " << NETS.size() << ", HPWL= " << wHPWL << ", ' font 'Times, 22'\n\n";

    fout << "set term qt\n";
    fout << "set key\n\n";

    fout << "set size ratio -1\n";
    fout << "plot[:][:] ";
    fout << "'../" << output_dir + benchname << "_chip.dat' w l lc -1, ";
    fout << "'../" << output_dir + benchname << "_cell.dat' w l lc 6, ";
    fout << "'../" << output_dir + benchname << "_pad.dat' w l lc 4, ";
    fout << "'../" << output_dir + benchname << "_pad_pin.dat' w l lc -1";
    // fout << "'../" << output_dir + benchname << "_row.dat' w l lc 4, "; 
    fout << "\n\n\n";

    fout << "pause -1 'Press any key'\n";

    fout.close();
    cout << left << setw(10) << ".plt" << " complete\n";
}
// hw2
void PARSER::WritePlFile()
{
    fout.open("plt/" + benchname + "/" + benchname + ".pl");
    if(!fout)
    {
        cerr << left << setw(10) << ".pl" << " failed\n";
        exit(EXIT_FAILURE);
    }
    for(auto const &pad : PADS)
    {
        fout << pad->name << "   ";
        fout << pad->lb.x << "   ";
        fout << pad->lb.y << "   ";
        fout << ":   ";
        if(pad->orientation > 3)
            fout << "F";
        switch (pad->orientation % 4)
        {
        case N:
            fout << "N";
            break;
        case S:
            fout << "S";
            break;
        case W:
            fout << "W";
            break;
        case E:
            fout << "E";
            break;
        default:
            break;
        }
        if(pad->fixed)
            fout << "   /FIXED\n";
        else
            fout << "\n";
    }
    fout.close();
    cout << left << setw(10) << ".pl" << " complete\n";
}
// hw3
void PARSER::WriteLegalPlFile()
{
    fout.open("legal/" + benchname + "/" + benchname + ".legal.pl");
    if(!fout)
    {
        cerr << left << setw(10) << ".legal.pl" << " failed\n";
        exit(EXIT_FAILURE);
    }
    for(auto const &cell : MODULES)
    {
        fout << cell->name << "   ";
        fout << cell->lb.x << "   ";
        fout << cell->lb.y << "   ";
        fout << ":   ";
        if(cell->orientation > 3)
            fout << "F";
        switch (cell->orientation % 4)
        {
        case N:
            fout << "N";
            break;
        case S:
            fout << "S";
            break;
        case W:
            fout << "W";
            break;
        case E:
            fout << "E";
            break;
        default:
            break;
        }
        if(cell->fixed)
            fout << "   /FIXED\n";
        else
            fout << "\n";
    }
    for(auto const &pad : PADS)
    {
        fout << pad->name << "   ";
        fout << pad->lb.x << "   ";
        fout << pad->lb.y << "   ";
        fout << ":   ";
        if(pad->orientation > 3)
            fout << "F";
        switch (pad->orientation % 4)
        {
        case N:
            fout << "N";
            break;
        case S:
            fout << "S";
            break;
        case W:
            fout << "W";
            break;
        case E:
            fout << "E";
            break;
        default:
            break;
        }
        if(pad->fixed)
            fout << "   /FIXED\n";
        else
            fout << "\n";
    }
    fout.close();
    cout << left << setw(10) << ".legal.pl" << " complete\n";
}
void PARSER::WriteRowFile(ABACUS &ABACUS_obj)
{
    fout.open(output_dir + benchname + "_row.dat");
    if(!fout)
    {
        cerr << left << setw(10) << "row.dat" << " failed\n";
        exit(EXIT_FAILURE);
    }
    fout << "\n# ROWS\n";\
    fout << "0, 0\n\n";
    for(auto const &srow_vec : ABACUS_obj.srow_vecvec)
    {
        for(auto const &srow : srow_vec)
        {
            // relative to core_lb
            srow->lb = srow->lb - core_lb;
            srow->ur.update_ur(srow->lb, srow->W_H);
            // write row file
            fout << "# " << srow->lb.y << "\n";
            fout << srow->lb.x << ", " << srow->lb.y << "\n";
            fout << srow->ur.x << ", " << srow->lb.y << "\n";
            fout << srow->ur.x << ", " << srow->ur.y << "\n";
            fout << srow->lb.x << ", " << srow->ur.y << "\n";
            fout << srow->lb.x << ", " << srow->lb.y << "\n\n";
            // relative to (0, 0)
            srow->lb = srow->lb + core_lb;
            srow->ur.update_ur(srow->lb, srow->W_H);
        }
    }
    fout.close();
    cout << left << setw(10) << "_row.dat" << " complete\n";
}
// matlab
void PARSER::WriteMFile()
{
    fout.open("plt/" + benchname + "/" + benchname + ".m");
    if(!fout)
    {
        cerr << left << setw(10) << ".m" << " failed\n";
        exit(EXIT_FAILURE);
    }
    fout << "axis equal;\n" << "hold on\n" << "grid on\n";
    for(auto const &tile : tile_set)
    {
        fout << "block_x=[";
        fout << " " << tile->lb.x;
        fout << " " << tile->lb.x;
        fout << " " << tile->ur.x;
        fout << " " << tile->ur.x;
        fout << " " << tile->lb.x;
        fout << "];\n";
        fout << "block_y=[";
        fout << " " << tile->lb.y;
        fout << " " << tile->ur.y;
        fout << " " << tile->ur.y;
        fout << " " << tile->lb.y;
        fout << " " << tile->lb.y;
        fout << "];\n";
        if(tile->macro)
            fout << "fill(block_x, block_y, 'm');\n";
        else
            fout << "fill(block_x, block_y, 'c');\n";
    }
    fout.close();
    cout << left << setw(10) << ".m" << " complete\n";
}
// cout
void PARSER::CoutInfo()
{
    cout << "benchmark_dir: " << benchmark_dir << endl;
    cout << "output_dir: " << output_dir << endl;
    cout << "Benchmark: " << benchname << endl;
    cout << endl;
    cout << "MODULES.size(): " << MODULES.size() << endl;
    cout << "PADS.size(): " << PADS.size() << endl;
    cout << "NETS.size(): " << NETS.size() << endl;
    // cout << endl;
    // cout << "core_lb: " << core_lb << endl;
    // cout << "core_ur: " << core_ur << endl;
    // cout << "core_W_H: " << core_W_H << endl;
    cout << endl;
    cout << "chip_lb: " << chip_lb << endl;
    cout << "chip_ur: " << chip_ur << endl;
    // cout << "chip_W_H: " << chip_W_H << endl;
    cout << endl;
    // cout << "total_module_area: " << total_module_area << endl;
    // cout << "total_pad_area: " << total_pad_area << endl;
    cout << "wHPWL: " << wHPWL << endl;
}

// private
// node
void PARSER::CreatModule(string line)
{
    stringstream ss(line);
    MODULE *new_ptr = new MODULE;
    ss >> new_ptr->name;
    new_ptr->id = stoul(new_ptr->name.substr(1));
    double w, h;
    ss >> w >> h;
    new_ptr->W_H = FPOS(w, h);
    MODULES.push_back(new_ptr);
    module_map[new_ptr->name] = new_ptr;
}
void PARSER::CreatPad(string line)
{
    stringstream ss(line);
    PAD *new_ptr = new PAD;
    ss >> new_ptr->name;
    new_ptr->id = stoul(new_ptr->name.substr(1));
    double w, h;
    ss >> w >> h;
    new_ptr->W_H = FPOS(w, h);
    if(line.back() == 'I')
        new_ptr->is_cover = true;
    PADS.push_back(new_ptr);
    pad_map[new_ptr->name] = new_ptr;
}
// net
void PARSER::CreatNet(string line)
{
    stringstream ss(line);
    string s;
    NET *new_ptr = new NET;
    ss >> s >> s >> s >> new_ptr->name;
    new_ptr->id = stoul(new_ptr->name.substr(1));
    NETS.push_back(new_ptr);
}
void PARSER::CreatPin(string line)
{
    stringstream ss(line);
    string s;
    ss >> s;
    // module
    auto module_it = module_map.find(s);
    if(module_it != module_map.end())
    {
        MODULE *module_ptr = module_it->second;
        // pin
        double x, y;
        ss >> s >> s >> x >> y;
        PIN *new_ptr = new PIN;
        new_ptr->module = module_ptr;
        new_ptr->offset = FPOS(x, y);
        // node
        module_ptr->pins.push_back(new_ptr);
        module_ptr->nets.push_back(NETS.back());
        // net
        NETS.back()->pins.push_back(new_ptr);
        NETS.back()->modules.push_back(module_ptr);
        return;
    }
    // pad
    auto pad_it = pad_map.find(s);
    if(pad_it != pad_map.end())
    {
        PAD *pad_ptr = pad_it->second;
        // pin
        double x, y;
        ss >> s >> s >> x >> y;
        PIN *new_ptr = new PIN;
        new_ptr->pad = pad_ptr;
        new_ptr->offset = FPOS(x, y);
        // node
        pad_ptr->pins.push_back(new_ptr);
        pad_ptr->nets.push_back(NETS.back());
        // net
        NETS.back()->pins.push_back(new_ptr);
        return;
    }
}
// pl
void PARSER::SetNodePos(string line)
{
    stringstream ss(line);
    string s;
    ss >> s;
    // module
    auto module_it = module_map.find(s);
    if(module_it != module_map.end()) 
    {
        MODULE *module_ptr = module_it->second;
        double x, y;
        ss >> x >> y >> s >> s;
        module_ptr->lb = FPOS(x, y);
        module_ptr->ur.update_ur(module_ptr->lb, module_ptr->W_H);
        module_ptr->center.Center(module_ptr->lb, module_ptr->W_H);
        if(module_ptr->lb.x < chip_lb.x)
            chip_lb.x = module_ptr->lb.x;
        if(module_ptr->lb.y < chip_lb.y)
            chip_lb.y = module_ptr->lb.y;
        if(chip_ur.x < module_ptr->ur.x)
            chip_ur.x = module_ptr->ur.x;
        if(chip_ur.y < module_ptr->ur.y)
            chip_ur.y = module_ptr->ur.y;
        switch (s.back())
        {
        case 'N':
            module_ptr->orientation = N;
            break;
        case 'S':
            module_ptr->orientation = S;
            break;
        case 'W':
            module_ptr->orientation = W;
            break;
        case 'E':
            module_ptr->orientation = E;
            break;
        default:
            break;
        }
        if(s.front() == 'F')
            module_ptr->orientation += F;
        ss >> s;
        if(s.back() == 'D')
            module_ptr->fixed = true;
        return;
    }
    // pad
    auto pad_it = pad_map.find(s);
    if(pad_it != pad_map.end()) 
    {
        PAD *pad_ptr = pad_it->second;
        double x, y;
        ss >> x >> y >> s >> s;
        pad_ptr->lb = FPOS(x, y);
        pad_ptr->ur.update_ur(pad_ptr->lb, pad_ptr->W_H);
        pad_ptr->center.Center(pad_ptr->lb, pad_ptr->W_H);
        if(pad_ptr->lb.x < chip_lb.x)
            chip_lb.x = pad_ptr->lb.x;
        if(pad_ptr->lb.y < chip_lb.y)
            chip_lb.y = pad_ptr->lb.y;
        if(chip_ur.x < pad_ptr->ur.x)
            chip_ur.x = pad_ptr->ur.x;
        if(chip_ur.y < pad_ptr->ur.y)
            chip_ur.y = pad_ptr->ur.y;
        switch (s.back())
        {
        case 'N':
            pad_ptr->orientation = N;
            break;
        case 'S':
            pad_ptr->orientation = S;
            break;
        case 'W':
            pad_ptr->orientation = W;
            break;
        case 'E':
            pad_ptr->orientation = E;
            break;
        default:
            break;
        }
        if(s.front() == 'F')
            pad_ptr->orientation += F;
        ss >> s;
        if(s.back() == 'D')
            pad_ptr->fixed = true;
        return;
    }
}
// scl
double PARSER::GetNum(string line)
{
    stringstream ss(line);
    string s;
    ss >> s >> s >> s;
    return stod(s);
}
// plt
void PARSER::CalTotal()
{
    for(auto const &module : MODULES)
        total_module_area += module->W_H.GetProduct();
    for(auto const &pad : PADS)
        total_pad_area += pad->W_H.GetProduct();
    for(auto const &net : NETS)
    {
        FPOS min_pos, max_pos;
        min_pos.Set(numeric_limits<double>::max());
        max_pos.Set(numeric_limits<double>::min());
        for(auto const &pin : net->pins)
        {
            FPOS pos;
            if(pin->module)
                pos = pin->module->center + pin->offset;
            if(pin->pad)
                pos = pin->pad->center + pin->offset;
            min_pos.Min(pos);
            max_pos.Max(pos);
        }
        wHPWL += (max_pos.x - min_pos.x) + (max_pos.y - min_pos.y);
    }
}