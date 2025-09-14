#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_IDS 100
int ID = 1;

typedef struct Book {
    int id;
    char title[50];
    char author[50];
    int isBorrowed;
    struct Book *right;
    struct Book *left;
} Book;
typedef struct BookNode {
    Book *book;
    struct BookNode *next;
} BookNode;
typedef struct User{
    int id;
    char name[50];
    struct User *next;
    struct User *prev;
    BookNode *borrowedHead;
    BookNode *borrowedTail;
} User;
typedef struct List{
    User *head;
    User *tail;
}List;

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
Book *creatBook(char *title, char *author, int id){
    Book *book = (Book *)malloc(sizeof(Book));
    if (book != NULL){
        book->id = id;
        strcpy(book->title, title);
        strcpy(book->author, author);
        book->isBorrowed = 0;
        book->left = book->right = NULL;
    }
    return book;
}
//insert book to BST
void insertBook(Book **head, char *title, char *author, int id){
    if (*head == NULL){
        *head = creatBook(title, author, id);
        printf("Book added sucsesfully!\n");
        return;
    }
    if (id > (*head)->id){
        insertBook(&((*head)->right), title, author, id);
    }
    else{
        insertBook(&((*head)->left), title, author, id);
    }
}
//find min id
Book *minID(Book *head){
    Book *temp = head;
    while (temp->left != NULL){
        temp = temp->left;
    }
    return temp;
}
//display book in inorder 
void inorder(Book *head){
    if (head == NULL) return;
    inorder(head->left);
    if (head->isBorrowed == 0) printf("Id: %d, %s By %s\n", head->id, head->title, head->author);
    inorder(head->right);

}
//remove book from bst
Book *removeBook(Book *head, int id){
    if (head == NULL){  
        printf("Book not Found!!\n"); 
        return head;
    }
    if (id > head->id){
        head->right = removeBook(head->right, id);
    }
    else if (id < head->id){
        head->left = removeBook(head->left, id);
    }
    else{
        //case 1 : no children
        if (head->left == NULL && head->right == NULL){
            free(head);
            head = NULL;
            printf("Book Deleted Succesfuly!!\n");
            return NULL;
        }
        //case 2 : only left
        else if (head->right == NULL){
            Book *temp = head->left;
            free(head);
            printf("Book Deleted Succesfuly!!\n");
            return temp;
        }
        //case 2: only right child
        else if (head->left == NULL){
            Book *temp = head->right;
            free(head);
            printf("Book Deleted Succesfuly!!\n");
            return temp;
        }
        //case 3: two children
        else {
            Book *temp = minID(head->right);
            strcpy(head->author, temp->author);
            strcpy(head->title, temp->title);
            head->id = temp->id;
            head->right = removeBook(head->right , temp->id);
        }
    }
    return head;
}
//search Book by id
Book *searchBookByID(Book *head, int id){
    if (head == NULL){
        printf("Book Not Found!!\n");
        return NULL;
    }
    if (id > head->id) head = searchBookByID(head->right, id);
    else if (id < head->id) head = searchBookByID(head->left, id);
    return head;
}


