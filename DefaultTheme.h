constexpr const char* defHomeTemplate = R"(
    <!doctype html>
        <html>
            <head>
                <title>{{name}}</title>
                <link rel="stylesheet" type="text/css" href="style.css">
            </head>
            <body>
                <div class="header">
                    {{header}}
                </div>
                <div class="content">
                    <div class="col projects">
                        <h2>Projects</h2>
                        {{projects}}
                    </div>
                    <div class="col-spacer"></div>
                    <div class="col posts">
                        <h2>Posts</h2>
                        {{posts}}
                    </div>
                </div>
            </body>
        </html>
)";

constexpr const char* defPostsTemplate = R"(
    <!doctype html>
        <html>
            <head>
                <title>Posts By {{name}}</title>
                <link rel="stylesheet" type="text/css" href="/style.css">
                <link rel="alternate" type="application/rss+xml" title="Posts RSS Feed" href="posts/rss/">
            </head>
            <body>
                <div class="content">
                    <div class="col posts">
                        <h2>All Posts By <a href="/">{{name}}</a></h2>
                        <a href="rss/index.xml">RSS</a>
                        <br>
                        <br>
                        {{posts}}
                    </div>
                </div>
            </body>
        </html>
)";

constexpr const char*  defPostTemplate = R"(
    <!doctype html>
    <html>
        <head>
            <title>{{title}}</title>
            <link rel="stylesheet" type="text/css" href="/style.css">
        </head>
        <body>
            
            <div class="post-content">
                <h1 class="post-title">{{title}}</h1>
                <em>{{tagline}}</em>
                <div class="post-body">
                    {{content}}
                </div>
                <strong>Post By <a href="/">{{name}}</a> on {{date}}</strong>
            </div>
        </body>
    </html>
)";

constexpr const char* defPostPreviewTemplate = R"(
    <div class="post-preview">
        <b><a href="{{link}}">{{title}}</a></b>
        <br>
        {{tagline}}
        <br>
        <i>{{date}}</i>
    </div>
)";

constexpr const char* defProjectsTemplate = R"(
    <!doctype html>
    <html>
        <head>
            <title>Projects By {{name}}</title>
            <link rel="stylesheet" type="text/css" href="/style.css">
        </head>
        <body>
            <div class="content">
                <div class="col projects">
                    <h2>All Projects By <a href="/">{{name}}</a></h2>
                    <br>
                    {{projects}}
                </div>
            </div>
        </body>
    </html>
)";

constexpr const char* defProjectPreviewTemplate = R"(
    <div class="project-preview">
        <b><a href="{{link}}">{{title}}</a></b>
        <br>
        {{tagline}}
        <div class="technologies">{{technologies}}</div>
    </div>
)";

