/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"
#include <algorithm>
#include <stack>

using namespace std;
#define DEBUGM
// TODO: Implement memeber functions for class CirMgr

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;
RandomNumGen rn;
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
              << ": Cannot redefine constant (" << errInt << ")!!" << endl;
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
   this->PO.clear();
   this->gate.clear();
   this->UNDEF.clear();
   this->PInum.clear();
   this->PInum.resize(0);
   this->POnum.clear();
   this->POnum.resize(0);
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
      this->gate.resize(m+1);  
      this->UNDEF.resize(m+1);
      this->UNDEF[0] = false;
      this->m = m;
      //cout << "reserved" << this->gate.max_size() << endl;
   }
   if(myStr2Int(I,i))
   {}
   if(myStr2Int(O,o))
   {}
   if(myStr2Int(A,a))
   {}
//Input
   // cout << "Input gate: " << endl;
   for(int cnt = 1 ; cnt <= m ;cnt++)
   {
      this->gate[cnt] = nullptr;
      this->UNDEF[cnt] = false;
   }
      
   this->PIs = i;
   for(int k = 0;k<i;k++)
   {
      int abc;
      getline(in,input);
      if(myStr2Int(input,abc))
      {
         this->gate[abc/2] = new CirPIGate();
         
         this->gate[abc/2]->isPI = true;
         this->gate[abc/2]->ID = abc/2;
         this->PInum.push_back(abc/2);
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
   this->PO.resize(m+1);
   for(int cnt = 1 ; cnt <= m ;cnt++)
      this->PO[cnt] = nullptr;

   this->POs = o;
   for(int k = 0;k<o;k++)
   {
      getline(in,input);
      int abc;
      if(myStr2Int(input,abc))
      {
         
         if(abc%2 == 1)
         {
            abc-=1;
            this->PO[abc/2] = new CirPOGate(true);
            
         }
         else
            this->PO[abc/2] = new CirPOGate(false);
         this->PO[abc/2]->ID = k+1+m;
         this->POnum.push_back(abc/2);
         this->PO[abc/2]->line = k+1+i+1;
      }
      else
      {
         //handle error
      }
      //cout << k << endl;
   }
//AIG
   // cout << "AIG" << endl;
   this->gate[0] = new CirPIGate();
   for(int k = 0;k<a;k++)
   {
      getline(in,input);
      int lhs,rhs1,rhs2;
      
      //erasing And gate NO:
      if(myStr2Int(input.substr(0,input.find(' ')),lhs)) 
      {
         this->gate[lhs/2] = new CirAIGGate();
         
         this->gate[lhs/2]->ID = lhs/2;
         this->gate[lhs/2]->isAIG = true;
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
//Output to AIG
   for(int cnt =0;cnt<this->PO.size();cnt++)
   {
      if(this->PO[cnt]!=nullptr && this->gate[cnt]!=NULL)
      {
         this->gate[cnt]->output.push_back(this->PO[cnt]);
         static_cast<CirPOGate* >(this->PO[cnt])->input = this->gate[cnt];
      }
   }
//Handle symbolic name
   //cout << "handle" << endl;
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
            // cout << input << endl;
            (this->gate[abc])->SymbolicName = input;
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
            abc = this->POnum[abc];
            
            input.erase(0,input.find(' ')+1);
            // cout << input << endl;
            (this->PO[abc])->SymbolicName = input;
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
   for(int cnt = 0 ;cnt<1+this->PInum.size()+this->POnum.size();cnt++)
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
            this->gate[lhs]->inverted1 = true;
         }
         rhs1 = rhs1 /2;
         this->gate[lhs]->in1 = rhs1;
         if(this->gate[rhs1]!=NULL)
            this->gate[rhs1]->output.push_back(this->gate[lhs]);
         else
            this->UNDEF[rhs1] = true;
         // cout << rhs1 << " is in1 address: " << (this->gate[lhs])->in1 << endl;
      
      }
   //RHS 2
      if(myStr2Int(input.substr(0,input.find(' ')),rhs2))
      {
         input.erase(0,input.find(' ')+1);
         if (rhs2 % 2 == 1)
         {
            rhs2 -= 1;
            this->gate[lhs]->inverted2 = true;
         }
         rhs2 = rhs2 /2;
         this->gate[lhs]->in2 = rhs2;
         if(this->gate[rhs2]!=NULL)
            this->gate[rhs2]->output.push_back(this->gate[lhs]);
         else
            this->UNDEF[rhs2] = true;
         
         //cout << rhs2 << " is in2 address: " << (this->gate[lhs])->in2 << endl;
      
      }
      //cout << "finish" << endl;
   }

	return true;
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
   cout << "  PI" << setw(12) << right << this->PInum.size() << endl;
   cout << "  PO" << setw(12) << right << this->POnum.size() << endl;
   size_t s=0;
   for(unsigned i = 1;i<this->m+1;i++)
      if((this->gate[i])!=NULL && this->gate[i]->isAIG)
         s++;
   cout << "  AIG" << setw(11) << right << s /*the -1 is for [0]'s ID0 */ << endl;
   cout << "------------------" << endl;
   cout << "  Total" << setw(9) <<right << s + this->POs + this->PIs << endl;
}


