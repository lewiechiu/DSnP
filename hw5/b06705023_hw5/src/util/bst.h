/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>
#include <stack>

#define LewieIsHandsome true
#define IHateGirls false
//#define DEBUG

using namespace std;

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class BSTreeNode
{
   friend class BSTree<T>;
   friend class BSTree<T>::iterator;
   //BSTreeNode(bool )
   BSTreeNode() :l(0), r(0), parent(nullptr) ,duplicate(0) // dummy node for every leaf
   {}
   BSTreeNode(const T& x, BSTreeNode<T>* L = nullptr, BSTreeNode<T>* R = nullptr) : _data(x) , l(L), r(R), parent(nullptr),  duplicate(0)
   {} 
   ~BSTreeNode()
   {
         l = r = parent = nullptr;
         duplicate = 0;
   }

   T _data;
   BSTreeNode<T> * l;
   BSTreeNode<T> * r; 
   BSTreeNode<T> * parent;  
   size_t duplicate;
   //bool isRed;
   // TODO: design your own class!!
};


template <class T>
class BSTree
{
   // TODO: design your own class!!
   
public:
void sort(){}
   class iterator 
   {
      friend class BSTree;
   public:
      iterator(BSTreeNode<T>* n): _node(n), dup(0){}
      iterator(BSTreeNode<T>* n,BSTreeNode<T>* roo ): _node(n),root(roo), dup(0) {}
      iterator(const iterator& i) : _node(i._node) , dup(i.dup){}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return (this->_node->_data); }
      T& operator * () { return (this->_node->_data); }

      iterator& operator ++ () 
      {
         if(dup < _node->duplicate)
         {
            dup++;
            return *this;
         }
         
         dup = 0;
         
         if( _node->r != NULL )
         {

            _node = _node->r;
            while (_node->l != NULL) 
               _node = _node->l; 

            return *this;
         }
         else
         {

            BSTreeNode<T>* p = _node->parent; 
            BSTreeNode<T>* org = _node;
            while(p != NULL && _node == p->r) 
            { 
               _node = p; 
               p = p->parent; 
            } 
            if(_node == root)
                  _node = NULL;
            else
                  _node = p;
            return *this;
         }
         
      }
      iterator operator ++ (int) 
      { iterator tmp(_node); operator++(); return tmp; }

      iterator& operator -- () 
      {
            
         if(dup > 0)
         {
            dup--;
            return *this;
         }
         if(_node == nullptr)
         {
            _node = root;
            while(_node->r != nullptr)
               _node = _node->r;
            dup = _node->duplicate;
            return *this; 
         }
         if( _node->l != NULL )
         {
            _node = _node->l;
            while (_node->r != NULL) 
               _node = _node->r; 
            dup = _node->duplicate;
            return *this;
         }
         else
         {
            BSTreeNode<T>* p = _node->parent; 
            
            BSTreeNode<T>* org = _node;
            while(p != NULL && _node == p->l) 
            { 
               _node = p; 
               p = p->parent; 
            } 
            if(_node == root)
                  _node = org;
            else
                  _node = p;
            dup = _node->duplicate;
            return *this;
         }
         
      }
      iterator operator -- (int) 
      {
            
            iterator tmp(_node,root); operator--(); return tmp;
      }

      iterator& operator = (const iterator& i)
      {
         this->_node = i._node; dup = i.dup; root = i.root; return (*this);
      }

