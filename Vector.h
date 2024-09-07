#pragma once

#include <iostream>
#include <stdexcept>
using namespace std;

template <typename T>
class Vector {
private:
    T* elements;
    int size_;
    int capacity_;

    void resize() {
        int newCapacity = capacity_ * 2;
        T* temp = new T[newCapacity];
        for (int i = 0; i < size_; ++i) {
            temp[i] = elements[i];
        }
        delete[] elements;
        elements = temp;
        capacity_ = newCapacity;
    }

    void reduceCapacity() {
        int newCapacity = capacity_ / 2;
        T* temp = new T[newCapacity];
        for (int i = 0; i < size_; ++i) {
            temp[i] = elements[i];
        }
        delete[] elements;
        elements = temp;
        capacity_ = newCapacity;
    }

public:
    Vector() : elements(new T[10]), size_(0), capacity_(10) {}

    Vector(int currentcapacity) : elements(new T[currentcapacity]), size_(0), capacity_(currentcapacity) {}

    ~Vector() {
        delete[] elements;
    }

    int size() const {
        return size_;
    }

    int capacity() const {
        return capacity_;
    }

    bool empty() const {
        return size_ == 0;
    }

    void push_back(const T& value) {
        if (size_ >= capacity_) {
            resize();
        }
        elements[size_++] = value;
    }

    void pop_back() {
        if (size_ == 0) {
            cerr << "Vector is empty" << endl;
            return;
        }
        --size_;
        if (size_ < capacity_ / 2 && capacity_ > 10) {
            reduceCapacity();
        }
    }

    T& front() {
        if (size_ == 0) {
            throw out_of_range("Vector is empty");
        }
        return elements[0];
    }

    T& back() {
        if (size_ == 0) {
            throw out_of_range("Vector is empty");
        }
        return elements[size_ - 1];
    }

    void insert(int index, const T& value) {
        if (index < 0 || index > size_) {
            throw out_of_range("Index out of range");
        }
        if (size_ >= capacity_) {
            resize();
        }
        for (int i = size_; i > index; --i) {
            elements[i] = elements[i - 1];
        }
        elements[index] = value;
        ++size_;
    }

    T& at(int index) {
        if (index < 0 || index >= size_) {
            throw out_of_range("Index out of range");
        }
        return elements[index];
    }

    void erase(int index) {
        if (index < 0 || index >= size_) {
            throw out_of_range("Index out of range");
        }
        for (int i = index; i < size_ - 1; ++i) {
            elements[i] = elements[i + 1];
        }
        --size_;
        if (size_ < capacity_ / 2 && capacity_ > 10) {
            reduceCapacity();
        }
    }

    void swap(Vector& other) {
        T* temp_elements = elements;
        elements = other.elements;
        other.elements = temp_elements;

        int temp_size = size_;
        size_ = other.size_;
        other.size_ = temp_size;

        int temp_capacity = capacity_;
        capacity_ = other.capacity_;
        other.capacity_ = temp_capacity;
    }

    void clear() {
        delete[] elements;
        elements = new T[10];
        size_ = 0;
        capacity_ = 10;
    }

    T* begin() const {
        return elements;
    }

    T* end() const {
        return elements + size_;
    }

    friend ostream& operator<<(ostream& os, const Vector<T>& vec) {
        os << "The elements of the vector:" << endl;
        for (int i = 0; i < vec.size(); i++) {
            os << vec.elements[i] << " ";
        }
        os << endl;

        os << "Size of Vector: " << vec.size_ << endl;
        os << "Capacity of Vector: " << vec.capacity_ << endl;

        return os;
    }

    bool operator==(const Vector<T>& other) const {
        if (size_ != other.size_) {
            return false;
        }
        for (int i = 0; i < size_; ++i) {
            if (elements[i] != other.elements[i]) {
                return false;
            }
        }
        return true;
    }

    T& operator[](int index) {
        return elements[index];
    }

    Vector<T>& operator=(const Vector<T>& other) {
        if (this == &other) {
            return *this;
        }
        delete[] elements;
        size_ = other.size_;
        capacity_ = other.capacity_;
        elements = new T[capacity_];
        for (int i = 0; i < size_; ++i) {
            elements[i] = other.elements[i];
        }
        return *this;
    }
};
