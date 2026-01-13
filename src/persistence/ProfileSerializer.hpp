#ifndef PROFILEMANAGERCLI_PROFILESERIALIZER_HPP
#define PROFILEMANAGERCLI_PROFILESERIALIZER_HPP

#include <string>

class ProfileStore;

class ProfileSerializer
{
    public:
        // Save all profiles to disk. Return true on success
        static bool save(const ProfileStore& store, const std::string& path);
        // Load profiles from disk into store (overwrites existing in-memory store)
        static bool load(ProfileStore& store, const std::string& path);
};


#endif //PROFILEMANAGERCLI_PROFILESERIALIZER_HPP