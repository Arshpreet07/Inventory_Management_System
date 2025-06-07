#include <iostream>
#include <memory>
#include "Electronics.h"
#include "Clothing.h"
#include "Shoes.h"
#include "DatabaseConnection.h"
#include "InventoryManager.h"
#include "CategoryRegistry.h"
#include "UserManager.h"

using namespace std;

void displayGeneralMenu(bool isLoggedIn, bool isAdmin) {
    cout << "\n=== Inventory Management System ===\n";
    if (!isLoggedIn) {
        cout << "1. Login\n";
        cout << "2. Exit\n";
    } else {
        cout << "1. Logout\n";
        cout << "2. View Inventory\n";
        cout << "3. Search Item by ID\n";
        cout << "4. Search by Category\n";
        cout << "5. Search by Name\n";
        cout << "6. Search by Price Range\n";
        cout << "7. Search by Stock Level\n";
        cout << "8. Process Sale\n";
        cout << "9. Process Restock\n";
        cout << "10. Auto Restock\n";

        if (isAdmin) {
            cout << "\n--- Admin Options ---\n";
            cout << "11. Add New Item\n";
            cout << "12. Update Stock\n";
            cout << "13. Delete Item\n";
            cout << "14. Apply Discount to Item\n";
            cout << "15. Apply Discount to Category\n";
            cout << "16. Add User\n";
            cout << "17. Bulk Update from File\n";
            cout << "18. Clear Entire Inventory\n";
        }
        cout << "0. Exit\n";
    }
    cout << "Select an option: ";
}

