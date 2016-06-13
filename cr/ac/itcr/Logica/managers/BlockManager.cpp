//
// Created by jonathan
//

#include "BlockManager.h"

/**@brief elimina un registro y los registros anexos al mismo
 * @param long offset: numero de registro (posicion)
 */
BlockManager::BlockManager(std::string pathParam, int numberOfBlocksParam){
    if(pathParam.length() && numberOfBlocksParam){
        numberOfBlocks = numberOfBlocksParam;
        firstEmptyBlock = 0;
        usedBlocks = 0;
        path = pathParam;
        FileManager::createFile(path, BLOCK_MANAGER_HEADER_LENGHT + numberOfBlocksParam*BLOCK_LENGHT);
        //Escribe el header
        updateHeader();
        //Llena los bloques
        int flag=-1;
        int tmp;
        for(int i = 0; i < numberOfBlocks ; i++){
            if(i<numberOfBlocksParam-1) tmp = i+1;
            else tmp = flag;
            FileManager::writeFile(std::addressof(tmp), path, BLOCK_MANAGER_HEADER_LENGHT+i*BLOCK_LENGHT+NEXT_LENGHT,
                                   NEXT_EMPTY_LENGHT);
            FileManager::writeFile(std::addressof(flag), path, BLOCK_MANAGER_HEADER_LENGHT+i*BLOCK_LENGHT, NEXT_LENGHT);
        }
    }else{
        printf(NULL_PARAMETER);
        throw NULL_PARAMETER_CODE;
    }
}

/**@brief elimina un registro y los registros anexos al mismo
 */
BlockManager::BlockManager(){
   numberOfBlocks = 0;
    firstEmptyBlock = 0;
    usedBlocks = 0;
    path = "";
}

void BlockManager::changeStorageBlock(std::string newPath){
    path = newPath;
    updateHeader();
}

/**@brief actualiza el header
 */
void BlockManager::updateHeader() {
    FileManager::writeFile(std::addressof(firstEmptyBlock), path,
                           FIRST_EMPTY_POSITION*BLOCK_MANAGER_COMPONENT_LENGHT, BLOCK_MANAGER_COMPONENT_LENGHT);
    FileManager::writeFile(std::addressof(usedBlocks), path,
                           USED_POSITION*BLOCK_MANAGER_COMPONENT_LENGHT, BLOCK_MANAGER_COMPONENT_LENGHT);
    FileManager::writeFile(std::addressof(numberOfBlocks), path,
                           MAX_BLOCKS_POSITION*BLOCK_MANAGER_COMPONENT_LENGHT, BLOCK_MANAGER_COMPONENT_LENGHT);
}

/**@brief agrega un nuevo registro
 * @param void* data: contenido
 * @param long size: tamano
 */
long BlockManager::addBlock(void* data, long dataSize){
    if(data){
        if(firstEmptyBlock>-1 || dataSize <= (numberOfBlocks-usedBlocks)*DATA_LENGHT){
            //toma el id
            int id = firstEmptyBlock;
            int flag = -1;
            //toma el id
            int tmp;
            for(int i = 0; i < dataSize/DATA_LENGHT; i++){
                //toma el id
                tmp = firstEmptyBlock;
                //Escribe el dato, suma porque:  | siguiente | siguiente vacio | dato
                FileManager::writeFile(static_cast<char*>(data+i*DATA_LENGHT), path, BLOCK_MANAGER_HEADER_LENGHT+ firstEmptyBlock*BLOCK_LENGHT
                                                                                     +NEXT_LENGHT+NEXT_EMPTY_LENGHT, DATA_LENGHT);
                //Toma el nuevo primer ultimo, suma porque:  | siguiente | siguiente vacio | dato
                FileManager::readFile(std::addressof(firstEmptyBlock), path, BLOCK_MANAGER_HEADER_LENGHT+firstEmptyBlock*BLOCK_LENGHT+NEXT_LENGHT,
                                      NEXT_EMPTY_LENGHT);
                //Escribe el siguiente vacio en -1, suma porque:  | siguiente | siguiente vacio | dato
                FileManager::writeFile(std::addressof(flag), path, BLOCK_MANAGER_HEADER_LENGHT+tmp*BLOCK_LENGHT+NEXT_LENGHT,
                                       NEXT_EMPTY_LENGHT);
                usedBlocks++;
                if(i < dataSize/DATA_LENGHT-1) {
                    //Si no es la ultima iteracion agrega siguiente, suma porque:  | siguiente | siguiente vacio | dato
                    FileManager::writeFile(std::addressof(firstEmptyBlock), path, BLOCK_MANAGER_HEADER_LENGHT+tmp*BLOCK_LENGHT,
                                           NEXT_LENGHT);
                }else{
                    //Escribe el siguiente -1, suma porque:  | siguiente | siguiente vacio | dato
                    FileManager::writeFile(std::addressof(flag), path, BLOCK_MANAGER_HEADER_LENGHT+tmp*BLOCK_LENGHT,
                                           NEXT_LENGHT);
                }
            }
            updateHeader();
            return id;
        }else{
            printf(NOT_ENOUGH_SPACE);
            throw NOT_ENOUGH_SPACE_CODE;
        }
    }else{
        printf(NULL_PARAMETER);
        throw NULL_PARAMETER_CODE;
    }
}

