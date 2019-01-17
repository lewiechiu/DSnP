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
#include "sat.h"
#include <map>

using namespace std;

// TODO: Feel free to define your own classes, variables, or functions.

class CirGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
class CirGate
{
  friend class CirMgr;
public:
    CirGate(){}
    CirGate(float in):in1(in){}
    ~CirGate() 
    {
        output.clear();
        in1 = in2 = INT_MIN;
        line = 0;
    }

    // Basic access methods
    
    unsigned getLineNo() const { return line; }
    virtual const char getTypeStr() const {};
    virtual string getSymbolicName() const{};
    virtual void setSymbolicName(string s) {};
    virtual float getInputNum() const{};
    virtual size_t genResult() const{};

    // Printing functions
    virtual void printGate() const{};
    void reportGate() const;
    void myreportGate() const;
    void reportFanin(int level) const;
    void reportFanout(int level) const;
    
protected:
    int ID = 0;
    
    double in1 = INT_MIN;
    double in2 = INT_MIN;
    int line = 0;
    int printIDX = 0;
    map<int, CirGate* > output;

};
class CirAIGGate : public  CirGate
{
    friend class CirMgr;
    friend class CirGate;
    size_t sig1;
    size_t sig2;
public:
    
    CirAIGGate() :CirGate(){}
    ~CirAIGGate() 
    {
        output.clear();
        in1 = 0;
        in2 = 0;
    }
    const char getTypeStr() const{ return 'a';}
    size_t genResult() const;
protected:
    void printGate() const;
    void setOutput(const size_t a, const size_t b);
    size_t result()const;
};
class CirPIGate : public CirGate
{
    friend class CirMgr;
    friend class CirGate;

    size_t inSig = 0;

    public:
    CirPIGate() : CirGate() {}
    ~CirPIGate() 
    {
        output.clear();
    }
    void printGate() const;
    const char getTypeStr() const{ return 'i';}
    string getSymbolicName() const{return SymbolicName;}
    void setSymbolicName(string s) {SymbolicName = s; return ;};
    bool isAig()const{return false;}
    size_t genResult() const;

protected:
    string SymbolicName;
};

class CirPOGate : public CirGate
{
    friend class CirMgr;
    friend class CirGate;
    size_t result;
    public:
    CirPOGate(): CirGate(){}
    CirPOGate(float in): CirGate(in){}
    ~CirPOGate() 
    {}
    const char getTypeStr() const{ return 'o';};
    string getSymbolicName() const{return SymbolicName;};
    void setSymbolicName(string s) {SymbolicName = s; return ;};
    float getInputNum()const {return in1;};
    size_t genResult() const;


protected:
    
    void printGate() const{};
    void setresult(size_t a);
    string SymbolicName;
};

class Cir0Gate : public CirGate
{
    friend class CirMgr;
public:
    Cir0Gate() : CirGate(){}
    ~Cir0Gate(){}
    const char getTypeStr() const{ return '0';}
    void printGate() const;
    size_t genResult() const;
};

class CirUNDEFGate : public CirGate
{
    friend class CirMgr;
public:
    CirUNDEFGate()  : CirGate() {}
    ~CirUNDEFGate() {}
    const char getTypeStr() const{return 'u';};
    void printGate() const{};
    size_t genResult() const {return 0;};
};

#endif // CIR_GATE_H
