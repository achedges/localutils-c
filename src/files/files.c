//
// Created by Adam Hedges on 2019-07-16.
//

#include "files.h"


size_t _write_data(void* buffer, size_t size, size_t nmemb, void* f) {
	size_t bytes = size * nmemb;
	if (bytes <= 0)	return 0;
	fwrite(buffer, size, nmemb, f);
	return bytes;
}

void download_file(const char* url, const char* filepath) {
	CURL* curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _write_data);

	FILE* f = fopen(filepath, "w");

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, f);
	curl_easy_perform(curl);

	fclose(f);

	curl_easy_cleanup(curl);
}

void extract_file(const char* archivepath, const char* extractpath) {
	// https://libzip.org/
}