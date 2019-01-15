/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir optimization functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Please keep "CirMgr::sweep()" and "CirMgr::optimize()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/**************************************************/
/*   Public member functions about optimization   */
/**************************************************/
// Remove unused gates
// DFS list should NOT be changed
// UNDEF, float and unused list may be changed

void
CirMgr::markSweep(int pt)
{
  if(pt > gate.size() || pt<0 || this->gate[pt]==nullptr)
    return;
  if(abs(gate[pt]->printIDX) == printidxs)
    return;
  int lef = abs(this->gate[pt]->in1);
  int rig = abs(this->gate[pt]->in2);
  markSweep(lef);
  markSweep(rig);
  if(abs(gate[pt]->printIDX) == printidxs)
    return;

  gate[pt]->printIDX = printidxs;
  
}
/*
 
printidxs++; 
int l =0;
for(int i=0;i<POs;i++)
{
  int a = abs(PO[i]->getInputNum());
  printPostOrder(a,l);
}
*/

void
CirMgr::sweep()
{
  printidxs++; 
  for(int i=0;i<POs;i++)
  {
    int a = abs(PO[i]->getInputNum());
    markSweep(a);
  }

  for(int i=1;i<gate.size();i++)
  {
    if(gate[i]==NULL)
      continue;
    if(gate[i]->printIDX != printidxs)
    {
      //remove gate

      // remove input 
      if(gate[i]->getTypeStr()=='i')
        continue;
      int i1 = abs(gate[i]->in1);
      int i2 = abs(gate[i]->in2);
      CirGate* c1 = getGate(i1);
      CirGate* c2 = getGate(i2);
      if(c1)
        c1->output.erase(gate[i]->ID);
      
      if(c2)
        c2->output.erase(gate[i]->ID);
      
      
      //remove output

      cout << "Sweeping: ";
      if(gate[i]->getTypeStr()=='u')
        cout << "UNDEF(" << i ;
      else
        cout << "AIG(" << i;
      cout << ") removed..." << endl;
      delete gate[i];
      gate[i] = NULL;
    }
  }
}

//CIROPTimize

