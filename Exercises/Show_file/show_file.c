#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>

int main(int argc, char* argv[]) {
	FILE* file=NULL;
	int ret_code,block;

	if (argc != 3) {
		fprintf(stderr,"Usage: %s <file_name> <block_size>\n",argv[0]);
		exit(1);
	}

	block = atoi(argv[2]);

	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);

	char buffer[block];
	/* Read file byte by byte */
	ret_code = fread(buffer, sizeof buffer[0], block, file);

	if (ret_code == block) {
		/* Print chars to stdout */
		fwrite(buffer, sizeof buffer[0], block, stdout);
	} else {
		if (feof(file))
			printf("Error reading test.bin: unexpected end of file\n");
		else if (ferror(file)) {
			perror("Error reading test.bin\n");
		}
	}

	fclose(file);
	return 0;
}
