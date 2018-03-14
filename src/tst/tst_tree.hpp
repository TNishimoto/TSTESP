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
#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <strstream>
#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <time.h>
#include <algorithm>
#include <stdint.h>
#include <cassert>

#include <io_functions.h>
//#include "my_function.h"
//#include "string_functions.h"
//#include "zstring.h"
#include "tst_node.hpp"
#include <stack>

namespace tst
{
//class TSTNodePointer;

class TST
{
  public:
  class TSTNodePointer
	{
	  public:
		TST &tree;
		NodeIndex index;
		int32_t position;
		int32_t pathLength;
		bool isOnBottom();
		bool isOnRoot();
		bool isOnEdge();
		bool isOnLeaf();
		TSTNode &node();
		void getPathString(istring &result);
		uint64_t getPathLength();
		bool proceed(ichar c);
		void useSuffixLink();
		uint64_t addLeaf(ichar newCharacter);
		void split();

		bool isOnChild();
		void moveNextLeave();
		void getLeave(vector<uint64_t>& result);
		void getInfo();
		bool onlineConstruct(ichar nowChar, ichar lastChar);
		uint64_t getNextLeafEdgeLength();
		bool moveToNextLeaf(ichar label);
		TSTNodePointer(TST &_tree, NodeIndex _index, int _pos) : tree(_tree), index(_index), position(_pos), pathLength(0)
		{
		}
		
	};

  private:
	void createTreeLog(vector<string> &log, int i);
	void createTreeLog(vector<string> &log);
	void getInfo();
	bool suffixLinkCheck(uint64_t i);
	uint64_t createRoot();
	NodeIndex createNode(uint64_t parent, uint64_t _textSourcePosition, uint64_t _edgeLength, bool isLeaf);
	InsertResult getInsertIndex(NodeIndex index, ichar edgeCharacter);
	TSTNode &operator[](NodeIndex index) { return index.second ? this->leave[index.first] : this->internalNodes[index.first]; }
	void getPathString(NodeIndex index, istring &result);
	uint64_t getPathLength(NodeIndex index);

	vector<TSTNode> internalNodes{};
	vector<TSTNode> leave{};
	vector<uint64_t> nextLeave{};
	vector<uint64_t> suffixLinks{};
	vector<uint64_t> countNonLeaveVec{};
	vector<uint64_t> countLeaveVec{};

	vector<vector<NodeIndex>> childrens{};
	istring truncatedText;

    static const uint64_t PROCESSBAR = 10000;
    static const uint64_t PROCESSBAR2 = 1000000;
	static NodeIndex ROOTINDEX;
  public:

	uint64_t truncatedLength;

	void construct(istring &text, uint64_t qgram);
	void constructCountVec(const vector<uint64_t>& translatedText, bool clearCountLeaveVec);
	TSTNode getRoot();
	void clear();
	void getAllQgrams(vector<string> result);
	void save(ofstream &os);
	void load(ifstream &ifs);
	void getLeafLink(uint64_t leafIndex, vector<std::pair<uint64_t, ichar>>& result);
	uint64_t getNextLeaf(uint64_t leafIndex, ichar label);

	TST() : truncatedLength(0)
	{
	}
	void constructQgramTree(vector<vector<uint64_t>>& _childrens, vector<ichar>& _labels, vector<uint64_t>& qgramNodePositionsOfTSTLeave);
	void locate(istring& pattern, vector<uint64_t>& result);
	uint64_t count(istring& pattern);
	std::pair<bool, uint64_t> countUsingInternalCountVec(istring &pattern);
	bool translatePattern(istring& pattern, vector<uint64_t>& result, bool isText);
	bool restoreText(vector<uint64_t> &text, istring &result, uint64_t start_pos, uint64_t end_pos);

	bool restoreText(vector<uint64_t>& text, istring& result);
	bool hasInternalCountVec(){
		return this->countNonLeaveVec.size() == this->internalNodes.size();
	}
	bool hasCountVec(){
		return this->countNonLeaveVec.size() == this->internalNodes.size() && this->countLeaveVec.size() == this->leave.size();
	}

