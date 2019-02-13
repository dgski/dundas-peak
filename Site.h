#include <string>
#include <vector>
#include <filesystem>

#include "html-element/HTMLElement.h"
#include "markdown-to-html/MarkdownToHTML.h"
#include "Post.h"
#include "Project.h"

#define PREVIEWS_LIMIT    4

using namespace std;

struct HeadingLink
{
    string text;
    string url;
};

struct About
{
    MarkdownToHTML content;
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

    string aboutTemplate;
    About about;

    string postTemplate;
    string postPreviewTemplate;
    vector<Post> posts;

    string projectTemplate;
    string projectPreviewTemplate;
    vector<Project> projects;
    
public:
    shared_ptr<HTMLElement> header;
    Site() { header = make_HTMLElement("div"); }

    void setPath(const char* path);
    void createCssFile();

    void processHeaderLine(const string& line);
    void processHeaderLinks(const string& linksString);

    void readHeader();
    void readAbout();
    void readPosts();
    void readProjects();

    void generateHeader();
    string generateHomePage();
    void generatePost(const Post& p);

    void generate();
};