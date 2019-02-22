#include <string>
#include <vector>
#include <filesystem>

#include "markdown-to-html/MarkdownToHTML.h"
#include "Post.h"
#include "Project.h"

#define PREVIEWS_LIMIT    3

using namespace std;

struct HeadingLink
{
    string text;
    string url;
};

class Site
{
    filesystem::path parentPath;
    filesystem::path sourcePath;
    filesystem::path publicPath;
    filesystem::path themePath;
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
    string postPreviewTemplate;
    string postsTemplate;
    vector<Post> posts;

    string projectTemplate;
    string projectPreviewTemplate;
    string projectsTemplate;
    vector<Project> projects;
    
public:
    shared_ptr<HTMLElement> header;
    Site() { header = make_HTMLElement("div"); }
    Site(const char* path) : Site()
    {
        setPath(path);
    }

    void setPath(const char* path);
    void createCssFile();

    void processHeaderLine(const string& line);
    void processHeaderLinks(const string& linksString);

    void readHeader();
    void readPosts();
    void readProjects();

    void generateHeader();
    string generateHomePage();
    string generatePostsPage();
    string generateProjectsPage();

    void generate();
};