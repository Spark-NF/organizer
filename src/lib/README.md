# The Organizer C++ library

Usage:
```cpp
// File to apply rules on
Media media("test.jpg");

// Load the profile file
Profile *profile = ProfileLoader::loadFile("profile.json");
if (profile == nullptr) {
    return false;
}

// Find matching rules of the highest priority
QList<Rule*> matches = profile->match(file);

// No matching rule found
if (matches.isEmpty()) {
    return false;
}

// Conflicting rules found
if (matches.count() > 1) {
    return false;
}

// Execute rule on the file
return matches.first()->execute(file);
```