/* esp_tree.cpp
 * Copyright (C) 2015, Yoshimasa Takabatake, all rights reserved.
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

#include"esp_tree.hpp"

using namespace std;

void ESPTree::Init(const uint64_t kReserveLength){
  
  uint64_t reserve_length = kReserveLength + CONSTANT::CHARMAX;
  
  pdict_.Init(reserve_length);
  rpdict_.Init(reserve_length);
  char_map_.Init();
  occ_symbols_.resize(CONSTANT::CHARMAX);
  vector<uint64_t> (occ_symbols_).swap(occ_symbols_);
  for(uint64_t i = 0; i < CONSTANT::CHARMAX; i++){
    occ_symbols_[i] = 0;
  }
}

void ESPTree::Build(ifstream &ifs){
  double time = Gettime::get_time();

  CONSTANT::CHARMAX = CharMap::GetMaxCharacter(ifs) + 1;
  input_buffer_pos_  = 0;
  previous_ = DUMMYCODE;
  next_level_len_ = 0;
  remaining_len_ = 0;
  
  Init(4 * TRANSMB / sizeof(uint64_t) + 1);  
  cout << "Read input text" << endl;
  ReadInputText(ifs);
  cout << "Finish reading input text" << endl;
  
  cout << "Build ESP tree" << endl;
  
  while(remaining_len_ != 1){
    
    previous_ = DUMMYCODE;
    input_buffer_pos_ = 0;
    next_level_len_ = 0;
    
    while(remaining_len_ > 5){
      if(Is2Tree(input_buffer_, 
		 input_buffer_pos_)){
	Build2Tree();
      }
      else{
	Build2_2Tree();
      }
    }
    if(remaining_len_ == 5){
      Build2Tree();
      Build2_2Tree();
    }
    else if(remaining_len_  == 4){
      Build2Tree();
      Build2Tree(); 
    }
    else if(remaining_len_ == 3){
      Build2_2Tree();
    }
    else if(remaining_len_  == 2){
      Build2Tree();
    }
    
    remaining_len_ = next_level_len_;

    Permutation *perm_index = pdict_.Sort();

    RenameExtractionLength(perm_index);
    
    RenameNextLevelVars(perm_index,
			0,
			1);

    perm_index->Clear();
    delete perm_index;
    rpdict_.Clear(pdict_);
    pdict_.SetOffset();
  }
  SetRoot(input_buffer_[0]);  

  time = Gettime::get_time() - time;
  cout << "Finish building ESP tree" << endl;
  cout <<"Total production rules : " << pdict_.num_prules() << endl;
  cout << "Build ESP tree time: "<< time << " sec" << endl;
}


void ESPTree::Build2Tree(){
  uint64_t var,left, right;
  
  var = rpdict_.ReverseAccessToPRule(pdict_,
				     (left = input_buffer_[input_buffer_pos_]), 
				     (right  = input_buffer_[input_buffer_pos_ + 1]));
  if(var == pdict_.num_prules()){
    pdict_.PushPRule(left, right);
    extraction_length_.push_back(extraction_length(left) 
				 + extraction_length(right));
    occ_symbols_.push_back(0);
  }
  input_buffer_[next_level_len_++] = var;
  occ_symbols_[var]++;
  previous_ = right;
  input_buffer_pos_ += 2;
  remaining_len_ -= 2;
}

void ESPTree::Build2_2Tree(){
  uint64_t var1, var2, left, right;
  var1 = rpdict_.ReverseAccessToPRule(pdict_,
				      (left = input_buffer_[input_buffer_pos_ + 1]), 
				      (right  = input_buffer_[input_buffer_pos_ + 2]));  
  if(var1 == pdict_.num_prules()){
    pdict_.PushPRule(left, right);
    extraction_length_.push_back(extraction_length(left) 
				 + extraction_length(right));

    occ_symbols_.push_back(0);
  }
  occ_symbols_[var1]++;
  var2 = rpdict_.ReverseAccessToPRule(pdict_,
				      (left = input_buffer_[input_buffer_pos_]), 
				      var1);
  if(var2 == pdict_.num_prules()){
    pdict_.PushPRule(left, var1);
    extraction_length_.push_back(extraction_length(left) 
				 + extraction_length(var1));
    occ_symbols_.push_back(0);
  }
  occ_symbols_[var2]++;
  input_buffer_[next_level_len_++] =  var2;
  previous_ = right;
  input_buffer_pos_ += 3;
  remaining_len_ -= 3;
}

void ESPTree::RenameExtractionLength(Permutation *perm_index){
  
  vector<uint64_t> tmp_extraction_length;
  vector<uint64_t> tmp_occ_symbols;

  tmp_extraction_length.resize(pdict_.num_prules() - GetOffset(Level() - 1));
  tmp_occ_symbols.resize(pdict_.num_prules() - GetOffset(Level() - 1));
  vector<uint64_t> (tmp_extraction_length).swap(tmp_extraction_length);
  vector<uint64_t> (tmp_occ_symbols).swap(tmp_occ_symbols);
  tmp_extraction_length.clear();
  tmp_occ_symbols.clear();
  for(size_t i = GetOffset(Level() - 1) ; i < pdict_.num_prules(); i++){
    tmp_extraction_length.push_back(extraction_length(pdict_.LevelVarToVar(perm_index->Access(pdict_.VarToLevelVar(i)))));
    tmp_occ_symbols.push_back(occ_symbols_[pdict_.LevelVarToVar(perm_index->Access(pdict_.VarToLevelVar(i)))]);
  }
  
  for(size_t i = 0; i < (pdict_.num_prules() - GetOffset(Level() - 1)); i++){
    extraction_length_[pdict_.LevelVarToVar(i) - CONSTANT::CHARMAX] = tmp_extraction_length[i];
    occ_symbols_[pdict_.LevelVarToVar(i)] = tmp_occ_symbols[i];  
  }
}

void ESPTree::RenameNextLevelVars(Permutation *perm_index,
				  const uint64_t kInputSpace,
				  const uint64_t kMode){
  if(kMode == 1){
    for(size_t i = 0; i < remaining_len_; i++){
      input_buffer_[i] = pdict_.LevelVarToVar(perm_index->ReverseAccess(pdict_.VarToLevelVar(input_buffer_[i])));
    }
    input_buffer_.resize(remaining_len_);
    vector<uint64_t> (input_buffer_).swap(input_buffer_);
  }
  else{

    ifstream ifs("DiskBuffer2");
    ofstream ofs("DiskBuffer1");
    
    uint64_t total_read_length = 0;
    uint64_t read_length = kInputSpace * TRANSMB;
    
    ifs.seekg(0,ios::end);
    uint64_t input_length = ifs.tellg()/sizeof(uint64_t);
    ifs.seekg(0,ios::beg);

      
    if(read_length > input_length){
      read_length = input_length;
    }
    
    while(ifs.read((char*)&input_buffer_[0], read_length * sizeof(uint64_t)) 
	  && total_read_length != input_length){
      for(size_t i = 0; i < read_length ; i++){
	input_buffer_[i] = pdict_.LevelVarToVar(perm_index->ReverseAccess(pdict_.VarToLevelVar(input_buffer_[i])));
      }
      ofs.write((char*)&input_buffer_[0], read_length * sizeof(uint64_t));
      
      total_read_length += read_length;
      if((total_read_length + read_length) > input_length){
	read_length = input_length - total_read_length;
      }
    }
    ifs.close();
    ofs.close();
  }
}

uint64_t ESPTree::GetNumPRules(){
  return pdict_.num_prules();
}

void ESPTree::SetRoot(const uint64_t kVar){
  root_ = kVar;
  rpdict_.Delete();
  pdict_.AdjustLength();
}

uint64_t ESPTree::root(){
  return root_;
}

uint64_t ESPTree::extraction_length(const uint64_t kVar){
  if(kVar < CONSTANT::CHARMAX){
    return 1;
  }
  else if(kVar == DUMMYCODE){
    return 0;
  }
  return extraction_length_[kVar - CONSTANT::CHARMAX];
}

CharMap ESPTree::char_map(){
  return char_map_;
}

uint64_t ESPTree::occ_symbol(const uint64_t kSymbol){
  return occ_symbols_[kSymbol];
}

uint64_t ESPTree::Level(){
  return pdict_.offset_length();
}

uint64_t ESPTree::GetOffset(const uint64_t kLevel){
  return pdict_.offset(kLevel);
}

PRule ESPTree::GetPRule(const uint64_t kIndex){
  return pdict_.prules(kIndex);
}

void ESPTree::Clear(){
  
  pdict_.Clear();
  rpdict_.Delete();
  vector<uint64_t> ().swap(extraction_length_);
  vector<uint64_t> ().swap(input_buffer_);
  root_ = 0;
  
}
