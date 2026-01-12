#include <iostream>
#include "service/ProfileStore.hpp"
#include "cli/Menu.hpp"

int main()
{
    ProfileStore store;

    Menu menu(store);
    menu.run();
    return 0;

}