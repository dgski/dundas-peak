#include <fstream>
#include <stdexcept>

#include "Project.h"
#include "Utils.h"

void Project::processMetadataLine(const string& line)
{
    const auto [key, value] = getLineKeyValuePair(line);

    if(key == "title")                  title = value;
    else if(key == "date")              datetime.set(value, DEFAULT_DATETIME_FORMAT);
    else if(key == "tagline")           tagline = value;
    else if(key == "link")              link = value;
    else if(key == "technologies")      technologies = splitString(value, ',');
}

void Project::readContents(const filesystem::path& filePath)
{
    ifstream page = openFile<ifstream>(filePath);        

    string line;

    // Metadata
    if(!getline(page,line) || line != "---")
        throw runtime_error("Post is missing Metadata");

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
    output = regex_replace(output, regex("\\{\\{date\\}\\}"), datetime.toString(DEFAULT_DATETIME_FORMAT));
    output = regex_replace(output, regex("\\{\\{link\\}\\}"), link);
    // TODO: MISSING LINKS
    output = regex_replace(output, regex("\\{\\{content\\}\\}"), parseResults.str());
    
    filesystem::create_directory(publicPath / filename);
    stringToFile(publicPath / filename / "index.html", output);
}

string Project::generateTechnologiesPreviews() const
{
    string output;

    for(const string& t: technologies)
    {
        auto span = make_HTMLElement("span");
        span
        ->setAttribute("class","technology")
        ->appendChild(make_TextElement(t.c_str()));
        stringstream ss;
        ss << *span;
        output.append(ss.str());
    }

    return output;
}

string Project::make_preview(const string& projectPreviewTemplate, const string& topAddress) const
{
    string output = regex_replace(projectPreviewTemplate, regex("\\{\\{title\\}\\}"), title);
    output = regex_replace(output, regex("\\{\\{tagline\\}\\}"), tagline);
    output = regex_replace(output, regex("\\{\\{technologies\\}\\}"), generateTechnologiesPreviews());
    output = regex_replace(output, regex("\\{\\{date\\}\\}"), datetime.toString(DEFAULT_DATETIME_FORMAT));
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