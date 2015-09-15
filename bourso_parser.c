#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "downloader.h"

#define ANSI_COLOR_RED     "\033[22;31m"
#define ANSI_COLOR_GREEN   "\033[22;32m"
#define ANSI_COLOR_YELLOW  "\033[22;33m"
#define ANSI_COLOR_BLUE    "\033[22;34m"
#define ANSI_COLOR_MAGENTA "\033[22;35m"
#define ANSI_COLOR_CYAN    "\033[22;36m"
#define ANSI_COLOR_RESET   "\033[0m"


static int remove_end(char * line, char * end)
{
	char * found;
	found = strstr(line, end);
	if (found) {
		*found=0;
		return 0;
	} else {
		fprintf(stderr, "%s not found in %s\n", end, line);
		return -1;
	}
}

#define MAX_NAME_LEN 30
#define MAX_NBR_LEN 10
static inline void print_loop(char c, int len)
{
	while (len>0) {
		printf(" ");
		len--;
	}
}

int color = 0;
int rt = 1;

static int parse_line (char * line)
{
	char * start;
	if (strstr(line,"<tr")) {
		//printf("\n");
	}
	else if (strstr(line,"</tr>")) {
		if (color)
			printf(ANSI_COLOR_RESET);
		printf("\n");
	} else if (strstr(line,"tdv-\"")) {
		if (strstr(line, "temps")) {
			rt = 1;
			printf("RT ");
		} else {
			rt = 0;
			printf("-- ");
		}
	} else if (strstr(line,"tdv-libelle")) {
		if (remove_end(line,"</a></td>")==0) {
			start = strrchr(line,'>');
			if (start) {
				printf("%s", start+1);
				print_loop(' ',MAX_NAME_LEN-strlen(start+1));
			} else
				print_loop('-',MAX_NAME_LEN);
		}
	} else if (strstr(line,"tdv-last")) {
		char * pattern;
		if (rt)
			pattern = "</span></span></td>";
		else
			pattern = "</span></td>";
		if (remove_end(line,pattern)==0) {
			start = strrchr(line,'>');
			if (start) {
				printf("%s", start+1);
				print_loop(' ',MAX_NBR_LEN-strlen(start+1));
			}
		} else
			print_loop('-',MAX_NBR_LEN);
	} else if (strstr(line,"tdv-var")) {
		if (strstr(line,"icon")) {
			if (strstr(line, "icon-fall")) {
				if (color)
					printf(ANSI_COLOR_RED);
				printf("F ");
			}
			else if (strstr(line, "icon-draw")) {
				if (color)
					printf(ANSI_COLOR_BLUE);
				printf("D ");
			}
			else if (strstr(line, "icon-rise")) {
				if (color)
					printf(ANSI_COLOR_GREEN);
				printf("R ");
			}
			else
				printf("- ");
		} else {
			if (remove_end(line,"</span></td>")==0) {
				start = strrchr(line,'>');
				if (start) {
					printf("%s", start+1);
					print_loop(' ',MAX_NBR_LEN-strlen(start+1));
				}
			} else
				print_loop('-',MAX_NBR_LEN);
		}
	} else if (strstr(line,"tdv-open")) {
		char * pattern;
		if (rt)
			pattern = "</span></span></td>";
		else
			pattern = "</span></td>";
		if (remove_end(line,pattern)==0) {
			start = strrchr(line,'>');
			if (start) {
				printf("%s", start+1);
				print_loop(' ',MAX_NBR_LEN-strlen(start+1));
			}
		} else
			print_loop('-',MAX_NBR_LEN);
	} else if (strstr(line,"tdv-high")) {
		char * pattern;
		if (rt)
			pattern = "</span></span></td>";
		else
			pattern = "</span></td>";
		if (remove_end(line,pattern)==0) {
			start = strrchr(line,'>');
			if (start) {
				printf("%s", start+1);
				print_loop(' ',MAX_NBR_LEN-strlen(start+1));
			}
		} else
			print_loop('-',MAX_NBR_LEN);
	} else if (strstr(line,"tdv-low")) {
		char * pattern;
		if (rt)
			pattern = "</span></span></td>";
		else
			pattern = "</span></td>";
		if (remove_end(line,pattern)==0) {
			start = strrchr(line,'>');
			if (start) {
				printf("%s", start+1);
				print_loop(' ',MAX_NBR_LEN-strlen(start+1));
			}
		} else
			print_loop('-',MAX_NBR_LEN);
	} else if (strstr(line,"tdv-var_an")) {
		if (remove_end(line,"</span></td>")==0) {
			start = strrchr(line,'>');
			if (start) {
				printf("%s", start+1);
				print_loop(' ',MAX_NBR_LEN-strlen(start+1));
			}
		} else
			print_loop('-',MAX_NBR_LEN);
	} else if (strstr(line,"tdv-tot_volume")) {
		char * pattern;
		if (rt)
			pattern = "</span></td>";
		else
			pattern = "</td>";
		if (remove_end(line,pattern)==0) {
			start = strrchr(line,'>');
			if (start) {
				printf("%s", start+1);
				print_loop(' ',MAX_NBR_LEN-strlen(start+1));
			}
		} else
			print_loop('-',MAX_NBR_LEN);
	}
}

