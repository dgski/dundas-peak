#include <regex>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <stdexcept>

#include "Site.h"
#include "Utils.h"

void Site::setPath(const char* path)
{
    parentPath = path;
    contentPath = parentPath / "content";
    publicPath = parentPath / "public";
    themePath = parentPath / "theme";
    postsPath = contentPath / "posts";
    projectsPath = contentPath / "projects";
}

void Site::createCssFile()
{
    string css = fileToString(themePath / "style.css", defCSSTemplate);
    css = regex_replace(css, regex("\\{\\{main_color\\}\\}"), mainColor);
    css = regex_replace(css, regex("\\{\\{secondary_color\\}\\}"), secondaryColor);

    stringToFile(publicPath / "style.css", css);
}

void Site::processHeaderLine(const string& line)
{   
    const auto [key, value] = getLineKeyValuePair(line);

    if(key == "name")                   name = value;
    else if(key == "top_address")       topAddress = value;
    else if(key == "tagline")           tagline = value;
    else if(key == "main_color")        mainColor = value;
    else if(key == "secondary_color")   secondaryColor = value;
    else if(key == "links")             processHeaderLinks(value);
}

void Site::processHeaderLinks(const string& linksString)
{
    smatch matches;
    
    for(string& l : splitString(linksString, ','))
    {
        if(regex_match(l, matches, regex("\\[(.+)\\]\\((.+)\\)")))
            links.push_back(HeadingLink{matches[1].str(),matches[2].str()});
    }
}

void Site::generateHeader()
{
    auto h1 = make_HTMLElement("h1");
    h1
    ->setAttribute("class","homepage-header")
    ->appendChild(make_TextElement(name.c_str()));

    auto em = make_HTMLElement("em");
    em
    ->setAttribute("class", "tagline")
    ->appendChild(make_TextElement(tagline.c_str()));

    auto div_header_links = make_HTMLElement("div");
    div_header_links->setAttribute("class","header-links");


    for(const auto& link : links)
    {
        auto a = make_HTMLElement("a");
        a->setAttribute("href", link.url.c_str());
        a->setAttribute("class", "header-link");
        a->appendChild(make_TextElement(link.text.c_str()));

        div_header_links->appendChild(a);
    }

    header->appendChild(h1);
    header->appendChild(em);
    header->appendChild(div_header_links);
}

void Site::readHeader()
{
    ifstream input = openFile<ifstream>(contentPath / "header.md");
    string line;
    while(getline(input,line))
        processHeaderLine(line);
}

void Site::readPosts()
{
    postTemplate = fileToString(themePath / "post.html", defPostTemplate);
    postPreviewTemplate = fileToString(themePath / "post_preview.html", defPostPreviewTemplate);

    for(auto& postFileName : filesystem::directory_iterator(postsPath))
    {
        if(postFileName.path().extension() != ".md")
            continue;

        Post& currentPost = posts.emplace_back();
        currentPost.filename = postFileName.path().stem().c_str();
        currentPost.readContents(postFileName.path());
    }

    sort(posts.begin(), posts.end(), [](const Post& a, const Post& b)
    {
        return a.datetime > b.datetime;
    });
}

void Site::readProjects()
{
    projectTemplate = fileToString(themePath / "project.html", defProjectsTemplate);
    projectPreviewTemplate = fileToString(themePath / "project_preview.html", defProjectPreviewTemplate);

    for(auto& projectFileName : filesystem::directory_iterator(projectsPath))
    {
        if(projectFileName.path().extension() != ".md")
            continue;

        Project& currentProject = projects.emplace_back();
        currentProject.filename = projectFileName.path().stem().c_str();
        currentProject.readContents(projectFileName.path());
    }

    sort(projects.begin(), projects.end(), [](const Project& a, const Project& b)
    {   
        return a.datetime > b.datetime;
    });
}

string Site::generateHomePage()
{   
    homeTemplate = fileToString(themePath / "home.html", defHomeTemplate);

    string output;

    //  Generate Header and add to output
    generateHeader();
    output = regex_replace(homeTemplate, regex("\\{\\{header\\}\\}"), header);

    // Add title to home page
    output = regex_replace(output, regex("\\{\\{name\\}\\}"), name);

    // Generate post previews and add to output
    string postPreviews;
    for(int i = 0; i < min(int(posts.size()), PREVIEWS_LIMIT); i++)
        postPreviews.append(posts.at(i).make_preview(postPreviewTemplate, topAddress + "/posts"));

    if(posts.size() > PREVIEWS_LIMIT)
        postPreviews += make_allPostPreviewsLink(topAddress + "/posts");

    output = regex_replace(output, regex("\\{\\{posts\\}\\}"), postPreviews);

    // Generate project previews and add to output
    string projectPreviews;
    for(int i = 0; i < min(int(projects.size()), PREVIEWS_LIMIT); i++)
        projectPreviews.append(projects.at(i).make_preview(projectPreviewTemplate, topAddress + "/projects"));

    if(projects.size() > PREVIEWS_LIMIT)
        projectPreviews += make_allProjectPreviewsLink(topAddress + "/projects");

    output = regex_replace(output, regex("\\{\\{projects\\}\\}"), projectPreviews);

    return output;
}

