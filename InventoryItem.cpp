#include "InventoryItem.h"
using namespace std;

//constructor
InventoryItem::InventoryItem(int id, const string& name, double price, int stock_quantity, const string &category, int threshold, int amount) 
    : id(id), name(name), price(price), stock_quantity(stock_quantity), category(category), restockThreshold(threshold), restockAmount(amount) {}

//update stock
void InventoryItem::updateStock(int new_stock_quantity){
    stock_quantity = new_stock_quantity;
}

//getter functions
const string& InventoryItem::getName() const{
    return name;
}

const string& InventoryItem::getCategory() const{
    return category;
}

int InventoryItem::getId() const{
    return id;
}

double InventoryItem::getPrice() const{
    return price;
}

int InventoryItem::getStockQuantity() const{
    return stock_quantity;
}

void InventoryItem::restock(){
    stock_quantity += restockAmount;
}

bool InventoryItem::needRestocking() const{
    return (stock_quantity < restockThreshold);
}

void InventoryItem::setRestockThreshold(int threshold){
    restockThreshold = threshold;
}
void InventoryItem::setRestockAmount(int amount){
    restockAmount = amount;
}

int InventoryItem::getRestockThreshold() const{
    return restockThreshold;
}
int InventoryItem::getRestockAmount() const{
    return restockAmount;
}

//display item
void InventoryItem::displayItem() const{
    cout<< "ID : "<< id << ", "
    << "Name : " << name << ", "
    << "Price : " << price << ", "
    << "Stock : " << stock_quantity << ", "
    << "Price after discount : " << getPriceAfterDiscount() << endl;
}

double InventoryItem::getPriceAfterDiscount() const{
    if(discount.type == "Percentage"){
        return price - (price*discount.value/100);
    }else if(discount.type == "Flat"){
        return std::max(price - discount.value, 0.0);
    }
    return price;
}

void InventoryItem::setDiscount(Discount& discount){
   this->discount = discount;
}

Discount InventoryItem::getDiscount() const{
    return discount;
}