#include "ProfileStore.hpp"
#include <algorithm> // std::sort


// ProfileStore: holds all profiles in memory, CRUD operations, id generation

int ProfileStore::create_profile(const std::string& name,
                                 int age,
                                 const std::string& city,
                                 const std::string& country)
{
    const int id = next_id_++;
    // Construct Profile in-place in the map.
    profiles_.emplace(id, Profile(id, name, age, city, country));
    return id;
}

// Finds a profile by ID and returns a pointer that allows modification & returns nullptr if the profile doesn't exist
Profile* ProfileStore::find(int id)
{
    auto it = profiles_.find(id);
    if (it == profiles_.end()) return nullptr;
    return &it->second; // returning the pointer to the stored profile object
}

//Const overload of find() & Allows read-only access when ProfileStore itself is const
const Profile* ProfileStore::find(int id) const
{
    auto it = profiles_.find(id);
    if (it == profiles_.end()) return nullptr;
    return &it->second;
}

// Removes a profile by ID & Returns true if a profile was removed, false if ID was found.
bool ProfileStore::remove(int id)
{
    return profiles_.erase(id) > 0; // erase() returns the number of elements removed (0 or 1 in this scenario)
}

//Returns a sorted list of all profile ID's currently stored
std::vector<int> ProfileStore::list_ids() const
{
    std::vector<int> ids;
    ids.reserve(profiles_.size()); // reserve memory upfront to avoid reallocations

    for (const auto& kv : profiles_) { // Extract all ID's (keys) from unordered_map
        ids.push_back(kv.first);
    }
    // Sort ID's so output order is consistent
    std::sort(ids.begin(), ids.end());
    return ids;
}

// Returns the number of profiles currently stored
std::size_t ProfileStore::size() const
{
    return profiles_.size(); // cheap O(1) query
}

void ProfileStore::clear()
{
    profiles_.clear();
    next_id_ = 1;
}

bool ProfileStore::insert_profile(const Profile& profile)
{
    const int id = profile.id();

    // If the id already exists, reject to avoid collisions.
    auto [it, inserted] = profiles_.emplace(id, profile);
    if (!inserted) return false;

    // Ensure next_id_ will never reuse an existing id.
    if (id >= next_id_) {
        next_id_ = id + 1;
    }
    return true;
}
