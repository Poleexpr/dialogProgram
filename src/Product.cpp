#include "Product.h"
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

// конструктор
Product::Product(unsigned int c, const string n, const string t, float bp, float d)
    : id(c), name(n), type(t), basePrice(bp), discount(d)
{
    calculateSalePrice();
}

// вычисление цены продажи
void Product::calculateSalePrice()
{
    salePrice = basePrice * (1 - discount / 100.0f);
}

// геттеры
unsigned int Product::getId() const { return id; }
const string Product::getName() const { return name; }
const string Product::getType() const { return type; }
float Product::getBasePrice() const { return basePrice; }
float Product::getDiscount() const { return discount; }
float Product::getSalePrice() const { return salePrice; }

// сеттеры
/*void Product::setBasePrice(float bp) {
  basePrice = bp;
  calculateSalePrice();
}*/

void Product::setDiscount(float d)
{
    discount = d;
    calculateSalePrice();
}

// форматирование поля продукта в таблицу
void Product::display() const
{
    cout << setw(5) << id << " | "
         << setw(40) << left << name.substr(0, 40)
         << setw(20) << left << type.substr(0, 20)
         << setw(15) << right << fixed << setprecision(2) << basePrice << "Р"
         << setw(15) << right << fixed << setprecision(2) << discount << "%"
         << setw(15) << right << fixed << setprecision(2) << salePrice << "Р" << endl;
}

// запись поля продукта в файл
void Product::writeToFile(ofstream &file) const
{
    file << id << ";" << name << ";" << type << ";"
         << basePrice << ";" << discount << "\n";
}