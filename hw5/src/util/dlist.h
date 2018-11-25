/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>
#include <vector>
#include <algorithm>
template <class T> class DList;
//#define DEBUG
// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   // [NOTE] DO NOT ADD or REMOVE any data member
   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   // TODO: decide the initial value for _isSorted
   DList(): _size(0){
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; // _head is a dummy node
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return (this->_node->_data); }
      T& operator * () { return (this->_node->_data); }
      iterator& operator ++ () { this->_node = this->_node->_next; return *(this); }
      iterator operator ++ (int) { iterator tmp(_node); operator++(); return tmp; }
      iterator& operator -- () { this->_node = this->_node->_prev; return (*this); }
      iterator operator -- (int) {  iterator tmp(_node); operator--(); return tmp; }

      iterator& operator = (const iterator& i) { this->_node = i._node; return (*this); }

      bool operator != (const iterator& i) const { if(this->_node == i._node)return false;else{return true;}}
      bool operator == (const iterator& i) const {if(this->_node == i._node)return true;else{return false;}}

   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const {return iterator(this->_head->_next);}
   iterator end() const {return iterator(this->_head);}
   
   bool empty() const {return _size == 0;}
   size_t size() const {  return this->_size; }

   void push_back(const T& x) 
   {
      
//      cout << this->_size << endl;
      
      DListNode<T> *post = this->_head;
      DListNode<T> *prev = this->_head->_prev;
      DListNode<T> *ins = new DListNode<T>(x,prev,post);
      post->_prev = ins;
      prev->_next = ins;

      #ifdef DEBUG
         cout << x << endl;
         cout << "post prev: " << post->_prev << endl;
         cout << "prev next: " << prev->_next << endl;
         cout << "post: " << post << endl;
         cout << "prev: " << prev << endl;
         cout << "ins: " << ins << endl;
         DListNode<T> *ptr = this->_head->_next;
         cout << "inside list :" << endl;
         while(ptr!=this->_head)
         {
            cout << ptr->_data << endl;
            ptr = ptr->_next;
         }
      #endif

      this->_size++;
      return;
      
   }
   void pop_front() 
   {
      if(_size ==0)
         return ;
      DListNode<T> *prev = this->_head;
      DListNode<T> *tod = prev->_next;
      DListNode<T> *post = tod->_next;
      prev->_next = post;
      post->_prev = prev;
      this->_size --;
      delete tod;
      
   }
   void pop_back() 
   {
      if(_size == 0)
         return ;
      DListNode<T> *post = this->_head;
      DListNode<T> *tod = post->_prev;
      DListNode<T> *prev = tod->_prev;
      
      prev->_next = post;
      post->_prev = prev;
      this->_size --;
      delete tod;

   }

   // return false if nothing to erase
   bool erase(iterator pos) 
   {
      if(_size == 0)
         return false;

      iterator ptr = begin();
      while(ptr!=end())
      {
         if(*ptr == *pos)
         {
            
            DListNode<T> *tod = ptr._node;
            
            DListNode<T> *prev = tod->_prev;
            DListNode<T> *post = tod->_next;
            
            prev->_next = post;
            post->_prev = prev;
            this->_size--;
            delete tod;
            return true;
            
         }
         ptr++;
      }
      return false;
   }
   bool erase(const T& x) 
   {
      if(_size == 0)
         return false;

      iterator ptr = begin();
      while(ptr!=end())
      {
         if(*ptr==x)
         {
            DListNode<T> *tod = ptr._node;
            DListNode<T> *prev = tod->_prev;
            DListNode<T> *post = tod->_next;
            
            prev->_next = post;
            post->_prev = prev;
            this->_size--;
            delete tod;
            return true;
            
         }
         ptr++;
      }
      return false;
      
   }

   iterator find(const T& x) 
   {
      if(begin()==end())
         return end();
      iterator at =begin();
      while(at!=end())
      {
         if(*at== x)
            return at;
         at++;
      }
      return end();
   }

   void clear() 
   {
      while(begin()!=end())
         pop_front();
      this->_size = 0;
      return ;
   }  // delete all nodes except for the dummy node

   void sort() const 
   {
      vector<T> tmp;
      
   }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   DListNode<T>*  _head;     // = dummy node if list is empty
   mutable bool   _isSorted; // (optionally) to indicate the array is sorted
   size_t _size;
   // [OPTIONAL TODO] helper functions; called by public member functions
};

#endif // DLIST_H
