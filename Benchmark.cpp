#include <iostream>
#include <benchmark/benchmark.h>
#include "SkipList.h"
#include <ranges>
#include <unordered_set>
#include <filesystem>
#include <fstream>

class sfc64
{
public:
	using result_type = uint64_t;

	static constexpr uint64_t (min)() { return 0; }
	static constexpr uint64_t (max)() { return UINT64_C(-1); }

	sfc64() : sfc64(std::random_device{}())
	{
	}

	explicit sfc64(uint64_t seed) : m_a(seed), m_b(seed), m_c(seed), m_counter(1)
	{
		for (int i = 0; i < 12; ++i)
		{
			operator()();
		}
	}

	uint64_t operator()() noexcept
	{
		auto const tmp = m_a + m_b + m_counter++;
		m_a = m_b ^ (m_b >> right_shift);
		m_b = m_c + (m_c << left_shift);
		m_c = rotl(m_c, rotation) + tmp;
		return tmp;
	}

private:
	template <typename T>
	T rotl(T const x, int k) { return (x << k) | (x >> (8 * sizeof(T) - k)); }

	static constexpr int rotation = 24;
	static constexpr int right_shift = 11;
	static constexpr int left_shift = 3;
	uint64_t m_a;
	uint64_t m_b;
	uint64_t m_c;
	uint64_t m_counter;
};

int64_t RandomNumber()
{
	static auto gen = sfc64{};
	return gen();
}

namespace Basic
{
	template <class Container>
	static void BM_ContainerFindRandom10XInX(benchmark::State& state)
	{
		for (auto _ : state)
		{
			Container cont;
			for (auto i : std::ranges::iota_view{0, state.range(0)})
				cont.insert(RandomNumber());

			for (auto i : std::ranges::iota_view{0, 10*state.range(0)})
				const auto p = cont.find(RandomNumber());
		}
	}
	
	BENCHMARK_TEMPLATE(BM_ContainerFindRandom10XInX, std::set<int64_t>)->Range(0,32<<10);
	BENCHMARK_TEMPLATE(BM_ContainerFindRandom10XInX, std::unordered_set<int64_t>)->Range(0,32<<10);
	BENCHMARK_TEMPLATE(BM_ContainerFindRandom10XInX, skip_list<int64_t>)->Range(0,32<<10);
}

