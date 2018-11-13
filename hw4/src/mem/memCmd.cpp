/****************************************************************************
  FileName     [ memCmd.cpp ]
  PackageName  [ mem ]
  Synopsis     [ Define memory test commands ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <iomanip>
#include "memCmd.h"
#include "memTest.h"
#include "cmdParser.h"
#include "util.h"

using namespace std;

extern MemTest mtest;  // defined in memTest.cpp

bool
initMemCmd()
{
   if (!(cmdMgr->regCmd("MTReset", 3, new MTResetCmd) &&
         cmdMgr->regCmd("MTNew", 3, new MTNewCmd) &&
         cmdMgr->regCmd("MTDelete", 3, new MTDeleteCmd) &&
         cmdMgr->regCmd("MTPrint", 3, new MTPrintCmd)
      )) {
      cerr << "Registering \"mem\" commands fails... exiting" << endl;
      return false;
   }
   return true;
}


//----------------------------------------------------------------------
//    MTReset [(size_t blockSize)]
//----------------------------------------------------------------------
CmdExecStatus
MTResetCmd::exec(const string& option)
{
   // check option
   string token;
   if (!CmdExec::lexSingleOption(option, token))
      return CMD_EXEC_ERROR;
   if (token.size()) {
      int b;
      if (!myStr2Int(token, b) || b < int(toSizeT(sizeof(MemTestObj)))) {
         cerr << "Illegal block size (" << token << ")!!" << endl;
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
      #ifdef MEM_MGR_H
      mtest.reset(toSizeT(b));
      #else
      mtest.reset();
      #endif // MEM_MGR_H
   }
   else
      mtest.reset();
   return CMD_EXEC_DONE;
}

void
MTResetCmd::usage(ostream& os) const
{  
   os << "Usage: MTReset [(size_t blockSize)]" << endl;
}

void
MTResetCmd::help() const
{  
   cout << setw(15) << left << "MTReset: " 
        << "(memory test) reset memory manager" << endl;
}


//----------------------------------------------------------------------
//    MTNew <(size_t numObjects)> [-Array (size_t arraySize)]
//----------------------------------------------------------------------
CmdExecStatus
MTNewCmd::exec(const string& option)
{
  // TODO
  string _opt = option;
  while(_opt[0]==' ')
  {_opt.erase(0,1);}
  int spaceCnt = 0;
  string firstP, SecP, ThirdP;
  while(_opt.find(' ')!=string::npos)
  {
    _opt[_opt.find(' ')] = '*';
    spaceCnt++;
  }
  string nu = _opt.substr(0,_opt.find(' '));
  int numObj = 0;
  int arrayS = -1;
  if(spaceCnt == 2)
  {
    firstP = _opt.substr(0,_opt.find('*'));
    _opt.erase(0,firstP.length()+1);
    SecP = _opt.substr(0,_opt.find('*'));
    _opt.erase(0,SecP.length()+1);
    ThirdP = _opt;
    string token = "Aa";
    if(firstP.find_first_of(token)!=string::npos)
    {
      // -a 2 100   100 * size2 arry
      if(!myStr2Int(SecP,arrayS) || !myStr2Int(ThirdP,numObj))
      {
        if(!myStr2Int(SecP,arrayS))
          cerr << "Error: Illegal option!! (" << SecP << ")" << endl;
        else
          cerr << "Error: Illegal option!! (" << ThirdP << ")" << endl;
        return CMD_EXEC_ERROR;
      }
      if(arrayS<=0 || numObj <= 0)
      {
        if(arrayS <= 0)
          cerr << "Error: Illegal option!! (" << arrayS << ")" << endl;
        else
          cerr << "Error: Illegal option!! (" << numObj << ")" << endl;
        return CMD_EXEC_ERROR;
      }
    }
    else if(SecP.find_first_of(token)!=string::npos)
    {
      // 100 -a 2
      if(!myStr2Int(firstP,numObj) || !myStr2Int(ThirdP,arrayS))
      {
        if(!myStr2Int(firstP,arrayS))
          cerr << "Error: Illegal option!! (" << firstP << ")" << endl;
        else
          cerr << "Error: Illegal option!! (" << ThirdP << ")" << endl;
        return CMD_EXEC_ERROR;
      }
      if(arrayS<=0 || numObj <= 0)
      {
        if(arrayS <= 0)
          cerr << "Error: Illegal option!! (" << arrayS << ")" << endl;
        else
          cerr << "Error: Illegal option!! (" << numObj << ")" << endl;
        return CMD_EXEC_ERROR;
      }
    }
    else
    {
      cerr << "Error: Extra option!! (" << SecP << ")" << endl;
      return CMD_EXEC_ERROR;
    }
    try
    {
      mtest.newArrs(numObj,arrayS);
    }
    catch(bad_alloc)
    {
      return CMD_EXEC_ERROR;
    }
  }
  else if(spaceCnt == 0)
  {
    if(!myStr2Int(_opt,numObj))
    {
      return CMD_EXEC_ERROR;
    }
    if(numObj<=0)
    {
      cerr << "Error: Illegal option!! (" << numObj << ")" << endl;
      return CMD_EXEC_ERROR;
    }
    try
    {
      mtest.newObjs(numObj);
    }
    catch(bad_alloc)
    {
      return CMD_EXEC_ERROR;
    }
  }
  else
  {
    return CMD_EXEC_ERROR;
  }
   return CMD_EXEC_DONE;
}

void
MTNewCmd::usage(ostream& os) const
{  
   os << "Usage: MTNew <(size_t numObjects)> [-Array (size_t arraySize)]\n";
}

void
MTNewCmd::help() const
{  
   cout << setw(15) << left << "MTNew: " 
        << "(memory test) new objects" << endl;
}


//----------------------------------------------------------------------
//    MTDelete <  -Index (size_t objId) | -Random (size_t numRandId)> [-Array]
//----------------------------------------------------------------------
CmdExecStatus
MTDeleteCmd::exec(const string& option)
{
  string _opt = option;
  while(_opt[0]==' ')
  {_opt.erase(0,1);}
  int spaceCnt = 0;
  string tokeni = "iI";
  string tokenr = "rR";
  string tokena = "Aa";
  string firstP, SecP, ThirdP;
  while(_opt.find(' ')!=string::npos)
  {
    _opt[_opt.find(' ')] = '*';
    spaceCnt++;
  }
  string nu = _opt.substr(0,_opt.find(' '));
  int numObj = -1;
  int arrayS = -1;
  if(spaceCnt == 1)
  {
    //it must be -i()# or -r()#
    firstP = _opt.substr(0,_opt.find('*'));
    _opt.erase(0,firstP.length()+1);
    SecP = _opt;
    //cout << SecP << endl;
    if(firstP.find_first_of(tokeni)!=string::npos)
    {
      // -i 
      if(myStr2Int(SecP,numObj))
      {
        if(numObj>=0 && numObj <= mtest.getObjListSize())
        {
          mtest.deleteObj(numObj);
        }
        else
          return CMD_EXEC_ERROR;
      }
    }
    else if(firstP.find_first_of(tokenr)!=string::npos)
    {
      // -r () #
      if(myStr2Int(SecP,numObj))
      {
        for(int i=0;i<numObj;i++)
        {
          mtest.deleteObj(rnGen(mtest.getObjListSize()));
        }
      
      }
    }
    
  }
  else if(spaceCnt == 2)
  {
    firstP = _opt.substr(0,_opt.find('*'));
    _opt.erase(0,firstP.length()+1);
    SecP = _opt.substr(0,_opt.find('*'));
    _opt.erase(0,SecP.length()+1);
    ThirdP = _opt;
    
    if(SecP.find_first_of(tokeni)!=string::npos)
    {
      // check the case // -a () -i () #
      if(firstP.find_first_of(tokena)==string::npos)
      {
        return CMD_EXEC_ERROR;
      }
      if(myStr2Int(ThirdP,numObj))
      {
        //cout << mtest.getArrListSize() << endl;
        //cout << numObj << endl;
        if(numObj >=0 &&(numObj <=(mtest.getArrListSize())))
        {
          cout << mtest.getArrListSize() << endl;
          cout << numObj << endl;
          mtest.deleteArr(numObj);
        }
        else
        {
          return CMD_EXEC_ERROR;
        }
      }
      else
        return CMD_EXEC_ERROR;
    }
    else if(SecP.find_first_of(tokenr)!=string::npos)
    {
      // check the case -a () -r () #
      if(firstP.find_first_of(tokena)==string::npos)
      {
        return CMD_EXEC_ERROR;
      }
      if(myStr2Int(ThirdP,numObj))
      {
        
        for(int i=0;i<numObj;i++)
        {
          if(mtest.getArrListSize()==0)
            return CMD_EXEC_ERROR;
          mtest.deleteArr(rnGen(mtest.getArrListSize()));
        }
      }
      else
        return CMD_EXEC_ERROR;
    }
    else if(firstP.find_first_of(tokeni)!=string::npos)
    {
      // check the case // -i () # () -a
      if(ThirdP.find_first_of(tokena)==string::npos)
      {
        return CMD_EXEC_ERROR;
      }
      if(myStr2Int(SecP,numObj))
      {
        
        if(numObj >=0 && numObj <=mtest.getArrListSize())
        {
          mtest.deleteArr(numObj);
        }
      }
      else
        return CMD_EXEC_ERROR;
    }
    else if(firstP.find_first_of(tokenr)!=string::npos)
    {
      // check the case  // -r () # () -a
      if(ThirdP.find_first_of(tokena)==string::npos)
      {
        return CMD_EXEC_ERROR;
      }
      if(myStr2Int(SecP,numObj))
      {
        
        
        for(int i=0;i<numObj;i++)
        {
          if(mtest.getArrListSize()==0)
            return CMD_EXEC_ERROR;
          mtest.deleteArr(rnGen(mtest.getArrListSize()));
          //cout << numObj << endl;
        }
      }
      else
        return CMD_EXEC_ERROR;
    }
  }
  else
  {
    return CMD_EXEC_ERROR;
  }


    return CMD_EXEC_DONE;
}

void
MTDeleteCmd::usage(ostream& os) const
{  
   os << "Usage: MTDelete <-Index (size_t objId) | "
      << "-Random (size_t numRandId)> [-Array]" << endl;
}

void
MTDeleteCmd::help() const
{  
   cout << setw(15) << left << "MTDelete: " 
        << "(memory test) delete objects" << endl;
}


//----------------------------------------------------------------------
//    MTPrint
//----------------------------------------------------------------------
CmdExecStatus
MTPrintCmd::exec(const string& option)
{
   // check option
   if (option.size())
      return CmdExec::errorOption(CMD_OPT_EXTRA, option);
   mtest.print();

   return CMD_EXEC_DONE;
}

void
MTPrintCmd::usage(ostream& os) const
{  
   os << "Usage: MTPrint" << endl;
}

void
MTPrintCmd::help() const
{  
   cout << setw(15) << left << "MTPrint: " 
        << "(memory test) print memory manager info" << endl;
}


