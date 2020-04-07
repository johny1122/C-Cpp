#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define FILE_INPUT
//#define DEBUG
#define MAX_STUDENT_NUMBER 5500
#define NAME 1
#define COUNTRY 2
#define CITY 3
#define NUMBER_OF_PARAMETERS 6
#define START_OF_ARRAY 0
#define LENGTH_LINE 59
#define MINIMUM_AGE 18
#define MAXIMUM_AGE 120
#define MINIMUM_GRADE 0
#define MAXIMUM_GRADE 100

/**
 * @brief a struck of student
 */
typedef struct Student
{
    long id;
    char name[42];
    int grade;
    int age;
    char country[42];
    char city[42];
} Student;

/**
 * @brief a struck which saves the start and end of an array
 */
typedef struct Array
{
    int start;
    int end;
} Array;

Student students[MAX_STUDENT_NUMBER];
int students_number = 0;

void findBestStudent();

void inputStudent();

int checkInput(Student student, int params_number);

int checkNames(char str[], char type);

void merge(Array a, Array b);

void mergeSort(Array currArray);

void quickSort(Array currArray);

int partition(Array currArray);

void swap(int i, int j);

//***************************************************

/**
 * @brief checks if all the strings input names and valid
 */
int checkNames(char str[], char type)
{
    if (type == NAME)
    {
        for (int i = 0; i < (int) strlen(str); i++)
        {
            if (!((str[i] >= 65 && str[i] <= 90) || (str[i] >= 97 && str[i] <= 122) || (str[i] == 32)
                || (str[i] == 45)))
            {
                return 1;
            }
        }
    }
    else if (type == COUNTRY || type == CITY)
    {
        for (int i = 0; i < (int) strlen(str); i++)
        {
            if (!((str[i] >= 65 && str[i] <= 90) || (str[i] >= 97 && str[i] <= 122)
                || (str[i] == 45)))
            {
                return 1;
            }
        }
    }
    return 0;
}

/**
 * @brief checks if a line input is valid
 * @param student: a struct of student type
 * @param params_number: number of the parameters that were entered
 * @return 1 for invalid and 0 for valid
 */
int checkInput(Student student, int params_number)
{
    if (params_number != NUMBER_OF_PARAMETERS)
    {
        printf("ERROR: info must match specified format\n");
        return 1;
    }

    else if ((student.id < 1000000000) || (student.id > 9999999999))
    {
        printf("ERROR: id must be a 10 digits number that does not start with 0\n");
        return 1;
    }

    else if (checkNames(student.name, NAME) == 1)
    {
        printf("ERROR: name can only contain alphabetic characters, whitespaces or '-'\n");
        return 1;
    }

    else if ((student.grade < MINIMUM_GRADE) || (student.grade > MAXIMUM_GRADE))
    {
        printf("ERROR: grade must be an integer between 0 and 100\n");
        return 1;
    }

    else if ((student.age < MINIMUM_AGE) || (student.age > MAXIMUM_AGE))
    {
        printf("ERROR: age must be an integer between 18 and 120\n");
        return 1;
    }

    else if (checkNames(student.country, COUNTRY) == 1)
    {
        printf("ERROR: country can only contain alphabetic characters or '-'\n");
        return 1;
    }

    else if (checkNames(student.city, CITY) == 1)
    {
        printf("ERROR: city can only contain alphabetic characters or '-'\n");
        return 1;
    }

    else
    {
        return 0;
    }

}

/**
 * @brief: function that handle the input of the program
 */
void inputStudent()
{
    int line_count = 0;
    char input[LENGTH_LINE];
    Student student;
#ifdef FILE_INPUT
    FILE *file = fopen("C:\\Users\\Jonathan\\ComputerSience\\semesterB\\CC++\\Ex1\\quick_input12multi_unsorted.txt",
            "r");
#endif
    while (students_number < MAX_STUDENT_NUMBER)
    {
#ifndef FILE_INPUT
        printf("Enter student info. To exit press q, then enter\n");
        fgets(input, LENGTH_LINE, stdin);
#else
        fgets(input, LENGTH_LINE, file);
#endif
        strtok(input, "\n");
        if (input[0] == 'q')
        {
            break;
        }
        int params_number = sscanf(input, "%ld,%[^,],%d,%d,%[^,],%[^,]", &student.id, student
                .name, &student.grade, &student.age, student.country, student.city);
        if (checkInput(student, params_number) == 0)
        {
            students[students_number] = student;
            line_count++;
        }
        else
        {
            printf("in line %d\n", line_count);
            line_count++;
            continue;
        }
        students_number++;
    }
}

/**
 * @brief function that finds the best student
 */
