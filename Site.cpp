#include <regex>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "Site.h"
#include "Utils.h"

void Site::setParentPath(const char* path)
{
    parentPath = path;
    sourcePath = parentPath / "source";
    publicPath = parentPath / "public";
    contentPath = sourcePath / "content";
    postsPath = contentPath / "posts";
    projectsPath = contentPath / "projects";
}

void Site::copyMainDirectory()
{
    auto from =  parentPath / "source";
    auto to = parentPath / "public";

    filesystem::remove_all(to);
    filesystem::copy(from, to, std::filesystem::copy_options::recursive);
}

void Site::processHeaderLine(const string& line)
{   
    const auto [key, value] = getLineKeyValuePair(line);

    if(key == "name")              name = value;
    if(key == "top_address")       topAddress = value;
    if(key == "tagline")           tagline = value;
    if(key == "main_color")        mainColor = value;
    if(key == "secondary_color")   secondaryColor = value;
    if(key == "links")             processHeaderLinks(value);
}

void Site::processHeaderLinks(const string& linksString)
{
    smatch matches;
    
    for(string& l : splitString(linksString, ','))
    {
        if(regex_match(l, matches, linkRegex))
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
    em->appendChild(make_TextElement(tagline.c_str()));

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
    ifstream input(sourcePath / "header.md");

    if(!input.is_open())
    {
        cout << "Error" << endl;
        throw "Could Not Open Header file!";
    }
    
    string line;
    while(getline(input,line))
        processHeaderLine(line);
}

void Site::readAbout()
{
    auto template_path = contentPath / "about.md";

    // read template
    ifstream fff(template_path);
    stringstream aboutHtmlTemplate;
    aboutHtmlTemplate << fff.rdbuf();
    aboutTemplate = aboutHtmlTemplate.str();
}

void Site::readPosts()
{
    postTemplate = fileToString(postsPath / "page_template.html");

    // read each post
    for(auto& postFileName : filesystem::directory_iterator(postsPath))
    {
        if(postFileName.path().extension() != ".md")
            continue;

        Post& currentPost = posts.emplace_back();

        string fileName = postFileName.path().c_str();

        // Metadata
        currentPost.title = fileName;
        currentPost.filename = postFileName.path().filename().c_str();
        currentPost.filename = currentPost.filename.substr(0, currentPost.filename.length() - 3);

        // Parse markdown file
        currentPost.readContents(fileName.c_str());
    }
}

void Site::readProjects()
{
    // read template
    projectTemplate = fileToString(projectsPath / "page_template.html");

    // read each post
    for(auto& projectFileName : filesystem::directory_iterator(projectsPath))
    {
        if(projectFileName.path().extension() != ".md")
            continue;

        Project& currentProject = projects.emplace_back();

        string fileName = projectFileName.path().c_str();

        // Metadata
        currentProject.title = fileName;
        currentProject.filename = projectFileName.path().filename().c_str();
        currentProject.filename = currentProject.filename.substr(0, currentProject.filename.length() - 3);

        // Parse markdown file
        currentProject.readContents(fileName.c_str());
    }
}

string Site::generateHomePage()
{   
    homeTemplate = fileToString(sourcePath / "page_template.html");

    string output;

    //  Generate Header and add to output
    {
        generateHeader();
        output = regex_replace(homeTemplate, regex("\\{\\{header\\}\\}"), header);
    }

    // Generate post previews and add to output
    {
        auto divPosts = make_HTMLElement("div");
        for(auto p : posts)
            divPosts->appendChild(p.make_preview(topAddress));

        output = regex_replace(output, regex("\\{\\{posts\\}\\}"), divPosts);
    }

    // Generate project previews and add to output
    {
        auto divProjects = make_HTMLElement("div");
        for(auto p : projects)
            divProjects->appendChild(p.make_preview(topAddress));

        output = regex_replace(output, regex("\\{\\{projects\\}\\}"), divProjects);
    }

    return output;
}

void Site::generate()
{
    filesystem::remove_all(publicPath);

    filesystem::create_directory(publicPath);
    filesystem::create_directory(contentPath);
    filesystem::create_directory(postsPath);
    filesystem::create_directory(projectsPath);

    filesystem::copy_file(sourcePath / "style.css", publicPath / "style.css");

    ofstream homePageFile(publicPath / "index.html");
    if(!homePageFile.is_open())
        throw "Could Not Open index.html to write!";

    // Generate Home Page
    homePageFile << generateHomePage();

    // GeneratePosts
    for(const auto p : posts)
        p.generate(postTemplate, publicPath);
}