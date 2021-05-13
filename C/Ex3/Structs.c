/**
 * @file Structs.c
 * @author  Jonathan Birnbaum
 * @date 18/05/2020
 *
 * @brief Few functions to activate on a Red-Black tree library.
 */

#include <stdlib.h>
#include <string.h>
#include "Structs.h"

/**
 * @brief used to return a exit status from functions
 */
#define FAILURE (0)
#define SUCCESS (1)

/**
 * @brief used when comparing vectors to know which vector is smaller/bigger
 */
#define EPSILON (0.01)

/**
 * @brief used to return the result of comparing vectors
 */
#define EQUAL (0)
#define A_SMALLER (-1)
#define B_SMALLER (1)


double calculateInsideOfNorm (const Vector* v);
int copyIfNormIsLarger(const void *pVector, void *pMaxVector);
Vector *findMaxNormVectorInTree(RBTree *tree);
void freeVector(void *pVector);
int stringCompare(const void *a, const void *b);
int concatenate(const void *word, void *pConcatenated);
void freeString(void *s);



//----------------- VECTOR FUNCTIONS -----------------------

/**
 */
int vectorCompare1By1(const void *a, const void *b)
{
    Vector* v1 = (Vector*) a;
    Vector* v2 = (Vector*) b;
    double* va = v1->vector;
    double* vb = v2->vector;
    int len_a = v1->len, len_b = v2->len;
    int i = 0;
    while ((i < len_a) && (i < len_b))
    {
        if ((vb[i] - va[i]) > EPSILON)
        {
            return A_SMALLER;          //if (va[i] < vb[i]) return A_SMALLER;
        }
        else if ((va[i] - vb[i]) > EPSILON)
        {
            return B_SMALLER;     //else if (va[i] > vb[i]) return B_SMALLER;
        }
        i++;
    }
    if (len_a < len_b)
    {
        return A_SMALLER;
    }
    else if (len_a > len_b)
    {
        return B_SMALLER;
    }
    return EQUAL;
}

/**
 * @brief calculate inside of square of norm of a vector
 * @param v: vector to calculate of
 * @return inside of norm
 */
double calculateInsideOfNorm (const Vector* v)
{
    int len = v->len;
    double sum = 0;
    for (int i = 0; i < len; i++)
    {
        sum += (v->vector[i] * v->vector[i]);
    }
    return sum;
}

/**
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
    if ((pMaxVector == NULL) || (pVector == NULL))
    {
        return FAILURE;
    }

    Vector* v = (Vector*) pVector;
    Vector* vmax = (Vector*) pMaxVector;

    if ((v->len == 0) || (v->vector == NULL))
    {
        return FAILURE;
    }

    double normV = 0, normVmax = 0;
    normV = calculateInsideOfNorm(v);
    if (vmax->vector != NULL)
    {
        normVmax = calculateInsideOfNorm(vmax);
    }
    if ((vmax->vector == NULL) || (normV > normVmax))
    {
        vmax->vector = (double*) realloc(vmax->vector, sizeof(double) * v->len);
        if (vmax->vector == NULL)
        {
            return FAILURE;
        }

        for (int i = 0; i < v->len; i++)
        {
            vmax->vector[i] = v->vector[i];
        }
        vmax->len = v->len;
    }
    return SUCCESS;
}

/**
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
    Vector* newVector = (Vector*) calloc(1, sizeof(Vector));
    if (forEachRBTree(tree, copyIfNormIsLarger, newVector))
    {
        return newVector;
    }
    free(newVector);
    return NULL;
}

/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector)
{
    Vector* v = (Vector*) pVector;
    free(v->vector);
    free(v);
}

//------------------- STRINGS FUNCTIONS --------------------------

/**
 */
int stringCompare(const void *a, const void *b)
{
    char* str_a = (char*) a;
    char* str_b = (char*) b;
    return strcmp(str_a, str_b);
}

/**
 */
int concatenate(const void *word, void *pConcatenated)
{
    if ((word == NULL) || (pConcatenated == NULL))
    {
        return FAILURE;
    }
    char* concatenate = (char*) pConcatenated;
    char* wordToAdd = (char*) word;
    strcat(concatenate, wordToAdd);
    strcat(concatenate, "\n");
    return SUCCESS;
}

/**
 */
void freeString(void *s)
{
    char* str = (char*) s;
    free(str);
}
