//
// Created by roberto on 06/06/15.
//

#ifndef HARFS_CLIENT_REGISTERMANAGER_H
#define HARFS_CLIENT_REGISTERMANAGER_H



#include <fstream>
#include "FileManager.h"
#include "../dataStructuresOnHD/List.h"
#include "../../dataStructures/DoubleLinkedList.h"
#include "BlockManager.h"
#include "../pathConstants.h"

// HEADER:      | PrimerVacio  4 Bytes | Usados 4 Bytes | Registros totales 4 Bytes | =  12 Bytes

#define REGISTER_MANAGER_HEADER_LENGHT 12
#define REGISTER_MANAGER_COMPONENT_LENGHT 4
#define FIRST_EMPTY_POSITION 0
#define USED_POSITION 1
#define MAX_REGISTERS_POSITION 2

class RegisterManager {
    BlockManager blocks;
public:
    RegisterManager(int,int);
    bool insertRegister(void*,void*);
    bool deleteRegister(void*);
    void* searchRegister(void*,int);
};

#endif //HARFS_CLIENT_REGISTERMANAGER_H