#include <iostream>
#include <chrono>
#include <stdexcept>

#include "html-element/HTMLElement.h"
#include "Site.h"

using namespace std;

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cout << "usage: ./dundas-peak directory" << endl;
        return -1;
    }

    Site site(argv[1]);
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