constexpr const char* defCSSTemplate = R"(
    body {
        margin: 0;
        font-family: Arial, Helvetica, sans-serif;
        line-height: 1.5em;
        --main-color: {{main_color}};
        --hilight-color: {{secondary_color}};
    }

    h1.homepage-header {
        font-size: 3em;
    }

    h4 {
        margin-bottom: 0px;
        text-decoration: underline;
    }

    a {
        color: var(--hilight-color);
    }

    .header {
        background-color: var(--main-color);
        color: white;
        display: inline-block;
        width: 100%;
        text-align: center;
        padding-top: 10px;
        padding-bottom: 10px;
    }

    .content {
        max-width: 700px;
        margin-left: calc(50% - 350px);
        margin-top: 30px;
        display: flex;
    }

    .col {
        flex: 1;
    }

    .col-spacer {
        width: 50px;
    }

    .header-links {
        padding: 30px;
    }

    .header-link {
        color: white;
        border: 2px solid white;
        padding: 5px;
        margin-left: 5px;
        margin-right: 5px;
        border-radius: 5px;
        text-decoration: none;
    }

    .header-link:hover {
        background-color: white;
        color: var(--main-color);
    }

    .tech {
        background-color: var(--main-color);
        color: white;
        font-size: 0.8em;
        padding: 3px;
        margin-right: 5px;
        border-radius: 5px;
    }

    .project {
        margin-bottom: 30px;
    }

    li {
    }

    table{
        border-collapse: collapse;
        width: 100%;
        margin-top: 40px;
        margin-bottom: 40px;
    }
    td 
    {
        border-bottom: 1px solid lightgrey;
        padding-bottom: 10px;
        padding-top: 10px;
    }

    .section {
        margin-bottom: 50px;
    }

    .posts {
        display: block;
    }

    .post-preview {
        margin-top: 15px;
        margin-bottom: 15px;
    }

    .post-header {
        background-color: black;
        color: white;
        text-align: center;
        padding-top: 20px;
        padding-bottom: 20px;
    }

    .post-content {
        max-width: 700px;
        margin-left: calc(50% - 350px);
        margin-bottom: 30px;
        margin-top: 30px;
        font-size: 18px;
        line-height: 2em;
    }

    .post-title {
        font-size: 36px;
        margin-bottom: 5px;
    }

    .post-body {
        margin-top: 60px;
    }

    .project-preview {
        margin-top: 15px;
        margin-bottom: 15px;
    }

    .code {
        background-color: black;
        color: white;
        font-family: monospace;
        padding: 10px;
        font-size: 14px;
        line-height: 1.2em;
        overflow-x: scroll;
        margin-top: 10px;
        margin-bottom: 40px;
    }

    .all-previews-link {
        display: inline-block;
    }

    .technology
    {
        color: black;
        border: 1px solid black;
        padding: 2px;
        border-radius: 3px;
        margin-right: 5px;
        font-size: 11px;
    }

    @media (max-width:768px) {
        body {
        }

        .content {
            margin: 0px;
            max-width: none;
            width: calc(100% - 20px);
            padding-left: 10px;
            padding-right: 10px;
            display: block;
        }

        .post-content {
            margin: 0px;
            max-width: none;
            width: calc(100% - 20px);
            padding-left: 10px;
            padding-right: 10px;
        }

        .post-title {
            margin-top: 20px;
        }

        .posts {
            margin-bottom: 30px;
            margin-top: 30px;
        }
    }


    @media (max-device-width:768px) and (orientation: portrait) {

        body {
            line-height: 4em;
            font-size: 16px;
        }

        .content {
            margin: 0px;
            max-width: none;
            width: calc(100% - 2em);
            padding-left: 1em;
            padding-right: 1em;
            font-size: 2.5em;
            display: block;
        }

        .header {
            font-size: 4em;
        }

        h1.homepage-header {
            font-size: 2em;
            margin: 0px;
            padding-top: 50px;
            padding-bottom: 50px;
        }

        .tagline {
            font-size: 0.7em;
            padding-left: 5px;
            padding-right: 5px;
        }

        .header-links {
            font-size: 0.7em;
            padding-top: 50px;
            padding-bottom: 50px;
        }

        .header-link {
            margin-left: 20px;
            margin-right: 20px;
            padding: 10px;
            border-width: 4px;
            border-radius: 10px;
        }
        
        .post-content {
            margin: 0px;
            max-width: none;
            width: calc(100% - 2em);
            padding-left: 1em;
            padding-right: 1em;
            font-size: 2.8em;
        }
        
        .post-title {
            font-size: 1.5em;
            margin-top: 1em;
        }
        
        .post-body {
            margin-top: 1.5em;
        }

        .post-preview {
            margin-top: 50px;
            margin-bottom: 50px;
        }

        .project-preview {
            margin-top: 50px;
            margin-bottom: 50px;
        }

        .posts {
            margin-top: 80px;
        }

        .technology
        {
            padding: 5px;
            border-radius: 10px;
            margin-right: 20px;
            font-size: 0.9em;
            border-width: 2px;
        }
    }
)";

constexpr const char*  defHeaderMd =
R"(---
name: Your Name
tagline: A Sentance or Two About You!
links: [Resume](#), [Email](#), [Github](https://github.com), [LinkedIn](http://linkedin.com)
main_color: darkslateblue
secondary_color: darkslateblue
top_address: http://127.0.0.1:8000
---
)";

constexpr const char* samplePost =
R"(---
title: Your First Post
tagline: Learning How to Use Dundas Peak Is Easy
date: 2018/10/16 13:34:00
---

This is where the Post's Content will go!)";

constexpr const char* sampleProject =
R"(---
title: Dundas Peak
date: 2018-02-23 00:00:00
tagline: A No-Nonsense, Low Configuration Personal Site Generator for Developers.
link: #
technologies: C++, STL, HTML
---
)";