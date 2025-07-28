#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cmath>
#include <sstream>
#include <algorithm> 

#include "json.hpp"  

using json = nlohmann::json;
typedef long double ld;

using namespace std;

 
ld decodeBase(const std::string& value, int base) {
    ld result = 0;
    for (size_t i = 0; i < value.size(); ++i) {
        char c = value[i];
        int digit;
        if (isdigit(c))
            digit = c - '0';
        else
            digit = tolower(c) - 'a' + 10; 
        result = result * base + digit;
    }
    return result;
}


ld lagrangeInterpolation(const vector<pair<ld, ld>>& points) {
    ld result = 0;
    for (int i = 0; i < points.size(); ++i) {
        ld xi = points[i].first;
        ld yi = points[i].second;
        ld li = 1;

        for (int j = 0; j < points.size(); ++j) {
            if (i != j) {
                ld xj = points[j].first;
                li *= (0 - xj) / (xi - xj);  
            }
        }

        result += yi * li;
    }
    return result;
}

ld solveTestCase(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << fileName << endl;
        exit(1);
    }

    json input;
    file >> input;

    int k = input["keys"]["k"];
    std::map<int, pair<int, std::string>> raw_points;

    
    for (auto it = input.begin(); it != input.end(); ++it) {
        if (it.key() == "keys") continue;

        int x = stoi(it.key()); 
        int base = stoi(it.value()["base"].get<std::string>());
        std::string val = it.value()["value"];
        raw_points[x] = {base, val};
    }

   
    vector<pair<ld, ld>> points;
    for (auto it = raw_points.begin(); it != raw_points.end(); ++it) {
        int x = it->first;
        int base = it->second.first;
        string val = it->second.second;
        ld y = decodeBase(val, base);
        points.push_back({(ld)x, y});
    }

   
    sort(points.begin(), points.end());

   
    vector<pair<ld, ld>> selectedPoints(points.begin(), points.begin() + k);

    return lagrangeInterpolation(selectedPoints);
}

int main() {
    std::string testFile1 = "testcase1.json";
    std::string testFile2 = "testcase2.json";

    ld secret1 = solveTestCase(testFile1);
    ld secret2 = solveTestCase(testFile2);

    cout << "Secret for Test Case 1: " << (long long)round(secret1) << endl;
    cout << "Secret for Test Case 2: " << (long long)round(secret2) << endl;

    return 0;
}
