#ifndef COMP6771_EUCLIDEAN_VECTOR_HPP
#define COMP6771_EUCLIDEAN_VECTOR_HPP

#include <memory>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <experimental/iterator>
#include <cstdint>

namespace comp6771 {
	class euclidean_vector_error : public std::runtime_error {
	public:
		explicit euclidean_vector_error(std::string const& what)
		: std::runtime_error(what) {}
	};

	class euclidean_vector {
	public:
		//Constructors
		euclidean_vector();
		explicit euclidean_vector(int const &size);
		euclidean_vector(int const &size, double const &num);
		euclidean_vector(std::vector<double>::const_iterator const begin, std::vector<double>::const_iterator const end);
		euclidean_vector(std::initializer_list<double> l);
		euclidean_vector(euclidean_vector const&ev);
		euclidean_vector(euclidean_vector &&Orig) noexcept;

		euclidean_vector& operator=(euclidean_vector const& ev);
		euclidean_vector& operator=(euclidean_vector &&Orig) noexcept;
		double operator[](int i) const;
		double& operator[](int i);
		euclidean_vector operator+(void) const;
		euclidean_vector operator-(void) const;
		euclidean_vector operator+=(euclidean_vector const& b);
		euclidean_vector operator-=(euclidean_vector const& b) ;
		euclidean_vector operator*=(double const& b);
		euclidean_vector operator/=(double const& b);
		explicit operator std::vector<double>() const;
		explicit operator std::list<double>() const;

		double at(int i) const;
		double& at(int i);
		int dimensions() const;

		friend bool operator==(euclidean_vector const& a, euclidean_vector const& b) {
			if (a.dimensions_ != b.dimensions_) {
				return false;
			}
			auto EpFactor = [] (double const& x, double const& y, double const& Epsilon = 0.0001) {
				return (std::abs(x-y) < Epsilon);
			};
			return std::equal(a.magnitude_.get(), a.magnitude_.get()+a.dimensions_
					,b.magnitude_.get(), b.magnitude_.get()+b.dimensions_, EpFactor);
		}

		friend bool operator!=(euclidean_vector const& a, euclidean_vector const& b) {
			return !(a==b);
		}

		friend euclidean_vector operator+(euclidean_vector const& a, euclidean_vector const& b) {
			//NEED to add exception
			/*Given: X = a.dimensions(), Y = b.dimensions()
      When: X != Y
      Throw: "Dimensions of LHS(X) and RHS(Y) do not match"
	  */
	  		if (a.dimensions_ != b.dimensions_) {
				throw euclidean_vector_error("Dimensions of LHS(" + std::to_string(a.dimensions_) +") and RHS(" +
				std::to_string(b.dimensions_) + ") do not match");
			}
			auto tmp = euclidean_vector(a);
			tmp += b;
			return tmp;
		}
		friend euclidean_vector operator-(euclidean_vector const& a, euclidean_vector const& b) {
			//NEED to add exception
			if (a.dimensions_ != b.dimensions_) {
				throw euclidean_vector_error("Dimensions of LHS(" + std::to_string(a.dimensions_) +") and RHS(" +
				std::to_string(b.dimensions_) + ") do not match");
			}
			auto tmp = euclidean_vector(a);
			tmp -= b;
			return tmp;
		}

		friend euclidean_vector operator*(euclidean_vector const& a, double const& b) {

			auto tmp = euclidean_vector(a);
			tmp *= b;
			return tmp;
		}

		friend euclidean_vector operator*(double const& b, euclidean_vector const& a) {

			auto tmp = euclidean_vector(a);
			tmp *= b;
			return tmp;
		}

		friend euclidean_vector operator/(euclidean_vector const& a, double const& b) {

			//NEED to add exception
			if (std::abs(b-0) < 0.0001) {
				throw euclidean_vector_error("Invalid vector division by 0");
			}
			auto tmp = euclidean_vector(a);
			tmp /= b;
			return tmp;
		}

		friend std::ostream& operator<<(std::ostream& os, euclidean_vector const& a) {
			os << "[";
			std::copy(a.magnitude_.get(), a.magnitude_.get()+a.dimensions_,
					std::experimental::make_ostream_joiner(os, " "));
			os << "]";
			return os;
		}

		~euclidean_vector() = default;

		friend double euclidean_norm(euclidean_vector const& v);
		friend double dot(euclidean_vector const& x, euclidean_vector const& y);
		friend euclidean_vector unit(euclidean_vector const& v);


	private:
		void AdjustMutables(bool const State, double const norm, double const dot) {
			State_ = State;
			norm_ = norm;
			dot_ = dot;
		}

		std::size_t dimensions_;
		mutable bool State_;
		mutable double norm_;
		mutable double dot_;
		std::unique_ptr<double[]> magnitude_;
	};
	double euclidean_norm(euclidean_vector const& v);
	double dot(euclidean_vector const& x, euclidean_vector const& y);
	euclidean_vector unit(euclidean_vector const& v);
} // namespace comp6771
#endif // COMP6771_EUCLIDEAN_VECTOR_HPP
