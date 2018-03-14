/* extract.cpp
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

#include "extract.hpp"

using namespace std;

void Extract::StartExtract(uint64_t pos, uint64_t len, vector<uint64_t> &output)
{

  RepeatExtract(esp_index_->root(), pos, pos + len - 1, output);

  //output.swap(substr_[num_pos_]);
}

void Extract::RepeatExtract(const uint64_t node, const uint64_t start_pos, const uint64_t end_pos, vector<uint64_t> &output)
{
  uint64_t left, right, left_length;
  if (node >= CONSTANT::CHARMAX)
  {
    left = esp_index_->left(node);
    right = esp_index_->right(node);
    left_length = esp_index_->extraction_length(left);
    
    uint64_t right_length =  esp_index_->extraction_length(right);
    
    if(left_length + right_length < end_pos - start_pos + 1){
      throw -1;
    }
    
    uint64_t len = end_pos - start_pos + 1;
    uint64_t now_pos = output.size();
    if(start_pos < left_length){
      RepeatExtract(left, start_pos, end_pos < left_length ? end_pos : left_length - 1, output);
    }
    if(end_pos >= left_length){ 
      RepeatExtract(right, start_pos < left_length ? 0 : start_pos - left_length, end_pos - left_length, output);
    }
    uint64_t now_end_pos = output.size();    
  }
  else
  {
    output.push_back(esp_index_->GetVarToChar(node));
  }
}
/*
void Extract::RepeatAddChar(const uint64_t node, vector<uint64_t> &output)
{
  if (len_ != 0)
  {
    if (node < CONSTANT::CHARMAX)
    {
      len_--;
      substr_[num_pos_].push_back(esp_index_->GetVarToChar(node));
      
    std::cout << "pos_ : " << pos_ << ", c = " << esp_index_->GetVarToChar(node) << std::endl;
      pos_++;
    }
    else
    {
      RepeatAddChar(esp_index_->left(node));
      RepeatAddChar(esp_index_->right(node));
    }
  }
}
*/
