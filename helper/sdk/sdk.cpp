#include "sdk.hpp"

auto sdk::request(std::string url, std::string path, std::string method, unsigned __int32 port) -> std::string
{
	auto h_internet = InternetOpenA("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Safari/537.36", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	auto h_connect = InternetConnectA(h_internet, &url[0], 80, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);

	const char* accept_types[] = { "text/*", 0 };

	auto h_request = HttpOpenRequestA(h_connect, &method[0], &path[0], NULL, NULL, accept_types, 0, 0);
	auto request_sent = HttpSendRequestA(h_request, NULL, 0, NULL, 0);

	std::string response;

	auto keep_reading = true;
	unsigned long bytes_read = 1;

	std::string buffer(0x400, 0);

	for (; keep_reading && bytes_read != 0;)
	{
		keep_reading = InternetReadFile(h_request, &buffer[0], 0x400, &bytes_read);
		response.append(&buffer[0], bytes_read);
	}

	return response;
}

auto sdk::enable_debug_handler(void* handle) -> bool
{
    std::string mp_name(32, 0);

    std::sprintf(&mp_name[0], "node-debug-handler-%u", GetProcessId(handle));

    auto mp = OpenFileMappingA(FILE_MAP_READ, FALSE, &mp_name[0]);;

    if (mp != nullptr)
    {
        auto data = MapViewOfFile(mp, FILE_MAP_READ, 0, 0, 8);

        if (data != nullptr)
        {
			auto thread = CreateRemoteThread(handle, nullptr, 0, *(LPTHREAD_START_ROUTINE*)data, nullptr, 0, nullptr);

            return thread != nullptr;
        }
    }

	return false;
}


