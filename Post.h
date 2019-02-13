#include <string>
#include <vector>
#include <filesystem>

#include "html-element/HTMLElement.h"
#include "markdown-to-html/MarkdownToHTML.h"

using namespace std;

struct Post
{
    string filename;

    string title;
    string date;
    string tagline;
    vector<string> tags;
    MarkdownToHTML content;
    
    Post() : content(false) {}
    void processMetadataLine(const string& line);
    void readContents(const filesystem::path& filePath);
    void generate(const string& postTemplate, const filesystem::path& publicPath) const;
    string make_preview(const string& postPreviewTemplate, const string& topAddress) const;
};

string make_allPostPreviewsLink(const string& postsPath);


#pragma once