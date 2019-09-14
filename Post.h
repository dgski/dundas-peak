#include <string>
#include <vector>
#include <filesystem>

#include "markdown-to-html/MarkdownToHTML.h"
#include "simple-time/SimpleTime.h"

using namespace std;

struct Post
{
    string filename;

    string title;
    SimpleTime datetime;
    string tagline;
    vector<string> tags;
    MarkdownToHTML content;
    
    Post() : content(false) {}
    void processMetadataLine(const string& line);
    void readContents(const filesystem::path& filePath);
    void generate(const string& postTemplate, const string& authorName, const filesystem::path& publicPath) const;
    string make_preview(const string& postPreviewTemplate, const string& topAddress) const;
    shared_ptr<HTMLElement> make_rssItem(const string& topAddress) const;
};

string make_allPostPreviewsLink(const string& postsPath);


#pragma once