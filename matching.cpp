#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <fstream>
#include <algorithm>
//#include <bits/stdc++.h>
using namespace std;
int k = 5;
map<long, int> H;
map<int, long> L;
vector<tuple<int, int>> matching_segments;
vector<string> mismatched_chars;
int length = 0;

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
      if (H.find(hash_value) != H.end()) {
         L.insert(pair<int, int>(i, H[hash_value]));
      }
      H.insert(pair<long, int>(hash_value, i));

   }
}


void first_level_matching(string tbc_seq) {
   vector<int> idx;
   int i;
   string mismatched_string;
   for (i = 0; i < tbc_seq.length() - k; i++) {
      string k_mer = "";
      for (int j = 0; j < k; j++) {
         k_mer += tbc_seq[i+j];
      }
      Hash_f hash_func = Hash_f(k_mer);

      long hash_value = hash_func.hash_value;
      
      if (H.find(hash_value) != H.end()) {
         // match
         if (mismatched_string.length() > 0) mismatched_chars.push_back(mismatched_string);
         mismatched_string = "";

         if (length == 0) length = k-1;

         // searching for all positions with the same hash value
         vector<int> temp;
         temp.push_back(H[hash_value]);
         while (L.find(temp.back()) != L.end()) {
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
            idx = other;
         } else {
            idx = temp;
         }
         length++;

      } else {
         // mismatch

         // printf("mismatch: char = %c ", k_mer[k_mer.length()-1]);
         // printf("%d %d", idx[0] - (length-k), length);

         // (position, length) = (idx[0] - (length-k), length)

         if (length != 0) {
            // end of matching segment
            // recording position and length     
            matching_segments.push_back(make_tuple(idx[0] - (length-k), length));
            mismatched_string += k_mer[k_mer.length()-1];
            i += k-1;
            length = 0;
            idx.clear();
         } else {
            // multiple mismatching characters
            mismatched_string += k_mer[0];
         }
      }
   }
   string final = "";
   for (--i; i < tbc_seq.length(); i++) {
      final += tbc_seq[i];
   }
   mismatched_chars.push_back(final);
}

// lowercase character matching

void lowercase_char_matching(void) {

}

void print(vector<int> const &input) {
   for (auto const &i: input) {
      cout << i << " ";
   }
}

int main(int argc, char const *argv[])
{
   // mini test
   string a = "ACGTGGTAACCCTGTGACCAAA";
   string b = "ACGTGTACCCTGTATCCAAAGTG";
   creating_hash_table(a);
   first_level_matching(b);

   //for (int i = 0; i < 15)
   return 0;
}
