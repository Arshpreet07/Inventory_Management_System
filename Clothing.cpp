#include "Clothing.h"
// #include "CategoryRegistry.h"
#include <iostream>

Clothing::Clothing(int id, const std::string& name, double price, int stock_quantity,
         int restockThreshold, int restockAmount,
         const std::string& discountType, double discountValue,
         const std::string& fabric_type, const std::string& size)
    : InventoryItem(id, name, price, stock_quantity, "Clothing"), fabric_type(fabric_type), size(size) {}

double Clothing::calculatePrice() const{
    double final_price = price;
    if(fabric_type == "Silk"){
        final_price += 500;
    }
    if(size == "L" || size == "XL" || size == "XXL"){
        final_price += 100;
    }
    return final_price;
}

void Clothing::displayItem() const{
    InventoryItem::displayItem();
    std::cout << "Fabric Type : " << fabric_type << ", "
    << "Size : "<< size << std::endl;
}

const std::string& Clothing::getFabricType() const{
    return fabric_type;
}

const std::string& Clothing::getSize() const{
    return size;
}

// static bool clothingRegistered = []() {
//     CategoryRegistry::getInstance().registerCategory("Clothing", 
//         [](int id, const std::string& name, double price, int stock_quantity, const std::vector<std::string> &extraFields) {
//             std::string fabricType = (extraFields.size() > 0) ? extraFields[0] : "Cotton";
//             std::string fabricSize = (extraFields.size() > 1) ? extraFields[1] : "M";
//             return std::make_unique<Clothing>(id, name, price, stock_quantity, fabricType, fabricSize);
//         });
//     return true;
// }();