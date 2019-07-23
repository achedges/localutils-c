//
// Created by Adam Hedges on 2019-07-16.
//

#ifndef LOCALUTILS_FILES_H
#define LOCALUTILS_FILES_H


#include <fcntl.h>
#include <zconf.h>


void download_file(const char* url, const char* filepath);
void extract_file(const char* archivepath, const char* extractpath, int deletearchive);


#endif //LOCALUTILS_FILES_H
