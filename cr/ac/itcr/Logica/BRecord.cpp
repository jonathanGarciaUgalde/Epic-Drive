//
// Created by jonathan
//

#include "BRecord.h"

/** Constructor por defecto
 */
template <typename  DataType, typename  KeyType> BRecord::BRecord(){
    data = 0;
    key = 0;
}

/** Constructor con parametros
 */
template <typename  DataType, typename  KeyType> BRecord::BRecord(DataType dataParam, KeyType keyParam){
    data = static_cast<DataType*>(malloc(sizeof(DataType)));
    key = static_cast<KeyType*>(malloc(sizeof(KeyType)));
    *data = dataParam;
    *key = keyParam;
}

/** Constructor copia
 */
template <typename  DataType, typename  KeyType> BRecord::BRecord(const BRecord& other){
    if(!data) data = static_cast<DataType*>(malloc(sizeof(DataType)));
    if(!key) key = static_cast<KeyType*>(malloc(sizeof(KeyType)));
    *data = *other.data;
    *key = *other.key;
}

/** Operador de igualdad
 */
template <typename  DataType, typename  KeyType> void BRecord::operator =(const BRecord& other){
    new(this) BRecord(other);
}

/** Destructor
 */
template <typename  DataType, typename  KeyType> ~BRecord::BRecord(){
    free(data);
    free(key);
}

/** Asigna un dato y una llave de busqueda
 */
template <typename  DataType, typename  KeyType> void BRecord::setData(DataType dataParam, KeyType keyParam){
    if(!data) data = static_cast<DataType*>(malloc(sizeof(DataType)));
    if(!key) key = static_cast<KeyType*>(malloc(sizeof(KeyType)));
    *data = dataParam;
    *key = keyParam;
}

/** Asigna un dato y una llave de busqueda
 *  No se debe hacer free sobre los punteros pasados como parametros
 */
template <typename  DataType, typename  KeyType> void BRecord::setData(DataType* dataParam, KeyType* keyParam){
    data = dataParam;
    key = keyParam;
}

/** Compara la llave
 */
template <typename  DataType, typename  KeyType> bool BRecord::compareKey(KeyType keyParam){
    return *key == keyParam;
}

/** Compara la llave
 */
template <typename  DataType, typename  KeyType> bool BRecord::compareKey(KeyType* keyParam){
    return *key == *keyParam;
}

/** Metodo swap
 */
template <typename  DataType, typename  KeyType> void BRecord::operator[](BRecord* toSwap){
    DataType tmpData = *data;
    KeyType tmpKey = *key;
}

/** Vacia el registro
 */
template <typename  DataType, typename  KeyType> void BRecord::empty(){
    free(data);
    free(key);
    data = 0;
    key = 0;
}

/** Accede al dato
 */
template <typename  DataType, typename  KeyType> DataType* BRecord::getData(){
    return data;
}

/** Accede a la clave
 */
template <typename  DataType, typename  KeyType> KeyType* BRecord::getKey(){
    return key;
}