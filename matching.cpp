#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <sstream>
//#include <bits/stdc++.h>
using namespace std;
int k = 3;
unordered_map<long, int> H;
unordered_map<int, int> L;
vector<pair<int, int>> matching_segments;
vector<string> mismatched_chars;
int length = 0, percent = 50;

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
   for (--i; i < tbc_seq.length(); i++) {
      final += tbc_seq[i];
   }
   mismatched_chars.push_back(final);
}


// hash table for to-be-compressed sequence
void create_hash_table_tbc_seq(vector<pair<int,int>> pos_len, vector<string> mism_char) {
   long hash_value, hash_value2, hash_value3;

   // for (int i = 0; i < tbc_seq.length(); i++) {
   //    string k_mer = "";
   //    for (int j = 0; j < k; j++) {
   //       k_mer += tbc_seq[i+j];
   //    }
   //    Hash_f hash_func = Hash_f(k_mer);
   //    long hash_value1 = hash_func.hash_value;
   //    hash_v
   // for_each(pos_len.begin(), pos_len.end(), [hash_value](tuple<int,int> const& value) {
      
   // });

   unordered_map<long, int> H_temp;
   unordered_map<int, long> L_temp;

   for (int i = 0; i < pos_len.size(); i++) {
      pair<int,int> pos_len_i = pos_len[i];
      hash_value = get<0>(pos_len_i) * 51787 + get<1>(pos_len_i) * 69061;
      Hash_f hash_func = Hash_f(mism_char[i]);
      hash_value += hash_func.hash_value;
      hash_value %= static_cast<int>(1e9 + 7);
      cout << hash_value << endl;

      if (H_temp.count(hash_value)) {
         L_temp.insert(pair<int, int>(i, H[hash_value]));
      }
      H_temp.insert(pair<long, int>(hash_value, i));
   }

   H_tbc_seq.push_back(H_temp);
   L_tbc_seq.push_back(L_temp);
}

// void saveMatchEntity(string path,  &entity) {

// }

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


// second level matching
void second_level_matching(int seq_count, string tbc_seqs[]) {
   vector<int> idx;
   int j;
   string mismatched_string;

   vector<vector<pair<int, int>>> seq_match_results;  // vector of match results for every sequence
   vector<vector<string>> seq_mism_chars;

   ofstream myfile;
   myfile.open("output.txt");


   for (j = 0; j < percent*seq_count/100; j++) {
      // for every sequence do first level matching and save the results
      first_level_matching(tbc_seqs[j]);
      seq_match_results.push_back(matching_segments);
      seq_mism_chars.push_back(mismatched_chars);

      if (!j) {
         for (int i = 0; i < seq_match_results[j].size(); i++) { 
            auto [pos, len] = seq_match_results[j][i];
            myfile << pos << " " << len << " " << seq_mism_chars[j][i] <<  " ";
         }
         myfile << endl;
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
         for (int hash_table_idx = 0; hash_table_idx < seq_idx; hash_table_idx++) {

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


      //printf(row.c_str());   
      //std::cout << row << endl;
      }
         
   std::cout << row << endl; 
   myfile << row << endl;     
   row = "";
   
         


   }

   myfile.close();
}

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
void first_level_decompress(string &ref_seq, vector<vector<pair<int, int>>> &seq_match_results, vector<vector<string>> &seq_mism_chars) {
   for (int seq_id = 0; seq_id < seq_match_results.size(); seq_id++) {
      string line = "";

      for (int i = 0; i < seq_match_results[seq_id].size(); i++) {
         auto [pos, len] = seq_match_results[seq_id][i];
         string mism_chars = seq_mism_chars[seq_id][i];
         for (int j = pos; j < pos+len; j++) {
            line += ref_seq[j];
         }
         line += mism_chars;
      }

      ofstream newfile;
      string filename = "tbc" + to_string(seq_id) + ".txt";
      newfile.open(filename);
      newfile << line;
      newfile.close();
   }
}

//decoding
void readingFile(string path) {
   ifstream myfile(path);
   string reference_seq;
   getline(myfile, reference_seq);

   vector<vector<pair<int, int>>> seq_match_results;  // vector of match results for every sequence
   vector<vector<string>> seq_mism_chars;


   //string tbc_first_level;
   //getline(myfile, tbc_first_level);
   vector<pair<int, int>> matching_segments_first_seq;
   vector<string> mismatched_chars_first_seq;
   int a, b;
   string c;   
   string line;
   getline(myfile, line);

   // reading first level matching result
   std::istringstream iss(line);
   while ((iss >> a >> b >> c)) { 
      matching_segments_first_seq.push_back(make_pair(a,b));
      mismatched_chars_first_seq.push_back(c);
   }
   

   seq_match_results.push_back(matching_segments_first_seq);
   seq_mism_chars.push_back(mismatched_chars_first_seq);
   

   // reading second level matching results
   while (getline(myfile, line)) {
      matching_segments_first_seq.clear();
      mismatched_chars_first_seq.clear();

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
      
      seq_match_results.push_back(matching_segments_first_seq);
      seq_mism_chars.push_back(mismatched_chars_first_seq);
   }
   
   myfile.close();
   first_level_decompress(reference_seq, seq_match_results, seq_mism_chars);
}



// lowercase character matching

void lowercase_char_matching(void) {

}

void print(vector<int> const &input) {
   for (auto const &i: input) {
      std::cout << i << " ";
   }
}


int main(int argc, char const *argv[])
{
   // mini test
   // string a = "AGATGGGCCCTTTAGGTATT";
   // ofstream myfile;
   // // myfile.open("output.txt");
   // // myfile << a << endl;
   // // myfile.close();
   // string b[] = {"AGCTGGTCCCTGGAGGAATC",
   //                "AGTTGGTCCCTGGAGGATTT",
   //                "AGTTGGTCCCTGAAGGATTT",
   //                "ATATGGTCCCTGAAGGATTT"};




   // // string d = "ACGTGTCCAAAGTGACCCTGTAT";
   // creating_hash_table(a);
   // second_level_matching(sizeof(b)/sizeof(b[0]), b);
   // first_level_matching(b);
   // create_hash_table_tbc_seq(matching_segments, mismatched_chars);
   // matching_segments.clear();
   // mismatched_chars.clear();
   // first_level_matching(c);
   // create_hash_table_tbc_seq(matching_segments, mismatched_chars);

   
   readingFile("output.txt");
   //for (int i = 0; i < 15)
   return 0;
}
