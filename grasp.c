#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define LINE_LENGTH 500

int count = 0;
int no_case = 0;
int number_scanned = 0;

void get_temp_line(char *line, char tmp[]);
void process(char *fname, char *pattern);

int main(int argc, char **argv)
{
    int c;
    char *pattern = NULL;

    while ((c = getopt(argc, argv, "ic")) != -1)
    {
        switch (c)
        {
        case 'i':
            no_case = 1;
            break;
        case 'c':
            count = 1;
            break;
        case '?':
            if (optopt == 'c')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint(optopt))
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            return EXIT_FAILURE;
        }
    }

    pattern = argv[optind];
    if (no_case == 1)
    {
        for (char *p = pattern; *p; ++p)
            *p = tolower(*p);
    }

    // process each file
    for (int index = optind + 1; index < argc; index++)
    {
        process(argv[index], pattern);
    }

    // scan count report
    if (count == 1)
        printf("\nTotal lines scanned: %d\n", number_scanned);
    return EXIT_SUCCESS;
}

void process(char *fname, char *pattern)
{
    FILE *file = fopen(fname, "r");
    char line[LINE_LENGTH];

    if (!file)
    {
        fprintf(stderr, "Error opening file: %s\n", fname);
    }
    else
    {
        while (fgets(line, LINE_LENGTH, file))
        {
            char tmp[LINE_LENGTH];
            get_temp_line(line, tmp);
            if (strstr(tmp, pattern) != NULL)
                printf("%s: %s", fname, line);
            number_scanned++;
        }
        fclose(file);
        if (ferror(file))
            fprintf(stderr, "Error reading file: %s.\n", fname);
    }
}

void get_temp_line(char *line, char tmp[])
{
    strncpy(tmp, line, strlen(line));
    if (no_case)
        for (char *p = tmp; *p; ++p)
            *p = tolower(*p);
}