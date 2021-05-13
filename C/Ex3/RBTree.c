/**
 * @file RBTree.c
 * @author  Jonathan Birnbaum
 * @date 18/05/2020
 *
 * @brief Small library of Red-Black tree.
 * The library can create a new tree, add and delete nodes, free the tree and
 * activate a given function on every node in the tree in-order
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "RBTree.h"


/**
 * @brief used to return a exit status from functions
 */
#define FAILURE (0)
#define SUCCESS (1)

/**
 * @brief used to return if node found/didnt found in tree
 */
#define FOUND (1)
#define NOT_FOUND (0)

/**
 * @brief used to mark a child's side from parent
 */
#define RIGHT (1)
#define LEFT (-1)


Node* getGrandfather(const Node* node);
Node* getBrother(const Node* father, const Node* child);
Node* getUncle(const Node* node);
void insertNode(RBTree *tree, void *data, Node* new_node);
void rotateLeft(RBTree* tree, Node* node);
void rotateRight(RBTree* tree, Node* node);
Node* findChildSuccessor(const Node* node);
bool isBlack(const Node* node);
bool isRed(const Node* node);
void insertCase1(Node* node);
void freeRBTree(RBTree **tree);
bool forEachRBTreeHelper(Node* node, forEachFunc func, void *args, bool funcFail);
void insertCase3(RBTree* tree, Node* node, Node* uncle);
void insertCase4(RBTree* tree, Node* node);
void insertFixTree(RBTree* tree, Node* node);
Node* findNode(const RBTree *tree, const void *data);
RBTree* newRBTree(CompareFunc compFunc, FreeFunc freeFunc);
bool forEachRBTreeHelper(Node* node, forEachFunc func, void *args, bool funcFail);
int forEachRBTree(const RBTree *tree, forEachFunc func, void *args);
void disconnectNodeWithChild(RBTree* tree, int childSide, Node* nodeFather, Node* deleteNode, Node* replacement,
                             Node** xFather);
void swapNode(RBTree *tree, Node* deleteNode, Node* replacement, Node** xFather);
void deleteCase0(Node* x);
void deleteCase1(RBTree* tree, Node* xFather, Node* x, Node** w);
void deleteCase2(Node** x, Node* xFather, Node* w);
void deleteCase3(RBTree* tree, Node* xFather, Node* x, Node** w);
void deleteCase4(RBTree* tree, Node* xFather, Node* x, Node* w);
void deleteFix(RBTree* tree, Node** xFather, Node** x);



//-------------------  GENERAL FUNCTIONS  ---------------------

/**
 * @brief get the grandparent of a node
 * @param node
 * @return the grandparent or NULL if root or child of the root
 */
Node* getGrandfather(const Node* node)
{
    if ((node->parent == NULL) || (node->parent->parent == NULL))
    {
        return NULL;
    }
    return node->parent->parent;
}

/**
* @brief get the brother of a node
* @param father: father of node and brother
* @param child: node to find his brother
* @return brother's node
*/
Node* getBrother(const Node* father, const Node* child)
{
    if (father == NULL)
    {
        return NULL;
    }
    if (child == father->left)
    {
        return father->right;
    }
    else
    {
        return father->left;
    }
}

/**
 * @brief get the uncle node
 * @param node
 * @return the uncle or null if uncle or grandfather doesnt  exist
 */
Node* getUncle(const Node* node)
{
    if (getGrandfather(node) != NULL)
    {
        return getBrother(getGrandfather(node), node->parent);
    }
    else
    {
        return NULL;
    }
}

/**
 * @brief Rotate left the tree on node. assume node.right is not NULL
 * @param node
 */
void rotateLeft(RBTree* tree, Node* node)
{
    Node* parent = node->parent;
    Node* temp_node = node->right;
    node->right = temp_node->left;
    temp_node->left = node;
    node->parent = temp_node;

    if (node->right != NULL)
    {
        node->right->parent = node;
    }
    if (parent != NULL)
    {
        if (node == parent->left)
        {
            parent->left = temp_node;
        }
        else if (node == parent->right)
        {
            parent->right = temp_node;
        }
    }
    temp_node->parent = parent;
    if (tree->root == node)
    {
        tree->root = temp_node;
    }
}

