#pragma once
#include <string>
#include <iostream>

using namespace std;

class Product
{
private:
    unsigned int id;
    string name;
    string type;
    float basePrice;
    float discount;
    float salePrice;

    void calculateSalePrice();

public:
    Product(unsigned int id, const string &name, const string &type, float basePrice, float discount);

    unsigned int getId() const;
    const string getName() const;
    const string getType() const;
    float getBasePrice() const;
    float getDiscount() const;
    float getSalePrice() const;

    void setBasePrice(float bp);
    void setDiscount(float d);

    void display() const;
    void writeToFile(ofstream &file) const;
};