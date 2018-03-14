/* char_map.hpp
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
#pragma once
#ifndef CHAR_MAP_HPP
#define CHAR_MAP_HPP

#include "constant_numbers.hpp"
#include <cstdint>
//#include "zstring.h"
//#include "string_functions.h"
using namespace std;

using espchar = uint64_t;
using espstring = vector<espchar>;

class CharMap
{
private:
  //std::vector<uint8_t> char_to_var_;//char-> var
  //std::vector<espchar> var_to_char_;//var -> char
  //bool *is_char_;//char ch はch bit目であるかないか判定
  //uint64_t char_size_; // 文字種類数

public:
  CharMap() {}
  //char_to_var_(CONSTANT::CHARMAX),
  //var_to_char_(CONSTANT::CHARMAX),
  //is_char_(),
  //char_size_(0) {}
  //クラスの初期化
  void Init()
  {
    /*
     is_char_ = new bool[CONSTANT::CHARMAX];
    for(size_t i = 0; i < CONSTANT::CHARMAX; i++){
      is_char_[i] = false;      
    }
    */
  }
  //char->var var-> charへの変換の登録
  void SetChar(const espchar kChar)
  {
    if (kChar == CONSTANT::CHARMAX)
    {
      throw "error"; 
      //CONSTANT::CHARMAX++;
    }
    /*
    char_to_var_[(uint8_t)kChar] = char_size_;
    var_to_char_[char_size_] = kChar;
    is_char_[(uint8_t)kChar] = true;
    char_size_++;
    */
  }
  //char -> varへの変換となかったら、新しいのを登録
  uint64_t char_to_var(const espchar kChar)
  {
    if (kChar == CONSTANT::CHARMAX)
    {
      throw "error";
      //SetChar(kChar);
    }
    if (kChar < CONSTANT::CHARMAX)
    {
      return (uint64_t)kChar;
    }
    else
    {
      throw "error";
    }
    /*
    if(!is_char_[(uint8_t)kChar]){
      SetChar(kChar);
    }
    return (uint64_t)char_to_var_[(uint8_t)kChar];
    */
  }
  
  //char -> varへの変換となかったら、新しいのを登録
  espchar var_to_char(const uint64_t var)
  {
    if (var < CONSTANT::CHARMAX)
    {
      return (espchar)var;
    }
    else
    {
      throw "error";
    }
  }
  //スペース
  uint64_t ByteSize()
  {
    /*return CONSTANT::CHARMAX * sizeof(uint8_t) 
      + CONSTANT::CHARMAX * sizeof(espchar)
      + CONSTANT::CHARMAX 
      + sizeof(uint64_t);
      */
    return CONSTANT::CHARMAX;
  }

  //領域の削除
  void Clear()
  {
    //CONSTANT::CHARMAX = 0;
    /*
    std::vector<uint8_t> ().swap(char_to_var_);
    std::vector<espchar> ().swap(var_to_char_);
    delete [] is_char_;
    char_size_ = 0;;
    */
  }

  void Save(std::ofstream &ofs)
  {
    /*
    ofs.write((char*)&char_to_var_[0], sizeof(uint8_t) * CONSTANT::CHARMAX);
    ofs.write((char*)&var_to_char_[0], sizeof(espchar) * CONSTANT::CHARMAX);
    ofs.write((char*)&is_char_[0], sizeof(bool) * CONSTANT::CHARMAX);
    ofs.write((char*)&char_size_, sizeof(uint64_t));
    */
    ofs.write((char *)&CONSTANT::CHARMAX, sizeof(uint64_t));
  }

  void Load(std::ifstream &ifs)
  {
    /*
    ifs.read((char*)&char_to_var_[0], sizeof(uint8_t) * CONSTANT::CHARMAX);
    ifs.read((char*)&var_to_char_[0], sizeof(espchar) * CONSTANT::CHARMAX);
    Init();
    ifs.read((char*)&is_char_[0], sizeof(bool) * CONSTANT::CHARMAX);
    ifs.read((char*)&char_size_, sizeof(uint64_t));
    */
    ifs.read((char *)&CONSTANT::CHARMAX, sizeof(uint64_t));
    if(CONSTANT::CHARMAX == 0) throw "error";
  }

  static uint64_t GetMaxCharacter(ifstream &ifs)
  {

    vector<espchar> char_buffer;
    uint64_t input_length = 0;

    ifs.seekg(0, std::ios::end);
    input_length = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    //remaining_len_ = input_length;
    //input_buffer_.resize(input_length);
    //vector<uint64_t> (input_buffer_).swap(input_buffer_);

    char_buffer.resize(input_length / sizeof(espchar));
    ifs.read((char *)&char_buffer[0], input_length);
    uint64_t max = 0;
    for (auto it : char_buffer)
    {
      if (it > max)
        max = it;
    }
    ifs.seekg(0, std::ios::beg);
    return max;
  }
};

#endif //CHAR_MAP_HPP
