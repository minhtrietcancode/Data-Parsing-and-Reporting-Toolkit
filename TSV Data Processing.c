#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <assert.h>

/*******************************************************************
                            DEFINE'S
*******************************************************************/

#define CHAR_CR '\r'
#define CHAR_NL '\n'
#define CHAR_TB '\t'

#define STATUS_EOL 1
#define STATUS_EOF 2
#define STATUS_NML 3

#define MAXFIELDLEN 50
#define MAX_ROWS 1000
#define MAX_COLUMNS 30

#define LEVEL_INDENT 4

// Data structure to get the record in the tsv file into a 2D array
typedef char field_t[MAXFIELDLEN+1];

// Data structure for a 2D array and its row 
typedef field_t row_t[MAX_COLUMNS];
typedef row_t tsv_data_t[MAX_ROWS];

/*******************************************************************
                        FUNCTION PROTOTYPES
*******************************************************************/
/* Stage 1 */
void read_tsv_data_into_array(tsv_data_t data, int *total_rows, 
                              int *total_columns);
void do_stage_1(tsv_data_t data, int *total_rows, int *total_columns);

/* Stage 2 */
void sort_data_by_columns(tsv_data_t data, int total_rows, int total_columns, 
                          int sort_columns_index[], int num_sort_columns);  
int compare_rows_by_sort_keys(tsv_data_t data, int row1, int row2, 
                              int sort_columns_index[], int num_sort_columns);  
void swap_rows(tsv_data_t data, int row1, int row2);
void print_rows(int row, int total_columns, tsv_data_t data);
void print_sort_column_header(row_t headers, int sort_columns_index[], 
                              int num_sort_columns);  
void do_stage_2(int total_rows, int total_columns, tsv_data_t data, 
                int sort_columns_index[], int num_sort_columns, row_t headers);

/* Stage 3 */
int calculate_print_line_length(int total_rows, int total_columns, 
                                tsv_data_t data, int sort_columns_index[], 
                                int num_sort_columns);  
int calculate_aligning_spaces(int total_length, int num_sort_columns, 
                              tsv_data_t data, int sort_columns_index[], 
                              int row);
void print_dashlines(int total_length);
void print_stage_3_header(int num_sort_columns, row_t headers, tsv_data_t data, 
                          int sort_columns_index[], int total_length); 
void print_stage_3_report(int total_rows, int total_columns, tsv_data_t data, 
                          int sort_columns_index[], int num_sort_columns,   
                          int total_length);
void do_stage_3(int total_rows, int total_columns, tsv_data_t data, 
                row_t headers, int sort_columns_index[], int num_sort_columns);  

/* Helper functions */
int mygetchar(void);
int getfield(field_t dest);

/* ta daa function */
void ta_daa(void);

/*******************************************************************
                    MAIN FUNCTION FOR TESTING
*******************************************************************/
/* traffic control for the whole 3 stages */
int 
main(int argc, char *argv[]) {
    /* setting up an 2D array to store data from tsv file*/
    tsv_data_t data;
    
    /* setting up two integers to store the number of rows and columns */
    int total_rows = 0, total_columns = 0;
    
    /* store the data from tsv file into the 2D array */
    read_tsv_data_into_array(data, &total_rows, &total_columns);
    
    /* STAGE 1 RESULT */
    do_stage_1(data, &total_rows, &total_columns);
    
    /* If no argument found, exit immmediately*/
    if (argc < 2) {
        ta_daa();
        exit(0);
    }
    
    /* Create an array to store the header row (first row of the 2D array) */
    row_t headers;
    for (int i = 0; i < total_columns; i++) {
        strcpy(headers[i], data[0][i]);
    }
    
    /* Create an array to store all the integers from the argument */
    int num_sort_columns =   argc - 1;
    int sort_columns_index[num_sort_columns];  
    for (int i = 0; i < num_sort_columns;   i++) {
        sort_columns_index[i] = atoi(argv[i + 1]) - 1;
    }
    
    /* Sort the 2D array based on the given argument */
    sort_data_by_columns(data, total_rows, total_columns, sort_columns_index, 
                         num_sort_columns);  
    
    /* STAGE 2 RESULT */
    do_stage_2(total_rows, total_columns, data, sort_columns_index, 
               num_sort_columns, headers);
    
    /* STAGE 3 RESULT */
    do_stage_3(total_rows, total_columns, data, headers, sort_columns_index, 
               num_sort_columns);  
    
    /* ALL DONE TA DAA */
    ta_daa();
    /* algorithms are fun */
    return 0;
}

