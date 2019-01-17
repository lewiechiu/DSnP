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


void
CirGate::reportGate() const
{
   cout << "================================================================================" << endl;
   cout << "= ";
   char z = getTypeStr();
   if(z=='i')
   {
      cout << "PI(" << ID << ")";
      if(!getSymbolicName().empty())
         cout << "\"" << getSymbolicName() << "\"";
   } 
   else if(z=='a')
      cout << "AIG(" << ID << ")";
   else if(z=='o')
   {
      cout << "PO(" << ID << ")";
      if(!getSymbolicName().empty())
         cout << "\"" << getSymbolicName() << "\"";
   }
   else if(z=='0')
      cout << "CONST(0)";
   else if(z=='u')
      cout << "UNDEF(" << ID << ")";
   cout <<", line " << line << endl;
   cout << "= FECs:";
   
   //print FECs:

   cout << endl;
   cout << "= Value: ";

   
   for(int i=63;i>=0;i--)
   {
      if(i% 8 == 7 && i!=63)
         cout << "_";
      cout << ((genResult() >> i) & 1U);
   }
   // print bitwisely.
   cout << endl;
   cout << "================================================================================" << endl;
}



// used at FANIN FANOUT
void
CirGate::myreportGate() const
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
      myreportGate();
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
      myreportGate();
      cout << endl;
   }
}

// This will print out the detail of the gate
// Used at netlist-----------------------

//--------------------AIG----------------------------//
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

size_t
CirAIGGate::genResult()const
{
   if(in1 < 0 && in2 <0)
      return ( ~sig1 & ~sig2);
   if(in1 <0)
      return (~sig1 & sig2);
   if(in1 >0 && in2 < 0)
      return (sig1 & ~sig2);
   if(in1 > 0)
      return (sig1 & sig2);
}


//--------------------AIG----------------------------//

//********************PI*****************************//
void
CirPIGate::printGate()const
{
   cout << "PI  " << ID;
   if(!SymbolicName.empty())
      cout << " (" << SymbolicName << ")" ;
   return; 
}

size_t
CirPIGate::genResult()const
{
   return this->inSig;
}
//********************PI*****************************//



//********************0******************************//
void
Cir0Gate::printGate()const
{
   cout << "CONST0";
   return;
}


size_t
Cir0Gate::genResult()const
{
   return 0;
}
//********************0******************************//



//********************PO*****************************//
size_t
CirPOGate::genResult()const
{
   if(in1 < 0)
      return ~result;
   return result;
}
//********************PO*****************************//


