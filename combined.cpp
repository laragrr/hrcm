#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

//reference genome id and vectors
string reference_id;

vector<int> reference_line_length;           //length of each line

vector<int> reference_lowercase_pos;         //start position of lowercase substring
vector<int> reference_lowercase_length;      //length of lowercase substring

string reference_extracted;               	//reference sequence once all is extracted

// vector<int> reference_n_pos;                 //start position of N substring 
// vector<int> reference_n_length;              //length of N substring

// vector<char> reference_other_char;           //char other than: A,C,G,T found in reference
// vector<int> reference_other_pos;             //start position of N substring
// vector<int> reference_other_length;          //length of N substring


//to-be-compressed genome id and vectors 
string tbc_id;

vector<int> tbc_line_length;                 //length of each line

vector<int> tbc_lowercase_pos;               //start position of lowercase substring
vector<int> tbc_lowercase_length;            //length of lowercase substring

vector<int> tbc_n_pos;                       //start position of N substring 
vector<int> tbc_n_length;                    //length of N substring

vector<int> tbc_other_char;                 //char other than: A,C,G,T found in tbc
vector<int> tbc_other_pos;                   //start position of N substring
vector<int> tbc_other_length;                //length of N substring

string tbc_extracted;                        //to-be-compressed sequence once all is extracted

//tbc == to-be-compressed from now on

//function for extraction of information from reference sequence
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

