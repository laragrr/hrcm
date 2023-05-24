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

//can be used for lowercase(pos, length), n(pos, length) and special characters(pos, spec_char)
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
string saveFirstMatch(vector<tuple<int,int>> matching_segments,vector<string> misMatch){
   string output="";
   for(int i=0;i<matching_segments.size();i++){
      output+=saveMatch(get<0>(matching_segments[i]),get<1>(matching_segments[i]),misMatch[i]);
   }
   output+="\n";
   return output;
}

int main(void){

   string placeholder_id="";
   string placeholder_rlc="";

   ofstream outputfile;
   outputfile.open("output.fa");
   outputfile<<placeholder_id<<"\n";
   outputfile<<placeholder_rlc;

   outputfile.close();
   system("7za a -m0=PPMd output.7a output.fa");
   return 0;
}