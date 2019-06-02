#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STRMAX 256

void clear_result() {
	FILE *fp = fopen("result.txt", "w");
	fclose(fp);
}

int count_slashes(char word[STRMAX]) {
	int slash = 0;
	
	for(int i = 0; word[i] != '\0'; i++) 
		if (word[i] == '/') slash++;
	
	return slash;
}

int comma_count(char word[STRMAX]) {
	for(int i = 0; word[i] != '\0'; i++) {
		if (word[i] == ',') return 1;
	}

	return 0;
}

int should_ignore(char w[STRMAX], int slash) {
	int size = strlen(w);

	if (w[size-3] == 'P' && w[size-2] == 'N' && w[size-1] == 'M') return 0;
	if (w[size-3] == 'D' && w[size-2] == 'G' && w[size-1] == 'T') return 0;
	if (isdigit(w[size-1])) return 0;
	if (slash < 2) return 1;
	
	return ((w[size-3] == 'P' && w[size-2] == 'N' && w[size-1] == 'T') || (w[0] == '<' || w[0] == '\n'));
}

void parse(char word[STRMAX], char dest[STRMAX]) {
	int slash_count = count_slashes(word);
	if (should_ignore(word, slash_count)) return;
	
	char to_write[STRMAX];
	
	if (slash_count < 2) {
		sscanf(word, "%[^/]", to_write);
		
	} else {
		int first_slash = 0;
		for(int i = 0; word[i] != '\0'; i++) {
			if (word[i] == '/') {
				first_slash = i;
				break;
			}
		}
		sscanf(word+first_slash+1, "%[^/]", to_write);

		if (to_write[0] == '?') {
			sscanf(word, "%[^/]", to_write);
		}
	}

	if (comma_count(to_write)) {
		sscanf(to_write, "%[^,]", to_write);
	}

	if (to_write[0] == ';' || to_write[0] == '&' || to_write[0] == '#') return;
	
	for (int i = 0; to_write[i]!='\0'; i++)
		to_write[i] = tolower(to_write[i]);
	
	FILE *fp = fopen(dest, "a");
	fprintf(fp, "%s\n", to_write);
	fclose(fp);
}

int main() {
	char word[STRMAX];

	char source[STRMAX], dest[STRMAX];

	printf("Source: ");
	scanf(" %s", source);

	printf("\nDestine: ");
	scanf(" %s", dest);

	FILE *fp = fopen(source, "r");
	int c;

	clear_result();

	fscanf(fp, "%*s");

	while ((c = fgetc(fp)) != EOF) {
		fscanf(fp, "%s", word);
		parse(word, dest);
	}
}