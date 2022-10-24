#include "comp6771/euclidean_vector.hpp"
#include <catch2/catch.hpp>
#include <sstream>
#include <vector>
#include <list>
#include <cmath>
#include <iterator>
#include <utility>

// You may assume that all arguments supplied by the user are valid. No error checking on constructors is required.

/*
	Here is my main rationale the test cases in general:
	Firstly, I follow the order of testing constructors,
	Operations, Member Functions, Friends and utility functions.
	I do constructors first, because these components need to ideally work
	before anything else can be further tested.
	Next I test operations, these with most of the others can be tested
	ideally anywhere, but there useful in a sense that they can be
	used to validate some friend functions. However, when testing operations
	I also was careful with the creation and testing of the 0 vector, a valid
	vector with 0 dimensions. Along with with this, i thoroughly used the operator
	[] to check if the right values have been put through, which was essential
	for the constructors and operations. There is some circular testing, but
	the fact that the operator [] function can help me validate a lot of
	other things speaks volumes for its functionality. I then go on to
	looking at member functions. The member functions are useful in that it
	can help me check if there any elements inside the euclidean vector of
	an empty vector, which i then utilised further downwards when testing my friend
	functions. Also, note that I have tested for exceptions too.
	Whilst testing the friends functions, I tested if my equal and not equal
	operator was correct, looking back I should've done this earlier as I would
	not have to check everything Index Wise, If i can pass doubles into my
	euclidean vector, and the == and !=  overloads give me the correct
	response even in regards to floating point errors, then I simply could have
	just used 1 line to check if the elements were equal or not!
	Once realising this I definitely utilised this useful overload for the
	testing of my next functions. I left utility to the last becasue it is
	the most complex function we have, without the rest working we would find
	it hard to validate the results of these functions!
	This is my bried explanation and rationale as to why I did my tests
	in this order, and also what I could've done better!
*/

TEST_CASE("TEST EMPTY CONSTRUCTOR's CONTENTS and DIMENSIONS") {
	auto const a = comp6771::euclidean_vector();
	CHECK(a.dimensions() == 1);
	REQUIRE(std::abs(a[0]-0.0) < 0.0001);

	auto const b = comp6771::euclidean_vector{};
	CHECK(b.dimensions() == 1);
	REQUIRE(std::abs(b[0]-0.0) < 0.0001);

}

TEST_CASE("TEST Single-argument Constructor where ARG > 0") {
	auto const a = comp6771::euclidean_vector(3);
	CHECK(a.dimensions() == 3);
	REQUIRE(std::abs(a[0]-0.0) < 0.0001);
	REQUIRE(std::abs(a[1]-0.0) < 0.0001);
	REQUIRE(std::abs(a[2]-0.0) < 0.0001);

}

TEST_CASE("TEST Single-argument Constructor where ARG == 0") {
	auto const a = comp6771::euclidean_vector(0);
	CHECK(a.dimensions() == 0);
}

TEST_CASE("TEST constructor which uses double values and size to initialise") {
	auto const a = comp6771::euclidean_vector(3, 2.5);
	CHECK(a.dimensions() == 3);
	REQUIRE(std::abs(a[0]-2.5) < 0.0001);
	REQUIRE(std::abs(a[1]-2.5) < 0.0001);
	REQUIRE(std::abs(a[2]-2.5) < 0.0001);
}

TEST_CASE("TEST VECTOR CONSTRUCTOR where VECTOR is NON EMPTY") {
	auto const a = std::vector<double>{1.1,2.2,3.3};
	auto const b = comp6771::euclidean_vector(a.begin(), a.end());
	CHECK(int(a.size()) == b.dimensions());
	REQUIRE(std::abs(a[0] - b[0]) < 0.0001);
	REQUIRE(std::abs(a[1] - b[1]) < 0.0001);
	REQUIRE(std::abs(a[2] - b[2]) < 0.0001);
}


TEST_CASE("TEST VECTOR CONSTRUCTOR where VECTOR is EMPTY") {
	auto const a = std::vector<double>{};
	auto const b = comp6771::euclidean_vector(a.begin(), a.end());
	CHECK(int(a.size()) == b.dimensions());

	auto const c = std::vector<double>({});
	auto const d = comp6771::euclidean_vector(a.begin(), a.end());
	CHECK(int(c.size()) == d.dimensions());
}

