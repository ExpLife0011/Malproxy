#include "Utils.h"
#include <locale>
#include <codecvt>

std::string FormatString_va(const char* fmt_str, va_list va)
{
	va_list temp_va;
	va_copy(temp_va, va);
	int required_size = vsnprintf(nullptr, 0, fmt_str, temp_va) + 1;
	auto formatted = std::make_unique<char[]>(required_size);
	vsnprintf(&formatted[0], required_size, fmt_str, va);
	va_end(temp_va);
	return std::string(formatted.get());
}

std::string StringUtils::FormatString(const char* fmt_str, ...)
{
	va_list va;
	va_start(va, fmt_str);
	std::string res = FormatString_va(fmt_str, va);
	va_end(va);
	return res;
}

//http://www.cplusplus.com/reference/codecvt/codecvt_utf8_utf16/
std::string StringUtils::Utf16ToUtf8(const std::wstring& utf16_str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t, 0x10ffffUL, std::little_endian>, wchar_t> converter;
	return converter.to_bytes(utf16_str);
}

std::wstring StringUtils::Utf8ToUtf16(const std::string& utf8_str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t, 0x10ffffUL, std::little_endian>, wchar_t> converter;
	return converter.from_bytes(utf8_str);
}
