
#include "esp_index.hpp"

using namespace std;
using namespace rsdic;

void ESPIndex::Decompress(ofstream &ofs){  
  vector<uint64_t> tmp_left;
  vector<uint64_t> tmp_right;
  tmp_left.resize(num_prules());
  tmp_right.resize(num_prules());
  for(uint64_t i = 0; i < num_prules();i++){
    tmp_left[i] = DUMMYCODE;
    tmp_right[i] = DUMMYCODE;
  }
  RepeatDecompress(root(), 
	 ofs,
	 tmp_left,
	 tmp_right);
  ofs.close();
}
uint64_t ESPIndex::GetCharToVar(const espchar kCharacter){
  //
    if (kCharacter < CONSTANT::CHARMAX)
    {
      return char_map_.char_to_var(kCharacter);
    }
    else
    {
      return DUMMYCODE;
  }

  //return char_map_.char_to_var(kCharacter);
}
void ESPIndex::RepeatDecompress(const uint64_t kIndex, 
				ofstream &ofs,
				vector<uint64_t>&tmp_left,
				vector<uint64_t> &tmp_right){
  if(kIndex < CONSTANT::CHARMAX){
    auto character = char_map_.var_to_char(kIndex);
    ofs.write(reinterpret_cast<const char *>(&character), 1 * sizeof(espchar));
    //ofs << char_map_.var_to_char(kIndex);
  }
  else{
    if(tmp_left[kIndex] == DUMMYCODE){
      tmp_left[kIndex] = left(kIndex);
    }
    RepeatDecompress(tmp_left[kIndex],
		     ofs,
		     tmp_left,
		     tmp_right);
    if(tmp_right[kIndex] == DUMMYCODE){
      tmp_right[kIndex] = right(kIndex);
    }
    RepeatDecompress(tmp_right[kIndex],
		     ofs,
		     tmp_left,
		     tmp_right);
  }
}
void ESPIndex::buildESP(ifstream& input, ofstream& espStream, uint32_t pr){ 
        ESPTree esp_tree;
        ESPIndex esp_index; 
        double time;
        time = Gettime::get_time();
        esp_tree.Build(input);
        esp_index.Build(esp_tree, pr, espStream);
        time = Gettime::get_time() - time;

        cout << "Build index time: " << time << " sec" << endl;   
    }