#include "writer_tests.h"

#include "../io/memory_writer.h"
#include "../io/buffered_writer.h"
#include "../io/writer.h"


void writer_tests()
{
	static_assert(sizeof(memory_writer) <= sizeof(writer), "");

	static_assert(sizeof(std::reference_wrapper<memory_writer>) <= sizeof(writer), "");
	static_assert(sizeof(std::reference_wrapper<buffered_writer>) <= sizeof(writer), "");

	{
		unsigned char buff = 0;
		unsigned char const data = 42;
		{
			auto bw = buffered_writer{writer{buffered_writer{writer{buffered_writer{writer{memory_writer{reinterpret_cast<std::uint8_t*>(reinterpret_cast<char*>(std::addressof(buff)))}}}}}}};
			auto w1 = writer{std::ref(bw)};
			w1.write_bytes(reinterpret_cast<std::uint8_t const*>(reinterpret_cast<char const*>(std::addressof(data))), sizeof(data));
		}
		VERIFY(buff == data);
	}
	{
		unsigned char buff = 0;
		unsigned char const data = 42;
		{
			auto bw = buffered_writer{writer{buffered_writer{writer{buffered_writer{writer{memory_writer{reinterpret_cast<std::uint8_t*>(reinterpret_cast<char*>(std::addressof(buff)))}}}}}}};
			auto w2 = writer{std::move(bw)};
			w2.write_bytes(reinterpret_cast<std::uint8_t const*>(reinterpret_cast<char const*>(std::addressof(data))), sizeof(data));
		}
		VERIFY(buff == data);
	}
	{
		unsigned char buff = 0;
		unsigned char const data = 42;
		{
			auto w3 = writer{memory_writer{reinterpret_cast<std::uint8_t*>(reinterpret_cast<char*>(std::addressof(buff)))}};
			w3.write_bytes(reinterpret_cast<std::uint8_t const*>(reinterpret_cast<char const*>(std::addressof(data))), sizeof(data));
		}
		VERIFY(buff == data);
	}
}
