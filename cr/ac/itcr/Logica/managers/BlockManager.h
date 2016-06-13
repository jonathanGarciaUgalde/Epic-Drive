//
// Created by jonathan
//

#ifndef HARFS_DISK_REGISTERMANAGER_H
#define HARFS_DISK_REGISTERMANAGER_H

#include <fstream>
#include "FileManager.h"
#include "../Exceptions.h"

// HEADER:      |    PrimerVacio  4 Bytes   |        Usados 4 Bytes         |   Bloques totales 4 Bytes  | =  12 Bytes

#define BLOCK_MANAGER_HEADER_LENGHT 12
#define BLOCK_MANAGER_COMPONENT_LENGHT 4
#define FIRST_EMPTY_POSITION 0
#define USED_POSITION 1
#define MAX_BLOCKS_POSITION 2

// BLOQUES:     |     Siguiente  4 Bytes    |    Siguiente Vacio 4 Bytes    |       Dato  512 Bytes      | =  520 Bytes

#define BLOCK_LENGHT 12
#define NEXT_LENGHT 4
#define NEXT_EMPTY_LENGHT 4
#define DATA_LENGHT 4
#define NEXT_POSITION 0
#define NEXT_EMPTY_POSITION 1
#define DATA_POSITION 2

class BlockManager {
    std::string path;
    int firstEmptyBlock;
    int usedBlocks;
    int numberOfBlocks;
    void updateHeader();
    public:
        BlockManager();
        BlockManager(std::string, int);
        long addBlock(void*, long);
        void* searchBlock(void*, long, long);
        void* searchBlock(void*);
        void deleteBlock(int);
        void print();
        void changeStorageBlock(std::string);
};

#endif //HARFS_DISK_REGISTERMANAGER_H
