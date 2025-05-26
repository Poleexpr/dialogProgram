#include "Database.h"
#include <optional>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <unordered_set>

using namespace std;

// проверка на корректность численных значений
bool Database::isNum(const string &str)
{
  try
  {
    size_t pos;
    stof(str, &pos);
    return pos == str.length();
  }
  catch (...)
  {
    return false;
  }
}

bool Database::isDatabaseLoad()
{
  if (products.empty())
  {
    cout << "Пожалуйста, загрузите/заполните БД" << endl;
    return false;
  }
  return true;
}

// метод загрузки БД из файла
bool Database::load(const string filename)
{
  ifstream file(filename);

  // проверяем, можем ли мы работать с файлом
  if (!file.is_open())
  {
    cerr << "Не удалось открыть файл" << endl;
    return false;
  }

  // очищаем вектор продуктов
  products.clear();

  unordered_set<unsigned int> id_set;
  string line;
  size_t line_num = 0;
  bool has_errors = false;

  while (getline(file, line))
  {
    // считаем строки, чтобы при необходимости отловить, где именно возникла ошибка
    line_num++;

    if (line.empty())
      continue;

    try
    {
      stringstream ss(line);
      string item;
      vector<string> items;

      // разбиваем строку на независимые элементы через разделитель ";"
      while (getline(ss, item, ';'))
      {
        items.push_back(item);
      }

      // вылидируем данные
      if (items.size() != 5)
        throw runtime_error("В файле данных должно быть 5 параметров");
      if (!isNum(items[0]))
        throw runtime_error("Код должен быть числом");
      if (!isNum(items[3]))
        throw runtime_error("Цена должна быть числом");
      if (!isNum(items[4]))
        throw runtime_error("Скидка должна быть числом");

      // конвертируем в нужный формат id, цену и скидку
      unsigned int id = stoi(items[0]);
      float basePrice = stof(items[3]);
      float discount = stof(items[4]);

      // проверяем цену и скидку на соответсвие требованиям
      if (basePrice < 0 || discount < 0 || discount > 100)
      {
        throw runtime_error("Цена должна быть больше 0, скидка должна быть больше 0 и меньше 100");
      }

      // проверяем уникальность кода
      if (id_set.count(id) > 0)
      {
        throw runtime_error("Дубликат кода товара: " + to_string(id));
      }
      id_set.insert(id);

      // парсим данные
      products.push_back(Product(id, items[1], items[2], basePrice, discount));
    }
    catch (const exception &e)
    {
      cerr << "Ошибка в строке " << line_num << ": " << e.what() << endl
           << "Строка: " << line << endl;
      has_errors = true;
    }
  }

  return !has_errors;
}

// метод просмотра БД в виде таблицы
void Database::display()
{
  // проверяем загружена/заполнена ли БД
  if (!isDatabaseLoad())
  {
    return;
  }
  // выводим заголовки таблицы
  cout << setw(5) << right << "Code" << " | "
       << setw(40) << left << "Name"
       << setw(20) << left << "Type" << setw(15) << right << "Base price" << setw(15) << right << "Discount" << setw(15) << right << "Sale Price" << endl;
  cout << string(115, '-') << endl;

  // проходимся циклом по продуктам и выводим каждый в формате таблицы
  for (size_t i = 0; i < products.size(); i++)
  {
    products[i].display();
  }
}

// метод добавления новой записи
void Database::addProduct(unsigned int id, const string name, const string type, float basePrice, float discount)
{
  products.push_back(Product(id, name, type, basePrice, discount));
  cout << "Товар добавлен" << endl;
}

// метод удаления записи
void Database::removeProduct(unsigned int id)
{
  unsigned int countProducts = 0;

  for (size_t i = 0; i < products.size(); i++)
  {
    if (products[i].getId() == id)
    {
      products.erase(products.begin() + i);
      countProducts++;
      cout << "Товар удален" << endl;
    }
  }

  if (countProducts == 0)
  {
    cout << "Товар с таким кодом не существует" << endl;
  }
}

// метод сохранения БД в файл
bool Database::saveToFile(const string filename) const
{
  string path = "../data/";
  string fullPath = path + filename;

  ofstream file(fullPath);
  if (!file.is_open())
  {
    return false;
  }

  for (size_t i = 0; i < products.size(); i++)
  {
    products[i].writeToFile(file);
  }

  file.close();
  return true;
}

// метод сортировки БД по коду товара
void Database::sortById()
{
  if (!isDatabaseLoad())
  {
    return;
  }

  // применяем лямбда-функцию и встроенный алгоритм сортировки
  sort(products.begin(), products.end(),
       [](const Product &a, const Product &b)
       {
         return a.getId() < b.getId();
       });
  cout << "БД отсортирована по коду товара" << endl;
}

// метод поиска записи по коду товара
bool Database::searchById(unsigned int id) const
{
  // применяем лямбда-функцию и встроенный алгоритм поиска
  auto iter = find_if(products.begin(), products.end(),
                      [id](const Product &p)
                      { return p.getId() == id; });
  if (iter != products.end())
  {
    cout << "Товар с таким кодом существует: " << endl;
    iter->display();
    return true;
  }
  else
  {
    cout << "Товар с таким кодом не существует" << endl;
    return false;
  }
}

// метод выборки товаров, попадающих в заданный диапазон цен за штуку
void Database::selectByPriceRange(float minPrice, float maxPrice) const
{
  unsigned int countProducts = 0;

  for (size_t i = 0; i < products.size(); i++)
  {
    float price = products[i].getBasePrice();
    if (price >= minPrice && price <= maxPrice)
    {
      products[i].display();
      countProducts++;
    }
  }

  if (countProducts == 0)
  {
    cout << "Товары в указанном диапазоне цен не найдены" << endl;
  }
}

// метод добавления скидки по акции
void Database::addDiscount(const vector<string> types, float additionalDiscount)
{
  unordered_set<string> matchTypes;
  // считаем количество совпадений по типам, чтобы сообщить пользователю найдены ли акционные товары
  unsigned int countProducts = 0;

  for (size_t i = 0; i < products.size(); i++)
  {
    if (find(types.begin(), types.end(), products[i].getType()) != types.end())
    {
      matchTypes.insert(products[i].getType());
      float newDiscount = products[i].getDiscount() + additionalDiscount;
      products[i].setDiscount(newDiscount);
      countProducts++;
    }
  }

  if (countProducts == 0)
  {
    cout << "Подходящих товаров не найдено" << endl;
  }
  else
  {
    cout << "Скидка применена для: " << endl;
    for (auto iter = matchTypes.begin(); iter != matchTypes.end(); iter++)
    {
      cout << *iter << endl;
    }
  }
}

// метод удаления товаров, цена продажи которых ниже заданного порога
void Database::removeProductsBelowThreshold(float threshold)
{
  unsigned int countProducts = 0;
  size_t i = 0;
  while (i < products.size())
  {
    if (products[i].getSalePrice() < threshold)
    {
      products.erase(products.begin() + i);
      countProducts++;
    }
    else
    {
      i++;
    }
  }

  if (countProducts == 0)
  {
    cout << "Товары не найдены" << endl;
  }
  else
  {
    cout << "Товары удалены" << endl;
  }
}