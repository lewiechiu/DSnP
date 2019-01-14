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
    virtual string getTypeStr() const {};
    unsigned getLineNo() const { return line; }

    // Printing functions
    virtual void printGate() const{};
    void reportGate() const;
    void reportFanin(int level) const;
    void reportFanout(int level) const;
    virtual string getSymbolicName() const{};
    virtual vector<CirGate* >& getOutput() const;
protected:
    size_t ID = 0;
    
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
    string getTypeStr() const{ return "CirAIGGate";}
    
protected:
    void printGate() const{};
    void setInGate(CirGate* i1, CirGate* i2);
    void setInInvGate(bool i1, bool i2);
    void setOutput(const vector<CirGate* > & out);
    vector<CirGate* >& getOutput() const{return &output};
    vector<CirGate* > output;

    
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
    string getTypeStr() const{ return "CirPIGate";};
    string getSymbolicName() const{return SymbolicName;}
protected:
    vector<CirGate* > output;
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
    string getTypeStr() const{ return "CirPOGate";}
    string getSymbolicName() const{return SymbolicName;}
protected:
    
    void printGate() const{};
    CirGate* input;
    bool inverted;
    
};


#endif // CIR_GATE_H
