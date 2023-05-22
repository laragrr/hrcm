#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

string reference_id;

vector<int> reference_line_length;           //length of each line

vector<int> reference_lowercase_pos;         //start position of lowercase substring
vector<int> reference_lowercase_length;      //length of lowercase substring

string reference_extracted;               	//reference sequence once all is extracted

//to-be-decompressed genome id and vectors 
string tbd_id;

vector<int> tbd_line_length;                 //length of each line

vector<int> tbd_lowercase_pos;               //start position of lowercase substring
vector<int> tbd_lowercase_length;            //length of lowercase substring

vector<int> tbd_n_pos;                       //start position of N substring 
vector<int> tbd_n_length;                    //length of N substring

vector<int> tbd_other_char;                  //char other than: A,C,G,T found in tbc
vector<int> tbd_other_pos;                   //start position of N substring
vector<int> tbd_other_length;                //length of N substring

string tbd_extracted;                        //to-be-decompressed sequence once all is extracted

//tbd == to-be-decompressed from now on

void reference_information_extraction(string seq_source) {

   string line;

   //try to open a file with a given name
   ifstream givenFile;
   givenFile.open(seq_source,ios::in);

   int checkpoint_i = 0;
   bool found_first = false;
   int length = 0;
   string helpful_string1="";

   // extract reference lowercase information and make everything uppercase
   while(getline(givenFile,line)){

      //write down the id
      if(line[0]=='>'){
         reference_id=line;
      }
      
      else{
         reference_line_length.push_back(line.length());
         //for each character in a line
         for (int i = 0; i < line.length(); i++) {
            char char_at_i = line[i];
            
            //check if it is a lowercase
            if (char_at_i < 123 && char_at_i > 96) {
               if (!found_first) {
               checkpoint_i = length;
               found_first = true;
               reference_lowercase_pos.push_back(checkpoint_i);
               length=1;
               //change it to uppercase and record it
               helpful_string1+=toupper(char_at_i);
               } else {
                  length++;
                  //change it to uppercase and record it
                  helpful_string1+=toupper(char_at_i);
               }

            //else it is an uppercase
            } else {
               if(found_first){
                  found_first = false;
                  reference_lowercase_length.push_back(length);
                  length = 1;
                  helpful_string1+=char_at_i;
               }else{
                  length++;
                  helpful_string1+=char_at_i;
               }
            }
         }
         
      }
   }
   if(found_first){
         reference_lowercase_length.push_back(length);
   }
   givenFile.close();

   // remove N and characters other than A,C,G,T 
   for(int i=0;i<helpful_string1.length();i++){
      char char_at_i = helpful_string1[i];
            
      if (char_at_i =='A' || char_at_i =='C' || char_at_i =='G' || char_at_i =='T') {
         reference_extracted+=char_at_i;
      }
   }
}

void construct(string decompressed){
   string line;
   ifstream targetFile(decompressed);
   
   string id="";
   string lengths="";
   string n_info="";
   string spec_info="";



}



int main(void){
   
   string compressed_file;
   string ref_file;

   //get reference genom file name
   cout<<"Please enter a reference genome file name: ";
   cin>>ref_file;

   //get compressed genom file name
   cout<<"Please enter the name of compressed genome file you wish to decompress: ";
   cin>>compressed_file;

   //decompress file
   string command="7z e"+compressed_file;
   system(command.c_str());

   //get decompressed file name
   string decompressed_file=compressed_file;
   decompressed_file.erase(compressed_file.length()-2);
   //TXT ZA SAD!!!!!!!!!!!!!!
   decompressed_file+="txt";

   //start to construct the original genome
   construct(decompressed_file);






   return 0;
}