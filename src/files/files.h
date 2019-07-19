//
// Created by Adam Hedges on 2019-07-16.
//

#ifndef LOCALUTILS_FILES_H
#define LOCALUTILS_FILES_H


#include <curl.h>
#include <libzip/zip.h>
#include <fcntl.h>
#include <zconf.h>


void download_file(const char* url, const char* filepath);
void extract_file(const char* archivepath, const char* extractpath);


#endif //LOCALUTILS_FILES_H
