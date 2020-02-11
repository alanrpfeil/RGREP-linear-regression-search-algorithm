#include <stdio.h>
#include <string.h>
#define MAXSIZE 4096

/**
 * You can use this recommended helper function 
 * Returns true if partial_line matches pattern, starting from
 * the first char of partial_line.
 */
int matches_leading(char *partial_line, char *pattern) {
  // Implement if desire 

  return 0;
}

/**
 * You may assume that all strings are properly null terminated 
 * and will not overrun the buffer set by MAXSIZE 
 *
 * Implementation of the rgrep matcher function
 */
int rgrep_matches(char *line, char *pattern) {
	int match = 0;
	int patlen = strlen(pattern);
	int i, k, j, repnum;
	char repchar = '\0';
	int repline = 0;
	int repcount = 0;
	int offset = 0;
	char backslash = 92;
	for (i=0; i<strlen(line);i++){
		if (pattern[0] == line[i] || pattern[0] == '.' || pattern[1] == '?' || (pattern[0] == backslash && pattern[2] == '?')){
			j = i;			
			while (j - i < patlen) {
				if (line[j-offset] == '\0'){     //checking if empty
					match = 0;
					break;
				}
				//-------------------------------CHECKING FOR '.' -------------------------------------------//
				
				if (pattern[j-i] == '.' && (pattern[j-i-1] != backslash || pattern[j-i-2] == backslash)) { //checked if next char is a wildcard
					if (pattern[j-i+1] == '+'){ //checks if next char is '+' * next next is '/'
						repchar = line[j-offset];
						repline = j - offset;
							while (line[repline] == repchar) {
								repline++;
								repcount++;
							}
						offset = offset - repcount + 2;
						j = j+2;
						repline = 0;
						repcount = 0;
						continue;
					}
					if (pattern[j-i+1] == '?'){
						if (pattern[j-i+2] != line[j+1-offset] && pattern[j-i+2] != line[j-offset]){
							match = 0;
							break;
						}
						if (pattern[j-i+2] == line[j-offset]){
						j++;
						offset++;
						}
					j = j+2;
					offset++;
					continue;
					}
					if (pattern[j-i-2] != backslash && j-i-2 >= 0 && pattern[j-i-1] == '.' && pattern[0] != 'w'){ //'w' needs fixed
						if ((patlen+1) != strlen(line)){
							match = 0;
							break;
						}
					}
				j++;
				continue;
				}
				
				//-------------------------------CHECKING FOR '+' -------------------------------------------//
				
				if (pattern[j-i+1] == '+' && (pattern[j-i] != backslash || pattern[j-i-1] == backslash)) {
					if (pattern[j-i] == line[j-offset]){
						if (pattern[j-i] != line[j+1-offset]){ 				   // THIS IS
							j = j+2;					           // IF 'a+'
							offset++;						   // MATCHES for
							continue;						   // A and AA+
						}
					}
					repnum = j-i;
					repline = j - offset;
					while (pattern[repnum] == line[repline]) {
						repline++;
						repcount++;
					}
				if (pattern[j-i+2] == pattern[j-i]){
					j++;
					offset++;
				}
				offset = offset - repcount + 2;
				j = j+2;
				repline = 0;
				repcount = 0;
				repnum = 0;
				}
				
				//-------------------------------CHECKING FOR '?' -------------------------------------------//
				if (pattern[j-i+1] == '?' && (pattern[j-i] != backslash || pattern[j-i-1] == backslash)) {
					if (pattern[j-i+2] == line[j-offset]){
						if (strlen(line) == 2){
						match = 1;			    //
						j = patlen;			    //This makes it always print out as a match under these conditions
						offset = patlen;		    //
						break;
						}
						j = strlen(pattern);
						offset++;
						continue;
					}
					if (pattern[j-i] == backslash && pattern[j-i] != line[j-offset]){
						if (pattern[j-i+2] != line[j-offset]){
							match = 1;
							j = patlen;
							break;
						}
					}
					if (pattern[j-i+2] == line[j-offset+1] && pattern[j-i] != line[j-offset] && line[j-offset] != '\n'){
						match = 0;
						break;
					}
				j = j+2;
				offset++;
				continue;
				}
				
				//-------------------------------CHECKING FOR '\'-------------------------------------------//
				
				if(pattern[j-i] == backslash && pattern[j-i-1] != backslash){
					if (pattern[j-i+1] == backslash){
						j++;
						offset++;
						continue;
					}
					j++;
					offset++;
					continue;
				}
				
				//-----------------------------------------------------------------------------------------//
				
				if (pattern[j-i] != line[j-offset]) {
					match = 0;
					offset = 0;
					repline = 0;
					repcount = 0;
					break;
				}
			j++;			
			}
			if (j-i == patlen && patlen <= (strlen(line) + offset)){
				match = 1;
				break;
			} 
		}
   	 match = 0;
	}
    //printf("i = %d, j = %d, patlen = %d, strlen(line) = %zu, offset = %d\n",i,j,patlen,strlen(line),offset);
    //printf("line at: %c pattern at: %c\n",line[j-offset],pattern[j-i]);
    if (match == 1){
		for (k=0;k<strlen(line);k++){
			printf("%c",line[k]);
		}
    }
	return 0;
}

int main(int argc, char **argv) {
	
	if (argc != 2) {
        fprintf(stderr, "Usage: %s <PATTERN>\n", argv[0]);
        return 2;
    }

    /* we're not going to worry about long lines */
    char buf[MAXSIZE];

    while (!feof(stdin) && !ferror(stdin)) {
        if (!fgets(buf, sizeof(buf), stdin)) {
            break;
        }
        if (rgrep_matches(buf, argv[1])) {
            fputs(buf, stdout);
            fflush(stdout);
        }
    }

    if (ferror(stdin)) {
        perror(argv[0]);
        return 1;
    }

    return 0;
}
