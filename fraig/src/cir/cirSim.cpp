/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir simulation functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Keep "CirMgr::randimSim()" and "CirMgr::fileSim()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/************************************************/
/*   Public member functions about Simulation   */
/************************************************/
void
CirMgr::randomSim()
{
  int lengthing = PIs;
  
  //generate the simulation signal
  
  // for(int i=0;i)
}
void
CirMgr::simpleDFS(int pt)
{
  if(pt > gate.size() || pt<0 || this->gate[pt]==nullptr)
    return;
  if(abs(gate[pt]->printIDX) == printidxs)
    return;
  int lef = abs(this->gate[pt]->in1);
  int rig = abs(this->gate[pt]->in2);
  simpleDFS(lef);
  simpleDFS(rig);
  if(abs(gate[pt]->printIDX) == printidxs)
    return;

  gate[pt]->printIDX = printidxs;
  
}
void
CirMgr::fileSim(ifstream& patternFile)
{
  string input;



  size_t a[PIs] = {0};
  int cnt = 0;
  while(patternFile >> input)
  {
    if(cnt==64)
    {
      //do some handling process
    }
    bool wrongPat = false;
    for(int i=0;i<input.length();i++)
    {
      if(input[i]!= 48 || input[i] != 49)
      {
        cout << "Error: Pattern(" << input << ") contains a non-0/1 character(\'" << input[i]
            << "\')." << endl;
        wrongPat = true;
        break;
      }
    }
    if(input.length()!=PIs)
    {
      wrongPat = true;
      cout << "Error: Pattern(" << input << ") length(" << input.length()
            << ") does not match the number of inputs(" << PIs << ") in a circuit!!"<< endl;
    }
      
    if(wrongPat)
      continue;
    
    for(int i=0;i<PIs;i++)
    {
      if(input[i]==48)
        a[i] |= (1UL << cnt);

    }
    
  }
  printidxs++;
  for(int i=0;i<POs;i++)
  {
    int a = abs(PO[i]->getInputNum());
    simpleDFS(a);
  }

}

/*************************************************/
/*   Private member functions about Simulation   */
/*************************************************/


// might need to add some function to handle when cnt reaches 64