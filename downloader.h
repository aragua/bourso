#ifndef DOWNLOADER_H
#define DOWNLOADER_H

struct raw_url_content {
	/* url info */
	char * path;
	/* buffer */
	unsigned int size;
	char * content;
	unsigned int w_offset;
};

struct raw_url_content * ruc_new(char * path);
int ruc_download(struct raw_url_content * ruc);
void ruc_free(struct raw_url_content * ruc);

#endif /*  DOWNLOADER_H */
