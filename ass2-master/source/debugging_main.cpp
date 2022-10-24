#include <iostream>
#include <list>
#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <numeric>
#include <utility>
#include <cstdint>
#include <vector>
#include <typeinfo>
#include <cassert>
#include <experimental/iterator>

class euclidean_vector {
	public:
		//Constructor 1 where there are NO arguements.
		euclidean_vector() : euclidean_vector(1,0.0) {
		}
		//Given a size
		explicit euclidean_vector(int const &size) : euclidean_vector(size, 0.0){
		}
		//Given a size and a num
		euclidean_vector(int const &size, double const &num) : dimensions_{std::size_t(size)} {
			//Use std::copy
			magnitude_ = std::make_unique<double[]>(dimensions_);
			std::fill(magnitude_.get(), magnitude_.get()+dimensions_, num);
		}
		//Given a vector
		euclidean_vector(std::vector<double>::iterator const begin, std::vector<double>::iterator const end)
		: dimensions_{std::size_t(end-begin)}{
			//Use std::copy
			magnitude_ = std::make_unique<double[]>(dimensions_);
			std::copy(begin, end, magnitude_.get());
		}
		//Contructing with an initialiser list
		euclidean_vector(std::initializer_list<double> l) : dimensions_{l.size()}{
			magnitude_ = std::make_unique<double[]>(dimensions_);
			std::copy(l.begin(), l.end(), magnitude_.get());
		}
		//Copy Constructor
		euclidean_vector(euclidean_vector const&ev) : dimensions_{ev.dimensions_}{
			//We will
			magnitude_ = std::make_unique<double[]>(dimensions_);
			//std::copy(ev.magnitude_.get(), ev.magnitude_.get()+dimensions_, magnitude_.get());
			std::memcpy(magnitude_.get(), ev.magnitude_.get(), sizeof(double)*dimensions_);
		}
		//Move Constructor
		euclidean_vector(euclidean_vector &&Orig) noexcept
			: dimensions_{std::exchange(Orig.dimensions_, 0)}
			, magnitude_{std::move(Orig.magnitude_)} {}
		//Copy Assignment
		euclidean_vector& operator=(euclidean_vector const& ev) {
			//Need to be careful about memory
			if (this == &ev) {
				return *this; //If it's itself then do this
			}
			magnitude_ = std::make_unique<double[]>(ev.dimensions_);
			dimensions_ = ev.dimensions_;
			std::memcpy(magnitude_.get(), ev.magnitude_.get(), sizeof(double)*dimensions_);
			return *this;
		}

		//Move Assignment

		euclidean_vector& operator=(euclidean_vector &&Orig) noexcept
	 	{
			if (this != &Orig) {
				magnitude_ = std::move(Orig.magnitude_);
				dimensions_= std::exchange(Orig.dimensions_, 0);
			}
			return *this; //Orig.magnitude now points to a nullptr
		}
		//Subscript Const version

		// const on the RHS
		double operator[](int i) const {
			assert(size_t(i) >= 0 and size_t(i) < dimensions_);
			return *(magnitude_.get()+i);
		}

		//Subscript nonconst version
		double& operator[](int i) {
			assert(size_t(i) >= 0 and size_t(i) < dimensions_);
			return *(magnitude_.get()+i);
		}




		//Unary + Overload
		euclidean_vector operator+(void) {
			euclidean_vector tmp = euclidean_vector(*this);
			return tmp;
		}
		//Negation - Overload Maybe It will call scalar multiplier instead
		euclidean_vector operator-(void) {
			//Need to return a copy
			euclidean_vector tmp = euclidean_vector(*this); //Calling my Copy Constructor
			tmp *= -1;
			return tmp; //Return the copy
		}
		//Compound addiotn a += b
		euclidean_vector operator+=(euclidean_vector const& b) {
			//Need to ensure b is a valid euclidean_vector (add later)
			// We are modifying the values of a
			std::transform(magnitude_.get(), magnitude_.get()+dimensions_,
						b.magnitude_.get(), magnitude_.get(), std::plus<double>());
			return *this;
		}
		//Compound Subtraction a -= b
		euclidean_vector operator-=(euclidean_vector const& b) {
			//Need to ensure b is a valid euclidean_vector (add later)
			// We are modifying the values of a
			std::transform(magnitude_.get(), magnitude_.get()+dimensions_,
						b.magnitude_.get(), magnitude_.get(), std::minus<double>());
			return *this;
		}
		//Compound multiplication
		euclidean_vector operator*=(double const& b) {
			std::transform(magnitude_.get(), magnitude_.get()+dimensions_,
						magnitude_.get(),std::bind(std::multiplies<double>(), std::placeholders::_1, b));
			return *this;
		}
		//Compound Division NEED TO ADD EXCEPTION
		euclidean_vector operator/=(double const& b) {
			//Need to add exception
			std::transform(magnitude_.get(), magnitude_.get()+dimensions_,
						magnitude_.get(),std::bind(std::divides<double>(), std::placeholders::_1, b));
			return *this;
		}
		// Vector Iniialisation
		explicit operator std::vector<double>() const{
			//Create new vector
			auto a = std::vector<double>();
			std::copy(magnitude_.get(), magnitude_.get()+dimensions_, std::back_inserter(a));
			return a;
		}
		//List Iniitialisation
		explicit operator std::list<double>() const{
			//Create new vector
			auto a = std::list<double>();
			std::copy(magnitude_.get(), magnitude_.get()+dimensions_, std::back_inserter(a));
			return a;
		}

