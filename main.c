


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>


#define ADMIN_PASSWORD "admin123"
void ekranaMetinYazdir(const char *metin) {
    int i = 0;
    while (metin[i] != '\0') {
        putchar(metin[i]);
        fflush(stdout);
        usleep(40000);
        i++;
    }
}

typedef struct Food {
    int foodNo;
    char name[50];
    float price;
    int stock;
    struct Food *next;
} Food;


typedef struct Queue {
    int tableNo;
    int foodNo;
    int quantity;
    struct Queue *next;
} Queue;


typedef struct Stack {
    int data;
    struct Stack *next;
} Stack;


typedef struct PaymentNode {
    char creditCardNumber[20];
    float amount;
    struct PaymentNode *left;
    struct PaymentNode *right;
} PaymentNode;


Food *head = NULL;


Queue *front = NULL;
Queue *rear = NULL;


int adminLoggedIn = 0;


Stack *top = NULL;


PaymentNode *paymentRoot = NULL;


void reorderFoodNumbers();


PaymentNode *insertPayment(PaymentNode *root, char creditCardNumber[], float amount) {

    if (root == NULL) {
        PaymentNode *newNode = (PaymentNode *)malloc(sizeof(PaymentNode));
        strcpy(newNode->creditCardNumber, creditCardNumber);
        newNode->amount = amount;
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    if (strcmp(creditCardNumber, root->creditCardNumber) < 0) {
        root->left = insertPayment(root->left, creditCardNumber, amount);
    } else if (strcmp(creditCardNumber, root->creditCardNumber) > 0) {
        root->right = insertPayment(root->right, creditCardNumber, amount);
    }
    return root;
}


void displayPayments(PaymentNode *root) {
    if (root != NULL) {
        displayPayments(root->left);
        printf("Credit Card Number: %s, Amount Paid: %.2f\n", root->creditCardNumber, root->amount);
        displayPayments(root->right);
    }
}


void addFood(char name[], float price, int stock) {
    Food *newFood = (Food *)malloc(sizeof(Food));
    static int foodCounter = 1;


    newFood->foodNo = foodCounter++;
    strcpy(newFood->name, name);
    newFood->price = price;
    newFood->stock = stock;
    newFood->next = NULL;


    if (head == NULL) {
        head = newFood;
    } else {
        Food *temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newFood;
    }


    reorderFoodNumbers();
}


void reorderFoodNumbers() {
    Food *current = head;
    int foodNo = 1;
    while (current != NULL) {
        current->foodNo = foodNo++;
        current = current->next;
    }
}


void removeFood(int foodNo) {
    if (head == NULL) {
        printf("\nMenu is empty!\n");
        return;
    }

    Food *current = head;
    Food *prev = NULL;

    while (current != NULL) {
        if (current->foodNo == foodNo) {
            if (prev == NULL) {
                head = current->next;
                free(current);
                printf("\nFood removed successfully.\n");
                break;
            } else {
                prev->next = current->next;
                free(current);
                printf("\nFood removed successfully.\n");
                break;
            }
        }
        prev = current;
        current = current->next;
    }


    reorderFoodNumbers();
}


void enqueue(int tableNo, int foodNo, int quantity) {
    Queue *newOrder = (Queue *)malloc(sizeof(Queue));
    newOrder->tableNo = tableNo;
    newOrder->foodNo = foodNo;
    newOrder->quantity = quantity;
    newOrder->next = NULL;

    if (front == NULL) {
        front = rear = newOrder;
    } else {
        rear->next = newOrder;
        rear = newOrder;
    }
}


int dequeue() {
    if (front == NULL) {
        printf("\nQueue is empty!\n");
        return -1;
    } else {
        int foodNo = front->foodNo;
        Queue *temp = front;
        front = front->next;
        free(temp);
        return foodNo;
    }
}



void showMenu() {
    printf("\033[0;34m");
    printf("\nMenu:\n");
    printf("| %-10s | %-15s | %-10s | %-10s |\n", "Food No.", "Food Name", "Price", "Stock");
    printf("----------------------------------------------\n");
    Food *current = head;
    while (current != NULL) {
        printf("| %-10d | %-15s | %-10.2f | %-10d |\n", current->foodNo, current->name, current->price, current->stock);
        printf("----------------------------------------------\n");
        current = current->next;
    }
     printf("\033[0m");
}



void adminLogin() {
    char password[20];
    do {
        printf("\nEnter the admin password (or type 'exit' to quit): ");
        scanf("%s", password);
        if (strcmp(password, "exit") == 0) {
            printf("Exiting...\n");
            exit(0);
        }
    } while (strcmp(password, ADMIN_PASSWORD) != 0);

    printf("Admin login successful!\n");
    adminLoggedIn = 1;
}


void placeOrder() {
    int tableNo, foodNo, quantity;
    printf("\nEnter the table number: ");
    scanf("%d", &tableNo);

    printf("\nEnter the food number to order (0 to stop ordering): ");
    scanf("%d", &foodNo);


    int selectedFoods[50];
    int quantities[50];
    int index = 0;


    while (foodNo != 0) {
        printf("Enter the quantity for food %d: ", foodNo);
        scanf("%d", &quantity);


        selectedFoods[index] = foodNo;
        quantities[index] = quantity;
        index++;

        printf("\nEnter the food number to order (0 to stop ordering): ");
        scanf("%d", &foodNo);
    }

    float totalAmount = 0;


    for (int i = 0; i < index; i++) {
        foodNo = selectedFoods[i];
        quantity = quantities[i];

        Food *current = head;
        while (current != NULL) {
            if (current->foodNo == foodNo) {
                if (current->stock >= quantity) {
                    current->stock -= quantity;
                    totalAmount += current->price * quantity;

                    break;
                } else {
                    printf("Sorry, only %d of %s left in stock.\n", current->stock, current->name);

                    break;
                }
            }
            current = current->next;
        }
    }

    printf("Total amount to pay: %.2f\n", totalAmount);
    printf("Payment Options:\n");
    printf("1. Credit Card\n");
    printf("2. Cash\n");
    printf("Enter your payment choice: ");
    int paymentChoice;
    scanf("%d", &paymentChoice);
    switch (paymentChoice) {
        case 1:
            printf("Enter your credit card number: ");
            char creditCardNumber[20];
            scanf("%s", creditCardNumber);
            paymentRoot = insertPayment(paymentRoot, creditCardNumber, totalAmount);
            printf("Payment received via Credit Card. Total amount: %.2f\n", totalAmount);
            break;
        case 2:
            printf("Payment received via Cash. Total amount: %.2f\n", totalAmount);
            break;
        default:
            printf("Invalid payment choice.\n");
            return;
    }


    for (int i = 0; i < index; i++) {
        enqueue(tableNo, selectedFoods[i], quantities[i]);
    }
}


void searchFoodByName(char *name) {
    Food *current = head;
    bool found = false;

    printf("\nSearch Results:\n");
    printf("| %-10s | %-15s | %-10s |\n", "Food No.", "Food Name", "Price");
    printf("--------------------------------\n");

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            printf("| %-10d | %-15s | %-10.2f |\n", current->foodNo, current->name, current->price);
            printf("--------------------------------\n");
            found = true;
        }
        current = current->next;
    }

    if (!found) {
        printf("No food found with the name '%s'.\n", name);
    }
}


