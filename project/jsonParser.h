
// jsonParser.h
#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype> // For isdigit
#include <stdexcept> // For runtime_error

using namespace std;
using namespace glm;

class SimplifiedLevelParser {
public:
    vector<vector<unsigned int>> parseLevelData(const string& filename) {
        string content = loadFileContent(filename);
        return extractTilesData(content);
    }

private:
    string loadFileContent(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Unable to open file: " + filename);
        }
        string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();
        return content;
    }

    vector<vector<unsigned int>> extractTilesData(const string& content) {
        vector<vector<unsigned int>> tiles;
        vector<unsigned int> row;
        bool inArray = false;
        string number;

        for (char ch : content) {
            if (ch == '[') {
                inArray = true;
                if (!row.empty()) { // For nested arrays
                    tiles.push_back(row);
                    row.clear();
                }
            }
            else if (ch == ']') {
                if (!number.empty()) {
                    row.push_back(stoi(number));
                    number.clear();
                }
                inArray = false;
                tiles.push_back(row);
                row.clear();
            }
            else if (isdigit(ch) && inArray) {
                number += ch;
            }
            else if ((ch == ',' || ch == ' ') && !number.empty()) {
                row.push_back(stoi(number));
                number.clear();
            }
        }

        // Handle the case where the last number might not be followed by a comma or space
        if (!number.empty()) {
            row.push_back(stoi(number));
            tiles.push_back(row);
        }

        return tiles;
    }
};


#endif // SIMPLE_JSON_PARSER_H
