# Dundas Peak Simple Static Site Generator
Dundas Peak is **No-Nonsense, Low Configuration Personal Site Generator** for Developers. It doesn't offer much flexibility because it  creates websites with only the basic features most developers need:
- Showcased Links (contact information, repos, resume, email, linkedIN etc...)
- A Portfolio for Projects
- A Simple Blog with RSS support

Because that's all you really need!

## Installation

Clone Repository and run compile script if running g++. Otherwise link .cpp files using C++17 compatible compiler.
```
$ ./compile
```
Compile Script Contents:
```
g++-8 -std=c++17 Main.cpp Site.cpp Post.cpp Project.cpp Utils.cpp markdown-to-html/html-element/HTMLElement.cpp markdown-to-html/MarkdownToHTML.cpp -o dundas-peak -g -lstdc++fs
```

## Quick Start

There are four simple steps to get up and running with Dundas Peak:
1. Create a folder for site: **$ mkdir site**
2. Generate content structure: **$ dundas-peak -c site/**
3. Add/Edit markdown files into the content folder
4. Generate public site: **$ dundas-peak -g site/**

And you should now have a **public** folder ready to serve!

## Site Structure

Dundas Peak starts with a very simple, almost flat directory structure. Here is a sample site directory with all the required files:

```
site/
    content/
        files/
        posts/
        projects/
        header.md
```

Run the application with the following two arguments specifying the location of your site directory. For example:

```
$ ./dundas-peak -g site
```

This will read your **contents directory** and generate a **public folder** beside it:
```
site/
    content/
    public/
```
This **public** folder will contain the generated html files and structure. Serve it as your site.

## Adding Content

### Files

You can place any files you reference in your posts or links here. These will copy over to the public folder and will eventually be reachable at **<yoursite.com>/files/<your files here>**.

### Posts

You can create posts by simply placing markdown files into **site/content/posts**. Add them with a proper metadata header and these posts will be added to your blog. The post markdown file format looks as follows ("---" is necessary):

```
---
title: {Post Title}
tagline: {A Short Description of Your Post}
date: {YYYY/MM/DD HH:MM:SS} 
---

{Post Markdown Content}
```

### Projects

You can add projects by simply placing markdown files into **site/content/projects**. A proper project file follows the following format ("---" is necessary):

```
---
title: {Project Title}
date: {YYYY/MM/DD HH:MM:SS}
tagline: {A Shirt Description of Your Project}
link: {Project Link}
technologies: {Tech}, {Tech}, {Tech}
---
```

### Header

You can alter the header of your website by editing **site/content/header.md**. A proper **header.md** looks like this ("---" is necessary):
```
---
name: {Name}
tagline: {A Short Description About Yourself}
links: [{Link Text}]({Link Dest}), [{Link Text}]({Link Dest})
main_color: {Header Background Color}
secondary_color: {Non-Header Link Color}
top_address: {Your sites top address - Usually will be your domain}
---
```

## Custom Themes

You can create your own custom theme by creating a third, **theme** folder alongside the **content** and **public** folders.

```
site/
    content/
    theme/
    public/
```

Inside this theme folder, place the following files. You can generate the theme folder with the default templates by running: **$ dundas-peak -t site/**

|    File Name     |   Description   |     Parameters    |
|------------------|-----------------|-------------------|
| home.html | The Home Page Template | {{name}}, {{header}}, {{projects}}, {{posts}} |
| posts.html | The All Posts Page Template | {{name}}, {{posts}} |
| post.html | The Post Content Template | {{title}}, {{tagline}}, {{content}}, {{name}}, {{date}} |
| post_preview.html | The Post Preview Template | {{title}}, {{tagline}}, {{date}} |
| projects.html | The All Projects Page Tempalte | {{name}}, {{projects}} |
| project_preview.html | The Project Preview Template | {{title}}, {{tagline}}, {{technologies}} |
| style.css | The Stylesheet Template | {{main_color}}, {{secondary_color}} |

Each template will accept different parameters. Place these template handlebars anywhere you want in your HTML and CSS. Example Theme Files Included in Dundas Peak Repository.
