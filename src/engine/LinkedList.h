#pragma once 


#include <cstddef>
#include <stdexcept>


template<typename T>  // TODO: make desctructor
class LinkedList {
protected:
    struct Node {
        Node* next = nullptr;
        Node* prev = nullptr;
        T data = nullptr;

        Node (T data) : data{data} {}
    };
public:
    void Push(T data) {
        Node* new_node = new Node(data);
        if (size_ == 0) {
            head_ = new_node;
            tail_ = new_node;
            size_ = 1;
        } else if (size_ == 1) {
            tail_ = new_node;
            tail_->prev = head_;
            head_->next = tail_;
            size_ = 2;
        } else {
            ++size_;
            tail_->next = new_node;
            new_node->prev = tail_;
            tail_ = new_node;
        }
    }

    void Erase(size_t index) {
        if (index >= size_) {
            throw std::invalid_argument("Index out of range");
        }

        if (size_ == 1) {
            delete head_;
            head_ = nullptr;
            tail_ = nullptr;
            size_ = 0;
            return;
        } 

        auto current = head_;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }

        auto next = current->next;
        auto prev = current->prev;

        if (index == 0) {
            head_ = next;
        } else if (index == size_ - 1) {
            tail_ = prev;
        }

        if (next) next->prev = prev;
        if (prev) prev->next = next;

        --size_;
        delete current;
    }

    T& operator[] (size_t index) {
        if (index >= size_) {
            throw std::invalid_argument("Index out of range");
        }

        auto current = head_;
        while (index --> 0) {
            current = current->next;
        }
        return current->data;
    }

    size_t size() {
        return size_;
    }

    void Sort() {

        auto less = std::less<T>{};
        auto is_sorted = [less](LinkedList<T> &sp) {
            for (int i = 0; i < sp.size() - 1; ++i) {
                if (less(sp[i + 1], sp[i])) {
                    return false;
                }
            }
            return true;
        };

        while (!is_sorted(*this)) {
            for (int i = 0; i < size() - 1; ++i) {
                if (less((*this)[i + 1], (*this)[i])) {
                    std::swap((*this)[i], (*this)[i + 1]);
                }
            }
        }        
        
    }

protected:
    size_t size_ = 0;
    Node* head_ = nullptr;
    Node* tail_ = nullptr;
};