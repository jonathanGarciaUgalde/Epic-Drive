//
// Created by jonathan on 6/8/16.
//

#include "BTree.h"

/**@brief contruye un arbol b en disco con los atributos por defecto
 */
BTree::BTree(){
    keyLenght = DEFAULT_KEY_LENGHT;
    order = DEFAULT_ORDER;
    firstEmpty = -1;
    lenght = 0;
    floors = 1;
    init(DEFAULT_NAME);
}

/**@brief construye un arbol b a gusto del usuario
 * @param long keyLenghtParam: tamano que se reservara para cada clave
 * @param long orderParam: numero de claves por nodos
 */
BTree::BTree(long keyLenghtParam, long orderParam){
    keyLenght = keyLenghtParam;
    order = orderParam;
    firstEmpty = -1;
    lenght = 0;
    floors = 1;
    init(DEFAULT_NAME);
}

/**@brief construye un arbol b a gusto del usuario
 * @param std::string name: nombre de los archivos
 * @param long keyLenghtParam: tamano que se reservara para cada clave
 * @param long orderParam: numero de claves por nodos
 */
BTree::BTree(std::string name, long keyLenghtParam, long orderParam){
    keyLenght = keyLenghtParam;
    order = orderParam;
    firstEmpty = -1;
    lenght = 0;
    floors = 1;
    init(name);
}

/**@brief reconstruye un arbol b a partir de sus archivos
 * @param std::string headerPathParam: ruta del header (.header)
 * @param std::string dataPathparam: ruta de los nodos (.data)
 */
BTree::BTree(std::string headerPathParam, std::string dataPathParam){
    headerPath = headerPathParam;
    dataPath = dataPathParam;
    readHeader();
}

/**@brief lee las variables necesarias del header
 */
void BTree::readHeader() {
    FileManager::readFile(std::addressof(order),headerPath,ORDER_ON_HEADER*HEADER_OFFSET,sizeof(order));
    FileManager::readFile(std::addressof(floors),headerPath,FLOORS_ON_HEADER*HEADER_OFFSET,sizeof(floors));
    FileManager::readFile(std::addressof(keyLenght),headerPath,KEY_LENGHT_ON_HEADER*HEADER_OFFSET,sizeof(keyLenght));
    FileManager::readFile(std::addressof(lenght),headerPath,LENGHT_ON_HEADER*HEADER_OFFSET,sizeof(lenght));
    FileManager::readFile(std::addressof(firstEmpty),headerPath,FIRST_EMPTY_ON_HEADER*HEADER_OFFSET,sizeof(firstEmpty));
}

/**@brief crea los archivos y los inicializa
 * @param std::string name: nombre que tendran los archivos .header y .data
 */
void BTree::init(std::string name) {
    nodeLenght = NODE_OFFSET + 3*NODE_ELEMENT_LENGHT + order*(NODE_ELEMENT_LENGHT + keyLenght);
    //Crea las rutas
    headerPath = PathConstants::PROJECT_PATH + name + PathConstants::HEADER_EXT;
    dataPath = PathConstants::PROJECT_PATH + name + PathConstants::DATA_EXT;
    //Crea el header
    FileManager::createFile(headerPath, HEADER_LENGHT);
    FileManager::writeFile(std::addressof(order),headerPath,ORDER_ON_HEADER*HEADER_OFFSET,sizeof(order));
    FileManager::writeFile(std::addressof(keyLenght),headerPath,KEY_LENGHT_ON_HEADER*HEADER_OFFSET,sizeof(keyLenght));
    updateHeader();
    //Crea el primer nodo
    FileManager::createFile(dataPath,nodeLenght);
}

/**@brief actualiza la longitud y el primer vacio
 */
