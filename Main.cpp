#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <regex>

#include "html-element/HTMLElement.h"
#include "markdown-to-html/MarkdownToHTML.h"

#include "Site.h"

using namespace std;
namespace fs = filesystem;



int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cout << "usage: ./dundas-peak directory" << endl;
        return -1;
    }


    Site site;

    // 0. get directory
    site.setParentPath(argv[1]);
    site.copyMainDirectory();

    // 1. read heading.md
    site.readHeader("sample/public/header.md");
    
    // 2. read content/
    vector<string> posts;
    //-----------
    {
        ifstream fff("sample/public/content/posts/template.html");
        stringstream post_html_template;
        post_html_template << fff.rdbuf();
        filesystem::remove("sample/public/content/posts/template.html");


        for(auto& page_name : fs::directory_iterator("sample/public/content/posts"))
        {
            posts.push_back(page_name.path());

            ifstream page(page_name.path());
            
            if(!page.is_open())
                throw "Error!";

            MarkdownToHTML parser;

            string line;
            while(getline(page, line))
                parser.processLine(line);

            filesystem::remove(page_name);


            stringstream parse_results;
            parse_results << parser;

            string output = regex_replace(post_html_template.str(), regex("\\{\\{content\\}\\}"), parse_results.str());


            string old_file = page_name.path().c_str();
            string new_file;

            for(char c : old_file)   
            {
                if(c == '.')
                    break;
                new_file.push_back(c);
            }
            new_file += ".html";

            ofstream html_out(new_file);

            if(!html_out.is_open())
                throw "Could not open output file!";

            html_out << output;
        }
    }



    //-----------




    // 3. read template and insert data
    
    ifstream home_template("sample/public/template.html");
    stringstream s;
    s << home_template.rdbuf();

    filesystem::remove("sample/public/template.html");


    site.generateHeader();
    stringstream header;
    header << *site.header;
    string output = regex_replace(s.str(), regex("\\{\\{header\\}\\}"), header.str());

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
        a->appendChild(make_TextElement(p.c_str()));
        div->appendChild(a);
    }
    
    stringstream content;
    content << *div;

    output = regex_replace(output, regex("\\{\\{content\\}\\}"), content.str());


    ofstream home_output("sample/public/index.html");
    home_output << output;
    
    
    return 0;
}