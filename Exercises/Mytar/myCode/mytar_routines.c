#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytar.h"

extern char *use;

/** Copy nBytes bytes from the origin file to the destination file.
 *
 * origin: pointer to the FILE descriptor associated with the origin file
 * destination:  pointer to the FILE descriptor associated with the destination file
 * nBytes: number of bytes to copy
 *
 * Returns the number of bytes actually copied or -1 if an error occured.
 */
int
copynFile(FILE * origin, FILE * destination, int nBytes)
{
	char* buffer = malloc(nBytes);
	/* Read file byte by byte */
	int sizeRead = fread(buffer, sizeof buffer[0], nBytes, origin);
	int sizeWritten = fwrite(buffer, sizeof buffer[0], sizeRead, destination);

	free(buffer);

	return sizeRead == sizeWritten ? sizeRead : -1;
}

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor 
 * 
 * The loadstr() function must allocate memory from the heap to store 
 * the contents of the string read from the FILE. 
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc()) 
 * 
 * Returns: !=NULL if success, NULL if error
 */
char*
loadstr(FILE * file)
{
	char* f = malloc(INT_MAX);
	fread(f, sizeof(char), INT_MAX, file);
	// Complete the function
	return f;
}

/** Read tarball header and store it in memory.
 *
 * tarFile: pointer to the tarball's FILE descriptor 
 * nFiles: output parameter. Used to return the number 
 * of files stored in the tarball archive (first 4 bytes of the header).
 *
 * On success it returns the starting memory address of an array that stores 
 * the (name,size) pairs read from the tar file. Upon failure, the function returns NULL.
 */
stHeaderEntry*
readHeader(FILE * tarFile, int *nFiles)
{
	fread(nFiles, sizeof(int), 1, tarFile);
	int nF = *nFiles;
	stHeaderEntry* entries = malloc(sizeof(stHeaderEntry) * nF);
	for(int i = 0; i < nF; i++) entries[i].name = (char *)malloc(120);

	for(int i = 0; i < nF; i++){
		char c; int index = 0;
		while ((c = fgetc(tarFile)) != '\0' && c != EOF) {
            entries[i].name[index++] = c;
        }
		entries[i].name[index] = '\0';
		fread(&entries[i].size, sizeof(int), 1, tarFile);
	}

	return entries;
}

/** Creates a tarball archive 
 *
 * nfiles: number of files to be stored in the tarball
 * filenames: array with the path names of the files to be included in the tarball
 * tarname: name of the tarball archive
 * 
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First reserve room in the file to store the tarball header.
 * Move the file's position indicator to the data section (skip the header)
 * and dump the contents of the source files (one by one) in the tarball archive. 
 * At the same time, build the representation of the tarball header in memory.
 * Finally, rewind the file's position indicator, write the number of files as well as 
 * the (file name,file size) pairs in the tar archive.
 *
 * Important reminder: to calculate the room needed for the header, a simple sizeof 
 * of stHeaderEntry will not work. Bear in mind that, on disk, file names found in (name,size) 
 * pairs occupy strlen(name)+1 bytes.
 *
 */
int
createTar(int nFiles, char *fileNames[], char tarName[])
{
	stHeaderEntry *entries = malloc(sizeof(stHeaderEntry) * nFiles);;
	int sizeHeader = sizeof(int);	//To write number of files

	FILE* file = fopen(tarName, "w");

    //Calcular tamaño de la cabecera
	for(int i = 0 ; i < nFiles; i++){
		sizeHeader += strlen(fileNames[i]) + 1 + sizeof(entries[i].size);
	}

	// desplazarse al final de la cabecera
	fseek(file, sizeHeader, 0);	

	// por cada fichero
	//    abrir fichero
	//    usar copynfile para copiar su contenido
	//    escribir en la cabecera su tamaño
	for (int i = 0; i < nFiles; ++i) {
		FILE* origin = fopen(fileNames[i],"r");
		int s = copynFile(origin, file, INT_MAX);
		if(s != -1) entries[i].size = s;
		else return EXIT_FAILURE;
	}

	// volver al comienzo y escribir la cabecera
	fseek(file, 0, 0);	
	fwrite(&nFiles, sizeof nFiles, 1, file);
	for (int i = 0; i < nFiles; i++) {
		// Escribir nombre
		fwrite(fileNames[i], sizeof fileNames[i][0], strlen(fileNames[i]) + 1, file);
		// Escribir size
		fwrite(&entries[i].size, sizeof entries[i].size, 1, file);
	}

	free(entries);
	fclose(file);

	return EXIT_SUCCESS;
}

/** Extract files stored in a tarball archive
 *
 * tarName: tarball's pathname
 *
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First load the tarball's header into memory.
 * After reading the header, the file position indicator will be located at the 
 * tarball's data section. By using information from the 
 * header --number of files and (file name, file size) pairs--, extract files 
 * stored in the data section of the tarball.
 *
 */
int
extractTar(char tarName[])
{
	FILE* file = fopen(tarName, "r");
	int nFiles = 0;
	stHeaderEntry* entries = readHeader(file, &nFiles);
	char* buffer = loadstr(file);
	fclose(file);

	int index = 0;
	for(int i = 0; i < nFiles; i++){
		FILE* file = fopen(entries[i].name, "w");
		fwrite(&buffer[index], sizeof(char), entries[i].size, file);
		index += entries[i].size;
		free(entries[i].name);
		fclose(file);
	}
	free(entries);
	free(buffer);
	
	// Complete the function
	return EXIT_SUCCESS;
}