//
// Created by roberto on 20/06/15.
//

#ifndef HARFS_CLIENT_STORAGEBLOCKMANAGER_H
#define HARFS_CLIENT_STORAGEBLOCKMANAGER_H
#include "../../../libs/rapidjson/document.h"
#include "../pathConstants.h"
#include "../dataStructuresOnHD/Buffer.h"
#include "FileManager.h"
#include "BlockManager.h"
#include "../dataStructuresOnHD/List.h"
#include "../network/JsonWriter.h"
#include "../network/TCPServer.h"

#define HEADER_NAME "storage.blocks"
#define BLOCK_NAME_LENGHT 32
#define NUMBER_OF_BLOCKS_LENGHT 4

#define CLIENTS_FILE "clients.md"
#define STORAGE_BLOCKS_EXT ".storage"
#define LIST_EXT ".list"

//USERS:  | Cantidad 4 B | Nombre 32 B | Clave 8 B | Nombre 32 B | Clave 8 B |...
//HEADER: | Cantidad 4 B | Nombre 128 B | Nombre 128 B |...

class StorageBlockManager {
    int users;
    int lenght;
    BlockManager* blocks;
    pthread_mutex_t* mutex;
    List* metadata;
    Buffer* storageBlocks;
    static StorageBlockManager* manager;
    StorageBlockManager();
    ~StorageBlockManager();
    void updateHeader();
    bool addStorageBlocks(void*,void*);
    bool addRegister(void*,long,int);
    bool deleteRegister(void*,int);
    bool confirmUser(void*,void*);
public:
    static StorageBlockManager* getInstance();
    void messageHandler(std::string);
    void addRegister(std::string);
    void deleteRegister(std::string);
    void newStorageBlock(std::string);
    void getRegister(std::string);
};


#endif //HARFS_CLIENT_STORAGEBLOCKMANAGER_H
