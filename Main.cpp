#include <iostream>
#include <chrono>
#include <stdexcept>
#include <string.h>

#include "Site.h"

using namespace std;

constexpr const char* USAGE_STRING = "usage: ./dundas-peak [-g | -c | -t] <directory>";

int main(int argc, char** argv)
{
    if(argc < 3)
    {
        cout << USAGE_STRING << endl;
        return -1;
    }

    if(!strcmp(argv[1], "-t"))
    {
        createThemeFolder(argv[2]);
        cout << "Dundas Peak : Created Theme Folder." << endl;      
        return 0;
    }

    if(!strcmp(argv[1], "-c"))
    {
        createContentStructure(argv[2]);
        cout << "Dundas Peak : Created Content Structure." << endl;      
        return 0;
    }

    if(strcmp(argv[1], "-g"))
    {
        cout << USAGE_STRING << endl;
        return -1;
    }

    Site site(argv[2]);
    auto start = chrono::system_clock::now();

    try
    {
        site.readHeader();
        site.readPosts();
        site.readProjects();
        site.generate();

        auto end = chrono::system_clock::now();
        auto dur = (end - start).count() /  1000000000.0;
        cout << "Dundas Peak Site Generation Took: " << dur << "s" << endl;
    }
    catch(const exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}