# Restaurant Management System

This project is a simple Restaurant Management System implemented in C. It allows customers to view the menu, place orders, and pay for their meals, while administrators can manage the menu, view payments, and generate bills for tables.

## Features

### Customer Panel
1. **Show Menu**: Displays the list of available food items.
2. **Place Order**: Allows customers to place orders.
3. **Search Food by Name**: Search for a food item by name.

### Admin Panel
1. **Show Menu**: Displays the list of available food items.
2. **Add Food**: Add new food items to the menu.
3. **Remove Food**: Remove food items from the menu.
4. **Show Bill for Table**: Generates a bill for a specific table.
5. **Show Payments**: Displays all payments made via credit card.


### Data Structures

1. **Food**: Linked list to store food items.
2. **Queue**: Queue to manage orders.
3. **Stack**: Stack to manage user navigation (customer/admin).
4. **PaymentNode**: Binary Search Tree to store payment details.

### Functions

1. **addFood**: Adds a new food item to the menu.
2. **removeFood**: Removes a food item from the menu.
3. **showMenu**: Displays the menu.
4. **placeOrder**: Places an order.
5. **searchFoodByName**: Searches for a food item by name.
6. **showBill**: Shows the bill for a specific table.
7. **insertPayment**: Inserts a payment into the payment tree.
8. **displayPayments**: Displays all payments.

## Default Menu
The program starts with a default menu which includes items like Burger, Cheeseburger, Veggie Burger, etc.
