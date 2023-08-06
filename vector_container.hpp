#include "iterator_pattern.hpp"
#include <vector>
template <class iterm_type>
class vector_container : public container<iterm_type> {
private:
    std::vector<iterm_type>vec_;
public:
    virtual size_t size() const override {
        return vec_.size();
    }
    virtual iterm_type& operator[] (size_t index) override {
        return vec_[index];
    }
    virtual void push(const iterm_type& e) override {
        vec_.push_back(e);
    }
};