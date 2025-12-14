#ifndef DOSM_TENSOR_EIGEN_TPP
#define DOSM_TENSOR_EIGEN_TPP

#include <Eigen/Dense>
#include <unsupported/Eigen/CXX11/Tensor>

namespace dosm {

	template<typename T, idx_t... dims>
		template<typename... I>
		T& tensor<T, dims...>::operator()(I... i)
		{
			return data[ flat(i...) ];
		}

	template<typename T, idx_t... dims>
		template<typename... I>
		const T& tensor<T, dims...>::operator()(I... i) const
		{
			return data[ flat(i...) ];
		}

	template<typename T, idx_t... dims>
		template<typename... I>
		constexpr idx_t tensor<T, dims...>::flat(I... i)
		{
			if constexpr (rank == 0)
				return 0;
			idx_t pos[rank] = { static_cast<idx_t>(i)... };
			idx_t out = 0;
			idx_t stride = 1;
			for (idx_t r = 0; r < rank; ++r)
			{
				out += pos[r] * stride;
				stride *= shape[r];
			}
			return out;
		}

	template<typename T, idx_t... dims>
		tensor<T, dims...>& tensor<T, dims...>::operator[](std::initializer_list<idx_t> list)
		{
			axes.assign(list.begin(), list.end());
			return *this;
		}

	template<typename T, idx_t... dims>
		T tensor<T, dims...>::norm(void) const
		{
			if constexpr (rank == 0)
				return (data[0] < 0 ? -data[0] : data[0]);
			Eigen::Map<const Eigen::Matrix<T, size, 1>> v(data);
			return v.norm();
		}

	template<typename T, idx_t... dims>
		tensor<T, dims...> tensor<T, dims...>::normalize(void) const
		{
			tensor<T, dims...> out;
			Eigen::Map<const Eigen::Matrix<T, size, 1>> v(data);
			Eigen::Map<Eigen::Matrix<T, size, 1>>       o(out.data);
			T n = v.norm();
			if (n == 0)
			{
				o.setZero();
				return out;
			}
			o = v / n;
			return out;
		}

	template<typename T, idx_t... dims>
		auto tensor<T, dims...>::t(void) const
		{
			if constexpr (rank == 0)
			{
				tensor<T> out;
				out.data[0] = data[0];
				return out;
			}
			else if constexpr (rank == 1)
			{
				constexpr idx_t n = shape[0];
				tensor<T, 1, n> out;
				Eigen::Map<const Eigen::Matrix<T, n, 1>> col(data);
				Eigen::Map<Eigen::Matrix<T, 1, n>>       row(out.data);
				row = col.transpose();
				return out;
			}
			else if constexpr (rank == 2)
			{
				constexpr idx_t n = shape[0];
				constexpr idx_t m = shape[1];
				tensor<T, m, n> out;
				Eigen::Map<const Eigen::Matrix<T, n, m>> mat(data);
				Eigen::Map<Eigen::Matrix<T, m, n>>       mt(out.data);
				mt = mat.transpose();
				return out;
			}
			else
				return tensor<T, dims...>{}; 
		}

	template<typename T, idx_t... dims1>
		auto operator+(const tensor<T, dims1...>& X, const tensor<T, dims1...>& Y)
		{
			tensor<T, dims1...> out;
			constexpr idx_t rank = tensor<T, dims1...>::rank;
			Eigen::TensorMap<Eigen::Tensor<const T, rank>> x(const_cast<T*>(X.data), dims1...);
			Eigen::TensorMap<Eigen::Tensor<const T, rank>> y(const_cast<T*>(Y.data), dims1...);
			Eigen::TensorMap<Eigen::Tensor<T, rank>>       o(out.data, dims1...);
			o = x + y;
			return out;
		}

	template<typename T, idx_t... dims1>
		auto operator-(const tensor<T, dims1...>& X, const tensor<T, dims1...>& Y)
		{
			tensor<T, dims1...> out;
			constexpr idx_t rank = tensor<T, dims1...>::rank;
			Eigen::TensorMap<Eigen::Tensor<const T, rank>> x(const_cast<T*>(X.data), dims1...);
			Eigen::TensorMap<Eigen::Tensor<const T, rank>> y(const_cast<T*>(Y.data), dims1...);
			Eigen::TensorMap<Eigen::Tensor<T, rank>>       o(out.data, dims1...);
			o = x - y;
			return out;
		}

	template<typename T, idx_t... dims1>
		auto operator-(const tensor<T, dims1...>& X)
		{
			tensor<T, dims1...> out;
			constexpr idx_t rank = tensor<T, dims1...>::rank;
			Eigen::TensorMap<Eigen::Tensor<const T, rank>> x(const_cast<T*>(X.data), dims1...);
			Eigen::TensorMap<Eigen::Tensor<T, rank>>       o(out.data, dims1...);
			o = -x;
			return out;
		}

	template<typename T, idx_t... dims1>
		auto operator*(const tensor<T, dims1...>& X, T s)
		{
			tensor<T, dims1...> out;
			constexpr idx_t rank = tensor<T, dims1...>::rank;
			Eigen::TensorMap<Eigen::Tensor<const T, rank>> x(const_cast<T*>(X.data), dims1...);
			Eigen::TensorMap<Eigen::Tensor<T, rank>>       o(out.data, dims1...);
			o = x * s;
			return out;
		}

	template<typename T, idx_t... dims1>
		auto operator*(T s, const tensor<T, dims1...>& X)
		{
			return X * s;
		}

