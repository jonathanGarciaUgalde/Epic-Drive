//
// Created by jonathan

#ifndef HARFS_CLIENT_BNODE_H
#define HARFS_CLIENT_BNODE_H

#include "BRecord.h"

//void* elements: || BNode* 0 || BRecord* 0 || BNode* 1 || BRecord* 1 || BNode* 2 || BRecord* 2 || BNode* 3 || ...(Orden 3)
//                      0      ,      1      ,   2      ,    3      ,      4      ,     5      ,      6
//                   0*N+0*R       1*N+0*R     1*N+1*R     2*N+1*R       2*N+2*R      (i-(i/2))*N+(i/2)*R     3*N+3*R
template <typename  DataType, typename  KeyType> class BNode {
    BNode* father;
    KeyType** keys;
    BNode* sons;
    BRecord<DataType,KeyType>* records = 0;
    int order;
    int lenght;
    bool evaluateConditions();
    void split();
    void binaryDeletion(KeyType*);
    bool binaryInsertion(KeyType*, DataType*, int, int);
    DataType* binarySearch(KeyType*, int, int);
    public:
        BNode(int, BNode*);
        BNode(const BNode&);
        void operator =(const BNode&);
        ~BNode();
        DataType* search(KeyType);
        DataType* search(KeyType*);
        bool insert(KeyType,DataType);
        bool insert(KeyType*,DataType*);
        void remove(KeyType);
        void remove(KeyType*);
        int getLenght();
};

#endif //HARFS_CLIENT_BNODE_H
