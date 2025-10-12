#include <iostream>
#include <string>

#include <curl/curl.h>

std::string getUsername()
{
    std::string username {};
    std::cin >> username;
    std::cin.ignore();
    return username;
}



int main()
{
    std::cout << "Enter the github username: ";
    std::string username = getUsername();

    CURL* handle = curl_easy_init;
    if (handle)
    {
        curl_easy_setopt(handle, CURLOPT_URL, "httos://api.github.com");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback); // error no writeCallback function

        CURLcode res = curl_easy_perform(handle); // CURLcode is enum for error message - easy preform does https request
    }
}