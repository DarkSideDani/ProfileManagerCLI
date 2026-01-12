#ifndef PROFILEMANAGERCLI_PROFILESTORE_HPP
#define PROFILEMANAGERCLI_PROFILESTORE_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include "../domain/Profile.hpp"

class ProfileStore
{
private:
    std::unordered_map<int, Profile> profiles_;
    int next_id_ = 1;

public:
    // Profile creation and return it's assigned ID
    int create_profile(const std::string& name,
                       int age,
                       const std::string& city,
                       const std::string& country);
    // Find profile by id (read-only overloads)
    Profile* find(int id);
    const Profile* find(int id) const;

    // Remove profile by ID. True if removed / False if not found
    bool remove(int ind);

    std::vector<int> list_ids() const; // Return all ID's
    std::size_t size() const; // num of profiles stored

};

#endif //PROFILEMANAGERCLI_PROFILESTORE_HPP