#include <iostream>
#include "Vector.h" 
#include "Set.h" 

using namespace std;

int main() {
    
    Vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    cout << "Vector v: " << v << endl;

    Set<int> set1;
    set1.insert(1);
    set1.insert(2);
    set1.insert(3);

    Set<int> set2;
    set2.insert(2);
    set2.insert(3);
    set2.insert(4);

    cout << "Set 1: ";
    set1.display();
    cout << "Set 2: ";
    set2.display();

    Set<int> unionSet = set1 + set2;
    cout << "Union: ";
    unionSet.display();

    Set<int> differenceSet = set1 - set2;
    cout << "Difference: ";
    differenceSet.display();

    Set<int> intersectionSet = set1 * set2;
    cout << "Intersection: ";
    intersectionSet.display();

    Set<int> symmetricDifferenceSet = set1 ^ set2;
    cout << "Symmetric Difference: ";
    symmetricDifferenceSet.display();

    if (set1 == set2) {
        cout << "Set 1 and Set 2 are equal (int)." << endl;
    }
    else {
        cout << "Set 1 and Set 2 are not equal (int)." << endl;
    }

    cout << "Set 1 before += operation (int): ";
    set1.display();
    set1 += set2;
    cout << "Set 1 after += operation (int): ";
    set1.display();

    
    Set<string> setStr1;
    setStr1.insert("Apple");
    setStr1.insert("Banana");
    setStr1.insert("Cherry");

    Set<string> setStr2;
    setStr2.insert("Banana");
    setStr2.insert("Date");
    setStr2.insert("Elderberry");

    cout << "Set 1 (string): ";
    setStr1.display();
    cout << "Set 2 (string): ";
    setStr2.display();

    Set<string> unionSetStr = setStr1 + setStr2;
    cout << "Union (string): ";
    unionSetStr.display();

    Set<string> differenceSetStr = setStr1 - setStr2;
    cout << "Difference (string): ";
    differenceSetStr.display();

    Set<string> intersectionSetStr = setStr1 * setStr2;
    cout << "Intersection (string): ";
    intersectionSetStr.display();

    Set<string> symmetricDifferenceSetStr = setStr1 ^ setStr2;
    cout << "Symmetric Difference (string): ";
    symmetricDifferenceSetStr.display();

    if (setStr1 == setStr2) {
        cout << "Set 1 and Set 2 are equal (string)." << endl;
    }
    else {
        cout << "Set 1 and Set 2 are not equal (string)." << endl;
    }

    cout << "Set 1 before += operation (string): ";
    setStr1.display();
    setStr1 += setStr2;
    cout << "Set 1 after += operation (string): ";
    setStr1.display();

    system("pause");
    return 0;
}