int extract_data(char * data)
{
	FILE * fp;
	char * line, *next=data;
	size_t len = 0;
	ssize_t read;
	int number = 1;
	int parse = 0;

	print_loop(' ',2);
	printf("Libellé");print_loop(' ',MAX_NAME_LEN-strlen("Libellé"));
	print_loop(' ',5);
	printf("Der");print_loop(' ',MAX_NBR_LEN-strlen("Der"));
	printf("Var");print_loop(' ',MAX_NBR_LEN-strlen("Var"));
	printf("Ouv");print_loop(' ',MAX_NBR_LEN-strlen("Ouv"));
	printf("max");print_loop(' ',MAX_NBR_LEN-strlen("max"));
	printf("min");print_loop(' ',MAX_NBR_LEN-strlen("min"));
	printf("1janv");print_loop(' ',MAX_NBR_LEN-strlen("1janv"));
	printf("Vol");print_loop(' ',MAX_NBR_LEN-strlen("Vol"));
	printf("\n");

	while (1) {
		line = next;
		next = strchr(line, '\n');
		if (next) {
			*next = 0;
			next++;
			parse_line(line);
		} else
			break;
	}
	return 0;
}

char * search_data_start(char * raw)
{
	return strstr(raw,"<tbody>");
}

char * search_data_end(char * raw)
{
	return strstr(raw,"</tbody>");
}


#define MARKET_ALL 0
#define MARKET_CAC40 1
#define MARKET_CACPME 2
#define MARKET_SBF 3

void Usage()
{
	fprintf(stderr, "Usage: extract_from_line_to_line [opt] <file>\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "\t -m to select a market (cac40, cacpme, sbf)\n");
	fprintf(stderr, "\t -v to select a value\n");
	fprintf(stderr, "\t -c to colorize\n");
	exit(1);
}

#define URL "http://www.boursorama.com/bourse/actions/cours_az.phtml"

int main (int argc, char **argv)
{
	int opt, market = MARKET_ALL, daemon=0;
	char * value;
	char * url = URL;

	while ((opt = getopt(argc, argv, "cdm:v:")) != -1) {
		switch (opt) {
		case 'c':
			color = 1;
			break;
		case 'd':
			daemon = 1;
			break;
		case 'm':
			if (strcmp(optarg,"cac40")==0) {
				market = MARKET_CAC40;
				url = URL"?MARCHE=1rPCAC";
			} else if (strcmp(optarg,"cacpme")==0) {
				market = MARKET_CACPME;
				url = URL"?MARCHE=1rPCAPME";
			}else if (strcmp(optarg,"SBF")==0) {
				market = MARKET_SBF;
				url = URL"?MARCHE=1rPPX4";
			}
			break;
		case 'v':
			fprintf(stderr, "Not implemented\n");
			break;
		default:
			Usage();
		}
	}

	if (daemon) {
		fprintf(stderr, "Not implemented\n");
		return 0;
	} else {
		struct raw_url_content * ruc;

		ruc = ruc_new(url);
		if (ruc) {
			char * dstart, *dend;
			ruc_download(ruc);
			dstart = search_data_start(ruc->content);
			dend = search_data_end(dstart);
			/*replace dend by 0 to stop search to this scope*/
			*dend = 0;
			extract_data(dstart);
			ruc_free(ruc);
		}
		return 0;
	}
}
