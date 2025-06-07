#include "InventoryManager.h"
#include "TransactionManager.h"
#include "AuditLogger.h"
#include "DatabaseConnection.h"
// #include "CategoryRegistry.h"

#include <iostream>
// #include <chrono>
// #include <ctime>
#include <memory>
#include <set>
using namespace std;

InventoryManager::InventoryManager(std::shared_ptr<sql::Connection> conn) : conn(std::move(conn)) {}

void InventoryManager::addItem(std::unique_ptr<InventoryItem> item){
    try {
        std::string category = item->getCategory();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("INSERT INTO Inventory (item_id, name, price, stock_quantity, category) VALUES (?, ?, ?, ?, ?)")
        );
        pstmt->setInt(1, item->getId());
        pstmt->setString(2, item->getName());
        pstmt->setDouble(3, item->getPrice());
        pstmt->setInt(4, item->getStockQuantity());
        pstmt->setString(5, category);
        pstmt->execute();

        if (category == "Clothing") {
            Clothing* clothing = dynamic_cast<Clothing*>(item.get());
            if (clothing) {
                std::unique_ptr<sql::PreparedStatement> stmt(
                    conn->prepareStatement("INSERT INTO Clothing (item_id, size, fabric_type) VALUES (?, ?, ?)")
                );
                stmt->setInt(1, clothing->getId());
                stmt->setString(2, clothing->getSize());
                stmt->setString(3, clothing->getFabricType());
                stmt->execute();
            }
        } else if (category == "Electronics") {
            Electronics* electronics = dynamic_cast<Electronics*>(item.get());
            if (electronics) {
                std::unique_ptr<sql::PreparedStatement> stmt(
                    conn->prepareStatement("INSERT INTO Electronics (electronics_id, warranty_period) VALUES (?, ?)")
                );
                stmt->setInt(1, electronics->getId());
                stmt->setInt(2, electronics->getWarranty());
                stmt->execute();
            }
        } else if (category == "Shoes") {
            Shoes* shoes = dynamic_cast<Shoes*>(item.get());
            if (shoes) {
                std::unique_ptr<sql::PreparedStatement> stmt(
                    conn->prepareStatement("INSERT INTO Shoes (shoes_id, brand, size) VALUES (?, ?, ?)")
                );
                stmt->setInt(1, shoes->getId());
                stmt->setString(2, shoes->getBrand());
                stmt->setString(3, shoes->getSize());
                stmt->execute();
            }
        }

        std::cout << "Item added to database successfully!" << std::endl;
    } catch (sql::SQLException& e) {
        std::cerr << "SQL Error in addItem(): " << e.what() << std::endl;
    }
}

