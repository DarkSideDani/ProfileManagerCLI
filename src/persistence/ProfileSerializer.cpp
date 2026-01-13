#include "ProfileSerializer.hpp"
#include "../service/ProfileStore.hpp"
#include "../domain/Profile.hpp"

#include <fstream>
#include <vector>
#include <string>

// ProfileSerializer: save/load from file
// One profile per line in a tab-separated format:
// <id>\t<age>\t<name>\t<city>\t<country>\t<hobby1|hobby2|hobby3>

namespace
{
    // Escapes chars that would break our separators.
    // We escape: backslash, tab, newline, pipe '|'
    std::string escape_field(const std::string& s)
    {
        std::string out;
        out.reserve(s.size());

        for (char ch : s)
        {
            switch (ch)
            {
                case '\\': out += "\\\\"; break; // '\'  -> "\\"
                case '\t': out += "\\t";  break; // tab -> "\t"
                case '\n': out += "\\n";  break; // nl  -> "\n"
                case '|':  out += "\\|";  break; // '|' -> "\|"
                default:   out += ch;     break;
            }
        }
        return out;
    }

    // Converts escaped sequences back to original characters.
    // Recognizes: "\\", "\t", "\n", "\|"
    std::string unescape_field(const std::string& s)
    {
        std::string out;
        out.reserve(s.size());

        for (size_t i = 0; i < s.size(); ++i)
        {
            char ch = s[i];

            // Escape sequence begins with backslash and must have a next char
            if (ch == '\\' && i + 1 < s.size())
            {
                char next = s[i + 1];

                switch (next)
                {
                    case '\\': out += '\\'; ++i; break; // "\\\\" -> '\'
                    case 't':  out += '\t'; ++i; break; // "\\t"  -> tab
                    case 'n':  out += '\n'; ++i; break; // "\\n"  -> newline
                    case '|':  out += '|';  ++i; break; // "\\|"  -> '|'
                    default:
                        // Unknown escape sequence: keep the backslash literally
                        out += ch;
                        break;
                }
            }
            else
            {
                out += ch;
            }
        }
        return out;
    }

    // Split hobbies by '|' BUT only when the '|' is NOT escaped.
    // This prevents breaking hobbies like "Gym|Weights" which are stored as "Gym\|Weights".
    std::vector<std::string> split_unescaped_pipes(const std::string& s)
    {
        std::vector<std::string> parts;
        std::string current;

        for (size_t i = 0; i < s.size(); ++i)
        {
            // If we see an escape introducer, copy it and the next char as-is
            // so the token keeps sequences like "\|" intact until unescape_field().
            if (s[i] == '\\' && i + 1 < s.size())
            {
                current.push_back(s[i]);
                current.push_back(s[i + 1]);
                ++i;
                continue;
            }

            // Split only on an actual separator '|'
            if (s[i] == '|')
            {
                parts.push_back(current);
                current.clear();
            }
            else
            {
                current.push_back(s[i]);
            }
        }

        parts.push_back(current);
        return parts;
    }
}

bool ProfileSerializer::save(const ProfileStore& store, const std::string& path)
{
    std::ofstream out(path, std::ios::trunc);
    if (!out) return false;

    // File header so that we can detect format/version later.
    out << "PMCLI1\n";

    // Store provides stable ordering via list_ids()
    for (int id : store.list_ids())
    {
        const Profile* p = store.find(id);
        if (!p) continue; // defensive

        // Serialize hobbies as hobby1|hobby2|hobby3
        // IMPORTANT:
        // - We escape EACH hobby text (so hobby text can contain '|', tabs, etc.)
        // - We do NOT escape the separator itself; separator stays as raw '|'
        std::string hobbies_joined;
        const auto& hobbies = p->hobbies();

        for (size_t i = 0; i < hobbies.size(); ++i)
        {
            hobbies_joined += escape_field(hobbies[i]);
            if (i + 1 < hobbies.size()) hobbies_joined += "|";
        }

        // We escape text fields to ensure TAB/NEWLINE/'|' never breaks parsing.
        out << p->id() << '\t'
            << p->age() << '\t'
            << escape_field(p->name()) << '\t'
            << escape_field(p->city()) << '\t'
            << escape_field(p->country()) << '\t'
            << hobbies_joined
            << "\n";
    }

    return true;
}

bool ProfileSerializer::load(ProfileStore& store, const std::string& path)
{
    std::ifstream in(path);
    if (!in) return false;

    // Read the first line as header
    std::string header;
    if (!std::getline(in, header)) return false;

    // Windows CRLF fix: strip trailing '\r'
    if (!header.empty() && header.back() == '\r') header.pop_back();

    // Validate file format
    if (header != "PMCLI1") return false;

    // Replace in-memory data with disk data
    store.clear();

    std::string line;
    while (std::getline(in, line))
    {
        // Windows CRLF fix: strip trailing '\r'
        if (!line.empty() && line.back() == '\r') line.pop_back();

        if (line.empty()) continue;

        // Split the line by TAB into fields:
        // fields[0]=id, fields[1]=age, fields[2]=name, fields[3]=city, fields[4]=country, fields[5]=hobbies(optional)
        std::vector<std::string> fields;
        {
            std::string current;
            for (char ch : line)
            {
                if (ch == '\t')
                {
                    fields.push_back(current);
                    current.clear();
                }
                else
                {
                    current.push_back(ch);
                }
            }
            fields.push_back(current);
        }

        // Require at least 5 fields (id, age, name, city, country)
        if (fields.size() < 5) continue;

        int id = 0;
        int age = 0;

        try
        {
            id  = std::stoi(fields[0]);
            age = std::stoi(fields[1]);
        }
        catch (...)
        {
            // Skip malformed numeric fields rather than crashing.
            continue;
        }

        // Unescape text fields to restore original content
        std::string name    = unescape_field(fields[2]);
        std::string city    = unescape_field(fields[3]);
        std::string country = unescape_field(fields[4]);

        Profile p(id, name, age, city, country);

        // Hobbies are optional
        if (fields.size() >= 6 && !fields[5].empty())
        {
            // Split only on unescaped pipes
            auto hobby_tokens = split_unescaped_pipes(fields[5]);

            // Unescape each hobby token to restore original hobby text
            for (const auto& token : hobby_tokens)
            {
                p.add_hobby(unescape_field(token));
            }
        }

        // Insert profile; if duplicate id exists, skip
        if (!store.insert_profile(p)) continue;
    }

    return true;
}