void showBill(int tableNumber) {

    Queue *currentOrder = front;
    float totalAmount = 0;
    printf("\nBill for Table %d:\n", tableNumber);
    printf("| %-15s | %-10s | %-10s |\n", "Food Name", "Quantity", "Price");
    printf("--------------------------------\n");


    while (currentOrder != NULL) {
        if (currentOrder->tableNo == tableNumber) {
            Food *currentFood = head;
            while (currentFood != NULL) {
                                if (currentFood->foodNo == currentOrder->foodNo) {
                    printf("| %-15s | %-10d | %-10.2f |\n", currentFood->name, currentOrder->quantity, currentFood->price * currentOrder->quantity);
                    totalAmount += currentFood->price * currentOrder->quantity;
                    break;
                }
                currentFood = currentFood->next;
            }
        }
        currentOrder = currentOrder->next;
    }
    printf("--------------------------------\n");
    printf("Total Amount: %.2f\n", totalAmount);
}

void createDefaultMenu() {
    addFood("Burger", 6.99, 30);
    addFood("Cheeseburger", 8.99, 30);
    addFood("Veggie_Burger", 7.49, 30);
    addFood("Sandwich", 6.49, 20);
    addFood("Pizza", 6.99, 20);
    addFood("Chicken_Nuggets", 5.49, 100);
    addFood("French_Fries", 3.49, 100);
    addFood("Onion_Rings", 3.99, 100);
    addFood("Mozzarella_Stick", 4.99, 100);
    addFood("Soda", 2.49, 40);
    addFood("Iced_Tea", 3.99, 47);
    addFood("Milkshakes", 3.99, 50);
    addFood("Bottled_Water", 1.49, 60);
    addFood("Cookies", 1.99, 50);
    addFood("Apple_Pie", 2.49, 20);
    addFood("Ice_Cream", 3.49, 35);
}



