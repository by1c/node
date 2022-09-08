
#include "helper/sdk/sdk.hpp"

auto main() -> __int32
{
    auto snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); snap != INVALID_HANDLE_VALUE;

    if (snap == INVALID_HANDLE_VALUE)
        return EXIT_FAILURE;

    tagPROCESSENTRY32 entry
    {
        sizeof(tagPROCESSENTRY32)
    };

    for (auto init = Process32First(snap, &entry); init && Process32Next(snap, &entry); Sleep(1))
    {
        if (std::string(entry.szExeFile).find("Discord.exe") == std::string::npos)
            continue;

        auto handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);

        if (handle == nullptr)
            continue;

        sdk::enable_debug_handler(handle);

        auto websocket = easywsclient::WebSocket::from_url("ws://localhost:9229");

        nlohmann::json payload = 
        {
            { "id", 1 },
            { "method" , "Runtime.evaluate" },
            { "params" , {
                { "expression", "alert(\"hi\")" },
                { "includeCommandLineAPI", true },
                { "contextId", 2 }
            }}
        };

        websocket->send(payload.dump());

        for (; websocket->getReadyState() != easywsclient::WebSocket::CLOSED;)
        {
            auto ptr = &*websocket;

            websocket->poll();

            auto dp_fn = [ptr](std::string message) -> void
            {
                ptr->close();
            };

            websocket->dispatch(dp_fn);
        }
    }

    return EXIT_SUCCESS;
}