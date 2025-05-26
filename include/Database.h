#pragma once
#include "Product.h"
#include <optional>
#include <vector>
#include <string>

using namespace std;

class Database
{
private:
    vector<Product> products;
    // вспомогательный метод
    bool isNum(const string &str);

public:
    bool isDatabaseLoad();
    bool load(const string &filename);
    void display();
    void addProduct(unsigned int id, const string &name, const string &type, float basePrice, float discount);
    void removeProduct(unsigned int id);
    bool saveToFile(const string &filename) const;
    void sortById();
    bool searchById(unsigned int id) const;
    void selectByPriceRange(float minPrice, float maxPrice) const;
    void addDiscount(const vector<string>& types, float additionalDiscount);
    void removeProductsBelowThreshold(float threshold);
};