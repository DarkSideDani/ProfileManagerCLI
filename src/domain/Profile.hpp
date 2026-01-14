#ifndef PROFILEMANAGERCLI_PROFILE_H
#define PROFILEMANAGERCLI_PROFILE_H

#include <string>
#include <vector>

class Profile{
    private:
        int id_;
        std::string name_;
        int age_;
        std::string city_;
        std::string country_;
        std::vector<std::string> hobbies_;

    public: // public interface (API) : requires the core data for valid Profile creation
        Profile(int id,
                const std::string& name,
                int age,
                const std::string& city,
                const std::string& country);
        // Read-only accessors
        // We add const to the end to imply that this method does not modify the object
        int id() const;
        const std::string& name() const;
        int age() const;
        const std::string& city() const;
        const std::string& country() const;
        const std::vector<std::string>& hobbies() const;

        //Mutators
        void add_hobby(const std::string& hobby);
        bool remove_hobby(const std::string& hobby);
        bool set_name(const std::string& name);
        bool set_age(int age);
        bool set_city(const std::string& city);
        bool set_country(const std::string& country);

    std::string to_string() const; // presentation

};

#endif //PROFILEMANAGERCLI_PROFILE_H