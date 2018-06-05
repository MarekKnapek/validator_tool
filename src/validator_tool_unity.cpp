#ifdef UNITY_BUILD


#include "com_port.cpp"
#include "hw2bin.cpp"
#include "hw2hex.cpp"
#include "IO.cpp"
#include "progress.cpp"
#include "txt2bin.cpp"
#include "txt2bin_impl.cpp"
#include "Utils.cpp"
#include "ValidatorUtil.cpp"

#include "commands/bin2hex.cpp"
#include "commands/bin2hex_impl.cpp"
#include "commands/bin2txt.cpp"
#include "commands/bin2txt_impl.cpp"
#include "commands/hex2bin.cpp"
#include "commands/hex2bin_impl.cpp"

#include "io/buffered_writer.cpp"
#include "io/line_view.cpp"
#include "io/txt_writer.cpp"
#include "io/windows_file.cpp"
#include "io/windows_file_byte_reader.cpp"
#include "io/windows_file_byte_writer.cpp"
#include "io/writer.cpp"
#include "io/writer_wrapper_base.cpp"

#include "tests/fake_byte_reader.cpp"
#include "tests/line_reader_tests.cpp"
#include "tests/tests.cpp"
#include "tests/type_eraser_tests.cpp"
#include "tests/type_eraser_tests1.cpp"
#include "tests/type_eraser_tests2.cpp"
#include "tests/type_eraser_tests3.cpp"
#include "tests/type_eraser_tests4.cpp"
#include "tests/type_eraser_tests5.cpp"
#include "tests/type_eraser_tests6.cpp"
#include "tests/writer_tests.cpp"

#include "utils/aligned_alloc.cpp"
#include "utils/convert.cpp"
#include "utils/verify.cpp"
#include "utils/windows_memory.cpp"


#endif