void BTree::updateHeader() {
    FileManager::writeFile(std::addressof(floors),headerPath,FLOORS_ON_HEADER*HEADER_OFFSET,sizeof(floors));
    FileManager::writeFile(std::addressof(lenght),headerPath,LENGHT_ON_HEADER*HEADER_OFFSET,sizeof(lenght));
    FileManager::writeFile(std::addressof(firstEmpty),headerPath,FIRST_EMPTY_ON_HEADER*HEADER_OFFSET,sizeof(firstEmpty));
}

/**@brief compara dos datos
 * @param void* one: primer dato
 * @param void* two: segundo dato
 * @param int size: tamano de los datos
 */
int BTree::compare(void* one, void* two, int elementSize){
    for(int i = 0; i < elementSize; i++){
        if(*static_cast<char*>(one+i)<*static_cast<char*>(two+i)) return SMALLER_CODE;
        else if(*static_cast<char*>(one+i)>*static_cast<char*>(two+i)) return BIGGER_CODE;
    }
    return EQUAL_CODE;
}

/**@brief devuelve el maximo de claves hasta un piso dado
 * @param int floor: piso
 */
long BTree::maximun(int floor){
    return order*pow(order+1, floor-1);
}

/**@brief recorre el arbol de forma binaria
 * @param void* key: llave a insertar
 * @param bool terminal: valor que indica si la hoja es terminal
 * @param long node: nodo actual
 * @param long first: primer indice
 * @param long last: ultimo indice
 */
long BTree::binarySearch(void* key, bool terminal, long node, long first, long last){
    Buffer* toCompare;
    long pointer;
    if(first != last) Buffer* toCompare = FileManager::readFile(dataPath,
                                                                node*nodeLenght+NODE_OFFSET+(keyLenght+NODE_ELEMENT_LENGHT)*(last-first)/2,
                                                                2*NODE_ELEMENT_LENGHT+keyLenght);
    else toCompare = FileManager::readFile(dataPath,
                                           node*nodeLenght+NODE_OFFSET+(keyLenght+NODE_ELEMENT_LENGHT)*last,
                                           2*NODE_ELEMENT_LENGHT+keyLenght);
    int comparison = compare(key,toCompare->get(NODE_ELEMENT_LENGHT),keyLenght);
    if(comparison == EQUAL_CODE){
        pointer = *static_cast<long*>(toCompare->get(0));
        //Si el nodo no es terminal
        if(terminal) return pointer;
        else{
            readAgainForBinaryMethods(pointer,std::addressof(terminal),std::addressof(last));
            first = 0;
        }
    }else{
        if(terminal){
            printf(OFFSET_OUT_OF_BOUND);
            throw OFFSET_OUT_OF_BOUND_CODE;
        }else if(comparison == SMALLER_CODE){
            if(first!=last) last = first+(last-first)/2;
            else{
                pointer = *static_cast<long*>(toCompare->get(0));
                readAgainForBinaryMethods(pointer,std::addressof(terminal),std::addressof(last));
                first = 0;
            }
        }else{
            if(first!=last) first = first+(last-first)/2;
            else {
                pointer = *static_cast<long*>(toCompare->get(NODE_ELEMENT_LENGHT+keyLenght));
                readAgainForBinaryMethods(pointer,std::addressof(terminal),std::addressof(last));
                first = 0;
            }
        }
    }
    free(toCompare);
    return binarySearch(key, terminal, pointer, first, last);
}

/**@brief actualiza variables
 * @param long pointer
 * @param bool* terminal
 * @param long* last
 */
void BTree::readAgainForBinaryMethods(long pointer, bool* terminal, long* last) {
    //Lee el header de hijo
    Buffer* nodeHeader = FileManager::readFile(dataPath, pointer*nodeLenght, NODE_OFFSET);
    *last = *static_cast<long*>(nodeHeader->get(NODE_ELEMENT_LENGHT+1));
    //Actualiza variables
    if(static_cast<char*>(nodeHeader->get(0))==" ") *terminal = false;
    else *terminal = true;
    free(nodeHeader);
}

/**@brief inserta un elemento al arbol
 * @param void* key: clave a insertar
 * @param long pointer: puntero con al dato representado por la clave
 */
