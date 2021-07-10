#include <gtest/gtest.h>
#include <ranges>
#include <algorithm>
#include "SkipList.h"


namespace functional_tests
{
	TEST(Basic, CanInsertOne)
	{
		skip_list sl;
		sl.push_back(5);
		std::cout << std::endl << sl << std::endl;
		ASSERT_EQ(sl.front(), 5);
	}

	TEST(Basic, FindEmptyIsEnd)
	{
		skip_list sl;
		std::cout << std::endl << sl << std::endl;
		ASSERT_EQ(sl.find(5), sl.end());
	}

	TEST(Basic, FindNonExistingIsEnd)
	{
		skip_list sl;
		sl.push_back(12);
		std::cout << std::endl << sl << std::endl;
		ASSERT_EQ(sl.find(5), sl.end());
	}

	TEST(Basic, DeleteEmptyIsNoOp)
	{
		skip_list sl;
		std::cout << std::endl << sl << std::endl;
		ASSERT_NO_THROW(sl.pop(1));
	}

	TEST(Basic, DeleteNonExistingIsNoOp)
	{
		skip_list sl;
		sl.push_back(123);
		std::cout << std::endl << sl << std::endl;
		ASSERT_NO_THROW(sl.pop(1));
	}

	TEST(Basic, SortedAfterInsert3DeleteMiddle)
	{
		skip_list sl;
		sl.push_back(32);
		sl.push_back(3);
		sl.push_back(6);
		sl.pop(6);
		ASSERT_EQ(sl.front(), 3);
		ASSERT_EQ(sl.back(), 32);
	}

	TEST(Basic, SortedAfterInsert3DeleteLast)
	{
		skip_list sl;
		sl.push_back(32);
		sl.push_back(3);
		sl.push_back(6);
		sl.pop(32);
		ASSERT_EQ(sl.front(), 3);
		ASSERT_EQ(sl.back(), 6);
	}

	TEST(Basic, SortedAfterInsert3DeleteFirst)
	{
		skip_list sl;
		sl.push_back(32);
		sl.push_back(3);
		sl.push_back(6);
		sl.pop(3);
		ASSERT_EQ(sl.front(), 6);
		ASSERT_EQ(sl.back(), 32);
	}

	TEST(Basic, CanDeleteOne)
	{
		skip_list sl;
		sl.push_back(5);
		sl.pop(5);
		std::cout << std::endl << sl << std::endl;
		ASSERT_EQ(sl.size(), 0);
	}

	TEST(Basic, CanInsertOneAndFindIt)
	{
		skip_list sl;
		sl.push_back(5);
		std::cout << std::endl << sl << std::endl;
		ASSERT_EQ(*sl.find(5), 5);
	}

	TEST(Basic, CanInsertSmallAndLargeSorted)
	{
		skip_list sl;
		sl.push_back(5);
		sl.push_back(7);
		std::cout << std::endl << sl << std::endl;
		ASSERT_EQ(sl.front(), 5);
		ASSERT_EQ(sl.back(), 7);
	}

	TEST(Basic, CanInsertLargeAndSmallSorted)
	{
		skip_list sl;
		sl.push_back(7);
		sl.push_back(5);
		std::cout << std::endl << sl << std::endl;
		ASSERT_EQ(sl.front(), 5);
		ASSERT_EQ(sl.back(), 7);
	}

	TEST(Insertion, CanInsertSorted10)
	{
		skip_list sl;
		auto initial = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

		std::ranges::copy(initial, std::back_inserter(sl));

		std::cout << std::endl << sl << std::endl;
		ASSERT_EQ(sl.size(), 10);
		ASSERT_TRUE(std::ranges::equal(initial, sl));
	}

	TEST(Insertion, CanInsertSame10)
	{
		skip_list sl;
		auto initial = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

		std::ranges::copy(initial, std::back_inserter(sl));

		std::cout << std::endl << sl << std::endl;
		ASSERT_EQ(sl.size(), 10);
		ASSERT_TRUE(std::ranges::equal(initial, sl));
	}

