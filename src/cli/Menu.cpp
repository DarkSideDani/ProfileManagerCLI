#include "Menu.hpp"
#include <iostream>
#include <limits> // needed to discard input safely std::numeric_limits<std::streamsize>::max()
#include "../persistence/ProfileSerializer.hpp"

// Menu: input/output + command loop (no business logic)

Menu::Menu(ProfileStore& store) : store_(store){}

void Menu::run()
{
    while (true)
    {
        std::cout << "\n=== Profile Manager CLI ===\n"
                  << "1) Create profile\n"
                  << "2) View profile (by id)\n"
                  << "3) List profiles\n"
                  << "4) Delete profile\n"
                  << "5) Add hobby\n"
                  << "6) Remove hobby\n"
                  << "7) Save to file\n"
                  << "8) Load from file\n"
                  << "0) Exit\n";
        int choice = read_int("Select option: ");

        switch (choice){
            case 1: create_profile(); break;
            case 2: view_profile(); break;
            case 3: list_profiles(); break;
            case 4: delete_profile(); break;
            case 5: add_hobby(); break;
            case 6: remove_hobby(); break;
            case 7: save_to_file(); break;
            case 8: load_from_file(); break;
            case 0:
                std::cout << "Goodbye.\n";
                return;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    }
}

void Menu::create_profile()
{
    std::string name = read_line("Name: ");
    int age = read_int("Age: ");
    std::string city = read_line("City: ");
    std::string country = read_line("Country: ");

    // store assigns an ID, creates the Profile, and stores it internally
    int id = store_.create_profile(name, age, city, country);
    std::cout << "Created profile with id: " << id << "\n";
}

void Menu::view_profile()
{
    int id = read_int("Enter profile id: ");
    const Profile* p = store_.find(id); // const overload of find() since we only read data here

    if (!p)
    {
        std::cout << "No profile found with ID: " << id << ".\n";
        return;
    }
    std::cout << "\n" << p->to_string();
}

void Menu::list_profiles() // list all profiles (ID and name) for quick browsing
{
    auto ids = store_.list_ids(); // list_ids() returns all IDs sorted for stable otuput
    if (ids.empty())
    {
        std::cout << "No profiles yet.\n";
        return;
    }
    std::cout << "Profiles ( " << store_.size() << "):\n";
    for (int id : ids)
    {
        const Profile* p = store_.find(id);
        // p should never be null here, but we keep it defensive
        if (p)
        {
            std::cout << "- [" << p->id() << "] " << p->name() << "\n";
        }
    }
}

void Menu::delete_profile()
{
    int id = read_int("Enter profile id to delete: ");
    if (store_.remove(id)) // remove() returns true if something was removed, false if not found
    {
        std::cout << "Deleted profile " << id << "\n";
    } else
    {
        std::cout << "No profile found with ID " << id << ".\n";
    }
}

void Menu::add_hobby()
{
    int id = read_int("Enter profile id: ");
    Profile* p = store_.find(id); // non-const find() because we want to modify the profile (add hobby)

    if (!p)
    {
        std::cout << "No profile found with ID: " << id << ".\n";
        return;
    }

    std::string hobby = read_line("Hobby to add: ");
    p->add_hobby(hobby);
    std::cout << "Hobby added.\n";
}

void Menu::remove_hobby()
{
    int id = read_int("Enter profile id: ");
    Profile* p = store_.find(id);
    if (!p)
    {
        std::cout << "No profile found with ID: " << id << ".\n";
        return;
    }

    std::string hobby = read_line("Hobby to remove: ");
    if (p->remove_hobby(hobby))
    {
        std::cout << "Hobby removed.\n";
    } else
    {
        std::cout << "Hobby not found.\n";
    }

}

// Handles non-numeric input & reads an integer safely from stdin
int Menu::read_int(const char* prompt)
{
    while (true)
    {
        std::cout << prompt;

        int value;
        if (std::cin >> value)
        {   // Consume the rest of the line so subsequent getline() works
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        // Clear fail state and discard bad input
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid number, Try again.\n";
    }
}
// Reads a full line (including spaces)
std::string Menu::read_line(const char* prompt)
{
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

void Menu::save_to_file()
{
    std::string path = read_line("Enter file path to save (ex: profiles.txt) ");
    if (ProfileSerializer::save(store_, path))
    {
        std::cout << "Saved to " << path << "\n";
    } else
    {
        std::cout << "Failed to save to " << path << "\n";
    }
}

void Menu::load_from_file()
{
    std::string path = read_line("Enter file path to load (ex: profiles.txt) ");
    if (ProfileSerializer::load(store_, path))
    {
        std::cout << "Loaded from " << path << "\n";
    } else
    {
        std::cout << "Failed to load from " << path << " (missing file or invalid format) \n";
    }
}
