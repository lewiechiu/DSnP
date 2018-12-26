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

using namespace std;

#include "cirDef.h"

extern CirMgr *cirMgr;

// TODO: Define your own data members and member functions
class CirMgr
{
public:
    CirMgr(){}
    ~CirMgr() {}

    // Access functions
    // return '0' if "gid" corresponds to an undefined gate.
    CirGate* getGate(unsigned gid) const;

    // Member functions about circuit construction
    bool readCircuit(const string&);

    // Member functions about circuit reporting
    void printSummary() const;
    void printNetlist() const;
    void printPIs() const;
    void printPOs() const;
    void printFloatGates() const;
    void writeAag(ostream&) const;
    int getGateSize() const{return this->m;}

private:
    vector<CirGate* > PO;
    vector<CirGate* > gate;
    vector<int> PInum;
    vector<int> POnum;
    vector<bool> UNDEF;
    int PIs;
    int POs;
    int m;
    CirGate* CONSTANT0;
    
    void printGate()const;
    void findUNDEF()const;
};

#endif // CIR_MGR_H