string Site::generatePostsPage()
{
    postsTemplate = fileToString(themePath / "posts.html", defPostsTemplate);

    string postPreviews;
    for(const Post& p : posts)
        postPreviews.append(p.make_preview(postPreviewTemplate, topAddress + "/posts"));

    string output = regex_replace(postsTemplate, regex("\\{\\{posts\\}\\}"), postPreviews);
    output = regex_replace(output, regex("\\{\\{name\\}\\}"), name);

    return output;
}

string Site::generatePostsRSS()
{
    auto rss = make_HTMLElement("rss");
    rss->setAttribute("version","2.0");

    auto channel = make_HTMLElement("channel");
    rss->appendChild(channel);
    
    auto title = make_HTMLElement("title");
    title->appendChild(make_TextElement(name.c_str()));
    channel->appendChild(title);

    for(const Post& p : posts)
    {
        auto item = make_HTMLElement("item");

        auto title = make_HTMLElement("title");
        title->appendChild(make_TextElement(p.title.c_str()));

        auto link = make_HTMLElement("link");
        link->appendChild(make_TextElement((publicPath / "posts" / p.filename / "index.html").c_str()));

        auto description = make_HTMLElement("description");
        description->appendChild(make_TextElement(p.tagline.c_str()));

        item->appendChild(title);
        item->appendChild(link);
        item->appendChild(description);

        channel->appendChild(item);
    }

    stringstream output;
    output << "<?xml version='1.0' encoding='UTF-8' ?>" << *rss;

    return output.str();
}

string Site::generateProjectsPage()
{
    projectsTemplate = fileToString(themePath / "projects.html", defProjectsTemplate);

    string projectsPreviews;
    for(const Project& p : projects)
        projectsPreviews.append(p.make_preview(projectPreviewTemplate, topAddress));

    string output = regex_replace(projectsTemplate, regex("\\{\\{projects\\}\\}"), projectsPreviews);
    output = regex_replace(output, regex("\\{\\{name\\}\\}"), name);

    return output;
}

void Site::generate()
{
    if(!filesystem::exists(publicPath))
        filesystem::create_directory(publicPath);

    if(filesystem::exists(contentPath / "files"))
    {
        if(filesystem::exists(publicPath / "files"))
            filesystem::remove_all(publicPath / "files");
        filesystem::copy(contentPath / "files", publicPath / "files");
        
    }

    if(filesystem::exists(publicPath / "posts"))
        filesystem::remove_all(publicPath / "posts");
    filesystem::create_directory(publicPath / "posts");
    
    if(filesystem::exists(publicPath / "projects"))
        filesystem::remove_all(publicPath / "projects");
    filesystem::create_directory(publicPath / "projects");
    
    if(filesystem::exists(publicPath / "index.html"))
        filesystem::remove(publicPath / "index.html");

    if(filesystem::exists(publicPath / "style.css"))
        filesystem::remove(publicPath / "style.css");

    createCssFile();

    stringToFile(publicPath / "index.html", generateHomePage());
    stringToFile(publicPath / "posts" / "index.html", generatePostsPage());
    stringToFile(publicPath / "projects" / "index.html", generateProjectsPage());

    if(filesystem::exists(publicPath / "posts" / "rss"))
        filesystem::remove(publicPath / "posts" / "rss");
    filesystem::create_directory(publicPath / "posts" / "rss");
    stringToFile(publicPath / "posts" / "rss" / "index.xml", generatePostsRSS());

    for(const auto& p : posts)
        p.generate(postTemplate, name, publicPath / "posts");
}

void createThemeFolder(const char* stringPath)
{
    filesystem::path sitePath(stringPath); 

    if(filesystem::exists(sitePath / "theme"))
    {
        cout << "Error: Theme Folder Already Exists." << endl;
        return;
    }

    filesystem::create_directory(sitePath / "theme");
    stringToFile(sitePath / "theme" / "home.html", defHomeTemplate);
    stringToFile(sitePath / "theme" / "posts.html", defPostsTemplate);
    stringToFile(sitePath / "theme" / "post.html", defPostTemplate);
    stringToFile(sitePath / "theme" / "post_preview.html", defPostPreviewTemplate);
    stringToFile(sitePath / "theme" / "projects.html", defProjectsTemplate);
    stringToFile(sitePath / "theme" / "project_preview.html", defProjectPreviewTemplate);
    stringToFile(sitePath / "theme" / "style.css", defCSSTemplate);
}

void createContentStructure(const char* stringPath)
{
    filesystem::path sitePath(stringPath); 

    if(!filesystem::exists(sitePath / "content"))
        filesystem::create_directory(sitePath / "content");

    if(!filesystem::exists(sitePath / "content" / "files"))
        filesystem::create_directory(sitePath / "content" / "files");

    if(!filesystem::exists(sitePath / "content" / "posts"))
        filesystem::create_directory(sitePath / "content" / "posts");

    if(!filesystem::exists(sitePath / "content" / "posts" / "first-post.md"))
        stringToFile(sitePath / "content" / "posts" / "first-post.md", string(samplePost));

    if(!filesystem::exists(sitePath / "content" / "projects"))
        filesystem::create_directory(sitePath / "content" / "projects");

    if(!filesystem::exists(sitePath / "content" / "projects" / "first-project.md"))
        stringToFile(sitePath / "content" / "projects" / "first-project.md", string(sampleProject));

    if(!filesystem::exists(sitePath / "content" / "header.md"))
        stringToFile(sitePath / "content" / "header.md", string(defHeaderMd));
}
