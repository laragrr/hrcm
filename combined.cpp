#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <unordered_map>
#include <sstream>
#include <cassert>

using namespace std;

//keeps countbof number of sequences
int seq_id=-1;

//reference genome id and vectors
string reference_id;
int reference_length;

vector<int> reference_line_length;           //length of each line

vector<int> reference_lowercase_pos;         //start position of lowercase substring
vector<int> reference_lowercase_length;      //length of lowercase substring

string reference_extracted;               	//reference sequence once all is extracted

//to-be-compressed genome id and vectors 
vector<string> tbc_id_vec;
vector<int> tbc_length_vec;

vector<vector<int>> tbc_line_length_vec;                 //length of each line

vector<vector<int>> tbc_lowercase_pos_vec;               //start position of lowercase substring
vector<vector<int>> tbc_lowercase_length_vec;            //length of lowercase substring

vector<vector<int>> tbc_n_pos_vec;                       //start position of N substring 
vector<vector<int>> tbc_n_length_vec;                    //length of N substring

vector<vector<int>> tbc_other_char_vec;                 //char other than: A,C,G,T found in tbc
vector<vector<int>> tbc_other_pos_vec;                   //start position of N substring
vector<vector<int>> tbc_other_length_vec;                //length of N substringž

vector<vector<int>> low_loc;                                   //vector for where ref and tbc have same lowercase info
vector<vector<int>> diff_low_pos;                              //vector for where ref and tbc have different lowercase info - position
vector<vector<int>> diff_low_len;                              //vector for where ref and tbc have different lowercase info - length


string tbc_extracted;                        //to-be-compressed sequence once all is extracted

//tbc == to-be-compressed from now on

//function for extraction of information from reference sequence
string reference_information_extraction(string seq_source) {

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

   return reference_extracted;
}