void BTree::insertKey(void* key, long pointer){
    if(lenght<=maximun(floors)){
        Buffer* nodeHeader = FileManager::readFile(dataPath, 0, NODE_OFFSET);
        long last = *static_cast<long*>(nodeHeader->get(NODE_ELEMENT_LENGHT+1));
        bool terminal;
        //Actualiza variables
        if(static_cast<char*>(nodeHeader->get(0))==" ") terminal = false;
        else terminal = true;
        free(nodeHeader);
        binaryInsertion(key, pointer,terminal,0,0,last);
    }else{
        split(0);
    }
}

/**@brief recorre el arbol de forma binaria
 * @param void* key: llave a insertar
 * @param long pointer: puntero al dato asociado a la llave
 * @param bool terminal: valor que indica si la hoja es terminal
 * @param long node: nodo actual
 * @param long first: primer indice
 * @param long last: ultimo indice
 */
bool BTree::binaryInsertion(void* key, long pointerToInsert, bool terminal, long node, long first, long last){
    Buffer* toCompare;
    long pointer;
    if(first != last) Buffer* toCompare = FileManager::readFile(dataPath,
                                                                node*nodeLenght+NODE_OFFSET+(keyLenght+NODE_ELEMENT_LENGHT)*(last-first)/2,
                                                                2*NODE_ELEMENT_LENGHT+keyLenght);
    else toCompare = FileManager::readFile(dataPath,
                                           node*nodeLenght+NODE_OFFSET+(keyLenght+NODE_ELEMENT_LENGHT)*last,
                                           2*NODE_ELEMENT_LENGHT+keyLenght);
    int comparison = compare(key,toCompare->get(NODE_ELEMENT_LENGHT),keyLenght);
    if(comparison == EQUAL_CODE){
        pointer = *static_cast<long*>(toCompare->get(0));
        //Si el nodo no es terminal
        if(terminal){
            printf(OFFSET_OUT_OF_BOUND);
            throw OFFSET_OUT_OF_BOUND_CODE;
        }else{
            readAgainForBinaryMethods(pointer,std::addressof(terminal),std::addressof(last));
            first = 0;
        }
    }else{
        if(terminal){
            //TODO
        }else if(comparison == SMALLER_CODE){
            if(first!=last) last = first+(last-first)/2;
            else{
                pointer = *static_cast<long*>(toCompare->get(0));
                readAgainForBinaryMethods(pointer,std::addressof(terminal),std::addressof(last));
                first = 0;
            }
        }else{
            if(first!=last) first = first+(last-first)/2;
            else {
                pointer = *static_cast<long*>(toCompare->get(NODE_ELEMENT_LENGHT+keyLenght));
                readAgainForBinaryMethods(pointer,std::addressof(terminal),std::addressof(last));
                first = 0;
            }
        }
    }
    free(toCompare);
    return binaryInsertion(key, pointerToInsert, terminal, pointer, first, last);
}

/**@brief rota a partir de una clave
 * @param long offset ubicacion de clave
 */
