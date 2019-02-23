# Dundas Peak Simple Static Site Generator
Dundas Peak is **No-Nonsense, Low Configuration Personal Site Generator** for Developers. It doesn't offer much flexibility because it  creates websites with the most basic features most developers need:
- Showcased Links (contact information, repos, resume, email, linkedIN etc...)
- A Portfolio for Projects
- A Simple Blog

Because that's all you really need!

## Getting Started

Dundas Peak starts with a very simple, almost flat directory structure. You must create these on your own. Here is a sample site directory with all the required files:

```
site/
    content/
        files/
        posts/
        projects/
        header.md
```

Run the application with a single argument specifying the location of your site directory. For example:

```
$ ./dundas-peak site
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
