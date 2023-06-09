#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <chrono>

using namespace std;

string reference_id;
int reference_length;

vector<int> reference_line_length;           //length of each line

vector<int> reference_lowercase_pos;         //start position of lowercase substring
vector<int> reference_lowercase_length;      //length of lowercase substring

string reference_extracted;               	//reference sequence once all is extracted

//to-be-decompressed genome id and vectors 
string tbd_id;
int tbd_length;

vector<int> tbd_line_length;                 //length of each line

vector<int> tbd_lowercase_pos;               //start position of lowercase substring
vector<int> tbd_lowercase_length;            //length of lowercase substring

vector<int> tbd_n_pos;                       //start position of N substring 
vector<int> tbd_n_length;                    //length of N substring

vector<int> tbd_other_char;                  //char other than: A,C,G,T found in tbc
vector<int> tbd_other_pos;                   //start position of N substring
vector<int> tbd_other_length;                //length of N substring

vector<int> low_loc;                         //vector for where ref and tbc have same lowercase info
vector<int> diff_low_pos;                    //vector for where ref and tbc have different lowercase info - position
vector<int> diff_low_len;                    //vector for where ref and tbc have different lowercase info - length

string tbd_extracted;                        //to-be-decompressed sequence once all is extracted

string tbd_upper;                            //vector that will have to be run through u_to_l to return lowercase

string tbd_decompressed;                      //original 

// written by Lara Grgurić
vector<vector<pair<int, int>>> seq_match_results;  // vector of match results for every sequence
vector<vector<string>> seq_mism_chars;
///////