//function for extraction of information from tbc sequence
void tbc_information_extraction(string seq_source) {
   string line;

   //try to open a file with a given name
   ifstream givenFile;
   givenFile.open(seq_source,ios::in);

   int checkpoint_i = 0;
   bool found_first = false;
   int length = 0;
   string helpful_string1="";

   // extract tbc lowercase information and make everything uppercase
   while(getline(givenFile,line)){

      //write down the id
      if(line[0]=='>'){
         tbc_id=line;
      }
      
      else{
         tbc_line_length.push_back(line.length());
         //for each character in a line
         for (int i = 0; i < line.length(); i++) {
            char char_at_i = line[i];
            
            //check if it is a lowercase
            if (char_at_i < 123 && char_at_i > 96) {
               if (!found_first) {
               checkpoint_i = length;
               found_first = true;
               tbc_lowercase_pos.push_back(checkpoint_i);
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
                  tbc_lowercase_length.push_back(length);
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
                  tbc_lowercase_length.push_back(length);
         }
         checkpoint_i = 0;
         found_first = false;
         length = 0;
   givenFile.close();

   // extract N character information
   for(int i=0;i<helpful_string1.length();i++){
      char char_at_i = helpful_string1[i];
            
         if (char_at_i =='N') {
            if (!found_first) {
            checkpoint_i = length;
            found_first = true;
            tbc_n_pos.push_back(checkpoint_i);
            length=1;
            } else {
               length++;
            }

         } else {
            if(found_first){
               found_first = false;
               tbc_n_length.push_back(length);
               length = 1;
               
            }else{
               length++;
            }
         }
      }
      if(found_first){
               tbc_n_length.push_back(length);
      }
      checkpoint_i = 0;
      found_first = false;
      length = 0;
   
   // extract other character information
   int length2=0;
   for(int i=0;i<helpful_string1.length();i++){
      char char_at_i = helpful_string1[i];
            
      if (char_at_i !='A' && char_at_i !='C' && char_at_i !='G' && char_at_i !='T' && char_at_i !='N') {
         if (!found_first) {
         checkpoint_i = length;
         found_first = true;
         tbc_other_char.push_back(char_at_i-'A');
         tbc_other_pos.push_back(length2);
         length=1;
         length2=0;
         } else {
            length++;
            tbc_other_char.push_back(char_at_i-'A');
            tbc_other_pos.push_back(length2);
            length2=0;
         }

      } else {
         if(found_first){
            found_first = false;
            tbc_other_length.push_back(length);
            length = 1;
            length2 = 1;
            tbc_extracted+=char_at_i;
         }else{
            length++;
            length2++;
            tbc_extracted+=char_at_i;
         }
      }
   }
   if(found_first){
      tbc_other_length.push_back(length);
   }
   tbc_extracted.erase(remove(tbc_extracted.begin(), tbc_extracted.end(), 'N'), tbc_extracted.end());    //remove N from tbc_extracted
}

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

//for line lengths and other one vector informations
string run_length_enc_int(vector<int> vec){
   string out;
   int counter=1;
   int last=vec[0];
   out+=to_string(vec[0]);
   for(int i=1;i<vec.size();i++){
      if(vec[i]==last){
         counter++;
      }else{
         out+=" "+to_string(counter);
         out+=" "+to_string(vec[i]);
         last=vec[i];
         counter=1;
      }
   }
   out+=" "+to_string(counter)+"\n";
   return out;
}

//je li zbilja potrebno???
string run_length_enc_id(string ID){
   string out;
   
   return out;
}

//can be used for lowercase(pos, length), n(pos,length) and special characters(pos, spec_char)
string enc_length(vector<int> pos, vector<int> length){
   string out;
   for(int i=0;i<pos.size()-1;i++){
      out+=to_string(pos[i])+" "+to_string(length[i])+" ";
   }
   out+=to_string(pos[pos.size()-1])+" "+to_string(length[pos.size()-1])+"\n"; 
   return out;
}

//saving mismatched information
string saveMatch(int pos, int length, string misMatch){
   string output="";
   if(!misMatch.empty()){
      //razmisli još jednom o ovom formatu, najviše zbog zapisa i dekompresije
      output+=to_string(pos)+" "+to_string(length)+" "+misMatch+"\n";
   }
   return output;
}

//saving information from First-level Matching
string saveFirstMatch(vector<int> pos,vector<int> length,vector<string> misMatch){
   string output="";
   for(int i=0;i<pos.size();i++){
      output+=saveMatch(pos[i],length[i],misMatch[i]);
   }
   output+="\n";
   return output;
}

int main(void) {

   string tbc_file;
   string ref_file;
   cout<<"Please enter a reference genome file name: ";
   cin>>ref_file;
   cout<<"Please enter a to-be-compressed genome file name: ";
   cin>>tbc_file;


   tbc_information_extraction(tbc_file);
   reference_information_extraction(ref_file);
   // std::cout<<"Extracted ref:\n";
   // for(char i:reference_extracted){
   //    std::cout<<i;
   // }
   // std::cout<<"\nLengths of lines:\n";
   // for (int i:reference_line_length)
   // {
   //     std::cout<<i<<", ";
   // }
   // std::cout<<"\nLowercase pos:\n";
   // for (int i:reference_lowercase_pos)
   // {
   //     std::cout<<i<<", ";
   // }
   // std::cout<<"\nLowercase lengths:\n";
   // for (int i:reference_lowercase_length)
   // {
   //     std::cout<<i<<", ";
   // }
   // std::cout<<"\nExtracted tbc:\n";
   // for(char i:tbc_extracted){
   //    std::cout<<i;
   // }
   // std::cout<<"\nLengths of lines:\n";
   // for (int i:tbc_line_length)
   // {
   //     std::cout<<i<<", ";
   // }
   // std::cout<<"\nLowercase pos:\n";
   // for (int i:tbc_lowercase_pos)
   // {
   //     std::cout<<i<<", ";
   // }
   // std::cout<<"\nLowercase lengths:\n";
   // for (int i:tbc_lowercase_length)
   // {
   //     std::cout<<i<<", ";
   // }
   // std::cout<<"\nN pos:\n";
   // for (int i:tbc_n_pos)
   // {
   //     std::cout<<i<<", ";
   // }
   // std::cout<<"\nN lengths:\n";
   // for (int i:tbc_n_length)
   // {
   //     std::cout<<i<<", ";
   // }
   // std::cout<<"\nOther pos:\n";
   // for (int i:tbc_other_pos)
   // {
   //     std::cout<<i<<", ";
   // }
   // std::cout<<"\nOther char:\n";
   // for (int i:tbc_other_char)
   // {
   //     std::cout<<i<<", ";
   // }
   // std::cout<<"\nOther lengths:\n";
   // for (int i:tbc_other_length)
   // {
   //     std::cout<<i<<", ";
   // }

   string id=tbc_id+"\n";
   string line_lengths=run_length_enc_int(tbc_line_length)+"\n";
    
   //dodati duljinu cijelog tbc genoma

   string n_info=enc_length(tbc_n_pos,tbc_n_length)+"\n";
   string spec_info=enc_length(tbc_other_pos,tbc_other_char)+"\n";

   //ZA SAD KAO PROBA
   string lowercase_info=enc_length(tbc_lowercase_pos,tbc_lowercase_length)+"\n";

   

   ofstream outputfile;
   outputfile.open("output.txt");
   outputfile<<id;
   outputfile<<line_lengths;
   outputfile<<n_info;
   outputfile<<spec_info;
   outputfile<<lowercase_info;

   outputfile.close();
   system("7z a -m0=PPMd output.7a output.txt");
   return 0;
}