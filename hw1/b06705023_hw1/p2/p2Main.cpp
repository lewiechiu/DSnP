/****************************************************************************
  FileName     [ p2Main.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define main() function ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2016-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <string>
#include "p2Json.h"

using namespace std;

int main()
{
   Json json;

   // Read in the csv file. Do NOT change this part of code.
   string jsonFile;
   cout << "Please enter the file name: ";
   cin >> jsonFile;
   if (json.read(jsonFile))
      cout << "File \"" << jsonFile << "\" was read in successfully." << endl;
   else {
      cerr << "Failed to read in file \"" << jsonFile << "\"!" << endl;
      exit(-1); // jsonFile does not exist.
   }

   while(true)
   {
    cout << "Enter command: ";
    string opt;
    cin >> opt;
    if(opt=="PRINT")
      json.Print();
    else if(opt=="AVE")
      json.Mean();
    else if(opt=="MAX")       
     json.Maximum();
    else if(opt=="MIN")
      json.Minimum();
    else if(opt=="SUM")
      cout << "The summation of the values is: " << json.Sum() << "." << endl;
    else if(opt=="ADD")
    {
      string Key;
      int Values = 0;
      cin >> Key >> Values;
      json.Add(Key,Values);
    }
    else if(opt=="EXIT")
      exit(0);
    else
      cout << "Error: No element found!!" << endl;

   }
}
