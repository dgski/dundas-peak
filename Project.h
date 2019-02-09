#include <string>
#include <vector>
#include <filesystem>

#include "html-element/HTMLElement.h"
#include "markdown-to-html/MarkdownToHTML.h"

using namespace std;

struct Project
{
    string filename;

    string title;
    string tagline;
    string date;
    string link;
    vector<string> technologies;
    MarkdownToHTML content;

    Project() : content(false) {}
    void processMetadataLine(const string& line);
    void readContents(const filesystem::path& filePath);
    void generate(const string& projectTemplate, const filesystem::path& publicPath) const;
    shared_ptr<HTMLElement> make_preview(const string& topAddress) const;
    string make_preview(const string& projectPreviewTemplate, const string& topAddress) const;
};

#pragma once