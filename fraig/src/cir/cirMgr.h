/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "../util/myHashSet.h"

using namespace std;

// TODO: Feel free to define your own classes, variables, or functions.

#include "cirDef.h"

extern CirMgr *cirMgr;

class CirMgr
{
public:
   CirMgr() :POnum(0),PInum(0){}
   ~CirMgr() {} 

   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned gid) const;
   int getGateSize()const{return m;};

   // Member functions about circuit construction
   bool readCircuit(const string&);

   // Member functions about circuit optimization
   void sweep();
   void optimize();

   // Member functions about simulation
   void randomSim();
   void fileSim(ifstream&);
   void setSimLog(ofstream *logFile) { _simLog = logFile; }

   // Member functions about fraig
   void strash();
   void printFEC() const;
   void fraig();

   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void printFECPairs() const;
   void writeAag(ostream&) const;
   void writeGate(ostream&, CirGate*) const;
   
   //helper function
   void printPostOrder(int pt,int &lines)const;
   void postOrderTraverse(int pt)const;
   bool printFanin(int pt,int &depth,int lim,int &tab)const;
   bool printFanOut(int pt,int &depth,int lim,int &tab,int prev)const;
   bool isInverse(int a)const;
   void markSweep(int a);
   void opt(int a);
   double inverse(double a)const;
   void trashDFS(int pt);

   //non related helper function
protected:
   
private:
   ofstream           *_simLog;
   vector<CirGate* > PO;
   vector<CirGate* > gate;
   int* POnum;
   int* PInum;
   int POs;
   int PIs;
   int m;
   HashSet<myKey> hs;
};

#endif // CIR_MGR_H