TEST_CASE("TEST CONSTRUCTOR with Initializer Lists") {

	auto const a = comp6771::euclidean_vector({});
	CHECK(a.dimensions() == 0);

	auto const b = comp6771::euclidean_vector{1.2,2.3,4.5};
	CHECK(b.dimensions() == 3);
	REQUIRE(std::abs(b[0] - 1.2) < 0.0001);
	REQUIRE(std::abs(b[1] -2.3) < 0.0001);
	REQUIRE(std::abs(b[2] - 4.5) < 0.0001);
}

TEST_CASE("Testing Copy Constructor where dimensions > 0") {
	auto const a = comp6771::euclidean_vector{1.2,2.3,4.5};
	auto const b = comp6771::euclidean_vector(a);
	CHECK(a.dimensions() == b.dimensions());
	REQUIRE(std::abs(a[0] - b[0]) < 0.0001);
	REQUIRE(std::abs(a[1] - b[1]) < 0.0001);
	REQUIRE(std::abs(a[2] - b[2]) < 0.0001);
}

TEST_CASE("Testing Copy Constructor where dimensions = 0") {
	auto const a = comp6771::euclidean_vector({});
	auto const b = comp6771::euclidean_vector(a);
	CHECK(a.dimensions() == 0);
	CHECK(a.dimensions() == b.dimensions());
}

TEST_CASE("TESTING THE MOVE CONSTRUCTOR") {
	auto a = comp6771::euclidean_vector{1.1,2.2,3.3};
	auto const b = comp6771::euclidean_vector(a);
	auto const c = comp6771::euclidean_vector(std::move(a));
	//dimensions of a a
	CHECK(a.dimensions() == 0);
	CHECK(b.dimensions() == c.dimensions());
	REQUIRE(std::abs(c[0] - b[0]) < 0.0001);
	REQUIRE(std::abs(c[1] - b[1]) < 0.0001);
	REQUIRE(std::abs(c[2] - b[2]) < 0.0001);
}

// NEED to test destructors??

//TESTING OPERATIONS

TEST_CASE("TEST Copy Assignment") {
	auto const a = comp6771::euclidean_vector{1.1,2.2,3.3};
	auto b = comp6771::euclidean_vector{1.5,2.3,4.5,6.7};
	b = a;
	CHECK(b.dimensions() == a.dimensions());
	REQUIRE(std::abs(a[0] - b[0]) < 0.0001);
	REQUIRE(std::abs(a[1] - b[1]) < 0.0001);
	REQUIRE(std::abs(a[2] - b[2]) < 0.0001);
}

TEST_CASE("TEST MOVE assignment") {
	auto a = comp6771::euclidean_vector{1.1,2.2,3.3};
	auto const b = a;
	auto c = comp6771::euclidean_vector{2.1,2.4,3.1,4.5};
	c = std::move(a);
	CHECK(b.dimensions() == c.dimensions());
	REQUIRE(std::abs(c[0] - b[0]) < 0.0001);
	REQUIRE(std::abs(c[1] - b[1]) < 0.0001);
	REQUIRE(std::abs(c[2] - b[2]) < 0.0001);
}

TEST_CASE ("TEST SUBSCRIPT") {
	auto a = comp6771::euclidean_vector{1.1,2.2,3.3};
	auto const b = a[0];
	REQUIRE(std::abs(b-a[0]) < 0.0001);
	a[0] = 3.2;
	//We have successfully changed the contents of a[0]
	REQUIRE(std::abs(b-a[0]) > 0.0001);
}

