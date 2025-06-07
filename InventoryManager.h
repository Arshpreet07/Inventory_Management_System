#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include "Clothing.h"
#include "InventoryItem.h"
#include "Electronics.h"
#include "Shoes.h"
#include <unordered_map>
#include <memory>
#include <algorithm> 
#include <vector>
#include <fstream>
#include <sstream>
#include "DatabaseConnection.h"

class InventoryManager{
private:
    std::shared_ptr<sql::Connection> conn;

public:
    InventoryManager(std::shared_ptr<sql::Connection> conn);
    
    void addItem(std::unique_ptr<InventoryItem> item);

    void displayAllItems() const;

    void searchItem(int id);

    void updateStock(int id, int new_quantity);

    void deleteItem(int id);

    std::unique_ptr<InventoryItem> createItemFromResultSet(const std::unique_ptr<sql::ResultSet>& res) const;

    std::vector<std::unique_ptr<InventoryItem>> searchByName(const std::string &name) const;

    std::vector<std::unique_ptr<InventoryItem>> searchByPriceRange(double minPrice, double maxPrice) const;

    std::vector<std::unique_ptr<InventoryItem>> searchByStockLevels(int minStock, int maxStock) const;

    std::vector<std::unique_ptr<InventoryItem>> searchByCategory(const std::string &category) const;

    void autoRestock();

    void bulkUpdate(const std::string& filename);

    bool processSale(int id, int quantity);
    bool processRestock(int id, int quantity);

    // std::string getCurrentTimestamp();
    
    void applyDiscountToItem(int id,Discount &discount);
    void applyDiscountToCategory(const std::string& category,Discount& discount);

    void clearInventory();
};

#endif