#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <regex>

#include "html-element/HTMLElement.h"

using namespace std;
namespace fs = filesystem;

struct Link
{
    string text;
    string url;
};

class SiteInfo
{
    string name;
    string tagline;
    vector<Link> links;
    string mainColor;
    string secondaryColor;

public:
    shared_ptr<HTMLElement> header;

    SiteInfo()
    {
        header = make_HTMLElement("div");
    }

    void processLine(string line)
    {
        smatch matches;

        if(!regex_match(line, matches, regex("(.+):(.+)")))
            return;

        for(auto m : matches)
        {
            if(matches[1].str() == "name")
            {
                name = matches[2].str();
            }
            else if(matches[1].str() == "tagline")
            {
                tagline = matches[2].str();
            }
            else if(matches[1].str() == "main_color")
            {
                mainColor = matches[2].str();
            }
            else if(matches[1].str() == "secondary_color")
            {
                secondaryColor = matches[2].str();
            }
            else if(matches[1].str() == "links")
            {
                // TODO: process links
            }
        }
    }

    void generateHeader()
    {
        auto h1 = make_HTMLElement("h1");
        h1->appendChild(make_TextElement(name.c_str()));

        auto em = make_HTMLElement("em");
        em->appendChild(make_TextElement(tagline.c_str()));

        auto div_header_links = make_HTMLElement("div");
        div_header_links->setAttribute("class","header-links");
        for(const auto& link : links)
        {
            auto a = make_HTMLElement("a");
            a->setAttribute("href", link.url.c_str());
            a->setAttribute("class", "header-link");
            a->appendChild(make_TextElement(link.text.c_str()));

            div_header_links->appendChild(a);
        }

        header->appendChild(h1);
        header->appendChild(em);
        header->appendChild(div_header_links);
    }
};

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cout << "usage: ./dundas-peak directory" << endl;
        return -1;
    }

    // 0. get directory

    string parent_directory = argv[1];
    // 1. read heading.md

    SiteInfo siteInfo;

    ifstream input("sample/source/header.md");

    if(!input.is_open())
    {
        cout << "Error" << endl;
        return -2;
    }
    
    string line;
    while(getline(input,line))
    {
        siteInfo.processLine(line);
    }

    siteInfo.generateHeader();


    stringstream header;
    header << *siteInfo.header;

    // 2. read content/
    // 3. read template and insert data

    ifstream home_template("sample/source/template.html");
    stringstream s;
    s << home_template.rdbuf();

    string output = regex_replace(s.str(), regex("\\{\\{header\\}\\}"), header.str());

    cout << output;
    
    return 0;
}