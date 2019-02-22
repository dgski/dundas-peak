#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include "Post.h"
#include "Utils.h"

void Post::processMetadataLine(const string& line)
{
    const auto [key, value] = getLineKeyValuePair(line);

    if(key == "title")             title = value;
    else if(key == "tagline")      tagline = value;
    else if(key == "date")         datetime.set(value, DEFAULT_DATETIME_FORMAT);
    else if(key == "tags")         tags = splitString(value, ',');
}

void Post::readContents(const filesystem::path& filePath)
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

void Post::generate(const string& postTemplate, const string& authorName, const filesystem::path& publicPath) const
{
    stringstream parseResults;
    parseResults << content;

    // Templating
    string output = regex_replace(postTemplate, regex("\\{\\{title\\}\\}"), title);
    output = regex_replace(output, regex("\\{\\{tagline\\}\\}"), tagline);
    output = regex_replace(output, regex("\\{\\{name\\}\\}"), authorName);
    output = regex_replace(output, regex("\\{\\{date\\}\\}"), datetime.toString(DEFAULT_DATETIME_FORMAT));
    output = regex_replace(output, regex("\\{\\{content\\}\\}"), parseResults.str());
    
    filesystem::create_directory(publicPath / filename);
    stringToFile(publicPath / filename / "index.html", output);
}

string Post::make_preview(const string& postPreviewTemplate, const string& topAddress) const
{
    string output = regex_replace(postPreviewTemplate, regex("\\{\\{title\\}\\}"), title);
    output = regex_replace(output, regex("\\{\\{tagline\\}\\}"), tagline);
    output = regex_replace(output, regex("\\{\\{date\\}\\}"), datetime.toString(DEFAULT_DATE_FORMAT));
    output = regex_replace(output, regex("\\{\\{link\\}\\}"), (topAddress + "/" + filename));

    return output;
}

string make_allPostPreviewsLink(const string& postsPath)
{
    stringstream ss;
    
    auto a = make_HTMLElement("a");
    a
    ->setAttribute("href", postsPath.c_str())
    ->setAttribute("class", "all-previews-link");
    a->appendChild(make_TextElement("All Posts..."));

    ss << *a;

    return ss.str();
}