/*******************************************************************
                            STAGE 1
*******************************************************************/
/* function for stage 1 to read thorughout the tsv data file and store the 
   records into a 2D array, and also count the number of rows and columns in 
   the file
*/
void read_tsv_data_into_array(tsv_data_t data, int *total_rows, 
                              int *total_columns) 
{
    field_t field;
    int status; 
    int row = 0, col = 0;

    while ((status = getfield(field)) != STATUS_EOF) {
        strcpy(data[row][col], field);
        /* count the column number based on the time end of a field */
        col++;

        if (status == STATUS_EOL) {
            /* count the row number based on the time end of a line */
            row++;
            *total_columns = col;
            col = 0;
        }
    }

    *total_rows = row;
}

/* the required stage 1 output:
   -- a summary of the numbers of rows and columns in the tsv data file
   -- print out the record of the last row of the file, also with the header 
      correspond to that record
*/
void do_stage_1(tsv_data_t data, 
                int *total_rows, int *total_columns) {
    printf("Stage 1\n");
    printf("input tsv data has %d rows and %d columns\n", 
           *total_rows-1, *total_columns);
    /* print out the last row records and the header correspond to it */
    printf("row %d is:\n", *total_rows-1);
    for (int i = 0; i < *total_columns; i++) {
        printf("%4d: %-11s%s\n", i+1, data[0][i], data[*total_rows-1][i]);
    }
    printf("\n");
}

/*******************************************************************
                            STAGE 2
*******************************************************************/
/* function for stage 2 to sort an array based on some certain index numbers
   given by the argument command line
*/
void sort_data_by_columns(tsv_data_t data, int total_rows, int total_columns, 
                          int sort_columns_index[], int num_sort_columns) 
{
    for (int i = 1; i < total_rows; i++) {
        for (int j = i; j > 1 && 
             compare_rows_by_sort_keys(data, j-1, j, 
                                       sort_columns_index, 
                                       num_sort_columns) > 0; 
             j--) {
            swap_rows(data, j-1, j);
        }
    }
}

/* helper function for the sort_data_by_columns function of stage 2, given an 
   array of integers given by the argument command line as comparing key, 
   return 0 if the two records do not need to be swapped, and a non-zero 
   integer otherwise
*/
int compare_rows_by_sort_keys(tsv_data_t data, int row1, int row2, 
                              int sort_columns_index[], int num_sort_columns) 
{
    for (int i = 0; i < num_sort_columns; i++) {
        int col = sort_columns_index[i];
        int cmp = strcmp(data[row1][col], data[row2][col]);
        if (cmp != 0) {
            return cmp;
        }
    }
    return 0;
}

/* helper function for the sort_data_by_columns function of stage 2 to swap 
   two rows, which are 2 arrays in the 2D array
*/
void swap_rows(tsv_data_t data, int row1, int row2) {
    for (int i = 0; i < MAX_COLUMNS; i++) {
        field_t temp;
        strcpy(temp, data[row1][i]);
        strcpy(data[row1][i], data[row2][i]);
        strcpy(data[row2][i], temp);
    }
}

/* helper function for stage 2 to print out the record of a row and the header 
   correspond to that record, ensure consistent formatting for the output
*/
void print_rows(int row, int total_columns, tsv_data_t data) {
    printf("row %d is:\n", row);
    for (int i = 0; i < total_columns; i++) {
        printf("%4d: %-11s%s\n", i+1, data[0][i], data[row][i]);
    }
}

/* helper function for stage 2 to print out which headers of the header row 
   (first row) are being regarded as sorting keys, based on the integers list 
   given by the argument command line
*/
void print_sort_column_header(row_t headers, int sort_columns_index[], 
                              int num_sort_columns) 
{
    printf("sorting by \"%s\"", headers[sort_columns_index[0]]);
    for (int i = 1; i < num_sort_columns; i++) {
        printf(",\n   then by \"%s\"", headers[sort_columns_index[i]]);
    }
    printf("\n");
}

