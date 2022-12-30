# The Organizer C++ library

Usage:
```cpp
// File to apply rules on
QFile file("test.jpg");

// Load the profile file
Profile *profile = ProfileLoader::load("profile.json");
if (profile == nullptr) {
    return;
}

// Apply first matching rule
for (const QList<Rule*> column : profile->rules()) {
    for (Rule* rule : column) {
        if (rule->match(file)) {
            return rule->execute(file);
        }
    }
}

// No matching rule found
return false;
```