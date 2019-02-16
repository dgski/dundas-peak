#include <iostream>
#include <fstream>
#include <chrono>

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
    site.setPath(argv[1]);
    //site.copyMainDirectory();

    auto start = chrono::system_clock::now();

    site.readHeader();
    site.readPosts();
    site.readProjects();
    //site.readAbout(); TODO
    site.generate();

    auto end = chrono::system_clock::now();
    auto dur = (end - start).count() /  1000000000.0;

    cout << "Dundas-Peak Site Generation Took: " << dur << "s" << endl;
    
    return 0;
}