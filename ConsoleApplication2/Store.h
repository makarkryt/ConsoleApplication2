#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "StoreException.h"
class Store
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
    unsigned int price;
    unsigned int countProduct;
    unsigned int dayMonitoring;
    std::vector<unsigned int> pricesData;
    unsigned int maxPrice;
    unsigned int minPrice;
    unsigned int drawPrice;
    const unsigned int OVERFLOW_PRICE{ 10000000 };
    void validateParameters(const std::string & url, unsigned int dayMonitoring, unsigned int countProduct, unsigned int price) const;
    void validateParameters(const std::string & url, unsigned int countProduct) const;
    void validateUpdate(unsigned int newPrice, unsigned int newCountProduct) const;
public:
    Store(const std::string & url, unsigned int dayMonitoring, unsigned int countProduct, unsigned int price);
    Store(const std::string & url, unsigned int countProduct);
    unsigned int getCountDay() const;
    std::string getUrl() const;
    unsigned int getPrice() const;
    unsigned int getCountProduct() const;
    unsigned int getDayMonitoring() const;
    unsigned int getMaxPrice() const;
    unsigned int getMinPrice() const;
    unsigned int getDrawPrice() const;
    bool checkExtraBuy() const;
    PriceTrend checkDynamic() const;
    void updateData(unsigned int newPrice, unsigned int newCountProduct);
    const unsigned int& operator[] (size_t index) const;
    void printInfo() const;
    void printInfo(size_t day) const;
    friend std::ostream& operator<<(std::ostream& os, const Store& obj);
};