//function for extraction of information from tbc sequence
string tbc_information_extraction(string seq_source) {
   string line;

   //try to open a file with a given name
   ifstream givenFile;
   givenFile.open(seq_source,ios::in);

   int checkpoint_i = 0;
   bool found_first = false;
   int length = 0;
   string helpful_string1="";
   tbc_length_vec.push_back(0);
   tbc_extracted = "";

   vector<int> temp_vec1;
   tbc_line_length_vec.push_back(temp_vec1);
   vector<int> temp_vec2;
   tbc_lowercase_pos_vec.push_back(temp_vec2);
   vector<int> temp_vec3;
   tbc_lowercase_length_vec.push_back(temp_vec3);
   vector<int> temp_vec4;
   tbc_n_pos_vec.push_back(temp_vec4);
   vector<int> temp_vec5;
   tbc_n_length_vec.push_back(temp_vec5);
   vector<int> temp_vec6;
   tbc_other_char_vec.push_back(temp_vec6);
   vector<int> temp_vec7;
   tbc_other_pos_vec.push_back(temp_vec7);
   vector<int> temp_vec8;
   tbc_other_length_vec.push_back(temp_vec8);

   // extract tbc lowercase information and make everything uppercase
   while(getline(givenFile,line)){

      //write down the id
      if(line[0]=='>'){
         tbc_id_vec.push_back(line);
         seq_id++;
      }
      
      else{
         tbc_length_vec[seq_id]+=line.length();
         tbc_line_length_vec[seq_id].push_back(line.length());
         //for each character in a line
         for (int i = 0; i < line.length(); i++) {
            char char_at_i = line[i];
            
            //check if it is a lowercase
            if (char_at_i < 123 && char_at_i > 96) {
               if (!found_first) {
               checkpoint_i = length;
               found_first = true;
               tbc_lowercase_pos_vec[seq_id].push_back(checkpoint_i);
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
                  tbc_lowercase_length_vec[seq_id].push_back(length);
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
                  tbc_lowercase_length_vec[seq_id].push_back(length);
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
            tbc_n_pos_vec[seq_id].push_back(checkpoint_i);
            length=1;
            } else {
               length++;
            }

         } else {
            if(found_first){
               found_first = false;
               tbc_n_length_vec[seq_id].push_back(length);
               length = 1;
               
            }else{
               length++;
            }
         }
      }
      if(found_first){
               tbc_n_length_vec[seq_id].push_back(length);
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
         tbc_other_char_vec[seq_id].push_back(char_at_i-'A');
         tbc_other_pos_vec[seq_id].push_back(length2);
         length=1;
         length2=0;
         } else {
            length++;
            tbc_other_char_vec[seq_id].push_back(char_at_i-'A');
            tbc_other_pos_vec[seq_id].push_back(length2);
            length2=0;
         }

      } else {
         if(found_first){
            found_first = false;
            tbc_other_length_vec[seq_id].push_back(length);
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
      tbc_other_length_vec[seq_id].push_back(length);
   }
   string ex=tbc_extracted;
   tbc_extracted.erase(remove(tbc_extracted.begin(), tbc_extracted.end(), 'N'), tbc_extracted.end());    //remove N from tbc_extracted
   return tbc_extracted;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
string saveMatch(tuple<int, int> matching_segment, string misMatch){
   string output="";
   if(!misMatch.empty()){
      //razmisli još jednom o ovom formatu, najviše zbog zapisa i dekompresije
      output+=to_string(get<0>(matching_segment))+" "+to_string(get<1>(matching_segment))+" "+misMatch+"\n";
   }
   return output;
}

// lowercase character matching

void lowercase_char_matching(int seq_id)
{
   vector<int> new_one;
   for(int i=0;i<tbc_lowercase_length_vec[seq_id].size();i++){
      new_one.push_back(-1);
   }
   vector<int> new_one_diff_pos;
   vector<int> new_one_diff_len;
   
	int start_position = 0;
	int _diff_low_len = 0;

	for (int i = 0; i < tbc_lowercase_length_vec[seq_id].size(); i++)
	{
	//search from the start_position to the end
	for (int j = start_position; j < reference_lowercase_length.size(); j++)
	{
		if ((tbc_lowercase_pos_vec[seq_id][i] == reference_lowercase_pos[j]) && (tbc_lowercase_length_vec[seq_id][i] == reference_lowercase_length[j]))
		{
			new_one[i] = j;
			start_position = j + 1;
			break;
		}
	}

	//search from the start_position to the begin
	if (new_one[i] == -1)
	{
		for (int j = start_position - 1; j > 0; j--) {
			if ((tbc_lowercase_pos_vec[seq_id][i] == reference_lowercase_pos[j]) && (tbc_lowercase_pos_vec[seq_id][i] == reference_lowercase_length[j]))
			{
				new_one[i] = j;
				start_position = j + 1;
				break;
			}
		}
	}

	//record the mismatched information
	if (new_one[i] == -1)
	{
		new_one_diff_pos.push_back(tbc_lowercase_pos_vec[seq_id][i]);
		new_one_diff_len.push_back(tbc_lowercase_length_vec[seq_id][i]);
	}
	}
   low_loc.push_back(new_one);
   diff_low_pos.push_back(new_one_diff_pos);
   diff_low_len.push_back(new_one_diff_len);
}

void print(vector<int> const &input) {
   for (auto const &i: input) {
      std::cout << i << " ";
   }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Written by Lara Grgurić

int k = 12;
unordered_map<long, int> H;
unordered_map<int, int> L;
vector<pair<int, int>> matching_segments;
vector<string> mismatched_chars;
int length = 0, percent = 10;

vector<unordered_map<long, int>> H_tbc_seq;  // vector of hash tables for new referential sequences
vector<unordered_map<int, long>> L_tbc_seq;

// Polynomial rolling hash function
struct Hash_f {
   long long p = 31, m = 1e9 + 7;
   long long hash_value; 
   Hash_f() {}
   Hash_f(const string& s)
   {
      long long hash_so_far = 0;
      long long p_pow = 1;
      const long long n = s.length();
      for (long long i = 0; i < n; ++i) {
         hash_so_far
               = (hash_so_far + encode(s[i]) * p_pow)
               % m;
         p_pow = (p_pow * p) % m;
      }
      hash_value = hash_so_far;
   }
   int encode(char c) {
      switch (c)
      {
      case 'A':
         return 1;
      case 'C':
         return 2;
      case 'G':
         return 3;
      case 'T':
         return 4;
      default:
         return 0;
      }
   }
   bool operator==(const Hash_f& other)
   {
      return (hash_value == other.hash_value);
   }
};

// B sequence matching
void creating_hash_table(string ref_seq) {

   // create hash table for reference B sequence
   for (int i = 0; i < ref_seq.length(); i++) {
      string k_mer = "";
      for (int j = 0; j < k; j++) {
         k_mer += ref_seq[i+j];
      }
      Hash_f hash_func = Hash_f(k_mer);
      long hash_value = hash_func.hash_value;
      if (H.count(hash_value)) {
         L.insert(pair<int, int>(i, H[hash_value]));
         H[hash_value] = i;
      } else {
         H.insert(pair<long, int>(hash_value, i));
      }
   }
}


void first_level_matching(string tbc_seq) {
   vector<int> idx;
   int i;
   string mismatched_string;
   int beginning_idx = 0;
   for (i = 0; i < tbc_seq.length() - k; i++) {
      string k_mer = "";
      for (int j = 0; j < k; j++) {
   k_mer += tbc_seq[i+j];
      }
      Hash_f hash_func = Hash_f(k_mer);

      long hash_value = hash_func.hash_value;
      
      if (H.count(hash_value)) {
         // match
         if (mismatched_string.length() > 0) mismatched_chars.push_back(mismatched_string);
         mismatched_string = "";

         if (length == 0) length = k-1;

         // searching for all positions with the same hash value
         vector<int> temp;
         temp.push_back(H[hash_value]);
         while (L.count(temp.back())) {
            temp.push_back(L[temp.back()]);
         }

         if (idx.size() > 0) {
            // filter vector of new indexes
            vector<int> other;
            copy_if(temp.begin(), temp.end(), back_inserter(other), [idx](int const& value) {
               bool found = false;
               for (auto const &i : idx) {
                  if (value == i+1) {
                     found = true;
                     break;
                  }
               }
               return found;
            });
            beginning_idx = idx[0] - length + k;
            idx = other;

            if (!idx.size()) {
               // recording position and length     
               matching_segments.push_back(make_pair(beginning_idx, length));
               mismatched_string += k_mer[k_mer.length()-1];
               i += k-1;
               length = 0;
            } else {
               length++;
            }
         } else {
            idx = temp;
            length++;
         }

      } else {
         // mismatch
         // (position, length) = (idx[0] - (length-k), length)

         if (length != 0) {
            // end of matching segment
            // recording position and length     
            matching_segments.push_back(make_pair(idx[0] - (length-k), length));
            mismatched_string += k_mer[k_mer.length()-1];
            i += k-1;
            length = 0;
            idx.clear();
         } else {
            // multiple mismatching characters
            mismatched_string += k_mer[0];
            if (!i) {
               matching_segments.push_back(make_pair(0, 0));
            }
         }
      }
   }
   string final = "";
   if (length != 0) {
            // end of matching segment
            // recording position and length     
            matching_segments.push_back(make_pair(idx[0] - (length-k), length));
            i += k;
            length = 0;
            idx.clear();
   }
   else{
      if(mismatched_string.length()){
         final=mismatched_string;
         i++;
      }
   }
   for (--i; i < tbc_seq.length(); i++) {
      final += tbc_seq[i];
   }
   std::cout<<final;
   mismatched_chars.push_back(final);
}


// hash table for to-be-compressed sequence
void create_hash_table_tbc_seq(vector<pair<int,int>> pos_len, vector<string> mism_char) {
   long hash_value, hash_value2, hash_value3;

   unordered_map<long, int> H_temp;
   unordered_map<int, long> L_temp;

   for (int i = 0; i < pos_len.size(); i++) {
      pair<int,int> pos_len_i = pos_len[i];
      hash_value = get<0>(pos_len_i) * 51787 + get<1>(pos_len_i) * 69061;
      Hash_f hash_func = Hash_f(mism_char[i]);
      hash_value += hash_func.hash_value;
      hash_value %= static_cast<int>(1e9 + 7);

      if (H_temp.count(hash_value)) {
         L_temp.insert(pair<int, int>(i, H[hash_value]));
      }
      H_temp.insert(pair<long, int>(hash_value, i));
   }

   H_tbc_seq.push_back(H_temp);
   L_tbc_seq.push_back(L_temp);
}

int search_match(int beginning_idx, vector<pair<int,int>> &seq_match, vector<string> &seq_mism, int hash_table_idx, vector<pair<int,int>> &ref_seq, vector<string> &ref_seq_mism) {
   int segment_len = 0;
   for (int i = beginning_idx; i < seq_match.size(); i++) {
      // calculate hash value
      auto [pos, len] = seq_match[i];
      int hash_value = pos * 51787 + len * 69061;
      Hash_f hash_func = Hash_f(seq_mism[i]);
      hash_value += hash_func.hash_value;
      hash_value %= static_cast<int>(1e9 + 7);

      if (H_tbc_seq[hash_table_idx].count(hash_value)) {
         int idx = H_tbc_seq[hash_table_idx][hash_value];
         
         if (pos == get<0>(ref_seq[idx]) && len == get<1>(ref_seq[idx]) && seq_mism[i] == ref_seq_mism[idx]) {
            segment_len++;
         } else {
            return segment_len;
         }
      } else {
         return segment_len;
      }
   }
   return segment_len;
}

// saving content for single sequence
void saveToFile(int seq_id, string &match_result) {
   string id=tbc_id_vec[seq_id]+"\n";
   string length=to_string(tbc_length_vec[seq_id])+"\n";

      
   
   string line_lengths=run_length_enc_int(tbc_line_length_vec[seq_id]);
   int s=tbc_n_pos_vec[seq_id].size();
   string n_info,spec_info,matched_lowercase,diff_lowercase_info;
   //are there any N, write down info
   if(s>0){
      n_info=enc_length(tbc_n_pos_vec[seq_id],tbc_n_length_vec[seq_id]);
   }
   //if no, continue
   else{
      n_info="\n";
   }
   s=tbc_other_pos_vec[seq_id].size();
   //are there any other characters,  write down info
   if(s>0){
      spec_info=enc_length(tbc_other_pos_vec[seq_id],tbc_other_char_vec[seq_id]);
   }
   //if no, continue
   else{
      spec_info="\n";
   }
   

   s=low_loc[seq_id].size();
   //are there any lowercase,  write down info
   if(s>0){
      matched_lowercase=run_length_enc_int(low_loc[seq_id]);
      diff_lowercase_info=enc_length(diff_low_pos[seq_id],diff_low_len[seq_id]);
   }
   //if no, continue
   else{
      matched_lowercase="\n";
      diff_lowercase_info="\n";
   }

   //write to file line by line
   ofstream outputfile;
   outputfile.open("output.fa", fstream::app);
   outputfile<<id;
   outputfile<<length;
   outputfile<<line_lengths;
   outputfile<<n_info;
   outputfile<<spec_info;
   outputfile<<matched_lowercase;
   outputfile<<diff_lowercase_info;
   outputfile<<match_result;
   if(seq_id<tbc_id_vec.size()-1){
      outputfile<<endl;
   }
   outputfile.close();
}


// first and second level matching
void second_level_matching(int seq_count, vector<string> &tbc_seqs) {
   vector<int> idx;
   int j;
   string mismatched_string;

   vector<vector<pair<int, int>>> seq_match_results;  // vector of match results for every sequence
   vector<vector<string>> seq_mism_chars;


   for (j = 0; j < percent*seq_count/100; j++) {
      // for every sequence do first level matching and save the results
      first_level_matching(tbc_seqs[j]);
      seq_match_results.push_back(matching_segments);
      seq_mism_chars.push_back(mismatched_chars);

      if (!j) {
         string match_result = "";
         for (int i = 0; i < seq_match_results[j].size(); i++) { 
            auto [pos, len] = seq_match_results[j][i];
            //myfile << pos << " " << len << " " << seq_mism_chars[j][i] <<  " ";
            match_result += to_string(pos) + " " + to_string(len) + " " + seq_mism_chars[j][i] +  " ";
         }
         lowercase_char_matching(j);
         //myfile << endl;
         saveToFile(j, match_result);
      }      

      // create hash table for every sequence in the percentage
      create_hash_table_tbc_seq(matching_segments, mismatched_chars);

      matching_segments.clear();
      mismatched_chars.clear(); 
   }
   for (; j < seq_count; j++) {
      // for every sequence do first level matching and save the results
      first_level_matching(tbc_seqs[j]);
      seq_match_results.push_back(matching_segments);
      seq_mism_chars.push_back(mismatched_chars);

      if (!j) {
         string match_result = "";
         for (int i = 0; i < seq_match_results[j].size(); i++) { 
            auto [pos, len] = seq_match_results[j][i];
            //myfile << pos << " " << len << " " << seq_mism_chars[j][i] <<  " ";
            match_result += to_string(pos) + " " + to_string(len) + " " + seq_mism_chars[j][i] +  " ";
         }
         lowercase_char_matching(j);
         //myfile << endl;
         saveToFile(j, match_result);
         // create hash table for every sequence in the percentage
         create_hash_table_tbc_seq(matching_segments, mismatched_chars);
      }  
      matching_segments.clear();
      mismatched_chars.clear(); 
   }


   vector<vector<tuple<int,int,int>>> second_level_results;
   
   // second level matching
   // iterate through vectors
   for (int seq_idx = 1; seq_idx < seq_match_results.size(); seq_idx++) {
      string row = "";
      
      for (int i = 0; i < seq_match_results[seq_idx].size(); i++) {
         tuple<int,int,int> max_matching = {0, 0 ,0};
         for (int hash_table_idx = 0; hash_table_idx < H_tbc_seq.size(); hash_table_idx++) {

            int len = search_match(i, seq_match_results[seq_idx], seq_mism_chars[seq_idx], hash_table_idx, seq_match_results[hash_table_idx], seq_mism_chars[hash_table_idx]);
            
            if (len > get<2>(max_matching)) {
               max_matching = {hash_table_idx, i, len};
            }      
         }

         if (get<2>(max_matching) < 2) {       
            auto [pos, len] = seq_match_results[seq_idx][i];
            row += to_string(pos) + " " 
                  + to_string(len) + " "
                  + seq_mism_chars[seq_idx][i] + " ";
         } else {
            auto [seq, pos, len] = max_matching;
            row += to_string(seq) + " " + to_string(pos) + " " + to_string(len) + " ";
            i += len-1;
         }
      }
         
   // std::cout << row << endl; 
   // myfile << row << endl;
      lowercase_char_matching(seq_idx);   
      saveToFile(seq_idx, row);
   }
   //myfile.close();
}

// lowercase character matching

void lowercase_char_matching(void) {

}

// void print(vector<int> const &input) {
//    for (auto const &i: input) {
//       std::cout << i << " ";
//    }
// }

// End of code written by Lara Grgurić
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//saving information from First-level Matching
string saveFirstMatch(vector<tuple<int,int>> matching_segments,vector<string> misMatch){
   string output="";
   for(int i=0;i<matching_segments.size();i++){
      std::tuple<int, int> holder=matching_segments[i];
      output+=saveMatch(holder,misMatch[i]);
   }
   output+="\n";
   return output;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {

   int tbc_num = 0;
   vector<string> tbc_files_unread;
   string s1, ref_file;

   try {
   

      assert(string(argv[1]) == "-r");
      assert(string(argv[3]) == "-f");

      ref_file = argv[2];

      assert(argc >= 5);

      
      for(int i = 4; i < argc; ++i)
         s1 = argv[i], tbc_files_unread.push_back(s1), ++tbc_num; 
   }
   catch (exception e) {
      return std::cout << "Usage: a.exe -r <ref_file> -f <tbc_file> <tbc_file> ..." << std::endl, 1;  
   }

   ofstream myfile("output.fa");
   myfile.close();

   
   // std::cout<<"Please enter a reference genome file name: ";
   // std::cin>>ref_file;
   // Written by Lara Grgurić
   // int tbc_num;
   // std::cout<<"Please enter the number of to-be-compressed genome files: ";
   // std::cin>>tbc_num;
   vector<string> tbc_files;
   
   // for (int i = 0; i < tbc_num; i++) {
      // std::cout<<"Please enter a to-be-compressed genome file name: ";
      // std::cin>>tbc_file;
      // tbc_files_unread.push_back(tbc_file);  
   // }
   std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
   for (int i = 0; i < tbc_num; i++) {
      tbc_files.push_back(tbc_information_extraction(tbc_files_unread[i]));  
   }

   creating_hash_table(reference_information_extraction(ref_file));
   second_level_matching(tbc_files.size(), tbc_files);

   //time elapsed
   std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
   std::cout << "\nTime difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

   system("7z a -m0=PPMd output.7a output.fa");
   

   return 0;
}