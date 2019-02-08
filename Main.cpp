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

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cout << "usage: ./dundas-peak directory" << endl;
        return -1;
    }

    // The Main Boi
    Site site;

    // 0. get directory
    site.setParentPath(argv[1]);
    //site.copyMainDirectory();

    // 1. read heading.md
    site.readHeader();
    
    // 2. read posts
    site.readPosts();

    // 3. read projects
    site.readProjects();

    // 4. read about
    //site.readAbout(); TODO

    // 5. read template and insert data
    site.generate();
    
    return 0;
}