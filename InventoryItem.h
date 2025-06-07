#ifndef INVENTORY_ITEM_H
#define INVENTORY_ITEM_H
#include<iostream>
#include<string>

struct Discount{
    std::string type;
    double value; 
};
class InventoryItem{
protected:
    int id;
    std::string name;
    double price;
    int stock_quantity;
    std::string category;

    int restockThreshold;
    int restockAmount;
    Discount discount{"None",0.0};

public:
    virtual ~InventoryItem() = default;
    InventoryItem(int id, const std::string& name, double price, int stock_quantity, const std::string& category, int restockThreshold = 5, int restockAmount = 10);

    virtual double calculatePrice() const = 0;
    void updateStock(int new_stock_quantity);

    const std::string& getName() const;
    int getId() const;
    double getPrice() const;
    int getStockQuantity() const;
    const std::string& getCategory() const;
    Discount getDiscount() const;

    virtual void displayItem() const;

    bool needRestocking() const;
    void restock();

    void setRestockThreshold(int threshold);
    void setRestockAmount(int amount);

    int getRestockThreshold() const;
    int getRestockAmount() const;

    void setDiscount(Discount& discount);
    double getPriceAfterDiscount() const;

};

#endif