/**
 * @brief Rotate right the tree on node. assume node.left is not NULL
 * @param node
 */
void rotateRight(RBTree* tree, Node* node)
{
    Node* parent = node->parent;
    Node* temp_node = node->left;
    node->left = temp_node->right;
    temp_node->right = node;
    node->parent = temp_node;

    if (node->left != NULL)
    {
        node->left->parent = node;
    }
    if (parent != NULL)
    {
        if (node == parent->right)
        {
            parent->right = temp_node;
        }
        else if (node == parent->left)
        {
            parent->left = temp_node;
        }
    }
    temp_node->parent = parent;
    if (tree->root == node)
    {
        tree->root = temp_node;
    }
}

/**
 * @brief find successor of node. assume node has 2 children
 * @param node - node to find successor to
 * @return successor node pointer
 */
Node* findChildSuccessor(const Node* node)
{
    Node* temp = node->right;
    while (temp->left != NULL)
    {
        temp = temp->left;
    }
    return temp;
}

/**
 * @brief check if a node is BLACK (NULL or has black color)
 * @param node - node to check color black
 * @return true if black, false if not
 */
bool isBlack(const Node* node)
{
    if ((node == NULL) || (node->color == BLACK))
    {
        return true;
    }
    return false;
}

/**
 * @brief check if a node is RED ( not NULL and has red color)
 * @param node - node to check color red
 * @return true if red, false otherwise
 */
bool isRed(const Node* node)
{
    if ((node != NULL) && (node->color == RED))
    {
        return true;
    }
    return false;
}

//----------------------- More Tree Functions -----------------------------------------

RBTree* newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
    if (compFunc == NULL || freeFunc == NULL) // check if input args are valid
    {
        return FAILURE;
    }
    RBTree* tree = (RBTree*) calloc(1, sizeof(RBTree));
    if (tree == NULL)
    {
        return NULL;
    }
    tree->freeFunc = freeFunc;
    tree->compFunc = compFunc;
    tree->size = 0;
    tree->root = NULL;

    return tree;
}

/**
 * check whether the tree RBTreeContains this item.
 * @param tree: the tree to check if has an item.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int RBTreeContains(const RBTree* tree, const void* data)
{
    if (tree == NULL || data == NULL) // check if input args are valid
    {
        return FAILURE;
    }
    Node* node = tree->root;
    while (node != NULL)
    {
        int compare = tree->compFunc(node->data, data);
        if (compare == 0) // found a node with the data
        {
            return FOUND;
        }
        else
        {
            if (compare < 0) // if data bigger - go to right sub-tree
            {
                node = node->right;
            }
            else if (compare > 0) // if data smaller - go to left sub-tree
            {
                node = node->left;
            }
        }
    }
    return NOT_FOUND;
}

/**
 * @brief help the function forEachRBTreeHelper
 * @param node: current node
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @param funcFail: bool as if the func failed
 * @return true if func failed and false if func succeeded
 */
