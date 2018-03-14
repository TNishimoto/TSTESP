/*
* Copyright (c) 2018 Takaaki Nishimoto
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published bytes
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include "esp_tree.hpp"
#include "esp_index.hpp"
#include "cmdline.h"
#include "get_time.hpp"
#include "tstesp.h"

using namespace std;

void build(ifstream &textStream, ofstream &tstStream, ofstream &espStream, string tmpFilename, uint32_t q, uint32_t pr)
{

    ofstream tmpStream;
    ifstream tmpInputStream;
    tmpStream.open(tmpFilename, ios::out | ios::binary);
    //buildQXBW(textStream, xbwStream, tmpStream, labelDicFilename, q, useOccArray);
    tst::TST::build(textStream, tstStream, &tmpStream, q);
    //buildTST(textStream, tstStream, tmpStream, q);

    std::cout << "end TST" << std::endl;

    tmpInputStream.open(tmpFilename, ios::binary);
    ESPIndex::buildESP(tmpInputStream, espStream, pr);
    tmpInputStream.close();
    espStream.close();
    int err = remove(tmpFilename.c_str());
}
bool build(string textfile, string outputFile, uint32_t q, uint32_t pr)
{

    auto output_tst_file = outputFile + ".tst";
    auto output_esp_file = outputFile + ".tesp";
    auto tmp_file = outputFile + ".ttext";

    ifstream inputStream;
    inputStream.open(textfile);
    if (!inputStream)
    {
        std::cout << "No Text File!" << std::endl;
        return false;
    }
    ofstream tstStream;
    tstStream.open(output_tst_file, ios::out | ios::binary);

    ofstream espStream;
    espStream.open(output_esp_file, ios::out | ios::binary);
    build(inputStream, tstStream, espStream, tmp_file, q, pr);

    inputStream.close();
    tstStream.close();
    espStream.close();
    return true;
}

int main(int argc, char **argv)
{

    cmdline::parser p;

    p.add<string>("input_file", 'i', "input file name", true);
    p.add<string>("output_file", 'o', "output file name", true);
    p.add<string>("qgram", 'q', "qgram length", true);

    p.add<uint32_t>("extraction_length_ratio", 'e', "store extracation length of nodes per ESP tree level e", false, 2);

    p.parse_check(argc, argv);

    string input_file = p.get<string>("input_file");
    int32_t qgramLen = std::stoi(p.get<string>("qgram"));
    if (qgramLen <= 0)
    {
        std::cout << "invalid q <= 0" << std::endl;
        return 0;
    }
    string output_file = p.get<string>("output_file");
    uint32_t pr = p.get<uint32_t>("extraction_length_ratio");

    build(input_file, output_file, qgramLen, pr);
    return 0;
}
