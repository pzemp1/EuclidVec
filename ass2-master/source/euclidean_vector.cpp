// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include "comp6771/euclidean_vector.hpp"
#include <iostream>
#include <list>
#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <cmath>
#include <stdexcept>
#include <string>
#include <numeric>
#include <utility>
#include <cstdint>
#include <vector>
#include <typeinfo>
#include <cassert>
#include <experimental/iterator>


namespace comp6771 {
	euclidean_vector::euclidean_vector() : euclidean_vector(1,0.0) {}

	euclidean_vector::euclidean_vector(int const &size) : euclidean_vector(size, 0.0){ }

	euclidean_vector::euclidean_vector(int const &size, double const &num) : dimensions_{std::size_t(size)}
		,State_{false}, norm_{0.0}, dot_{-1.0} {
		magnitude_ = std::make_unique<double[]>(dimensions_);
		std::fill(magnitude_.get(), magnitude_.get()+dimensions_, num);
	}

	euclidean_vector::euclidean_vector(std::vector<double>::const_iterator const begin, std::vector<double>::const_iterator const end)
		: dimensions_{std::size_t(end-begin)}, State_{false}, norm_{0.0}, dot_{-1.0} {
		magnitude_ = std::make_unique<double[]>(dimensions_);
		std::copy(begin, end, magnitude_.get());
	}

	euclidean_vector::euclidean_vector(std::initializer_list<double> l) : dimensions_{l.size()}
		, State_{false}, norm_{0.0}, dot_{-1.0}{
		magnitude_ = std::make_unique<double[]>(dimensions_);
		std::copy(l.begin(), l.end(), magnitude_.get());
	}

	euclidean_vector::euclidean_vector(euclidean_vector const&ev) : dimensions_{ev.dimensions_}
		, State_{ev.State_}, norm_{ev.norm_}, dot_{ev.dot_}{
		magnitude_ = std::make_unique<double[]>(dimensions_);
		std::memcpy(magnitude_.get(), ev.magnitude_.get(), sizeof(double)*dimensions_);
	}

	euclidean_vector::euclidean_vector(euclidean_vector &&Orig) noexcept
		: dimensions_{std::exchange(Orig.dimensions_, 0.0)}
		, State_{std::exchange(Orig.State_, false)}
		, norm_{std::exchange(Orig.norm_, 0.0)}
		, dot_{std::exchange(Orig.dot_, -1.0)}
		, magnitude_{std::move(Orig.magnitude_)} {}

	euclidean_vector& euclidean_vector::operator=(euclidean_vector const& ev) {
		if (this == &ev) {
			return *this;
		}
		AdjustMutables(ev.State_, ev.norm_, ev.dot_);
		magnitude_ = std::make_unique<double[]>(ev.dimensions_);
		dimensions_ = ev.dimensions_;
		std::memcpy(magnitude_.get(), ev.magnitude_.get(), sizeof(double)*dimensions_);
		return *this;
	}

	euclidean_vector& euclidean_vector::operator=(euclidean_vector &&Orig) noexcept
	{
		if (this != &Orig) {
			State_ = std::exchange(Orig.State_, false);
			norm_ = std::exchange(Orig.norm_, 0.0);
			magnitude_ = std::move(Orig.magnitude_);
			dot_ = std::exchange(Orig.dot_, -1.0);
			dimensions_= std::exchange(Orig.dimensions_, 0.0);
		}
		return *this;
	}

	double euclidean_vector::operator[](int i) const {
		assert(size_t(i) >= 0 and size_t(i) < dimensions_);
		return *(magnitude_.get()+i);
	}

	double& euclidean_vector::operator[](int i) {
		assert(size_t(i) >= 0 and size_t(i) < dimensions_);
		AdjustMutables(false, 0.0, -1.0);
		return *(magnitude_.get()+i);
	}

	euclidean_vector euclidean_vector::operator+(void) const{
		euclidean_vector tmp = euclidean_vector(*this);
		return tmp;
	}

	euclidean_vector euclidean_vector::operator-(void) const{
		euclidean_vector tmp = euclidean_vector(*this); //Calling my Copy Constructor
		tmp *= -1;
		return tmp;
	}

