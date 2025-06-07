#include "Electronics.h"
// #include "CategoryRegistry.h"
#include <iostream>

//constructor
Electronics::Electronics(int id, const std::string& name, double price, int stock_quantity,
            int restockThreshold, int restockAmount,
            const std::string& discountType, double discountValue,
            int warranty)
    : InventoryItem(id, name, price, stock_quantity, "Electronics"), warranty_period(warranty) {}

int Electronics::getWarranty() const{
    return warranty_period;
}

//overriding calculatePrice to include warranty price
double Electronics::calculatePrice() const{
    double final_price = price;
    if(warranty_period > 12){
        final_price += 1000;
    }
    return final_price;
}

//overriding to include warranty in displayItem
void Electronics::displayItem() const{
    InventoryItem::displayItem();
    std::cout << "Warranty : "<< warranty_period << " months" << std::endl;
}

// static bool electronicsRegistered = []() {
//     CategoryRegistry::getInstance().registerCategory("Electronics", 
//         [](int id, const std::string& name, double price, int stock_quantity, const std::vector<std::string>& extraFields) {
//             int warranty = (extraFields.size() > 0) ? stoi(extraFields[0]) : 12;
//             return std::make_unique<Electronics>(id, name, price, stock_quantity, warranty);
//         });
//     return true;
// }();