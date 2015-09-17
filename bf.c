/*
 * BrainFuck interpreter
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STK 512   /* Maximum size for stack  */
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

inline int evaluate(char *prog, long int plen) {
	unsigned char data[MAX_LEN]; /* Data */
	unsigned short stack[MAX_STK]; /* Stack */
	int sp = -1, dc = 0, pc = 0; /* Stack ptr, data and program counters */
	int bal, rc = 0; /* Paren. balances, Return count */
	/* Zero out the data array */
	memset(data, 0, MAX_LEN * sizeof(char));
	while (prog[pc] != '\0' && pc < plen) {
		switch (prog[pc]) {
			/* Increment data pointer */
			case '>':
				/* If we exceed maximum data length */
				if (dc + 1 >= MAX_LEN) {
					fprintf(stderr, "Data ptr overflow\n");
					rc = -1;
					goto out;
				}
				++dc;
				break;
			/* Decrement data pointer */
			case '<':
				/* If we went below 0 */
				if(dc - 1 < 0) {
					fprintf(stderr,"Data ptr underflow\n");
					rc = -2;
					goto out;
				}
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
				/*
				 * If data pointer is zero
				 * skip past the matching ]
				 * and go out of the loop
				 */
				if (data[dc] == '\0') {
					bal = 1;
					while (bal > 0) {
						++pc;
						if (pc >= plen) {
							fprintf(stderr, "Unbalanced parenthesis\n");
							rc = -3;
							goto out;
						} else if (prog[pc] == '[')
							++bal;
						else if (prog[pc] == ']')
							--bal;
					}
				} else {
					/* Push program ptr onto the stack */
					if (sp + 1 >= MAX_STK) {
						fprintf(stderr, "Stack Overflow\n");
						rc = -4;
						goto out;
					}
					stack[++sp] = pc;
				}
				break;
			/*
			 * Go back to the saved program
			 * counter (pointing to matching ])
			 * present on top of stack.
			 * Also pop the stack
			 * */
			case ']':
				/* Check if stack is empty, error out */
				if (sp == -1) {
					fprintf(stderr, "Stack Underflow\n");
					rc = -5;
					goto out;
				}
				pc = stack[sp--];
				continue;
		}
		++pc;
	}
	/* Check for program counter overflow */
	if (prog[pc] != '\0') {
		fprintf(stderr, "Program counter overflow\n");
		rc = -6;
		goto out;
	}
	/* Check for stack not empty */
	if (sp != -1) {
		fprintf(stderr, "Unmatched left parenthesis\n");
		rc = -7;
		goto out;
	}
out:
	return rc;
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
	rc = evaluate(buf, plen);
	/* Free allocated buffer */
	free(buf);
	return rc;
}
