#include "Store.h"
using namespace std;
Store::Store(const string & url, unsigned int dayMonitoring, unsigned int countProduct, unsigned int price) :
    countDay(1),              
    url(url),                 
    price(price),             
    countProduct(countProduct), 
    dayMonitoring(dayMonitoring),
    maxPrice(price),
    minPrice(price),
    drawPrice(price)
{
    validateParameters(url, dayMonitoring, countProduct, price);
    pricesData.push_back(price);
}
Store::Store(const string& url, unsigned int countProduct) :
    countDay(1),
    url(url),
    price(10),
    countProduct(countProduct),
    dayMonitoring(30),
    maxPrice(price),
    minPrice(price),
    drawPrice(price)
{
    validateParameters(url, countProduct);
    pricesData.push_back(price);
}
void Store::validateParameters(const string& url, unsigned int dayMonitoring, unsigned int countProduct, unsigned int price) const
{
    // 1. Проверка URL
    if (url.empty()) {
        throw StoreException(StoreException::INVALID_URL, "URL не может быть пустым");
    }

    if (url.find("https://") != 0) {
        throw StoreException(StoreException::INVALID_URL, "URL должен начинаться с 'https://'");
    }

    if (url.length() < 12) { // "https://a.ru" - минимальная длина
        throw StoreException(StoreException::INVALID_URL, "URL слишком короткий");
    }

    if (url.find(' ') != string::npos) {
        throw StoreException(StoreException::INVALID_URL, "URL не должен содержать пробелов");
    }

    // 2. Проверка времени мониторинга
    if (dayMonitoring == 0) {
        throw StoreException(StoreException::ZERO_PARAM, "Время мониторинга не может быть нулевым");
    }

    if (dayMonitoring > 365) {
        throw StoreException(StoreException::TIME_EXCEEDED, "Время мониторинга не может превышать 365 дней");
    }

    // 3. Проверка количества товара
    if (countProduct == 0) {
        throw StoreException(StoreException::SOLD_OUT, "Товар отсутствует на складе");
    }

    // 4. Проверка цены
    if (price == 0) {
        throw StoreException(StoreException::INVALID_PRICE, "Цена не может быть нулевой");
    }

    if (price < 0) {
        throw StoreException(StoreException::INVALID_PRICE, "Цена слишком низкая");
    }
    if (price > OVERFLOW_PRICE) {
        throw StoreException(StoreException::INVALID_PRICE, "Цена слишком высокая");
    }

    // 5. Дополнительные проверки URL
    size_t dotPos = url.find('.', 8); // Ищем точку после "https://"
    if (dotPos == string::npos) {
        throw StoreException(StoreException::INVALID_URL, "URL должен содержать доменное имя");
    }

    if (url.find("..") != string::npos) {
        throw StoreException(StoreException::INVALID_URL, "URL содержит недопустимые символы '..'");
    }

    // 6. Проверка на критически низкий остаток
    if (countProduct < 5) {
        throw StoreException(StoreException::LOW_STOCK_ALERT, "Критически низкий остаток товара");
    }
};
void Store::validateParameters(const std::string& url, unsigned int countProduct) const {
    // 1. Проверка URL
    if (url.empty()) {
        throw StoreException(StoreException::INVALID_URL, "URL не может быть пустым");
    }

    if (url.find("https://") != 0) {
        throw StoreException(StoreException::INVALID_URL, "URL должен начинаться с 'https://'");
    }

    if (url.length() < 12) { // "https://a.ru" - минимальная длина
        throw StoreException(StoreException::INVALID_URL, "URL слишком короткий");
    }

    if (url.find(' ') != string::npos) {
        throw StoreException(StoreException::INVALID_URL, "URL не должен содержать пробелов");
    }
    // 3. Проверка количества товара
    if (countProduct == 0) {
        throw StoreException(StoreException::SOLD_OUT, "Товар отсутствует на складе");
    }

};
void Store::validateUpdate(unsigned int newPrice, unsigned int newCountProduct) const
{
    // 1. Проверка цены
    if (newPrice == 0) {
        throw StoreException(StoreException::INVALID_PRICE, "Цена не может быть нулевой");
    }

    if (newPrice < 0) {
        throw StoreException(StoreException::INVALID_PRICE, "Цена слишком низкая");
    }
    // 2. Проверка количества товара
    if (countProduct == 0) {
        throw StoreException(StoreException::SOLD_OUT, "Товар отсутствует на складе");
    }

};
unsigned int Store::getCountDay() const 
{
    return countDay; 
}
string Store::getUrl() const 
{
    return url; 
}
unsigned int Store::getPrice() const 
{
    return price;
}
unsigned int Store::getCountProduct() const 
{ 
    return countProduct;
}
unsigned int Store::getDayMonitoring() const 
{
    return dayMonitoring;
}
unsigned int Store::getMaxPrice() const 
{
    return maxPrice;
}
unsigned int Store::getMinPrice() const 
{ 
    return minPrice; 
}
unsigned int Store::getDrawPrice() const 
{
    return drawPrice;
}

