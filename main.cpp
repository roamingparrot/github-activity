#include <iostream>

void getUsername(std::string& username)
{
    std::cin >> username;
    std::cin.ignore();
}

int main()
{
    std::string username {};

    getUsername(username);

    std::cout << username << "\n";
}