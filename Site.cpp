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

void Site::readPosts(const char* path)
{

    string template_path(path);
    template_path += "/template.html";

    // read and delete template
    ifstream fff(template_path);
    stringstream post_html_template;
    post_html_template << fff.rdbuf();
    filesystem::remove(template_path);
    postTemplate = post_html_template.str();

    // read each post
    for(auto& page_name : filesystem::directory_iterator(path))
    {
        string fileName = page_name.path().c_str();

        Post& currentPost = posts.emplace_back();

        // Metadata
        currentPost.title = fileName;
        currentPost.filename = page_name.path().filename().c_str();
        currentPost.filename = currentPost.filename.substr(0, currentPost.filename.length() - 3);

        // Parse markdown file
        currentPost.readContents(fileName.c_str());

        // Delete markdown file
        filesystem::remove(page_name);
    }
}

void Site::generate()
{
    ifstream home_template("sample/public/template.html");
    stringstream s;
    s << home_template.rdbuf();

    filesystem::remove("sample/public/template.html");


    generateHeader();
    stringstream h;
    h << *header;
    string output = regex_replace(s.str(), regex("\\{\\{header\\}\\}"), h.str());

    auto div = make_HTMLElement("div");
    div->setAttribute("class","posts");

    auto h2 = make_HTMLElement("h2");
    h2->appendChild(make_TextElement("Posts"));
    div->appendChild(h2);
    
    for(auto p : posts)
    {
        auto a = make_HTMLElement("a");
        a->setAttribute("href","#")
        ->setAttribute("style","display: block");
        a->appendChild(make_TextElement(p.filename.c_str()));
        div->appendChild(a);

        stringstream parseResults;
        parseResults << p.content;

        // Templating
        string output = regex_replace(postTemplate, regex("\\{\\{content\\}\\}"), parseResults.str());


        filesystem::create_directory(parent_path + "/" + p.filename);

        string outputPath = parent_path + "/" + p.filename + "/index.html";



        ofstream html_out(outputPath);

        if(!html_out.is_open())
            throw "Could not open output file!";

        html_out << output;

    }
    
    stringstream content;
    content << *div;

    output = regex_replace(output, regex("\\{\\{content\\}\\}"), content.str());


    ofstream home_output("sample/public/index.html");
    home_output << output;
}