/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"

using namespace std;

class CirGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
// TODO: Define your own data members and member functions, or classes
class CirGate
{
  friend class CirMgr;
public:
    // CirGate(){}
    CirGate() {}
    ~CirGate() {}

    // Basic access methods
    string getTypeStr() const { return ""; }
    unsigned getLineNo() const { return 0; }

    // Printing functions
    virtual void printGate() const{};
    void reportGate() const;
    void reportFanin(int level) const;
    void reportFanout(int level) const;
protected:
    size_t ID = 0;
    vector<CirGate* > output;
    int in1 = -1;
    int in2 = -1;
    int line = 0;
    bool isAIG = false;
    bool isPI = false;
    bool inverted2 = false;
    bool inverted1 = false;
    
    string SymbolicName;
    
    
    
    

};
class CirAIGGate : public  CirGate
{
    friend class CirMgr;
    friend class CirGate;
    public:
    CirAIGGate() :CirGate(){}
    ~CirAIGGate() 
    {
        in1 = 0;
        in2 = 0;
        output.clear();
    }
protected:
    void printGate() const{};
    void setInGate(CirGate* i1, CirGate* i2);
    void setInInvGate(bool i1, bool i2);
    void setOutput(const vector<CirGate* > & out);


    
};
class CirPIGate : public CirGate
{
    friend class CirMgr;
    friend class CirGate;
    public:
    CirPIGate() : CirGate() {}
    ~CirPIGate() 
    {
        output.clear();
    }
    void printGate() const{};
protected:
};

class CirPOGate : public CirGate
{
    friend class CirMgr;
    friend class CirGate;
    public:
    CirPOGate(): input(nullptr){}
    CirPOGate(bool INV): inverted(INV){}
    ~CirPOGate() 
    {
        input = nullptr;
    }
protected:
    
    void printGate() const{};
    CirGate* input;
    bool inverted;
    
};


#endif // CIR_GATE_H
