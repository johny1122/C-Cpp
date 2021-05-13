#include <stdio.h>
#include <stdlib.h>

// part 1:
typedef struct Date Date;

struct Date {
    unsigned int day; // a number between 1 to 31
    unsigned int month; // a number between 1 to 12
    char * event;
    Date * next_date;
};

Date * create_date(unsigned int day,
                   unsigned int month, char * event)
{
    if (!(day >= 1 && day <= 31) ||
        !(month >= 1 && month <= 12) ||
        event == NULL) {
        return NULL;
    }
    Date * d = (Date *) malloc (sizeof(Date));
    if (d == NULL) {
        return NULL;
    }
    d->day = day;
    d->month = month;
    d->event = event;
    d->next_date = NULL;
    return d;
}

int are_equals(Date * date1, Date * date2)
{
    if ((date1 == NULL) || (date2 == NULL))
        return 0;
    if((date1->day == date2->day) && (date1->month == date2->month))
        return 1;
    return 0;
}

void free_date(Date *date)
{
    if (date != NULL)
        free(date);
}

// part 2:
typedef struct Calendar {
    Date * first_date;
} Calendar;

void init_calendar(Calendar * c)
{
    c->first_date = NULL;
}

int add_to_calendar(Calendar * c, unsigned int day,
                    unsigned int month, char * event)
{
    if (c == NULL)
        return 0;

    if ((day > 31) || (day < 1) || (month > 12) || (month < 1))
        return 0;

    Date* new_day = (Date*) malloc(sizeof(Date));
    if (new_day == NULL)
        return 0;
    new_day->day = day;
    new_day->month = month;
    new_day->event = event;

    Date* curr_day = c->first_date;
    if (curr_day == NULL)
    {
        c->first_date = new_day;
        return 1;
    }

    while (curr_day->next_date!= NULL)
    {
        if (are_equals(curr_day,new_day))
        {
            free_date(new_day);
            return 0;
        }
        curr_day = curr_day->next_date;
    }
    if (are_equals(curr_day, new_day))
    {
        free_date(new_day);
        return 0;
    }

    curr_day->next_date = new_day;
    return 1;
}

void free_calendar(Calendar * c)
{
    if (c == NULL)
        return;
    Date* curr_day = c->first_date;
    Date* temp = NULL;
    while (curr_day != NULL)
    {
        temp = curr_day;
        curr_day = curr_day->next_date;
        free_date(temp);
    }
    c->first_date = NULL;
}

void print_calendar(Calendar * c)
{
    if (c == NULL) {
        return;
    }
    Date * current = c->first_date;
    while (current != NULL) {
        printf("%s\n", current->event);
        current = current->next_date;
    }
}