/* the required stage 2 output:
   -- printing the headers that has been regarded as sorting keys
   -- print the record and the header correspond to that record of the first, 
      middle, and last rows
*/
void do_stage_2(int total_rows, int total_columns, tsv_data_t data, 
                int sort_columns_index[], int num_sort_columns, row_t headers)
{
    /* Get the index of the mid row in each cases */
    int mid_row;
    if((total_rows - 1) % 2 == 0) {
        mid_row = (total_rows -1)/2;
    } else {
        mid_row = total_rows/2;
    }
    
    printf("Stage 2\n");
    
    /* Print the name of headers that are taken as the key for sorting */ 
    print_sort_column_header(headers, sort_columns_index, num_sort_columns);  
    
    /* Print the first, middle and last row of the sorted array */
    print_rows(1, total_columns, data);
    print_rows(mid_row, total_columns, data);
    print_rows(total_rows-1, total_columns, data);
    
    /* End stage with an empty line */
    printf("\n");
}

/*******************************************************************
                            STAGE 3
*******************************************************************/
/* helper function for stage 3 to calculate the total length of a printed line,
   including white spaces
   calculated by determining the longest entry (including white space) and then 
   generate the total length that is equal to the longest entry + 6 (one space 
   and 5 (the length of "Count"))
*/
int calculate_print_line_length(int total_rows, int total_columns, 
                                tsv_data_t data, int sort_columns_index[], 
                                int num_sort_columns) 
{
    int longest_entry_length = 0;
    /* regarding the records in all the columns that are specified by the 
       argument command line of every single rows in the 2D array */
    for (int i = 0; i < num_sort_columns;   i++) {
        for (int j = 0; j < total_rows; j++) {
            /* calculating the current entry's length by the addition of 
            white space number at its level and its length */
            int current_string_length = strlen(data[j][sort_columns_index[i]]);
            int current_length = i*LEVEL_INDENT + current_string_length;
            if (current_length > longest_entry_length) {
                longest_entry_length = current_length;
            }
        }
    }
    
    /* Total length equals to the length of longest entry + one space  
       + 5 (length of word "Count") */
    return longest_entry_length + 6;
}

/* helper function for stage 3 to calculate the aligning space for the last 
   column ("Count" and the number of counting) 
*/
int calculate_aligning_spaces(int total_length, int num_sort_columns, 
                              tsv_data_t data, int sort_columns_index[], 
                              int row) 
{
    int indent = (num_sort_columns - 1) * LEVEL_INDENT;
    int last_col_index = sort_columns_index[num_sort_columns - 1];
    int last_col_length = strlen(data[row][last_col_index]);
    /* if print "Count" */
    if (row == 0) {
        return total_length - last_col_length - 5 - indent;
    }

    /* if print the counting numbers */
    return total_length - last_col_length - indent;
}

/* helper function for stage 3 to print dashlines based on the total length of 
   a printed line
*/
void print_dashlines(int total_length) {
    for (int i = 0; i < total_length; i++) {
        printf("-");
    }
    printf("\n");
}

/* helper function for stage 3 to print out the header of the selected columns
   based on the integers given by the argument command line and the total 
   lemgth of a printed line
*/
void print_stage_3_header(int num_sort_columns, row_t headers, tsv_data_t data, 
                          int sort_columns_index[], int total_length) 
{
    for (int i = 0; i < num_sort_columns;   i++) {
        /* for each new level of column, move to the right 4 spaces */
        printf("%*s%s", i * LEVEL_INDENT, "", headers[sort_columns_index[i]]);
        if (i < num_sort_columns - 1) {
            printf("\n");
        }
    }
    /* calculate space to align the word "Count" */
    int spaces = calculate_aligning_spaces(total_length, num_sort_columns, 
                                           data, sort_columns_index, 0);
    
    /* print "Count" with the aligned space */
    printf("%*sCount\n", spaces, "");
}

