#include <string>
#include <vector>

#include "html-element/HTMLElement.h"
#include "markdown-to-html/MarkdownToHTML.h"

using namespace std;

struct HeadingLink
{
    string text;
    string url;
};

class Site
{
    string parent_path;
    string name;
    string tagline;
    vector<HeadingLink> links;
    string mainColor;
    string secondaryColor;

    
public:
    shared_ptr<HTMLElement> header;
    Site() { header = make_HTMLElement("div"); }

    void setParentPath(const char* path);
    void copyMainDirectory();


    void processHeaderLine(const string line);
    void processHeaderLinks(const string& linksString);
    void generateHeader();

    void readHeader(const char* path);
};