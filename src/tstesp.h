/* 
 * Copyright (C) 2018, Takaaki Nishimoto, all rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1. Redistributions of source code must retain the above Copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above Copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of the authors nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 */

#pragma once
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include "esp_tree.hpp"
#include "esp_index.hpp"
#include "get_time.hpp"
//#include "tst_file_manager.h"
#include "io_functions.h"
#include "tst_tree.hpp"
#include "tst_test.hpp"

#include "search_query.hpp"
#include "extract.hpp"


namespace tstesp
{
    void showResult(uint64_t time, uint64_t occ, uint64_t totallen, uint64_t totalqueries){
        
        cout << "OCC : " << occ << endl;
        cout << "OCC / query :" << ((double)occ / totalqueries) << endl;
        
        double mtime = (double)time / (1000*1000);
        cout << "Total Query Time : " << mtime << "msec" << endl;
        cout << "Total Query Time / character : " << mtime / totallen << "msec" << endl;
        cout << "Total Query Time / query : " << mtime / totalqueries << "msec" << endl;
    }
    
class TSTESPIndex
{
  public:
    static void decompress_esp(ESPIndex& index, ofstream &output) {
		double time;
		cout << "start decompression" << endl;

		time = Gettime::get_time();
		index.Decompress(output);
		time = Gettime::get_time() - time;
		cout << "end decompression" << endl;
		cout << "Decompression time: " << time << " sec" << endl;
		output.close();

	}
    
    
    static void decompress(ESPIndex &esp, tst::TST &tst, string tmpFilename, tst::istring &output)
    {
		ofstream tmpStream;
		ifstream tmpInputStream;
		tmpStream.open(tmpFilename, ios::out | ios::binary);
		tstesp::TSTESPIndex::decompress_esp(esp, tmpStream);
        tmpStream.close();

		tmpInputStream.open(tmpFilename, ios::binary);
		espstring inp;
		my::IO::loadVec<espchar>(tmpInputStream, inp);
        tmpInputStream.close();
        int err=remove(tmpFilename.c_str());
		
		tst.restoreText(inp, output);

    }
    /*
    unique_ptr<> extract_esp(string &outputFile)
    {
        ofstream ofs(outputFile.c_str());
        double time;
        cout << "start decompression" << endl;

        time = Gettime::get_time();
        this->esp_index.Decompress(ofs);
        time = Gettime::get_time() - time;
        cout << "end decompression" << endl;
        cout << "Decompression time: " << time << " sec" << endl;
        ofs.close();
    }
    */
    
    static void extract(ESPIndex &esp, tst::TST &tst, Extract &ext, uint64_t pos, uint64_t len, tst::istring &output)
    {
        vector<uint64_t> output64;

        uint64_t npos = pos < tst.truncatedLength ? 0 : pos - tst.truncatedLength;
        uint64_t diff_pos = pos < tst.truncatedLength ? pos : 0;
        
        uint64_t nlen = len < tst.truncatedLength ? 1 : len - tst.truncatedLength + 1 + diff_pos;
        //std::cout << "npos : " << npos << ", nlen : " << nlen << std::endl;
        ext.StartExtract(npos, nlen, output64);
        if(output64.size() != nlen){
            std::cout << "error" << std::endl;
            throw -1;
        }
        
        
        //for(auto& p : output64){
         //   std::cout << p << ", ";
        //}
        //std::cout << std::endl;
        //std::cout << "restore : " << diff_pos << ", " << len << std::endl;
        tst.restoreText(output64, output, diff_pos, diff_pos + len - 1);
        
        
        //for(auto& p : output){
        //    std::cout << p << ", ";
        //}
        //std::cout << std::endl;
        

        output.resize(len);
        //return pattern8->substr(0,len);
        
    }
    

    

    static void locate(ESPIndex &esp, tst::TST &tst, SearchQuery &st, tst::istring &pattern, vector<uint64_t>& result)
    {
        
        result.clear();
        if (pattern.size() <= tst.truncatedLength)
        {
            vector<uint64_t> tst_occ;
            tst.locate(pattern,tst_occ);

            vector<uint64_t> resultESP;
            for (auto pos : tst_occ)
            {
                resultESP.clear();
                auto posStr = espstring();
                posStr.push_back(pos);
                st.LocateQuery(posStr, resultESP);
                for (auto it : resultESP)
                {
                    result.push_back(it);
                }
            }

        
        }
        else
        {
            espstring qPattern;
            bool b = tst.translatePattern(pattern, qPattern, false);
            //auto qpattern = tst->translateQgramText(pattern);
            if (!b)
            {            
            }
            else
            {
                st.LocateQuery(qPattern, result);
            }
        }

        
    }
    static uint64_t count(ESPIndex &esp, tst::TST &tst, SearchQuery &st, tst::istring &pattern)
    {
        
        if (pattern.size() <= tst.truncatedLength)
        {
            uint64_t occ = 0;
            espstring posStr;
            posStr.clear();

            if(tst.hasInternalCountVec()){
               std::pair<bool,uint64_t> result = tst.countUsingInternalCountVec(pattern);               
               posStr.push_back(result.second);
               occ = result.first ? result.second : st.CountQuery(posStr);
            }else{
                vector<uint64_t> tst_occ;
                tst.locate(pattern, tst_occ);
                //auto r = make_unique<vector<uint64_t>>();
                for (auto pos : tst_occ)
                {
                    posStr.push_back(pos);
                    occ += st.CountQuery(posStr);
                    posStr.clear();
                }
            }
            return occ;
        }
        else
        {
            espstring qPattern;
            bool b = tst.translatePattern(pattern, qPattern, false);
            //auto qpattern = tst->translateQgramText(pattern);
            if (!b)
            {                
                return 0;
            }
            else
            {
                return st.CountQuery(qPattern);
            }
        }
        
    }
};
}