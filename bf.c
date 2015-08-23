/* BrainFuck interpreter
 *
 * NOTE: Cannot handle nested loops (notice I'm not using a stack here)
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 30000 /* Maximum length for data */

/* Read the file and give you the string buffer */
inline char *read_file(char *path, long int* read_len) {
	FILE *fp;
	char *contents;
	long int filesz;
	/* Set read_len to zero initially */
	*read_len = 0;
	/* Open the file */
	fp = fopen(path, "rb");
	if (!fp) {
		fprintf(stderr, "Cannot open file: %s, error: %d\n",
			path, errno);
		return NULL;
	}
	/* Go to the end of the file */
	if (fseek(fp, 0, SEEK_END)) {
		fprintf(stderr, "Failed to seek end of file, error: %d\n",
			errno);
		fclose(fp);
		return NULL;
	}
	/* Get the current location (in our case, the file size) */
	if ((filesz = ftell(fp)) < 0) {
		fprintf(stderr, "Failed to get the file size, error: %d\n",
			errno);
		fclose(fp);
		return NULL;
	}
	/* Come back to the beginning of the file */
	rewind(fp);
	/* Allocate memory for file contents */
	contents= malloc((filesz + 1) * sizeof(char));
	if (!contents) {
		fprintf(stderr, "Failed to allocate memory, error: %d\n",
			errno);
		fclose(fp);
		return NULL;
	}
	/* Read the file */
	if (fread(contents, sizeof(char), filesz, fp) != filesz) {
		fprintf(stderr, "Failed to read the file, error: %d\n",
			errno);
		free(contents);
		fclose(fp);
		return NULL;
	}
	/* Null out the last character (after string) */
	contents[filesz] = '\0';
	/* Close the file */
	fclose(fp);
	/* Populate the file size */
	*read_len = filesz;
	return contents;
}

inline void evaluate(char *prog, long int plen) {
	char data[MAX_LEN]; /* Data */
	long int bal, dc = 0, pc = 0; /* Current data and program counters */
	/* Zero out the data array */
	memset(data, 0, MAX_LEN * sizeof(char));
	while (prog[pc] != '\0') {
		switch (prog[pc]) {
			/* Increment data pointer */
			case '>':
				++dc;
				break;
			/* Decrement data pointer */
			case '<':
				--dc;
				break;
			/* Increment value at data pointer */
			case '+':
				++data[dc];
				break;
			/* Decrement value at data pointer */
			case '-':
				--data[dc];
				break;
			/* Print character at data pointer onto screen */
			case '.':
				putchar(data[dc]);
				break;
			/* Read from input into data pointer */
			case ',':
				data[dc] = getchar();
				break;
			/* Jump past ] if data pointer is zero */
			case '[':
				if (data[dc] == '\0') {
					bal = 1;
					while (bal != 0) {
						++pc;
						if (prog[pc] == '[')
							++bal;
						else if (prog[pc] == ']')
							--bal;
					}
				}
				break;
			/* Jump past [ if data pointer is non-zero */
			case ']':
				if (data[dc] != '\0') {
					bal = -1;
					while (bal != 0) {
						--pc;
						if (prog[pc] == '[')
							++bal;
						else if (prog[pc] == ']')
							--bal;
					}
				}
				break;
		}
		++pc;
	}
}

int main(int argc, char *argv[]) {
	int rc = 0;
	long int plen;
	char *buf = NULL;
	/* Sanity check on arguments */
	if (argc != 2) {
		fprintf(stderr, "Arguments found: %d, required: %d\n", argc, 2);
		return 1;
	}
	/* Read the file into buf */
	buf = read_file(argv[1], &plen);
	if (!buf) {
		/* Error already printed */
		return 2;
	}
	/* Interpret the program read */
	evaluate(buf, plen);
	/* Free allocated buffer */
	free(buf);
	return 0;
}
