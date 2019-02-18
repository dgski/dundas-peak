#include <string>
#include <vector>
#include <tuple>
#include <regex>
#include <filesystem>

#include "markdown-to-html/html-element/HTMLElement.h"

using namespace std;

vector<string> splitString(const string& source, char del);
pair<string,string> getLineKeyValuePair(const string& line);
string regex_replace(string templ, const regex& expression, shared_ptr<HTMLElement> elem);
string fileToString(const filesystem::path& input);
void stringToFile(const filesystem::path& filePath, const string& output);
template <class T> T openFile(const filesystem::path& filePath);

#pragma once