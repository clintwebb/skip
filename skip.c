/*
	skip
	(c) 2024 Copyright, Clinton Webb.

	Contact:
		Clinton Webb
		dev@rhokz.com

	It is released under GPL v3 or later license.  Details are included in the LICENSE file.
*/

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define VERSION "0.0.4"

#define OPT_N		1
#define OPT_BLANK	2
#define OPT_HEADER	3
#define OPT_BEFORE	4
#define OPT_AFTER	5
#define OPT_ALL		6
#define OPT_PLUS	7

#define BUFLEN (4096*18)

typedef struct __option {
	int type;
	union {
		int lines;
		char *before;
	} data;
} _option;

// A list of options will be established, and it will iterate through the list.
// Once an entry in the list is completed, it will be removed (to reduce the iterations)
_option *gOptions = NULL;
int gOptionsRemaining = 0;

// When it has triggered the 'blank' state, it will ignore all blank lines from that point on.
int gBlank = false;
int gIgnore = 0;


void remove_option() {
	assert(gOptionsRemaining > 0);

	int i;
	for (i=0; i < (gOptionsRemaining-1); i++ ) {
		gOptions[i].type = gOptions[i+1].type;
		gOptions[i].data = gOptions[i+1].data;
	}
	gOptionsRemaining --;
	assert(gOptionsRemaining >= 0);
}

void usage(void) {

	fprintf(stderr,
			"Usage: skip [option [option...]]\n"
			"Options:\n"
			"  <number>          - skip <number> amount of lines.  eg skip 3 (will skipp 3 lines)\n"
			"  blank             - skip all blank lines.\n"
			"  header            - skip all non-blank lines until it encounters a blank line, \n"
			"                      and then outputs all remaining lines.\n"
			"  before \"string\" - Skip all lines before a line beginning with the 'string' is enountered.\n"
			"                      By default output the string line itself.  To strip it choose the '1' option after it.\n"
			"  after \"string\"  - Skip all lines after a line beginning with the 'string' is encountered.\n"
			"                      By default output the string line itself.  To strip it choose the '1' option after it.\n"
			"  all               - Skip everything after this point.  Only thing to stop this is a 'before', but in that\n"
			"                      'all' would be useless anyway.\n"
			"  +<number>         - Do NOT skip <number> of lines.  Basically the opposite of skip, but may be useful.\n"
			"                      Can already do it by specifying a sequence of 0's.. but this will be easier.\n"
			"\n"
			"Examples:\n"
			"# This will output somefile.txt but skip the first line\n"
			"cat somefile.txt | skip\n\n"

			"# This will output somefile.txt but skip the first 3 lines\n"
			"cat somefile.txt | skip 3\n\n"

			"# This will output somefile.txt but will not output any blank lines.\n"
			"cat somefile.txt | skip blank\n\n"

			"# This will output somefile.txt but will ignore all the lines at the start until an empty line is encountered, and then will output the rest.\n"
			"cat somefile.txt | skip header\n\n"

			"# This will output somefile.txt but will ignore all lines until it encounters a line that begins with \"Data:\" (and will output that \"Data:\" line)\n"
			"cat somefile.txt | skip before \"Data:\"\n\n"

			"# This will output somefile.txt but will ignore all lines until it encounters a line that begins with \"Data:\" (and will not output that \"Data:\" line)\n"
			"cat somefile.txt | skip before \"Data:\" 1\n\n"

			"# This will output somefile.txt but will ignore all lines until it encounters a line that begins with \"Data:\",\n"
			"# will not remove the 'Data:' line, but will remove the next line.\n"
			"cat somefile.txt | skip before \"Data:\" 0 1\n\n"

			"# This will output somefile.txt and will output all lines until it encounters a line that begins with \"Data:\".\n"
			"# (Will include the line that begins with 'Data:')\n"
			"cat somefile.txt | skip after \"Data:\"\n\n"

			"# This will output somefile.txt but will ignore all lines until it encounters a line that begins with \"Data:\",\n"
			"# will not remove the 'Data:' line and will output lines until it finds another line named 'End:'\n"
			"cat somefile.txt | skip before \"Data:\" after \"End:\"\n\n"

			"# This will skip all blank lines, skip 3 remaining lines, and then output the rest\n"
			"cat somefile.txt | skip blank 3\n\n"

			"# This will skip the first 3 lines (whether they contain any blank ones or not), and then output the rest except any blank lines\n"
			"cat somefile.txt | skip 3 blank\n\n"

			"# This will skip all lines until it reaches one that matches 'Data:'.  By default it would also output 'Data:' but in this case, it is being informed to skip that line.\n"
			"cat somefile.txt | skip before 'Data:' 1\n\n"

			"# This is a slightly odd one, and can be done by some other tools anyway.. but adding for some convenience.\n"
			"# 'all' will skip everything after that point.  In this example, we are skipping 3 lines, displaying 4 lines, and\n"
			"# then skipping everything after that.\n"
			"cat somefile.txt | skip 3 +4 all\n\n"
			);

	exit(1);
}