void
CirMgr::printNetlist() const
{
   vector<bool > visited(this->m+1,false);
   vector<bool > printed(this->m+1,false);
   int lines = 0;

   for(int pos = 0;pos<this->POs;pos++)
   {
      if(lines == 0)
         cout << endl;
      int abc = this->POnum[pos];
      //check abc is what kind of num?
      stack<int > st; 
      
      CirGate* cur = static_cast<CirPOGate*>(this->PO[abc])->input;
      

      st.push(cur->ID);
      do
      { 
         // Move to leftmost node 
         while (cur != NULL && cur->isPI==false && cur->ID != 0 && visited[cur->ID] == false) 
         { 
            //cout << cur->ID << endl;
            // Push root's right child and then root to stack. 
            if (this->gate[(cur)->in2] != NULL ) 
               st.push((cur)->in2);
            st.push(cur->ID);

            // Set root as root's left child   
            cur = this->gate[(cur)->in1];
            
         } 

         if(cur!=NULL && cur->ID!=0)
         {
            size_t dis = cur->ID;
            
            if(visited[dis]==false)
            {
               cout << "[" << lines++ << "] " << "PI  " << dis ;
               if(!cur->SymbolicName.empty() && printed[dis] == false)
                  cout << " (" << cur->SymbolicName << ")" ;
               printed[dis] = true;
               cout << endl;
               visited[dis] = true;
            }
         }
         else if(cur!= NULL && cur->ID == 0 && visited[0]==false)
         {
            cout << "[" << lines++ << "] " << "CONST0" << endl;
            visited[0] = true;
         }
            
         cur = this->gate[st.top()];
         st.pop();

         if ( cur && this->gate[(cur)->in2] && !st.empty() && this->gate[st.top()] == this->gate[(cur)->in2]) 
         { 
            st.pop(); 
            st.push(cur->ID);
            cur = this->gate[(cur)->in2]; 
         } 
         else  // Else print root's data and set root as NULL 
         { 
            // cout << "ohy" << endl;
            size_t dis = cur->ID;
            if(visited[dis]==false && dis!=0)
            {
               cout << "[" << lines++ << "] " << "AIG " << dis << " ";
               cout.flush();
               if(this->gate[cur->in1] == NULL)
                  cout << "*";
               if((cur)->inverted1)
                  cout << "!";
               cout.flush();
               size_t _dis = cur->in1;
               cout << _dis << " ";

               cout.flush();
               if(this->gate[cur->in2] == NULL)
                  cout << "*";
               if((cur)->inverted2)
                  cout << "!";
               _dis =  cur->in2;
               cout << _dis;
               if(this->gate[cur->in1] != NULL && !this->gate[cur->in1]->SymbolicName.empty() && printed[cur->in1] == false)
                  cout << " (" <<this->gate[cur->in1]->SymbolicName << ")" ;
               printed[cur->in1] = true;
               cout.flush();
               if(this->gate[cur->in2] != NULL && !this->gate[cur->in2]->SymbolicName.empty() && printed[cur->in2] == false)
                  cout << " (" << this->gate[cur->in2]->SymbolicName << ")";
               printed[cur->in2] = true;
               cout << endl;
               visited[dis] = true;
            }

            cur = NULL;
         } 
      } while (!st.empty()); 
      // Check for empty tree 
      cur = static_cast<CirPOGate*>(this->PO[abc])->input;
      

      cout << "[" << lines++ << "] PO  " << this->m + pos+1 << " ";
      if(static_cast<CirPOGate*>(this->PO[abc])->inverted)
         cout << "!";
      cout << cur->ID;
      if(!this->PO[abc]->SymbolicName.empty())
         cout << " (" <<  this->PO[abc]->SymbolicName << ")";
      cout << endl;
   }
}


