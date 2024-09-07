#pragma once

#include <iostream>
#include "Vector.h"

using namespace std;

template <typename T>
class Set {
private:
    Vector<T> elements;

public:
    void insert(const T& value) {
        if (!contains(value)) {
            elements.push_back(value);
        }
    }

    void erase(const T& value) {
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if (*it == value) {
                elements.erase(it - elements.begin());
                break;
            }
        }
    }

    bool contains(const T& value) const {
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if (*it == value) {
                return true;
            }
        }
        return false;
    }

    void clear() {
        elements.clear();
    }

    int size() const {
        return elements.size();
    }

    friend ostream& operator<<(ostream& os, const Set<T>& s) {
        os << "{ ";
        for (auto it = s.elements.begin(); it != s.elements.end(); ++it) {
            os << *it << " ";
        }
        os << "}";
        return os;
    }

    Set<T> operator+(const Set<T>& other) const {
        Set<T> result;
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            result.insert(*it);
        }
        for (auto it = other.elements.begin(); it != other.elements.end(); ++it) {
            result.insert(*it);
        }
        return result;
    }

    Set<T> operator-(const Set<T>& other) const {
        Set<T> result;
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if (!other.contains(*it)) {
                result.insert(*it);
            }
        }
        return result;
    }

    Set<T> operator*(const Set<T>& other) const {
        Set<T> result;
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if (other.contains(*it)) {
                result.insert(*it);
            }
        }
        return result;
    }

    Set<T> operator^(const Set<T>& other) const {
        Set<T> result;
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if (!other.contains(*it)) {
                result.insert(*it);
            }
        }
        for (auto it = other.elements.begin(); it != other.elements.end(); ++it) {
            if (!contains(*it)) {
                result.insert(*it);
            }
        }
        return result;
    }

    bool operator==(const Set<T>& other) const {
        if (size() != other.size()) {
            return false;
        }
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if (!other.contains(*it)) {
                return false;
            }
        }
        return true;
    }

    void operator+=(const Set<T>& other) {
        for (auto it = other.elements.begin(); it != other.elements.end(); ++it) {
            insert(*it);
        }
    }

    void display() const {
        cout << *this << endl;
    }
};
