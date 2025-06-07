#ifndef SHOES_H
#define SHOES_H

#include "InventoryItem.h"
#include <string>

class Shoes : public InventoryItem{
private:
    std::string brand;
    std::string size;
public:
    Shoes(int id, const std::string& name, double price, int stock_quantity,
      int restockThreshold, int restockAmount,
      const std::string& discountType, double discountValue,
      const std::string& brand, const std::string& size);


    double calculatePrice() const override;
    void displayItem() const override;

    const std::string& getBrand() const;
    const std::string& getSize() const;
};

#endif