	euclidean_vector euclidean_vector::operator+=(euclidean_vector const& b) {
		// NEED to add exception
		if (dimensions_ != b.dimensions_) {
			throw euclidean_vector_error("Dimensions of LHS(" + std::to_string(dimensions_) +") and RHS(" +
			std::to_string(b.dimensions_) + ") do not match");
		}

		AdjustMutables(false, 0.0,-1.0);
		std::transform(magnitude_.get(), magnitude_.get()+dimensions_,
					b.magnitude_.get(), magnitude_.get(), std::plus<double>());
		return *this;
	}

	euclidean_vector euclidean_vector::operator-=(euclidean_vector const& b) {
		// NEED to add exception
		if (dimensions_ != b.dimensions_) {
			throw euclidean_vector_error("Dimensions of LHS(" + std::to_string(dimensions_) +") and RHS(" +
			std::to_string(b.dimensions_) + ") do not match");
		}

		AdjustMutables(false, 0.0, -1.0);
		std::transform(magnitude_.get(), magnitude_.get()+dimensions_,
					b.magnitude_.get(), magnitude_.get(), std::minus<double>());
		return *this;
	}

	euclidean_vector euclidean_vector::operator*=(double const& b) {

		AdjustMutables(false, 0.0, -1.0);
		std::transform(magnitude_.get(), magnitude_.get()+dimensions_,
					magnitude_.get(),std::bind(std::multiplies<double>(), std::placeholders::_1, b));
		return *this;
	}

	euclidean_vector euclidean_vector::operator/=(double const& b) {
		//NEED to add exception
		if (std::abs(b-0) < 0.0001) {
			throw euclidean_vector_error("Invalid vector division by 0");
		}

		AdjustMutables(false, 0.0, -1.0);
		std::transform(magnitude_.get(), magnitude_.get()+dimensions_,
					magnitude_.get(),std::bind(std::divides<double>(), std::placeholders::_1, b));
		return *this;
	}

	euclidean_vector::operator std::vector<double>() const{
		auto a = std::vector<double>();
		std::copy(magnitude_.get(), magnitude_.get()+dimensions_, std::back_inserter(a));
		return a;
	}

	euclidean_vector::operator std::list<double>() const{
		auto a = std::list<double>();
		std::copy(magnitude_.get(), magnitude_.get()+dimensions_, std::back_inserter(a));
		return a;
	}

	double euclidean_vector::at(int i) const {
		//NEED to add exception
		if (i < 0 or i >= this->dimensions()) {
			throw euclidean_vector_error("Index " + std::to_string(i) +" is not valid for this euclidean_vector object");
		}
		return *(magnitude_.get()+i);
	}

	double& euclidean_vector::at(int i) {
		//NEED to add exception
		if (i < 0 or i >= this->dimensions()) {
			throw euclidean_vector_error("Index " + std::to_string(i) +" is not valid for this euclidean_vector object");
		}
		AdjustMutables(false, 0.0, -1.0);
		return *(magnitude_.get()+i);
	}

	int euclidean_vector::dimensions() const {
		return static_cast<int>(dimensions_);
	}

	double euclidean_norm(euclidean_vector const&v) {
		//ADD EXCEPTIONS
		if (v.State_) {
			return v.norm_;
		} else {
			auto z = std::sqrt(comp6771::dot(v,v));
			v.norm_ = z;
			v.State_ = true;
			return z;
		}
	}


	double dot(euclidean_vector const& x, euclidean_vector const& y) {
		//ADD EXCEPTIONS HERE
		if (y.dimensions_ != x.dimensions_) {
			throw euclidean_vector_error("Dimensions of LHS(" + std::to_string(x.dimensions_) +") and RHS(" +
			std::to_string(y.dimensions_) + ") do not match");
		}
		auto key = false;
		if (&x == &y) {
			key = true;
			if (std::abs(x.dot_-(-1)) > 0.0001) {
				return x.dot_;
			}
		}
		double r1 = std::inner_product(x.magnitude_.get()
									,x.magnitude_.get()+x.dimensions_
									, y.magnitude_.get(), 0.0);
		if (key == true) {
			x.dot_ = r1;
		}
		return r1;
	}
	euclidean_vector unit(euclidean_vector const& v) {
		//ADD EXCEPTIONS
		if (v.dimensions() == 0) {
			throw euclidean_vector_error("euclidean_vector with no dimensions does not have a unit vector");
		}
		auto x = v;
		auto d = v.norm_;
		if (std::abs(d-0) < 0.0001) {
			throw euclidean_vector_error("euclidean_vector with zero euclidean normal does not have a unit vector");
		}
		x /= d;
		return x;
	}


} // namespace comp6771
