#include <iostream>
#include <string>
#include <algorithm>

template<class T, typename = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>>
void add_to_str(std::string& str, T val){ str += std::to_string(val); }

void add_to_str(std::string& str, char val){ str += val; }
void add_to_str(std::string& str, const char* val){ str += val; }
void add_to_str(std::string& str, const std::string& val){ str += val; }

template<class Arg>
void format_str_impl(const std::string& fstr, std::string& result, std::string::const_iterator curr, const Arg& arg)
{
	auto it = std::find(curr, std::cend(fstr), '%');
	if(it == std::cend(fstr)){
		return;
	}
	if(it == std::cbegin(fstr) || *(it - 1) != '\\'){
		std::copy(curr, it, std::back_inserter(result));
		add_to_str(result, arg);
		std::copy(it + 1, std::cend(fstr), std::back_inserter(result));
		return;
	}
	else {
		std::copy(curr, it - 1, std::back_inserter(result));
		result += *it;
	}
	format_str_impl(fstr, result, it + 1, arg);
}

template<class Arg, class... Args>
void format_str_impl(const std::string& fstr, std::string& result, std::string::const_iterator curr, const Arg& arg, Args&&... args)
{
	auto it = std::find(curr, std::cend(fstr), '%');
	if(it == std::cend(fstr)){
		return;
	}
	if(it == std::cbegin(fstr) || *(it - 1) != '\\'){
		std::copy(curr, it, std::back_inserter(result));
		add_to_str(result, arg);
		format_str_impl(fstr, result, it + 1, std::forward<Args>(args)...);
	}
	else {
		std::copy(curr, it - 1, std::back_inserter(result));
		result += *it;
		format_str_impl(fstr, result, it + 1, arg, std::forward<Args>(args)...);
	}
}

template<class... Args>
std::string format_str(const std::string& fstr, Args&&... args)
{
	std::string result;
	result.reserve(std::size(fstr));
	format_str_impl(fstr, result, std::cbegin(fstr), std::forward<Args>(args)...);
	return result;
}

int main(int argc, char* argv[])
{
	std::string str = "a = %,\\%\\% b = %,\\% c = % sdf";
	std::cout << format_str(str, "1", std::string("2"), 3.0) << std::endl;
	return 0;
}