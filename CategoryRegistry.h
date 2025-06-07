#ifndef CATEGORY_REGISTERY_H
#define CATEGORY_REGISTERY_H

#include "InventoryItem.h"
#include <unordered_map>
#include <string>
#include <functional>
#include <memory>

class CategoryRegistry{
private:
    std::unordered_map<std::string, std::function<std::unique_ptr<InventoryItem>(int,const std::string&, double, int, const std::vector<std::string>& )>> registry;
public:
    static CategoryRegistry& getInstance(){
        static CategoryRegistry instance;
        return instance;
    }

    void registerCategory(const std::string& categoryName, std::function<std::unique_ptr<InventoryItem>(int,const std::string&, double, int, const std::vector<std::string>& )> factoryFunction){
        registry[categoryName] = factoryFunction;
    }

    std::unique_ptr<InventoryItem> createItem(const std::string &categoryName, int id, const std::string& name, double price, int stock_quantity, const std::vector<std::string>& extraFields){
        if(registry.find(categoryName) != registry.end()){
            return registry[categoryName](id, name, price, stock_quantity, extraFields);
        }
        throw std::runtime_error("Category not found : " + categoryName);
    }
    CategoryRegistry(const CategoryRegistry&) = delete;
    void operator=(const CategoryRegistry&) = delete;
private:
    CategoryRegistry() = default;
};

#endif