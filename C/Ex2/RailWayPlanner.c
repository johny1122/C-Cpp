#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>

#define LENGTH_OF_LINE (1024 + 1)
#define ALLOC_SIZE 10
#define PRICE_WHEN_LENGTH_0 0
//#define DEBUG

/**
 * @brief struct of a single piece and important data to save
 */
typedef struct Piece
{
    int start;
    int end;
    int dist;
    int price;
}Piece;

/**
 * @brief struct which saves the minimum price and the start connector
 * of the piece with minimum price for each length
 */
typedef struct min_price_per_length_data
{
    int min_price;
    int min_start;
}min_price_per_length_data;

/**
 * @brief struct of a single connector and important data to save
 */
typedef struct ConnectorData
{
    int index_of_connector;
    char connector;
    int list_size;
    int max_length_found;
    min_price_per_length_data* list_min_price_per_length_pointer_continue;
    min_price_per_length_data* list_min_price_per_length_pointer_no_continue;
    Piece* list_pointer;
}ConnectorData;


void buildTable();
int findMinPrice();
bool isEmpty(FILE*  file);
void exitProgram(int  exit_type);
void readInput(char*  input_file_Address);
void readFirstLine();
void readSecondLine();
void readThirdLine();
void readFourAndAfterLines();
int comparePrices(int l, int c, int min_price_line);

int length;
int connectors_number_from_file = 0;
int connectors_number = 0;
int line_counter = 1;
int end_connector_chosen = 0;
FILE* output_file_ptr = NULL;
FILE* input_file_ptr = NULL;
char* connectors_line = NULL;
ConnectorData all_data[LENGTH_OF_LINE / 2];
int** table = NULL;


/**
 * @brief check if a file is empty
 * @param file
 * @return true if empty and false if not
 */
bool isEmpty(FILE * const file)
{
    long savedOffset = ftell(file);
    fseek(file, 0, SEEK_END);

    if (ftell(file) == 0)
    {
        return true;
    }

    fseek(file, savedOffset, SEEK_SET);
    return false;
}


/**
 * @brief The function closes all files, free memory and exit the program with EXIT_FAILURE
 */
