/****************************************************************************
  FileName     [ cmdParser.cpp ]
  PackageName  [ cmd ]
  Synopsis     [ Define command parsing member functions for class CmdParser ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <cassert>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "util.h"
#include "cmdParser.h"

using namespace std;

//----------------------------------------------------------------------
//    External funcitons
//----------------------------------------------------------------------
void mybeep();

//----------------------------------------------------------------------
//    Member Function for class cmdParser
//----------------------------------------------------------------------

// Please refer to the comments in "DofileCmd::exec", cmdCommon.cpp
bool
CmdParser::openDofile(const string& dof)
{
  // TODO...


  //This part is to check the current depth
  static unsigned MAX_Depth = 1024;
  // static bool STACKOVER = false;
  
  if(_dofileStack.size()==MAX_Depth)
  {
    _dofile->clear(istream::eofbit);
    return false;
  }
  else
  {
    // STACKOVER = false;
    if(_dofile!=0)
    {
      _dofileStack.push(_dofile);
    }
      
    _dofile = new ifstream(dof.c_str(),ios::in);
    // return false if file cannot be opened
    if(_dofile->fail())
    {

      delete _dofile;
     _dofile = nullptr;
      if(_dofileStack.size())
      {
        _dofile = _dofileStack.top();
        _dofileStack.pop();
      }
        
      return false;
    }
    return true;
  }
  
}

// Must make sure _dofile != 0
void
CmdParser::closeDofile()
{
  assert(_dofile != 0);
   // TODO...
  //Whenever closefile is invoked
  //We 
  _dofile->close();
  delete _dofile;
  _dofile = nullptr;


  // cerr << "after deleting dofile" << endl;
  if(_dofileStack.size())
  {
    _dofile = _dofileStack.top();
    _dofileStack.pop();
  }

}

// Return false if registration fails
bool
CmdParser::regCmd(const string& cmd, unsigned nCmp, CmdExec* e)
{
   // Make sure cmd hasn't been registered and won't cause ambiguity
   //cout << cmd << endl; 
   string str = cmd;
   unsigned s = str.size();
   if (s < nCmp) return false;
   while (true) {
      if (getCmd(str)) return false;
      if (s == nCmp) break;
      str.resize(--s);
   }

   // Change the first nCmp characters to upper case to facilitate
   //    case-insensitive comparison later.
   // The strings stored in _cmdMap are all upper case
   //
   assert(str.size() == nCmp);  // str is now mandCmd
   string& mandCmd = str;
   for (unsigned i = 0; i < nCmp; ++i)
      mandCmd[i] = toupper(mandCmd[i]);
   string optCmd = cmd.substr(nCmp);
   assert(e != 0);
   e->setOptCmd(optCmd);

   // insert (mandCmd, e) to _cmdMap; return false if insertion fails.
   return (_cmdMap.insert(CmdRegPair(mandCmd, e))).second;
}

// Return false on "quit" or if excetion happens
CmdExecStatus
CmdParser::execOneCmd()
{
   bool newCmd = false;
   if (_dofile != 0)
      newCmd = readCmd(*_dofile);
   else
      newCmd = readCmd(cin);

    
  //cout << "stack size" << this->_dofileStack.size() << endl;
  //cout << _dofile << endl;

   // execute the command
   if (newCmd) {
      string option;
      CmdExec* e = parseCmd(option);
      
      if (e != 0)
         return e->exec(option);
   }
   return CMD_EXEC_NOP;
}

// For each CmdExec* in _cmdMap, call its "help()" to print out the help msg.
// Print an endl at the end.
void
CmdParser::printHelps() const
{
   for(auto const& r : this->_cmdMap)
   {
     r.second->help();
   }
   cout << endl;

   // TODO...
}

void
CmdParser::printHistory(int nPrint) const
{
   assert(_tempCmdStored == false);
   if (_history.empty()) {
      cout << "Empty command history!!" << endl;
      return;
   }
   int s = _history.size();
   if ((nPrint < 0) || (nPrint > s))
      nPrint = s;
   for (int i = s - nPrint; i < s; ++i)
      cout << "   " << i << ": " << _history[i] << endl;
}



//
// 1. Read the command string (may contain multiple words) from the leading
//    part of str (i.e. the first word) and retrive the corresponding
//    CmdExec* from _cmdMap
//    ==> If command not found, print to cerr the following message:
//        Illegal command!! "(string cmdName)"
//    ==> return it at the end.
// 2. Call getCmd(cmd) to retrieve command from _cmdMap.
//    "cmd" is the first word of "str".
// 3. Get the command options from the trailing part of str (i.e. second
//    words and beyond) and store them in "option"
//
CmdExec*
CmdParser::parseCmd(string& option)
{
   assert(_tempCmdStored == false);
   assert(!_history.empty());
// Parse the command from _history.back();
// Let string str = _history.back();
   string str = _history.back();

   // TODO...
   assert(str[0] != 0 && str[0] != ' ');
    if(str.find(' ')!=string::npos)
      option = str.substr(str.find(' ')+1);
    else
      option = "";
  //  cout << "str is:"<<str ;
  // cout << " option is:" << option << endl;
   string cmd;
   if(option.length()!=0)
    cmd = str.substr(0, str.length()-option.length()-1);
   else
    cmd = str;
  //  cout << "cmd:" << cmd << "#"<< endl;
   CmdExec* TORUN = getCmd(cmd);
   if(TORUN==0)
   {
   cerr << "Illegal command!! ("<< cmd <<")" << endl;
   return NULL;
   }
   else
   {
     return TORUN;
   }
    
}

// Remove this function for TODO...
//
// This function is called by pressing 'Tab'.
// It is to list the partially matched commands.
// "str" is the partial string before current cursor position. It can be 
// a null string, or begin with ' '. The beginning ' ' will be ignored.
//
// Several possibilities after pressing 'Tab'
// (Let $ be the cursor position)
// 1. LIST ALL COMMANDS
//    --- 1.1 ---
//    [Before] Null cmd
//    cmd> $
//    --- 1.2 ---
//    [Before] Cmd with ' ' only
//    cmd>     $
//    [After Tab]
//    ==> List all the commands, each command is printed out by:
//           cout << setw(12) << left << cmd;
//    ==> Print a new line for every 5 commands
//    ==> After printing, re-print the prompt and place the cursor back to
//        original location (including ' ')
// 1. LIST ALL COMMANDS



// 2. LIST ALL PARTIALLY MATCHED COMMANDS
//    --- 2.1 ---
//    [Before] partially matched (multiple matches)
//    cmd> h$                   // partially matched
//    [After Tab]
//    HELp        HIStory       // List all the parially matched commands
//    cmd> h$                   // and then re-print the partial command
//    --- 2.2 ---
//    [Before] partially matched (multiple matches)
//    cmd> h$llo                // partially matched with trailing characters
//    [After Tab]
//    HELp        HIStory       // List all the parially matched commands
//    cmd> h$llo                // and then re-print the partial command




// 3. LIST THE SINGLY MATCHED COMMAND
//    ==> In either of the following cases, print out cmd + ' '
//    ==> and reset _tabPressCount to 0
//    --- 3.1 ---
//    [Before] partially matched (single match)
//    cmd> he$
//    [After Tab]
//    cmd> heLp $               // auto completed with a space inserted
//    --- 3.2 ---
//    [Before] partially matched with trailing characters (single match)
//    cmd> he$ahah
//    [After Tab]
//    cmd> heLp $ahaha
//    ==> Automatically complete on the same line
//    ==> The auto-expanded part follow the strings stored in cmd map and
//        cmd->_optCmd. Insert a space after "heLp"
//    --- 3.3 ---
//    [Before] fully matched (cursor right behind cmd)
//    cmd> hElP$sdf
//    [After Tab]
//    cmd> hElP $sdf            // a space character is inserted



// 4. NO MATCH IN FITST WORD
//    --- 4.1 ---
//    [Before] No match
//    cmd> hek$
//    [After Tab]
//    ==> Beep and stay in the same location

// 5. FIRST WORD ALREADY MATCHED ON FIRST TAB PRESSING
//    --- 5.1 ---
//    [Before] Already matched on first tab pressing
//    cmd> help asd$gh
//    [After] Print out the usage for the already matched command
//    Usage: HELp [(string cmd)]
//    cmd> help asd$gh

// 6. FIRST WORD ALREADY MATCHED ON SECOND AND LATER TAB PRESSING
//    ==> Note: command usage has been printed under first tab press
//    ==> Check the word the cursor is at; get the prefix before the cursor
//    ==> So, this is to list the file names under current directory that
//        match the prefix
//    ==> List all the matched file names alphabetically by:
//           cout << setw(16) << left << fileName;
//    ==> Print a new line for every 5 commands
//    ==> After printing, re-print the prompt and place the cursor back to
//        original location
//    --- 6.1 ---
//    Considering the following cases in which prefix is empty:
//    --- 6.1.1 ---
//    [Before] if prefix is empty, and in this directory there are multiple
//             files and they do not have a common prefix,
//    cmd> help $sdfgh
//    [After] print all the file names
//    .               ..              Homework_3.docx Homework_3.pdf  Makefile
//    MustExist.txt   MustRemove.txt  bin             dofiles         include
//    lib             mydb            ref             src             testdb
//    cmd> help $sdfgh
//    --- 6.1.2 ---
//    [Before] if prefix is empty, and in this directory there are multiple
//             files and all of them have a common prefix,
//    cmd> help $orld
//    [After]
//    ==> auto insert the common prefix and make a beep sound
//    ==> DO NOT print the matched files
//    cmd> help mydb-$orld
//    --- 6.1.3 ---
//    [Before] if prefix is empty, and only one file in the current directory
//    cmd> help $ydb
//    [After] print out the single file name followed by a ' '
//    cmd> help mydb $
//    --- 6.2 ---
//    [Before] with a prefix and with mutiple matched files
//    cmd> help M$Donald
//    [After]
//    Makefile        MustExist.txt   MustRemove.txt
//    cmd> help M$Donald
//    --- 6.3 ---
//    [Before] with a prefix and with mutiple matched files,
//             and these matched files have a common prefix
//    cmd> help Mu$k
//    [After]
//    ==> auto insert the common prefix and make a beep sound
//    ==> DO NOT print the matched files
//    cmd> help Must$k
//    --- 6.4 ---
//    [Before] with a prefix and with a singly matched file
//    cmd> help MustE$aa
//    [After] insert the remaining of the matched file name followed by a ' '
//    cmd> help MustExist.txt $aa
//    --- 6.5 ---
//    [Before] with a prefix and NO matched file
//    cmd> help Ye$kk
//    [After] beep and stay in the same location
//    cmd> help Ye$kk
//
//    [Note] The counting of tab press is reset after "newline" is entered.


// 6. FIRST WORD ALREADY MATCHED ON SECOND AND LATER TAB PRESSING
// 7. FIRST WORD NO MATCH
//    --- 7.1 ---
//    [Before] Cursor NOT on the first word and NOT matched command
//    cmd> he haha$kk
//    [After Tab]
//    ==> Beep and stay in the same location


void
CmdParser::listCmd(const string& str)
{

  //First, we remove ' ' in the leading part.

  string _str(str);
  while(_str[0]==' ')
    _str.erase(0,1);
  

// [NOTE] Make sure whenever we print 5 elements,
//        a Newline is implemented.

// 1. LIST ALL COMMANDS
  if(_str.size()==0)
  {

    cout << endl;
    int NL = 0; //used as counting newline
      for(auto const& r : this->_cmdMap)
      {
        if(NL == 5)
        {
          cout << endl;
          NL = 0;
        }
        string _toPrint(r.first+r.second->getOptCmd());
        cout << setw(12) << left << _toPrint;
        NL++;
      }
      NL = 0;
      reprintCmd();
  }
  else
  {

//To separate 5,6,7 and 2,3,4 we have to check if there exist ' ' in it.
    //This is is for 2,3,4
    if(_str.find(' ')==string::npos)
    {
        //This if is for 1,2,3,4


        //To separate 2 and 3, we have to first detect 
        //whether there are more than 1 matched or not
        int match = 0;

        for(auto const& r : this->_cmdMap)
        {
          string _toPrint(r.first+r.second->getOptCmd());


          //We use myStrNCmp here is to check the first r.first.length() letters
          //Those letters must exist to decide how many matched letters.
          //The rest of the letters are not necessary
          //However, we do need to compare it. to see if there are any diffs
          //In the non-mandatory part of the string

          // cout << endl << _str << "#" << endl << _toPrint << "#" << endl;
          int Length = 0;
          if(r.first.length() < _str.length())
            Length = r.first.length();
          else
            Length = _str.length();
          if(myStrNCmp(_toPrint,_str,Length)==0)
            match ++ ;


        }

        //match can be viewed as will always be greater than 1
        if(match==1)
        {
          //DO 3
          // 3. LIST THE SINGLY MATCHED COMMAND

          string _toPrint;
          //We first make the rest part of the string (r.first)
          //Then we change _readBuf
          for(auto const& r : this->_cmdMap)
          {
            _toPrint = r.first + r.second->getOptCmd() ;
            
            int Length = 0;
            if(r.first.length() < _str.length())
              Length = r.first.length();
            else
              Length = _str.length();
            if(myStrNCmp(_toPrint,_str,Length)==0)
            {

              _toPrint.erase(0,_str.length());
              _toPrint += ' ';
              for(unsigned i = 0;i<_toPrint.length();i++)
                insertChar(_toPrint[i]);
              
              string _toCP(_readBuf);
              
              //_toPrint is the string containing whole word

              char* pointing = this->_readBufPtr;
              
              deleteLine();

              for(unsigned i =0;i<_toCP.length();i++)
                insertChar(_toCP[i]);
              
              moveBufPtr(pointing);

            }
            
          }
          //
        }
        else if(match>1)
        {
          //DO 2
          // 2. LIST ALL PARTIALLY MATCHED COMMANDS
          int NL = 5; //used as counting newline
          for(auto const& r : this->_cmdMap)
          {
            string _toPrint(r.first+r.second->getOptCmd());
            int Length = 0;
            if(r.first.length() < _str.length())
              Length = r.first.length();
            else
              Length = _str.length();
            if(myStrNCmp(_toPrint,_str,Length)==0)
            {
              if(NL == 5)
              {
                cout << endl;
                NL = 0;
              }
              cout << setw(12) << left << _toPrint;
              NL++;
            }
            
          }
          NL = 0;
          reprintCmd();
        }
        else
        {
          // 4. NO MATCH IN FIRST WORD
          //BEEEEEPPPPP
          mybeep();
        }
    }
    //This if is for 5,6,7
    else
    {
      //cout << "       tab count:" << this->_tabPressCount << endl;
      static unsigned previous = this->_history.size();
      static unsigned starting = this->_tabPressCount;
      if(previous!=this->_history.size())
      {
        previous = this->_history.size();
        starting = this->_tabPressCount;
      }
      
      string First_str = _str.substr(0,_str.find(' '));
      //First tab pressed
    //Do 5
      //whenever
      bool _cmdMatched = false;
      for(auto const& r : this->_cmdMap)
      {
        string _toPrint(r.first+r.second->getOptCmd());
        if(myStrNCmp(_toPrint,First_str,r.first.length())==0)
        {
          _cmdMatched = true;
          break;
        }
      }
      if(this->_tabPressCount == starting && _cmdMatched)
      {
        for(auto const& r : this->_cmdMap)
        {
          string _toPrint(r.first+r.second->getOptCmd());
          if(myStrNCmp(_toPrint,First_str,r.first.length())==0)
          {
            cout << endl;
            r.second->usage(cout);
            reprintCmd();
            this->_tabPressCount++;
            _cmdMatched = true;
            break;
          }
        }
      }
    //end Do5
      //Second tab pressed
    //DO Separate 6,7
      else if(_cmdMatched)
      {
        string Prefix = _str.substr(_str.find(' ')+1);
        //Consider two cicumstance, Prefix is either empty -> DO 6.1
        //Prefix is not empty


        //Prefix is empty
      //DO 6.1
        if(Prefix.length()==0)
        {
          vector<string> _FileName;

          if(listDir(_FileName,Prefix,".")==0)
          {
            //There are three cicumstances here
            //1. Multiple files No common Prefix
            //2. Multiple files Common Prefix
            //3. Only single file in it
            if(_FileName.size()>1)
            {
              int NL = 0;
              //If there exists a common prefix in the filename
              //Starting from any of the file and check through whether all the others
              //have the same file prefix.
              string CommonPrefix = _FileName[0].substr(0,1);
              bool ExistCommon = true;
            
              for(unsigned i =1;i<_FileName.size();i++)
              {
                if(CommonPrefix != _FileName[i].substr(0,1))
                {
                  ExistCommon = false;
                  break;
                } 
              }

              //If true, then threre is a common one.
              //Loop deeper into the string to find the Longest common subsequence
              if(ExistCommon)
              {
                //Outter loop is for string length
                //Inner loop is for file name comparison
                
                for(unsigned lengthCommon = 2;lengthCommon <= _FileName[0].length();lengthCommon++)
                {
                  CommonPrefix = _FileName[0].substr(0,lengthCommon);
                  for(unsigned i = 1;i<_FileName.size();i++)
                  {
                    // cout << i << ",Common Prefix:#" << CommonPrefix << "#" << _FileName[i] << "#" << endl;
                    if(CommonPrefix.compare(_FileName[i].substr(0,lengthCommon))!=0)
                    {
                      ExistCommon = false;
                      break;
                    }
                  }
                  if(!ExistCommon)
                  {
                    CommonPrefix.pop_back();
                    break;
                  }
                }

                //After extending the longest Common string length
                //We have to insert it into _readBuf

                //               This part is just inserting and reprinting
                for(unsigned i =0;i<CommonPrefix.length();i++)
                  insertChar(CommonPrefix[i]);
                
                string _toPrint(_readBuf);
                char* pointing = _readBufPtr;
                deleteLine();

                for(unsigned i =0;i<_toPrint.length();i++)
                  insertChar(_toPrint[i]);
                
                moveBufPtr(pointing);
                mybeep();
              }

              //Common sequence Not Found
              else
              {
                //This part is for no common prefix
                //Print out all the files in the current wd()
                for(unsigned i = 0; i <_FileName.size();i++)
                {
                  if(NL == 5)
                  {
                    NL = 0;
                    cout << endl;
                  }
                    
                  cout  << setw(16) << left << _FileName[i];
                  NL ++;
                }
                _FileName.clear();
                NL = 0;
                cout << endl;
                reprintCmd();
              }

            }
            
            //Single file in the directory
            else if(_FileName.size()==1)
            {
              string FN = _FileName[0];
              FN += ' ';
              for(unsigned l = 0;l<FN.length();l++)
                insertChar(FN[l],1);
            
              string Orig(_readBuf);
              char *pointing = _readBufPtr;
              deleteLine();

              for(unsigned i =0;i<Orig.length();i++)
                insertChar(Orig[i]);
              moveBufPtr(pointing);
            }
            
          }
          //Fail Safe device
          else
            _FileName.clear();
        }
      //end DO 6.1
      //DO 6.2
        //Prefix is not empty
        else
        {
          vector<string> _FileName;

          //first we fetch the filename from the current dir
          if(listDir(_FileName,Prefix,".")==0)
          {}


          //We check among the filename vector, to see whether it all fits
          //Set the bool CommonPrefix 
          int matchPrefix = 0;
          for(unsigned i =0;i<_FileName.size();i++)
          {
            if(myStrNCmp(_FileName[i],Prefix,Prefix.length())==0)
            {
              matchPrefix ++;
            }
          }
          // cout << endl << matchPrefix << endl;
          //1 file matched
          if(matchPrefix==1)
          {
            for(unsigned i =0;i<_FileName.size();i++)
            {
              if(myStrNCmp(_FileName[i],Prefix,Prefix.length())==0)
              {
                string toInsert = _FileName[i];
                toInsert.erase(0,Prefix.length());
                toInsert += ' ';
                for(unsigned j = 0;j< toInsert.length();j++)
                {
                  insertChar(toInsert[j]);
                }
                char* pointing = _readBufPtr;

                string newstring(_readBuf);

                deleteLine();
                for(unsigned j = 0;j<newstring.length();j++)
                  insertChar(newstring[j]);
                
                moveBufPtr(pointing);
                //insert the whole file name into readbuf
              }
            }
          }
          else if(matchPrefix>=2)
          {
            int NL = 0;
            //Check if there exist longer CS
            //else
            //print out the file that matches

            bool ExistCommon = true;
            string CommonPrefix;
            for(unsigned lengthCommon = 2;lengthCommon <= _FileName[0].length();lengthCommon++)
            {
              CommonPrefix = _FileName[0].substr(0,lengthCommon);
              for(unsigned i = 1;i<_FileName.size();i++)
              {
                // cout << i << ",Common Prefix:#" << CommonPrefix << "#" << _FileName[i] << "#" << endl;
                if(CommonPrefix.compare(_FileName[i].substr(0,lengthCommon))!=0)
                {
                  ExistCommon = false;
                  break;
                }
              }
              if(!ExistCommon)
              {
                CommonPrefix.pop_back();
                break;
              }
            }
            // cout << Prefix << "common prefix:" << Prefix << endl;
            if(CommonPrefix.length()==1)
            {
              cout << endl;
              for(unsigned i =0;i<_FileName.size();i++)
              {
                if(myStrNCmp(_FileName[i],CommonPrefix,CommonPrefix.length())==0)
                {
                  if(NL==5)
                  {
                    cout << endl;
                  }
                  cout << setw(16) << left << _FileName[i];
                  NL++;
                  //print out those file that matches
                }
              }
              cout << endl;
              reprintCmd();
            }
            else
            {
              CommonPrefix.erase(0,Prefix.length());
              for(unsigned j = 0;j< CommonPrefix.length();j++)
              {
                insertChar(CommonPrefix[j]);
              }
              char* pointing = _readBufPtr;

              string newstring(_readBuf);

              deleteLine();
              for(unsigned j = 0;j<newstring.length();j++)
                insertChar(newstring[j]);
              
              moveBufPtr(pointing);
              mybeep();
            }
          }
          else
          {
            mybeep();
          }
            

          _FileName.clear();

        }
        //
      }
      else
      {
        mybeep();
      }
      

    }
  }


// 5. FIRST WORD ALREADY MATCHED ON FIRST TAB PRESSING
// 6. FIRST WORD ALREADY MATCHED ON SECOND AND LATER TAB PRESSING
// 7. FIRST WORD NO MATCH




   // TODO...
}



// 1. strlen(s1) must >= n
// 2. The first n characters of s2 are mandatory, they must be case-
//    insensitively compared to s1. Return less or greater than 0 if unequal.
// 3. The rest of s2 are optional. Return 0 if EOF of s2 is encountered.
//    Otherwise, perform case-insensitive comparison until non-equal result
//    presents.

// Parse the string "str"

// cmd is a copy of the original input
//
// return the corresponding CmdExec* if "cmd" matches any command in _cmdMap
// return 0 if not found.
//
// Please note:
// ------------
// 1. The mandatory part of the command string (stored in _cmdMap) must match
// 2. The optional part can be partially omitted.
// 3. All string comparison are "case-insensitive".
//
CmdExec*
CmdParser::getCmd(string cmd)
{
  if(_history.size()!=0)
  {
      for (auto const& x : this->_cmdMap)
      {
          string _inside = x.first + x.second->getOptCmd(); 
          if(cmd.length() >= x.first.length() && cmd.length() <= _inside.length())
          {
            // cout << "cmd:" << cmd << "<--->inside:" << _inside << myStrNCmp( cmd,_inside.substr(0,cmd.length()) , cmd.length()) << endl;
            if(myStrNCmp(_inside.substr(0,cmd.length()) , cmd , cmd.length())==0)
              {
                return x.second;
              }
          }
            
      }
  }
   if(this->_cmdMap.find(cmd)==this->_cmdMap.end())
      return 0;
   else
   {
    //cout << "#" << this->_cmdMap.find(cmd)->first << "#" << endl;
     return this->_cmdMap.find(cmd)->second;
   }
   // TODO...
}


//----------------------------------------------------------------------
//    Member Function for class CmdExec
//----------------------------------------------------------------------
// return false if option contains an token
bool
CmdExec::lexNoOption(const string& option) const
{
   string err;
   myStrGetTok(option, err);
   if (err.size()) {
      errorOption(CMD_OPT_EXTRA, err);
      return false;
   }
   return true;
}

// Return false if error options found
// "optional" = true if the option is optional XD
// "optional": default = true
//
bool
CmdExec::lexSingleOption
(const string& option, string& token, bool optional) const
{
   size_t n = myStrGetTok(option, token);
   if (!optional) {
      if (token.size() == 0) {
         errorOption(CMD_OPT_MISSING, "");
         return false;
      }
   }
   if (n != string::npos) {
      errorOption(CMD_OPT_EXTRA, option.substr(n));
      return false;
   }
   return true;
}

// if nOpts is specified (!= 0), the number of tokens must be exactly = nOpts
// Otherwise, return false.
//
bool
CmdExec::lexOptions
(const string& option, vector<string>& tokens, size_t nOpts) const
{
   string token;
   size_t n = myStrGetTok(option, token);
   while (token.size()) {
      tokens.push_back(token);
      n = myStrGetTok(option, token, n);
   }
   if (nOpts != 0) {
      if (tokens.size() < nOpts) {
         errorOption(CMD_OPT_MISSING, "");
         return false;
      }
      if (tokens.size() > nOpts) {
         errorOption(CMD_OPT_EXTRA, tokens[nOpts]);
         return false;
      }
   }
   return true;
}

CmdExecStatus
CmdExec::errorOption(CmdOptionError err, const string& opt) const
{
   switch (err) {
      case CMD_OPT_MISSING:
         cerr << "Error: Missing option";
         if (opt.size()) cerr << " after (" << opt << ")";
         cerr << "!!" << endl;
      break;
      case CMD_OPT_EXTRA:
         cerr << "Error: Extra option!! (" << opt << ")" << endl;
      break;
      case CMD_OPT_ILLEGAL:
         cerr << "Error: Illegal option!! (" << opt << ")" << endl;
      break;
      case CMD_OPT_FOPEN_FAIL:
         cerr << "Error: cannot open file \"" << opt << "\"!!" << endl;
      break;
      default:
         cerr << "Error: Unknown option error type!! (" << err << ")" << endl;
      exit(-1);
   }
   return CMD_EXEC_ERROR;
}

