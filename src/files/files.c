//
// Created by Adam Hedges on 2019-07-16.
//

#include "files.h"
#include "curl.h"
#include "libzip/zip.h"


void download_file(const char* url, const char* filepath) {
	FILE* f = fopen(filepath, "w");
	CURL* curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, f);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	CURLcode result = curl_easy_perform(curl);

	curl_version_info_data* versionInfoData = curl_version_info(CURLVERSION_NOW);

	if (result != CURLE_OK) {
		printf("curl error %d (v. %s)\n", result, versionInfoData->version);
	}

	fclose(f);
	curl_easy_cleanup(curl);
}

void extract_file(const char* archivepath, const char* extractpath, int deletearchive) {
	int ziperr = 0;
	zip_t* archive = zip_open(archivepath, 0, &ziperr);

	mode_t filewriteopt = O_CREAT | O_RDWR | O_TRUNC;
	mode_t filepermsopt = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

	if (archive != NULL) {
		for (int i = 0; i < zip_get_num_entries(archive, 0); i++) {
			zip_stat_t stat;
			if (zip_stat_index(archive, i, 0, &stat) == 0) { // able to get file stats...
				zip_file_t* archfile = zip_fopen_index(archive, i, 0);

				int extractfile = open(extractpath, filewriteopt, filepermsopt);
				long long byteswritten = 0;
				char buffer[100];
				while (byteswritten != stat.size) {
					int _len = zip_fread(archfile, buffer, 100);
					write(extractfile, buffer, _len);
					byteswritten += _len;
				}

				close(extractfile);
				zip_fclose(archfile);
			}
		}
	}

	zip_close(archive);
	if (deletearchive) remove(archivepath);
}