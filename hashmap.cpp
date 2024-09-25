// #include "hashmap.h"

// #include <iostream>
// using namespace std;
// int main() {
//   // Define a hashmap that maps strings to doubles and has a maximum
//   // load factor of 0.2. It will initially have 10 buckets when empty.
//   list<int>* hi = new list<int>[100];
//   if (hi[5].empty()) {
//     cout << "yes" << endl;
//   }
//   HashMap<double> nameToGPA(0.2);
//   for (int i = 0; i < 20000; i++) {
//     nameToGPA.insert(to_string(char((random() % 26) + 'A')) +
//                          to_string(char((random() % 26) + 'A')) +
//                          to_string(char((random() % 26) + 'A')),
//                      random());
//   }
//   for (int i = 0; i < 20000; i++) {
//     nameToGPA.find(to_string(char((random() % 26) + 'A')) +
//                    to_string(char((random() % 26) + 'A')) +
//                    to_string(char((random() % 26) + 'A')));
//   }
// }