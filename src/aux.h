//
//  splay.h
//  graphcut
//
//  Created by Ivan Dmitrievsky on 16.11.13.
//  Copyright (c) 2013 Ivan Dmitrievsky. All rights reserved.
//

#ifndef __graphcut__splay__
#define __graphcut__splay__

template <class T>
struct TreeNode
{
    TreeNode<T> *pathParent;
    TreeNode<T> *left, *right;
    TreeNode<T> *parent;
    double key;
    T *element;
    TreeNode<T>(const double& init = double()) : left(0), right(0), parent(0), key(init), element(0), pathParent(0) { }
};

template <class T>
class AuxTree
{
    
protected:
    unsigned long _size;
    TreeNode<T> *root;
    
    void leftRotate(TreeNode<T> *x);
    void rightRotate(TreeNode<T> *x);
    TreeNode<T> *findNode(const double &key);
    void replace(TreeNode<T> *u, TreeNode<T> *v);
    TreeNode<T>* subtreeMinimum(TreeNode<T> *u);
    TreeNode<T>* subtreeMaximum(TreeNode<T> *u);
    
public:
    AuxTree() : root(0), _size(0) { }
    
    void insert(const double &key, T *element);
    T *find(const double &key);
    void splay(TreeNode<T> *x);
    void erase(const double &key);
    
    const double& minimum() { return subtreeMinimum(root)->key; }
    const double& maximum() { return subtreeMaximum(root)->key; }
    
    bool empty() const { return root == 0; }
    unsigned long size() const { return _size; }
};

/* Protected methods */

template <class T>
void AuxTree<T>::leftRotate(TreeNode<T> *x)
{
    TreeNode<T> *y = x->right;
    x->right = y->left;
    if (y->left)
        y->left->parent = x;
    
    y->parent = x->parent;
    if(!x->parent)
    {
        root = y;
        y->pathParent = x->pathParent;
    }
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    
    y->left = x;
    x->parent = y;
}

template <class T>
void AuxTree<T>::rightRotate(TreeNode<T> *x)
{
    TreeNode<T> *y = x->left;
    x->left = y->right;
    if (y->right)
        y->right->parent = x;
    
    y->parent = x->parent;
    if (!x->parent)
    {
        root = y;
        y->pathParent = x->pathParent;
    }
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    
    y->right = x;
    x->parent = y;
}

template <class T>
TreeNode<T> *AuxTree<T>::findNode(const double &key)
{
    TreeNode<T> *z = root;
    while (z)
    {
        if(z->key < key)
            z = z->right;
        else if (key < z->key)
            z = z->left;
        else
            return z;
    }
    return 0;
}

template <class T>
void AuxTree<T>::replace(TreeNode<T> *u, TreeNode<T> *v)
{
    if (!u->parent)
    {
        root = v;
        v->pathParent = u->pathParent;
    }
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    
    if (v)
        v->parent = u->parent;
}

template <class T>
TreeNode<T>* AuxTree<T>::subtreeMinimum(TreeNode<T> *u)
{
    while (u->left)
        u = u->left;
    return u;
}

template <class T>
TreeNode<T>* AuxTree<T>::subtreeMaximum(TreeNode<T> *u)
{
    while (u->right)
        u = u->right;
    return u;
}

/* Public methods */

template <class T>
void AuxTree<T>::insert(const double &key, T *element)
{
    TreeNode<T> *z = root;
    TreeNode<T> *p = 0;
    
    while (z)
    {
        p = z;
        if (z->key < key)
            z = z->right;
        else
            z = z->left;
    }
    
    z = new TreeNode<T>(key);
    z->element = element;
    z->parent = p;
    
    if (!p)
        root = z;
    else if (p->key < z->key)
        p->right = z;
    else
        p->left = z;
    
    splay(z);
    _size++;
}

template <class T>
T *AuxTree<T>::find(const double &key)
{
    TreeNode<T> *z = root;
    while (z)
    {
        if(z->key < key)
            z = z->right;
        else if (key < z->key)
            z = z->left;
        else
            return z->element;
    }
    return 0;
}

template <class T>
void AuxTree<T>::splay(TreeNode<T> *x)
{
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

template <class T>
void AuxTree<T>::erase(const double &key)
{
    TreeNode<T> *z = findNode(key);
    if (!z)
        return;
    
    splay(z);
    
    if (!z->left)
        replace(z, z->right);
    else if (!z->right)
        replace(z, z->left);
    else
    {
        TreeNode<T> *y = subtreeMinimum(z->right);
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

#endif /* defined(__graphcut__splay__) */
