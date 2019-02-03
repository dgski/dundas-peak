#include <regex>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "Site.h"
#include "Utils.h"

void Site::setParentPath(const char* path)
{
    parent_path = path;
}

void Site::copyMainDirectory()
{
    string from =  parent_path + "/source";
    string to = parent_path + "/public";

    filesystem::remove_all(to);
    filesystem::copy(from, to, std::filesystem::copy_options::recursive);
}

void Site::processHeaderLine(const string line)
{   
    const auto [key, value] = getLineKeyValuePair(line);

    if(key == "name")              name = value;
    if(key == "tagline")           tagline = value;
    if(key == "main_color")        mainColor = value;
    if(key == "secondary_color")   secondaryColor = value;
    if(key == "links")             processHeaderLinks(value);
}

void Site::processHeaderLinks(const string& linksString)
{
    smatch matches;
    
    for(string& l : splitString(linksString, ','))
    {
        if(regex_match(l, matches, linkRegex))
            links.push_back(HeadingLink{matches[1].str(),matches[2].str()});
    }
}

void Site::generateHeader()
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

void Site::readHeader(const char* path)
{
    ifstream input(path);

    if(!input.is_open())
    {
        cout << "Error" << endl;
        throw "Could Not Open Header file!";
    }
    
    string line;
    while(getline(input,line))
        processHeaderLine(line);
}