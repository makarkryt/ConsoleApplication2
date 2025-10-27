#include "StoreInfo.h"
using namespace std;
StoreInfo::StoreInfo(
    const string& url, 
    unsigned int dayMonitoring, 
    unsigned int countProduct, 
    unsigned int price, 
    const unsigned int positiveReviews, 
    const unsigned int negativeReviews):
	Store(url, dayMonitoring, countProduct, price),
    rating(positiveReviews / (negativeReviews + positiveReviews) * 5.0f),
    positiveReviews(positiveReviews),
    negativeReviews(negativeReviews)
{} // TODO расчитать рейтинг
float StoreInfo::getRating() const { return rating; }
unsigned int StoreInfo::getPositiveReviews() const { return positiveReviews; }
unsigned int StoreInfo::getNegativeReviews() const { return negativeReviews; }
void StoreInfo::setReviews(bool Reviews) {
    Reviews ? positiveReviews++ : negativeReviews++;
    rating = positiveReviews / (negativeReviews + positiveReviews) * 5.0f;
}

void StoreInfo::printInfo() const { // TODO изменить название
    Store::printInfo();
    cout << "=== ƒќѕќЋЌ»“≈Ћ№Ќјя »Ќ‘ќ–ћј÷»я ===" << endl;
    cout << "ƒень мониторинга: " << getDayMonitoring() << endl;
    cout << "–ейтинг: " << rating << endl;
    cout << " ол-во положительных отзывов: " << positiveReviews << endl;
    cout << " ол-во отрицательных отзывов: " << negativeReviews << "\n\n";
}
std::ostream& operator<<(std::ostream& os, const StoreInfo& obj) {
    os << "=== ƒќѕќЋЌ»“≈Ћ№Ќјя »Ќ‘ќ–ћј÷»я ===" << std::endl;
    os << "–ейтинг: " << obj.rating << std::endl;
    os << " ол-во положительных отзывов: " << obj.positiveReviews << std::endl;
    os << " ол-во отрицательных отзывов: " << obj.negativeReviews << std::endl;
    return os;
}