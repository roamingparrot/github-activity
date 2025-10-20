#include <curl/curl.h>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::string getUsername()
{
    std::string username{};
    std::cin >> username;
    std::cin.ignore();
    return username;
}

// Write callback for curl
size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    auto *buffer = static_cast<std::vector<char> *>(userp);
    char *data = static_cast<char *>(contents);
    buffer->insert(buffer->end(), data, data + size * nmemb);
    return size * nmemb;
}

void extractAndAppend(std::string &dest, const std::string &source,
                      const std::string &key)
{
    // Match "key":value (value can be number, boolean, null, or quoted string)
    std::regex pattern("\"" + key + "\"\\s*:\\s*([^,}]+)");
    std::smatch match;

    if (std::regex_search(source, match, pattern))
    {
        // match[1] contains the value as string
        dest += key + ": " + match[1].str() + "\n";
    }
    else
    {
        std::cout << key << " not found.\n";
    }
}

int main()
{
    std::cout << "Enter the GitHub username: ";
    std::string username = getUsername();

    CURL *handle = curl_easy_init();
    if (handle)
    {
        std::string url = "https://api.github.com/users/" + username;
        std::vector<char> buffer;

        curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, &buffer);
        curl_easy_setopt(handle, CURLOPT_USERAGENT,
                         "Willy-Vim-Curl"); // GitHub API requires a User-Agent

        CURLcode res = curl_easy_perform(handle);
        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: "
                      << curl_easy_strerror(res) << std::endl;
        }
        else
        {
            std::string handledOutput{};

            std::string response(buffer.begin(), buffer.end());
            extractAndAppend(handledOutput, response, "login");
            extractAndAppend(handledOutput, response, "public_repos");
            extractAndAppend(handledOutput, response, "followers");
            std::cout << handledOutput;
        }

        curl_easy_cleanup(handle);
    }

    return 0;
}
