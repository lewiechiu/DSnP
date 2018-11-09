/****************************************************************************
  FileName     [ dbJson.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Json member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <climits>
#include <cmath>
#include <string>
#include <algorithm>
#include "dbJson.h"
#include "util.h"

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
ostream&
operator << (ostream& os, const DBJsonElem& j)
{
   os << "\"" << j._key << "\" : " << j._value;
   return os;
}

istream& operator >> (istream& is, DBJson& j)
{
   // TODO: to read in data from Json file and store them in a DB 
   // - You can assume the input file is with correct JSON file format
   // - NO NEED to handle error file format
   //assert(!j._obj.empty());
    string comd;
		string total;

		while(is >> comd)
    {
      total += comd;

      //The if condition is to prevent EOF to be read in.
      if(comd == "}")
      {
        break;
      }
        
    }
			
		total.erase(total.find("{"),1);
		
		
		int keyCnt = 0;
		while(total.find(":")!=string::npos)
		{
			total.erase(total.find(":"),1);
			if(total.find(",")!=string::npos)
				total.erase(total.find(","),1);
			keyCnt++;
		}
		string key,content;

		for(int i=0;i<keyCnt;i++)
		{
			total.erase(0,1);
			key = total.substr(0,total.find("\""));
			total.erase(0,key.length()+1); //already considered \"
			content = total.substr(0,total.find("\""));
			total.erase(0,content.length());
			DBJsonElem temp(key,stoi(content,nullptr));
			j._obj.push_back(temp);
		}

    j.initialized = true;
   return is;
}

ostream& operator << (ostream& os, const DBJson& j)
{
   for(unsigned int i=0;i<j._obj.size();i++)
   {
     cout << j._obj[i].key() << " : " << j._obj[i].value() << endl;
   }
   return os;
}

/**********************************************/
/*   Member Functions for class DBJsonElem    */
/**********************************************/
/*****************************************/
/*   Member Functions for class DBJson   */
/*****************************************/
void
DBJson::reset()
{
  //This is function works with the feature of DBRead
  //Whenever DBRead is called, this function will also be called in some way.
  this->_obj.clear();
  return ;
   // TODO
}

// return false if key is repeated
bool
DBJson::add(const DBJsonElem& elm)
{
  for(unsigned int i=0;i<this->_obj.size();i++)
  {
    if(elm.key()==_obj[i].key())
      return false;
  }
  DBJsonElem _add(elm.key(),elm.value());
  this->_obj.push_back(_add);
   return true;
}

// return NAN if DBJson is empty
float
DBJson::ave() const
{
   // TODO
   if(!this->initialized)
    return NAN;
  float ans = 0 ;
  
  for(unsigned int i=0;i<this->_obj.size();i++)
     ans += this->_obj[i].value();

  ans = ans / this->_obj.size();

   return ans;
}

// If DBJson is empty, set idx to size() and return INT_MIN
int
DBJson::max(size_t& idx) const
{
   // TODO
   int maxN = INT_MIN;
   
   if(!this->initialized)
   {
     idx = this->_obj.size();
     return INT_MIN;
   }
   else
   {
     for(unsigned int i=0;i<this->_obj.size();i++)
      if(this->_obj[i].value()>maxN)
      {
        maxN = this->_obj[i].value();
        idx = i;
      }
        
   }
   return  maxN;
}

// If DBJson is empty, set idx to size() and return INT_MIN
int
DBJson::min(size_t& idx) const
{
   // TODO
   int minN = INT_MAX;

   if(!this->initialized)
   {
     idx = this->_obj.size();
     return INT_MAX;
   }
   else
   {
     for(unsigned int i=0;i<this->_obj.size();i++)
      if(this->_obj[i].value()<minN)
      {
        minN = this->_obj[i].value();
        idx = i;
      }
    return  minN;
   }

}

void
DBJson::sort(const DBSortKey& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

void
DBJson::sort(const DBSortValue& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

// return 0 if empty
int
DBJson::sum() const
{
   // TODO
   int s = 0;
   if(!this->initialized)
    return s;
  else
  {
    for(unsigned int i=0;i<this->_obj.size();i++)
      s += this->_obj[i].value();
  }
    return s;
}