//tbd == to-be-decompressed from now on
///Writen by Paulo Erak
void reference_information_extraction(string seq_source) {

   string line;

   //try to open a file with a given name
   ifstream givenFile;
   givenFile.open(seq_source,ios::in);

   int checkpoint_i = 0;
   bool found_first = false;
   int length = 0;
   string helpful_string1="";
   reference_length=0;

   // extract reference lowercase information and make everything uppercase
   while(getline(givenFile,line)){

      //write down the id
      if(line[0]=='>'){
         reference_id=line;
      }
      
      else{
         reference_length+=line.length();
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

//return original lowercase
void u_to_l(){
   string returned;
   int counter_u=0;
   int counter_l=0;
   int last_l=0;
   

   while(returned.length()<tbd_length){
      if((last_l+tbd_lowercase_pos[counter_l]>returned.length() || counter_l==tbd_lowercase_pos.size()) && counter_u<tbd_length){
         returned+=tbd_upper[counter_u];
         counter_u++;
      }
      else if (last_l+tbd_lowercase_pos[counter_l]==returned.length() && counter_l<tbd_lowercase_pos.size())
      {
         last_l=returned.length()+tbd_lowercase_length[counter_l];
         for(int i=0;i<tbd_lowercase_length[counter_l];i++){
            returned+=tolower(tbd_upper[counter_u]);
            counter_u++;
         }
         counter_l++;
      }
      else{
         std::cout<<"Something went wrong during returning.";
         std::cout<<returned;
         std::cout<<"\n";
         exit(-1);
      }
      //cout<<"Test returned: "+returned+"\n";
   }
   
   tbd_decompressed=returned;
   
}

//return original N and other characters
void n_spec_returned(){
   string returned;
   int counter_e=0;
   int counter_n=0;
   int last_n=0;
   int counter_s=0;
   int last_s=0;

   while(returned.length()<tbd_length){
      if((last_n+tbd_n_pos[counter_n]>returned.length() || counter_n==tbd_n_pos.size()) && (last_s+tbd_other_pos[counter_s]>returned.length() || counter_s==tbd_other_pos.size()) && counter_e<tbd_length){
         returned+=tbd_extracted[counter_e];
         counter_e++;
      }
      else if (last_n+tbd_n_pos[counter_n]==returned.length() && counter_n<tbd_n_pos.size())
      {
         last_n=returned.length()+tbd_n_length[counter_n];
         for(int i=0;i<tbd_n_length[counter_n];i++){
            returned+="N";
         }
         counter_n++;
      }
      else if (last_s+tbd_other_pos[counter_s]==returned.length() && counter_s<tbd_other_pos.size())
      {
         last_s=returned.length()+1;
         returned+=(tbd_other_char[counter_s]+'A');
         counter_s++;
      }
      else{
         std::cout<<"Something went wrong during returning.";
         std::cout<<returned;
         std::cout<<"\n";
         exit(-1);
      }
   }
   
   tbd_upper=returned;
}

////return original N characters
void n_returned(){
   string returned;
   int counter_e=0;
   int counter_n=0;
   int last_n=0;

   while(returned.length()<tbd_length){
      if((last_n+tbd_n_pos[counter_n]>returned.length() || counter_n==tbd_n_pos.size()) && counter_e<tbd_length){
         returned+=tbd_extracted[counter_e];
         counter_e++;
      }
      else if (last_n+tbd_n_pos[counter_n]==returned.length() && counter_n<tbd_n_pos.size())
      {
         last_n=returned.length()+tbd_n_length[counter_n];
         for(int i=0;i<tbd_n_length[counter_n];i++){
            returned+="N";
         }
         counter_n++;
      }
      else{
         std::cout<<"Something went wrong during returning.";
         std::cout<<returned;
         std::cout<<"\n";
         exit(-1);
      }
      //cout<<"Test returned: "+returned+"\n";
   }
   tbd_upper=returned;
}

//return original other characters
void spec_returned(){
   string returned;
   int counter_e=0;
   int counter_s=0;
   int last_s=0;

   while(returned.length()<tbd_length){
      if((last_s+tbd_other_pos[counter_s]>returned.length() || counter_s==tbd_other_pos.size()) && counter_e<tbd_length){
         returned+=tbd_extracted[counter_e];
         counter_e++;
      }
      else if (last_s+tbd_other_pos[counter_s]==returned.length() && counter_s<tbd_other_pos.size())
      {
         last_s=returned.length()+1;
         returned+=(tbd_other_char[counter_s]+'A');
         counter_s++;
      }
      else{
         std::cout<<"Something went wrong during returning.";
         std::cout<<returned;
         std::cout<<"\n";
         exit(-1);
      }
   }
   tbd_upper=returned;
}


//reverse run-length-encoding
vector<int> rev_rle_int(vector<int> vec){
   vector<int> normal;
   int value;
   int counter;
   for(int i=0;i<vec.size();i+=2){
      value=vec[i];   
      counter=vec[i+1];
      for(int j=0;j<counter;j++){
         normal.push_back(value);
      }
   }
   return normal;
}

//separate given string and store positions
vector<int> separate_pos(vector<int> vec){
   vector<int> pos;

   for(int i=0;i<vec.size();i+=2){
      pos.push_back(vec[i]);
   }

   return pos;
}

//separate given string and store lengths
vector<int> separate_len(vector<int> vec){
   vector<int> len;

   for(int i=1;i<vec.size();i+=2){
      len.push_back(vec[i]);
   }

   return len;
}

//take a string and return vector of numbers
vector<int> unpack(string line){
   vector<int> numbers;
   stringstream ss(line);

   
   string word;
   vector<string> str_numbers;
   while(ss>>word){
      str_numbers.push_back(word);
   }
   
   for(int i=0;i<str_numbers.size();i++){
      numbers.push_back(stoi(str_numbers[i]));
   }
   return numbers;
}

// lowercase character matching
void lowercase_char_matching(){
   int z=0;
   int pos,loc;
   for(int i=0;i<low_loc.size();i++){
      if(low_loc[i]==-1){
         pos=diff_low_pos[z];
         tbd_lowercase_pos.push_back(pos);
         loc=diff_low_len[z];
         tbd_lowercase_length.push_back(loc);
         z++;
      }
      else if(low_loc[i]>=0){
         tbd_lowercase_pos.push_back(reference_lowercase_pos[low_loc[i]]);
         tbd_lowercase_length.push_back(reference_lowercase_length[low_loc[i]]);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
// written by Lara Grgurić

// check if a string is a number
bool isNumber(string str) {
   return !str.empty() && find_if(str.begin(), str.end(), 
   [](unsigned char c) { return !isdigit(c);}) == str.end();
}

// second level decompress
void second_level_decompress(int &seq_id, int &pos, int &len, vector<vector<pair<int, int>>> &seq_match_results, vector<vector<string>> &seq_mism_chars, vector<pair<int, int>> &matching_segments_seq, vector<string> &mismatched_chars_seq) {
   for (int i = pos; i < pos + len; i++) {
      matching_segments_seq.push_back(seq_match_results[seq_id][i]);
      mismatched_chars_seq.push_back(seq_mism_chars[seq_id][i]);
   }
}

// first level decompress
string first_level_decompress(vector<vector<pair<int, int>>> &seq_match_results, vector<vector<string>> &seq_mism_chars, int seq_id) {
   //for (int seq_id = 0; seq_id < seq_match_results.size(); seq_id++) {
   string line = "";

   for (int i = 0; i < seq_match_results[seq_id].size(); i++) {
      auto [pos, len] = seq_match_results[seq_id][i];
      string mism_chars = seq_mism_chars[seq_id][i];
      for (int j = pos; j < pos+len; j++) {
         line += reference_extracted[j];
      }
      line += mism_chars;
   }

   // ofstream newfile;
   // string filename = "tbc" + to_string(seq_id) + ".txt";
   // newfile.open(filename);
   // newfile << line;
   // newfile.close();
   return line;
   
}

//decoding first and second level matching results
string readingFile(int seq_id, string line) {
   //string tbc_first_level;
   //getline(myfile, tbc_first_level);
   vector<pair<int, int>> matching_segments_first_seq;
   vector<string> mismatched_chars_first_seq;
   int a, b;
   string c;   
   //getline(myfile, line);

   if (!seq_id) {
      // reading first level matching result of 0th tbc sequence
      std::istringstream iss(line);
      while ((iss >> a >> b >> c)) { 
         matching_segments_first_seq.push_back(make_pair(a,b));
         mismatched_chars_first_seq.push_back(c);
      }
   } else {
      // reading second level matching result of other tbc sequences
      std::istringstream iss(line);
      while ((iss >> a >> b >> c)) { 

         // getting first level matching results from second level
         if (isNumber(c)) {
            int len = stoi(c);
            second_level_decompress(a, b, len, seq_match_results, seq_mism_chars, matching_segments_first_seq, mismatched_chars_first_seq);
         } else {
            matching_segments_first_seq.push_back(make_pair(a,b));
            mismatched_chars_first_seq.push_back(c);
         }
      }
   }
   
   seq_match_results.push_back(matching_segments_first_seq);
   seq_mism_chars.push_back(mismatched_chars_first_seq);
   
   return first_level_decompress(seq_match_results, seq_mism_chars, seq_id);


   // reading second level matching results
   // while (getline(myfile, line)) {
   //    matching_segments_first_seq.clear();
   //    mismatched_chars_first_seq.clear();
}


///////////////////////////////////////////////////////////////////////////////
///Writen by Paulo Erak
//write sequence to file
void write_down(int file_number){
   string file_name="final_"+to_string(file_number)+".fa";
   ofstream myfile;
	myfile.open(file_name);
   myfile << tbd_id << endl;
   string leftover=tbd_decompressed;
   for(int i=0;i<tbd_line_length.size();i++){
      myfile<<leftover.substr(0,tbd_line_length[i])<<endl;
      if(leftover.size()!=tbd_line_length[i]){
         leftover=leftover.substr(tbd_line_length[i],leftover.size()-1);
      }
      
   }
   myfile.close();

}

//reconstruct a given compressed sequence
void construct(string decompressed){
   string line;
   ifstream targetFile(decompressed);
   int seq_id = 0;
   while (!targetFile.eof()) {
   //string id="";
   //string length="";
   //vector<int> lengths;
   vector<int> n_info;
   vector<int> spec_info;
   vector<int> lowercase_info;


   
   getline(targetFile,tbd_id);
   getline(targetFile,line);
   tbd_length=stoi(line);
   getline(targetFile,line);
   tbd_line_length=unpack(line);
   tbd_line_length=rev_rle_int(tbd_line_length);
   getline(targetFile,line);
   n_info=unpack(line);
   tbd_n_pos=separate_pos(n_info);
   tbd_n_length= separate_len(n_info);
   getline(targetFile,line);
   spec_info=unpack(line);
   tbd_other_pos=separate_pos(spec_info);
   tbd_other_char= separate_len(spec_info);



   getline(targetFile,line);
   lowercase_info=unpack(line);
   low_loc=rev_rle_int(lowercase_info);
   getline(targetFile,line);
   lowercase_info=unpack(line);
   diff_low_pos= separate_pos(lowercase_info);
   diff_low_len= separate_len(lowercase_info);
   lowercase_char_matching();

   ////////////////////////////////////////////////////
   // Written by Lara Grgurić
   getline(targetFile, line);
   tbd_extracted = readingFile(seq_id, line);
   string extracted=tbd_extracted;

   ////////////////////////////////////////////////////
   ///Writen by Paulo Erak
   int s=tbd_n_pos.size();
   int s2=tbd_other_pos.size();

   if(s>0 && s2>0){
      n_spec_returned();
   }
   else if(s>0 && s2==0){
      n_returned();
   }
   else if(s==0 && s2>0){
      spec_returned();
   }
   else{
      tbd_upper=tbd_extracted;
   }

   s=low_loc.size();
   if(s>0){
      u_to_l();
   }
   else{
      tbd_decompressed=tbd_upper;
   }
   
   write_down(seq_id);

   seq_id++;
   }

}
int main(int argc, char** argv){
   string compressed_file;
   string ref_file;

   //get reference genom file name
   // std::cout<<"Please enter a reference genome file name: ";
   // cin>>ref_file;

   if (argc != 3)
      return std::cout << "Usage: a.exe <ref_file> <cpf>" << std::endl, 1;

   ref_file = argv[1];
   reference_information_extraction(ref_file);

   //get compressed genom file name
   // std::cout<<"Please enter the name of compressed genome file you wish to decompress: ";
   // cin>>compressed_file;

   compressed_file = argv[2];

   std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

   //start to construct the original genome
   construct(compressed_file);

   std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
   std::cout << "\nTime difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
   
   return 0;
}