bool forEachRBTreeHelper(Node* node, forEachFunc func, void *args, bool funcFail)
{
    bool result = funcFail;
    if(node != NULL && funcFail == false)
    {
        result = forEachRBTreeHelper(node->left, func, args, funcFail);
        if (result)
        {
            return true;
        }
        if (func(node->data, args) == 0)
        {
            return true;
        }
        result = forEachRBTreeHelper(node->right, func, args, funcFail);
    }
    if (result == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(const RBTree *tree, forEachFunc func, void *args)
{
    if ((tree == NULL) || (func == NULL))
    {
        return FAILURE;
    }
    if (tree->size == 0 || tree->root == NULL)
    {
        return SUCCESS;
    }
    bool funcFail = forEachRBTreeHelper(tree->root, func, args, false);
    if (funcFail)
    {
        return FAILURE;
    }
    return SUCCESS;
}


void freeRBTreeHelper(RBTree **tree, Node* node)
{
    if (node == NULL)
    {
        return;
    }

    freeRBTreeHelper(tree, node->left);
    freeRBTreeHelper(tree, node->right);

    (*tree)->freeFunc(node->data);
    free(node);
}

/**
 * free all memory of the data structure.
 * @param tree: pointer to the tree to free.
 */
void freeRBTree(RBTree **tree)
{
    freeRBTreeHelper(tree, (*tree)->root);
    free(*tree);
}


//-------------------  INSERTION   -------------------------------

/**
 * @brief insertion fix case 1
 * @param node - new added node
 */
void insertCase1(Node* node)
{
    node->color = BLACK;
}

/**
 * @brief insertion fix case 3
 * @param node - new added node
 * @param uncle node's uncle
 */
void insertCase3(RBTree* tree, Node* node, Node* uncle)
{
    uncle->color = BLACK;
    node->parent->color = BLACK;
    Node* grandfather = getGrandfather(node);
    grandfather->color = RED;
    insertFixTree(tree, grandfather);
}

/**
 * @brief insertion fix case 4
 * @param node - new added node
 */
void insertCase4(RBTree* tree, Node* node)
{
    Node* grandfather = getGrandfather(node);
    Node* nodeFather = node->parent;
    if ((node == node->parent->right) && (node->parent == grandfather->left)) // case 4a - right son of left dad
    {
        rotateLeft(tree, node->parent);
    }
    else if ((node == node->parent->left) && (node->parent == grandfather->right)) // case 4a - left son of right dad
    {
        rotateRight(tree, node->parent);
    }

    if ((node == node->parent->left) && (nodeFather == nodeFather->parent->left)) // case 4b - left son of left dad
    {
        rotateRight(tree, grandfather);
    }
    else if ((node == node->parent->right) && (nodeFather == nodeFather->parent->right)) // case 4b - right son of right dad
    {
        rotateLeft(tree, grandfather);
    }
    // case 4c
    grandfather->color = RED;
    grandfather->parent->color = BLACK;
}

/**
 * @brief insert a new node to the right place in tree
 * @param tree
 * @param data
 * @param new_node pointer new_node
 */
void insertNode(RBTree *tree, void *data, Node* new_node)
{
    Node* node = tree->root;
    Node* parent = NULL;
    int child_side = 0;
    while (node != NULL)
    {
        int compare = tree->compFunc(node->data, data);
        if (compare < 0) // if data bigger - go to right sub-tree
        {
            parent = node;
            node = node->right;
            child_side = RIGHT;
        }
        else if (compare > 0) // if data smaller - go to left sub-tree
        {
            parent = node;
            node = node->left;
            child_side = LEFT;
        }
    }
    if (tree->root == NULL)
    {
        tree->root = new_node; // if first node in tree
    }
    else // if node is not root - update parent child as new_node
    {
        (child_side == RIGHT) ? (parent->right = new_node) : (parent->left = new_node);
    }

    new_node->parent = parent;
    new_node->right = NULL;
    new_node->left = NULL;
    new_node->data = data;
    new_node->color = RED;
}

/**
 * @brief fix the tree after insertion
 * @param node - new added node
 */
void insertFixTree(RBTree* tree, Node* node)
{
    // ---- case 1 - node is the root -------
    if (node->parent == NULL)
    {
        insertCase1(node);
        return;
    }
    // ---- case 2 - nothing to do -------
    else if (node->parent->color == BLACK)
    {
        return;
    }
    else // parent color is RED
    {
        Node* uncle = getUncle(node);

        // ---- case 3 - uncle is RED -------
        if (uncle != NULL && uncle->color == RED)
        {
            insertCase3(tree, node, uncle);
        }
        // ---- case 4 - uncle is NULL or uncle is BLACK -------
        else
        {
            insertCase4(tree, node);
        }
    }
}

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int insertToRBTree(RBTree* tree, void *data)
{
    if (tree == NULL || data == NULL) // check if input args are valid
    {
        return FAILURE;
    }
    if (RBTreeContains(tree, data) == FOUND) // if data already exist
    {
        return FAILURE;
    }

    Node* new_node = (Node*) calloc(1, sizeof(Node));
    if (new_node == NULL)
    {
        return FAILURE;
    }

    insertNode(tree, data, new_node);

    insertFixTree(tree, new_node);

    tree->size += 1;

    return SUCCESS;
}


//------------------  DELETION  --------------------------------

/**
 * @brief find pointer to node with given data (assume data is in the tree)
 * @param tree - tree to search the node in
 * @param data - data to search
 * @return pointer to the matching node
 */
Node* findNode(const RBTree *tree, const void *data)
{
    Node* node = tree->root;
    while (node != NULL)
    {
        int compare = tree->compFunc(node->data, data);
        if (compare == 0) // found a node with the data
        {
            break;
        }
        else
        {
            if (compare < 0) // if data bigger - go to right sub-tree
            {
                node = node->right;
            }
            else if (compare > 0) // if data smaller - go to left sub-tree
            {
                node = node->left;
            }
        }
    }
    return node;
}


/**
* @brief swap a node to another one
* @param tree: node's tree
* @param deleteNode: node to swap with replacement
* @param replacement: node to swap with deleteNode
* @param xFather: x' father for update
*/
void swapNode(RBTree *tree, Node* deleteNode, Node* replacement, Node** xFather)
{
    if (deleteNode == (*xFather))
    {
        (*xFather) = replacement;
    }
    if (replacement != NULL)
    {
        replacement->parent = deleteNode->parent;
        if (deleteNode == tree->root)
        {
            tree->root = replacement; // deleteNode is the root
        }
        else if (deleteNode == deleteNode->parent->left) // deleteNode is left child
        {
            deleteNode->parent->left = replacement;
        }
        else if (deleteNode == deleteNode->parent->right) // deleteNode is right child
        {
            deleteNode->parent->right = replacement;
        }
        if ((deleteNode->left != replacement) && (deleteNode->right != replacement))
        {
            replacement->left = deleteNode->left;
            replacement->right = deleteNode->right;
        }
        if (replacement->left != NULL)
        {
            replacement->left->parent = replacement;
        }
        if (replacement->right != NULL)
        {
            replacement->right->parent = replacement;
        }
    }
    else
    {
        (*xFather) = deleteNode->parent;
        if (deleteNode == tree->root)
        {
            tree->root = replacement; // deleteNode is the root
        }
        else if (deleteNode == deleteNode->parent->left) // deleteNode is left child
        {
            deleteNode->parent->left = replacement;
        }
        else if (deleteNode == deleteNode->parent->right) // deleteNode is right child
        {
            deleteNode->parent->right = replacement;
        }
    }
}


/**
 * @brief
 * @param tree: node's tree
 * @param childSide: x's side a child
 * @param nodeFather: node's father
 * @param deleteNode: node to disconnect from tree
 * @param replacement: node to connect to deleteNode's parent (deleteNode's right child)
 * @param xFather: x's father to update
 */
void disconnectNodeWithChild(RBTree* tree, int childSide, Node* nodeFather, Node* deleteNode, Node* replacement,
                             Node** xFather)
{
    (*xFather) = nodeFather;
    if (replacement != NULL)
    {
        replacement->parent = nodeFather; // node has real child
        if (childSide == LEFT) // connect replacement to node's left child
        {
            replacement->right = deleteNode->right;
        }
        else // connect replacement to node's right child
        {
            replacement->left = deleteNode->left;
        }
    }
    if (deleteNode == tree->root) // node is the root
    {
        tree->root = replacement;
    }
    else // node is not the root
    {
        if (deleteNode == nodeFather->left) // node is left child
        {
            nodeFather->left = replacement;
        }
        else // node is right child
        {
            nodeFather->right = replacement;
        }
    }
}

/**
 * @brief case 0 of deletion
 * @param x - node to fix in the tree
 */
void deleteCase0(Node* x)
{
    x->color = BLACK;
}


/**
 * @brief case 1 of deletion
 * @param x - node to fix in the tree
 * @param w - brother of x
 * @param tree: node's tree
 * @param xFather: x's father to update
 */
void deleteCase1(RBTree* tree, Node* xFather, Node* x, Node** w)
{
    if (*w != NULL)
    {
        (*w)->color = BLACK;
    }
    xFather->color = RED;
    (x == xFather->left) ? rotateLeft(tree, xFather) : rotateRight(tree, xFather);
    (x == xFather->left) ? ((*w) = xFather->right) : ((*w) = xFather->left);
}

/**
 * @brief case 2 of deletion
 * @param x - pointer node to fix in the tree
 * @param xFather: x's father to update
 * @param w - brother of x
 */
void deleteCase2(Node** x, Node* xFather, Node* w)
{
    if (w != NULL)
    {
        w->color = RED;
    }
    *x = xFather;
}


/**
 * @brief case 3 of deletion
 * @param tree: nodes's tree
 * @param xFather: x's father to update
 * @param x - node to fix in the tree
 * @param w - pointer to brother of x
 */
void deleteCase3(RBTree* tree, Node* xFather, Node* x, Node** w)
{
    (x == xFather->left) ? ((*w)->left->color = BLACK) : ((*w)->right->color = BLACK);
    (*w)->color = RED;
    (x == xFather->left) ? rotateRight(tree, *w) : rotateLeft(tree, *w);
    (x == xFather->left) ? ((*w) = xFather->right) : ((*w) = xFather->left);
    deleteCase4(tree, xFather, x, *w);
}

/**
 * @brief case 4 of deletion
 * @param tree: nodes's tree
 * @param xFather: x's father to update
 * @param x - node to fix in the tree
 * @param w - pointer to brother of x
 */
void deleteCase4(RBTree* tree, Node* xFather, Node* x, Node* w)
{
    w->color = xFather->color;
    xFather->color = BLACK;
    if ((x == xFather->left) && (w->right != NULL))
    {
        w->right->color = BLACK;
    }
    else if ((x == xFather->right) && (w->left != NULL))
    {
        w->left->color = BLACK;
    }

    (x == xFather->left) ? rotateLeft(tree, xFather) : rotateRight(tree, xFather);
}

/**
 * @brief fix tree after deletion
 * @param tree: node's tree
 * @param xFather: x's father to update
 * @param x - pointer the node to fix after remove of deleted node
 */
void deleteFix(RBTree* tree, Node** xFather, Node** x)
{
    bool fatherExist = false;
    Node* w = NULL;
    if ((*xFather) != NULL)
    {
        w = getBrother((*xFather), *x);
        fatherExist = true;
    }
    // ---- case 0 ------
    if (((*x) != NULL) && isRed(*x))
    {
        deleteCase0(*x);
        return;
    }

    // ---- case 1 -------
    else if (fatherExist && isBlack(*x) && isRed(w))
    {
        deleteCase1(tree, (*xFather), *x, &w);
    }

    // ---- case 2 -------
    if ((fatherExist && isBlack(*x) && (w == NULL)) ||
        (fatherExist && isBlack(*x) && isBlack(w) && (isBlack(w->left) && isBlack(w->right))))
    {
        deleteCase2(x, (*xFather), w);
        if ((*x) != NULL)
        {
            (*xFather) = (*x)->parent;
        }
        if (((*x) != NULL) && isRed(*x))
        {
            (*x)->color = BLACK;
            return;
        }
        deleteFix(tree, xFather, x);
    }

    // ---- case 3 -------
    else if (fatherExist && (isBlack(*x) && isBlack(w)) && (((*x == (*xFather)->left) && isRed(w->left) && isBlack
             (w->right)) || ((*x == (*xFather)->right) && isRed(w->right) && isBlack(w->left))))
    {
        deleteCase3(tree, (*xFather), *x, &w);
    }

    // ---- case 4 -------
    else if (fatherExist && (isBlack(*x) && isBlack(w)) &&
             (((*x == (*xFather)->left) && isRed(w->right)) || ((*x == (*xFather)->right) && isRed(w->left))))
    {
        deleteCase4(tree, (*xFather), *x, w);
        return;
    }
}

/**
 * remove an item from the tree
 * @param tree: the tree to remove an item from.
 * @param data: item to remove from the tree.
 * @return: 0 on failure, other on success. (if data is not in the tree - failure).
 */
int deleteFromRBTree(RBTree *tree, void *data)
{
    if (tree == NULL || data == NULL) // check if input args are valid
    {
        return FAILURE;
    }
    if (RBTreeContains(tree, data) == NOT_FOUND) // if data doesnt exist
    {
        return FAILURE;
    }

    Node* deleteNode = findNode(tree, data);
    Node* replacement = NULL; // replacement node with deleteNode
    Node* x = NULL;
    Node* xFather = NULL;
    Node* replaceFather = NULL;
    int childSide = 0;

    //---------------- FINDING REPLACEMENT NODE x ---------------------
    if ((deleteNode->left != NULL) && ((deleteNode->right != NULL))) // deleteNode has 2 children
    {
        replacement = findChildSuccessor(deleteNode);
        xFather = replacement;
        childSide = RIGHT;
    }
    else if (deleteNode->left != NULL)
    {
        replacement = deleteNode->left;
        x = deleteNode->left; // deleteNode has only left child
        xFather = deleteNode;
        childSide = LEFT;
    }
    else if (deleteNode->right != NULL)
    {
        replacement = deleteNode->right;
        x = deleteNode->right; // deleteNode has only right child
        xFather = deleteNode;
        childSide = RIGHT;
    }
    else // deleteNode has 2 NULL children
    {
        xFather = deleteNode;
    }

    if (replacement != NULL)
    {
        x = replacement->right; // save replacement's right child as x
        replaceFather = replacement->parent;
    }
    else
    {
        replaceFather = deleteNode;
    }

    // ------------------------ CHECK THE COLORS ------------------------------------------

    Node* temp = replacement;
    if (isRed(deleteNode) && ((replacement == NULL) || isRed(replacement)))
    {
        disconnectNodeWithChild(tree, childSide, replaceFather, replacement, x, &xFather);
        swapNode(tree, deleteNode, temp, &xFather);
    }
    else if (isRed(deleteNode) && isBlack(replacement))
    {
        xFather = replacement;
        disconnectNodeWithChild(tree, childSide, replaceFather, replacement, x, &xFather);
        swapNode(tree, deleteNode, temp, &xFather);
        if (replacement != NULL)
        {
            replacement->color = RED;
        }
        deleteFix(tree, &xFather, &x);
    }
    else if (isBlack(deleteNode) && isRed(replacement))
    {
        disconnectNodeWithChild(tree, childSide, replaceFather, replacement, x, &xFather);
        swapNode(tree, deleteNode, temp, &xFather);
        if (replacement != NULL)
        {
            replacement->color = BLACK;
        }
    }
    else if (isBlack(deleteNode) && ((replacement == NULL) || isBlack(replacement)))
    {
        disconnectNodeWithChild(tree, childSide, replaceFather, replacement, x, &xFather);
        swapNode(tree, deleteNode, temp, &xFather);
        if (x != NULL)
        {
            xFather = (*x).parent;
        }
        deleteFix(tree, &xFather, &x);
    }


    tree->freeFunc(deleteNode->data);
    free(deleteNode);
    tree->size--;

    return SUCCESS;
}