void push(int data) {
    Stack *newNode = (Stack *)malloc(sizeof(Stack));
    newNode->data = data;
    newNode->next = top;
    top = newNode;
}


int pop() {
    if (top == NULL) {
        printf("\nStack is empty!\n");
        return -1;
    } else {
        int data = top->data;
        Stack *temp = top;
        top = top->next;
        free(temp);
        return data;
    }
}

int main() {
    createDefaultMenu();
     printf("\033[2J");
    printf("\033[0m");

    const char *metin = "\033[0;35mWhere Taste Meets Happiness :) \n\n\033[0m";
    ekranaMetinYazdir(metin);
    int choice;

    while (true) {
        int userType;
        printf("Welcome to our restaurant. Please select an option:\n");
        printf("1) Customer Panel\n");
        printf("2) Admin Panel\n");
        printf("Enter your choice: ");
        scanf("%d", &userType);

        switch (userType) {
            case 1:
                push(1);
                do {
                    printf("\nOptions:\n");
                    printf("1. Show Menu\n");
                    printf("2. Place Order\n");
                    printf("3. Search Food by Name\n");
                    printf("4. Back\n");
                    printf("Enter your choice: ");
                    scanf("%d", &choice);

                    switch(choice) {
                        case 1:
                            showMenu();
                            break;
                        case 2:
                            placeOrder();
                            break;
                        case 3:
                            {
                                char name[50];
                                printf("\nEnter the name of the food to search: ");
                                scanf("%s", name);
                                searchFoodByName(name);
                            }
                            break;
                        case 4:
                            pop();
                            break;
                        default:
                            printf("\nInvalid choice!\n");
                    }
                } while (choice != 4);
                break;
            case 2:
                {
                    char password[20];
                    printf("\nEnter the admin password (or type 'exit' to quit): ");
                    scanf("%s", password);
                    if (strcmp(password, ADMIN_PASSWORD) == 0) {
                        printf("Admin login successful!\n");
                        adminLoggedIn = 1;
                    } else if (strcmp(password, "exit") == 0) {
                        printf("Exiting...\n");
                        exit(0);
                    } else {
                        printf("Invalid password!\n");
                        break;
                    }
                }
                do {
                    printf("\nOptions:\n");
                    printf("1. Show Menu\n");
                    printf("2. Add Food\n");
                    printf("3. Remove Food\n");
                    printf("4. Show Bill for Table\n");
                    printf("5. Show Payments\n");
                    printf("6. Back\n");
                    printf("Enter your choice: ");
                    scanf("%d", &choice);

                    switch(choice) {
                        case 1:
                            showMenu();
                            break;
                        case 2:
                            {
                                char name[50];
                                float price;
                                int stock;
                                printf("\nEnter the name of the food: ");
                                scanf("%s", name);
                                printf("Enter the price of the food: ");
                                scanf("%f", &price);
                                printf("Enter the stock of the food: ");
                                scanf("%d", &stock);
                                addFood(name, price, stock);
                            }
                            break;
                        case 3:
                            {
                                int foodNo;
                                printf("\nEnter the food number to remove: ");
                                scanf("%d", &foodNo);
                                removeFood(foodNo);
                            }
                            break;
                        case 4:
                            {
                                int tableNo;
                                printf("\nEnter the table number to show bill: ");
                                scanf("%d", &tableNo);
                                showBill(tableNo);
                            }
                            break;
                        case 5:
                            printf("\nPayment Details:\n");
                            displayPayments(paymentRoot);
                            break;
                        case 6:
                            adminLoggedIn = 0;
                            break;
                        default:
                            printf("\nInvalid choice!\n");
                    }
                } while (choice != 6);
                break;
            default:
                printf("\nInvalid choice!\n");
        }
    }

    return 0;
}
