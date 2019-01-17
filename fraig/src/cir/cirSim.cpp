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
#include <set>

using namespace std;

// TODO: Keep "CirMgr::randimSim()" and "CirMgr::fileSim()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

static int simtimes=0;

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
CirMgr::fileSim(ifstream& patternFile)
{
  string input;



  size_t a[PIs] = {0};
  int cnt = 0;
  int totalcnt = 0;
  while(patternFile >> input)
  {
    if(cnt==64)
    {
      putInputSig(a);
      runSim();
      if(_simLog)
        writeSimResult(a,cnt);
      fecGrouping();
      cnt = 0;
      //do some handling process
      //put into fec groups
    }
    bool wrongPat = false;
    for(int i=0;i<input.length();i++)
    {
      if(input[i]!= '0' && input[i] != '1')
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
      if(input[i]=='1')
        a[i] |= (1UL << cnt);
    }
    // cout << "->>>";
    // for(int j=0;j<PIs;j++)
    // {
    //   cout << ((a[j] >> cnt) & 1);
    // }
    // cout << endl;
    cnt++;
    totalcnt++;
  }
  putInputSig(a);
  runSim();
  if(_simLog)
    writeSimResult(a,cnt);
  fecGrouping();
  cout << totalcnt << " patterns simulated." << endl;
}

/*************************************************/
/*   Private member functions about Simulation   */
/*************************************************/


void
CirMgr::putInputSig(size_t *ins)
{
  for(int i=0;i<PIs;i++)
  {
    int num = PInum[i];
    static_cast<CirPIGate *>(gate[num])->inSig = ins[i];
  }

}


void
CirMgr::fecGrouping()
{
  printidxs++;
  for(int i=0;i<POs;i++)
  {
    int asdf = abs(PO[i]->getInputNum());
    markSweep(asdf);
  }
  
  if(fecgrps.size()==0)
  {
    for(int i=0;i<gate.size();i++)
    {
      if(gate[i]->getTypeStr()!='a' && gate[i]->printIDX == printidxs)
        continue;
      
      myFECgrps a(gate[i]->genResult());
      if(fecgrps.insert(a).second==false)
      {
        myFECgrps ins = *fecgrps.find(a);
        ins.add(i);
        fecgrps.insert(a);
      }
      else
        fecgrps.insert(a);

    }
    for(auto it = fecgrps.begin();it != fecgrps.end();it++)
    {
      if((*it).grps.size()==1)
        fecgrps.erase(*it);
    }
  }
  else
  {
    for(auto it = fecgrps.begin();it != fecgrps.end();it++)
    {
      if((*it).grps.size()==1)
        fecgrps.erase(*it);
    }
  }
  cout << "Total #FEC Group = " << fecgrps.size() << endl;
}

// might need to add some function to handle when cnt reaches 64


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
  
  if(gate[pt]->getTypeStr()=='a')
  {
    int l = abs(gate[pt]->in1);
    int r = abs(gate[pt]->in2);
    static_cast<CirAIGGate* >(gate[pt])->sig1 = getGate(l)->genResult();
    static_cast<CirAIGGate* >(gate[pt])->sig2 = getGate(r)->genResult();
  }



}

void
CirMgr::runSim()
{
  printidxs++;
  for(int i=0;i<POs;i++)
  {
    int a = abs(PO[i]->getInputNum());
    simpleDFS(a);
    static_cast<CirPOGate* >(PO[i])->result = getGate(a)->genResult();
  }
}

void
CirMgr::writeSimResult(size_t *ins,int thresh)
{

  for(int pat = 0;pat < thresh;pat++)
  {
    for(int i=0;i<PIs;i++)
      (*_simLog) << ((ins[i] >> pat) & 1);
    (*_simLog) << " ";
    for(int i=0;i<POs;i++)
    {
      int rest = ( (PO[i]->genResult() >> pat) & 1U );
      (*_simLog) << rest;
    }
    (*_simLog) << endl; 
  }

  for(int i=0;i<PIs;i++)
  {
    ins[i] = 0;
  }

  return;
}