/**@brief busca un registro
 * @param long offset: numero de registro (posicion)
 * @return void*: dato del registro junto con los anexos
 */
void* BlockManager::searchBlock(void* toRet, long offset, long counter){
    if(toRet){
        if(offset < numberOfBlocks){
            FileManager::readFile(toRet+counter*DATA_LENGHT, path, BLOCK_MANAGER_HEADER_LENGHT+offset*BLOCK_LENGHT+NEXT_LENGHT+NEXT_EMPTY_LENGHT, DATA_LENGHT);
            counter++;
            long* next = static_cast<long*>(malloc(sizeof(long)));
            FileManager::readFile(next, path, BLOCK_MANAGER_HEADER_LENGHT+offset*BLOCK_LENGHT, NEXT_LENGHT);
            if(*next!=-1)searchBlock(toRet,*next,counter);
            free(next);
            return toRet;
        }else {
            printf(OFFSET_OUT_OF_BOUND);
            throw OFFSET_OUT_OF_BOUND_CODE;
        }
    }else{
        printf(NULL_PARAMETER);
        throw NULL_PARAMETER_CODE;
    }
}

/**@brief elimina un registro y los registros anexos al mismo
 * @param long offset: numero de registro (posicion)
 */
void BlockManager::deleteBlock(int offset){
    if(offset < numberOfBlocks){
        //Escribe el primer vacio como siguiente vacio y actualiza primer vacio
        FileManager::writeFile(std::addressof(firstEmptyBlock), path, BLOCK_MANAGER_HEADER_LENGHT+offset*BLOCK_LENGHT+NEXT_LENGHT,
                               NEXT_EMPTY_LENGHT);
        firstEmptyBlock = offset;
        //Se lee el siguiente
        int* next = static_cast<int*>(malloc(DATA_LENGHT));
        FileManager::readFile(next, path, BLOCK_MANAGER_HEADER_LENGHT+offset*BLOCK_LENGHT, NEXT_LENGHT);
        //si el siguiente no es -1 lo borra, ya que -1 indica que es el ultimo bloque de la cadena
        if(*next!=-1) {
            deleteBlock(*next);
            *next = -1;
            FileManager::writeFile(next, path, offset*BLOCK_LENGHT + BLOCK_MANAGER_HEADER_LENGHT, NEXT_LENGHT);
        }
        usedBlocks--;
        updateHeader();
        free(next);
    }else {
        printf(OFFSET_OUT_OF_BOUND);
        throw OFFSET_OUT_OF_BOUND_CODE;
    }
}

/**@brief imprime los bloques
 */
void BlockManager::print(){
    //Buffer
    int* a = static_cast<int*>(malloc(sizeof(int)));
    //Header
    FileManager::readFile(a, path, FIRST_EMPTY_POSITION*BLOCK_MANAGER_COMPONENT_LENGHT, BLOCK_MANAGER_COMPONENT_LENGHT);
    std::cout << "PRIMER BLOQUE VACIO: " << *a << std::endl;
    FileManager::readFile(a, path, USED_POSITION*BLOCK_MANAGER_COMPONENT_LENGHT, BLOCK_MANAGER_COMPONENT_LENGHT);
    std::cout << "BLOQUES USADOS: " << *a << std::endl;
    FileManager::readFile(a, path, MAX_BLOCKS_POSITION*BLOCK_MANAGER_COMPONENT_LENGHT, BLOCK_MANAGER_COMPONENT_LENGHT);
    std::cout << "TOTAL DE BLOQUES: " << *a << std::endl;
    //Bloques
    for(int i = 0; i < numberOfBlocks ; i++){
        FileManager::readFile(a, path, BLOCK_MANAGER_HEADER_LENGHT+i*BLOCK_LENGHT+NEXT_LENGHT, NEXT_EMPTY_LENGHT);
        std::cout << "BLOQUE " << i << " -----> SIGUIENTE BLOQUE VACIO: " << *a << NULL_CHR;
        FileManager::readFile(a, path, BLOCK_MANAGER_HEADER_LENGHT+i*BLOCK_LENGHT, NEXT_LENGHT);
        std::cout << "SIGUIENTE BLOQUE: " << *a << NULL_CHR;
        FileManager::readFile(a, path, BLOCK_MANAGER_HEADER_LENGHT+i*BLOCK_LENGHT+NEXT_LENGHT+NEXT_EMPTY_LENGHT, DATA_LENGHT);
        std::cout << "DATO: " << *a << NULL_CHR << std::endl;
    }
    std::cout << std::endl;
    free(a);
}