#include "Shoes.h"
// #include "CategoryRegistry.h"

#include <iostream>

Shoes::Shoes(int id, const std::string& name, double price, int stock_quantity,
      int restockThreshold, int restockAmount,
      const std::string& discountType, double discountValue,
      const std::string& brand, const std::string& size)
    : InventoryItem(id, name, price, stock_quantity, "Shoes"), brand(brand), size(size) {}

double Shoes::calculatePrice() const{
    double final_price = price;
    if(size == "L" || size=="XL"){
        final_price += 100;
    }
    return final_price;
}

void Shoes::displayItem() const{
    InventoryItem::displayItem();
    std::cout << "Brand : " << brand << ", "
    << "Size : " << size << std::endl;
}

const std::string& Shoes::getSize() const{
    return size;
}

const std::string& Shoes::getBrand() const{
    return brand;
}

// static bool ShoesRegistered = []() {
//     CategoryRegistry::getInstance().registerCategory("Shoes",
//     [](int id, const std::string& name, double price, int stock_quantity, const std::vector<std::string>& extraFields){
//         std::string brand = (extraFields.size() > 0) ? extraFields[0] : "Default";
//         std::string size = (extraFields.size() > 1) ? extraFields[1] : "M";
//         return std::make_unique<Shoes>(id, name, price, stock_quantity, brand, size);
//     });
//     return true;
// }();