void
CirMgr::printPIs() const
{
   cout << "PIs of the circuit: ";
   for(unsigned cnt=0;cnt<this->PIs;cnt++)
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
   for(int i=1;i<this->m+1;i++)
   {
      if(this->gate[i] !=NULL && this->gate[i]->isAIG)
      {
         int i1 = this->gate[i]->in1;
         int i2 = this->gate[i]->in2;
         
         if(this->gate[i1] == NULL ||
            this->gate[i2] == NULL)
            {
               ans.push_back(i);
            }
      }
   }
   for(int i=0;i<this->POs;i++)
   {
      if(this->PO[i]!=NULL && dynamic_cast<CirPOGate* >(this->PO[i])->input == NULL)
      {
         ans.push_back(i);
      }
   }
   sort(ans.begin(),ans.end());
   for(int i=0;i<ans.size();i++)
   {
      if(i==0)
         cout << "Gates with floating fanin(s):";
      cout << " " << ans[i];
      if(i==ans.size()-1)
         cout << endl;
   }
      
   

   
   ans.clear();
   
   for(int i=1;i<this->m+1;i++)
   {
      if(this->gate[i]!=NULL)
      {
         if(this->gate[i]->isAIG)
         {
            if(dynamic_cast<CirAIGGate* >(this->gate[i])->output.size()==0)
               ans.push_back(i);
         }
         else
            if(dynamic_cast<CirPIGate* >(this->gate[i])->output.size()==0)
               ans.push_back(i);
      }
   }
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

void
CirMgr::writeAag(ostream& outfile) const
{
   vector<bool > visited(this->m+1,false);
   vector<bool > printed(this->m+1,false);
   int lines = 0;
   outfile << "aag " << this->m << " " << this->PIs << " 0 " << this->POs;
   ostringstream tmp;
      
   
   for(int pos = 0;pos<this->POs;pos++)
   {

      int abc = this->POnum[pos];
      //check abc is what kind of num?
      stack<int > st; 
      
      CirGate* cur = static_cast<CirPOGate*>(this->PO[abc])->input;
      

      st.push(cur->ID);
      do
      { 
         // Move to leftmost node 
         while (cur != NULL && cur->isPI==false && cur->ID != 0 && visited[cur->ID] == false) 
         { 
            //cout << cur->ID << endl;
            // Push root's right child and then root to stack. 
            if (this->gate[(cur)->in2] != NULL ) 
               st.push((cur)->in2);
            st.push(cur->ID);

            // Set root as root's left child   
            cur = this->gate[(cur)->in1];
            
         } 

         if(cur!=NULL && cur->ID!=0)
         {
            size_t dis = cur->ID;
            
            if(visited[dis]==false)
            {

               printed[dis] = true;
               visited[dis] = true;
            }
         }
         else if(cur!= NULL && cur->ID == 0 && visited[0]==false)
         {
            visited[0] = true;
         }
            
         cur = this->gate[st.top()];
         st.pop();

         if ( cur && this->gate[(cur)->in2] && !st.empty() && this->gate[st.top()] == this->gate[(cur)->in2]) 
         { 
            st.pop(); 
            st.push(cur->ID);
            cur = this->gate[(cur)->in2]; 
         } 
         else  // Else print root's data and set root as NULL 
         { 
            // cout << "ohy" << endl;
            size_t dis = cur->ID;
            if(visited[dis]==false && dis!=0)
            {
               tmp <<  dis*2 << " ";

               size_t _dis = cur->in1;
               if(cur->inverted1)
                  tmp << _dis*2+1 << " ";
               else
                  tmp << _dis*2 << " ";

               _dis =  cur->in2;
                if(cur->inverted2)
                  tmp << _dis*2+1 ;
               else
                  tmp << _dis*2 ;
               tmp << endl;
               printed[cur->in1] = true;
               
               printed[cur->in2] = true;
               visited[dis] = true;
               lines ++;
            }

            cur = NULL;
         } 
      } while (!st.empty()); 
      // Check for empty tree 
      cur = static_cast<CirPOGate*>(this->PO[abc])->input;
   }
   outfile << " " << lines << endl;
   for(int i=0;i<this->PIs;i++)
      outfile << this->PInum[i]*2 << endl;
   for(int i=0;i<this->POs;i++)
   {
      if(static_cast<CirPOGate* >(this->PO[this->POnum[i]])->inverted)
         outfile << this->POnum[i]*2+1 << endl;
      else
         outfile << this->POnum[i]*2 << endl;
   }
      
   
   outfile << tmp.str();
   for(int i=0;i<this->PIs;i++)
   {
      if(!this->gate[PInum[i]]->SymbolicName.empty())
         outfile << "i" << i << " " << this->gate[PInum[i]]->SymbolicName << endl;
   }
      
   for(int i=0;i<this->POs;i++)
   {
      if(!this->PO[POnum[i]]->SymbolicName.empty())
         outfile << "o" << i << " " << this->PO[POnum[i]]->SymbolicName << endl;
   }
      
   outfile << "c" << endl;
   outfile << "DSnP is the hardest class on EARTH" << endl;
   outfile << "And I hate it as much as I love it" << endl;
}

CirGate*
CirMgr::getGate(unsigned id ) const
{
   // m is 1-some number
   if(id<0)
   {
      //handle error
   }
   if(id>this->m && id <= this->m + this->POs)
   {
      id -= this->m;
      id--;
      id = this->POnum[id];
      return this->PO[id];
   }
   if(id > this->m+this->POs)
      return nullptr;
   if(this->gate[id]==nullptr && this->UNDEF[id]==true)
   {
      CirGate* tmp = new CirGate();
      tmp->in1 = -9487;
      tmp->ID = id;
      return tmp;
   }
   else if(this->gate[id]==nullptr)
      return nullptr;
   else
      return this->gate[id];
}