TEST_CASE("UNARY PLUS") {
	auto const a = comp6771::euclidean_vector{1.1,2.2,3.3};
	auto d = comp6771::euclidean_vector({});
	auto const b = +a;
	auto c = +a;
	CHECK(b.dimensions() == a.dimensions());
	REQUIRE(std::abs(a[0] - b[0]) < 0.0001);
	REQUIRE(std::abs(a[1] - b[1]) < 0.0001);
	REQUIRE(std::abs(a[2] - b[2]) < 0.0001);

	CHECK(c.dimensions() == b.dimensions());
	REQUIRE(std::abs(c[0] - b[0]) < 0.0001);
	REQUIRE(std::abs(c[1] - b[1]) < 0.0001);
	REQUIRE(std::abs(c[2] - b[2]) < 0.0001);

	d = +b;

	CHECK(d.dimensions() == b.dimensions());
	REQUIRE(std::abs(d[0] - b[0]) < 0.0001);
	REQUIRE(std::abs(d[1] - b[1]) < 0.0001);
	REQUIRE(std::abs(d[2] - b[2]) < 0.0001);
}

TEST_CASE("TEST NEGATION") {
	auto const a = comp6771::euclidean_vector{-6, 1};
	auto const b = comp6771::euclidean_vector{6, -1};
	auto c = -b;
	REQUIRE(std::abs(c[0] - a[0]) < 0.0001);
	REQUIRE(std::abs(c[1] - a[1]) < 0.0001);
}

TEST_CASE("TEST COMPOUND ADDITION AND SUBTRACTION") {
	//NEED TO UPDATE AFTER ADDING EXCEPTIONS
	auto a = comp6771::euclidean_vector{1,2,3};
	auto const b = comp6771::euclidean_vector{1,2,3};
	auto const c = comp6771::euclidean_vector{2,4,6};
	auto const d = comp6771::euclidean_vector{0,0,0};
	auto const big = comp6771::euclidean_vector{2,4,6,7};
	auto empty = comp6771::euclidean_vector({});
	auto const empty2 = comp6771::euclidean_vector({});

	SECTION("COMPOUND ADDITION") {
		a += b;
		CHECK(a.dimensions() == b.dimensions());
		REQUIRE(std::abs(a[0] - c[0]) < 0.0001);
		REQUIRE(std::abs(a[1] - c[1]) < 0.0001);
		REQUIRE(std::abs(a[2] - c[2]) < 0.0001);

		//FOR EMPTY
		REQUIRE_THROWS(empty += b);
		empty += empty2;
		CHECK(empty.dimensions() == empty2.dimensions());
		REQUIRE_THROWS(empty.at(0));

		REQUIRE_THROWS(a+=big);

	}

	SECTION("COMPOUND SUBTRACTION") {
		a -= b;
		CHECK(a.dimensions() == b.dimensions());
		REQUIRE(std::abs(a[0] - d[0]) < 0.0001);
		REQUIRE(std::abs(a[1] - d[1]) < 0.0001);
		REQUIRE(std::abs(a[2] - d[2]) < 0.0001);

		//FOR EMPTY
		REQUIRE_THROWS(empty -= b);
		empty -= empty2;
		CHECK(empty.dimensions() == empty2.dimensions());
		REQUIRE_THROWS(empty.at(0));

		REQUIRE_THROWS(a-=big);
	}

}

TEST_CASE("TEST COMPOUND MULTIPLICATION AND DIVISION") {
	auto a = comp6771::euclidean_vector{2,4,6};
	auto const b = comp6771::euclidean_vector{1,2,3};
	auto const c = comp6771::euclidean_vector{4,8,12};
	auto empty = comp6771::euclidean_vector({});

	SECTION("COMPOUND DIVISION") {
		a *= 2;
		REQUIRE(std::abs(a[0] - c[0]) < 0.0001);
		REQUIRE(std::abs(a[1] - c[1]) < 0.0001);
		REQUIRE(std::abs(a[2] - c[2]) < 0.0001);

		empty *= 2; //THIS IS VALID
		REQUIRE_THROWS(empty.at(0));
	}

	SECTION("COMPOUND MULTIPLICATION") {
		a /= 2;
		REQUIRE(std::abs(a[0] - b[0]) < 0.0001);
		REQUIRE(std::abs(a[1] - b[1]) < 0.0001);
		REQUIRE(std::abs(a[2] - b[2]) < 0.0001);

		empty /= 2; //THIS IS VALID
		REQUIRE_THROWS(empty.at(0));

		//CANNOT DIVIDE BY 0
		REQUIRE_THROWS(a /= 0);
	}

}

