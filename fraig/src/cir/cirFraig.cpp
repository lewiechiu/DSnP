/****************************************************************************
  FileName     [ cirFraig.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir FRAIG functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2012-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "sat.h"
#include "myHashMap.h"
#include "util.h"

using namespace std;

// TODO: Please keep "CirMgr::strash()" and "CirMgr::fraig()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/*******************************************/
/*   Public member functions about fraig   */
/*******************************************/
// _floatList may be changed.
// _unusedList and _undefList won't be changed


void
CirMgr::trashDFS(int pt)
{
  if(pt > gate.size() || pt<0 || this->gate[pt]==nullptr)
    return;
  if(abs(gate[pt]->printIDX) == printidxs)
    return;
  int lef = abs(this->gate[pt]->in1);
  int rig = abs(this->gate[pt]->in2);
  trashDFS(lef);
  trashDFS(rig);
  if(abs(gate[pt]->printIDX) == printidxs)
    return;

  gate[pt]->printIDX = printidxs;
  if(gate[pt]->getTypeStr()!='a')
    return;
  int l = abs(gate[pt]->in1);
  int r = abs(gate[pt]->in2);
  myKey a(l,r,pt);
  if(hs.check(a))
  {
    //remove pt
    cout << "Strashing: " << a.getID() << " merging " << pt << "..." << endl;
    getGate(a.getID())->output.insert(gate[pt]->output.begin(),gate[pt]->output.end());
    getGate(l)->output.erase(pt);
    getGate(r)->output.erase(pt);
    for(auto i = gate[pt]->output.begin(); i != gate[pt]->output.end() ; i++)
    {
      if(abs(i->second->in1)== pt)
      {
        if(isInverse(i->second->in1))
          i->second->in1 = inverse(a.getID());
        else
          i->second->in1 = a.getID();
      }
      if(abs(i->second->in2)==pt)
      {
        if(isInverse(i->second->in2))
          i->second->in2 = inverse(a.getID());
        else
          i->second->in2 = gate[pt]->in1;
      }
    }
    delete gate[pt];
    gate[pt] = nullptr;
  }
  else
    hs.insert(a);


  
}


void
CirMgr::strash()
{
  hs.init(m);
  for(int i=0;i<POs;i++)
  {
    trashDFS(abs(PO[i]->in1));
  }
}

void
CirMgr::fraig()
{
}

/********************************************/
/*   Private member functions about fraig   */
/********************************************/
