#ifndef PROFILEMANAGERCLI_MENU_H
#define PROFILEMANAGERCLI_MENU_H

#include "../service/ProfileStore.hpp"

class Menu
{
public:
    // Menu holds a reference to the store (ProfileStore& store_), so we don’t copy it.
    // explicit prevents accidental implicit conversions
    explicit Menu(ProfileStore& store);

    // Main CLI loop
    void run();

private:
    ProfileStore& store_;

    // Menu Actions
    void create_profile();
    void view_profile();
    void list_profiles();
    void delete_profile();
    void add_hobby();
    void remove_hobby();
    void save_to_file();
    void load_from_file();

    // Input helpers
    int read_int(const char* prompt);
    std::string read_line(const char* prompt);

};




#endif //PROFILEMANAGERCLI_MENU_H
