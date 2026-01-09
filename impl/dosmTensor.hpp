#ifndef DOSM_TENSOR_HPP
#define DOSM_TENSOR_HPP

#include "dosmBasic.hpp"
#include <initializer_list>

namespace dosm {

    template<typename T, idx_t... dims>
        struct tensor
        {
            static inline constexpr idx_t rank = sizeof...(dims);
            static inline constexpr idx_t shape[rank] = { dims... };
            static inline constexpr idx_t size = (dims * ... * 1);

            T data[size];
            vector_t<idx_t> axes;

            template<typename... I> T& operator()(I... i);
            template<typename... I> const T& operator()(I... i) const;
            template<typename... I> static constexpr idx_t flat(I... i);
            tensor& operator[](std::initializer_list<idx_t> list);

            T norm(void) const;
            tensor<T, dims...> normalize(void) const;
            auto t(void) const;
        };
    template<typename T, idx_t... dims1> using tensor_t = tensor<T, dims1...>;
    template<typename T, idx_t... dims1> auto operator+(const tensor<T, dims1...>& X, const tensor<T, dims1...>& Y);
    template<typename T, idx_t... dims1> auto operator-(const tensor<T, dims1...>& X, const tensor<T, dims1...>& Y);
    template<typename T, idx_t... dims1> auto operator-(const tensor<T, dims1...>& X);
    template<typename T, idx_t... dims1> auto operator*(const tensor<T, dims1...>& X, T s);
    template<typename T, idx_t... dims1> auto operator*(T s, const tensor<T, dims1...>& X);
    template<typename T, idx_t... dims1, idx_t... dims2> auto operator*(const tensor<T, dims1...>& X, const tensor<T, dims2...>& Y);
    template<typename T, idx_t... dims1> auto operator/(const tensor<T, dims1...>& X, T s);
    template<typename T, idx_t... dims1> auto operator/(T s, const tensor<T, dims1...>& X);
    template<typename T> auto operator^(const tensor<T,3>& X, const tensor<T,3>& Y);
    template<typename T, idx_t... dims1, idx_t... dims2> auto operator|(const tensor<T, dims1...>& X, const tensor<T, dims2...>& Y);
    template<typename T, idx_t... dims1, idx_t... dims2> auto operator||(const tensor<T, dims1...>& X, const tensor<T, dims2...>& Y);

} // namespace dosm

#ifdef DOSM_MATH_EIGEN
#include "dosmTensorEigen.tpp"
#endif

#endif // DOSM_TENSOR_HPP
