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
#include <stack>
#include <list>

using namespace std;

extern CirMgr *cirMgr;

// TODO: Implement memeber functions for class(es) in cirGate.h

/**************************************/
/*   class CirGate member functions   */
/**************************************/
void
CirGate::reportGate() const
{
   cout << "==================================================" << endl;
   ostringstream out;
   out << "= ";

   if(this->in1 == -9487)
   {
      out << "UNDEF(" << ID << "), line 0";
   }
   else if(this->isAIG == false && this->isPI == false && this->ID >0)
   {
      out << "PO(" << this->ID << ")";
      if(!this->SymbolicName.empty())
         out <<"\"" << this->SymbolicName << "\"";
      out << ", line " << this->line;
   }
   else if (this->isAIG == true && this->ID >0)
   {
      out << "AIG(" <<this->ID << "), line ";
      out << this->line;
   }
   else if(this->isPI== true)
   {
      out << "PI(" << this->ID << ")";
      if(!this->SymbolicName.empty())
         out <<"\"" << this->SymbolicName << "\"";
      out << ", line " << this->line;
   }
   else if (this->ID==0)
   {
      out << "CONST(0), line 0";
   }
   
   string st = out.str();
   cout << st;
   cout << setw(48 - st.length()+2) <<  right << "=";
   cout << endl;
   cout << "==================================================" << endl;
}

void
CirGate::reportFanin(int level) const
{
   assert (level >= 0);
   
   CirGate *cur = cirMgr->getGate(ID);
   vector<bool > visited;
   list<int > LEVEL;
   stack<pair<bool,int > > st; 
   int depth=0;
   
   st.push(make_pair(true,ID));
   int m = cirMgr->getGateSize();
   visited.resize(m+1);
   LEVEL.push_back(depth);
   

   for(int i=0;i<=m;i++)
      visited[i] = false;
   
   while(!st.empty())
   {
      cur = cirMgr->getGate(st.top().second);
      bool inversionsadsfaxvc = st.top().first;
      st.pop();
      depth = LEVEL.front();
      LEVEL.pop_front();
      bool star = false;
      if(cur)
      {
         for(int x=0;x<depth;x++)
            cout << "  ";
         if(inversionsadsfaxvc && depth != 0)
            cout << "!";
         if(cur->in1 == -9487)
            cout << "UNDEF ";
         else if (cur->ID==0)
            cout << "CONST 0";
         else if(cur->isPI)
            cout << "PI ";
         else if(cur->isPI == false && cur->isAIG == false)
         {
            cout << "PO ";
            if(depth+1 <= level && !star)
            {
               st.push(make_pair(static_cast<CirPOGate* >(cur)->inverted ,static_cast<CirPOGate* >(cur)->input->ID));
               LEVEL.push_front(depth+1);
               
               visited[cur->ID] = true;
            }
         }
            
         else
         {
            
            cout << "AIG ";
            if(visited[cur->ID] && depth != level)
            {
               star = true;

            }
            if(depth+1 <= level && !star)
            {
               st.push(make_pair(cur->inverted2,cur->in2));
               LEVEL.push_front(depth+1);
               st.push(make_pair(cur->inverted1,cur->in1));
               LEVEL.push_front(depth+1);
               visited[cur->ID] = true;
            }
         }
         
         if(!cur->isAIG)
            visited[cur->ID] = true;
         if(cur->ID!=0)
            cout << cur->ID;
         if(star)
            cout << " (*)";
         cout << endl;
      }
   }
}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
   
   const CirGate *cur = this;
   vector<bool > visited;
   list<int > LEVEL;
   stack<pair<bool,const CirGate* > > st; 
   int depth=0;
   
   st.push(make_pair(true,cur));
   int m = cirMgr->getGateSize();
   visited.resize(m+1);
   LEVEL.push_back(depth);
   

   for(int i=0;i<=m;i++)
      visited[i] = false;
   
   while(!st.empty())
   {
      cur = cirMgr->getGate(st.top().second->ID);
      bool inversionsadsfaxvc = st.top().first;
      
      st.pop();
      depth = LEVEL.front();
      LEVEL.pop_front();
      bool star = false;
      if(cur)
      {
         for(int x=0;x<depth;x++)
            cout << "  ";
         if(inversionsadsfaxvc && depth != 0)
            cout << "!";
         if(cur->in1 == -9487)
            cout << "UNDEF ";
         else if (cur->ID==0)
            cout << "CONST 0";
         else if(cur->isPI == false && cur->isAIG == false)
            cout << "PO ";
         else
         {
            if(cur->isPI)
               cout << "PI ";
            else
               cout << "AIG ";
            if(visited[cur->ID] && depth != level)
            {
               star = true;

            }
            if(depth+1 <= level && !star)
            {
               size_t s = cur->output.size();
               for(int i = s-1;i >=0;i--)
               {
                  
                  if(cur->output[i]->isAIG)
                  {
                     // cout << "in1: " << cur->output[i]->in1 << endl;
                     // cout << "in2: " << cur->output[i]->in2 << endl;
                     
                     if(cur->output[i]->in1 == ID)
                        inversionsadsfaxvc = cur->output[i]->inverted1;
                     else
                        inversionsadsfaxvc = cur->output[i]->inverted2;
                  }
                  else
                     inversionsadsfaxvc = static_cast<CirPOGate*>(cur->output[i])->inverted;
                     
                     
                  st.push(make_pair(inversionsadsfaxvc,cur->output[i]));
                  LEVEL.push_front(depth+1);
                  visited[cur->ID] = true;
               }
            }
         }
         
         if(!cur->isAIG)
            visited[cur->ID] = true;
         if(cur->ID!=0)
            cout << cur->ID;
         if(star)
            cout << " (*)";
         cout << endl;
      }
   }
}



/**************************************/
/* class CirPIGate member functions   */
/**************************************/

/**************************************/
/* class CirPOGate member functions   */
/**************************************/

/**************************************/
/* class CirPAIGGate member functions */
/**************************************/