void BTree::rotateLeft(Buffer* toInsert, long node, int key){
    //lee el header del nodo
    Buffer* nodeToRead = FileManager::readFile(dataPath, node*nodeLenght, nodeLenght+16);
    long last = *static_cast<long*>(nodeToRead->get(NODE_ELEMENT_LENGHT+1));
    bool terminal;
    //Actualiza variables
    if(static_cast<char*>(nodeToRead->get(0))==" ") terminal = false;
    else terminal = true;
    free(nodeToRead);
    //Lee la clave a mover
    Buffer* keyToMove = FileManager::readFile(dataPath, node*nodeLenght+NODE_OFFSET+key*(8+keyLenght), 16+keyLenght);
    if(terminal){
        if(key<order){
            bool tmpFlag = true;
            for(int i = NODE_ELEMENT_LENGHT; i<NODE_ELEMENT_LENGHT+keyLenght; i++){
                if(*static_cast<unsigned char*>(keyToMove->get(i)) != 0) tmpFlag = false;
            }
            if(tmpFlag) rotateLeft(keyToMove, *static_cast<long*>(toInsert->get(0)), 0);
            free(toInsert);
        }else{
            FileManager::writeFile(toInsert->get(NODE_ELEMENT_LENGHT),dataPath, node*nodeLenght+NODE_OFFSET+key*(8+keyLenght), keyLenght);
            rotateLeft(keyToMove, *static_cast<long*>(nodeToRead->get(nodeLenght)),0);
            free(toInsert);
        }
    }else{
        FileManager::writeFile(toInsert->get(NODE_ELEMENT_LENGHT),dataPath, node*nodeLenght+NODE_OFFSET+key*(8+keyLenght), keyLenght);
        rotateLeft(toInsert, *static_cast<long*>(toInsert->get(0)),0);
        free(toInsert);
    }
}

/**@brief rota a partir de una clave
 * @param long offset ubicacion de clave
 */
void BTree::rotateRight(Buffer* toInsert, long node, int key){
    //lee el header del nodo
    Buffer* nodeToRead = FileManager::readFile(dataPath, node*nodeLenght, nodeLenght+16);
    long last = *static_cast<long*>(nodeToRead->get(NODE_ELEMENT_LENGHT+1));
    bool terminal;
    //Actualiza variables
    if(static_cast<char*>(nodeToRead->get(0))==" ") terminal = false;
    else terminal = true;
    free(nodeToRead);
    //Lee la clave a mover
    Buffer* keyToMove = FileManager::readFile(dataPath, node*nodeLenght+NODE_OFFSET+key*(8+keyLenght), 16+keyLenght);
    if(terminal){
        if(key<order){
            FileManager::writeFile(toInsert->get(NODE_ELEMENT_LENGHT),dataPath, node*nodeLenght+NODE_OFFSET+key*(8+keyLenght), keyLenght);
            bool tmpFlag = true;
            for(int i = NODE_ELEMENT_LENGHT; i<NODE_ELEMENT_LENGHT+keyLenght; i++){
                if(*static_cast<unsigned char*>(keyToMove->get(i)) != 0) tmpFlag = false;
            }
            if(tmpFlag) rotateRight(keyToMove, *static_cast<long*>(toInsert->get(NODE_ELEMENT_LENGHT+keyLenght)),0);
            free(toInsert);
        }else{
            FileManager::writeFile(toInsert->get(NODE_ELEMENT_LENGHT),dataPath, node*nodeLenght+NODE_OFFSET+key*(8+keyLenght), keyLenght);
            rotateRight(keyToMove, *static_cast<long*>(nodeToRead->get(nodeLenght+8)),0);
            free(toInsert);
        }
    }else{
        FileManager::writeFile(toInsert->get(NODE_ELEMENT_LENGHT),dataPath, node*nodeLenght+NODE_OFFSET+key*(8+keyLenght), keyLenght);
        rotateRight(toInsert, *static_cast<long*>(toInsert->get(NODE_ELEMENT_LENGHT+keyLenght)),0);
        free(toInsert);
    }
}

/**@brief divide el arbol
 */
void BTree::split(long node){
    Buffer* nodeToRead = FileManager::readFile(dataPath, node*nodeLenght, nodeLenght+24);
    if(static_cast<char*>(nodeToRead->get(0))==" "){
        long next = *static_cast<long*>(nodeToRead->get(nodeLenght+16));
        if(firstEmpty!=-1){
            Buffer* nextEmptyLeft = FileManager::readFile(dataPath, firstEmpty*nodeLenght, nodeLenght+24);
            long tmpNext = firstEmpty;
            firstEmpty = *static_cast<long*>(nextEmptyLeft->get((NEXT_EMPTY-1)*NODE_ELEMENT_LENGHT+1));
            if(firstEmpty!=-1){
                Buffer* nextEmptyRight = FileManager::readFile(dataPath, firstEmpty*nodeLenght, nodeLenght+24);

            }else{

            }
        }else{

        }
        FileManager::writeFile(static_cast<char*>(" "), dataPath, node*nodeLenght, nodeLenght+24);
        split(next);
    }
}

