#include <random>
#include <vector>

#include "Gates.h"
#include "Shifter.h"
#include "Extender.h"
#include "DirectAddress.h"

#include "gtest/gtest.h"
#include "LogicUnitTest.h"

using ::testing::InSequence;
using ::testing::AnyNumber;
using ::testing::AtLeast;
using ::testing::_;

static std::uniform_int_distribution<unsigned> u_numbers;
static std::uniform_int_distribution<char> c_numbers;
static std::uniform_int_distribution<int> s_numbers;
static std::random_device rd;
static std::default_random_engine engine{rd()};

class GatesTest : public ::testing::Test {
  protected:
    GatesTest() : first_value{u_numbers(engine)}, port{s_numbers(engine)},
	second_value{u_numbers(engine)}
    {
	EXPECT_CALL(and_listener, update_input(port, _))
	    .Times(1);
	and_gate.register_listener(&and_listener, port);
    }

    const Value<32> first_value;
    const Value<32> second_value;
    const int port;

    MockListener<32> and_listener;
    AndGate<32> and_gate;
};

TEST_F(GatesTest, Construct) {
    EXPECT_NO_FATAL_FAILURE(({
	AndGate<32> and_gate;
    }) );
}

TEST_F(GatesTest, FirstThenSecond) {
    Value<32> result = first_value & second_value;
    { InSequence seq;
	EXPECT_CALL(and_listener, update_input(port, _))
	    .Times(AnyNumber());
	EXPECT_CALL(and_listener, update_input(port, result))
	    .Times(AtLeast(1));
    }

    and_gate.update_input(0, first_value);
    and_gate.update_input(1, second_value);
}

TEST_F(GatesTest, SecondThenFirst) {
    Value<32> result = first_value & second_value;
    { InSequence seq;
	EXPECT_CALL(and_listener, update_input(port, _))
	    .Times(AnyNumber());
	EXPECT_CALL(and_listener, update_input(port, result))
	    .Times(AtLeast(1));
    }

    and_gate.update_input(1, second_value);
    and_gate.update_input(0, first_value);
}

TEST_F(GatesTest, ManyValues) {
    std::vector<unsigned int> first_values, second_values;
    std::vector<Value<32>> results;

    first_values.push_back(u_numbers(engine));
    second_values.push_back(u_numbers(engine));
    results.push_back(first_values.back() & second_values.back());

    for (int i = 1; i < 1000; ++i) {
	first_values.push_back(u_numbers(engine));
	results.push_back(first_values.back() & second_values.back());
	second_values.push_back(u_numbers(engine));
	results.push_back(first_values.back() & second_values.back());
    }

    { InSequence seq;
	EXPECT_CALL(and_listener, update_input(port, _))
	    .Times(AnyNumber());

	for (auto result : results) {
	    EXPECT_CALL(and_listener, update_input(port, result))
		.Times(AtLeast(1));
	}
    }

    for (int i = 0; i < 1000; ++i) {
	and_gate.update_input(0, first_values[i]);
	and_gate.update_input(1, second_values[i]);
    }
}

class LeftShifterTest : public ::testing::Test {
  protected:
    LeftShifterTest() : port{s_numbers(engine)}
    {
	EXPECT_CALL(shift_listener, update_input(port, _))
	    .Times(1);
	shifter.register_listener(&shift_listener, port);
    }

    const int port;

    MockListener<32> shift_listener;
    LeftShifter<32, 4> shifter;
};

TEST_F(LeftShifterTest, Construct) {
    EXPECT_NO_FATAL_FAILURE(({
	LeftShifter<32, 2> test_shifter;
    }) );
}

TEST_F(LeftShifterTest, VariousShiftValues) {
    std::vector<unsigned int> first_values;
    std::vector<Value<32>> results;

    for (int i = 0; i < 1000; ++i) {
	first_values.push_back(u_numbers(engine));
	results.push_back(first_values.back() << 4);
    }

    { InSequence seq;
	EXPECT_CALL(shift_listener, update_input(port, _))
	    .Times(AnyNumber());

	for (auto result : results) {
	    EXPECT_CALL(shift_listener, update_input(port, result))
		.Times(AtLeast(1));
	}
    }

    for (int i = 0; i < 1000; ++i) {
	shifter.update_input(0, first_values[i]);
    }
}

