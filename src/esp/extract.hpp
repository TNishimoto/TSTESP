/* extract.hpp
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

#ifndef EXTRACT_HPP
#define EXTRACT_HPP

#include <string>
#include <sstream>
#include "esp_index.hpp"
#include "get_time.hpp"

class Extract{
private:
  uint64_t len_;
  std::vector<espstring> substr_;
  uint64_t pos_;
  uint64_t num_pos_;
public:
  ESPIndex* esp_index_;
  Extract():
             len_(),
             substr_(),
             pos_(),
             num_pos_(){}
  ~Extract(){}
  void StartExtract(uint64_t pos, uint64_t len, vector<uint64_t> &output);
  void RepeatExtract(const uint64_t node, const uint64_t start_pos, const uint64_t end_pos, vector<uint64_t> &output);
  //void RepeatAddChar(const uint64_t node);
};

#endif //EXTRACT_HPP
