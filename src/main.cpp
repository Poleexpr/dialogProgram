#include "Product.h"
#include "Database.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

// вспомогательная функция для удаления пробелов в начале и в конце строки при вводе
string trim(const string &s)
{
    auto start = s.find_first_not_of(" \t\n\r\f\v");
    auto end = s.find_last_not_of(" \t\n\r\f\v");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

// функция вывода меню
void displayMenu()
{
    cout << "Меню управления БД:" << endl;
    cout << "1. Загрузить БД из файла" << endl;
    cout << "2. Просмотреть БД в виде таблицы" << endl;
    cout << "3. Добавить новую запись" << endl;
    cout << "4. Удалить запись по коду товара" << endl;
    cout << "5. Сохранить БД в файл" << endl;
    cout << "6. Сортировать БД по коду товара" << endl;
    cout << "7. Поиск записи по коду товара" << endl;
    cout << "8. Выборка товаров по диапазону цен за штуку" << endl;
    cout << "9. Применить дополнительную скидку к товарам определенных типов" << endl;
    cout << "10. Удалить товары с ценой продажи ниже заданного порога" << endl;
    cout << "0. Выход" << endl;
    cout << "Выберите действие: ";
}

// функция загрузки БД из файла
void load(Database &db)
{
    string filename;

    cout << "Введите наименования файла БД: ";
    getline(cin, filename);

    if (db.load(filename))
    {
        cout << "БД загружена" << endl;
    }
    else
    {
        cout << "Не удавлось загрузить БД" << endl;
    }
}

// функция добавления новой записи
void addNewProduct(Database &db)
{
    unsigned int id;
    string name, type;
    float basePrice, discount;
    bool isIDExists;

    // вводим и валидируем код
    cout << "Введите код товара: ";
    if (!(cin >> id))
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cerr << "Код должен быть числом" << endl;
        return;
    }

    isIDExists = db.searchById(id);
    if (!isIDExists)
    {
        cout << "Можете продолжить процедуру добавления товара" << endl;
    }
    else
    {
        return;
    }
    cin.ignore();

    // вводим и валидируем название
    cout << "Введите название: ";
    getline(cin, name);
    name = trim(name);

    if (name.empty())
    {
        cerr << "Название не должно быть пустым" << endl;
        return;
    }

    // вводим и валидируем тип
    cout << "Введите тип: ";
    getline(cin, type);
    type = trim(type);

    if (type.empty())
    {
        cerr << "Тип не должен быть пустым" << endl;
        return;
    }

    // вводим и валидируем цену
    cout << "Введите цену за штуку: ";
    if (!(cin >> basePrice) || basePrice <= 0)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cerr << "Цена должна быть числом больше 0" << endl;
        return;
    }

    // вводим и валидируем скидку
    cout << "Введите скидку (%): ";
    if (!(cin >> discount) || discount < 0 || discount > 100)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cerr << "Скидка должна быть числом от 0 до 100%" << endl;
        return;
    }

    db.addProduct(id, name, type, basePrice, discount);
}

// функция удаления по коду товара
void removeProduct(Database &db)
{
    if (!db.isDatabaseLoad())
    {
        return;
    }

    unsigned int id;
    cout << "Введите код, чтобы удалить товар: ";
    if (!(cin >> id))
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cerr << "Код должен быть числом" << endl;
        return;
    }

    db.removeProduct(id);
}

// функция сохранения БД в файл
void saveToFile(Database &db)
{
    if (!db.isDatabaseLoad())
    {
        return;
    }

    string filename;
    cout << "Введите имя нового файла с расширением: ";
    getline(cin, filename);

    if (filename.empty())
    {
        cerr << "Имя не должно быть пустым" << endl;
        return;
    }

    if (db.saveToFile(filename))
    {
        cout << "БД сохранена в файл" << endl;
    }
    else
    {
        cout << "Ошибка при сохранении БД в файл" << endl;
    }
}

