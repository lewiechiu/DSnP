/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Implement memeber functions for class CirMgr

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;
int printidxs = 0;
enum CirParseError {
   EXTRA_SPACE,
   MISSING_SPACE,
   ILLEGAL_WSPACE,
   ILLEGAL_NUM,
   ILLEGAL_IDENTIFIER,
   ILLEGAL_SYMBOL_TYPE,
   ILLEGAL_SYMBOL_NAME,
   MISSING_NUM,
   MISSING_IDENTIFIER,
   MISSING_NEWLINE,
   MISSING_DEF,
   CANNOT_INVERTED,
   MAX_LIT_ID,
   REDEF_GATE,
   REDEF_SYMBOLIC_NAME,
   REDEF_CONST,
   NUM_TOO_SMALL,
   NUM_TOO_BIG,

   DUMMY_END
};

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
static unsigned lineNo = 0;  // in printint, lineNo needs to ++
static unsigned colNo  = 0;  // in printing, colNo needs to ++
static char buf[1024];
static string errMsg;
static int errInt;
static CirGate *errGate;

static bool
parseError(CirParseError err)
{
   switch (err) {
      case EXTRA_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Extra space character is detected!!" << endl;
         break;
      case MISSING_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing space character!!" << endl;
         break;
      case ILLEGAL_WSPACE: // for non-space white space character
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal white space char(" << errInt
              << ") is detected!!" << endl;
         break;
      case ILLEGAL_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal "
              << errMsg << "!!" << endl;
         break;
      case ILLEGAL_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal identifier \""
              << errMsg << "\"!!" << endl;
         break;
      case ILLEGAL_SYMBOL_TYPE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal symbol type (" << errMsg << ")!!" << endl;
         break;
      case ILLEGAL_SYMBOL_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Symbolic name contains un-printable char(" << errInt
              << ")!!" << endl;
         break;
      case MISSING_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing " << errMsg << "!!" << endl;
         break;
      case MISSING_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing \""
              << errMsg << "\"!!" << endl;
         break;
      case MISSING_NEWLINE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": A new line is expected here!!" << endl;
         break;
      case MISSING_DEF:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing " << errMsg
              << " definition!!" << endl;
         break;
      case CANNOT_INVERTED:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": " << errMsg << " " << errInt << "(" << errInt/2
              << ") cannot be inverted!!" << endl;
         break;
      case MAX_LIT_ID:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Literal \"" << errInt << "\" exceeds maximum valid ID!!"
              << endl;
         break;
      case REDEF_GATE:
         cerr << "[ERROR] Line " << lineNo+1 << ": Literal \"" << errInt
              << "\" is redefined, previously defined as "
              << errGate->getTypeStr() << " in line " << errGate->getLineNo()
              << "!!" << endl;
         break;
      case REDEF_SYMBOLIC_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ": Symbolic name for \""
              << errMsg << errInt << "\" is redefined!!" << endl;
         break;
      case REDEF_CONST:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Cannot redefine const (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_SMALL:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too small (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_BIG:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too big (" << errInt << ")!!" << endl;
         break;
      default: break;
   }
   return false;
}

/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/
bool
CirMgr::readCircuit(const string& fileName)
{
   if(POnum!=0)
      delete POnum;
   if(PInum!=0)
      delete PInum;
   this->PO.clear();
   this->gate.clear();
   this->gate.resize(0);
   this->PO.resize(0);
   string input;
   int m,i,o,a;
   fstream in(fileName);
   if(!in)
   {
      cerr << "fail to open " << endl;
      return false;
   } 
   // aag M I L O A
   getline(in,input);
   string M,I,O,A;
   input.erase(0,4);
   M = input.substr(0,input.find(' '));
   input.erase(0,input.find(' ')+1);
   I = input.substr(0,input.find(' '));
   input.erase(0,input.find(' ')+1);

   //Erase L
   input.erase(0,input.find(' ')+1);


   O = input.substr(0,input.find(' '));
   input.erase(0,input.find(' ')+1);
   A = input.substr(0,input.find(' '));
   input.erase(0,A.length()+1);
   if(myStr2Int(M,m))
   {
      //cout << m << endl;
      this->m = m;
      this->gate.resize(m+1);  
      
      //cout << "reserved" << this->gate.max_size() << endl;
   }
   if(myStr2Int(I,i))
   {
      this->PIs = i;
      this->PInum = new int[PIs];
   }
   if(myStr2Int(O,o))
   {
      this->POs = o;
      this->POnum = new int[POs];
   }
   if(myStr2Int(A,a))
   {}
//Input
   // cout << "Input gate: " << endl;
   for(int cnt = 1 ; cnt <= m ;cnt++)
   {
      this->gate[cnt] = nullptr;
   }

   for(int k = 0;k<i;k++)
   {
      int abc;
      getline(in,input);
      if(myStr2Int(input,abc))
      {
         this->gate[abc/2] = new CirPIGate();

         this->gate[abc/2]->ID = abc/2;
         this->PInum[k] = (abc/2);
         this->gate[abc/2]->line = k+1+1;
         //cout << this->gate.size() << endl;
         // cout << abc << " ";
      }
      else
      {
         //handle error
      }
   }

   // cout << this->gate.size() << endl;
   // cout << endl << "Output gate: " << endl;
   
//Output
   this->PO.resize(POs);
   for(int cnt = 0 ; cnt < POs ;cnt++)
      this->PO[cnt] = nullptr;

   for(int k = 0;k<o;k++)
   {
      getline(in,input);
      int abc;
      if(myStr2Int(input,abc))
      {

         int prev = abc;
         if(abc%2 == 1)
         {
            
            abc--;
            double ins = abc;
            
            if(abc == 0)
               ins += 0.1;
            this->PO[k] = new CirPOGate(-ins/2);
         }


         else
         {
            this->PO[k] = new CirPOGate(abc/2);
         }
            
         this->PO[k]->ID = k+1+m;
         this->POnum[k] = abc/2;
         this->PO[k]->line = k+1+i+1;
         
      }
      else
      {
         //handle error
      }
      //cout << k << endl;
      
   }
   
//AIG
   // cout << "AIG" << endl;
   this->gate[0] = new Cir0Gate();
   gate[0]->ID = 0;
   for(int k = 0;k<a;k++)
   {
      getline(in,input);
      int lhs,rhs1,rhs2;
      
      //erasing And gate NO:
      if(myStr2Int(input.substr(0,input.find(' ')),lhs)) 
      {
         this->gate[lhs/2] = new CirAIGGate();
         
         this->gate[lhs/2]->ID = lhs/2;
         this->gate[lhs/2]->line = k+i+o+1+1;
         input.erase(0,input.find(' ')+1);
      }
   //RHS 1
      if(myStr2Int(input.substr(0,input.find(' ')),rhs1))
      {
         input.erase(0,input.find(' ')+1);
      }
   //RHS 2
      if(myStr2Int(input.substr(0,input.find(' ')),rhs2))
      {
         input.erase(0,input.find(' ')+1);
      }
   }

//Handle symbolic name
   // cout << "handle" << endl;
   while(getline(in,input))
   {
      if(input[0]=='i')
      {
         // input symbol
         input.erase(0,1);
         int abc;
         
         if(myStr2Int(input.substr(0,input.find(' ')),abc))
         {
            abc = this->PInum[abc];
            input.erase(0,input.find(' ')+1);
            //cout << input << endl;
            (this->gate[abc])->setSymbolicName(input);
         }
         else
         {
            // handle -> "ia xxx", where a should be num.
         }

      }
      else if(input[0]=='o')
      {
         input.erase(0,1);
         int abc;
         
         if(myStr2Int(input.substr(0,input.find(' ')),abc))
         {
            input.erase(0,input.find(' ')+1);
            //cout << input << endl;
            (this->PO[abc])->setSymbolicName(input);
         }
         else
         {
            // handle -> "ia xxx", where a should be num.
         }

      }
      else if(input.length()==1 && input[0] == 'c')
      {
         //handle comment
         break;
      }
      else
         break;
   }
   in.close();
   in.open(fileName);
   for(int cnt = 0 ;cnt<1+this->PIs+this->POs;cnt++)
      getline(in,input);
//AIG
   for(int k = 0;k<a;k++)
   {
      // cout << k << endl;
      getline(in,input);
      int lhs,rhs1,rhs2;
      
      //erasing And gate NO:
      if(myStr2Int(input.substr(0,input.find(' ')),lhs)) 
      {
         input.erase(0,input.find(' ')+1);
      }
         

   //RHS 1
      lhs = lhs/2;
      if(myStr2Int(input.substr(0,input.find(' ')),rhs1))
      {
         input.erase(0,input.find(' ')+1);

         
         // cout << "---------------------" << endl;
         // cout << "gate NO: " <<  lhs << " address: " << this->gate[lhs] << endl;
         if (rhs1 % 2 == 1)
         {
            
            rhs1 -= 1;
            double r1 =rhs1/2;
            if(rhs1==0)
               r1 += 0.1;
            rhs1 = rhs1/2;
            this->gate[lhs]->in1 = -r1;
            
         }
         else
         {
            rhs1 = rhs1 /2;
            this->gate[lhs]->in1 = rhs1;
         }
         if(this->gate[rhs1]==NULL)   
            gate[rhs1] = new CirUNDEFGate();
         
         this->gate[rhs1]->output.insert(make_pair(lhs,this->gate[lhs]) );
         // cout << rhs1 << " is in1 address: " << (this->gate[lhs])->in1 << endl;
      
      }
   //RHS 2
      if(myStr2Int(input.substr(0,input.find(' ')),rhs2))
      {
         input.erase(0,input.find(' ')+1);
         if (rhs2 % 2 == 1)
         {
            
            rhs2 -= 1;
            double r2 = rhs2/2;
            if(rhs2==0)
               r2 += 0.1;
            rhs2 = rhs2/2;
            this->gate[lhs]->in2 = -r2;
         }
         else
         {
            
            rhs2 = rhs2 /2;
            this->gate[lhs]->in2 = rhs2;
         }
         
         if(this->gate[rhs2]==NULL)
            gate[rhs2] = new CirUNDEFGate();

         this->gate[rhs2]->output.insert(make_pair(lhs,this->gate[lhs]) );
         
            
         //cout << rhs2 << " is in2 address: " << (this->gate[lhs])->in2 << endl;
      
      }
      // cout << "finish" << endl;
   }
//Output to AIG
   for(int cnt =0;cnt<this->POs;cnt++)
   {
      int ap = abs(PO[cnt]->getInputNum());
      if(gate[ap]==nullptr)
         gate[ap] = new CirUNDEFGate();
      this->gate[ap]->output.insert( make_pair(cnt+m+1,this->PO[cnt]) );
   }

//DFS List
	return true;
}


bool
CirMgr::isInverse(int a)const
{
   if(a<0)
      return true;
   return false;
}

CirGate* 
CirMgr::getGate(unsigned id) const
{
   if(id<0)
   {
      return 0;
   }
   if(id>this->m && id <= this->m + this->POs)
   {
      id -= this->m;
      id--;
      return this->PO[id];
   }
   if(id > this->m+this->POs)
      return nullptr;
   else
      return this->gate[id];
}

/**********************************************************/
/*   class CirMgr member functions for circuit printing   */
/**********************************************************/
/*********************
Circuit Statistics
==================
  PI          20
  PO          12
  AIG        130
------------------
  Total      162
*********************/
void
CirMgr::printSummary() const
{
   cout << endl;
   cout << "Circuit Statistics" << endl;
   cout << "==================" << endl;
   cout << "  PI" << setw(12) << right << this->PIs << endl;
   cout << "  PO" << setw(12) << right << this->POs << endl;
   size_t s=0;
   for(unsigned i = 1;i<this->gate.size();i++)
   {
      if(this->gate[i] != NULL&& this->gate[i]->getTypeStr()=='a')
         s++;
   }
   cout << "  AIG" << setw(11) << right << s /*the -1 is for [0]'s ID0 */ << endl;
   cout << "------------------" << endl;
   cout << "  Total" << setw(9) <<right << s + this->PIs +this->POs<< endl;
}


void
CirMgr::printPostOrder(int pt,int &lines)const
{
   //cout << pt << endl;
   if(getGate(pt) == NULL) 
      return;
   if((gate[pt]->getTypeStr()=='u'))
      return;
   if(gate[pt]->printIDX == printidxs)
      return;
   int lef = abs(this->gate[pt]->in1);
   int rig = abs(this->gate[pt]->in2);
   printPostOrder(lef,lines);
   printPostOrder(rig,lines);
   if(gate[pt]->printIDX == printidxs)
      return;
   
   gate[pt]->printIDX = printidxs;
   cout << "[" << lines++ << "] ";
   gate[pt]->printGate();
   cout << endl;
}
void
CirMgr::printNetlist() const
{
   printidxs++; 
   int l =0;
   cout << endl;
   for(int i=0;i<POs;i++)
   {
      int a = abs(PO[i]->getInputNum());
      printPostOrder(a,l);
      cout << "[" << l++ << "] PO  " << i + m + 1  << " ";
      if(gate[a]->getTypeStr()=='u')
         cout << "*";
      if((PO[i])->getInputNum() <0)
         cout << "!";
      cout << a;
      if(!PO[i]->getSymbolicName().empty())
         cout <<  " (" << PO[i]->getSymbolicName() << ")" ;
      cout << endl;
   }
/*
   cout << endl;
   for (unsigned i = 0, n = _dfsList.size(); i < n; ++i) {
      cout << "[" << i << "] ";
      _dfsList[i]->printGate();
   }
*/
}

void
CirMgr::printPIs() const
{
   cout << "PIs of the circuit: ";
   for(int cnt=0;cnt<this->PIs;cnt++)
   {
      if(cnt != this->PIs-1)
         cout << this->PInum[cnt] << " ";
      else
         cout << this->PInum[cnt];
   }
      
   cout << endl;
}

void
CirMgr::printPOs() const
{
   cout << "POs of the circuit: ";
   for(unsigned cnt=0;cnt<this->POs;cnt++)
   {  if(cnt != this->POs-1)
         cout << cnt + this->m+1 << " ";
      else
         cout << cnt + this->m+1;
   }
   cout << endl;
}

void
CirMgr::printFloatGates() const
{
   
   vector<int> ans;
   for(int i=1;i<gate.size();i++)
   {
      // cout << i << endl;
      if(gate[i]!=nullptr && gate[i]->getTypeStr() != 'i')
      {
         double in1 = abs(gate[i]->in1);
         double in2 = abs(gate[i]->in2);
         
         
         if(getGate(in1) != NULL && getGate(in1)->getTypeStr()=='u')
            ans.push_back(in1);
         if(getGate(in2) != NULL && getGate(in2)->getTypeStr()=='u')
            ans.push_back(in2);

         //QUES:
         // Is it possible to have a gate that has in-deg = out-deg = 0??
      }
   }
   for(int i=0;i<PO.size();i++)
   {
      int a = abs(PO[i]->getInputNum());
      if(a < gate.size() && gate[a] && gate[a]->getTypeStr()=='u')
         ans.push_back(i+m+1);
      // cout << i << endl;
   }
   
   if(!ans.empty())
   {
      sort(ans.begin(),ans.end());
      ans.erase(unique(ans.begin(),ans.end()),ans.end());
      cout << "Gates with floating fanin(s):";
      for(int i =0;i<ans.size();i++)
         cout << " " << ans[i] ;
      cout << endl;
      ans.clear();
   }

   // cout << "fs" << endl;
   for(int i=1;i<this->m+1;i++)
   {
      if(this->gate[i]!=NULL)
      {
         if(gate[i]->output.size()==0 && gate[i]->getTypeStr()!='o')
            ans.push_back(i);
      }
   }
   if(!ans.empty())
   {
      sort(ans.begin(),ans.end());
      for(int i=0;i<ans.size();i++)
      {
         if(i==0)
            cout << "Gates defined but not used  :";
         cout << " " << ans[i];
         if(i==ans.size()-1)
            cout << endl;
      }
   }
   
}

void
CirMgr::printFECPairs() const
{
}

void
CirMgr::writeAag(ostream& outfile) const
{
}

void
CirMgr::writeGate(ostream& outfile, CirGate *g) const
{
}

bool
CirMgr::printFanin(int pt,int &depth,int lim,int &tab)const
{
   
   if(depth>lim)
      return false;

   int posPT = abs(pt);
   if(posPT >= gate.size() || pt==INT_MIN)
      return false;

   for(int i=0;i<tab;i++)
      cout << "  ";
   if(isInverse(pt) )
      cout << "!";
   cout.flush();
   gate[posPT]->reportGate();
   if(gate[posPT]->printIDX == printidxs && gate[posPT]->getTypeStr()!= 'i'  && depth !=lim)
   {
      cout << " (*)" << endl;
      return true;
   }
   cout << endl;
   int lef = this->gate[posPT]->in1;
   int rig = this->gate[posPT]->in2;
   depth++;
   tab++;
   
   bool l = printFanin(lef,depth,lim,tab);
   bool r = printFanin(rig,depth,lim,tab);
   if(l || r)
      gate[posPT]->printIDX = printidxs;

   depth--;
   tab--;
   return true;
}
bool
CirMgr::printFanOut(int pt,int &depth,int lim,int &tab,int prev)const
{
   
   if(depth>lim)
      return false;

   int posPT = abs(pt);
   if(posPT >= gate.size() || pt==INT_MIN || this->gate[posPT]==nullptr)
   {
      if(posPT >=gate.size() && posPT <= gate.size()+PO.size())
      {
         posPT -= (m+1);
         for(int i=0;i<tab;i++)
            cout << "  ";
         if(PO[posPT]->getInputNum()<0)
            cout << "!";
         PO[posPT]->reportGate();
         cout << endl;
         return true;
      }
      else
         return false;
   }
      

   for(int i=0;i<tab;i++)
      cout << "  ";
   if(abs(gate[pt]->in1)==prev && gate[pt]->in1<0)
      cout << "!";
   else if(abs(gate[pt]->in2)==prev && gate[pt]->in2<0)
      cout << "!";
   cout.flush();
   gate[posPT]->reportGate();
   if(gate[posPT]->printIDX == printidxs && gate[posPT]->getTypeStr()!= 'o' && depth !=lim)
   {
      cout << " (*)" << endl;
      return true;
   }
   cout << endl;
   depth++;
   tab++;
   int pass = 0;
   for(auto it = gate[posPT]->output.begin();it != gate[posPT]->output.end(); it++)
   {
      int ou = it->second->ID;
      if(printFanOut(ou,depth,lim,tab, gate[posPT]->ID))
         pass++;
   }
   if(pass == gate[posPT]->output.size())
      gate[posPT]->printIDX = printidxs;

   depth--;
   tab--;
   return true;
}

double
CirMgr::inverse(double a)const
{
   double ans = a;
   if( 0 <= a && a < 0.3)
      ans = -0.1;
   else if (-0.3 < a && a < 0)
      ans = 0.0;
   else
      ans = -ans;
   return ans;
}