#include "writer_tests.h"

#include "../io/windows_file_byte_writer.h"
#include "../io/buffered_writer.h"
#include "../io/writer.h"


void writer_tests()
{
	static_assert(sizeof(windows_file_byte_writer) <= sizeof(writer), "");

	static_assert(sizeof(std::reference_wrapper<windows_file_byte_writer>) <= sizeof(writer), "");
	static_assert(sizeof(std::reference_wrapper<buffered_writer>) <= sizeof(writer), "");

	{
		auto bw = buffered_writer{writer{buffered_writer{writer{buffered_writer{writer{windows_file_byte_writer{reinterpret_cast<HANDLE>(0x1)}}}}}}};
		auto w1 = writer{std::ref(bw)};
		VERIFY(dynamic_cast<writer_wrapper_base*>(w1.get()));
		VERIFY((dynamic_cast<writer_wrapper<std::reference_wrapper<buffered_writer>, true>*>(w1.get())));
		auto const& layer1 = dynamic_cast<writer_wrapper<std::reference_wrapper<buffered_writer>, true>*>(w1.get())->get().get().get();
		VERIFY(dynamic_cast<writer_wrapper_base*>(layer1.get()));
		VERIFY((dynamic_cast<writer_wrapper<buffered_writer, false>*>(layer1.get())));
		auto const& layer2 = dynamic_cast<writer_wrapper<buffered_writer, false>*>(layer1.get())->get().get();
		VERIFY(dynamic_cast<writer_wrapper_base*>(layer2.get()));
		VERIFY((dynamic_cast<writer_wrapper<buffered_writer, false>*>(layer2.get())));
		auto const& layer3 = dynamic_cast<writer_wrapper<buffered_writer, false>*>(layer2.get())->get().get();
		VERIFY(dynamic_cast<writer_wrapper_base*>(layer3.get()));
		VERIFY((dynamic_cast<writer_wrapper<windows_file_byte_writer, true>*>(layer3.get())));
		auto const& layer4 = dynamic_cast<writer_wrapper<windows_file_byte_writer, true>*>(layer3.get())->get().get();
		VERIFY(layer4 == reinterpret_cast<HANDLE>(0x1));
	}
	{
		auto bw = buffered_writer{writer{buffered_writer{writer{buffered_writer{writer{windows_file_byte_writer{reinterpret_cast<HANDLE>(0x2)}}}}}}};
		auto w2 = writer{std::move(bw)};
		VERIFY(dynamic_cast<writer_wrapper_base*>(w2.get()));
		VERIFY((dynamic_cast<writer_wrapper<buffered_writer, false>*>(w2.get())));
		auto const& layer1 = dynamic_cast<writer_wrapper<buffered_writer, false>*>(w2.get())->get().get();
		VERIFY(dynamic_cast<writer_wrapper_base*>(layer1.get()));
		VERIFY((dynamic_cast<writer_wrapper<buffered_writer, false>*>(layer1.get())));
		auto const& layer2 = dynamic_cast<writer_wrapper<buffered_writer, false>*>(layer1.get())->get().get();
		VERIFY(dynamic_cast<writer_wrapper_base*>(layer2.get()));
		VERIFY((dynamic_cast<writer_wrapper<buffered_writer, false>*>(layer2.get())));
		auto const& layer3 = dynamic_cast<writer_wrapper<buffered_writer, false>*>(layer2.get())->get().get();
		VERIFY(dynamic_cast<writer_wrapper_base*>(layer3.get()));
		VERIFY((dynamic_cast<writer_wrapper<windows_file_byte_writer, true>*>(layer3.get())));
		auto const& layer4 = dynamic_cast<writer_wrapper<windows_file_byte_writer, true>*>(layer3.get())->get().get();
		VERIFY(layer4 == reinterpret_cast<HANDLE>(0x2));
	}
	{
		auto w3 = writer{windows_file_byte_writer{reinterpret_cast<HANDLE>(0x3)}};
		VERIFY(dynamic_cast<writer_wrapper_base*>(w3.get()));
		VERIFY((dynamic_cast<writer_wrapper<windows_file_byte_writer, true>*>(w3.get())));
		auto const& layer1 = dynamic_cast<writer_wrapper<windows_file_byte_writer, true>*>(w3.get())->get().get();
		VERIFY(layer1 == reinterpret_cast<HANDLE>(0x3));
	}
}
