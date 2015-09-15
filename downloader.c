#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "downloader.h"

struct raw_url_content * ruc_new(char * path)
{
	struct raw_url_content * ruc;
	ruc = malloc(sizeof(struct raw_url_content));
	if (ruc) {
		ruc->path = strdup(path);
		if (!ruc->path) {
			perror("ruc->path");
			free(ruc);
			goto exit;
		}
		ruc->size = 1000000;
		ruc->w_offset = 0;
		ruc->content = malloc(ruc->size);
		if (!ruc->content) {
			perror("ruc->content");
			free(ruc->path);
			free(ruc);
			goto exit;
		}
	}
exit:
	return ruc;
}

void ruc_free(struct raw_url_content * ruc)
{
	if (ruc) {
		if (ruc->path)
			free(ruc->path);
		if (ruc->content)
			free(ruc->content);
	}
}

static unsigned int write_cb(char *in, unsigned int size, unsigned int nmemb, struct raw_url_content *ruc)
{
	uint r;
	r = size * nmemb;
	if (r > (ruc->size - ruc->w_offset)) {
		fprintf(stderr, "Not enough memory available\n");
		return -1;
	}
	memcpy(ruc->content+ruc->w_offset, in, r);
	ruc->w_offset += r;
	ruc->content[ruc->w_offset] = 0;
	return(r);
}

int ruc_download(struct raw_url_content * ruc)
{
	CURL *curl;
	CURLcode res;

	if (!ruc || !ruc->path || !ruc->content)
		return -EINVAL;

	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, ruc->path);
		/* example.com is redirected, so we tell libcurl to follow redirection */
		//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, ruc);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));

		/* always cleanup */
		curl_easy_cleanup(curl);
	}

	return 0;
}

#ifdef TEST_RUC
int __attribute__((weak)) main(int argc, char **argv)
{
	if (argc < 1)
		return -1;
	struct raw_url_content * ruc = NULL;

	ruc = ruc_new(argv[1]);
	if (!ruc)
		return -ENOMEM;
	ruc_download(ruc);
	printf("%s", ruc->content);
	ruc_free(ruc);
}
#endif
