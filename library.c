#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_IDS 100

typedef struct Book {
    int id;
    char title[50];
    char author[50];
    int copies;
    struct Book *right;
    struct Book *left;
} Book;


typedef struct User{
    int id;
    char name[50];
} User;
User *Borrowed = NULL;
User *history = NULL;

//random ID generator
int usedIDs[MAX_IDS];
int usedCount = 0;

int idIsUnique(int id){
    for (int i = 0; i < usedCount ; i++){
        if (usedIDs[i] == id){
            return 0;
        }
    }
    return 1;
}
int randID(int min, int max){
    int id;
    do {
        id = min + rand() % (max - min + 1);
    } while (!idIsUnique(id));

    usedIDs[usedCount++] = id;
    return id;
}

// books Managent
//creat Book
Book *creatBook(char *title, char *author , int copies){
    Book *book = (Book *)malloc(sizeof(Book));
    if (book != NULL){
        book->id = randId(1, MAX_IDS);
        strcpy(book->title, title);
        strcpy(book->author, author);
        book->copies = copies;
        book->left = book->right = NULL;
    }
    return book;
}
//insert book to BST
void insertBook(Book **head, char *title, char *author , int copies){
    Book *book = creatBook(title, author, copies);
    if (*head == NULL){
        *head = book;
        printf("Book added sucsesfully!\n");
        return;
    }
    if (book->id > (*head)->id){
        insertBook(&((*head)->right), title, author, copies);
    }
    else{
        insertBook(&((*head)->left), title, author, copies);
    }

}


int main(){
    int option;
    Book *Books = NULL;
    printf("========= Library Menu ==============\n");
    printf("1. Add Book\n2. Delete Book\n3. Search Book\n4. Register User\n5. Borrow Book\n6. Return Book\n7. Show All Books\n8. Undo Last Operation\n9. Exit\n");
    while (1){
        printf("->: ");
        scanf("%d", &option);
        switch (option) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                break;
            case 0:
                break;
            default:
                break;
        }
    }

}
