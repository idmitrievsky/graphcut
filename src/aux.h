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
struct Aux
{
    Aux<T> *pathParent;
    Aux<T> *left, *right;
    Aux<T> *parent;
    int key;
    T *element;
    Aux<T>(T *el, int init = 0) : left(0), right(0), parent(0), key(init), element(el), pathParent(0) { }

    Aux<T>*  splay(void);
    Aux<T>*  rightRotate(void);
    Aux<T>*  leftRotate(void);
    Aux<T>* subtreeMinimum(void);
    Aux<T>* subtreeMaximum(void);
    Aux<T>* findNode(const int key);
    void replace(Aux<T> *v);
    Aux<T>*  insert(const int key, T *element);
    T *find(const int key);
    void erase(const int key);
};

template <class T>
Aux<T>*  Aux<T>::splay(void)
{
    Aux<T>* x = this;
    Aux<T>* temp = 0;

    while (x->parent)
    {
        if (!x->parent->parent)
        {
            if (x->parent->left == x)
            {
                x = x->parent->rightRotate();
            }
            else
            {
                x = x->parent->leftRotate();
            }
        }
        else if (x->parent->left == x && x->parent->parent->left == x->parent)
        {
            temp = x->parent->parent->rightRotate();
            x = temp->parent->rightRotate();
        }
        else if (x->parent->right == x && x->parent->parent->right == x->parent)
        {
            temp = x->parent->parent->leftRotate();
            x = temp->parent->leftRotate();
        }
        else if (x->parent->left == x && x->parent->parent->right == x->parent)
        {
            x = x->parent->rightRotate();
            x = x->parent->leftRotate();
        }
        else
        {
            x = x->parent->leftRotate();
            x = x->parent->rightRotate();
        }
    }
    return x;
}

template <class T>
Aux<T>*  Aux<T>::leftRotate(void)
{
    Aux<T> *y = right;
    right = y->left;
    if (y->left)
        y->left->parent = this;

    y->parent = parent;
    if (!parent)
    {
        y->pathParent = pathParent;
    }
    else if (this == parent->left)
        parent->left = y;
    else
        parent->right = y;

    y->left = this;
    parent = y;
    return y;
}

template <class T>
Aux<T>*  Aux<T>::rightRotate(void)
{
    Aux<T> *y = left;
    left = y->right;
    if (y->right)
        y->right->parent = this;

    y->parent = parent;
    if (!parent)
    {
        y->pathParent = pathParent;
    }
    else if (this == parent->left)
        parent->left = y;
    else
        parent->right = y;

    y->right = this;
    parent = y;
    return y;
}

template <class T>
Aux<T>* Aux<T>::subtreeMinimum(void)
{
    Aux<T> *u = this;

    while (u->left)
        u = u->left;
    return u;
}

template <class T>
Aux<T>* Aux<T>::subtreeMaximum(void)
{
    Aux<T> *u = this;

    while (u->right)
        u = u->right;
    return u;
}

template <class T>
Aux<T> *Aux<T>::findNode(const int key)
{
    Aux<T> *z = this;
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
void Aux<T>::replace(Aux<T> *v)
{
    assert(v);
    if (!parent)
    {
        v->pathParent = pathParent;
    }
    else if (this == parent->left)
        parent->left = v;
    else
        parent->right = v;

    v->parent = parent;
}

template <class T>
Aux<T> *Aux<T>::insert(const int key, T *element)
{
    Aux<T> *z = this;
    Aux<T> *p = 0;
    Aux<T> *newRoot = 0;

    while (z)
    {
        p = z;
        if (z->key < key)
            z = z->right;
        else
            z = z->left;
    }

    z = new Aux<T>(element, key);
    z->parent = p;

    if (!p)
        ;
    else if (p->key < z->key)
        p->right = z;
    else
        p->left = z;

    return newRoot = z->splay();
}

template <class T>
T *Aux<T>::find(const int key)
{
    Aux<T> *z = this;
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
void Aux<T>::erase(const int key)
{
    Aux<T> *z = findNode(key);
    if (!z)
        return;

    z->splay();

    if (!z->left)
        z->replace(z->right);
    else if (!z->right)
        z->replace(z->left);
    else
    {
        Aux<T> *y = z->right->subtreeMinimum();
        if (y->parent != z)
        {
            y->replace(y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        z->replace(y);
        y->left = z->left;
        y->left->parent = y;
    }
}

#endif /* defined(__graphcut__splay__) */
