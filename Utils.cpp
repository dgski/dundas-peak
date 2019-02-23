#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include "Utils.h"

vector<string> splitString(const string& source, char del)
{
    vector<string> result;
    
    string current;
    for(char i : source)
    {
        if(i == del)
        {   
            result.push_back(current);
            current.clear();
            continue;
        }

        if(i != ' ' && i != '\n' && i != '\r' && i != '\t')
            current += i;
    }

    if(!current.empty())
        result.push_back(current);

    return result;
}

pair<string,string> getLineKeyValuePair(const string& line)
{
    pair<string,string> result;

    string* dest = &(get<0>(result));
    bool keyGrabbed = false;

    for(char c : line)
    {
        if(c == ':' && keyGrabbed == false)
        {
            keyGrabbed = true;
            dest = &(get<1>(result));
        }
        else
        {
            *dest += c;
        }
    }

    return result;
}

string regex_replace(string templ, const regex& expression, shared_ptr<HTMLElement> elem)
{
    stringstream s;
    s << *elem;
    return regex_replace(templ, expression, s.str());
}

string fileToString(const filesystem::path& filePath, const char* def)
{
    if(!filesystem::exists(filePath))
        return string(def);

    stringstream s;
    s << openFile<ifstream>(filePath).rdbuf();

    return s.str();
}

void stringToFile(const filesystem::path& filePath, const string& output)
{
    openFile<ofstream>(filePath) << output;
}

template <class T>
T openFile(const filesystem::path& filePath)
{
    T f(filePath);
    if(!f.is_open())
    {
        string errString("Could Not Open File: ");
        errString.append(filePath);
        throw runtime_error(errString);
    }

    return f;
}