#include <fstream>
#include <iomanip>
#include <sstream>

#include "Post.h"
#include "Utils.h"

void Post::processMetadataLine(const string& line)
{
    const auto [key, value] = getLineKeyValuePair(line);

    if(key == "title")             title = value;
    else if(key == "tagline")      tagline = value;
    else if(key == "date")
    {
        date = value;
        cout << "DATE:" << date << endl;
        tm t = {};

        stringstream ss {date};
        
        ss >> get_time(&t, "%Y-%b-%d %H:%M:%S");

        cout << t.tm_min << endl;
        cout << t.tm_hour << endl;
        cout << t.tm_year << endl;
        cout << t.tm_mon << endl;

    }
    else if(key == "tags")         tags = splitString(value, ',');
}

void Post::readContents(const filesystem::path& filePath)
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

shared_ptr<HTMLElement> Post::make_preview(const string& topAddress) const
{
    auto div = make_HTMLElement("div");
    div->setAttribute("class","post-preview");

    auto a = make_HTMLElement("a");
    a
    ->setAttribute("href", (topAddress + "/" + filename).c_str())
    ->setAttribute("style","display: block");
    a->appendChild(make_TextElement(title.c_str()));
    div->appendChild(a);

    auto i = make_HTMLElement("i");
    i->appendChild(make_TextElement(tagline.c_str()));
    div->appendChild(i);

    return div;
}

string Post::make_preview(const string& postPreviewTemplate, const string& topAddress) const
{
    string output = regex_replace(postPreviewTemplate, regex("\\{\\{title\\}\\}"), title);
    output = regex_replace(output, regex("\\{\\{tagline\\}\\}"), tagline);
    output = regex_replace(output, regex("\\{\\{date\\}\\}"), date);
    output = regex_replace(output, regex("\\{\\{link\\}\\}"), (topAddress + "/" + filename));

    return output;
}
