/****************************************************************************
  FileName     [ cirDef.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic data or var for cir package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2012-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_DEF_H
#define CIR_DEF_H

#include <vector>
#include "myHashMap.h"
#include <algorithm>
using namespace std;

// TODO: define your own typedef or enum
extern int printidxs;
class CirGate;
class CirMgr;
class SatSolver;
#define zero INT_MAX-3;
#define zeroi (INT_MAX*-1)+3
#define mynull 1000000000
#include <list>
#include <set>



#endif // CIR_DEF_H
