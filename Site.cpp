#include <regex>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "Site.h"
#include "Utils.h"

void Site::setPath(const char* path)
{
    parentPath = path;
    sourcePath = parentPath / "source";
    publicPath = parentPath / "public";
    contentPath = sourcePath / "content";
    postsPath = contentPath / "posts";
    projectsPath = contentPath / "projects";
}

void Site::createCssFile()
{
    if(!filesystem::exists(sourcePath / "style.css"))
        return;

    string cssInput = fileToString(sourcePath / "style.css");
    cssInput = regex_replace(cssInput, regex("\\{\\{main_color\\}\\}"), mainColor);
    cssInput = regex_replace(cssInput, regex("\\{\\{secondary_color\\}\\}"), secondaryColor);

    ofstream cssOutput(publicPath / "style.css");
    cssOutput << cssInput;
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
    postPreviewTemplate = fileToString(postsPath / "preview_template.html");

    for(auto& postFileName : filesystem::directory_iterator(postsPath))
    {
        if(postFileName.path().extension() != ".md")
            continue;

        Post& currentPost = posts.emplace_back();
        currentPost.filename = postFileName.path().stem().c_str();
        currentPost.readContents(postFileName.path());
    }
}

void Site::readProjects()
{
    projectTemplate = fileToString(projectsPath / "page_template.html");
    projectPreviewTemplate = fileToString(projectsPath / "preview_template.html");

    for(auto& projectFileName : filesystem::directory_iterator(projectsPath))
    {
        if(projectFileName.path().extension() != ".md")
            continue;

        Project& currentProject = projects.emplace_back();
        currentProject.filename = projectFileName.path().stem().c_str();
        currentProject.readContents(projectFileName.path());
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
        string postPreviews;
        for(auto p : posts)
            postPreviews.append(p.make_preview(postPreviewTemplate, topAddress));

        output = regex_replace(output, regex("\\{\\{posts\\}\\}"), postPreviews);
    }

    // Generate project previews and add to output
    {
        string projectPreviews;
        for(auto p : projects)
            projectPreviews.append(p.make_preview(projectPreviewTemplate, topAddress));

        output = regex_replace(output, regex("\\{\\{projects\\}\\}"), projectPreviews);
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

    createCssFile();

    ofstream homePageFile(publicPath / "index.html");
    if(!homePageFile.is_open())
        throw "Could Not Open index.html to write!";

    // Generate Home Page
    homePageFile << generateHomePage();

    // Generate Posts
    for(const auto p : posts)
        p.generate(postTemplate, publicPath);
}