void InventoryManager::displayAllItems() const{ 
    try {
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT * FROM Inventory")
        );
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

        cout << left
             << setw(5) << "ID"
             << setw(15) << "Name"
             << setw(10) << "Price"
             << setw(8) << "Stock"
             << setw(12) << "Category"
             << setw(10) << "Discount"
             << setw(10) << "Value"
             << setw(20) << "Extra Info"
             << endl;
        cout << string(90, '-') << endl;

        while (res->next()) {
            int id = res->getInt("item_id");
            std::string name = res->getString("name");
            double price = res->getDouble("price");
            int stock = res->getInt("stock_quantity");
            std::string category = res->getString("category");
            std::string discountType = res->getString("discount_type");
            double discountValue = res->getDouble("discount_value");

            std::string extraInfo;

            if (category == "Clothing") {
                std::unique_ptr<sql::PreparedStatement> stmt2(
                    conn->prepareStatement("SELECT fabric_type, size FROM Clothing WHERE item_id = ?")
                );
                stmt2->setInt(1, id);
                std::unique_ptr<sql::ResultSet> res2(stmt2->executeQuery());
                if (res2->next()) {
                    extraInfo = "Fabric: " + res2->getString("fabric_type") +
                                ", Size: " + res2->getString("size");
                }
            } else if (category == "Electronics") {
                std::unique_ptr<sql::PreparedStatement> stmt2(
                    conn->prepareStatement("SELECT warranty_period FROM Electronics WHERE electronics_id = ?")
                );
                stmt2->setInt(1, id);
                std::unique_ptr<sql::ResultSet> res2(stmt2->executeQuery());
                if (res2->next()) {
                    extraInfo = "Warranty: " + res2->getString("warranty_period") + " months";
                }
            } else if (category == "Shoes") {
                std::unique_ptr<sql::PreparedStatement> stmt2(
                    conn->prepareStatement("SELECT brand, size FROM Shoes WHERE shoes_id = ?")
                );
                stmt2->setInt(1, id);
                std::unique_ptr<sql::ResultSet> res2(stmt2->executeQuery());
                if (res2->next()) {
                    extraInfo = "Brand: " + res2->getString("brand") +
                                ", Size: " + res2->getString("size");
                }
            }

            cout << left
                 << setw(5) << id
                 << setw(15) << name
                 << setw(10) << price
                 << setw(8) << stock
                 << setw(12) << category
                 << setw(10) << discountType
                 << setw(10) << discountValue
                 << setw(20) << extraInfo
                 << endl;
        }

    } catch (const sql::SQLException &e) {
        cerr << "SQL error in displayAllItems: " << e.what() << endl;
    }
}

void InventoryManager::searchItem(int id){
    try {
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT * FROM Inventory WHERE item_id = ?")
        );
        stmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

        if (res->next()) {
            cout << "Item Details:\n";
            cout << "ID: " << res->getInt("item_id") << endl;
            cout << "Name: " << res->getString("name") << endl;
            cout << "Price: " << res->getDouble("price") << endl;
            cout << "Stock Quantity: " << res->getInt("stock_quantity") << endl;
            cout << "Category: " << res->getString("category") << endl;
            cout << "Restock Threshold: " << res->getInt("restock_threshold") << endl;
            cout << "Restock Amount: " << res->getInt("restock_amount") << endl;
            cout << "Discount Type: " << res->getString("discount_type") << endl;
            cout << "Discount Value: " << res->getDouble("discount_value") << endl;

            std::string category = res->getString("category");

            if (category == "Clothing") {
                std::unique_ptr<sql::PreparedStatement> stmt2(
                    conn->prepareStatement("SELECT fabric_type, size FROM Clothing WHERE clothing_id = ?")
                );
                stmt2->setInt(1, id);
                std::unique_ptr<sql::ResultSet> res2(stmt2->executeQuery());
                if (res2->next()) {
                    cout << "Fabric Type: " << res2->getString("fabric_type") << endl;
                    cout << "Size: " << res2->getString("size") << endl;
                }
            } else if (category == "Electronics") {
                std::unique_ptr<sql::PreparedStatement> stmt2(
                    conn->prepareStatement("SELECT warranty_period FROM Electronics WHERE electronics_id = ?")
                );
                stmt2->setInt(1, id);
                std::unique_ptr<sql::ResultSet> res2(stmt2->executeQuery());
                if (res2->next()) {
                    cout << "Warranty Period: " << res2->getString("warranty_period") << " months" << endl;
                }
            } else if (category == "Shoes") {
                std::unique_ptr<sql::PreparedStatement> stmt2(
                    conn->prepareStatement("SELECT brand, size FROM Shoes WHERE shoes_id = ?")
                );
                stmt2->setInt(1, id);
                std::unique_ptr<sql::ResultSet> res2(stmt2->executeQuery());
                if (res2->next()) {
                    cout << "Brand: " << res2->getString("brand") << endl;
                    cout << "Size: " << res2->getString("size") << endl;
                }
            }

        } else {
            cout << "Item with ID " << id << " not found." << endl;
        }

    } catch (const sql::SQLException &e) {
        cerr << "SQL error in searchItem: " << e.what() << endl;
    }
}

