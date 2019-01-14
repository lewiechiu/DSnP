/****************************************************************************
  FileName     [ cirGate.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define class CirAigGate member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"
#include "list"
#include "stack"

using namespace std;

// TODO: Keep "CirGate::reportGate()", "CirGate::reportFanin()" and
//       "CirGate::reportFanout()" for cir cmds. Feel free to define
//       your own variables and functions.

extern CirMgr *cirMgr;
extern int printidxs;
/**************************************/
/*   class CirGate member functions   */
/**************************************/
// used at FANIN FANOUT
void
CirGate::reportGate() const
{
   char z = getTypeStr();
   if(z=='i')
      cout << "PI " << ID;
   else if(z=='a')
      cout << "AIG " << ID;
   else if(z=='o')
      cout << "PO " << ID;
   else if(z=='0')
      cout << "CONST 0";
   else if(z=='u')
      cout << "UNDEF " << ID;
   return ;
}

void
CirGate::reportFanin(int level) const
{
   printidxs++;
   int a = 0;
   int b = 0;
   if(getTypeStr()!='o')
      cirMgr->printFanin(ID,a,level,b);
   else
   {
      reportGate();
      cout << endl;
      a++;
      b++;
      cirMgr->printFanin(getInputNum(),a,level,b);
   }
}

void
CirGate::reportFanout(int level) const
{
   printidxs++;
   int a = 0;
   int b = 0;
   if(getTypeStr()!='o')
      cirMgr->printFanOut(ID,a,level,b,ID);
   else
   {
      reportGate();
      cout << endl;
   }
}

// This will print out the detail of the gate
// Used at netlist-----------------------
void
CirAIGGate::printGate()const
{
   cout << "AIG " << ID << " ";
   if(cirMgr->getGate(abs(in1)) && cirMgr->getGate(abs(in1))->getTypeStr()=='u')
      cout << "*";
   if(in1 < 0)
      cout<< "!";
   cout << (int)abs(in1) << " ";
   if(cirMgr->getGate(abs(in2)) && cirMgr->getGate(abs(in2))->getTypeStr()=='u')
      cout << "*";
   if(in2<0)
      cout << "!";
   cout << (int)abs(in2);
   return ;
}

void
CirPIGate::printGate()const
{
   cout << "PI  " << ID;
   if(!SymbolicName.empty())
      cout << " (" << SymbolicName << ")" ;
   return; 
}

void
Cir0Gate::printGate()const
{
   cout << "CONST0";
   return;
}
// Used at netlist-----------------------