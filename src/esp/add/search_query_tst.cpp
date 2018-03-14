

#include "search_query.hpp"
#include "io_functions.h"

using namespace std;

uint64_t SearchQuery::CountQuery(espstring &pattern)
{

  double time = 0;
  double sum_find_evidence_time = 0;
  double sum_traverse_tree_time = 0.0;
  size_t query_length = 0;
  size_t sum_query_length = 0;
  size_t sum_occ = 0;
  double count_occ_time = 0.0;
  uint64_t c;
  bool find = false;
  bool cflag = true;

  num_query_ = 1;

  query_.clear();
  evidences_.clear();
  evidences_level_.clear();

  cflag = true;
  query_length = 0;


  for (size_t y = 0; y < pattern.size(); y++)
  {
    query_length++;
    if ((c = esp_index_->GetCharToVar(pattern[y])) == DUMMYCODE)
    {
      cflag = false;
    }
    query_.push_back(c);
  }

  sum_query_length += query_length;

  if (cflag)
  {
    time = Gettime::get_time();
    find = FindEvidences();
    auto find_evidence_time = Gettime::get_time() - time;
    //std::cout << "find_evidence_time = " << (find_evidence_time * 1000) << "msec" << std::endl;
    sum_find_evidence_time += find_evidence_time;
    if (find)
    {

      occount_ = 0;
      time = Gettime::get_time();
      count_occ_time += RepeatCountQuery(evidences_[core_],
                                         core_,
                                         num_evidences_ - core_ - 1);
      
      auto traverse_tree_time = Gettime::get_time() - time;
      //std::cout << "traverse_tree_time = " << (traverse_tree_time * 1000) << "msec" << std::endl;
      sum_traverse_tree_time += traverse_tree_time;
      sum_occ += occount_;

      if (occount_ != 0)
      {
      }
      else
      {
      }
    }
    else
    {
    }
  }
  else
  {
  }

  return occount_;
}

void SearchQuery::LocateQuery(espstring &pattern, vector<uint64_t> &result)
{
  //auto r = make_unique<vector<uint64_t>>();
  double time = 0.0;
  size_t query_length = 0;
  size_t sum_query_length = 0;
  size_t sum_occ = 0;
  double sum_find_evidence_time = 0;
  double find_position_time = 0;
  double sum_traverse_tree_time = 0.0;
  //string line;
  uint64_t c;
  bool find = false;
  bool cflag;

  num_query_ = 1;
  query_.clear();
  cflag = true;
  query_length = 0;
  location_.push_back(vector<uint64_t>());

  for (size_t y = 0; y < pattern.size(); y++)
  {
    query_length++;
    if ((c = esp_index_->GetCharToVar(pattern[y])) == DUMMYCODE)
    {
      cflag = false;
    }
    query_.push_back(c);
  }


  sum_query_length += query_length;

  if (cflag)
  {
    time = Gettime::get_time();
    find = FindEvidences();
    auto find_evidence_time = Gettime::get_time() - time;
    //std::cout << "find_evidence_time = " << (find_evidence_time * 1000) << "msec" << std::endl;
    sum_find_evidence_time += find_evidence_time;

    if (find)
    {

      occount_ = 0;
      time = Gettime::get_time();
      find_position_time += RepeatLocateQuery(evidences_[core_],
                                              core_,
                                              num_evidences_ - core_ - 1,
                                              0);
      auto traverse_tree_time = Gettime::get_time() - time;
      //std::cout << "traverse_tree_time = " << (traverse_tree_time * 1000) << "msec" << std::endl;
      sum_traverse_tree_time += traverse_tree_time;

      sum_occ += occount_;
      if (occount_ != 0)
      {

        //std::sort(location_[num_query_-1].begin(), location_[num_query_-1].end());
        for (size_t i = 0; i < location_[num_query_ - 1].size(); i++)
        {

          //cout << location_[num_query_ - 1][i] << " ";
          result.push_back(location_[num_query_ - 1][i]);
        }
      }
      else
      {
      }
    }
    else
    {
    }
  }
  else
  {
  }
  evidences_.clear();
  //line.clear();
  query_.clear();
  evidences_level_.clear();
  location_.pop_back();

  //std::cout << "occurrences = " << my::MyStrings::toIntegerString(*r) << std::endl;
  //my::MyStrings::print(*r);
  //return r;
}
void SearchQuery::GetPatterns(std::ifstream &pfs, vector<espstring>& output)
{
  auto vect = my::IO::loadVector<espchar>(pfs);
  //auto r = vector<espstring>();
  auto str = espstring();

  for (int i = 0; i <= vect->size(); i++)
  {
    espchar c = i == vect->size() ? 0 : vect->at(i);
    //std::cout << c << "/" << NEWLINECODE << std::endl;
    if (c == NEWLINECODE || i == vect->size())
    {
      output.push_back(str);
      str.clear();
    }
    else
    {
      str.push_back(c);
    }
  }
}