void InventoryManager::updateStock(int id, int new_quantity){
    try {
        // First, check if the item exists
        std::unique_ptr<sql::PreparedStatement> checkStmt(
            conn->prepareStatement("SELECT COUNT(*) AS count FROM Inventory WHERE item_id = ?"));
        checkStmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(checkStmt->executeQuery());

        if (res->next() && res->getInt("count") == 0) {
            std::cout << "Item with ID " << id << " not found.\n";
            return;
        }

        // Perform the update
        std::unique_ptr<sql::PreparedStatement> updateStmt(
            conn->prepareStatement("UPDATE Inventory SET stock_quantity = ? WHERE item_id = ?"));
        updateStmt->setInt(1, new_quantity);
        updateStmt->setInt(2, id);
        updateStmt->executeUpdate();

        std::cout << "Stock updated successfully for item ID " << id << ". New Quantity: " << new_quantity << "\n";
    } catch (sql::SQLException& e) {
        std::cerr << "Error updating stock: " << e.what() << "\n";
    }
}

void InventoryManager::deleteItem(int id){
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("DELETE FROM Inventory WHERE item_id = ?")
        );
        pstmt->setInt(1, id);
        int affectedRows = pstmt->executeUpdate();

        if (affectedRows > 0) {
            std::cout << "Item with ID " << id << " deleted successfully!" << std::endl;
        } else {
            std::cerr << "Item with ID " << id << " not found!" << std::endl;
        }
    } catch (const sql::SQLException& e) {
        std::cerr << "SQL Error during deletion: " << e.what() << std::endl;
    }
}

unique_ptr<InventoryItem> InventoryManager::createItemFromResultSet(const unique_ptr<sql::ResultSet>& res) const {
    int id = res->getInt("item_id");
    string name = res->getString("name");
    double price = res->getDouble("price");
    int stock = res->getInt("stock_quantity");
    string category = res->getString("category");
    int restockThreshold = res->getInt("restock_threshold");
    int restockAmount = res->getInt("restock_amount");
    string discountType = res->getString("discount_type");
    double discountValue = res->getDouble("discount_value");

    if (category == "Shoes") {
        std::unique_ptr<sql::PreparedStatement> substmt(
            conn->prepareStatement("SELECT brand, size FROM Shoes WHERE shoes_id = ?")
        );
        substmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> subres(substmt->executeQuery());
        if (subres->next()) {
            string brand = subres->getString("brand");
            string size = subres->getString("size");
            return make_unique<Shoes>(id, name, price, stock, restockThreshold, restockAmount, discountType, discountValue, brand, size);
        }

    } else if (category == "Clothing") {
        std::unique_ptr<sql::PreparedStatement> substmt(
            conn->prepareStatement("SELECT fabric_type, size FROM Clothing WHERE item_id = ?")
        );
        substmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> subres(substmt->executeQuery());
        if (subres->next()) {
            string fabric = subres->getString("fabric_type");
            string size = subres->getString("size");
            return make_unique<Clothing>(id, name, price, stock, restockThreshold, restockAmount, discountType, discountValue, fabric, size);
        }

    } else if (category == "Electronics") {
        std::unique_ptr<sql::PreparedStatement> substmt(
            conn->prepareStatement("SELECT warranty_period FROM Electronics WHERE electronics_id = ?")
        );
        substmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> subres(substmt->executeQuery());
        if (subres->next()) {
            int warranty = subres->getInt("warranty_period");
            return make_unique<Electronics>(id, name, price, stock, restockThreshold, restockAmount, discountType, discountValue, warranty);
        }

    }
    return nullptr;
}

