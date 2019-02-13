#include <fstream>

#include "Project.h"
#include "Utils.h"

void Project::processMetadataLine(const string& line)
{
    const auto [key, value] = getLineKeyValuePair(line);

    if(key == "title")                  title = value;
    else if(key == "date")              date = value;
    else if(key == "tagline")           tagline = value;
    else if(key == "link")              link = value;
    else if(key == "technologies")      technologies = splitString(value, ',');
}

void Project::readContents(const filesystem::path& filePath)
{
    ifstream page(filePath);        
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

void Project::generate(const string& projectTemplate, const filesystem::path& publicPath) const
{
    stringstream parseResults;
    parseResults << content;

    // Templating
    string output = regex_replace(projectTemplate, regex("\\{\\{title\\}\\}"), title);
    output = regex_replace(output, regex("\\{\\{tagline\\}\\}"), tagline);
    output = regex_replace(output, regex("\\{\\{date\\}\\}"), date);
    output = regex_replace(output, regex("\\{\\{link\\}\\}"), date);
    // TODO: MISSING LINKS
    output = regex_replace(output, regex("\\{\\{content\\}\\}"), parseResults.str());
    
    filesystem::create_directory(publicPath / filename);
    ofstream html_out(publicPath / filename / "index.html");

    if(!html_out.is_open())
        throw "Could not open output file!";

    html_out << output;
}

shared_ptr<HTMLElement> Project::make_preview(const string& topAddress) const
{
    auto div = make_HTMLElement("div");
    div->setAttribute("class","projects-preview");

    auto a = make_HTMLElement("a");
    a
    ->setAttribute("href", (link).c_str())
    ->setAttribute("style","display: block");
    a->appendChild(make_TextElement(title.c_str()));
    div->appendChild(a);

    auto i = make_HTMLElement("i");
    i->appendChild(make_TextElement(tagline.c_str()));
    div->appendChild(i);

    return div;
}

string Project::make_preview(const string& projectPreviewTemplate, const string& topAddress) const
{
    string output = regex_replace(projectPreviewTemplate, regex("\\{\\{title\\}\\}"), title);
    output = regex_replace(output, regex("\\{\\{tagline\\}\\}"), tagline);
    output = regex_replace(output, regex("\\{\\{date\\}\\}"), date);
    output = regex_replace(output, regex("\\{\\{link\\}\\}"), link);

    return output;
}

string make_allProjectPreviewsLink(const string& postsPath)
{
    stringstream ss;
    
    auto a = make_HTMLElement("a");
    a
    ->setAttribute("href", postsPath.c_str())
    ->setAttribute("class", "all-previews-link");
    a->appendChild(make_TextElement("All Projects..."));

    ss << *a;

    return ss.str();
}