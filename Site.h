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

struct Post
{
    string title;
    string filename;
    string date;
    string description;
    string tagline;
    MarkdownToHTML content;
    
    Post() : content(false) {}
    void readContents(const char* filename)
    {
        ifstream page(filename);        
        if(!page.is_open()) throw "Error!";

        string line;
        while(getline(page, line))
            content.processLine(line);
    }
};

class Site
{
    string parent_path;
    string name;
    string tagline;
    string mainColor;
    string secondaryColor;

    string homeTemplate;
    vector<HeadingLink> links;

    string postTemplate;
    vector<Post> posts;




    // Projects
    vector<string> projects;
    
public:
    shared_ptr<HTMLElement> header;
    Site() { header = make_HTMLElement("div"); }

    void setParentPath(const char* path);
    void copyMainDirectory();

    void processHeaderLine(const string line);
    void processHeaderLinks(const string& linksString);
    void generateHeader();

    void readHeader(const char* path);
    void readPosts(const char* path);

    void generate();
};