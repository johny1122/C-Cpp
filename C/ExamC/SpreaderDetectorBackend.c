/**
 * @file SpreaderDetectorBackend.c
 * @author  Jonathan Birnbaum
 * @date 02/08/2020
 *
 * @brief Spreader Detector Backend program implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "SpreaderDetectorParams.h"


/**
 * @brief size, quantity and index macros
 */
#define LINE_LENGTH (1024)
#define NUMBER_OF_INPUT_ARGUMENTS (3)
#define ID_SIZE (9)
#define MAX_AGE_SIZE (5)
#define NUMBER_OF_POSSIBLE_SPACES_IN_LINE (2)
#define MAX_NAME_SIZE (LINE_LENGTH - ID_SIZE - MAX_AGE_SIZE - NUMBER_OF_POSSIBLE_SPACES_IN_LINE)
#define ALLOC_SIZE (10)
#define PEOPLE_FILE_PATH_INDEX (1)
#define MEETINGS_FILE_PATH_INDEX (2)
#define SPREADER_INFECTION_VALUE (1)

/**
 * @brief comparator values
 */
#define P1_IS_SMALLER (-1)
#define P1_IS_BIGGER (1)
#define P1_AND_P2_ARE_EQUAL (0)

/**
 * @brief Error messages
 */
#define ERROR_MSG_IN_INPUT_FILES "Error in input files.\n"
#define ERROR_MSG_IN_OUTPUT_FILE "Error in output file.\n"
#define ERROR_MSG_INPUT_FORMAT "Usage:./SpreaderDetectorBackend <Path to People.in> <Path to Meetings.in>\n"


/**
 * @brief struct of a single person and his data (name, number id, age, infection_chance value)
 */
typedef struct Person
{
    char name [MAX_NAME_SIZE];
    long id;
    float age;
    float infection_chance;
}Person;


void exitProgram(int const exit_type);
bool readInputPeople(char* const input_file_Address);
bool readInputMeetings(char* const input_file_Address);
int idComparator(const void* person1, const void* person2);
int chanceComparator(const void* person1, const void* person2);
void printResultsToFile(bool peopleFileEmpty, bool meetingsFileEmpty);


FILE* output_file_ptr = NULL;
FILE* input_people_file_ptr = NULL;
FILE* input_meeting_file_ptr = NULL;
Person* people = NULL;
int number_of_people = 0;



/**
 * @brief The function closes all files, free memory and exit the program with the given exit_type
 */
