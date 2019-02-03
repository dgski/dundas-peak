#include <string>

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