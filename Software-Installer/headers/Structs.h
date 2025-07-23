#pragma once
#include <string>

using namespace std;

// Structure to hold software information
struct Software {
    string name;
    string url;
    string silentFlags;
	// Optional Liscense
    string license; // Optional field for license information
};

