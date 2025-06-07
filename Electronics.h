#ifndef ELECTRONICS_H
#define ELECTRONICS_H

#include "InventoryItem.h"

class Electronics : public InventoryItem{
private:
    int warranty_period;

public:
    Electronics(int id, const std::string& name, double price, int stock_quantity,
            int restockThreshold, int restockAmount,
            const std::string& discountType, double discountValue,
            int warranty);


    int getWarranty() const;

    double calculatePrice() const override;

    void displayItem() const override;
};

#endif