		double at(int i) const {
			//EXCEPTIONS Here please
			return *(magnitude_.get()+i);
		}

		double at(int i) {
			//EXCEPTIONS Here please
			return *(magnitude_.get()+i);
		}

		double GetX() const {
			return x;
		}

		int dimensions() const {
			return static_cast<int>(dimensions_);
		}

		friend bool operator==(euclidean_vector const& a, euclidean_vector const& b);
		friend bool operator!=(euclidean_vector const& a, euclidean_vector const& b);
		friend euclidean_vector operator+(euclidean_vector const& a, euclidean_vector const& b);
		friend euclidean_vector operator-(euclidean_vector const& a, euclidean_vector const& b);
		friend euclidean_vector operator*(euclidean_vector const& a, double const& b);
		friend euclidean_vector operator*(double const& b, euclidean_vector const& a);
		friend euclidean_vector operator/(euclidean_vector const& a, double const& b);
		friend std::ostream& operator<<(std::ostream& os, euclidean_vector const& a);
		friend double dot(euclidean_vector const& x, euclidean_vector const& y);
		//auto get_dimensions() -> std::size_t const&;

	private:

		double mutable x;
		std::size_t dimensions_;
		std::unique_ptr<double[]> magnitude_;
		// TODO more if needed
};

void PrintFunction(std::string const &x) {
	std::cout << x << std::endl;
}

bool operator==(euclidean_vector const& a, euclidean_vector const& b) {
	if (a.dimensions_ != b.dimensions_) {
		return false;
	}
	return (std::equal(a.magnitude_.get(), a.magnitude_.get()+a.dimensions_
			,b.magnitude_.get(), b.magnitude_.get()+b.dimensions_));
}

bool operator!=(euclidean_vector const& a, euclidean_vector const& b) {
	return !(a==b);
}

euclidean_vector operator+(euclidean_vector const& a, euclidean_vector const& b) {
	// tmp = a -> New copy
	// tmp = tmp + b  or tmp += b (Modify this new copy)

	//Handle exceptions first
	auto tmp = euclidean_vector(a); //Calling my Copy Constructor
	tmp += b;
	return tmp;
}
euclidean_vector operator-(euclidean_vector const& a, euclidean_vector const& b) {
	// tmp = a -> New copy
	// tmp = tmp + b  or tmp += b (Modify this new copy)

	//Handle exceptions first
	auto tmp = euclidean_vector(a); //Calling my Copy Constructor
	tmp -= b; //then u subtract so its a - b
	return tmp;
}

euclidean_vector operator*(euclidean_vector const& a, double const& b) {

	auto tmp = euclidean_vector(a); //Calling my Copy Constructor
	tmp *= b; // a* b
	return tmp; //However is just doing a for loop
}

euclidean_vector operator*(double const& b, euclidean_vector const& a) {

	auto tmp = euclidean_vector(a); //Calling my Copy Constructor
	tmp *= b; // a* b
	return tmp; //However is just doing a for loop
}

euclidean_vector operator/(euclidean_vector const& a, double const& b) {
	//Check division by 0

	auto tmp = euclidean_vector(a);
	tmp /= b;
	return tmp;
}

std::ostream& operator<<(std::ostream& os, euclidean_vector const& a) {
	std::cout << "[";
	std::copy(a.magnitude_.get(), a.magnitude_.get()+a.dimensions_,
              std::experimental::make_ostream_joiner(os, " "));
	std::cout << "]";
	return os;
}

double dot(euclidean_vector const& x, euclidean_vector const& y) {
	if (&x == &y) {
		std::cout << "YES" << std::endl;
		x.x = 4.0;
		std::cout << y.GetX() << std::endl;
	}
	return 0;
}







auto main() -> int {


	auto const a = euclidean_vector{3.0,3.0,3.0};
	auto const b = euclidean_vector{1.0,1.0,1.0};
	auto &m = a;

	std::cout << dot(a,a) << std::endl;
	std::cout << a.GetX() << std::endl;
	std::cout << m.GetX() << std::endl;

	auto c = a;
	auto d = b;

	//NOTE i do &a[2]+1, because i cant access a[3], ill get an error
	double r1 = std::inner_product(&c[0], &c[2]+1, &c[0], 0.0);
	std::cout << r1 << std::endl;



	/*
	size_t n = 10;
	auto array = std::make_unique<double[]>(n);
	auto x = std::make_unique<double[]>(n);
	double *example = new double[n];
	//Create Vector that goes from 0 to 9 (Inclusive)
	std::vector<int> from_vector(n);
    std::iota(from_vector.begin(), from_vector.end(), 0);
	std::copy(from_vector.begin(), from_vector.end(), example); //This works
	std::copy(from_vector.begin(), from_vector.end(), &array[0]); //This works

	for (size_t i = 0; i < n; i++) {
		std::cout << x[i] << std::endl;
	}

	std::fill(&x[0], &x[n], 3);

	for (size_t i = 0; i < n; i++) {
		std::cout << x[i] << std::endl;
	}

	for (size_t i = 0; i < n; i++) {
		std::cout << *(x.get()+i) << std::endl;
	}





	delete []example;
	*/
}
