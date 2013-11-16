//
//  splay.h
//  graphcut
//
//  Created by Ivan Dmitrievsky on 16.11.13.
//  Copyright (c) 2013 Ivan Dmitrievsky. All rights reserved.
//

#ifndef __graphcut__splay__
#define __graphcut__splay__

#include <functional>

template< typename T, typename Comp = std::less< T > >
class SplayTree {
private:
    Comp comp;
    unsigned long _size;
    
    struct TrNode
    {
        TrNode *left, *right;
        TrNode *parent;
        T key;
        TrNode(const T& init = T()) : left(0), right(0), parent(0), key(init) { }
    } *root;
    
    void leftRotate(TrNode *x)
    {
        TrNode *y = x->right;
        x->right = y->left;
        if (y->left)
            y->left->parent = x;
        
        y->parent = x->parent;
        if(!x->parent)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        
        y->left = x;
        x->parent = y;
    }
    
    void rightRotate(TrNode *x)
    {
        TrNode *y = x->left;
        x->left = y->right;
        if (y->right)
            y->right->parent = x;
        
        y->parent = x->parent;
        if (!x->parent)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        
        y->right = x;
        x->parent = y;
    }
    
    void splay(TrNode *x) {
        while(x->parent)
        {
            if (!x->parent->parent)
            {
                if (x->parent->left == x)
                    rightRotate(x->parent);
                else
                    leftRotate(x->parent);
                
            }
            else if (x->parent->left == x && x->parent->parent->left == x->parent)
            {
                rightRotate(x->parent->parent);
                rightRotate(x->parent);
            }
            else if (x->parent->right == x && x->parent->parent->right == x->parent)
            {
                leftRotate(x->parent->parent);
                leftRotate(x->parent);
            }
            else if (x->parent->left == x && x->parent->parent->right == x->parent)
            {
                rightRotate(x->parent);
                leftRotate(x->parent);
            }
            else
            {
                leftRotate(x->parent);
                rightRotate(x->parent);
            }
        }
    }
    
    void replace(TrNode *u, TrNode *v)
    {
        if (!u->parent)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        
        if (v)
            v->parent = u->parent;
    }
    
    TrNode* subtreeMinimum(TrNode *u)
    {
        while (u->left)
            u = u->left;
        return u;
    }
    
    TrNode* subtreeMaximum(TrNode *u)
    {
        while (u->right)
            u = u->right;
        return u;
    }
public:
    SplayTree() : root(0), _size(0) { }
    
    void insert(const T &key)
    {
        TrNode *z = root;
        TrNode *p = 0;
        
        while (z)
        {
            p = z;
            if (comp(z->key, key))
                z = z->right;
            else
                z = z->left;
        }
        
        z = new TrNode(key);
        z->parent = p;
        
        if (!p)
            root = z;
        else if (comp(p->key, z->key))
            p->right = z;
        else
            p->left = z;
        
        splay(z);
        _size++;
    }
    
    TrNode* find(const T &key)
    {
        TrNode *z = root;
        while (z)
        {
            if(comp(z->key, key))
                z = z->right;
            else if (comp(key, z->key))
                z = z->left;
            else
                return z;
        }
        return 0;
    }
    
    void erase(const T &key)
    {
        TrNode *z = find(key);
        if (!z)
            return;
        
        splay(z);
        
        if (!z->left)
            replace(z, z->right);
        else if (!z->right)
            replace(z, z->left);
        else
        {
            TrNode *y = subtreeMinimum(z->right);
            if (y->parent != z)
            {
                replace(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            replace(z, y);
            y->left = z->left;
            y->left->parent = y;
        }
        
        _size--;
    }
    
    const T& minimum() { return subtreeMinimum(root)->key; }
    const T& maximum() { return subtreeMaximum(root)->key; }
    
    bool empty() const { return root == 0; }
    unsigned long size() const { return _size; }
};

#endif /* defined(__graphcut__splay__) */
