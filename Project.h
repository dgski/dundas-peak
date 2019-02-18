#include <string>
#include <vector>
#include <filesystem>

#include "markdown-to-html/MarkdownToHTML.h"
#include "simple-time/SimpleTime.h"

using namespace std;

struct Project
{
    string filename;

    string title;
    string tagline;
    SimpleTime datetime;
    string link;
    vector<string> technologies;
    MarkdownToHTML content;

    Project() : content(false) {}
    void processMetadataLine(const string& line);
    void readContents(const filesystem::path& filePath);
    void generate(const string& projectTemplate, const filesystem::path& publicPath) const;
    string generateTechnologiesPreviews() const;
    string make_preview(const string& projectPreviewTemplate, const string& topAddress) const;
};

string make_allProjectPreviewsLink(const string& postsPath);

#pragma once