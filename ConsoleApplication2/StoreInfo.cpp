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
{} // TODO ��������� �������
float StoreInfo::getRating() const { return rating; }
unsigned int StoreInfo::getPositiveReviews() const { return positiveReviews; }
unsigned int StoreInfo::getNegativeReviews() const { return negativeReviews; }
void StoreInfo::setReviews(bool Reviews) {
    Reviews ? positiveReviews++ : negativeReviews++;
    rating = positiveReviews / (negativeReviews + positiveReviews) * 5.0f;
}

void StoreInfo::printInfo() const { // TODO �������� ��������
    Store::printInfo();
    cout << "=== �������������� ���������� ===" << endl;
    cout << "���� �����������: " << getDayMonitoring() << endl;
    cout << "�������: " << rating << endl;
    cout << "���-�� ������������� �������: " << positiveReviews << endl;
    cout << "���-�� ������������� �������: " << negativeReviews << "\n\n";
}
std::ostream& operator<<(std::ostream& os, const StoreInfo& obj) {
    os << "=== �������������� ���������� ===" << std::endl;
    os << "�������: " << obj.rating << std::endl;
    os << "���-�� ������������� �������: " << obj.positiveReviews << std::endl;
    os << "���-�� ������������� �������: " << obj.negativeReviews << std::endl;
    return os;
}