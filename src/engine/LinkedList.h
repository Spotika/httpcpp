#pragma once

#include <stdexcept>


template <class T>
class LinkedList {
public:

    LinkedList() {
        head_->next = tail_;
        tail_->prev = head_;
    }

    void Push(T data) {
        auto new_node = new Node(data);

        std::swap(tail_->data, new_node->data);
        tail_->next = new_node;
        new_node->prev = tail_;
        tail_ = new_node;
        ++size_;
    }

    size_t getSize() const {
        return size_;
    }

    T& operator[](size_t index) const {
        auto current_node = head_->next;

        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        
        while (index --> 0) {
            current_node = current_node->next;
        }
        return *(current_node->data);
    }

    void Sort() {
        auto check = [&](){
            for (int i = 0; i < getSize() - 1; ++i) {
                if (operator[](i) > operator[](i + 1)) {
                    return false;
                }
            }
            return true;
        };

        while (!check()) {
            for (int i = 0; i < getSize() - 1; ++i) {
                if (operator[](i) > operator[](i + 1)) {
                    std::swap(operator[](i), operator[](i + 1));
                }
            }
        }
    }

    void Clear() {
        while (getSize() != 0) {
            Erase_(head_->next); // WARNING: may not workdp
        }
    }

    void Pop(size_t index) {
        if (index >= getSize()) {
            throw std::out_of_range("Index out of range");
        }

        auto current_node = head_->next;
        while (index --> 0) {
            current_node = current_node->next;
        }
        Erase_(current_node);
    }

private:
    struct Node {
        T* data = nullptr;
        Node* prev = nullptr;
        Node* next = nullptr;

        Node() = default;

        Node(
            T data
        ) {
            this->data = new T(data);
        }

        ~Node() {
            if (data) {
                delete data;
            }
        }
    };
    
    void Erase_(Node* target) {
        target->prev->next = target->next;
        target->next->prev = target->prev;
        --size_;
        delete target;
    }

    Node* head_ = new Node();
    Node* tail_ = new Node();
    size_t size_ = 0;
};