/* helper function to print stage 3 report regarding the selected columns
   it counts how many consecutive rows share the same values across specified 
   sorting columns and displays the results in a neatly formatted report.
*/
void print_stage_3_report(int total_rows, int total_columns, tsv_data_t data, 
                          int sort_columns_index[], int num_sort_columns,   
                          int total_length)
{
    /* initialize count for consecutive matching rows */
    int count = 1;
    
    /* initializes an array to keep track of the previous row values in the 
       sorted columns */
    char prev_values[num_sort_columns][MAXFIELDLEN + 1];
    /* set initial values to empty strings */
    for (int i = 0; i < num_sort_columns;   i++) {
        prev_values[i][0] = '\0';
    }
    
    /* loop through each row of the dataset, starting from the second row */
    for (int i = 1; i < total_rows; i++) {
        /* initialize diff_level to the number of sorting columns */
        int diff_level = num_sort_columns;
          /* loop through the sorting columns to find where the current row 
           differs from the previous row */
        for (int j = 0; j < num_sort_columns;   j++) {
            /* compare current row value with the previous row's value in the 
               sort column */
            if (strcmp(data[i][sort_columns_index[j]], prev_values[j]) != 0) {
                /* set diff_level to the index of the first different column 
                and stop checking when a difference is found */
                diff_level = j;
                break;
            }
        }
        
        /* if the current row differs from the previous row */
        if (i > 1 && diff_level < num_sort_columns) {
            /* calculating the number of spaces for alignment */
            int spaces = calculate_aligning_spaces(total_length, 
                                                   num_sort_columns, data, 
                                                   sort_columns_index, i - 1);
            printf("%*d\n", spaces, count);
            /* reset count for the new group */
            count = 1;
        }

    
        /* print the values for the current row starting from the first 
           differing column */
        for (int j = diff_level; j < num_sort_columns;   j++) {
            /* print the value with indentation */
            printf("%*s%s", j * LEVEL_INDENT, "", 
                   data[i][sort_columns_index[j]]);
            if (j < num_sort_columns -   1) {
                printf("\n");
            }
            /* update previous values with the current row's values */
            strcpy(prev_values[j], data[i][sort_columns_index[j]]);
        }
        
        /* if the row matches the previous row, increment the count */
        if (diff_level == num_sort_columns)   {
            count++;
        }
    }

    /* calculating spaces for alignment */
    int spaces = calculate_aligning_spaces(total_length, num_sort_columns, 
                                           data, sort_columns_index, 
                                           total_rows - 1);
    printf("%*d\n", spaces, count);
}

/* the required stage 3 output:
   -- dashlines to separate the header and the report of the output
   -- the header of the selected columns 
   -- the report based on the selected columns
*/
void do_stage_3(int total_rows, int total_columns, tsv_data_t data, 
                row_t headers, int sort_columns_index[], int num_sort_columns)
{
    printf("Stage 3\n");
    /* get the total length of a printed line */
    int total_length = calculate_print_line_length(total_rows, total_columns, 
                                                   data, sort_columns_index, 
                                                   num_sort_columns);  
    /* traffic control for the stage 3 output */
    print_dashlines(total_length);
    print_stage_3_header(num_sort_columns, headers, data, sort_columns_index, 
                         total_length);
    print_dashlines(total_length);
    print_stage_3_report(total_rows, total_columns, data, sort_columns_index, 
                         num_sort_columns, total_length);
    print_dashlines(total_length);
}

/*******************************************************************
                      SOME HELPER FUNCTIONS
*******************************************************************/
/* provided helper function by our lecturer
*/
int mygetchar(void) {
    int c;
    while ((c=getchar()) == CHAR_CR) {
        /* empty loop body */
    }
    return c;
}

/* provided helper function by our lecturer
*/
int getfield(field_t dest) {
    int ch, nchs=0;
    int status=STATUS_NML;

    dest[0] = '\0';
    while ((ch=mygetchar())
        && (ch != CHAR_TB) && (ch != CHAR_NL) && (ch != EOF)) {

        if (nchs < MAXFIELDLEN) {
            dest[nchs++] = ch;
            dest[nchs] = '\0';
        } else {
            /* silently discard extra characters if present */
        }
    }

    if (ch == EOF) {
        status = STATUS_EOF;
    } else if (ch == CHAR_NL) {
        status = STATUS_EOL;
    } else if (ch == CHAR_TB) {
        status = STATUS_NML;
    }
    return status;
}

/* function to say that TA DAA all done yessir
*/
void ta_daa(void) {
    printf("ta daa!\n");
}

/*******************************************************************
                            THE END 
*******************************************************************/
