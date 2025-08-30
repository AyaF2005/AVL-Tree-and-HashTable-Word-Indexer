
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structure for AVL tree node
typedef struct node *AVLtree;
struct node
{
    char word[100];
    int frequency;
    AVLtree left;
    AVLtree right;
    int height;
};

// creat new node for AVL tree
AVLtree NewTreeNode(char* word){
    AVLtree new = (AVLtree)malloc(sizeof(struct node));
    strcpy(new->word, word);
    new->frequency = 1;
    new->left = NULL;
    new->right = NULL;
    new->height = 1;
    return new;
}

// Function to get the height of the tree
int height(AVLtree t){
    if (t == NULL)
        return 0;
    return t->height;
}

// Function to get the maximum of two integers
int MAX (int a, int b){
    if (a > b)
        return a;
    return b;

}
// Function to get the balance factor of the tree node
    int GetTheBalance(AVLtree t){
    if (t == NULL)
        return 0;
    return height(t->left) - height(t->right);
}


// Function to left rotate subtree rooted with x , x is the root of the subtree
AVLtree RotateToTheLeft(AVLtree x){
    AVLtree t1 = x->right; // t1 is the right child of x
    AVLtree t2 = t1->left; // t2 is the left child of t
    t1->left = x;
    x->right = t2;
    // Update heights by adding 1 to the maximum height of the left and right children of x and t1
    x->height = 1+ MAX(height(x->left), height(x->right));
    t1->height = 1+ MAX(height(t1->left), height(t1->right));

    return t1; // return new root
}


// Function to right rotate subtree rooted with y , y is the root of the subtree
AVLtree RotateToTheRight(AVLtree y){
    AVLtree t1 = y->left; // t1 is the left child of y
    AVLtree t2 = t1->right; // t2 is the right child of t
    // the rotation process
    t1->right = y;
    y->left = t2;

    // Update heights by adding 1 to the maximum height of the left and right children of y and t1
    y->height = 1+ MAX(height(y->left), height(y->right));
    t1->height = 1+ MAX(height(t1->left), height(t1->right));

    // Return new root
    return t1;
}
// Function to insert a word to the AVL tree
void InsertWordToAVLTree(AVLtree *t, char* word){ // passes the address of the root of the tree and the word to be inserted
    // If the tree is empty, create a new node
    if (*t == NULL){
        *t = NewTreeNode(word);
        return;
    }
    // Compare the word with the word of the current node

    if (strcmp(word, (*t)->word) < 0)
        InsertWordToAVLTree(&(*t)->left, word); // insert to the left subtree ,& is used to pass the address of the left child
    else if (strcmp(word, (*t)->word) > 0)
        InsertWordToAVLTree(&(*t)->right, word); // insert to the right subtree ,& is used to pass the address
    else
        (*t)->frequency++; // if the word is already in the tree, increment its frequency

    // Update the height of the current node by adding 1 to the maximum height of its left and right children
    (*t)->height = 1 + MAX(height((*t)->left), height((*t)->right));

    // Get the balance factor of the current node to check if it is unbalanced or not
    int balance = GetTheBalance(*t);
    // single rotation cases
    // Left Left Case
    if (balance > 1 && strcmp(word, (*t)->left->word) < 0)
        *t = RotateToTheRight(*t);
    // Right Right Case
    if (balance < -1 && strcmp(word, (*t)->right->word) > 0)
        *t = RotateToTheLeft(*t);

    // double rotation cases
    // Left Right Case
    if (balance > 1 && strcmp(word, (*t)->left->word) > 0){
        (*t)->left = RotateToTheLeft((*t)->left);
        *t = RotateToTheRight(*t);
    }
    // Right Left Case
    if (balance < -1 && strcmp(word, (*t)->right->word) < 0){
        (*t)->right = RotateToTheRight((*t)->right);
        *t = RotateToTheLeft(*t);
    }
}


// function that reads only words from the input.txt file to then insert to the AVL tree
void ReadWordsFromFile(AVLtree *t){ // pass the address of the root of the tree to insert the words to it
    FILE *file = fopen("input.txt", "r");
    if (file == NULL){
        printf("File is not found\n");
        return;
    }
    char word[100];
    while (fscanf(file, "%s", word) != EOF){ // EOF is the end of file
        // Convert the word to lower case and remove any non-alphabetical characters
        int i = 0;
        while (word[i] != '\0'){
            if (word[i] >= 'A' && word[i] <= 'Z')
                word[i] += 32;
            if (word[i] < 'a' || word[i] > 'z'){
                for (int j = i; word[j] != '\0'; j++)
                    word[j] = word[j+1];
            }
            else
                i++;
        }
        // check if tree is empty or not to insert the word to the AVL tree
        if (*t == NULL)
            *t = NewTreeNode(word);
        else
            InsertWordToAVLTree(t, word);
    }
    fclose(file);
}
// Function to delet by searching a word in the AVL tree then delete it

