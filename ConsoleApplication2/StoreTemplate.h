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
        return static_cast<int>(std::round(value * 100));  // ����� -> �������
    }

    T convertFromKopecks(T kopecks) const {
        return kopecks / 100.0; // ������� -> �����
    }

    void validateParameters(const std::string& url, unsigned int dayMonitoring, unsigned int countProduct, T price) const {
        // 1. �������� URL
        if (url.empty()) {
            throw StoreException(StoreException::INVALID_URL, "URL �� ����� ���� ������");
        }

        if (url.find("https://") != 0) {
            throw StoreException(StoreException::INVALID_URL, "URL ������ ���������� � 'https://'");
        }

        if (url.length() < 12) {
            throw StoreException(StoreException::INVALID_URL, "URL ������� ��������");
        }

        if (url.find(' ') != std::string::npos) {
            throw StoreException(StoreException::INVALID_URL, "URL �� ������ ��������� ��������");
        }

        // 2. �������� ������� �����������
        if (dayMonitoring == 0) {
            throw StoreException(StoreException::ZERO_PARAM, "����� ����������� �� ����� ���� �������");
        }

        if (dayMonitoring > 365) {
            throw StoreException(StoreException::TIME_EXCEEDED, "����� ����������� �� ����� ��������� 365 ����");
        }

        // 3. �������� ���������� ������
        if (countProduct == 0) {
            throw StoreException(StoreException::SOLD_OUT, "����� ����������� �� ������");
        }

        // 4. �������� ���� (���������� ��� ���������� ����)
        if (price <= 0) {
            throw StoreException(StoreException::INVALID_PRICE, "���� ������ ���� �������������");
        }

        if (convertToKopecks(price) > OVERFLOW_PRICE) {
            throw StoreException(StoreException::INVALID_PRICE, "���� ������� �������");
        }

        // 5. �������������� �������� URL
        size_t dotPos = url.find('.', 8);
        if (dotPos == std::string::npos) {
            throw StoreException(StoreException::INVALID_URL, "URL ������ ��������� �������� ���");
        }

        if (url.find("..") != std::string::npos) {
            throw StoreException(StoreException::INVALID_URL, "URL �������� ������������ ������� '..'");
        }

        // 6. �������� �� ���������� ������ �������
        if (countProduct < 5) {
            throw StoreException(StoreException::LOW_STOCK_ALERT, "���������� ������ ������� ������");
        }
    }

    void validateParameters(const std::string& url, unsigned int countProduct) const {
        // 1. �������� URL
        if (url.empty()) {
            throw StoreException(StoreException::INVALID_URL, "URL �� ����� ���� ������");
        }

        if (url.find("https://") != 0) {
            throw StoreException(StoreException::INVALID_URL, "URL ������ ���������� � 'https://'");
        }

        if (url.length() < 12) {
            throw StoreException(StoreException::INVALID_URL, "URL ������� ��������");
        }

        if (url.find(' ') != std::string::npos) {
            throw StoreException(StoreException::INVALID_URL, "URL �� ������ ��������� ��������");
        }

        // 2. �������� ���������� ������
        if (countProduct == 0) {
            throw StoreException(StoreException::SOLD_OUT, "����� ����������� �� ������");
        }
    }

    void validateUpdate(T newPrice, unsigned int newCountProduct) const {
        // 1. �������� ���� (���������� ��� ���������� ����)
        if (newPrice <= 0) {
            throw StoreException(StoreException::INVALID_PRICE, "���� ������ ���� �������������");
        }

        // 2. �������� ���������� ������ (����������: newCountProduct ������ countProduct)
        if (newCountProduct == 0) {
            throw StoreException(StoreException::SOLD_OUT, "����� ����������� �� ������");
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

    // �������� ������� �������
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
            throw StoreException(StoreException::LITTLE_DATA, "������� ���� ��������� � pricesData");
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
        std::cout << "=== ������� ���������� ===" << std::endl;
        std::cout << "URL: " << url << std::endl;
        std::cout << "������� ����: " << price << std::endl;
        std::cout << "������� ������: " << countProduct << std::endl;
        std::cout << "��������� �������: " << getThresholdInRubles() << " ���." << std::endl;
        std::cout << "������ �������: " << (isLowStock() ? "��" : "���") << std::endl;
        std::cout << "���� �����������: " << countDay << " �� " << dayMonitoring << std::endl;
        std::cout << "������������ ����: " << maxPrice << std::endl;
        std::cout << "����������� ����: " << minPrice << std::endl;
        std::cout << "������������ � �������: " << (checkExtraBuy() ? "������ ������!" : "����� ���������") << std::endl;
        std::cout << "==========================" << std::endl;
    }

    void printInfo(size_t day) const {
        if (day >= pricesData.size()) {
            std::cout << "������: ���� " << day << " ������� �� ������� ������" << std::endl;
            return;
        }

        std::cout << "=== ������� ���������� �� " << day << " ��� ===" << std::endl;
        std::cout << "URL: " << url << std::endl;
        std::cout << "����: " << pricesData[day] << std::endl;
        std::cout << "��������� �������: " << getThresholdInRubles() << " ���." << std::endl;
        std::cout << "������������ � �������: " << (checkExtraBuy() ? "������ ������!" : "����� ���������") << std::endl;
        std::cout << "==========================" << std::endl;
    }

    virtual ~StoreTemplate() = default;
};

template <typename T, int Threshold>
std::ostream& operator<<(std::ostream& os, const StoreTemplate<T, Threshold>& obj) {
    os << "=== ������� ���������� ===" << std::endl;
    os << "URL: " << obj.url << std::endl;
    os << "������� ����: " << obj.price << std::endl;
    os << "������� ������: " << obj.countProduct << std::endl;
    os << "��������� �������: " << obj.getThresholdInRubles() << " ���." << std::endl;
    os << "������ �������: " << (obj.isLowStock() ? "��" : "���") << std::endl;
    os << "���� �����������: " << obj.countDay << " �� " << obj.dayMonitoring << std::endl;
    os << "������������ ����: " << obj.maxPrice << std::endl;
    os << "����������� ����: " << obj.minPrice << std::endl;
    os << "������������ � �������: " << (obj.checkExtraBuy() ? "������ ������!" : "����� ���������") << std::endl;
    os << "==========================" << std::endl;
    return os;
}