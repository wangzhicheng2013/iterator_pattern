#pragma once
#include <stdio.h>
#include <cassert>
#include <memory>
template <class iterm_type>
class container;


template <class iterm_type>
class simple_iterator {
private:
    container<iterm_type>*container_;
    size_t current_pos_ = 0;
public:
    simple_iterator(container<iterm_type>*c) : container_(c) { 
        assert(container_ != nullptr);
    }
    inline bool has_next() const {
        return current_pos_ < container_->size();
    }
    inline iterm_type& get_next() {
        return (*container_)[current_pos_++];
    }
};

template <class iterm_type>
class container {
public:
    using iterator_type = simple_iterator<iterm_type>;
    std::unique_ptr<iterator_type> make_iterator() {
        return std::make_unique<iterator_type>(this);
    }
    virtual size_t size() const = 0;
    virtual iterm_type& operator[] (size_t index) = 0;
    virtual void push(const iterm_type& e) = 0;
};