	uint64_t getCount(NodeIndex index){
		//std::cout << leafIndex << "/" << this->countNonLeaveVec.size() << std::endl;
		if(index.second){
			return this->countLeaveVec[index.first];			
		}else{
			return this->countNonLeaveVec[index.first];						
		}
	}
	TSTNodePointer createLeafPointer(uint64_t leafIndex){
		NodeIndex p(leafIndex, true);
		TSTNode leaf = (*this)[p];
		TSTNodePointer pointer(*this, p, leaf.edgeLength);
		pointer.pathLength = this->truncatedLength;
		return pointer;
	}
	void print(){
		//std::cout << tree.countNonLeaveVec.size() << std::endl;
        for(auto& p : this->countNonLeaveVec){
            std::cout << p << ", ";
        }
	}
	void printInfo(){
		uint64_t totalBytes = 0;
		std::cout << "internalNode Vector        : " << sizeof(TSTNode) <<" bytes * " << this->internalNodes.size() << std::endl;
		totalBytes += sizeof(TSTNode) * this->internalNodes.size();
		std::cout << "leave vector               : " << sizeof(TSTNode) <<" bytes * " << this->leave.size() << std::endl;
		totalBytes += sizeof(TSTNode) * this->leave.size();

		std::cout << "next leave vector          : " << sizeof(uint64_t) << " bytes * " << this->nextLeave.size() << std::endl;
		totalBytes += sizeof(uint64_t) * this->nextLeave.size();

		std::cout << "suffix link vector         : " << sizeof(uint64_t) << " bytes * " << this->suffixLinks.size() << std::endl;
		totalBytes += sizeof(uint64_t) * this->suffixLinks.size();

		uint64_t p = 0;
		for(auto& children : this->childrens){
			p += children.size();
		}
		std::cout << "children vector            : " <<  sizeof(NodeIndex) << " bytes * " << p << std::endl;
		totalBytes += sizeof(NodeIndex) * p;


		std::cout << "truncated text             : " <<  sizeof(ichar) << " bytes * " << this->truncatedText.size() << std::endl;
		totalBytes += sizeof(ichar) * this->truncatedText.size();

		std::cout << "internal node count vector : " << sizeof(uint64_t) << " bytes * " << this->countNonLeaveVec.size() << std::endl;
		totalBytes += sizeof(uint64_t) * this->countNonLeaveVec.size();

		std::cout << "leave count vector         : " << sizeof(uint64_t) <<  " bytes * " << this->countLeaveVec.size() << std::endl;
		totalBytes += sizeof(uint64_t) * this->countLeaveVec.size();


		std::cout << "Total : " << totalBytes << "bytes" << std::endl;
	}

	static void build(ifstream &textStream, ofstream &tstStream, ofstream *tmpStream, uint32_t q){
		string text;
        std::cout << "loading text" << std::endl;         
        bool b = my::IO::load(textStream, text);
        tst::istring itext(text.begin(), text.end());
        //std::string().swap(text);

		tst::TST tst;
        std::cout << "construct TST" << std::endl; 
        tst.construct(itext, q);

        vector<ochar> outputText;
        std::cout << "translate text to qgram code" << std::endl; 
        tst.translatePattern(itext, outputText, true);
        std::cout << "constructing CountNonLeaveVec" << std::endl;
        tst.constructCountVec(outputText, true);
        std::cout << "constructed." << std::endl;

        if(outputText.size() != itext.size() - tst.truncatedLength + 1){
            throw "error";
        }

        std::cout << "save qgram code" << std::endl;
		if(tmpStream != NULL){
			//my::IO::writeVector<espchar>(*tmpStream,outputText);

			tmpStream->write((const char *)(&outputText[0]), sizeof(tst::ochar) * outputText.size());
		}
        std::cout << "save TST" << std::endl;
        tst.save(tstStream);

		textStream.close();
		tstStream.close();
		if(tmpStream != NULL)tmpStream->close();
	}
	/*
	void getPathStringFromLeafIndex(uint64_t index, istring& result){
			TSTNodePointer pointer = this->createLeafPointer(index);
			return pointer.getPathString(result);
	}
	*/
};
}