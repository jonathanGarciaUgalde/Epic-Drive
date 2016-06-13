//
// Created by jonathan
//

#ifndef HARFS_CLIENT_BTREE_H
#define HARFS_CLIENT_BTREE_H
#include "//home/jonathan/ClionProjects/Epic-Drive/cr/ac/itcr/Logica/Buffer.h"

#include "/home/jonathan/ClionProjects/Epic-Drive/cr/ac/itcr/Logica/Manager/FileManager.h"
#include <math.h>

#define DEFAULT_ORDER 100
#define DEFAULT_KEY_LENGHT 8
#define DEFAULT_NAME "tr"

#define HEADER_LENGHT 33
#define HEADER_OFFSET 8
#define ORDER_ON_HEADER 0
#define FLOORS_ON_HEADER 1
#define LENGHT_ON_HEADER 2
#define FIRST_EMPTY_ON_HEADER 3
#define KEY_LENGHT_ON_HEADER 4

#define NODE_OFFSET 7
#define NODE_ELEMENT_LENGHT 8
#define TERMINAL_ON_NODE 0
#define FATHER_ON_NODE 1
#define LAST_KEY 2
#define NEXT_EMPTY 3

#define SMALLER_CODE 0
#define EQUAL_CODE 1
#define BIGGER_CODE 2

/**HEADER: | ORDEN 8 BYTES | PISOS 8 BYTES | LONGITUD 8 BYTES | PRIMER VACIO 8 BYTES | LONGITUD DE CLAVE 1 BYTE |*/

/**NODO:   | TERMINAL 1 BYTES | PADRE 8 BYTES | ULTIMA CLAVE 8 BYTES | NEXT EMPTY 8 BYTES | PUNTERO 8 BYTES | CLAVE N BYTES | PUNTERO 8 BYTES | PUNTERO 8 BYTES | PUNTERO 8 BYTES |*/


class BTree {
    std::string headerPath;
    std::string dataPath;
    int nodeLenght;
    long order;
    long floors;
    long lenght;
    long firstEmpty;
    unsigned char keyLenght;
    void init(std::string);
    long maximun(int);
    int compare(void*,void*,int);
    void readHeader();
    void updateHeader();
    void readAgainForBinaryMethods(long,bool*,long*);
    long binarySearch(void*,bool,long,long,long);
    void binaryDeletion(void*,bool,long,long,long);
    bool binaryInsertion(void*,long,bool,long,long,long);
    void rotateRight(Buffer*,long, int);
    void rotateLeft(Buffer*,long, int);
    void mix(long,long);
    void split(long);
    public:
        BTree();
        BTree(std::string,long,long);
        BTree(long,long);
        BTree(std::string,std::string);
        void insertKey(void*,long);
        long searchKey(void*);
        void deleteKey(void*);
};

#endif //HARFS_CLIENT_BTREE_H
