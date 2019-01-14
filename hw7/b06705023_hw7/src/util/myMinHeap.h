/****************************************************************************
  FileName     [ myMinHeap.h ]
  PackageName  [ util ]
  Synopsis     [ Define MinHeap ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_MIN_HEAP_H
#define MY_MIN_HEAP_H

#include <algorithm>
#include <vector>

template <class Data>
class MinHeap
{
public:
   MinHeap(size_t s = 0) { if (s != 0) _data.reserve(s); }
   ~MinHeap() {}

   void clear() { _data.clear(); }

   // For the following member functions,
   // We don't respond for the case vector "_data" is empty!
   const Data& operator [] (size_t i) const { return _data[i]; }   
   Data& operator [] (size_t i) { return _data[i]; }

   size_t size() const { return _data.size(); }

   // TODO
   const Data& min() const
   {return _data[0];}

   void insert(const Data& d) 
   {
      int piv = _data.size();
      _data.push_back(d);
      while(piv >0 &&  _data[piv] < _data[(piv-1)/2])
      {
         swap(_data[piv],_data[(piv-1)/2]);
         piv = (piv-1)/2;
      }
   }

   void delMin() 
   {
      int piv = _data.size()-1;
      _data[0] = _data.back();

      piv = 0;
      _data.pop_back();
      int thresh = _data.size();
      
      while(piv < thresh)
      {
         int small=0;
         if(piv*2 +2 < thresh)
         {
            if(_data[piv*2+1].getLoad() <= _data[piv*2+2].getLoad())
               small = piv*2+1;
            else
               small = piv*2+2;
         }
         else if(piv*2 +1 <thresh)
            small = piv*2+1;
         else
            break;
         if(_data[small] < _data[piv])
         {
            swap(_data[piv],_data[small]);
            piv = small;
         }
         else
            break;
      }
      // for(int i=0;i<this->_data.size();i++)
      //    cout << _data[i] << endl;
   }
   void delData(size_t i) 
   {
      int piv = _data.size()-1;
      int s = piv+1;
      _data[i] = _data.back();

      piv = i;
      _data.pop_back();

      while(piv < _data.size())
      {
         int small=0;
         if(piv*2 +2 < _data.size() && _data[piv*2+1] < _data[piv*2+2])
            small = piv*2+1;
         else if (piv*2 +2 < _data.size())
            small = piv*2+2;
         else
            break;
         if(_data[small] < _data[piv])
         {
            
            swap(_data[piv],_data[small]);
            piv = small;
         }
         else
            break;
      }
   }

private:
   // DO NOT add or change data members
   vector<Data>   _data;
};

#endif // MY_MIN_HEAP_H
