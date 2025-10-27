#pragma once
#include "Store.h"

class StoreInfo final : public Store
{
private:
    float rating{ 0.0f }; //от 0 до 5 
    unsigned int positiveReviews;
    unsigned int negativeReviews;
public:
    StoreInfo(
        const std::string& url, 
        unsigned int dayMonitoring, 
        unsigned int countProduct, 
        unsigned int price, 
        unsigned int positiveReviews, 
        unsigned int negativeReviews);
    float getRating() const;
    unsigned int getPositiveReviews() const;
    unsigned int getNegativeReviews() const;
    void setReviews(bool Reviews);
    friend std::ostream& operator<<(std::ostream& os, const StoreInfo& obj);
    void printInfo() const override;
};