//user management system
//register a user
User *createUser(char *name){
    User *user = (User *)malloc(sizeof(User));
    if (user != NULL){
        user->id = ID++;
        strcpy(user->name, name);
        user->next = user->prev = NULL;
        user->borrowedHead = user->borrowedTail = NULL;
    }
    return user;
}
void registerUser(List *list, char *name){
    User *user = createUser(name);
    //insert at head at the end
    user->next = NULL;
    user->prev = list->tail;
    if (list->tail == NULL) list->head = user;
    else list->tail->next = user;
    list->tail = user;
    printf("User added succesfuly!!\n");
}
//delet User
void deletUser(List *list, int id){
    //list is empty
    if (list->head == NULL){
        printf("there's no users in List!!\n");
        return;
    }

    User *current = list->head;
    while (current != NULL){
        if (current->id == id){
            //element in hed
            if (current == list->head){
                list->head = list->head->next;
                if (list->head != NULL) list->head->prev = NULL;
                else list->tail = NULL;
            }
            //deleting tail
            else if (current == list->tail){
                list->tail = current->prev;
                if (list->tail != NULL) list->tail->next = NULL;
                else list->head = NULL;
            }
            //elemten in middle
            else{
                current->next->prev = current->prev;
                current->prev->next = current->next;
            }
            free(current);
            printf("user deleted succesfuly!!\n");
            return;
        }
        current = current->next;
    }
    printf("User not Found!!\n");
    return;
}
//searche users BY id
User *searchUserByID(List *list, int id){
    if (list->head == NULL){
        printf("list empty!!\n");
        return NULL;
    }
    User *current = list->head;
    while (current != NULL){
        if (current->id == id){
            return current;
        }
        current = current->next;
    }
    printf("User not Found!!\n");
    return NULL;
}
//display all users
void displayUsers(List *list){
    if (list->head == NULL){
        printf("list is empty!!\n");
        return;
    }
    User *current = list->head;
    while (current != NULL){
        printf("ID: %d, %s\n", current->id, current->name);
        current = current->next;
    }
    return;
}
//borrow system management

void borrowBook(User *user, Book *book){
    BookNode *node = (BookNode *)malloc(sizeof(BookNode));
    node->book = book;
    node->next = NULL;
    if (user->borrowedHead == NULL){
        user->borrowedHead = user->borrowedTail = node; //queen empty
    }
    else {
        user->borrowedTail->next = node;
        user->borrowedTail = node;
    }
}
Book *returnBook(User *user){
    if (user->borrowedHead == NULL){ //quene empty
        printf("user didn't borrow any book\n");
        return NULL;
    }
    BookNode *temp = user->borrowedHead;
    Book *book = temp->book;
    user->borrowedHead = temp->next;
    if (user->borrowedHead == NULL) user->borrowedTail = NULL; //quene is empty
    free(temp);
    return book;
}



int main(){
    Book *Books = NULL;
    List list;
    list.head = list.tail = NULL;
    printf("========= Library Menu ==============\n");
    printf("1. Add Book\n2. Delete Book\n3. Search Book\n4. Register User\n5. Borrow Book\n6. Return Book\n7. Show All Books\n8. Undo Last Operation\n0. Exit\n");
    while (1){
        int option, id, bookID, userID;
        char title[50], author[50], name[50];
        printf("->: ");
        scanf("%d", &option);
        switch (option) {
            case 1:
                printf("Enter titel: ");
                scanf("%s", title);
                printf("Enter author name: ");
                scanf("%s", author);
                insertBook(&Books, title, author, randID(1, 100));
                break;
            case 2:
                printf("Enter Book ID: ");
                scanf("%d", &id);
                Books = removeBook(Books, id);
                break;
            case 3:
                printf("Enter Book ID: ");
                scanf("%d", &id);
                searchBookByID(Books, id);
                break;
            case 4:
                printf("Enter User name: ");
                scanf("%s", name);
                registerUser(&list, name);
                break;
            case 5:
                printf("Enter User ID: ");
                scanf("%d", &userID);
                printf("Enter Book ID: ");
                scanf("%d", &bookID);
                Book *book = searchBookByID(Books, bookID);
                if (book != NULL){
                    if (book->isBorrowed == 1){
                        printf("Book already Borrowed!!\n");
                    }
                    else{
                        book->isBorrowed = 1;
                        borrowBook(searchUserByID(&list, userID), book);
                    }
                }
                break;
            case 6:
                printf("Enter User ID: ");
                scanf("%d", &userID);
                book = returnBook(searchUserByID(&list, userID));
                if (book != NULL){
                    book->isBorrowed = 0;
                }
                break;
            case 7:
                if (Books == NULL) printf("Library is empty!!\n");
                inorder(Books);
                break;
            case 8:
                displayUsers(&list);
                break;
            case 0:
                exit(0);
                break;
            default:
                break;
        }
    }

}
 