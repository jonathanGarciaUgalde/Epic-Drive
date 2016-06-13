//
// Created by jonthan


#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include "../Logica/pathConstants.h"
#include "../Exceptions.h"
#include "../dataStructuresOnHD/Buffer.h"

#define NULL_CHR " "
#define NULL_CHR_SIZE 1

class FileManager{
public:
    static void createFile(std::string path, int);
    static void writeFile(void*, std::string, int, int);
    static void readFile(void*, std::string, int, int);
    static Buffer* readFile(std::string, int, int);
    static void deleteFile(std::string);
};