void DeleteWordFromAVLTree(AVLtree *t, char* word) { // pass the address of the root of the tree and the word to be deleted
    // if the word is not found in the tree
    if (*t == NULL) {
        printf("The word %s is not found !\n", word);
        return;
    }
    // Compare the word with the word of the current node to search for the word to be deleted

    if (strcmp(word, (*t)->word) < 0)
        DeleteWordFromAVLTree(&(*t)->left, word);
    else if (strcmp(word, (*t)->word) > 0)
        DeleteWordFromAVLTree(&(*t)->right, word);
    else {
        // if the frequency of the word is more than 1, decrement its frequency
        if ((*t)->frequency > 1)
            (*t)->frequency--;
        else {

            if ((*t)->left == NULL || (*t)->right == NULL) {

                AVLtree temp; // create a temporary node
                if ((*t)->left == NULL) {
                    temp = (*t)->right;
                } else {
                    temp = (*t)->left;
                }

                if (temp == NULL) {
                    temp = *t;
                    *t = NULL;
                } else {
                    strcpy((*t)->word, temp->word); // copy the word from the child node to the node to be deleted
                    (*t)->frequency = temp->frequency; // copy the frequency from the child node to the node to be deleted
                    (*t)->left = temp->left;
                    (*t)->right = temp->right;
                    (*t)->height = temp->height;
                }
                free(temp);
            } else {
                AVLtree temp = (*t)->right;
                while (temp->left != NULL)
                    temp = temp->left;
                strcpy((*t)->word, temp->word);
                (*t)->frequency = temp->frequency;
                DeleteWordFromAVLTree(&(*t)->right, temp->word);
            }
        }
    }
    if (*t == NULL)
        return;
    // Update the height of the current node by adding 1 to the maximum height of its left and right children
    (*t)->height = 1 + MAX(height((*t)->left), height((*t)->right));
    // Get the balance factor of the current node to check if it is unbalanced or not
    int balance = GetTheBalance(*t);
    // single rotation cases
    // Left Left Case
    if (balance > 1 && GetTheBalance((*t)->left) >= 0)
        *t = RotateToTheRight(*t);
    // Right Right Case
    if (balance < -1 && GetTheBalance((*t)->right) <= 0)
        *t = RotateToTheLeft(*t);
    // double rotation cases
    // Left Right Case
    if (balance > 1 && GetTheBalance((*t)->left) < 0) {
        (*t)->left = RotateToTheLeft((*t)->left);
        *t = RotateToTheRight(*t);
    }
    // Right Left Case
    if (balance < -1 && GetTheBalance((*t)->right) > 0) {
        (*t)->right = RotateToTheRight((*t)->right);
        *t = RotateToTheLeft(*t);
    }
}


// Function to print the words and their frequencies using in-order traversal
void InorderPrint(AVLtree t){
    if (t == NULL)
        return;
    InorderPrint(t->left);
    printf("%s %d\n", t->word, t->frequency);
    InorderPrint(t->right);
}
// now will implement the hash table to store the words and their frequencies from the AVL tree

#define HashTableSize 10

// Hash Structure
typedef struct Hashnode *Hash;
struct Hashnode
{
    char word[100];
    int frequency;
    Hash next;
};
Hash HashTable[HashTableSize];

// Function to get the hash value of a word using the sum of the ASCII values of its characters
int HashFunction(char* word){
    int HashVal = 0;
    while (*word != '\0'){
        HashVal += *word;
        word++;
    }
    return HashVal % HashTableSize;
}

// REhashing after adding elements mor than 75% of the size of the hash table
void Rehash(){
    // Create a temporary hash table to store the elements of the current hash table
    Hash   temp[HashTableSize];
    // Copy the elements of the current hash table to the temporary hash table
    for (int i = 0; i < HashTableSize; i++)
        temp[i] = HashTable[i];
    // Initialize the current hash table
    for (int i = 0; i < HashTableSize; i++)
        HashTable[i] = NULL;
    // Rehash the elements of the temporary hash table to the current hash table
    for (int i = 0; i < HashTableSize; i++){
        Hash t = temp[i];
        while (t != NULL){
            Hash next = t->next;
            t->next = NULL;
            int index = HashFunction(t->word);
            if (HashTable[index] == NULL)
                HashTable[index] = t;
            else{
                Hash temp = HashTable[index]; // create a temporary node to store the elements of the hash table
                while (temp->next != NULL)
                    temp = temp->next;
                temp->next = t;
            }
            t = next;
        }
    }
}


// Function to insert a word to the hash table and call rehashing if needed
void InsertWordToHashTable(char* word, int frequency){
    int index = HashFunction(word);
    Hash Head = HashTable[index];
    while (Head != NULL){
        if (strcmp(Head->word, word) == 0){
            Head->frequency += frequency;
            return;
        }
        Head = Head->next;
    }
    Hash new = (Hash)malloc(sizeof(struct Hashnode));
    strcpy(new->word, word);
    new->frequency = frequency;
    new->next = NULL;
    if (HashTable[index] == NULL)
        HashTable[index] = new;
    else{
        Hash temp = HashTable[index];
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new;
    }
    int count = 0;
    for (int i = 0; i < HashTableSize; i++){
        Hash temp = HashTable[i];
        while (temp != NULL){
            count++;
            temp = temp->next;
        }
    }
    if (count > 0.75 * HashTableSize){
        printf("Rehashing is done\n");
        Rehash();
    }
}

