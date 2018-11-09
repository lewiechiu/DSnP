/****************************************************************************
  FileName     [ cmdReader.cpp ]
  PackageName  [ cmd ]
  Synopsis     [ Define command line reader member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <cassert>
#include <cstring>
#include "cmdParser.h"

using namespace std;

//----------------------------------------------------------------------
//    Extrenal funcitons
//----------------------------------------------------------------------
void mybeep();
char mygetc(istream&);
ParseChar getChar(istream&);


//----------------------------------------------------------------------
//    Member Function for class Parser
//----------------------------------------------------------------------
void
CmdParser::readCmd()
{
   if (_dofile.is_open()) {
      readCmdInt(_dofile);
      _dofile.close();
   }
   else
      readCmdInt(cin);
}

void
CmdParser::readCmdInt(istream& istr)
{
   resetBufAndPrintPrompt();

   while (1) {
      ParseChar pch = getChar(istr);
      if (pch == INPUT_END_KEY) break;
      switch (pch) {
         case LINE_BEGIN_KEY :
         case HOME_KEY       : moveBufPtr(_readBuf); break;
         case LINE_END_KEY   :
         case END_KEY        : moveBufPtr(_readBufEnd); break;
         case BACK_SPACE_KEY :
		 {
			if(_readBufPtr!=_readBuf)
			{

				moveBufPtr(_readBufPtr-1);
				deleteChar();
			}
			else
				mybeep();
		}break;
         case DELETE_KEY     : deleteChar(); break;
         case NEWLINE_KEY    : addHistory();
                               cout << char(NEWLINE_KEY);
                               resetBufAndPrintPrompt(); break;
         case ARROW_UP_KEY   : moveToHistory(_historyIdx - 1); break;
         case ARROW_DOWN_KEY : moveToHistory(_historyIdx + 1); break;
         case ARROW_RIGHT_KEY: moveBufPtr(_readBufPtr+1);/* TODO */ break;
         case ARROW_LEFT_KEY : moveBufPtr(_readBufPtr-1);/* TODO */ break;
         case PG_UP_KEY      : moveToHistory(_historyIdx - PG_OFFSET); break;
         case PG_DOWN_KEY    : moveToHistory(_historyIdx + PG_OFFSET); break;
         case TAB_KEY        :
		{
			int _pos = (_readBufPtr - _readBuf) % TAB_POSITION;
			if( _pos != 0)
			 	insertChar(' ',TAB_POSITION - _pos);
			else
				insertChar(' ',TAB_POSITION);
		}break;

         case INSERT_KEY     : // not yet supported; fall through to UNDEFINE
         case UNDEFINED_KEY:   mybeep(); break;
         default:  // printable character
            insertChar(char(pch),1); break; //DONE
      }
      #ifdef TA_KB_SETTING
      taTestOnly();
      #endif
   }
}


// This function moves _readBufPtr to the "ptr" pointer
// It is used by left/right arrowkeys, home/end, etc.
//
// Suggested steps:
// 1. Make sure ptr is within [_readBuf, _readBufEnd].
//    If not, make a beep sound and return false. (DON'T MOVE)
// 2. Move the cursor to the left or right, depending on ptr
// 3. Update _readBufPtr accordingly. The content of the _readBuf[] will
//    not be changed
//
// [Note] This function can also be called by other member functions below
//        to move the _readBufPtr to proper position.
bool
CmdParser::moveBufPtr(char* const ptr)
{
   // TODO...

   if((ptr >= _readBuf) && (ptr <= _readBufEnd))
   {
     //cout << "called" << endl;

     if(_readBufPtr > ptr)
     {
		while(_readBufPtr != ptr)
		{
			_readBufPtr--;
			cout << '\b';
		}
     }
    else
    {
		while(_readBufPtr != ptr)
      	{
			cout << *_readBufPtr;
			_readBufPtr++;
		}


    }
    return true;
   }
   else
   {
     mybeep();
     return false;
   }


}


