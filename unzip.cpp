#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

int main(void){
   
   string compressed_file;
   string ref_file;

   //get ziped genom file name
   std::cout<<"Please enter the name of compressed genome file you wish to unzip: ";
   cin>>compressed_file;

   //command for unziping
   string command="7z e "+string(compressed_file);

   std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
   //system command to unzip
   system(command.c_str());
   std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
   std::cout << "\nTime difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
   
   return 0;
}