/**@brief mezcla dos nodos
 * @param long nodeOne
 * @param long nodeTwo
 */
void BTree::mix(long nodeOne, long nodeTwo){
    //TODO
}

/**@brief busca un elemento en el arbol
 * @param void* key: clave sobre la que se operara
 */
long BTree::searchKey(void* key){
    //Lee el header de la raiz
    Buffer* nodeHeader = FileManager::readFile(dataPath, 0, NODE_OFFSET);
    long last = *static_cast<long*>(nodeHeader->get(NODE_ELEMENT_LENGHT+1));
    bool terminal;
    //Actualiza variables
    if(static_cast<char*>(nodeHeader->get(0))==" ") terminal = false;
    else terminal = true;
    free(nodeHeader);
    return binarySearch(key,terminal,0,0,last);
}


/**@brief borra una clave
 * @param void* key: llave a insertar
 * @param bool terminal: valor que indica si la hoja es terminal
 * @param long node: nodo actual
 * @param long first: primer indice
 * @param long last: ultimo indice
 */
void BTree::binaryDeletion(void* key, bool terminal, long node, long first, long last){
    Buffer* toCompare;
    long pointer;
    if(first != last) Buffer* toCompare = FileManager::readFile(dataPath,
                                                                node*nodeLenght+NODE_OFFSET+(keyLenght+NODE_ELEMENT_LENGHT)*(last-first)/2,
                                                                2*NODE_ELEMENT_LENGHT+keyLenght);
    else toCompare = FileManager::readFile(dataPath,
                                           node*nodeLenght+NODE_OFFSET+(keyLenght+NODE_ELEMENT_LENGHT)*last,
                                           2*NODE_ELEMENT_LENGHT+keyLenght);
    int comparison = compare(key,toCompare->get(NODE_ELEMENT_LENGHT),keyLenght);
    if(comparison == EQUAL_CODE){
        pointer = *static_cast<long*>(toCompare->get(0));
        //Si el nodo no es terminal
        if(terminal){
            //TODO
        }else{
            readAgainForBinaryMethods(pointer,std::addressof(terminal),std::addressof(last));
            first = 0;
        }
    }else{
        if(terminal){
            printf(OFFSET_OUT_OF_BOUND);
            throw OFFSET_OUT_OF_BOUND_CODE;
        }else if(comparison == SMALLER_CODE){
            if(first!=last) last = first+(last-first)/2;
            else{
                pointer = *static_cast<long*>(toCompare->get(0));
                readAgainForBinaryMethods(pointer,std::addressof(terminal),std::addressof(last));
                first = 0;
            }
        }else{
            if(first!=last) first = first+(last-first)/2;
            else {
                pointer = *static_cast<long*>(toCompare->get(NODE_ELEMENT_LENGHT+keyLenght));
                readAgainForBinaryMethods(pointer,std::addressof(terminal),std::addressof(last));
                first = 0;
            }
        }
    }
    free(toCompare);
    binaryDeletion(key, terminal, pointer, first, last);
}

/**@brief elimina un elemento del arbol
 * @param void* key: clave sobre la que se operara
 */
void BTree::deleteKey(void* key){
    Buffer* nodeHeader = FileManager::readFile(dataPath, 0, NODE_OFFSET);
    long last = *static_cast<long*>(nodeHeader->get(NODE_ELEMENT_LENGHT+1));
    bool terminal;
    //Actualiza variables
    if(static_cast<char*>(nodeHeader->get(0))==" ") terminal = false;
    else terminal = true;
    free(nodeHeader);
    binaryDeletion(key,terminal,0,0,last);
}