void exitProgram(int const exit_type)
{
    if (input_people_file_ptr != NULL)
    {
        if(fclose(input_people_file_ptr) == EOF)
        {
            exit(EXIT_FAILURE);
        }
        input_people_file_ptr = NULL;
    }

    if (input_meeting_file_ptr != NULL)
    {
        if(fclose(input_meeting_file_ptr) == EOF)
        {
            exit(EXIT_FAILURE);
        }
        input_meeting_file_ptr = NULL;
    }

    if (output_file_ptr != NULL)
    {
        if(fclose(output_file_ptr) == EOF)
        {
            exit(EXIT_FAILURE);
        }
        output_file_ptr = NULL;
    }

    if (people != NULL)
    {
        free(people);
        people = NULL;
    }

    if (exit_type == EXIT_FAILURE)
    {
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}


 /**
  * @brief read the People file and checks for errors
  * @param input_file_Address: path to People file
  * @return true if people file is empty and false otherwise
  */
bool readInputPeople(char* const input_file_Address)
{
    //     ------------ Check Errors ------------
    input_people_file_ptr = fopen(input_file_Address, "r");
    if (input_people_file_ptr == NULL)
    {
        fprintf(stderr, ERROR_MSG_IN_INPUT_FILES);
        exitProgram(EXIT_FAILURE);
    }

    //     ------------ ALLOCATE MEMORY ------------
    people = (Person*) calloc(ALLOC_SIZE, sizeof(Person));
    if (people == NULL)
    {
        fprintf(stderr, STANDARD_LIB_ERR_MSG);
        exitProgram(EXIT_FAILURE);
    }

    //     ------------ INPUT PEOPLE ------------
    bool first_line_is_empty = true;
    char line[LINE_LENGTH];

    while (fgets(line, LINE_LENGTH, input_people_file_ptr) != NULL)
    {
        if (first_line_is_empty)
        {
            first_line_is_empty = false;
        }
        Person person;
        strtok(line, "\n");
        sscanf(line, "%s %ld %f", person.name, &person.id, &person.age);
        number_of_people++;
        if ((number_of_people % ALLOC_SIZE) == 0) // allocate more memory if array is full
        {
            people = (Person*) realloc(people, (number_of_people + ALLOC_SIZE) * sizeof(Person));
            if (people == NULL)
            {
                fprintf(stderr, STANDARD_LIB_ERR_MSG);
                exitProgram(EXIT_FAILURE);
            }
        }
        person.infection_chance = 0;
        people[number_of_people - 1] = person;
    }
    if (first_line_is_empty) // if file is empty
    {
        return true;
    }
    return false;
}


 /**
  * @brief read the Meetings file and checks for errors
  * @param input_file_Address: path to Meetings file
  * @return true if meetings file is empty and false otherwise
  */
bool readInputMeetings(char* const input_file_Address)
{
    //     ------------ Check Errors ------------
    input_meeting_file_ptr = fopen(input_file_Address, "r");
    if (input_meeting_file_ptr == NULL)
    {
        fprintf(stderr, ERROR_MSG_IN_INPUT_FILES);
        exitProgram(EXIT_FAILURE);
    }

    //     ------------ INPUT MEETINGS ------------
    char line[LINE_LENGTH];
    if (fgets(line, LINE_LENGTH, input_meeting_file_ptr) == NULL) // gets first line and checks if file is empty
    {
        return true;
    }
    strtok(line, "\n");
    long spreader_id = 0;
    sscanf(line, "%ld", &spreader_id); // first line input

    long infector_id = 0, infected_id = 0, last_infector_id = 0;
    float distance = 0, time = 0, last_infector_chance = 0;

    //     ------------ INIT SPREADER INFECTION_VALUE WITH 1 ------------
    Person find_person;
    find_person.id = spreader_id;
    Person* spreader = bsearch(&find_person, people, number_of_people, sizeof(Person), idComparator);
    if (spreader == NULL)
    {
        fprintf(stderr, STANDARD_LIB_ERR_MSG);
        exitProgram(EXIT_FAILURE);
    }
    spreader->infection_chance = SPREADER_INFECTION_VALUE; // spreader should absolutely go to hospital
    last_infector_id = spreader_id;
    last_infector_chance = SPREADER_INFECTION_VALUE;

    while (fgets(line, LINE_LENGTH, input_meeting_file_ptr) != NULL)
    {
        strtok(line, "\n");
        sscanf(line, "%ld %ld %f %f", &infector_id, &infected_id, &distance, &time);

        //     ------------ FIND INFECTED PERSON ------------
        find_person.id = infected_id;
        Person* infected = bsearch(&find_person, people, number_of_people, sizeof(Person), idComparator);
        if (infected == NULL)
        {
            fprintf(stderr, STANDARD_LIB_ERR_MSG);
            exitProgram(EXIT_FAILURE);
        }

        //     ------------ CALCULATE AND ADD INFECTION CHANCE TO PERSON ------------

        float curr_infected_chance =  (time * MIN_DISTANCE) / (distance * MAX_TIME); //Crna function

        if (infector_id == last_infector_id) // current infector is same as last infector
        {
            infected->infection_chance = curr_infected_chance * last_infector_chance;
        }

        else // new infector
        {
            find_person.id = infector_id;
            Person* infector = bsearch(&find_person, people, number_of_people, sizeof(Person), idComparator);
            if (infector == NULL)
            {
                fprintf(stderr, STANDARD_LIB_ERR_MSG);
                exitProgram(EXIT_FAILURE);
            }
            infected->infection_chance = curr_infected_chance * infector->infection_chance;
            last_infector_id = infector_id;
            last_infector_chance = infector->infection_chance;
        }
    }
    return false;
}


/**
 * @brief a comparator function between person's id
 * @param person1: object of person 1
 * @param person2: object of person 2
 * @return: -1 if person1 is smaller, 1 if person1 is bigger and 0 if equal
 */
int idComparator(const void* person1, const void* person2)
{
    Person* p1 = (Person*) person1;
    Person* p2 = (Person*) person2;
    if (p1->id < p2->id)
    {
        return P1_IS_SMALLER;
    }
    else if (p1->id > p2->id)
    {
        return P1_IS_BIGGER;
    }
    return P1_AND_P2_ARE_EQUAL;
}


/**
 * @brief a comparator function between person's infection chance
 * @param person1: object of person 1
 * @param person2: object of person 2
 * @return: -1 if person1 is smaller, 1 if person1 is bigger and 0 if equal
 */
int chanceComparator(const void* person1, const void* person2)
{
    Person* p1 = (Person*) person1;
    Person* p2 = (Person*) person2;
    if (p1->infection_chance < p2->infection_chance)
    {
        return P1_IS_SMALLER;
    }
    else if (p1->infection_chance > p2->infection_chance)
    {
        return P1_IS_BIGGER;
    }
    return P1_AND_P2_ARE_EQUAL;
}


 /**
  * @brief the function sorts the people array by the infected_chance and print them accordingly
  * @param peopleFileEmpty: boolean as if the people file is empty or not
  * @param meetingsFileEmpty: boolean as if the meetings file is empty or not
  */
void printResultsToFile(bool peopleFileEmpty, bool meetingsFileEmpty)
{
    output_file_ptr = fopen(OUTPUT_FILE, "w");
    if (output_file_ptr == NULL)
    {
        fprintf(stderr, ERROR_MSG_IN_OUTPUT_FILE);
        fprintf(stderr, ERROR_MSG_IN_OUTPUT_FILE);
        exitProgram(EXIT_FAILURE);
    }

    if (peopleFileEmpty)
    {
        return; // exit because no people to print
    }

    if (meetingsFileEmpty)
    {
        for (int index = 0; index < number_of_people; index++) // no meetings so all people are clean
        {
            fprintf(output_file_ptr, CLEAN_MSG, people[index].name, people[index].id);
        }
        return;
    }

    qsort(people, number_of_people, sizeof(Person), chanceComparator); // sorts the people array by infection_chance

    for (int index = number_of_people - 1; index >= 0; index--)
    {
        if ((people[index].infection_chance >= MEDICAL_SUPERVISION_THRESHOLD))
        {
            fprintf(output_file_ptr, MEDICAL_SUPERVISION_THRESHOLD_MSG, people[index].name, people[index].id);
        }
        else if (people[index].infection_chance >= REGULAR_QUARANTINE_THRESHOLD)
        {
            fprintf(output_file_ptr, REGULAR_QUARANTINE_MSG, people[index].name, people[index].id);
        }
        else
        {
            fprintf(output_file_ptr, CLEAN_MSG, people[index].name, people[index].id);
        }
    }
}


/**
 * @brief main function
 * @param argc: number of input arguments
 * @param argv: array of arguments
 * @return: EXIT_SUCCESS/EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
    if (argc != NUMBER_OF_INPUT_ARGUMENTS)
    {
        fprintf(stderr, ERROR_MSG_INPUT_FORMAT);
        exitProgram(EXIT_FAILURE);
    }

    bool peopleFileEmpty = readInputPeople(argv[PEOPLE_FILE_PATH_INDEX]);
    bool meetingsFileEmpty = false;
    if (!peopleFileEmpty)
    {
        qsort(people, number_of_people, sizeof(Person), idComparator); // sorts the people array by id number
        meetingsFileEmpty = readInputMeetings(argv[MEETINGS_FILE_PATH_INDEX]);
    }
    printResultsToFile(peopleFileEmpty, meetingsFileEmpty);
    exitProgram(EXIT_SUCCESS);
}