int main() {
    try {
        auto conn = DatabaseConnection::getConnection();
        InventoryManager inventoryManager(conn);
        UserManager userManager(conn);

        int choice;
        do {
            displayGeneralMenu(userManager.getCurrentUser() != nullptr, userManager.isAdmin());
            cin >> choice;

            if (!userManager.getCurrentUser()) {
                switch (choice) {
                    case 1: {
                        string username, password;
                        cout << "Username: "; cin >> username;
                        cout << "Password: "; cin >> password;
                        userManager.login(username, password);
                        break;
                    }
                    case 2:
                        cout << "Exiting program.\n";
                        return 0;
                    default:
                        cout << "Invalid option. Please login first.\n";
                }
            } else {
                switch (choice) {
                    case 0:
                        cout << "Exiting program.\n";
                        return 0;
                    case 1:
                        userManager.logout();
                        break;
                    case 2:
                        cout << endl;
                        inventoryManager.displayAllItems();
                        break;
                    case 3: {
                        int id;
                        cout << "Enter Item ID: ";
                        cin >> id;
                        cout << endl;
                        inventoryManager.searchItem(id);
                        break;
                    }
                    case 4: {
                        string category;
                        cout << "Enter Category: ";
                        cin >> category;
                        auto items = inventoryManager.searchByCategory(category);
                        cout << endl;
                        for (auto& item : items) item->displayItem();
                        break;
                    }
                    case 5: {
                        string name;
                        cout << "Enter Name: ";
                        cin >> name;
                        auto items = inventoryManager.searchByName(name);
                        cout << endl;
                        for (auto& item : items) item->displayItem();
                        break;
                    }
                    case 6: {
                        double min, max;
                        cout << "Enter Min Price: ";
                        cin >> min;
                        cout << "Enter Max Price: ";
                        cin >> max;
                        auto items = inventoryManager.searchByPriceRange(min, max);
                        cout << endl;
                        for (auto& item : items) item->displayItem();
                        break;
                    }
                    case 7: {
                        int min, max;
                        cout << "Enter Min Stock: ";
                        cin >> min;
                        cout << "Enter Max Stock: ";
                        cin >> max;
                        auto items = inventoryManager.searchByStockLevels(min, max);
                        cout << endl;
                        for (auto& item : items) item->displayItem();
                        break;
                    }
                    case 8: {
                        int id, qty;
                        cout << "Enter Item ID: "; cin >> id;
                        cout << "Quantity to Sell: "; cin >> qty;
                        cout << endl;
                        inventoryManager.processSale(id, qty);
                        break;
                    }
                    case 9: {
                        int id, qty;
                        cout << "Enter Item ID: "; cin >> id;
                        cout << "Quantity to Restock: "; cin >> qty;
                        cout << endl;
                        inventoryManager.processRestock(id, qty);
                        break;
                    }
                    case 10:
                        cout << endl;
                        inventoryManager.autoRestock();
                        break;

                    // Admin-only options
                    case 11: {
                        string category;
                        cout << "Enter Category (Clothing/Electronics/Shoes): ";
                        cin >> category;
                        if (category == "Clothing") {
                            int id, stock, threshold, restockAmount;
                            double price, discountVal;
                            string name, dtype, fabric, size;
                            cout << "ID, Name, Price, Stock, MinStock, Threshold, DiscountType, DiscountValue, Fabric, Size:\n";
                            cin >> id >> name >> price >> stock >> threshold >> restockAmount >> dtype >> discountVal >> fabric >> size;

                            Clothing* c = new Clothing(id, name, price, stock, threshold, restockAmount, dtype, discountVal, fabric, size);
                            inventoryManager.addItem(unique_ptr<InventoryItem>(c));
                        } 
                        else if (category == "Electronics") {
                            int id, stock, threshold, restockAmount, warranty;
                            double price, discountVal;
                            string name, dtype;
                            cout << "ID, Name, Price, Stock, Threshold, restockAmount, DiscountType, DiscountValue, Warranty (in months):\n";
                            cin >> id >> name >> price >> stock >> threshold >> restockAmount >> dtype >> discountVal >> warranty;

                            Electronics* e = new Electronics(id, name, price, stock, threshold, restockAmount, dtype, discountVal, warranty);
                            inventoryManager.addItem(unique_ptr<InventoryItem>(e));
                        } 
                        else if (category == "Shoes") {
                            int id, stock, threshold, restockAmount;
                            double price, discountVal;
                            string name, dtype, brand, size;
                            cout << "ID, Name, Price, Stock, MinStock, Threshold, DiscountType, DiscountValue, Brand, Size:\n";
                            cin >> id >> name >> price >> stock >> threshold >> restockAmount >> dtype >> discountVal >> brand >> size;

                            Shoes* s = new Shoes(id, name, price, stock, threshold, restockAmount, dtype, discountVal, brand, size);
                            inventoryManager.addItem(unique_ptr<InventoryItem>(s));
                        } 
                        else {
                            cout << "Invalid category. Please enter one of: Clothing, Electronics, Shoes.\n";
                        }
                        break;
                    }
                    case 12: {
                        int id, newStock;
                        cout << "Enter Item ID and New Stock: ";
                        cin >> id >> newStock;
                        cout << endl;
                        inventoryManager.updateStock(id, newStock);
                        break;
                    }
                    case 13: {
                        int id;
                        cout << "Enter Item ID to Delete: ";
                        cin >> id;
                        cout << endl;
                        inventoryManager.deleteItem(id);
                        break;
                    }
                    case 14: {
                        int id;
                        string type;
                        double value;
                        cout << "Item ID, Discount Type (Percentage/Flat), Discount Value:\n";
                        cin >> id >> type >> value;
                        Discount d{type, value};
                        cout << endl;
                        inventoryManager.applyDiscountToItem(id, d);
                        break;
                    }
                    case 15: {
                        string cat, type;
                        double value;
                        cout << "Category, Discount Type (Percentage/Flat), Discount Value:\n";
                        cin >> cat >> type >> value;
                        Discount d{type, value};
                        cout << endl;
                        inventoryManager.applyDiscountToCategory(cat, d);
                        break;
                    }
                    case 16: {
                        string uname, pass, role;
                        cout << "New Username, Password, Role (admin/employee):\n";
                        cin >> uname >> pass >> role;
                        cout << endl;
                        userManager.addUser(uname, pass, role);
                        break;
                    }
                    case 17: {
                        string filename;
                        cout << "Enter Bulk Update Filename (e.g., bulk_update.txt): ";
                        cin >> filename;
                        inventoryManager.bulkUpdate(filename);
                        break;
                    }
                    case 18: {
                        cout << "Are you sure you want to delete all inventory data? (yes/no): ";
                        string confirm;
                        cin >> confirm;
                        cout << endl;
                        if (confirm == "yes") {
                            inventoryManager.clearInventory(); 
                            cout << "All inventory data deleted.\n";
                        } else {
                            cout << "Operation cancelled.\n";
                        }
                        break;
                    }
                    default:
                        cout << "Invalid option.\n";
                }
            }

        } while (true);

    } catch (sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}