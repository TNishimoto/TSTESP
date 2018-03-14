
#include"esp_tree.hpp"

using namespace std;
void ESPTree::ReadInputText(ifstream &ifs)
{

  vector<espchar> char_buffer;
  uint64_t input_length = 0;

  ifs.seekg(0, ios::end);
  input_length = ifs.tellg();
  ifs.seekg(0, ios::beg);

  remaining_len_ = input_length / sizeof(espchar);

  input_buffer_.resize(remaining_len_);
  vector<uint64_t>(input_buffer_).swap(input_buffer_);

  char_buffer.resize(remaining_len_);

  vector<espchar>(char_buffer).swap(char_buffer);
  ifs.read((char *)&char_buffer[0], input_length);
  for (uint64_t i = 0; i < remaining_len_; i++)
  {
    //"char_map.hppをsuffix treeの変換に書き換えればよい。"
    input_buffer_[i] = char_map_.char_to_var(char_buffer[i]);
  }
  ifs.close();
  vector<espchar>().swap(char_buffer);
}