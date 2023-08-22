#pragma once

#include <queue>
#include <vector>

namespace nnmcpp::parsing {

template <typename T>
struct Stream {
public:
    Stream();
    Stream(const std::vector<T>& elements);

    T Peek() const;
    T Read();
    void Put(const T& elem);

    bool IsBroken();
    void Break();

private:
    std::queue<T> buf;
    bool broken_;

};

template <typename T>
Stream<T>::Stream() :
    buf(),
    broken_(false) {}

template <typename T>
Stream<T>::Stream(const std::vector<T>& elements) :
    Stream() {

    for (auto elem : elements) {
        Put(elem);
    }
}

template <typename T>
T Stream<T>::Peek() const {
    if (buf.empty())
        return T();

    return buf.front();
}

template <typename T>
T Stream<T>::Read() {
    if (buf.empty())
        return T();

    T elem = buf.front();
    buf.pop();
    return elem;
}

template <typename T>
void Stream<T>::Put(const T& elem) {
    buf.push(elem);
}

template <typename T>
void Stream<T>::Break() {
    broken_ = true;
}

template <typename T>
bool Stream<T>::IsBroken() {
    return broken_;
}

} // namespace nnmcpp::parsing
