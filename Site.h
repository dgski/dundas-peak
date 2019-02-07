#include <string>
#include <vector>
#include <filesystem>

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
    string filename;

    string title;
    string date;
    string tagline;
    vector<string> tags;
    MarkdownToHTML content;
    
    Post() : content(false) {}
    void processMetadataLine(const string& line);
    void readContents(const char* filename);
    void generate(const string& postTemplate, const filesystem::path& publicPath) const;
};

class Site
{
    filesystem::path parentPath;
    filesystem::path sourcePath;
    filesystem::path publicPath;
    filesystem::path contentPath;
    filesystem::path postsPath;
    filesystem::path projectsPath;

    string name;
    string tagline;
    string mainColor;
    string secondaryColor;
    string topAddress;

    string homeTemplate;
    vector<HeadingLink> links;

    string postTemplate;
    vector<Post> posts;

    string projectTemplate;
    vector<string> projects;
    
public:
    shared_ptr<HTMLElement> header;
    Site() { header = make_HTMLElement("div"); }

    void setParentPath(const char* path);
    void copyMainDirectory();

    void processHeaderLine(const string& line);
    void processHeaderLinks(const string& linksString);

    void readHeader();
    void readPosts();

    void generateHeader();
    string generateHomePage();
    void generatePost(const Post& p);

    void generate();
};