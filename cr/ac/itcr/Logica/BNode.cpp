//
// Created by jonathan
//

#include "BNode.h"

/**@brief Constructor
*/
template <typename  DataType, typename  KeyType> BNode::BNode(int orderParam, BNode* fatherParam){
    lenght = 0;
    sons = 0;
    keys = static_cast<KeyType**>(calloc(order,sizeof(KeyType*)));
    records = static_cast<BRecord*>(calloc(order,sizeof(BRecord)));
    father = fatherParam;
    order = orderParam;
}

/**@brief Constructor copia
*/
template <typename  DataType, typename  KeyType> BNode::BNode(const BNode& other){

}

/**@brief Operador de igualdad
*/
template <typename  DataType, typename  KeyType> void BNode::operator =(const BNode& other){

}

/**@brief Destructor
*/
template <typename  DataType, typename  KeyType> BNode::~BNode(){
    father = 0;
    free(keys);
    free(sons);
    free(records);
}

/**@brief Busqueda binaria
*/
template <typename  DataType, typename  KeyType> bool BNode::binaryInsertion(KeyType* toSearch, DataType* data, int begin, int end){
    int offset;
    //Si no se ha disminuido al minimo el tamano
    if(begin!=end) {
        //Si se ha encontrado el dato
        if (*(keys + (end - begin) / 2) == *toSearch) {
            if (records) return false;
            else offset = (end - begin) / 2;
        }
            //Si el dato es menor que la clave
        else if (*(keys + (end - begin) / 2) < *toSearch) return binaryInsertion(toSearch, data, begin, (end - begin) / 2);
        else return binaryInsertion(toSearch, data, 1 + (end - begin) / 2, end);
    }
        //Si se ha dismunuido el tamano al minimo
    else{
        if(*(keys + begin) == *toSearch) {
            if(records) return false;
            else offset = begin;
        }
        else if(*(keys + begin)<*toSearch) offset = begin;
        else offset = begin + 1;
    }
    //Si no retorna, busca en el hijo o retorna error
    if(sons) return (sons+offset)->insert(toSearch,data);
    else{
        printf(KEY_NOT_FOUND);
        throw OFFSET_OUT_OF_BOUND_CODE;
    }
}

/**@brief Busqueda binaria
*/
template <typename  DataType, typename  KeyType> DataType* BNode::binarySearch(KeyType* toSearch, int begin, int end){
    int offset;
    //Si no se ha disminuido al minimo el tamano
    if(begin!=end) {
        //Si se ha encontrado el dato
        if (*(keys + (end - begin) / 2) == *toSearch) {
            if (records) return records + (end - begin) / 2;
            else offset = (end - begin) / 2;
        }
            //Si el dato es menor que la clave
        else if (*(keys + (end - begin) / 2) < *toSearch) return binarySearch(toSearch, begin, (end - begin) / 2);
        else return binarySearch(toSearch, 1 + (end - begin) / 2, end);
    }
    //Si se ha dismunuido el tamano al minimo
    else{
        if(*(keys + begin) == *toSearch) {
            if(records) return records + begin;
            else offset = begin;
        }
        else if(*(keys + begin)<*toSearch) offset = begin;
        else offset = begin + 1;
    }
    //Si no retorna, busca en el hijo o retorna error
    if(sons) return (sons+offset)->search(toSearch);
    else{
        printf(KEY_NOT_FOUND);
        throw OFFSET_OUT_OF_BOUND_CODE;
    }
}

/**@brief Busqueda publica
*/
template <typename  DataType, typename  KeyType> DataType* BNode::search(KeyType toSearch){
    search(&toSearch);
}

/**@brief Busqueda publica
*/
template <typename  DataType, typename  KeyType> DataType* BNode::search(KeyType* toSearch){
    binarySearch(toSearch,0,lenght-1);
}

/**@brief Insercion
*/
template <typename  DataType, typename  KeyType> bool BNode::insert(KeyType keyParam,DataType dataParam){
    insert(&keyParam,&dataParam);
}

/**@brief Insercion
*/
template <typename  DataType, typename  KeyType> bool BNode::insert(KeyType* keyParam,DataType* dataParam){
     binaryInsertion(keyParam,dataParam,0,lenght-1);
}

/**@brief Eliminacion
*/
template <typename  DataType, typename  KeyType> void BNode::remove(KeyType toRemove){
    remove(&toRemove);
}

/**@brief Eliminacion
*/
template <typename  DataType, typename  KeyType> void BNode::remove(KeyType* toRemove){

}