void
CirMgr::opt(int pt)
{
  if(pt > gate.size() || pt<0 || this->gate[pt]==nullptr)
    return;
  if(abs(gate[pt]->printIDX) == printidxs)
    return;
  int lef = abs(this->gate[pt]->in1);
  int rig = abs(this->gate[pt]->in2);
  opt(lef);
  opt(rig);
  if(abs(gate[pt]->printIDX) == printidxs)
    return;


  gate[pt]->printIDX = printidxs;
  if(gate[pt]->getTypeStr()!='a')
    return;
  bool opted = false;
  // 2 same
  if(gate[pt]->in1 == gate[pt]->in2)
  {
    cout << "Simplifying: " << abs(gate[pt]->in1) << " merging " ;
    if(isInverse(gate[pt]->in1))
      cout << "!";
    cout << pt << "..." << endl;
    opted = true;
    for(auto i = gate[pt]->output.begin(); i != gate[pt]->output.end(); i++)
    {
      //replace all of the fanouts of gate[pt] with 0*
      if(abs(i->second->in1)==pt)
      {
        if(isInverse(i->second->in1))
          i->second->in1 = inverse(gate[pt]->in1);
        else
          i->second->in1 = gate[pt]->in1;
      }
      if(abs(i->second->in2)==pt)
      {
        if(isInverse(i->second->in2))
          i->second->in2 = inverse(gate[pt]->in1);
        else
          i->second->in2 = gate[pt]->in1;
      }
      
      gate[abs(gate[pt]->in1)]->output.insert(gate[pt]->output.begin(),gate[pt]->output.end());
    }
    gate[abs(gate[pt]->in1)]->output.erase(pt);
    
  }
  
  // 1 ?  or ? 1
  else if(gate[pt]->in1 == -0.1 || gate[pt]->in2 == -0.1)
  {
    opted = true;
    //removing pt from 0's output
    gate[0]->output.erase(pt);
    
    // add pt's output into non 0 input's output
    if(gate[pt]->in1 != -0.1)
    {
      cout << "Simplifying: " << abs(gate[pt]->in1) << " merging " ;
      if(isInverse(gate[pt]->in1))
        cout << "!";
      cout << pt << "..." << endl;
      getGate(abs(gate[pt]->in1))->output.insert(gate[pt]->output.begin(),gate[pt]->output.end());
      getGate(abs(gate[pt]->in1))->output.erase(pt);
    }
    else
    {
      cout << "Simplifying: " << abs(gate[pt]->in2) << " merging " ;
      if(isInverse(gate[pt]->in2))
        cout << "!";
      cout << pt << "..."<< endl;
      getGate(abs(gate[pt]->in2))->output.insert(gate[pt]->output.begin(),gate[pt]->output.end());
      getGate(abs(gate[pt]->in2))->output.erase(pt);
    }
      
    
    // setting pt's output's input as pt's non-0 input
    for(auto i = gate[pt]->output.begin(); i!= gate[pt]->output.end(); i++)
    {

      if(abs(i->second->in1) == pt && isInverse(i->second->in1))
      {
        if(gate[pt]->in1 != -0.1)
          i->second->in1 = inverse(gate[pt]->in1);
        else
          i->second->in1 = inverse(gate[pt]->in2);
          ///Start from here
      }
      else if(abs(i->second->in1) == pt && !isInverse(i->second->in1))
      {
        if(gate[pt]->in1 != -0.1)
          i->second->in1 = gate[pt]->in1;
        else
          i->second->in1 = gate[pt]->in2;
          ///Start from here
      }
      else if(abs(i->second->in2) == pt && isInverse(i->second->in2))
      {
        if(gate[pt]->in2 != -0.1)
          i->second->in2 = inverse(gate[pt]->in2);
        else
          i->second->in2 = inverse(gate[pt]->in1);
      }
      else if(abs(i->second->in2) == pt && !isInverse(i->second->in2))
      {
        if(gate[pt]->in2 != -0.1)
          i->second->in2 = gate[pt]->in2;
        else
          i->second->in2 = gate[pt]->in1;
      }
    }

  }

  // 3 -3
  else if(abs(gate[pt]->in1 + gate[pt]->in2) < 0.3)
  {
    opted = true;
    
    gate[0]->output.insert(gate[pt]->output.begin(),gate[pt]->output.end());
    cout << "Simplifying: 0 merging " << pt << "..." << endl;
    for(auto i = gate[pt]->output.begin(); i != gate[pt]->output.end(); i++)
    {
      
      if(abs(i->second->in1)==pt)
      {
        if(isInverse(i->second->in1))
        {
          i->second->in1 = -0.1;
        }
        else
          i->second->in1 = 0;  
      }
      else
      {
        if(isInverse(i->second->in2))
        {
          i->second->in2 = -0.1;

        }
        else
          i->second->in2 = 0;  
      }

    }
    getGate(abs(gate[pt]->in1))->output.erase(pt);
  }
  // 0 ? / ? 0
  else if(gate[pt]->in1 == 0 || gate[pt]->in2 ==0)
  {
    opted = true;
    gate[0]->output.erase(pt);
    gate[0]->output.insert(gate[pt]->output.begin(),gate[pt]->output.end());
    cout << "Simplifying: 0 merging " << pt << "..." << endl;

    if(gate[pt]->in1!=0)
      getGate(abs(gate[pt]->in1))->output.erase(pt);
    else
      getGate(abs(gate[pt]->in2))->output.erase(pt);
    for(auto i = gate[pt]->output.begin();i!= gate[pt]->output.end();i++)
    {

      if(abs(i->second->in1)==pt)
      {
        if(isInverse(i->second->in1))
          i->second->in1 = -0.1;
        else
          i->second->in1 = 0;  
      }
      else
      {
        if(isInverse(i->second->in2))
          i->second->in2 = -0.1;
        else
          i->second->in2 = 0;  
      }
      
      
    }


  }

  if(opted)
  {
    delete gate[pt];
    gate[pt] = nullptr;
  }


  
}


// Recursively simplifying from POs;
// _dfsList needs to be reconstructed afterwards
// UNDEF gates may be delete if its fanout becomes empty...
void
CirMgr::optimize()
{
  printidxs++; 
  for(int i=0;i<POs;i++)
  {
    int a = abs(PO[i]->getInputNum());
    opt(a);
  }
}



/***************************************************/
/*   Private member functions about optimization   */
/***************************************************/
