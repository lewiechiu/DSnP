/****************************************************************************
  FileName     [ array.h ]
  PackageName  [ util ]
  Synopsis     [ Define dynamic array package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef ARRAY_H
#define ARRAY_H

#include <cassert>
#include <algorithm>

using namespace std;

// NO need to implement class ArrayNode
//
template <class T>
class Array
{
public:
   // TODO: decide the initial value for _isSorted
   Array() : _data(0), _size(0), _capacity(256), _isSorted(true) 
   {
      this->_data = new T [this->_capacity];
   }
   ~Array() { delete []_data; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class Array;

   public:
      iterator(T* n= 0): _node(n) {}
      iterator(const iterator& i): _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return (*this); }
      T& operator * () { return (*_node); }
      iterator& operator ++ () { this->_node++; return (*this); }
      iterator operator ++ (int) { iterator tmp(_node); operator++(); return tmp; }
      iterator& operator -- () { this->_node--; return (*this); }
      iterator operator -- (int) {  iterator tmp(_node); operator--(); return tmp; }

      iterator operator + (int i) const { (_node + i); return (*this); }
      iterator& operator += (int i) { (_node + i);return (*this); }

      iterator& operator = (const iterator& i) { this->_node = i._node; return (*this); }

      bool operator != (const iterator& i) const { if(this->_node == i._node)return false;else{return true;}}
      bool operator == (const iterator& i) const { if(this->_node == i._node)return true;else{return false;}}

   private:
      T*    _node;
   };

   // TODO: implement these functions
   iterator begin() const 
   {
      if(this->_size == 0)
         return 0;
      else
         return iterator(&this->_data[0]);
   }
   iterator end() const 
   {
      if(this->_size == 0)
         return 0;
      else
      //[CONCERN] will we get to some point where it is out of range??
         return iterator(&this->_data[_size]);
   }

   bool empty() const 
   {
      if(this->_size == 0 )
         return true;
      else
         return false;
   }
   size_t size() const {return this->_size;}

   T& operator [] (size_t i) { return _data[i]; }
   const T& operator [] (size_t i) const { return _data[i]; }

   void push_back(const T& x) 
   {
      if(this->_size == this->_capacity)
         this->resize(this->_capacity*2);
      this->_data[this->_size] = x;
      this->_size++;
   }
   
   void pop_front() 
   {
      if(this->_size==0)
         return ;
      if(this->_size==1)
      {
         this->_size=0;
         return ;
      }
      this->_data[0] = this->_data[this->_size-1];
      this->_size --;
   }
   
   void pop_back() 
   {
      if(this->_size >0)
         this->_size --;
      else
         return;
   }

   bool erase(iterator pos) 
   { 
      for(int i=0;i<this->_size;i++)
         if(this->_data[i]==*pos)
            {
               this->_data[i] = this->_data[this->_size-1];
               this->_size --;
               return true;
            }
      return false;
   }
   bool erase(const T& x) { return false; }

   iterator find(const T& x) { return end(); }

   void clear() {this->_size = 0;}

   // [Optional TODO] Feel free to change, but DO NOT change ::sort()
   void sort() const { if (!empty()) ::sort(_data, _data+_size); }
   void resize(size_t n)
   {
      if(n <= this->_capacity)
         return;
      T* tmp = new T[n];
      for(int i=0;i< this->_size; i++)
         tmp[i] = this->_data[i];
      
      delete [] this->_data;
      this->_data = tmp;
      this->_capacity = n;
   }
   void reserve(size_t n)
   {
      T* tmp = new T[this->_capacity+n];
      for(int i=0;i<this->_size;i++)
         tmp[i] = this->_data[i];
      delete [] this->_data;
      this->_data = tmp;
      this->_capacity += n;
   }
   // Nice to have, but not required in this homework...
   // void reserve(size_t n) { ... }
   // void resize(size_t n) { ... }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   T*            _data;
   size_t        _size;       // number of valid elements
   //We update _size whenever we insert or pops element out.
   //do NOT uses it as parameter to go through *_data
   //It ONLY servese as a counter.


   size_t        _capacity;   // max number of elements
   mutable bool  _isSorted;   // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] Helper functions; called by public member functions
};

#endif // ARRAY_H