//search with filters
vector<unique_ptr<InventoryItem>> InventoryManager::searchByName(const string &name) const {
    vector<unique_ptr<InventoryItem>> results;
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("SELECT * FROM Inventory WHERE name LIKE ?")
        );
        pstmt->setString(1, "%" + name + "%");
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
            results.push_back(createItemFromResultSet(res));
        }
    } catch (const sql::SQLException& e) {
        std::cerr << "SQL Error in searchByName: " << e.what() << std::endl;
    }
    return results;
}

vector<unique_ptr<InventoryItem>> InventoryManager::searchByPriceRange(double minPrice, double maxPrice) const {
    vector<unique_ptr<InventoryItem>> results;
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("SELECT * FROM Inventory WHERE price BETWEEN ? AND ?")
        );
        pstmt->setDouble(1, minPrice);
        pstmt->setDouble(2, maxPrice);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
            results.push_back(createItemFromResultSet(res));
        }
    } catch (const sql::SQLException& e) {
        std::cerr << "SQL Error in searchByPriceRange: " << e.what() << std::endl;
    }
    return results;
}


vector<unique_ptr<InventoryItem>> InventoryManager::searchByStockLevels(int minStock, int maxStock) const {
    vector<unique_ptr<InventoryItem>> results;
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("SELECT * FROM Inventory WHERE stock_quantity BETWEEN ? AND ?")
        );
        pstmt->setInt(1, minStock);
        pstmt->setInt(2, maxStock);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
            results.push_back(createItemFromResultSet(res));
        }
    } catch (const sql::SQLException& e) {
        std::cerr << "SQL Error in searchByStockLevels: " << e.what() << std::endl;
    }
    return results;
}


vector<unique_ptr<InventoryItem>> InventoryManager::searchByCategory(const string &category) const {
    vector<unique_ptr<InventoryItem>> results;
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("SELECT * FROM Inventory WHERE category = ?")
        );
        pstmt->setString(1, category);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
            results.push_back(createItemFromResultSet(res));
        }
    } catch (const sql::SQLException& e) {
        std::cerr << "SQL Error in searchByCategory: " << e.what() << std::endl;
    }
    return results;
}


void InventoryManager::autoRestock(){
    try {
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement(
                "SELECT item_id, name, stock_quantity, restock_amount "
                "FROM Inventory WHERE stock_quantity < restock_threshold"
            )
        );
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

        std::unique_ptr<sql::PreparedStatement> updateStmt(
            conn->prepareStatement(
                "UPDATE Inventory SET stock_quantity = ? WHERE item_id = ?"
            )
        );

        while (res->next()) {
            int id = res->getInt("item_id");
            std::string name = res->getString("name");
            int currentStock = res->getInt("stock_quantity");
            int restockAmount = res->getInt("restock_amount");
            int newStock = currentStock + restockAmount;

            cout << "Auto Restocking item: " << name << " (ID: " << id << ")" << endl;

            updateStmt->setInt(1, newStock);
            updateStmt->setInt(2, id);
            updateStmt->executeUpdate();

            cout << "New stock level: " << newStock << endl;
        }
    } catch (sql::SQLException &e) {
        cerr << "SQL error in autoRestock: " << e.what() << endl;
    }
}