void findBestStudent()
{
    int best_student = 0;
    float best_result = 0;
    for (int i = 0; i <= students_number; i++)
    {
        if (((float) students[i].grade / (float) students[i].age) > best_result)
        {
            best_result = ((float) students[i].grade / (float) students[i].age);
            best_student = i;
        }
    }
    printf("best student info is: %ld,%s,%d,%d,%s,%s\n", students[best_student].id,
           students[best_student].name, students[best_student].grade, students[best_student].age,
           students[best_student].country, students[best_student].city);
}

/**
 * @brief merge 2 arrays to 1 sorted array
 * @param a struct of Array
 * @param b struct of Array
 */
void merge(Array a, Array b)
{
    int i = a.start;
    int j = b.start;
    int k = 0;
    Student mergeArray[(b.end - a.start) + 1];

    while (i <= a.end && j <= b.end)
    {
        if (students[i].grade <= students[j].grade)
        {
            mergeArray[k] = students[i];
            i++;
        }
        else
        {
            mergeArray[k] = students[j];
            j++;
        }
        k++;
    }

    while (i <= a.end)
    {
        mergeArray[k] = students[i];
        i++;
        k++;
    }

    while (j <= b.end)
    {
        mergeArray[k] = students[j];
        j++;
        k++;
    }

    //put back the sort elements in the original array
    k = 0;
    for (int t = a.start; t <= b.end; t++)
    {
        students[t] = mergeArray[k];
        k++;
    }
}

/**
 * @brief sort an array with merge-sort algorithm
 */
void mergeSort(Array currArray)
{
    if (currArray.start < currArray.end)
    {
        int pivot = (currArray.end + currArray.start) / 2;

        Array left;
        left.start = currArray.start;
        left.end = pivot;

        Array right;
        right.start = pivot + 1;
        right.end = currArray.end;

        mergeSort(left);
        mergeSort(right);

        merge(left, right);
    }
}


/**
 * @brief swap 2 student in the students array
 * @param i student index in array
 * @param j student index in array
 */
void swap(int i, int j)
{
    Student temp = students[i];
    students[i] = students[j];
    students[j] = temp;
}

/**
 * @brief do the partition part of quick-sort
 * @param currArray struct of the current array handled
 */
int partition(Array currArray)
{
    int store_index = currArray.start - 1;
    for (int i = currArray.start; i < currArray.end; i++)
    {
        if (students[i].name[START_OF_ARRAY] < students[currArray.end].name[START_OF_ARRAY])
        {
            store_index++;
            swap(i, store_index);
        }

        else if (students[i].name[START_OF_ARRAY] == students[currArray.end].name[START_OF_ARRAY])
        {
            int name_length = strlen(students[i].name);
            int j = 0;
            do
            {
                j++;
                if (students[i].name[j] < students[currArray.end].name[j])
                {
                    store_index++;
                    swap(i, store_index);
                    break;
                }
            } while ((students[i].name[j] == students[currArray.end].name[j]) && (j < name_length - 1));
        }
    }
    swap(store_index + 1, currArray.end);
    return store_index + 1;
}

/**
 * @brief sort the students array alphabetically with quick-sort algorithm
 */
void quickSort(Array currArray)
{
    if (currArray.start < currArray.end)
    {
        int partition_index = partition(currArray);

        Array small;
        small.start = currArray.start;
        small.end = partition_index - 1;
        quickSort(small);

        Array big;
        big.start = partition_index + 1;
        big.end = currArray.end;
        quickSort(big);

    }
}

/**
 * @brief prints a sorted student array
 */
void printSort()
{
    for (int i = 0; i < students_number; i++)
    {
        printf("%ld,%s,%d,%d,%s,%s\n", students[i].id, students[i].name, students[i].grade,
               students[i].age, students[i].country, students[i].city);
    }
}

/**
 * @brief the main function
 * @param argc: number of arguments entered
 * @param argv: array of all the arguments
 * @return 1 if the program fail and 0 otherwise
 */
int main(int argc, char *argv[])
{
    //check the input arguments
    if ((argc == 2) && ((strcmp(argv[1], "best") == 0) || (strcmp(argv[1], "merge") == 0) ||
        (strcmp(argv[1], "quick") == 0)))
    {
        inputStudent();
        if (students_number == 0)
        {
            return 0;
        }

        else if (strcmp(argv[1], "best") == 0)
        {
            findBestStudent();
        }

        else if (strcmp(argv[1], "merge") == 0)
        {
            Array full_array;
            full_array.start = 0;
            full_array.end = students_number - 1;
            mergeSort(full_array);
            printSort();
        }

        else if (strcmp(argv[1], "quick") == 0)
        {
#ifdef DEBUG
            printSort();
            printf("\n");
#endif
            Array full_array;
            full_array.start = 0;
            full_array.end = students_number - 1;
            quickSort(full_array);
            printSort();
        }
    }
    else
    {
        printf("USAGE: the input argument is invalid. please run the program again\n");
        return 1;
    }
    return 0;
}