void exitProgram(int const exit_type)
{
    if (input_file_ptr != NULL)
    {
        if(fclose(input_file_ptr) == EOF)
        {
            exit(EXIT_FAILURE);
        }
    }

    if (output_file_ptr != NULL)
    {
        if(fclose(output_file_ptr) == EOF)
        {
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < connectors_number; i++)
    {
        free(all_data[i].list_pointer);
        free(all_data[i].list_min_price_per_length_pointer_continue);
        free(all_data[i].list_min_price_per_length_pointer_no_continue);
        all_data[i].list_pointer = NULL;
        all_data[i].list_min_price_per_length_pointer_continue = NULL;
        all_data[i].list_min_price_per_length_pointer_no_continue = NULL;
    }

    if (connectors_line != NULL)
    {
        free(connectors_line);
        connectors_line = NULL;
    }

    if (table != NULL)
    {
        for (int j = 0; j < connectors_number; j++)
        {
            free(table[j]);
            table[j] = NULL;
        }
        free(table);
        table = NULL;
    }

    if(exit_type == EXIT_FAILURE)
    {
        exit(EXIT_FAILURE);
    }
    else
    {
        exit(EXIT_SUCCESS);
    }
}


/**
 * @brief reads the first line and checks for errors
 */
void readFirstLine()
{
    char line1[LENGTH_OF_LINE];
    fgets(line1, LENGTH_OF_LINE, input_file_ptr);
    for (int j = 0; j < LENGTH_OF_LINE; j++)
    {
        if (line1[j] == '\0' || line1[j] == '\n')
        {
            break;
        }
        if (isdigit(line1[j]) == 0)
        {
            fprintf(output_file_ptr, "Invalid input in line: %d.", line_counter);
            exitProgram(EXIT_FAILURE);
        }
    }

    if ((sscanf(line1, "%d", &length) != 1) || (length < 0))
    {
        fprintf(output_file_ptr, "Invalid input in line: %d.", line_counter);
        exitProgram(EXIT_FAILURE);
    }

    line_counter++;
}


/**
 * @brief reads the second line and check for errors
 */
void readSecondLine()
{
    char line2[LENGTH_OF_LINE];
    fgets(line2, LENGTH_OF_LINE, input_file_ptr);
    for (int j = 0; j < LENGTH_OF_LINE; j++)
    {
        if (line2[j] == '\0' || line2[j] == '\n')
        {
            break;
        }
        if (isdigit(line2[j]) == 0)
        {
            fprintf(output_file_ptr, "Invalid input in line: %d.", line_counter);
            exitProgram(EXIT_FAILURE);
        }
    }
    if ((sscanf(line2, "%d", &connectors_number_from_file) != 1) || (connectors_number_from_file <= 0))
    {
        fprintf(output_file_ptr, "Invalid input in line: %d.", line_counter);
        exitProgram(EXIT_FAILURE);
    }
    line_counter++;
}


/**
 * @brief reads the third line and checks for errors
 */
void readThirdLine()
{
    connectors_line = (char*) calloc((connectors_number_from_file * 2), sizeof(char));
    if (connectors_line == NULL)
    {
        fprintf(output_file_ptr, "Invalid input in line: %d.", line_counter);
        exitProgram(EXIT_FAILURE);
    }
    fgets(connectors_line, (connectors_number_from_file * 2), input_file_ptr);
#ifdef DEBUG
    printf("third line:%s|\n", connectors_line);
#endif
    int index = 0;
    bool separetor_before = true;
    bool already_exists = false;
    if  ((connectors_line[0] == '\n') || (connectors_line[0] == ','))
    {
        fprintf(output_file_ptr, "Invalid input in line: %d.", line_counter);
        exitProgram(EXIT_FAILURE);
    }
    while (connectors_line[index] != '\n' && connectors_line[index] != '\0')
    {
        if (connectors_line[index] != ',')
        {
            if (!separetor_before)
            {
                fprintf(output_file_ptr, "Invalid input in line: %d.", line_counter);
                exitProgram(EXIT_FAILURE);
            }
            for (int i = 0; i < connectors_number_from_file; i++) // check if connector already exists
            {
                if (connectors_line[index] == all_data[i].connector)
                {
                    already_exists = true;
                    break;
                }
            }
            if (!already_exists)
            {
                all_data[connectors_number].connector = connectors_line[index];
                all_data[connectors_number].index_of_connector = connectors_number;
                all_data[connectors_number].list_size = 0;
                all_data[connectors_number].list_pointer = (Piece*) calloc(ALLOC_SIZE, sizeof(Piece));
                if (all_data[connectors_number].list_pointer == NULL)
                {
                    fprintf(output_file_ptr, "Invalid input in line: %d.", line_counter);
                    exitProgram(EXIT_FAILURE);
                }
                connectors_number++;
            }
            else
            {
                already_exists = false;
            }
            separetor_before = false;
        }
        else
        {
            separetor_before = true;
        }
        index++;
    }
    if (connectors_number == 0)
    {
        fprintf(output_file_ptr, "Invalid input in line: %d.", line_counter);
        exitProgram(EXIT_FAILURE);
    }
    line_counter++;
}


/**
 * @brief add a new piece to the all_data data base
 * @param end_index - index of the end connector in the data base
 * @param start_index - index of the start connector in the data base
 * @param price - price of the piece
 * @param dist - distance of the piece
 */
void addPieceToData(int const end_index, int const start_index, int const price, int const dist)
{
    if ((all_data[end_index].list_size % ALLOC_SIZE) == (ALLOC_SIZE - 1))
    {
        all_data[end_index].list_pointer = (Piece *) realloc(all_data[end_index].list_pointer,
                                                             (ALLOC_SIZE + all_data[end_index].list_size) * sizeof(Piece));
        if (all_data[end_index].list_pointer == NULL)
        {
            fprintf(output_file_ptr, "Invalid input in line: %d.", line_counter);
            exitProgram(EXIT_FAILURE);
        }
    }
    all_data[end_index].list_pointer[all_data[end_index].list_size].start = start_index;
    all_data[end_index].list_pointer[all_data[end_index].list_size].end = end_index;
    all_data[end_index].list_pointer[all_data[end_index].list_size].price = price;
    all_data[end_index].list_pointer[all_data[end_index].list_size].dist = dist;

#ifdef DEBUG
    printf("============================\n");
    printf("end connector = %c\n", all_data[all_data[end_index].list_pointer[all_data[end_index].list_size].end]
            .connector);
    printf("start = %d\n", all_data[end_index].list_pointer[all_data[end_index].list_size].start);
    printf("end = %d\n", all_data[end_index].list_pointer[all_data[end_index].list_size].end);
    printf("price = %d\n", all_data[end_index].list_pointer[all_data[end_index].list_size].price);
    printf("dist = %d\n", all_data[end_index].list_pointer[all_data[end_index].list_size].dist);
    printf("============================\n");
#endif
    if (all_data[end_index].list_size == 0)
    {
        all_data[end_index].max_length_found = dist;
    }
    else
    {
        if (all_data[end_index].max_length_found < dist)
        {
            all_data[end_index].max_length_found = dist;
        }
    }
}


/**
 * @brief reads all the pieces lines and check for errors
 */
void readFourAndAfterLines()
{
    char start, end;
    int price, dist, start_index, end_index;
    char string_line[LENGTH_OF_LINE];
    char left_overs[LENGTH_OF_LINE];
    fgets(string_line, LENGTH_OF_LINE, input_file_ptr); // to remove unwanted chars
    while (true)
    {
        if (fgets(string_line, LENGTH_OF_LINE, input_file_ptr) == NULL)
        {
            break;
        }

        int scan_result = sscanf(string_line, "%c,%c,%d,%d%s", &start, &end, &dist, &price, left_overs);

        if ((scan_result != 4) || (price <= 0) || (dist <= 0))
        {
            fprintf(output_file_ptr, "Invalid input in line: %d.", line_counter);
            exitProgram(EXIT_FAILURE);
        }
        bool start_exists = false;
        bool end_exists = false;
        for (int i = 0; i < connectors_number; i++)
        {
            if (all_data[i].connector == start)
            {
                start_exists = true;
                start_index = i;
            }
            if (all_data[i].connector == end)
            {
                end_exists = true;
                end_index = i;
            }

            if (start_exists && end_exists)
            {
                addPieceToData(end_index, start_index, price, dist);
                all_data[end_index].list_size++;
                break;
            }
        }

        if ((start_exists == false) || (end_exists == false))
        {
            fprintf(output_file_ptr, "Invalid input in line: %d.", line_counter);
            exitProgram(EXIT_FAILURE);
        }
        line_counter++;
    }

    for (int c = 0; c < connectors_number; c++)
    {
        // no continue
        all_data[c].list_min_price_per_length_pointer_no_continue = (min_price_per_length_data*) calloc(all_data[c]
                                                                                                                .max_length_found + 1, sizeof(min_price_per_length_data));
        if (all_data[c].list_min_price_per_length_pointer_no_continue == NULL)
        {
            exitProgram(EXIT_FAILURE);
        }
        for (int i = 0; i < all_data[c].max_length_found; i++) // init min price with INT MAX
        {
            all_data[c].list_min_price_per_length_pointer_no_continue[i + 1].min_price = INT_MAX;
        }

        for (int j = 0; j < all_data[c].list_size; j++) // for each piece
        {
            if (all_data[c].list_pointer[j].price <
                all_data[c].list_min_price_per_length_pointer_no_continue[all_data[c].list_pointer[j].dist].min_price)
            {
                all_data[c].list_min_price_per_length_pointer_no_continue[all_data[c].list_pointer[j].dist].min_price
                        = all_data[c].list_pointer[j].price;
                all_data[c].list_min_price_per_length_pointer_no_continue[all_data[c].list_pointer[j].dist].min_start
                        = all_data[c].list_pointer[j].start;
            }
        }


        // continue
        all_data[c].list_min_price_per_length_pointer_continue = (min_price_per_length_data*) calloc(all_data[c]
                                                                                                             .max_length_found + 1, sizeof(min_price_per_length_data));
        if (all_data[c].list_min_price_per_length_pointer_continue == NULL)
        {
            exitProgram(EXIT_FAILURE);
        }
        for (int i = 0; i < all_data[c].max_length_found; i++) // init min price with INT MAX
        {
            all_data[c].list_min_price_per_length_pointer_continue[i + 1].min_price = INT_MAX;
        }

        for (int j = 0; j < all_data[c].list_size; j++) // for each piece
        {
            if (((all_data[c].list_pointer[j].price <
                  all_data[c].list_min_price_per_length_pointer_continue[all_data[c].list_pointer[j].dist].min_price) &&
                 (all_data[all_data[c].list_pointer[j].start].list_size != 0)) ||
                ((all_data[c].list_pointer[j].price) ==
                 (all_data[c].list_min_price_per_length_pointer_continue[all_data[c].list_pointer[j].dist].min_price) &&
                 (all_data[c].list_pointer[j].start == all_data[c].list_pointer[j].end)))
            {
                all_data[c].list_min_price_per_length_pointer_continue[all_data[c].list_pointer[j].dist].min_price
                        = all_data[c].list_pointer[j].price;
                all_data[c].list_min_price_per_length_pointer_continue[all_data[c].list_pointer[j].dist].min_start
                        = all_data[c].list_pointer[j].start;
            }
        }
    }
}


/**
 * @brief deal with the input file and saves the elements inside.
 * @param input_file_Address : string of the input file address
 * @param output_file_ptr : pointer to output file
 * @return if all went right returns true and otherwise return false.
 */
void readInput(char* const input_file_Address)
{
    //     ---check Errors---
    input_file_ptr = fopen(input_file_Address, "r");
    if (input_file_ptr == NULL)
    {
        fprintf(output_file_ptr, "File doesn't exists.");
        exitProgram(EXIT_FAILURE);
    }
    if (isEmpty(input_file_ptr) == true)
    {
        fprintf(output_file_ptr, "File is empty.");
        exitProgram(EXIT_FAILURE);
    }

    readFirstLine();
    readSecondLine();
    readThirdLine();
    readFourAndAfterLines();
    if (length == 0)
    {
        fprintf(output_file_ptr, "The minimal price is: %d", PRICE_WHEN_LENGTH_0);
        exitProgram(EXIT_SUCCESS);
    }
#ifdef DEBUG
    printf("## connector numbers = %d ##\n", connectors_number);
#endif
}


/**
 * @brief the function build the dynamic table
 */
void buildTable()
{
    table = (int**) calloc(connectors_number, sizeof(int*));
    if (table == NULL) // check if calloc didnt work
    {
        exitProgram(EXIT_FAILURE);
    }
    for (int i = 0; i < connectors_number; i++)
    {
        table[i] = (int*) calloc((length + 1), sizeof(int));
        if (table[i] == NULL) // check if calloc didnt work
        {
            exitProgram(EXIT_FAILURE);
        }
    }
}


/**
 * @brief compare the price with other length and return the lowest price found
 * @param l index of the current length comparing
 * @param c index of the current connector comparing
 * @param min_price_line minimum price of the current line in table
 * @return the minimum price found
 */
int comparePrices(int const l, int const c, int min_price_line)
{
    int min_price = INT_MAX;
    int min_price_connector = INT_MAX;
    int min_start = 0;
    bool found = false;
    for (int ll = 0; ll < l ; ll++) // look for pairs from up to down
    {
        if (ll == 0) // full size so no continue needed
        {
            if (l <= all_data[c].max_length_found) // if searched size is smaller than maximum pieces size
            {
                if (all_data[c].list_min_price_per_length_pointer_no_continue[l-ll].min_price < INT_MAX)
                {
                    min_price = all_data[c].list_min_price_per_length_pointer_no_continue[l-ll].min_price;
                    min_start = all_data[c].list_min_price_per_length_pointer_no_continue[l-ll].min_start;
                    found = true;
                }
            }
        }
        else if ((l - ll) <= all_data[c].max_length_found) // smaller size so continue needed
        {
            if (all_data[c].list_min_price_per_length_pointer_continue[l-ll].min_price < INT_MAX)
            {
                min_price = all_data[c].list_min_price_per_length_pointer_continue[l-ll].min_price;
                min_start = all_data[c].list_min_price_per_length_pointer_continue[l-ll].min_start;
                found = true;
            }
        }
        if (found)
        {
#ifdef DEBUG
            printf("min_price = %d\n", min_price);
            printf("+table[%d][%d] = %d\n\n", min_start, ll, table[min_start][ll]);
#endif
            min_price += table[min_start][ll];
            if (min_price < 0)
            {
                min_price = INT_MAX;
            }
            if (min_price < min_price_connector)
            {
                min_price_connector = min_price;
#ifdef DEBUG
                printf("min_price_connector = %d\n", min_price_connector);
#endif
            }
        }
    }

    table[c][l] = min_price_connector;
#ifdef DEBUG
    printf("table[%d][%d] = %d\n", c, l, min_price_connector);
#endif
    if (min_price_connector < min_price_line)
    {
        min_price_line = min_price_connector;
        if (l == length)
        {
            end_connector_chosen = c;
        }
    }

    return min_price_line;
}


/**
 * @brief the function fills the dynamic table and finds the minimum price
 * @return the minimum price
 */
int findMinPrice()
{
    for (int i = 0; i < connectors_number; i++) // fill bottom line with 0
    {
        table[i][0] = 0;
    }

    int min_price_line; // saves the min of each line for final result
    for (int l = 1; l <= length; l++) // for length
    {
        min_price_line = INT_MAX;
        for (int c = 0; c < connectors_number; c++) // for each connector
        {
            min_price_line = comparePrices(l, c, min_price_line);
        }
    }
#ifdef DEBUG
    printf("\nend connector chosen= %d", end_connector_chosen);
#endif
    return min_price_line;
}

/**
 * @brief main function
 * @param argc number of arguments
 * @param argv string of the address of the input file
 * @return Success or Failure exit
 */
int main(int argc, char *argv[])
{
    output_file_ptr = fopen("railway_planner_output.txt", "w");
    if (output_file_ptr == NULL)
    {
        exitProgram(EXIT_FAILURE);
    }

    if (argc != 2)
    {
        fprintf(output_file_ptr, "Usage: RailwayPlanner <InputFile>");
        exitProgram(EXIT_FAILURE);
    }

    readInput(argv[1]);
    buildTable();
    int min_price = findMinPrice();
    if (min_price == INT_MAX)
    {
        min_price = -1;
    }
#ifdef DEBUG
    printf("\n***************\n");
    printf("min price= %d\n", min_price);
    printf("***************");
#endif
    fprintf(output_file_ptr, "The minimal price is: %d", min_price);
    exitProgram(EXIT_SUCCESS);
}