bool Store::checkExtraBuy() const
{
    return countProduct < 2;
}

Store::PriceTrend Store::checkDynamic() const
{
    if (pricesData.size() < 2) {
        throw StoreException(StoreException::LITTLE_DATA, "Слишком мало элементов в pricesData");
        
    }

    short int difference = pricesData[pricesData.size() - 2] - pricesData[pricesData.size() - 1];

    return (difference == 0 ? 
             PriceTrend::STABLE
                : 
                 (difference > 0 ? 
                     (difference == maxPrice ? 
                         PriceTrend::AT_MAX
                         : 
                         PriceTrend::RISING)
                     :
                     (difference == minPrice ? 
                         PriceTrend::AT_MIN 
                         : 
                         PriceTrend::FALLING)));
}

void Store::updateData(unsigned int newPrice, unsigned int newCountProduct)
{
    validateUpdate(newPrice, newCountProduct);
    price = newPrice;
    countProduct = newCountProduct;
    countDay++;

    pricesData.push_back(newPrice);

    if (newPrice > maxPrice) {
        maxPrice = newPrice;
    }
    if (newPrice < minPrice || minPrice == 0) {
        minPrice = newPrice;
    }
}

const unsigned int& Store::operator[] (size_t index) const
{
    return pricesData[index];
}

void Store::printInfo() const
{
    cout << "=== СВОДНАЯ ИНФОРМАЦИЯ ===" << endl;
    cout << "URL: " << url << endl;
    cout << "Текущая цена: " << price << endl;
    cout << "Остаток товара: " << countProduct << endl;
    cout << "Дней мониторинга: " << countDay << " из " << dayMonitoring << endl;
    cout << "Максимальная цена: " << maxPrice << endl;
    cout << "Минимальная цена: " << minPrice << endl;
    cout << "Рекомендация к покупке: " << (checkExtraBuy() ? "СРОЧНО КУПИТЬ!" : "Можно подождать") << endl;
    cout << "==========================" << endl;
}

void Store::printInfo(size_t day) const
{
    if (day >= pricesData.size()) {
        cout << "Ошибка: день " << day << " выходит за пределы данных" << endl;
        return;
    }

    cout << "=== СВОДНАЯ ИНФОРМАЦИЯ ПО " << day << " ДНЮ ===" << endl;
    cout << "URL: " << url << endl;
    cout << "Цена: " << pricesData[day] << endl;
    cout << "Рекомендация к покупке: " << (checkExtraBuy() ? "СРОЧНО КУПИТЬ!" : "Можно подождать") << endl;
    cout << "==========================" << endl;
}
std::ostream& operator<<(std::ostream& os, const Store& obj) {
    os << "=== СВОДНАЯ ИНФОРМАЦИЯ ===" << endl;
    os << "URL: " << obj.url << endl;
    os << "Текущая цена: " << obj.price << endl;
    os << "Остаток товара: " << obj.countProduct << endl;
    os << "Дней мониторинга: " << obj.countDay << " из " << obj.dayMonitoring << endl;
    os << "Максимальная цена: " << obj.maxPrice << endl;
    os << "Минимальная цена: " << obj.minPrice << endl;
    os << "Рекомендация к покупке: " << (obj.checkExtraBuy() ? "СРОЧНО КУПИТЬ!" : "Можно подождать") << endl;
    os << "==========================" << endl;
    return os;
}