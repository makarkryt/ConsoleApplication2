#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "StoreException.h"

template <class T, int Threshold = 1000>
class StoreTemplate
{
private:
    enum PriceTrend {
        STABLE,
        RISING,
        FALLING,
        AT_MIN,
        AT_MAX
    };

    unsigned int countDay;
    std::string url;
    T price;                   
    unsigned int countProduct;
    unsigned int dayMonitoring;
    std::vector<T> pricesData;  
    T maxPrice;                 
    T minPrice;                 
    T drawPrice;                
    const T OVERFLOW_PRICE{ 10000000 };

    T convertToKopecks(T value) const {
        return static_cast<int>(std::round(value * 100));  // Рубли -> копейки
    }

    T convertFromKopecks(T kopecks) const {
        return kopecks / 100.0; // копейки -> Рубли
    }

    void validateParameters(const std::string& url, unsigned int dayMonitoring, unsigned int countProduct, T price) const {
        // 1. Проверка URL
        if (url.empty()) {
            throw StoreException(StoreException::INVALID_URL, "URL не может быть пустым");
        }

        if (url.find("https://") != 0) {
            throw StoreException(StoreException::INVALID_URL, "URL должен начинаться с 'https://'");
        }

        if (url.length() < 12) {
            throw StoreException(StoreException::INVALID_URL, "URL слишком короткий");
        }

        if (url.find(' ') != std::string::npos) {
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

        // 4. Проверка цены (исправлено для шаблонного типа)
        if (price <= 0) {
            throw StoreException(StoreException::INVALID_PRICE, "Цена должна быть положительной");
        }

        if (convertToKopecks(price) > OVERFLOW_PRICE) {
            throw StoreException(StoreException::INVALID_PRICE, "Цена слишком высокая");
        }

        // 5. Дополнительные проверки URL
        size_t dotPos = url.find('.', 8);
        if (dotPos == std::string::npos) {
            throw StoreException(StoreException::INVALID_URL, "URL должен содержать доменное имя");
        }

        if (url.find("..") != std::string::npos) {
            throw StoreException(StoreException::INVALID_URL, "URL содержит недопустимые символы '..'");
        }

        // 6. Проверка на критически низкий остаток
        if (countProduct < 5) {
            throw StoreException(StoreException::LOW_STOCK_ALERT, "Критически низкий остаток товара");
        }
    }

    void validateParameters(const std::string& url, unsigned int countProduct) const {
        // 1. Проверка URL
        if (url.empty()) {
            throw StoreException(StoreException::INVALID_URL, "URL не может быть пустым");
        }

        if (url.find("https://") != 0) {
            throw StoreException(StoreException::INVALID_URL, "URL должен начинаться с 'https://'");
        }

        if (url.length() < 12) {
            throw StoreException(StoreException::INVALID_URL, "URL слишком короткий");
        }

        if (url.find(' ') != std::string::npos) {
            throw StoreException(StoreException::INVALID_URL, "URL не должен содержать пробелов");
        }

        // 2. Проверка количества товара
        if (countProduct == 0) {
            throw StoreException(StoreException::SOLD_OUT, "Товар отсутствует на складе");
        }
    }

    void validateUpdate(T newPrice, unsigned int newCountProduct) const {
        // 1. Проверка цены (исправлено для шаблонного типа)
        if (newPrice <= 0) {
            throw StoreException(StoreException::INVALID_PRICE, "Цена должна быть положительной");
        }

        // 2. Проверка количества товара (ИСПРАВЛЕНО: newCountProduct вместо countProduct)
        if (newCountProduct == 0) {
            throw StoreException(StoreException::SOLD_OUT, "Товар отсутствует на складе");
        }
    }

public:
    StoreTemplate(const std::string& url, unsigned int dayMonitoring, unsigned int countProduct, T price) :
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

    StoreTemplate(const std::string& url, unsigned int countProduct) :
        countDay(1),
        url(url),
        price(10),
        countProduct(countProduct),
        dayMonitoring(30),
        maxPrice(10),
        minPrice(10),
        drawPrice(10)
    {
        validateParameters(url, countProduct);
        pricesData.push_back(price);
    }

    unsigned int getCountDay() const { return countDay; }
    std::string getUrl() const { return url; }
    T getPrice() const { return price; }
    unsigned int getCountProduct() const { return countProduct; }
    unsigned int getDayMonitoring() const { return dayMonitoring; }
    T getMaxPrice() const { return maxPrice; }
    T getMinPrice() const { return minPrice; }
    T getDrawPrice() const { return drawPrice; }

    // проверка низкого остатка
    bool isLowStock() const {
        int stockValue = convertToKopecks(price) * countProduct;
        return stockValue <= Threshold;
    }

    bool checkExtraBuy() const {
        return isLowStock() || countProduct < 2;
    }

    T getThresholdInRubles() const {
        return convertFromKopecks(Threshold);
    }

    PriceTrend checkDynamic() const {
        if (pricesData.size() < 2) {
            throw StoreException(StoreException::LITTLE_DATA, "Слишком мало элементов в pricesData");
        }

        T lastPrice = pricesData[pricesData.size() - 1];
        T prevPrice = pricesData[pricesData.size() - 2];
        T difference = lastPrice - prevPrice;

        if (difference == 0) {
            return PriceTrend::STABLE;
        }
        else if (difference > 0) {
            return (lastPrice == maxPrice) ? PriceTrend::AT_MAX : PriceTrend::RISING;
        }
        else {
            return (lastPrice == minPrice) ? PriceTrend::AT_MIN : PriceTrend::FALLING;
        }
    }

    void updateData(T newPrice, unsigned int newCountProduct) {
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

    const T& operator[] (size_t index) const {
        if (index >= pricesData.size()) {
            throw StoreException("Index out of bounds");
        }
        return pricesData[index];
    }

    void printInfo() const {
        std::cout << "=== СВОДНАЯ ИНФОРМАЦИЯ ===" << std::endl;
        std::cout << "URL: " << url << std::endl;
        std::cout << "Текущая цена: " << price << std::endl;
        std::cout << "Остаток товара: " << countProduct << std::endl;
        std::cout << "Пороговый уровень: " << getThresholdInRubles() << " руб." << std::endl;
        std::cout << "Низкий остаток: " << (isLowStock() ? "ДА" : "НЕТ") << std::endl;
        std::cout << "Дней мониторинга: " << countDay << " из " << dayMonitoring << std::endl;
        std::cout << "Максимальная цена: " << maxPrice << std::endl;
        std::cout << "Минимальная цена: " << minPrice << std::endl;
        std::cout << "Рекомендация к покупке: " << (checkExtraBuy() ? "СРОЧНО КУПИТЬ!" : "Можно подождать") << std::endl;
        std::cout << "==========================" << std::endl;
    }

    void printInfo(size_t day) const {
        if (day >= pricesData.size()) {
            std::cout << "Ошибка: день " << day << " выходит за пределы данных" << std::endl;
            return;
        }

        std::cout << "=== СВОДНАЯ ИНФОРМАЦИЯ ПО " << day << " ДНЮ ===" << std::endl;
        std::cout << "URL: " << url << std::endl;
        std::cout << "Цена: " << pricesData[day] << std::endl;
        std::cout << "Пороговый уровень: " << getThresholdInRubles() << " руб." << std::endl;
        std::cout << "Рекомендация к покупке: " << (checkExtraBuy() ? "СРОЧНО КУПИТЬ!" : "Можно подождать") << std::endl;
        std::cout << "==========================" << std::endl;
    }

    virtual ~StoreTemplate() = default;
};

template <typename T, int Threshold>
std::ostream& operator<<(std::ostream& os, const StoreTemplate<T, Threshold>& obj) {
    os << "=== СВОДНАЯ ИНФОРМАЦИЯ ===" << std::endl;
    os << "URL: " << obj.url << std::endl;
    os << "Текущая цена: " << obj.price << std::endl;
    os << "Остаток товара: " << obj.countProduct << std::endl;
    os << "Пороговый уровень: " << obj.getThresholdInRubles() << " руб." << std::endl;
    os << "Низкий остаток: " << (obj.isLowStock() ? "ДА" : "НЕТ") << std::endl;
    os << "Дней мониторинга: " << obj.countDay << " из " << obj.dayMonitoring << std::endl;
    os << "Максимальная цена: " << obj.maxPrice << std::endl;
    os << "Минимальная цена: " << obj.minPrice << std::endl;
    os << "Рекомендация к покупке: " << (obj.checkExtraBuy() ? "СРОЧНО КУПИТЬ!" : "Можно подождать") << std::endl;
    os << "==========================" << std::endl;
    return os;
}