	TEST(Insertion, CanInsertReversed10)
	{
		skip_list sl;
		auto initial = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

		std::ranges::copy(initial | std::views::reverse, std::back_inserter(sl));

		std::cout << std::endl << sl << std::endl;
		ASSERT_EQ(sl.size(), 10);
		ASSERT_TRUE(std::ranges::equal(initial, sl));
	}

	TEST(Insertion, CanInsertShuffle10)
	{
		skip_list sl;
		std::array initial = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		std::random_device rd;
		std::mt19937 gen{rd()};

		std::ranges::shuffle(initial, gen);
		std::ranges::copy(initial, std::back_inserter(sl));

		std::cout << std::endl << sl << std::endl;
		std::ranges::sort(initial);
		ASSERT_EQ(sl.size(), 10);
		ASSERT_TRUE(std::ranges::equal(initial, sl));
	}

	auto random_integer_generator()
	{
		static std::uniform_int_distribution distribution{-100, 100};
		static std::random_device device;
		static std::mt19937 engine{device()};
		return distribution(engine);
	}

	TEST(Insertion, CanInsertRandom10)
	{
		skip_list sl;

		std::array<int, 10> initial{};
		std::ranges::generate(initial, random_integer_generator);
		std::ranges::copy(initial, std::back_inserter(sl));

		std::cout << std::endl << sl << std::endl;
		std::ranges::sort(initial);
		ASSERT_EQ(sl.size(), 10);
		ASSERT_TRUE(std::ranges::equal(initial, sl));
	}

	TEST(Deletion, CanInsertSorted10AndDeleteEvens)
	{
		skip_list sl;
		std::array initial = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

		std::ranges::copy(initial, std::back_inserter(sl));

		for (const auto& i : initial)
			if (i % 2 == 0) sl.pop(i);

		std::cout << std::endl << sl << std::endl;
		ASSERT_EQ(sl.size(), 5);
		ASSERT_TRUE(std::ranges::equal(
			initial | std::views::filter([](const auto& i) { return i % 2 != 0; }),
			sl));
	}

	TEST(Deletion, CanInsertSorted10AndDeleteDuplicates)
	{
		skip_list sl;
		std::array initial = {1, 2, 2, 4, 5, 5, 5, 8, 9, 9};

		std::ranges::copy(initial, std::back_inserter(sl));
		auto p = std::ranges::remove_if(sl, [](int i) { return i % 2 == 0; });

		ASSERT_EQ(sl.size(), 5);
		ASSERT_TRUE(std::ranges::equal(initial, sl));
	}

	/******
	TEST(Insertion, CanInsertReversed10)
	{
		skip_list sl;
		auto initial = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

		std::ranges::copy(initial | std::views::reverse, std::back_inserter(sl));

		std::cout << std::endl << sl << std::endl;
		ASSERT_EQ(sl.size(), 10);
		ASSERT_TRUE(std::ranges::equal(initial, sl));
	}

	TEST(Insertion, CanInsertShuffle10)
	{
		skip_list sl;
		std::array initial = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		std::random_device rd;
		std::mt19937 gen{rd()};

		std::ranges::shuffle(initial, gen);
		std::ranges::copy(initial, std::back_inserter(sl));

		std::cout << std::endl << sl << std::endl;
		std::ranges::sort(initial);
		ASSERT_EQ(sl.size(), 10);
		ASSERT_TRUE(std::ranges::equal(initial, sl));
	}

	auto random_integer_generator()
	{
		static std::uniform_int_distribution distribution{-100, 100};
		static std::random_device device;
		static std::mt19937 engine{device()};
		return distribution(engine);
	}

	TEST(Insertion, CanInsertRandom10)
	{
		skip_list sl;

		std::array<int, 10> initial{};
		std::ranges::generate(initial, random_integer_generator);
		std::ranges::copy(initial, std::back_inserter(sl));

		std::cout << std::endl << sl << std::endl;
		std::ranges::sort(initial);
		ASSERT_EQ(sl.size(), 10);
		ASSERT_TRUE(std::ranges::equal(initial, sl));
	}***/
}
