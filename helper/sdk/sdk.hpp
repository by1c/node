
#include "../hdr.hpp"

namespace sdk
{
	extern auto request(std::string url, std::string path, std::string method, unsigned __int32 port) -> std::string;

	extern auto enable_debug_handler(void* handle) -> bool;
}