#pragma once
#include <string>
namespace OutputFormatting
{
	static std::string ToGreen(std::string text) {
		return "\033[38;5;34m" + text + "\033[0m";
	}
	static std::string ToBlue(std::string text) {
		return "\033[34m" + text + "\033[0m";
	}
	static std::string ToBrightGreen(std::string text) {
		return "\033[38;5;154m" + text + "\033[0m";
	}
	static std::string ToBold(std::string text) {
		return "\033[1m" + text + "\033[0m";
	}
};