void InventoryManager::bulkUpdate(const string &filename){
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, name, priceStr, quantityStr, category;
        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, priceStr, ',');
        getline(ss, quantityStr, ',');
        getline(ss, category, ',');

        int id = stoi(idStr);
        double price = stod(priceStr);
        int quantity = stoi(quantityStr);

        // Read remaining category-specific fields
        vector<string> extraFields;
        string field;
        while (getline(ss, field, ',')) {
            extraFields.push_back(field);
        }

        try {
            // Step 1: Insert/update main inventory item
            std::unique_ptr<sql::PreparedStatement> stmt(
                conn->prepareStatement(
                    "INSERT INTO Inventory (item_id, name, price, stock_quantity, category) "
                    "VALUES (?, ?, ?, ?, ?) "
                    "ON DUPLICATE KEY UPDATE name=VALUES(name), price=VALUES(price), stock_quantity=VALUES(stock_quantity), category=VALUES(category)"
                )
            );
            stmt->setInt(1, id);
            stmt->setString(2, name);
            stmt->setDouble(3, price);
            stmt->setInt(4, quantity);
            stmt->setString(5, category);
            stmt->executeUpdate();

            // Step 2: Handle category-specific inserts/updates
            if (category == "Clothing" && extraFields.size() >= 2) {
                std::unique_ptr<sql::PreparedStatement> stmt2(
                    conn->prepareStatement(
                        "INSERT INTO Clothing (item_id, fabric_type, size) "
                        "VALUES (?, ?, ?) "
                        "ON DUPLICATE KEY UPDATE fabric_type=VALUES(fabric_type), size=VALUES(size)"
                    )
                );
                stmt2->setInt(1, id);
                stmt2->setString(2, extraFields[0]);
                stmt2->setString(3, extraFields[1]);
                stmt2->executeUpdate();
            } else if (category == "Electronics" && extraFields.size() >= 1) {
                std::unique_ptr<sql::PreparedStatement> stmt2(
                    conn->prepareStatement(
                        "INSERT INTO Electronics (electronics_id, warranty_period) "
                        "VALUES (?, ?) "
                        "ON DUPLICATE KEY UPDATE warranty_period=VALUES(warranty_period)"
                    )
                );
                stmt2->setInt(1, id);
                stmt2->setString(2, extraFields[0]);
                stmt2->executeUpdate();
            } else if (category == "Shoes" && extraFields.size() >= 2) {
                std::unique_ptr<sql::PreparedStatement> stmt2(
                    conn->prepareStatement(
                        "INSERT INTO Shoes (shoes_id, brand, size) "
                        "VALUES (?, ?, ?) "
                        "ON DUPLICATE KEY UPDATE brand=VALUES(brand), size=VALUES(size)"
                    )
                );
                stmt2->setInt(1, id);
                stmt2->setString(2, extraFields[0]);
                stmt2->setString(3, extraFields[1]);
                stmt2->executeUpdate();
            } else {
                cerr << "Insufficient extra fields for category: " << category << " at item ID: " << id << endl;
            }

        } catch (const sql::SQLException& e) {
            cerr << "SQL error during bulkUpdate: " << e.what() << endl;
        }
    }

    file.close();
}

bool InventoryManager::processSale(int id, int quantity){
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("SELECT stock_quantity, price FROM Inventory WHERE item_id = ?")
        );
        pstmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        AuditLogger auditLogger(conn);  

        if (!res->next()) {
            std::cout << "Item with ID " << id << " not found.\n";
            auditLogger.logEvent("Failed sale: Item ID " + std::to_string(id) + " not found.");
            return false;
        }

        int currentStock = res->getInt("stock_quantity");
        double price = res->getDouble("price");

        if (currentStock < quantity) {
            std::cout << "Insufficient stock for item ID " << id << ".\n";
            auditLogger.logEvent("Failed sale: Insufficient stock for ID " + std::to_string(id));
            return false;
        }

        std::unique_ptr<sql::PreparedStatement> updateStmt(
            conn->prepareStatement("UPDATE Inventory SET stock_quantity = ? WHERE item_id = ?")
        );
        updateStmt->setInt(1, currentStock - quantity);
        updateStmt->setInt(2, id);
        updateStmt->executeUpdate();

        Transaction transaction{id, quantity, price, "sale"};
        logTransaction(conn, transaction);

        std::cout << "Sale processed successfully!\n";
        auditLogger.logEvent("Sale processed: ID " + std::to_string(id) + ", Quantity " + std::to_string(quantity));
        return true;
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL error during sale: " << e.what() << std::endl;
        return false;
    }
}

