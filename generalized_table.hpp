#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include "iterator_pattern.hpp"
enum class node_type {
    HEAD_TYPE,
    VALUE_TYPE,
    SUB_TABLE_TYPE
};
struct generalized_node {
    node_type type;
    union {
        int value;
        generalized_node* sub_table;
    };
    generalized_node *next = nullptr;
    generalized_node(node_type t) : type(t) { sub_table = nullptr; }
    generalized_node(node_type t, int val) : type(t), value(val) { }
};
class generalize_table : public container<int> {
private:
    const char* valid_expression_ = nullptr;
    generalized_node *head_ = nullptr;
    std::vector<int>vec_;
public:
    generalize_table() = default;
    virtual ~generalize_table() {
        uninit(head_);
    }
    // [1,[4,[6]]] or [[1,1],2,[1,1]] or [1,1,2,1,1]
    bool init(const char* expression) {
        if (false == check_expression(expression)) {
            return false;
        }
        valid_expression_ = expression;
        head_ = create(valid_expression_);
        if (head_ != nullptr) {
            transfer(head_);
            return true;
        }
        return false;
    }
    void print() const {
        print(head_);
    }
    int depth() const {
        return depth(head_);
    }
    virtual size_t size() const override  {
        return size(head_);
    }
    virtual int& operator[] (size_t index) override {
        return vec_[index];
    }
    virtual void push(const int& e) override {
    }
    bool check_expression(const char* expression) {
        if (nullptr == expression) {
            return false;
        } 
        int size = strlen(expression);
        if ((0 == size) || (false == is_left_bracket(*expression))) {
            return false;
        }
        int bracket_num = 0;
        for (int i = 0;i < size;i++) {
            char ch = expression[i];
            if (is_left_bracket(ch)) {
                ++bracket_num;
            }
            else if (is_right_bracket(ch)) {
                --bracket_num;
            }
            else if (!isdigit(ch) && (ch != ',')) {
                return false;
            }
            else if (',' == ch) {
                if ((i - 1 < 0) || (i + 1 >= size)) {
                    return false;
                }
                char pre_ch = expression[i - 1];
                char next_ch = expression[i + 1];
                if ((',' == pre_ch) || (',' == next_ch)) {
                    return false;
                }
            }
        }
        return 0 == bracket_num;
    }
    inline bool is_left_bracket(char ch) {
        return ('(' == ch) || ('[' == ch) || ('{' == ch);
    }
    inline bool is_right_bracket(char ch) {
        return (')' == ch) || (']' == ch) || ('}' == ch);
    }
private:
    generalized_node* create(const char*& str) {
        assert(is_left_bracket(*str));
        generalized_node* head = new generalized_node(node_type::HEAD_TYPE);
        generalized_node* prev = head;
        ++str;
        std::string tmp;
        while (*str) {
            if (isdigit(*str)) {
                tmp += *str++;
                continue;
            }
            if (false == tmp.empty()) {
                generalized_node *node = new generalized_node(node_type::VALUE_TYPE, atoi(tmp.c_str()));
                tmp.clear();
                prev->next = node;
                prev = prev->next;
            }
            if (is_left_bracket(*str)) {
                generalized_node* node = new generalized_node(node_type::SUB_TABLE_TYPE);
                prev->next = node;
                prev = prev->next;
                prev->sub_table = create(str);
            }
            else if (is_right_bracket(*str)) {
                prev->next = nullptr;
                return head;
            }
            ++str;
        }
        if (false == tmp.empty()) {
            generalized_node *node = new generalized_node(node_type::VALUE_TYPE, atoi(tmp.c_str()));
            prev->next = node;
        }
        return head;
    }
    void print(generalized_node *node) const {
        generalized_node *current = node;
        while (current != nullptr) {
            if (node_type::VALUE_TYPE == current->type) {
                std::cout << current->value;
                if (current->next != nullptr) {
                    std::cout << ",";
                }
            }
            else if (node_type::SUB_TABLE_TYPE == current->type) {
                print(current->sub_table);
                if (current->next != nullptr) {
                    std::cout << ",";
                }
            }
            else {
                std::cout << "(";
            }
            current = current->next;
        }
        std::cout << ")";
    }
    void transfer(generalized_node *node) {
        generalized_node *current = node;
        while (current != nullptr) {
            if (node_type::VALUE_TYPE == current->type) {
                vec_.push_back(current->value);
            }
            else if (node_type::SUB_TABLE_TYPE == current->type) {
                transfer(current->sub_table);
            }
            current = current->next;
        }
    }
    int depth(generalized_node *node) const {
        generalized_node *current = node;
        int max_depth = 1;
        while (current != nullptr) {
            int current_depth = 1;
            if (node_type::SUB_TABLE_TYPE == current->type) {
                current_depth += depth(current->sub_table);
                if (current_depth > max_depth) {
                    max_depth = current_depth;
                }
            }
            current = current->next;
        }
        return max_depth;
    }
    size_t size(generalized_node *node) const {
        generalized_node *current = node;
        size_t count = 0;
        while (current != nullptr) {
            if (node_type::VALUE_TYPE == current->type) {
                ++count;
            }
            else if (node_type::SUB_TABLE_TYPE == current->type) {
                count += size(current->sub_table);
            }
            current = current->next;
        }
        return count;
    }
    void uninit(generalized_node *&node) {
        generalized_node *current = node;
        while (current != nullptr) {
            generalized_node* tmp = current;
            if (node_type::SUB_TABLE_TYPE == current->type) {
                uninit(current->sub_table);
            }
            current = current->next;
            delete tmp;
            tmp = nullptr;
        }
    }
};