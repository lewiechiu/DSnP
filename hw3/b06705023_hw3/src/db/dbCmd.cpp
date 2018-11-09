/****************************************************************************
  FileName     [ dbCmd.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database commands ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <iomanip>
#include <cassert>
#include <cmath>
#include "util.h"
#include "dbCmd.h"
#include "dbJson.h"

// Global variable
DBJson dbjson;

//This part is to register event into the cmdMgr, which
// is similar to the one in initCmd()
bool
initDbCmd()
{
   // TODO...

   if (!(cmdMgr->regCmd("DBAPpend", 4, new DBAppendCmd) &&
         cmdMgr->regCmd("DBAVerage", 4, new DBAveCmd) &&
         cmdMgr->regCmd("DBCount", 3, new DBCountCmd) &&
         cmdMgr->regCmd("DBMAx", 4, new DBMaxCmd) &&
         cmdMgr->regCmd("DBMIn", 4, new DBMinCmd) &&
         cmdMgr->regCmd("DBPrint", 3, new DBPrintCmd) &&
         cmdMgr->regCmd("DBRead", 3, new DBReadCmd) &&
         cmdMgr->regCmd("DBSOrt", 4, new DBSortCmd) &&
         cmdMgr->regCmd("DBSUm", 4, new DBSumCmd) 
      )) {
      cerr << "Registering \"DB\" commands fails... exiting" << endl;
      return false;
   }

   return true;
}

//----------------------------------------------------------------------
//    DBAPpend <(string key)><(int value)>
//----------------------------------------------------------------------
CmdExecStatus
DBAppendCmd::exec(const string& option)
{
   // TODO...
        //cout << "option:" << option << "#" << endl;
   string _opt = option;
        string _key;
        int _value;
        if(!dbjson)
        {
                cerr << "Error: DB is not created yet!!" << endl;
                return CMD_EXEC_ERROR;
        }
            
        

        //check if it at least longer than null and contains two parts(seped by ' ')
        if(_opt.find(' ')!=string::npos && 
           _opt.length() >= 3 )
        {
                _key = _opt.substr(0,_opt.find(' '));
                
                //checking whether "key is valid input"
                //We have already considered user typing Key. So need not to check it exist or not.

                if(isValidVarName(_key))
                {
                        _opt.erase(0,_key.length()+1);
                        if(_opt.find(' ')!=string::npos)
                        {
                                _opt.erase(0,_opt.find(' ')+1);
                                cerr << "Error: Extra option!! (" << _opt << ")" << endl;
                                return CMD_EXEC_ERROR;
                        }
                        //The following if condition will also parse _value with _opt.
                        //Same as previous, it will fail if it is not a valid string.
                        if(myStr2Int(_opt,_value))
                        {
                                for(unsigned i = 0;i < dbjson.size();i++)
                                        if(dbjson[i].key().compare(_key) == 0)
                                        {
                                                cerr << "Error: Element with key \""<< _key << "\" already exists!!" << endl;
                                                return CMD_EXEC_ERROR;
                                        }
                                DBJsonElem ADD(_key,_value);
                                dbjson.add(ADD);
                        }
                        else
                        {
                                cerr << "Error: Illegal option!! (" << _opt << ")" << endl;
                                return CMD_EXEC_ERROR;
                        }
                            
                }
                else
                {
                    cerr << "Error: Illegal option!! (" << _key << ")" << endl;
                    return CMD_EXEC_ERROR;    
                }
        }
   else
   {
        cerr << "Error: Missing option!!" << endl;
        return CMD_EXEC_ERROR;
   }
   
   
   // check option

   return CMD_EXEC_DONE;
}

void
DBAppendCmd::usage(ostream& os) const
{
   os << "Usage: DBAPpend <(string key)><(int value)>" << endl;
}

void
DBAppendCmd::help() const
{
   cout << setw(15) << left << "DBAPpend: "
        << "append an JSON element (key-value pair) to the end of DB" << endl;
}


//----------------------------------------------------------------------
//    DBAVerage
//----------------------------------------------------------------------
CmdExecStatus
DBAveCmd::exec(const string& option)
{  
   // check option
   if (!CmdExec::lexNoOption(option))
      return CMD_EXEC_ERROR;

   float a = dbjson.ave();
   if (isnan(dbjson.ave())) {
      cerr << "Error: The average of the DB is nan." << endl;
      return CMD_EXEC_ERROR;
   }
   ios_base::fmtflags origFlags = cout.flags();
   cout << "The average of the DB is " << fixed
        << setprecision(2) << a << ".\n";
   cout.flags(origFlags);

   return CMD_EXEC_DONE;
}

void
DBAveCmd::usage(ostream& os) const
{     
   os << "Usage: DBAVerage" << endl;
}

void
DBAveCmd::help() const
{
   cout << setw(15) << left << "DBAVerage: "
        << "compute the average of the DB" << endl;
}


//----------------------------------------------------------------------
//    DBCount
//----------------------------------------------------------------------
CmdExecStatus
DBCountCmd::exec(const string& option)
{  
   // check option
   if (!CmdExec::lexNoOption(option))
      return CMD_EXEC_ERROR;

   size_t n = dbjson.size();
   if (n > 1)
      cout << "There are " << n << " JSON elements in DB." << endl;
   else if (n == 1)
      cout << "There is 1 JSON element in DB." << endl;
   else
      cout << "There is no JSON element in DB." << endl;

   return CMD_EXEC_DONE;
}

void
DBCountCmd::usage(ostream& os) const
{     
   os << "Usage: DBCount" << endl;
}

void
DBCountCmd::help() const
{
   cout << setw(15) << left << "DBCount: "
        << "report the number of JSON elements in the DB" << endl;
}


//----------------------------------------------------------------------
//    DBMAx
//----------------------------------------------------------------------
CmdExecStatus
DBMaxCmd::exec(const string& option)
{  
   // check option
   if (!CmdExec::lexNoOption(option))
      return CMD_EXEC_ERROR;

   size_t maxI;
   int maxN = dbjson.max(maxI);
   if (maxN == INT_MIN) {
      cerr << "Error: The max JSON element is nan." << endl;
      return CMD_EXEC_ERROR;
   }
   cout << "The max JSON element is { " << dbjson[maxI] << " }." << endl;

   return CMD_EXEC_DONE;
}

void
DBMaxCmd::usage(ostream& os) const
{     
   os << "Usage: DBMAx" << endl;
}

void
DBMaxCmd::help() const
{
   cout << setw(15) << left << "DBMAx: "
        << "report the maximum JSON element" << endl;
}


//----------------------------------------------------------------------
//    DBMIn
//----------------------------------------------------------------------
CmdExecStatus
DBMinCmd::exec(const string& option)
{  
   // check option
   if (!CmdExec::lexNoOption(option))
      return CMD_EXEC_ERROR;

   size_t minI;
   int minN = dbjson.min(minI);
   if (minN == INT_MAX) {
      cerr << "Error: The min JSON element is nan." << endl;
      return CMD_EXEC_ERROR;
   }
   cout << "The min JSON element is { " << dbjson[minI] << " }." << endl;

   return CMD_EXEC_DONE;
}

void
DBMinCmd::usage(ostream& os) const
{     
   os << "Usage: DBMIn" << endl;
}

void
DBMinCmd::help() const
{
   cout << setw(15) << left << "DBMIn: "
        << "report the minimum JSON element" << endl;
}


//----------------------------------------------------------------------
//    DBPrint [(string key)]
//----------------------------------------------------------------------
CmdExecStatus
DBPrintCmd::exec(const string& option)
{  
   // TODO...
   //Check if option is valid input.
    if(!dbjson)
    {
        cerr << "Error: DB is not created yet!!" << endl;
        return CMD_EXEC_ERROR;
    }
        


   if(option.length()==0)
   {
        //This means that we print out all of the DBjson
        cout << "{" << endl;

        for(unsigned i =0 ;i < dbjson.size();i++)
        {
            if(i!=dbjson.size()-1)
                cout << "  \"" << dbjson[i].key() << "\" : " << dbjson[i].value() << "," << endl;
            else
                cout << "  \"" << dbjson[i].key() << "\" : " << dbjson[i].value() << endl;
        }
        cout << "}" << endl;
        cout << "Total JSON elements: " << dbjson.size() << endl;
   }
   else if(option.find(' ')==string::npos)
   {
        //We have to further check whether the specified "key" exist or not
        if(!isValidVarName(option))
            return CmdExec::errorOption(CMD_OPT_ILLEGAL,option);
            
        bool FoundFlag = false;
        for(unsigned i = 0 ;i < dbjson.size();i++)
        {
            if(dbjson[i].key().compare(option)==0)
            {
                cout << "{ \"" << option << "\" : " << dbjson[i].value() << " }" << endl;
                FoundFlag = true;
                break;
            }
            else
                continue;
        }

        if(!FoundFlag)
        {
            cerr << "Error: No JSON element with key \"" << option << "\" is found." << endl;
            return CMD_EXEC_ERROR;
        }
   }
   else
   {
        string ERRopt = option.substr(option.find(' '));
        return CmdExec::errorOption(CMD_OPT_EXTRA,ERRopt);
   }

   return CMD_EXEC_DONE;
}

void
DBPrintCmd::usage(ostream& os) const
{
   os << "DBPrint [(string key)]" << endl;
}

void
DBPrintCmd::help() const
{
   cout << setw(15) << left << "DBPrint: "
        << "print the JSON element(s) in the DB" << endl;
}


//----------------------------------------------------------------------
//    DBRead <(string jsonFile)> [-Replace]
//----------------------------------------------------------------------
CmdExecStatus
DBReadCmd::exec(const string& option)
{
   // check option
   vector<string> options;
   if (!CmdExec::lexOptions(option, options))
      return CMD_EXEC_ERROR;

   if (options.empty())
      return CmdExec::errorOption(CMD_OPT_MISSING, "");

   bool doReplace = false;
   string fileName;
   for (size_t i = 0, n = options.size(); i < n; ++i) {
      if (myStrNCmp("-Replace", options[i], 2) == 0) {
         if (doReplace) return CmdExec::errorOption(CMD_OPT_EXTRA,options[i]);
         doReplace = true;
      }
      else {
         if (fileName.size())
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[i]);
         fileName = options[i];
      }
   }

   ifstream ifs(fileName.c_str());
   if (!ifs) {
      cerr << "Error: \"" << fileName << "\" does not exist!!" << endl;
      return CMD_EXEC_ERROR;
   }
   //After implementing object bool operator, this part will no longer be wrong.

   if (dbjson) {
      if (!doReplace) {
         cerr << "Error: DB exists. Use \"-Replace\" option for "
              << "replacement.\n";
         return CMD_EXEC_ERROR;
      }
      cout << "DB is replaced..." << endl;
      dbjson.reset();
   }

  if (!(ifs >> dbjson)) {cerr << "fail reading JSON file" << endl; return CMD_EXEC_ERROR;}

   cout << "\"" << fileName << "\" was read in successfully." << endl;

   return CMD_EXEC_DONE;
}

void
DBReadCmd::usage(ostream& os) const
{
   os << "Usage: DBRead <(string jsonFile)> [-Replace]" << endl;
}

void
DBReadCmd::help() const
{
   cout << setw(15) << left << "DBRead: "
        << "read data from .json file" << endl;
}


//----------------------------------------------------------------------
//    DBSOrt <-Key | -Value>
//----------------------------------------------------------------------
CmdExecStatus
DBSortCmd::exec(const string& option)
{
   // check option
   string token;
   if (!CmdExec::lexSingleOption(option, token, false))
      return CMD_EXEC_ERROR;

   if (myStrNCmp("-Key", token, 2) == 0) dbjson.sort(DBSortKey());
   else if (myStrNCmp("-Value", token, 2) == 0) dbjson.sort(DBSortValue());
   else return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);

   return CMD_EXEC_DONE;
}

void
DBSortCmd::usage(ostream& os) const
{
   os << "Usage: DBSOrt <-Key | -Value>" << endl;
}

void
DBSortCmd::help() const
{
   cout << setw(15) << left << "DBSOrt: "
        << "sort the JSON object by key or value" << endl;
}


//----------------------------------------------------------------------
//    DBSUm
//----------------------------------------------------------------------
CmdExecStatus
DBSumCmd::exec(const string& option)
{  
   // check option
   if (!CmdExec::lexNoOption(option))
      return CMD_EXEC_ERROR;

   if (dbjson.empty()) {
      cerr << "Error: The sum of the DB is nan." << endl;
      return CMD_EXEC_ERROR;
   }
   cout << "The sum of the DB is " << dbjson.sum() << "." << endl;

   return CMD_EXEC_DONE;
}

void
DBSumCmd::usage(ostream& os) const
{     
   os << "Usage: DBSUm" << endl;
}

void
DBSumCmd::help() const
{
   cout << setw(15) << left << "DBSUm: "
        << "compute the summation of the DB" << endl;
}