class ExtenderTest : public ::testing::Test {
  protected:
    ExtenderTest() : port{s_numbers(engine)}
    {
	EXPECT_CALL(extender_listener, update_input(port, _))
	    .Times(1);
	extender.register_listener(&extender_listener, port);
    }

    const int port;

    MockListener<16> extender_listener;
    Extender<8, 16> extender;
};

TEST_F(ExtenderTest, Construct) {
    EXPECT_NO_FATAL_FAILURE(({
	Extender<16, 32> test_extender;
    }) );
}

TEST_F(ExtenderTest, CornerShiftCases) {
    { InSequence seq;
	EXPECT_CALL(extender_listener, update_input(port, _))
	    .Times(AnyNumber());
	EXPECT_CALL(extender_listener, update_input(port, Value<16>{0}))
	    .Times(AtLeast(1));
	EXPECT_CALL(extender_listener, update_input(port, Value<16>{255}))
	    .Times(AtLeast(1));
	EXPECT_CALL(extender_listener, update_input(port, Value<16>{65535}))
	    .Times(AtLeast(1));
    }

    extender.update_input(0, True);
    extender.update_input(0, Value<8>{0});
    extender.update_input(0, False);
    extender.update_input(0, Value<8>{255});
    extender.update_input(0, True);
}

TEST_F(ExtenderTest, VariousShiftValues) {
    std::vector<char> first_values;
    std::vector<Value<16>> results;

    for (int i = 0; i < 1000; ++i) {
	first_values.push_back(c_numbers(engine));
	results.push_back(first_values.back());
    }

    { InSequence seq;
	EXPECT_CALL(extender_listener, update_input(port, _))
	    .Times(AnyNumber());

	for (auto result : results) {
	    EXPECT_CALL(extender_listener, update_input(port, result))
		.Times(AtLeast(1));
	}
    }

    extender.update_input(0, True);

    for (int i = 0; i < 1000; ++i) {
	extender.update_input(0, Value<8>{first_values[i]});
    }
}

class DirectAddressTest : public ::testing::Test {
  protected:
    DirectAddressTest() : port{s_numbers(engine)}
    {
	EXPECT_CALL(address_listener, update_input(port, _))
	    .Times(1);
	concatenator.register_listener(&address_listener, port);
    }

    const int port;

    MockListener<32> address_listener;
    DirectAddress concatenator;
};

TEST_F(DirectAddressTest, ManyValues) {
    std::vector<unsigned int> first_values, second_values;
    std::vector<Value<32>> results;

    first_values.push_back(u_numbers(engine));
    second_values.push_back(u_numbers(engine));
    results.push_back(  (first_values.back() & 0xF0000000)
		      | ((second_values.back() << 2) & 0x0FFFFFFF) );

    for (int i = 1; i < 1000; ++i) {
	first_values.push_back(u_numbers(engine));
	results.push_back(  (first_values.back() & 0xF0000000)
			  | ((second_values.back() << 2) & 0x0FFFFFFF) );
	second_values.push_back(u_numbers(engine));
	results.push_back(  (first_values.back() & 0xF0000000)
			  | ((second_values.back() << 2) & 0x0FFFFFFF) );
    }

    { InSequence seq;
	EXPECT_CALL(address_listener, update_input(port, _))
	    .Times(AnyNumber());

	for (auto result : results) {
	    EXPECT_CALL(address_listener, update_input(port, result))
		.Times(AtLeast(1));
	}
    }

    for (int i = 0; i < 1000; ++i) {
	concatenator.update_input(0, Value<32>{first_values[i]});
	concatenator.update_input(1, Value<26>{second_values[i]});
    }
}
