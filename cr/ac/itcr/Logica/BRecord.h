//
// Created by jonathan

#ifndef HARFS_CLIENT_BREGISTER_H
#define HARFS_CLIENT_BREGISTER_H

#include "../Exceptions.h"
#include <malloc.h>

/** Contiene una clave y un dato
 *
 */
template <typename  DataType, typename  KeyType> class BRecord {
    DataType* data = 0;
    KeyType* key = 0;
    public:
        BRecord();
        BRecord(DataType,KeyType);
        ~BRecord();
        BRecord(const BRecord&);
        void operator =(const BRecord&);
        void setData(DataType,KeyType);
        void setData(DataType*,KeyType*);
        bool compareKey(KeyType);
        bool compareKey(KeyType*);
        void operator[](BRecord*);
        void empty();
        DataType* getData();
        KeyType* getKey();
};


#endif //HARFS_CLIENT_BREGISTER_H