bool InventoryManager::processRestock(int id, int quantity){
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("SELECT stock_quantity, price FROM Inventory WHERE item_id = ?")
        );
        pstmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        AuditLogger auditLogger(conn); 

        if (!res->next()) {
            std::cout << "Item with ID " << id << " not found.\n";
            auditLogger.logEvent("Failed restock: Item ID " + std::to_string(id) + " not found.");
            return false;
        }

        int currentStock = res->getInt("stock_quantity");
        double price = res->getDouble("price");

        std::unique_ptr<sql::PreparedStatement> updateStmt(
            conn->prepareStatement("UPDATE Inventory SET stock_quantity = ? WHERE item_id = ?")
        );
        updateStmt->setInt(1, currentStock + quantity);
        updateStmt->setInt(2, id);
        updateStmt->executeUpdate();

        Transaction transaction{id, quantity, price, "restock"};
        logTransaction(conn, transaction);

        std::cout << "Restock processed successfully!\n";
        auditLogger.logEvent("Restock processed: ID " + std::to_string(id) + ", Quantity " + std::to_string(quantity));
        return true;
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL error during restock: " << e.what() << std::endl;
        return false;
    }
}

// string InventoryManager::getCurrentTimestamp(){
//     auto now = std::chrono::system_clock::now();
//     auto time = std::chrono::system_clock::to_time_t(now);
//     std::string timestamp = std::ctime(&time);
//     timestamp.pop_back();  // Remove the newline at the end
//     return timestamp;
// }

void InventoryManager::applyDiscountToItem(int id, Discount& discount){
    try {
        std::unique_ptr<sql::PreparedStatement> checkStmt(
            conn->prepareStatement("SELECT COUNT(*) AS count FROM Inventory WHERE item_id = ?"));
        checkStmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(checkStmt->executeQuery());

        if (res->next() && res->getInt("count") == 0) {
            std::cout << "Item with ID: " << id << " not found!" << std::endl;
            return;
        }

        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("UPDATE Inventory SET discount_type = ?, discount_value = ? WHERE item_id = ?")
        );
        stmt->setString(1, discount.type);   // e.g., "Percentage"
        stmt->setDouble(2, discount.value);  // e.g., 10.0
        stmt->setInt(3, id);

        int affected = stmt->executeUpdate();
        if (affected > 0) {
            std::cout << "Discount applied to item ID: " << id << " successfully!" << std::endl;
        } else {
            std::cout << "Item ID " << id << " already has this discount set." << std::endl;
        }

    } catch (const sql::SQLException& e) {
        std::cerr << "SQL Error in applyDiscountToItem: " << e.what() << std::endl;
    }
}

void InventoryManager::applyDiscountToCategory(const string &category, Discount& discount){
    try {
        std::unique_ptr<sql::PreparedStatement> checkStmt(
            conn->prepareStatement("SELECT COUNT(*) AS count FROM Inventory WHERE category = ?"));
        checkStmt->setString(1, category);
        std::unique_ptr<sql::ResultSet> res(checkStmt->executeQuery());

        if (res->next() && res->getInt("count") == 0) {
            std::cout << "No items with category: " << category << " found!" << std::endl;
            return;
        }

        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("UPDATE Inventory SET discount_type = ?, discount_value = ? WHERE category = ?")
        );
        stmt->setString(1, discount.type);
        stmt->setDouble(2, discount.value);
        stmt->setString(3, category);

        int affected = stmt->executeUpdate();
        if (affected > 0) {
            std::cout << "Discount applied to items with category: " << category << " successfully!" << std::endl;
        } else {
            std::cout << "Items with category " << category << " already have this discount set." << std::endl;
        }

    } catch (const sql::SQLException& e) {
        std::cerr << "SQL Error in applyDiscountToCategory: " << e.what() << std::endl;
    }
}

void InventoryManager::clearInventory(){
    std::unique_ptr<sql::Statement> stmt(conn->createStatement());
    stmt->execute("DELETE FROM Inventory");
}