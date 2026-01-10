#include "Profile.hpp"
#include <algorithm>
#include <sstream> // streams that behave like std::cout, but write into memory (a string) instead of the terminal.
#include <cstddef> // not super required but in strict env you might want it (for size_t)

// domain model + methods (add/remove hobby, view string)

Profile::Profile(int id, const std::string& name, int age, const std::string& city, const std::string& country)
                : id_(id), name_(name), age_(age), city_(city), country_(country){} // <-- initializer list

// Getters
int Profile::id() const{return id_;}
const std::string& Profile::name() const{return name_;} // const - avoids copying the str every time & more efficient
int Profile::age() const{return age_;}
const std::string& Profile::city() const{return city_;}
const std::string& Profile::country() const{return country_;}
const std::vector<std::string>& Profile::hobbies() const{return hobbies_;}

// Not const because of object mutation
void Profile::add_hobby(const std::string& hobby){
    if (hobby.empty()) return; // validation for MVP
    hobbies_.push_back(hobby);
}

bool Profile::remove_hobby(const std::string& hobby){
    auto it = std::find(hobbies_.begin(), hobbies_.end(), hobby);
    if (it == hobbies_.end()) return false;
    hobbies_.erase(it); // if found remove the element from the vector
    return true;
}

std::string Profile::to_string() const{
    std::ostringstream out; // output string stream
    out << "Id: " << id_ << "\n"
        << "Name: " << name_ << "\n"
        << "Age: " << age_ << "\n"
        << "City: " << city_ << "\n"
        << "Country: " << country_ << "\n"
        << "Hobbies: ";

    for (size_t i = 0; i < hobbies_.size(); ++i){
        out << hobbies_[i];
        if (i + 1 < hobbies_.size()) out << ", ";
    }
    out << "\n";
    return out.str(); // str() returns the accumulated contents of the stream as an std::string
}