int main(int argc, char **argv)
{
	// It is expecting all input to be via stdin, so it will input each line, compare it to the options specified, and output.

	// If no parameter is specified, it will simply skip the first line.
	if (argc <= 1) {
		assert(gOptions == NULL);
		gOptions = calloc(1, sizeof(_option));
		assert(gOptions);
		assert(gOptionsRemaining == 0);

		gOptions[gOptionsRemaining].type = OPT_N;
		gOptions[gOptionsRemaining].data.lines = 1;

		gOptionsRemaining++;
		assert(gOptionsRemaining == 1);
	}
	else {
		assert(gOptions == NULL);
		gOptions = calloc(argc-1, sizeof(_option));
		assert(gOptions);
		assert(gOptionsRemaining == 0);

		// Go through the command-line options to establish the list of options to be processed.
		int i;
		for (i=1; i<argc; i++) {
			assert(gOptionsRemaining < (argc-1));

			if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
				usage();
			}
			else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
				fprintf(stderr, "Version: v%s\n", VERSION);
				fprintf(stderr, "(c) Copyright 2024, Clinton Webb.\n\n");
				fprintf(stderr,
						"License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n"
						"This is free software: you are free to change and redistribute it.\n"
						"There is NO WARRANTY, to the extent permitted by law.\n");
				exit(1);
			}
			else if (strcmp(argv[i], "blank") == 0 ) {
				if (gOptionsRemaining == 0) {
					// if this is the first parameter, immediately just put it in 'blank' mode (dont need to add it to the list).
					gBlank = true;
				}
				else {
					assert(gOptionsRemaining >= 0);
					assert(gOptionsRemaining < (argc-1));

					gOptions[gOptionsRemaining].type = OPT_BLANK;

					gOptionsRemaining++;
					assert(gOptionsRemaining <= (argc-1));
				}
			}
			else if (strcmp(argv[i], "header") == 0 ) {
				assert(gOptionsRemaining >= 0);
				assert(gOptionsRemaining < (argc-1));

				gOptions[gOptionsRemaining].type = OPT_HEADER;

				gOptionsRemaining++;
				assert(gOptionsRemaining <= (argc-1));
			}
			else if (strcmp(argv[i], "before") == 0 ) {
				assert(gOptionsRemaining >= 0);
				assert(gOptionsRemaining < (argc-1));

				// Need to make sure that the next parameter even exists.
				if ( (argc)  <= (i + 1) ) {
					fprintf(stderr, "Missing 'before' parameter.\n\n");
					return(1);
				}

				// we need to also account for the extra parameter, and skip that.
				gOptions[gOptionsRemaining].type = OPT_BEFORE;
				gOptions[gOptionsRemaining].data.before = argv[i+1];
				assert(strlen(gOptions[gOptionsRemaining].data.before) > 0);
				i++;

				gOptionsRemaining++;
				assert(gOptionsRemaining <= (argc-1));
			}
			else if (strcmp(argv[i], "after") == 0 ) {
				assert(gOptionsRemaining >= 0);
				assert(gOptionsRemaining < (argc-1));

				// Need to make sure that the next parameter even exists.
				if ( (argc)  <= (i + 1) ) {
					fprintf(stderr, "Missing 'after' parameter.\n\n");
					return(1);
				}

				// we need to also account for the extra parameter, and skip that.
				gOptions[gOptionsRemaining].type = OPT_AFTER;
				gOptions[gOptionsRemaining].data.before = argv[i+1];
				assert(strlen(gOptions[gOptionsRemaining].data.before) > 0);
				i++;

				gOptionsRemaining++;
				assert(gOptionsRemaining <= (argc-1));
			}
			else if (strcmp(argv[i], "all") == 0 ) {
				assert(gOptionsRemaining >= 0);
				assert(gOptionsRemaining < (argc-1));

				gOptions[gOptionsRemaining].type = OPT_ALL;

				gOptionsRemaining++;
				assert(gOptionsRemaining <= (argc-1));
			}
			else if (argv[i][0] >= '0' && argv[i][0] <= '9') {
				assert(gOptionsRemaining >= 0);
				assert(gOptionsRemaining < (argc-1));

				gOptions[gOptionsRemaining].type = OPT_N;
				gOptions[gOptionsRemaining].data.lines = atoi(argv[i]);

				gOptionsRemaining++;
				assert(gOptionsRemaining <= (argc-1));
			}
			else if (argv[i][0] == '+') {
				assert(gOptionsRemaining >= 0);
				assert(gOptionsRemaining < (argc-1));

				gOptions[gOptionsRemaining].type = OPT_PLUS;
				gOptions[gOptionsRemaining].data.lines = atoi(argv[i]);

				gOptionsRemaining++;
				assert(gOptionsRemaining <= (argc-1));
			}
			else {
				fprintf(stderr, "Unexpected option: '%s'\n\n", argv[i]);
				return(1);
			}
		}
	}

	assert(gOptionsRemaining >= 0);

	// Now we have established the patterns to match, now we process through each line.

	char buffer[BUFLEN];
	int nLen, sLen;
	int skip = false;

	while (fgets(buffer, BUFLEN, stdin) != NULL) {

		skip = false;
		nLen = strlen(buffer);

		while( gOptionsRemaining > 0 && gOptions[0].type == OPT_BLANK) {
			remove_option();
			assert(gOptionsRemaining >= 0);
			gBlank = true;
		}

		if (gBlank == true && nLen <= 1) {
			skip = true;
		}
		else {
			if (gOptionsRemaining > 0) {
				switch (gOptions[0].type) {
					case OPT_N :
						if (gOptions[0].data.lines > 0) { skip = true; }

						assert(gOptions[0].data.lines >= 0);
						gOptions[0].data.lines --;
						if (gOptions[0].data.lines <= 0) {
							// this option is finished... so we can remove it.
							remove_option();
						}
						break;
					case OPT_PLUS :
						if (gOptions[0].data.lines > 0) { 
							skip = false;
							gIgnore = 0;
						}

						assert(gOptions[0].data.lines >= 0);
						gOptions[0].data.lines --;
						if (gOptions[0].data.lines <= 0) {
							// this option is finished... so we can remove it.
							remove_option();
						}
						break;
					case OPT_HEADER:
						skip = true;
						if (nLen == 1) {
							// we found the blank line, so we have removed all the headers.
							remove_option();
						}
						break;
					case OPT_BEFORE:
						assert(gOptions[0].data.before);
						sLen = strlen(gOptions[0].data.before);
						assert(sLen > 0);
						if (strncmp(buffer, gOptions[0].data.before, sLen) == 0) {
							// matched, so we can remove this option.
							remove_option();

							// if 'after' was specified, it will skip everything until it finds another 'before'.
							gIgnore = 0;

							// Since it is very likely that the only time a "before something 1" is used, is when the actual header is not wanted.  So in that case, we check the next entry
							if (gOptionsRemaining > 0) {
								if (gOptions[0].type == OPT_N) {
									if (gOptions[0].data.lines == 0) {
										skip = false;
										remove_option();
									}
									else if (gOptions[0].data.lines == 1) {
										skip = true;
										remove_option();
									}
								}
							}
						}
						else {
							skip = true;
						}
						break;
					case OPT_AFTER:
						assert(gOptions[0].data.before);
						sLen = strlen(gOptions[0].data.before);
						assert(sLen > 0);
						if (strncmp(buffer, gOptions[0].data.before, sLen) == 0) {
							// matched, so we can remove this option (and pretty much all others except begin).
							remove_option();
							gIgnore = 2;

							// Since it is very likely that the only time an "after something 1" is used, is when the actual string is wanted to be output.
							// So in that case, we check the next entry
							if (gOptionsRemaining > 0) {
								if (gOptions[0].type == OPT_N) {
									if (gOptions[0].data.lines == 1) {
										gIgnore = 1;
										remove_option();
									}
								}
							}
							skip = false;
						}
						else {
							skip = false;
						}
						break;
					case OPT_ALL:
						// Skip everything after this point.
						gIgnore = 1;
						break;
				}
			}
		}


		if (skip == false && gIgnore != 1) {
			fputs(buffer, stdout);
			if (gIgnore == 2) { gIgnore = 1; }
		}
	}

	return(0);
}