	template<typename T, idx_t... dims1, idx_t... dims2>
		auto operator*(const tensor<T, dims1...>& X, const tensor<T, dims2...>& Y)
		{
			constexpr idx_t rank1 = tensor<T, dims1...>::rank;
			constexpr idx_t rank2 = tensor<T, dims2...>::rank;
			if constexpr (rank1 == 0 && rank2 == 0)
			{
				tensor<T> out;
				out.data[0] = X.data[0] * Y.data[0];
				return out;
			}
			else if constexpr (rank1 == 0 && rank2 > 0)
			{
				tensor<T, dims2...> out;
				Eigen::TensorMap<Eigen::Tensor<const T, rank2>> y(const_cast<T*>(Y.data), dims2...);
				Eigen::TensorMap<Eigen::Tensor<T, rank2>>       o(out.data, dims2...);
				o = X.data[0] * y;
				return out;
			}
			else if constexpr (rank1 > 0 && rank2 == 0)
			{
				tensor<T, dims1...> out;
				Eigen::TensorMap<Eigen::Tensor<const T, rank1>> x(const_cast<T*>(X.data), dims1...);
				Eigen::TensorMap<Eigen::Tensor<T, rank1>>       o(out.data, dims1...);
				o = x * Y.data[0];
				return out;
			}
			else if constexpr (rank1 == 1 && rank2 == 1)
			{
				constexpr idx_t n = tensor<T, dims1...>::shape[0];
				tensor<T> out;
				Eigen::Map<const Eigen::Matrix<T, n, 1>> x(X.data);
				Eigen::Map<const Eigen::Matrix<T, n, 1>> y(Y.data);
				out.data[0] = x.dot(y);
				return out;
			}
			else if constexpr (rank1 == 2 && rank2 == 1)
			{
				constexpr idx_t n = tensor<T, dims1...>::shape[0];
				constexpr idx_t m = tensor<T, dims1...>::shape[1];
				tensor<T, n> out;
				Eigen::Map<const Eigen::Matrix<T, n, m>> x(X.data);
				Eigen::Map<const Eigen::Matrix<T, m, 1>>                  y(Y.data);
				Eigen::Map<Eigen::Matrix<T, n, 1>>                        o(out.data);
				o = x * y;
				return out;
			}
			else if constexpr (rank1 == 1 && rank2 == 2)
			{
				constexpr idx_t n1 = tensor<T, dims1...>::shape[0];
				constexpr idx_t n2 = tensor<T, dims2...>::shape[0];
				constexpr idx_t m2 = tensor<T, dims2...>::shape[1];
				tensor<T, m2> out;
				Eigen::Map<const Eigen::Matrix<T, 1, n1>>                   x(X.data);
				Eigen::Map<const Eigen::Matrix<T, n2, m2>> y(Y.data);
				Eigen::Map<Eigen::Matrix<T, 1, m2>>                         o(out.data);
				o = x * y;
				return out;
			}
			else if constexpr (rank1 == 2 && rank2 == 2)
			{
				constexpr idx_t n1 = tensor<T, dims1...>::shape[0];
				constexpr idx_t m1 = tensor<T, dims1...>::shape[1];
				constexpr idx_t m2 = tensor<T, dims2...>::shape[1];
				tensor<T, n1, m2> out;
				Eigen::Map<const Eigen::Matrix<T, n1, m1>> x(X.data);
				Eigen::Map<const Eigen::Matrix<T, m1, m2>> y(Y.data);
				Eigen::Map<Eigen::Matrix<T, n1, m2>>       o(out.data);
				o = x * y;
				return out;
			}
			else
			{
				tensor<T, dims1...> out;
				Eigen::TensorMap<Eigen::Tensor<const T, rank1>> x(const_cast<T*>(X.data), dims1...);
				Eigen::TensorMap<Eigen::Tensor<const T, rank2>> y(const_cast<T*>(Y.data), dims2...);
				Eigen::TensorMap<Eigen::Tensor<T, rank1>>       o(out.data, dims1...);
				o = x * y;
				return out;
			}
		}

	template<typename T>
		auto operator^(const tensor<T,3>& X, const tensor<T,3>& Y)
		{
			tensor<T,3> out;
			const T x0 = X.data[0]; 
			const T x1 = X.data[1]; 
			const T x2 = X.data[2];
			const T y0 = Y.data[0];
			const T y1 = Y.data[1];
			const T y2 = Y.data[2];
			out.data[0] = x1*y2 - x2*y1;
			out.data[1] = x2*y0 - x0*y2;
			out.data[2] = x0*y1 - x1*y0;
			return out;
		}

	template<typename T, idx_t... dims1, idx_t... dims2>
		auto operator|(const tensor<T, dims1...>& X, const tensor<T, dims2...>& Y)
		{
			constexpr idx_t rank1 = tensor<T, dims1...>::rank;
			constexpr idx_t rank2 = tensor<T, dims2...>::rank;
			if constexpr (rank1 == 0 || rank2 == 0)
				return X * Y;
			constexpr idx_t rank  = rank1 + rank2;
			tensor<T, dims1..., dims2...> out;
			Eigen::TensorMap<Eigen::Tensor<const T, rank1>> x(const_cast<T*>(X.data), dims1...);
			Eigen::TensorMap<Eigen::Tensor<const T, rank2>> y(const_cast<T*>(Y.data), dims2...);
			Eigen::TensorMap<Eigen::Tensor<T, rank>>        o(out.data, dims1..., dims2...);
			o = x.contract(y, Eigen::array<Eigen::IndexPair<int>, 0>{});
			return out;
		}

	template<typename T, idx_t... dims1, idx_t... dims2>
		auto operator||(const tensor<T, dims1...>& X, const tensor<T, dims2...>& Y)
		{	
			// No contraction used
			return tensor<T>{};
		}

} // namespace dosm

#endif // DOSM_TENSOR_EIGEN_TPP