TEST_CASE("TYPE CONVERSIONS") {
	auto const a = comp6771::euclidean_vector{2,4,6};
	auto const b = std::vector<double>{2,4,6};
	auto c = std::list<double>{2,4,6};

	auto a1 = static_cast<std::vector<double>>(a);
	auto a2 = static_cast<std::list<double>>(c);

	CHECK(b.size() == a1.size());
	REQUIRE(std::abs(a1[0] - b[0]) < 0.0001);
	REQUIRE(std::abs(a1[1] - b[1]) < 0.0001);
	REQUIRE(std::abs(a1[2] - b[2]) < 0.0001);

	CHECK(a2.size() == c.size());
	auto a2Front = a2.begin();
	auto cFront = c.begin();
	REQUIRE(std::abs(*a2Front - *cFront) < 0.0001);
	std::advance(a2Front, 1), std::advance(cFront, 1);
	REQUIRE(std::abs(*a2Front - *cFront) < 0.0001);
	std::advance(a2Front, 2), std::advance(cFront, 2);
	REQUIRE(std::abs(*a2Front - *cFront) < 0.0001);
}

//NEED TO ADD EXCEPTIONS HERE
TEST_CASE("TEST AT function when variable is const") {
	auto const a = comp6771::euclidean_vector{2,4,6};
	REQUIRE(std::abs(a.at(0)-2.0) < 0.0001);
	REQUIRE_THROWS(a.at(4));
	REQUIRE_THROWS(a.at(-1));
	//We can only get values
}

TEST_CASE("TEST AT function when variable is non const") {
	auto a = comp6771::euclidean_vector{2,4,6};
	REQUIRE(std::abs(a.at(0)-2.0) < 0.0001);
	a.at(0) = 3.5;
	REQUIRE(std::abs(a.at(0)-3.5) < 0.0001);
	REQUIRE_THROWS(a.at(4));
	REQUIRE_THROWS(a.at(-1));
}

TEST_CASE("TEST dimensions") {
	//Avoided testing this earlier due to circular testing etc
	auto const a = comp6771::euclidean_vector{2,4,6};
	CHECK(a.dimensions() == 3);
	auto b = comp6771::euclidean_vector{1,2,3,4};
	b = a;
	CHECK(b.dimensions() == a.dimensions());

}

TEST_CASE("TEST EQUAL AND NOT EQUAL") {
	auto const a = comp6771::euclidean_vector({1.1,2.54332,3,4,5});
	auto const b = a;
	auto const c = comp6771::euclidean_vector({1.1,2.54332,3,4,5,6});
	auto const empty = comp6771::euclidean_vector({});
	auto const empty2 = comp6771::euclidean_vector({});
	SECTION("TEST EQUAL SIGN but it returns FALSE") {
		REQUIRE_FALSE(a == c);
	}

	SECTION("TEST EQUAL SIGN but it returns TRUE") {
		REQUIRE(a == b);
		REQUIRE(empty == empty2);
	}
	SECTION("TEST NOT EQUAL SIGN but it returns TRUE") {
		REQUIRE(a != c);
	}

	SECTION("TEST EQUAL SIGN but it returns FALSE") {
		REQUIRE_FALSE(a != b);
		REQUIRE_FALSE(empty != empty2);
	}
}

	//WE CAN USE EQUAL SIGNS BECAUSE WE VALIDATED IT ABOVE
TEST_CASE("TEST ADDITION AND SUBTRACTION") {
	auto const a = comp6771::euclidean_vector{1,2,3};
	auto const b = comp6771::euclidean_vector{1,2,3};
	auto const c = comp6771::euclidean_vector{2,4,6};
	auto const d = comp6771::euclidean_vector{0,0,0};
	auto const e = comp6771::euclidean_vector{0,0,0,0};

	SECTION ("TESTING ADDITION") {
		REQUIRE(c == (a+b));
		REQUIRE_THROWS(e+a);
	}
	SECTION("TESTING SUBTRACTION") {
		REQUIRE((c-a) == b);
		REQUIRE((a-b) == d);
		REQUIRE_THROWS(e-a);
	}
}