// функция поиска по коду товара
void searchById(Database &db)
{
    unsigned int id;

    if (!db.isDatabaseLoad())
    {
        return;
    }

    cout << "Введите код, чтобы найти товар: ";
    if (!(cin >> id))
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cerr << "Код должен быть числом" << endl;
        return;
    }

    db.searchById(id);
}

// функция выборки по диапазону цен за штуку
void selectByPriceRange(Database &db)
{
    if (!db.isDatabaseLoad())
    {
        return;
    }

    float minPrice, maxPrice;
    cout << "Введите минимальную цену за штуку: ";
    if (!(cin >> minPrice) || minPrice <= 0)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cerr << "Цена должна быть числом больше 0" << endl;
        return;
    }
    cout << "Введите максимальную цену за штуку: ";
    if (!(cin >> maxPrice) || maxPrice <= 0)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cerr << "Цена должна быть числом больше 0" << endl;
        return;
    }

    if (minPrice > maxPrice)
    {
        cerr << "Минимальная цена должна быть меньше максимальной" << endl;
        return;
    }

    db.selectByPriceRange(minPrice, maxPrice);
}

// функция добавления скидки по акции
void addDiscount(Database &db)
{
    if (!db.isDatabaseLoad())
    {
        return;
    }

    vector<string> types;
    string type;
    float discount;

    cout << "Введите типы товаров, к которым будет применена акционная скидка (или введите 'далее' для продолжения): " << endl;
    while (true)
    {
        cout << "Тип: ";
        getline(cin, type);
        type = trim(type);

        if (type == "далее")
        {
            if (types.empty())
            {
                cout << "Должен быть указан хотя бы один тип" << endl;
                continue;
            }
            break;
        }

        if (type.empty())
        {
            cerr << "Тип не должен быть пустым" << endl;
            continue;
        }

        types.push_back(type);
    }

    cout << "Введите дополнительную скидку (%): ";
    if (!(cin >> discount) || discount < 0 || discount > 100)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cerr << "Скидка должна быть числом от 0 до 100%" << endl;
        return;
    }

    db.addDiscount(types, discount);
}

// функция для удаления товаров с ценой ниже заданного порога
void removeProductsBelowThreshold(Database &db)
{
    if (!db.isDatabaseLoad())
    {
        return;
    }

    float threshold;
    cout << "Введите пороговое значение цены продажи: ";
    if (!(cin >> threshold) || threshold <= 0)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cerr << "Порог должен быть числом больше 0" << endl;
        return;
    }

    db.removeProductsBelowThreshold(threshold);
}

int main()
{
    Database db;
    int choice;

    // установка локали для корректного отображения кириллицы
    setlocale(LC_ALL, "Russian");

    while (true)
    {
        displayMenu();
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Ошибка: введите число от 0 до 10\n";
            continue;
        }
        cin.ignore();

        switch (choice)
        {
        // загрузка БД из файла
        case 1:
            load(db);
            break;

        // просмотр БД в виде таблицы
        case 2:
            db.display();
            break;

        // добавление новой записи
        case 3:
            addNewProduct(db);
            break;

        // удаление записи по коду
        case 4:
            removeProduct(db);
            break;

        // сохранение БД в файл
        case 5:
            saveToFile(db);
            break;

        // cортировка  БД по коду товара
        case 6:
            db.sortById();
            break;

        // поиск записи по коду товара
        case 7:
            searchById(db);
            break;

        // выборка по диапазону цен за штуку
        case 8:
            selectByPriceRange(db);
            break;

        // добавление скидки по акции
        case 9:
            addDiscount(db);
            break;

        // удаление товаров с ценой продажи ниже порога
        case 10:
            removeProductsBelowThreshold(db);
            break;

        case 0:
            cout << "Выход из программы" << endl;
            return 0;

        default:
            cout << "Такой опции не существует" << endl;
        }
    }

    return 0;
}