// Function to create the hash table
void CreateHashTable(AVLtree t){
    if (t == NULL)
        return;

    CreateHashTable(t->left); // create the hash table for the left subtree
    InsertWordToHashTable(t->word, t->frequency); // insert the word to the hash table
    CreateHashTable(t->right);// create the hash table for the right subtree
}

// Function to search for a word in the hash table and print its frequency
void SearchForWordInHT(char* word){
    // Calculate the hash value of the word
    int index = HashFunction(word);
    // Start from the head of the linked list at the calculated hash index
    Hash temp = HashTable[index];
    // Traverse the linked list at the calculated hash index
    while (temp != NULL){
        if (strcmp(temp->word, word) == 0){
            printf("The word %s is found at index %d with frequency %d\n", word, index, temp->frequency);
            return;
        }
        // Move to the next node in the linked list
        temp = temp->next;
    }
    printf("The word %s is not found in hash table\n", word);
}


// Function to delete a word from the hash table
void DeleteWordFromHashTable(char* word){
    int index = HashFunction(word);
    Hash temp = HashTable[index];
    Hash prev = NULL;
    while (temp != NULL){
        if (strcmp(temp->word, word) == 0){
            if (temp->frequency > 1)
                temp->frequency--;
            else{
                if (prev == NULL)
                    HashTable[index] = temp->next;
                else
                    prev->next = temp->next;
                free(temp);
                printf("The word %s is deleted from the hash table\n", word);
            }
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("The word %s is not found in the hash table\n", word);
}

// Function to print the words statistics
void PrintWordsStatistics(){
    int alluni = 0;
    int max = 0;
    char mostFrequentWord[100];

    for (int i = 0; i < HashTableSize; i++){
        Hash temp = HashTable[i];
        while (temp != NULL){
            if (temp->frequency == 1) {
                alluni++;
            }
            if (temp->frequency > max){
                max = temp->frequency;
                strcpy(mostFrequentWord, temp->word);
            }
            temp = temp->next;
        }
    }

    printf("The total number of unique words(the freq is 1 )  is %d\n", alluni );
    printf("The most frequent word is '%s' and it is repeated %d times\n", mostFrequentWord, max);

    // Add user input for a specific threshold
    int threshold;
    printf("Enter a threshold to find words repeated more than it: ");
    scanf("%d", &threshold);
    printf("The words repeated more than %d times are:\n", threshold);

    for (int i = 0; i < HashTableSize; i++){
        Hash temp = HashTable[i];
        while (temp != NULL){
            if (temp->frequency > threshold)
                printf("%s %d\n", temp->word, temp->frequency);
            temp = temp->next;
        }
    }
}


// Function to print the menu
void PrintMenu(){
    printf("1. Load data from the file.\n");
    printf("2. Create the AVL tree.\n");
    printf("3. Insert a word to the AVL tree.\n");
    printf("4. Delete a word from the AVL tree.\n");
    printf("5. Print the words as sorted in the AVL tree.\n");
    printf("6. Create the Hash Table.\n");
    printf("7. Insert a word to the Hash table.\n");
    printf("8. Delete a word from the hash table.\n");
    printf("9. Search for a word in the hash table and print its frequency.\n");
    printf("10. Print words statistics.\n");
    printf("11. Exit the application.\n");
}

// Main function
int main(){
    AVLtree t = NULL;
    int choice;
    while (1){
        PrintMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            ReadWordsFromFile(&t);
            printf("The words are read from the file ! \n");
        }
        else if (choice == 2)
            printf("The AVL tree is created ! \n");
        else if (choice == 3){
            char word[100];
            printf("Enter the word: ");
            scanf("%s", word);
            InsertWordToAVLTree(&t, word);
            printf("The word %s is inserted\n", word);
        }
        else if (choice == 4){
            char word[100];
            printf("Enter the word: ");
            scanf("%s", word);
            DeleteWordFromAVLTree(&t, word);
            printf("The word %s is deleted\n", word);
        }
        else if (choice == 5){
            printf("The words and their frequencies are in Inorder :\n");
            InorderPrint(t);
        }
        else if (choice == 6){
            CreateHashTable(t);
            printf("The hash table is created\n");
        }
        else if (choice == 7){
            char word[100];
            printf("Enter the word: ");
            scanf("%s", word);
            InsertWordToHashTable(word , 1);
            printf("The word %s is inserted to the hash table\n", word);
        }
        else if (choice == 8){
            char word[100];
            printf("Enter the word: ");
            scanf("%s", word);
            DeleteWordFromHashTable(word);
            printf("The word %s is deleted from the hash table\n", word);
        }
        else if (choice == 9){
            char word[100];
            printf("Enter the word: ");
            scanf("%s", word);
            printf("searching for the word %s ... \n", word);
            SearchForWordInHT(word);
        }
        else if (choice == 10)
            PrintWordsStatistics();
        else if (choice == 11)
            break;
        else
            printf("Invalid choice\n");
    }
    return 0;
}







