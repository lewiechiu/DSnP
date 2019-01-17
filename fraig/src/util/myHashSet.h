/****************************************************************************
  FileName     [ myHashSet.h ]
  PackageName  [ util ]
  Synopsis     [ Define HashSet ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_HASH_SET_H
#define MY_HASH_SET_H

#include <vector>
using namespace std;

//---------------------
// Define HashSet class
//---------------------
// To use HashSet ADT,
// the class "Data" should at least overload the "()" and "==" operators.
//
// "operator ()" is to generate the hash key (size_t)
// that will be % by _numBuckets to get the bucket number.
// ==> See "bucketNum()"
//
// "operator ==" is to check whether there has already been
// an equivalent "Data" object in the HashSet.
// Note that HashSet does not allow equivalent nodes to be inserted
//

class myKey
{
   double in1,in2;
   int location;
public:

   friend class CirMgr;
   myKey():in1(0),in2(0), location(0){} 
   myKey(int a,int b,int loc):in1(a),in2(b),location(loc){}
   size_t operator()()const
   {
      return (in1 * in2 + in1+ in2 );
   }
   bool operator==(const myKey& k)const
   {
      if(in1 == k.in1 && in2 == k.in2)
         return true;
      if(in2 == k.in1 && in1 == k.in2)
         return true;
      return false;
   }
   int getID()const{return location;}
   int geti1()const{return in1;}
   int geti2()const{return in2;}
};

class myFECgrps
{
   
public:
   int times=0;

   size_t key;
   vector<int>  grps;
   friend class CirMgr;
   myFECgrps(): key(0){}
   myFECgrps(size_t KEY) : key(KEY) {}
   size_t operator()()const{return key;}
   bool operator==(const myFECgrps& k) const
   {
      return k.key == key;
   }
   bool operator<(const myFECgrps& k)const
   {
      return key < k.key;
   }
   void add(int a){grps.push_back(a);} 
};


template <class Data>
class HashSet
{
public:
   HashSet(size_t b = 0) : _numBuckets(0), _buckets(0) { if (b != 0) init(b); }
   ~HashSet() { reset(); }

   // TODO: implement the HashSet<Data>::iterator
   // o An iterator should be able to go through all the valid Data
   //   in the Hash
   // o Functions to be implemented:
   //   - constructor(s), destructor
   //   - operator '*': return the HashNode
   //   - ++/--iterator, iterator++/--
   //   - operators '=', '==', !="
   //
   class iterator
   {
      friend class HashSet<Data>;

   public:
      iterator():dat(NULL), _d(NULL){}
      iterator(Data& d) : _d(&d) {}
      iterator(Data& d, vector<Data>* out, size_t n) : _d(&d) ,  dat(out) , n_dat(n)
      {

         row = d() % n_dat;
         for(int i=0;i<dat[row].size();i++)
         {
            if(dat[row][i]==d)
            {
               po = i;
               break;
            }
         }
         for(int i = n_dat-1;i>=0;i--)
         {
            if(!dat[i].empty())
            {
               last = &dat[i].back();
               return;
            }
         }
      }
      ~iterator(){};
      const Data& operator * () const { return *_d; }
      iterator& operator ++ () 
      {
         
         iterator p = *this;
         if(_d == last)
         {
            p._d = NULL;
            // cout << "last" << endl;
            _d = NULL;
            return p;
            
         }
            
         if(po+1<dat[row].size())
         {
            po++;
            _d = &dat[row][po];
            
            return p;
         }

         do
         {
            row++;

         }
         while(row < n_dat && dat[row].empty());

         po = 0;
         _d = &dat[row][po];
         


         return p;
         
      }
      iterator& operator -- () { return (*this); }
      iterator& operator ++ (int) 
      {

         
      }
      iterator& operator -- (int) { return (*this); }
      bool operator != (const iterator& i) const 
      {
         // cout << i._d << endl;
         // cout << _d << endl;
         return i._d != _d;
         
      }
      bool operator == (const iterator& i) const 
      {
         // cout << i._d << endl;
         // cout << _d << endl;
         return i._d == _d;
      }
      iterator& operator = (const iterator& i) const 
      {
         _d = *i;
         row = i.row;
         po = i.po;
         dat = i.dat;
         n_dat = i.n_dat;
         return (*this);
      }
   private:
      Data* _d;
      int row;
      int po;
      vector<Data>* dat;
      size_t n_dat;
      Data* last;
   };

   void init(size_t b) { _numBuckets = b; _buckets = new vector<Data>[b]; }
   void reset() {
      _numBuckets = 0;
      if (_buckets) { delete [] _buckets; _buckets = 0; }
   }
   void clear() {
      for (size_t i = 0; i < _numBuckets; ++i) _buckets[i].clear();
   }
   size_t numBuckets() const { return _numBuckets; }

   vector<Data>& operator [] (size_t i) { return _buckets[i]; }
   const vector<Data>& operator [](size_t i) const { return _buckets[i]; }

   // TODO: implement these functions
   //
   // Point to the first valid data
   iterator begin() const {
      for(int i=0;i< _numBuckets;i++){
         if(!_buckets[i].empty())
         {
            return iterator( _buckets[i][0],this->_buckets,_numBuckets);
         }
            
      }
      return iterator();
   }

   // Pass the end
   iterator end() const {return iterator();}

   // return true if no valid data
   bool empty() const 
   {
      if(_numBuckets==0)
         return true;
      for(int i=0;i<_numBuckets;i++)
         if(!_buckets[i].empty())
            return false;
      
      return true;
   }
   // number of valid data
   size_t size() const 
   {
      size_t s = 0;
      for(int i=0;i<_numBuckets;i++)
      {
         if(!_buckets[i].empty())
            s += _buckets[i].size();
      }
      return s; 
   }

   // check if d is in the hash...
   // if yes, return true;
   // else return false;
   bool check(const Data& d) const 
   {
      size_t _po = bucketNum(d);
      for(int i=0;i<_buckets[_po].size();i++)
         if(_buckets[_po][i]==d)
            return true;
      return false;
   }

   // query if d is in the hash...
   // if yes, replace d with the data in the hash and return true;
   // else return false;
   bool query(Data& d) const 
   {
      size_t _po = bucketNum(d);
      for(int i=0;i<_buckets[_po].size();i++)
         if(_buckets[_po][i]==d)
         {
            
            d = _buckets[_po][i];
            return true;
         }
      return false;
   }

   // update the entry in hash that is equal to d (i.e. == return true)
   // if found, update that entry with d and return true;
   // else insert d into hash as a new entry and return false;
   bool update(const Data& d) 
   {
      size_t _po = bucketNum(d);
      for(int i=0;i<_buckets[_po].size();i++)
         if(_buckets[_po][i]==d)
         {
            _buckets[_po][i] = d;
            return true;
         }
      _buckets[_po].push_back(d);
      return false;
   }

   // return true if inserted successfully (i.e. d is not in the hash)
   // return false is d is already in the hash ==> will not insert
   bool insert(const Data& d) 
   {
      size_t _po = bucketNum(d);
      for(int i=0;i<_buckets[_po].size();i++)
         if(_buckets[_po][i]==d)
            return false;
      
      _buckets[_po].push_back(d);
      return true;
   }

   // return true if removed successfully (i.e. d is in the hash)
   // return fasle otherwise (i.e. nothing is removed)
   bool remove(const Data& d) 
   {
      
      size_t _po = bucketNum(d);
      for(int i=0;i<_buckets[_po].size();i++)
      {
         if(_buckets[_po][i]==d)
         {
            _buckets[_po][i] = _buckets[_po].back();
            _buckets[_po].pop_back();
            return true;
         }
      }
         

      return false;
   }

private:
   // Do not add any extra data member
   size_t            _numBuckets;
   vector<Data>*     _buckets;

   size_t bucketNum(const Data& d) const {
      return (d() % _numBuckets); }
};

#endif // MY_HASH_SET_H