// [Notes]
// 1. Delete the char at _readBufPtr
// 2. mybeep() and return false if at _readBufEnd
// 3. Move the remaining string left for one character
// 4. The cursor should stay at the same position
// 5. Remember to update _readBufEnd accordingly.
// 6. Don't leave the tailing character.
// 7. Call "moveBufPtr(...)" if needed.
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteChar()---
//
// cmd> This is he command
//              ^
//
bool
CmdParser::deleteChar()
{
   // TODO...
   	if(_readBufPtr==_readBufEnd)
    	{mybeep();return false;}
  	else
  	{
		cout << '\r';
		int space = _readBufEnd - _readBufPtr;
		int _startP = _readBufPtr - _readBuf;

		for(int i=0,j=_startP;i<space;i++,j++)
		{
			_readBuf[j] = _readBuf[j+1];
		}

		*_readBufEnd = 0;
		_readBufEnd--;
		cout << "cmd> ";
		for(int i=0;_readBuf[i]!=0;i++)
			cout << _readBuf[i];
		cout << " " << '\0' << '\b';
		for(int i=0;i<space-1;i++)
			cout << '\b';
		cout.flush();
		return true;
  	}

}

// 1. Insert character 'ch' for "repeat" times at _readBufPtr
// 2. Move the remaining string right for "repeat" characters
// 3. The cursor should move right for "repeats" positions afterwards
// 4. Default value for "repeat" is 1. You should assert that (repeat >= 1).
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling insertChar('k', 3) ---
//
// cmd> This is kkkthe command
//                 ^
//
void
CmdParser::insertChar(char ch, int repeat)
{
   // TODO...
   	assert(repeat >= 1);
   	for(int a = 0; a < repeat ; a++)
   	{
		cout << '\r' << "cmd> ";
		char *_real_end = _readBufEnd+1;
		for(; _readBufEnd != _readBufPtr ;)
		{
			*_readBufEnd = *(_readBufEnd-1);
		_readBufEnd--;
		}

		*_readBufPtr = ch;
		_readBufEnd = _real_end;
		for(int i=0;_readBuf[i]!= 0;i++)
		{
			cout << _readBuf[i];
		}
		int move = (_readBufEnd-1) - _readBufPtr;
		for(int i=0;i<move;i++)
			cout << "\b";
		cout.flush();
		_readBufPtr++;
  	}

}

// 1. Delete the line that is currently shown on the screen
// 2. Reset _readBufPtr and _readBufEnd to _readBuf
// 3. Make sure *_readBufEnd = 0
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteLine() ---
//
// cmd>
//      ^
//
void
CmdParser::deleteLine()
{
	cout << '\r' << "cmd> ";
	for(int i=0;i<_readBufEnd - _readBuf;i++)
		cout << ' ';
	
	for(int i=0;i<_readBufEnd - _readBuf;i++)
		cout << '\b';


	cout.flush();

	for(int i=0;_readBuf[i]!=0;i++)
		_readBuf[i] = 0;

	_readBufPtr = _readBufEnd = 0;
}


