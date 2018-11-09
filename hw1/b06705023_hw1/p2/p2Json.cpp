/****************************************************************************
  FileName     [ p2Json.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define member functions of class Json and JsonElem ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2018-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "p2Json.h"

using namespace std;

string JsonElem::getKey() const
{
	return this->_key;
}

int JsonElem::getVal() const
{
	return this->_value;
}


// Implement member functions of class Row and Table here
bool
Json::read(const string& jsonFile)
{
	fstream inputJs;
	inputJs.open(jsonFile);
	if(inputJs.is_open())
	{
		//cout << "open successfully" << endl;
		string comd;
		string total;

		while(inputJs >> comd)
			total += comd;

		total.erase(total.find("{"),1);
		total.erase(total.find("}"),1);
		
		int keyCnt = 0;
		while(total.find(":")!=string::npos)
		{
			total.erase(total.find(":"),1);
			if(total.find(",")!=string::npos)
				total.erase(total.find(","),1);
			keyCnt++;
		}
		//cout << "complete json command:" << endl << total << endl;
		string key,content;
		for(int i=0;i<keyCnt;i++)
		{
			total.erase(0,1);
			key = total.substr(0,total.find("\""));
			//cout << "key: " << key << endl;
			total.erase(0,key.length()+1); //already considered \"
			content = total.substr(0,total.find("\""));
			//cout << "content: " << content << endl;
			total.erase(0,content.length());
			JsonElem temp(key,stoi(content,nullptr));
			this->_obj.push_back(temp);
		}
		return true;
	}
   	else
   		return false;
}

void Json::Print() const
{
	cout << "{" << endl;
	for(unsigned int i=0;i < this->_obj.size();i++)
	{
		cout << "  " << this->_obj[i];
		if(i!=this->_obj.size()-1)
			cout << "," << endl;
		else
			cout << endl;		
	}
	cout << "}" << endl;
}

const int Json::Sum() const
{
	int summation = 0;
	for(unsigned int i = 0 ; i < this->_obj.size() ; i++)
	{
		summation += this->_obj[i].getVal();
	}
	return summation;
}

void Json::Mean() const
{
	double meanVal = this->Sum();
	meanVal = meanVal / this->_obj.size();
	cout.precision(1);
	cout << "The average of the values is: " << fixed << meanVal << "." << endl;
}

void Json::Maximum() const
{
	int max = this->_obj[0].getVal();
	int maxPos = 0;
	for(unsigned int i = 1 ; i < this->_obj.size() ; i++)
	{
		if(this->_obj[i].getVal() > max)
		{
			max = this->_obj[i].getVal();
			maxPos = i;
		}
	}
	cout << "The maximum element is: { \"" << this->_obj[maxPos].getKey();
	cout << "\" : " << this->_obj[maxPos].getVal() << " }." << endl;

}

void Json::Minimum() const
{
	int min = this->_obj[0].getVal();
	int minPos = 0;
	for(unsigned int i = 1 ; i < this->_obj.size() ; i++)
	{
		if(this->_obj[i].getVal() < min)
		{
			min = this->_obj[i].getVal();
			minPos = i;
		}
	}
	cout << "The minimum element is: { \"" << this->_obj[minPos].getKey();
	cout << "\" : " << this->_obj[minPos].getVal() << " }." << endl;
}

void Json::Add(string& Key,int & Val)
{
	JsonElem temp(Key,Val);
	this->_obj.push_back(temp);
}

ostream&
operator << (ostream& os, const JsonElem& j)
{
   return (os << "\"" << j._key << "\" : " << j._value);
}

