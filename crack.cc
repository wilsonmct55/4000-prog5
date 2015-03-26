#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <vector>
#include <omp.h>
using namespace std;
int main(int argc, char *argv[]) {

  ifstream in1;
  ifstream in2;
  
  in1.open(argv[1]); // Dictionary
  in2.open(argv[2]); // Salted passwords

  vector<string> dictionary;

  while (!in1.eof()) {
    string line;

    getline(in1,line);
    if (!in1.fail()) {
      dictionary.push_back(line);
    }
  }

  while (!in2.eof()) {
    string line;
    getline(in2,line);
    if (!in2.fail()) {
      int count = 0;
      int i=0;
      while (i<line.size() && (count < 3)) {
	if (line[i]=='$') count++;
	i++;
      }
      //cout << line.substr(0,i-1) <<endl;
      string salt = line.substr(0,i-1);
      //cout << line << endl;
      bool found=false;
      #pragma omp parallel for
      for (int j=0;j<dictionary.size();j++) {
	string t;
	//cout << "Trying " << dictionary[j] << " " << j << endl;
	//if ((j%10000)==0) cout << j << endl;
      	t = crypt(dictionary[j].c_str(),salt.c_str());
	//cout << t << endl;
	if (t==line) {
	  cout << "Password = " << dictionary[j] << endl;
	  #pragma omp critical
	  found = true;
	  //break;
	}
      }
      if (!found) cout << "Password not found" << endl;
    }
  }
}
