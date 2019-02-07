#include <regex>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "Site.h"
#include "Utils.h"

void Post::processMetadataLine(const string& line)
{
    const auto [key, value] = getLineKeyValuePair(line);

    if(key == "title")             title = value;
    if(key == "tagline")           tagline = value;
    if(key == "date")              date = value;
    if(key == "tags")              tags = splitString(value, ',');
}

void Post::readContents(const char* filename)
{
    ifstream page(filename);        
    if(!page.is_open()) throw "Error!";

    string line;

    // Metadata
    if(!getline(page,line) || line != "---")
        throw "Post is missing Metadata";

    while(getline(page, line))
    {
        if(line == "---") break;
        processMetadataLine(line);
    }

    // Content
    while(getline(page, line))
        content.processLine(line);
}

void Post::generate(const string& postTemplate, const filesystem::path& publicPath) const
{
    stringstream parseResults;
    parseResults << content;

    // Templating
    string output = regex_replace(postTemplate, regex("\\{\\{title\\}\\}"), title);
    output = regex_replace(output, regex("\\{\\{tagline\\}\\}"), tagline);
    output = regex_replace(output, regex("\\{\\{date\\}\\}"), date);
    output = regex_replace(output, regex("\\{\\{content\\}\\}"), parseResults.str());
    
    filesystem::create_directory(publicPath / filename);
    ofstream html_out(publicPath / filename / "index.html");

    if(!html_out.is_open())
        throw "Could not open output file!";

    html_out << output;
}

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

void Site::readPosts()
{
    auto template_path = postsPath / "template.html";

    // read template
    ifstream fff(template_path);
    stringstream postHtmlTemplate;
    postHtmlTemplate << fff.rdbuf();
    postTemplate = postHtmlTemplate.str();

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

string Site::generateHomePage()
{
    ifstream homeTemplate(sourcePath / "template.html");

    stringstream s;
    s << homeTemplate.rdbuf();

    generateHeader();
    stringstream h;
    h << *header;
    string output = regex_replace(s.str(), regex("\\{\\{header\\}\\}"), h.str());

    auto div = make_HTMLElement("div");
    
    for(auto p : posts)
    {
        auto a = make_HTMLElement("a");
        a
        ->setAttribute("href", (topAddress + "/" + p.filename).c_str())
        ->setAttribute("style","display: block");
        a->appendChild(make_TextElement(p.title.c_str()));
        div->appendChild(a);

        auto i = make_HTMLElement("i");
        i->appendChild(make_TextElement(p.tagline.c_str()));
        div->appendChild(i);
    }

    stringstream postsContent;
    postsContent << *div;

    output = regex_replace(output, regex("\\{\\{posts\\}\\}"), postsContent.str());

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