
# TST-index
This program is a C++ implementation of the TST-index.  
Our implementation is based on the idea in [https://arxiv.org/abs/1711.02855] 
and uses modified esp-index-I[https://github.com/tkbtkysms/esp-index-I].  

Let z be the number of factors in LZ77 factorization of T.  
Then the TST index of T uses O(z(q + log |T| log^{*} |T|)) space. 

# Compile
The source codes in 'module' directory are maintained in different repositories. 
So, to download all the necessary source codes, do the following:

> git clone https://github.com/TNishimoto/TSTESP.git  
> cd TSTESP  
> git submodule init  
> git submodule update  
> mkdir build  
> cd build   
> cmake ..  
> make  

# Usage
You can construct q-TST index using the following command.  
./build-index -i input_file -o output_file -q qgram_length
-i : the file path of input text T,  
-o : the file path of output data(q-TST index),   
-q : the value q in TST.  

You can decompress the original input text T using the following command.  
./decompress -i tst_index -o output_file  
-i : the file path of TST-index (without ".tst"),  
-o : the file path of the output.  

You can random access the original input text using the following command.  
./extract -i tst_index -o output_file  
-i : the file path of TST-index (without ".tst"),  
-p : the file path of a query file.  
The format of each line in the query file is "b e". these b and e represent the starting and end positions of the random access query. 
This program reads each query and prints the substring represented by the query in T. 

You can compute all occurrences of a pattern in the text T using the following command.  
./search -i tst_index -q query_file -m locate  
-i : the file path of TST-index (without ".tst"),  
-q : the file path of query file.  
(each line of query file represents a pattern P)  

You can count P in the text T using the following command.  
./search -i tst_index -q query_file -m count  
-i : the file path of TST-index (without ".tst"),  
-q : the file path of query file.  
(each line of query file represents a pattern P)  

# Example 
> cd build  
> ./build-index -i ../test/test.txt -o testTST -q 5  
> ./decompress -i testTST -o test_decompress.txt  
> ./extract -i testTST -p ../test/position.txt  
> ./search -i testTST -q ../test/query.txt -m locate  
> ./search -i testTST -q ../test/query.txt -m count  