TEST_CASE("TESTING MULTIPLICATION AND DIVISION") {
	auto const a = comp6771::euclidean_vector{2,4,6};
	auto const c = comp6771::euclidean_vector{4,8,12};
	auto const d = comp6771::euclidean_vector({});
	auto const e = comp6771::euclidean_vector({});

	SECTION("TESTING MULTIPLICATION") {
		REQUIRE(a*2==c);
		REQUIRE(d*2==e);
	}
	SECTION("TESTING MULTIPLICATION") {
		REQUIRE(c/2==a);
		REQUIRE(d/2==e); //TESTING EMPTY VECTORS with a DIMENSION of 0
		REQUIRE_THROWS(c/0);
	}
}

TEST_CASE("TESTING OSTREAM OPERATOR") {
	auto const a1 = comp6771::euclidean_vector(3, 3.0);
	auto const a2 = comp6771::euclidean_vector(3, 3.0);
	auto const a3 = comp6771::euclidean_vector(3, 3.0);

	auto const empty = comp6771::euclidean_vector({});

	auto oss = std::ostringstream{};
	auto oss2 = std::ostringstream{};
	oss << (a1 + a2 + a3);
	CHECK(oss.str() == "[9 9 9]");

	oss2 << empty;
	CHECK(oss2.str() == "[]");
}

TEST_CASE("TESTING DOT") {
	//Dot product with
	auto const a = comp6771::euclidean_vector{1,1,1};
	auto const b = comp6771::euclidean_vector{2,2,2};
	auto const c = comp6771::euclidean_vector{1,2};
	auto const empty = comp6771::euclidean_vector({});
	auto const empty2 = comp6771::euclidean_vector({});

	REQUIRE(std::abs(3-comp6771::dot(a,a)) < 0.0001);

	REQUIRE(std::abs(6-comp6771::dot(a,b)) < 0.0001);

	REQUIRE_THROWS(comp6771::dot(a,c));

	REQUIRE(std::abs(0-comp6771::dot(empty,empty2)) < 0.0001);

}

TEST_CASE("TESTING UNIT VECTOR") {
	auto const empty = comp6771::euclidean_vector({});
	REQUIRE_THROWS(comp6771::unit(empty));

	auto const a = comp6771::euclidean_vector{3,4};
	auto const b = comp6771::euclidean_vector{0.6,0.8};
	REQUIRE_THROWS(comp6771::unit(a));

	auto tmp = comp6771::euclidean_norm(a);
	REQUIRE(std::abs(tmp - 5) < 0.0001);
	REQUIRE(comp6771::unit(a) == b);
}

TEST_CASE("TESTING NORMAL") {
	auto a = comp6771::euclidean_vector{1,1,1};
	auto tmp = comp6771::dot(a,a);
	auto norm = comp6771::euclidean_norm(a);
	REQUIRE(std::abs(std::sqrt(tmp)-norm) < 0.0001);
	auto const empty = comp6771::euclidean_vector({});
	REQUIRE(std::abs(0-comp6771::euclidean_norm(empty)) < 0.0001);
	auto const un = comp6771::unit(a);

	//gotta check what happens if i copy it, move it, at it, [] change it!
	SECTION("BEHAVIOUR WHEN USING AT") {
		a.at(0) = 0; //Testing if the cache has been reset or not
		REQUIRE_THROWS(comp6771::unit(a));
	}

	SECTION("BEHAVIOUR WHEN USING AT") {
		a[0] = 0; //Testing if the cache has been reset or not
		REQUIRE_THROWS(comp6771::unit(a));
	}

	SECTION("BEHAVIOUR WHEN COPYING") {
		auto c = a;
		norm = comp6771::euclidean_norm(c);
		REQUIRE(std::abs(std::sqrt(tmp)-norm) < 0.0001);
		REQUIRE(un == comp6771::unit(c));
	}

	SECTION("BEHAVIOUR WHEN MOVING") {
		auto c = std::move(a);
		REQUIRE(un == comp6771::unit(c));
		REQUIRE_THROWS(comp6771::unit(a));
	}
}