      bool operator != (const iterator& i) const { if(this->_node == i._node)return false;else{return true;}}
      bool operator == (const iterator& i) const {if(this->_node == i._node)return true;else{return false;}}
   private:
       BSTreeNode<T>* _node;
       BSTreeNode<T>* root;
      size_t dup;
   };

   
   BSTree<T> () : _size(0)
   { _root = new BSTreeNode<T>();}

   void clear() 
   {
      BSTreeNode<T>* ptr = _root;
      stack<BSTreeNode<T>* > tmp;
      do
      { 
         // Move to leftmost node 
         while (ptr != nullptr) 
         { 
            //Move to left-most point
            if (ptr->r) 
                  tmp.push(ptr->r);
            tmp.push(ptr); 
            ptr = ptr->l; 
         } 

         
         ptr = tmp.top();
         tmp.pop();

         //check if we can go right.
         if (ptr && ptr->r && !tmp.empty() && tmp.top() == ptr->r) 
         { 

            tmp.pop();  
            tmp.push(ptr); // push root back to stack 
            ptr = ptr->r; // change root so that the right  
                                 // child is processed next 
         } 
         else  // Else print root's data and set root as NULL 
         { 

            delete ptr;
            ptr = nullptr;
         }

      }  while (!tmp.empty()); 

      _root = new BSTreeNode<T>();
      this->_size = 0;
   }

   bool empty() const {return _size == 0;}
   
   iterator begin() const
   {
      if(_size == 0)
         return 0;
      BSTreeNode<T>* ptr = this->_root;
      while(ptr->l!= nullptr)
         ptr = ptr->l;

      return iterator(ptr,_root);
      // return left-most node;
   }
   
   
   iterator end() const
   {
      if(_size == 0)
         return 0;
      BSTreeNode<T>* ptr = this->_root;
      while(ptr->r != nullptr)
         ptr = ptr->r;
      ptr = ptr->r;
      
      return iterator(ptr,_root);
      //
   }
   
   size_t size() const {return _size;}
   void pop_front()
   {
      // pop begin()
      if(_size==0)
         return;
      if(_size == 1)
      {

         _size--;
         return ;
      }
      _size --;
      
      BSTreeNode<T>* tod = _root;
      
      while(tod->l != nullptr)
         tod = tod->l;
      
      if(tod->duplicate!=0)
      {
         tod->duplicate--;
         return ;
      }

      if(tod->r == nullptr)
      {
         tod->parent->l = nullptr;
         delete tod;
         return ;
      }
      else
      {
         if(tod != _root)
         {
            tod->parent->l = tod->r;
            tod->r->parent = tod->parent;
         }
         else
         {
            _root = tod->r;
            _root->parent = nullptr;
         }
         delete tod;
      }
      
   }
   void pop_back()
   {
      // remove the largest (right-most)
     // pop begin()
      if(_size==0)
         return;
      if(_size == 1)
      {

         _size--;
         return ;
      }
      _size --;
      
      BSTreeNode<T>* tod = _root;
      
      while(tod->r != nullptr)
         tod = tod->r;
      if(tod->duplicate!=0)
      {
         tod->duplicate--;
         return ;
      }

      if(tod->l == nullptr)
      {
         tod->parent->r = nullptr;
         delete tod;
         return ;
      }
      else
      {
         if(tod != _root)
         {
            tod->parent->r = tod->l;
            tod->l->parent = tod->parent;
         }
         else
         {
            _root = tod->l;
            _root->parent = nullptr;

         }
         delete tod;
      }
      
   }
   bool erase(const T& x)
   {
      if(_size==0)
         return false;
      if(_size == 1)
      {
         _size = 0;
         return true;
      }
      BSTreeNode<T>* ins = Search(x);
      if(ins->_data != x)
            return false;
      return erase(find(x));
      
   }
   bool erase(iterator pos)
   {
      // get the elem in pos and erase it
      // this should be a constant time opertion
      #ifdef DEBUG
      cout << "delete " << pos._node << endl;
      #endif
      if(pos == end())
            return false;
      if(_size==0)
         return false;
      if(_size == 1)
      {
         _size = 0;
         return true;
      }
      BSTreeNode<T>* ins = pos._node;
      if(ins->duplicate!=0)
      {
            ins->duplicate --;
            _size--;
            return true;
      }
      
      
      
      if(ins->r != nullptr && ins->l != nullptr)
      {
            BSTreeNode<T>* smallest = ins->r;
            while(smallest->l!=nullptr)
                  smallest = smallest->l;
            if(smallest == ins->r)
            {
                  smallest->l = ins->l;
                  smallest->l->parent = smallest;
                  smallest->parent = ins->parent;
                  if(smallest->parent != NULL && ins->parent->l == ins) smallest->parent->l = smallest;
                  else if(smallest->parent != NULL && ins->parent->r == ins) smallest->parent->r = smallest;
                  else { _root = smallest; smallest->parent = nullptr;}
                  delete ins;
                  _size--;
                  return true;
            }
            else
            {
                  if(smallest->r == nullptr)
                  {
                        //take care of the right sub-tree
                        //connect it to the parent of smallest
                        #ifdef DEBUG
                        cout << "2 r null" << endl;
                        #endif
                        ins->_data = smallest->_data;
                        ins->duplicate = smallest->duplicate;
                        smallest->parent->l = nullptr;
                        #ifdef DEBUG
                        cout << smallest << endl;
                        #endif
                        delete smallest;
                        _size--;
                        return true;
                  }
                  else
                  {
                       // cout << smallest->parent << " " << smallest->r->parent << endl;
                       #ifdef DEBUG
                       cout << "2- " << endl;
                       #endif
                        smallest->parent->l = smallest->r;
                        smallest->r->parent = smallest->parent;
                        #ifdef DEBUG
                        cout << smallest << endl;
                        cout << smallest->parent->l << " " << smallest->r->parent << endl;
                        #endif
                        ins->_data = smallest->_data;
                        ins->duplicate = smallest->duplicate;
                         #ifdef DEBUG
                        cout << smallest << endl;
                        #endif
                        delete smallest;
                        _size--;
                        return true;
                  }
                  
                  // change the complete node
                  //normal case
            }
      }
      else if (ins->r)
      {
             #ifdef DEBUG
            cout << "r case: " << endl;
            #endif
            if(ins->parent== nullptr)
            {

                  _root = ins->r;
                  ins->r->parent = nullptr;
                  delete ins;
                  _size--;
                  return true;
            }
            if(ins->parent->r == ins) ins->parent->r = ins->r;
            else ins->parent->l = ins->r;
            ins->r->parent = ins->parent;
            delete ins;

            _size--;
            return true;
      }
      else if(ins->l)
      {
             #ifdef DEBUG
            cout << "l case: " << endl;
            #endif
            if(ins->parent== nullptr)
            {

                  _root = ins->l;
                  ins->l->parent = nullptr;
                  delete ins;
                  _size--;
                  return true;
            }
            if(ins->parent->r == ins) ins->parent->r = ins->l;
            else ins->parent->l = ins->l;
            ins->l->parent = ins->parent;
            delete ins;
 
            _size--;
            return true;
      }
      else
      {
            #ifdef DEBUG
            cout << "no child" << endl;
            #endif
            if(ins->parent->r == ins) ins->parent->r = nullptr;
            else ins->parent->l = nullptr;
            delete ins;
            _size--;
            return true;
      }
   }
   iterator find(const T& x) const
   {
      BSTreeNode<T>* ptr =  Search(x);
      if(ptr->_data == x)
         return iterator(ptr,_root);
      return end();
   }
   void insert(const T& x) 
   {
      if(_size==0)
      {
         
         _root->_data = x;
         //_root->isRed = false;
         _size++;
         _root->parent = nullptr;
         return ;
      }

      _size++;
      BSTreeNode<T>* ins = Search(x);
      if(x > ins->_data)
      {
         
         ins->r = new BSTreeNode<T>(x,nullptr,nullptr);
         ins->r->parent = ins;
         ins = ins->r;

      }
      else if (x < ins->_data)
      {
         ins->l = new BSTreeNode<T>(x,nullptr,nullptr);
         ins->l->parent = ins;
         ins = ins->l;
         
      }
      else
      {
         ins->duplicate++;
         return ;
      }
   }

   void print () const
   {
      BSTreeNode<T>* ptr = _root;
      stack<BSTreeNode<T>* > tmp;
      while(tmp.empty() ==false || ptr != nullptr)
      {
         while(ptr != nullptr)
         {
            tmp.push(ptr);
            ptr = ptr->l;
         }
         ptr = tmp.top();
         tmp.pop();
         
         cout << ptr->_data;

         ptr = ptr->r;
      }
   }

private:
   // [MEMBER]
      BSTreeNode<T>* _root;
   size_t _size;
   BSTreeNode<T>* Search(const T& x ) const
   {
      BSTreeNode<T>* loc = _root;
      BSTreeNode<T>* prev = loc;
      while(loc != nullptr && loc->_data != x)
      {
         prev = loc;
         if(loc->_data < x)
            loc = loc->r;
         else
            loc = loc->l;
      }
      if(loc == NULL)
         return prev;
      return loc;
   }
};

#endif // BST_H