// This functions moves _historyIdx to index and display _history[index]
// on the screen.
//
// Need to consider:
// If moving up... (i.e. index < _historyIdx)
// 1. If already at top (i.e. _historyIdx == 0), beep and do nothing.
// 2. If at bottom, temporarily record _readBuf to history.
//    (Do not remove spaces, and set _tempCmdStored to "true")
// 3. If index < 0, let index = 0.
//
// If moving down... (i.e. index > _historyIdx)
// 1. If already at bottom, beep and do nothing
// 2. If index >= _history.size(), let .
//
// Assign _historyIdx to index at the end.
//
// [Note] index should not = _historyIdx
//
void
CmdParser::moveToHistory(int index)
{
	// cout << endl << "BEFORE:  index: " << index << " _history: " << _historyIdx;
	// cout << " Hist.size():" << _history.size() << " booL: ";
	// if(_tempCmdStored)
	// 	cout << "True";
	// else
	// 	cout << "False";
	// cout << endl;
	// for( unsigned int i =0 ;i< _history.size();i++)
	// 	cout << "i:" << i << " " << _history[i] << endl;


	if(index < _historyIdx)
	{
		if(index<0)
			index = 0;
		if(_historyIdx == 0)
		{
			mybeep();
			return;
		}
		if(_tempCmdStored && (unsigned(_historyIdx) == _history.size()-1))
		{
			string _tempStr(_readBuf);
			_history[_historyIdx] = _tempStr;
			//cout << "updated user input: " << _history[_historyIdx] << endl;
			_tempCmdStored = true;
			deleteLine();
			cout << _history[index];

		}
		else if(unsigned(_historyIdx) == _history.size()-1)
		{
			string _tempStr(_readBuf);

			if(!_tempCmdStored)
			{
				_history[_historyIdx] = _tempStr;
				//cout << "pushed" << endl;
			}
			else
			{
				_history[_historyIdx] = _tempStr;
				//cout << "copied" << endl;
			}
				
			
			_tempCmdStored = true;

			deleteLine();
			cout << _history[index];
		}
		else
		{
			deleteLine();
			cout << _history[index];
		}

	}
	else if(index > _historyIdx)
	{
		if(int(_history.size())==0)
		{
			mybeep();
			return;
		}
		if(index >= int(_history.size()))
			index = _history.size() - 1;
			
		if(_historyIdx == int(_history.size()-1))
		{
			mybeep();
			return;
		}
		
		else
		{
			deleteLine();
			cout << _history[index];
		}
	}
	// cout << endl;
	// for (unsigned int i=0;i<_history.size();i++)
	// 	cout << "i:"<< i << "   #" << _history[i] << endl;

	string _tempStr = _history[index];
	for(unsigned int i=0;i<_tempStr.length();i++)
		_readBuf[i] = _tempStr[i];

	_readBufEnd = &_readBuf[_tempStr.length()];
	_readBufPtr = _readBufEnd;
	cout.flush();
	_historyIdx = index;

	// cout << endl << "AFTER:  index: " << index << " _history: " << _historyIdx;
	// cout << " Hist.size():" << _history.size() << " booL: ";
	// if(_tempCmdStored)
	// 	cout << "True";
	// else
	// 	cout << "False";
	// cout << endl;

   // TODO...
}


// This function adds the string in _readBuf to the _history.
// The size of _history may or may not change. Depending on whether
// there is a temp history string.
//
// 1. Remove ' ' at the beginning and end of _readBuf
// 2. If not a null string, add string to _history.
//    Be sure you are adding to the right entry of _history.
// 3. If it is a null string, don't add anything to _history.
// 4. Make sure to clean up "temp recorded string" (added earlier by up/pgUp,     <-----What is happening??
//    and reset _tempCmdStored to false
// 5. Reset _historyIdx to _history.size() // for future insertion
//
void
CmdParser::addHistory()
{
	// removing ' ' part
	string tmp(_readBuf);



	if(tmp[0]==' ')
	{	
		int _nonSP = 0;
		for(int i=0;tmp[i]== ' ';i++)
		{
			_nonSP++;
			//cout << "called" << endl;
		}
		tmp.erase(0,_nonSP);
		//cout << endl << tmp << endl;
		for(int i=0;_readBuf[i]!=0;i++)
			_readBuf[i]=0;
		for(unsigned int i=0;i<tmp.length();i++)
			_readBuf[i] = tmp[i];
		_readBufEnd = &_readBuf[tmp.length()];

	}
	
	char* ending = _readBufEnd-1;
	while(*ending == ' ')
	{
		//cout << "called" << endl;
		*ending = 0;
		ending--;
	}
	
//Finish removing ' '


	if(_readBuf[0]!=0)
	{
		_tempCmdStored = false;
		string to_copy(_readBuf);
		if(_history.size()==0)
			_history.push_back(to_copy);
		else
			_history.back() = to_copy;
		_history.push_back("");
		_historyIdx = _history.size()-1;
		deleteLine();
		cout << to_copy;
		cout.flush();
		return;
	}
	else
	{
		_history.back() = "";
		_historyIdx = _history.size()-1;
	}
   // TODO...
}


// 1. Replace current line with _history[_historyIdx] on the screen
// 2. Set _readBufPtr and _readBufEnd to end of line
//
// [Note] Do not change _history.size().
//
void
CmdParser::retrieveHistory()
{
   deleteLine();
   strcpy(_readBuf, _history[_historyIdx].c_str());
   cout << _readBuf;
   _readBufPtr = _readBufEnd = _readBuf + _history[_historyIdx].size();
}
