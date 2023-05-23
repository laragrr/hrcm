#include <string>
using namespace std;

vector<int> reference_lowercase_information;
vector<int> reference_lowercase_information_length;
void information_extraction(string ref_seq, string tbc_seq) {
   // extract reference lowercase information
   int checkpoint_i = 0;
   bool found_first = false;
   int length = 0;
   for (int i = 0; i < ref_seq.length(); i++) {
      char char_at_i = ref_seq[i];

      if (char_at_i < 123 && char_at_i > 96) {
         if (!found_first) {
            checkpoint_i = i;
            found_first = true;
            reference_lowercase_information.pushback(i);
         } else {
            length++;
         }

      } else {
      found_first = false;
      reference_lowercase_information_length.pushback(length);
      length = 0;
      }

   }

   // extract to-be-compressed lowercase information



   // extract N character information

   // extract other character information

}

int main(void) {

   return 0;
}