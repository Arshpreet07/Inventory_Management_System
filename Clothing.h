#ifndef CLOTHING_H
#define CLOTHING_H

#include "InventoryItem.h"

class Clothing : public InventoryItem{
private:
    std::string fabric_type;
    std::string size;

public:
    Clothing(int id, const std::string& name, double price, int stock_quantity,
         int restockThreshold, int restockAmount,
         const std::string& discountType, double discountValue,
         const std::string& fabric_type, const std::string& size);


    double calculatePrice() const override;

    void displayItem() const override;

    const std::string& getFabricType() const;
    const std::string& getSize() const;
};

#endif