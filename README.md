# Dundas Peak Simple Static Site Generator
Dundas Peak is **No-Nonsense, Low Configuration Personal Site Generator** for Developers. It doesn't offer much flexibility because it  creates websites with the most basic features most developers need:
- Showcased Links (contact information, repos, resume, email, linkedIN etc...)
- A portfolio for projects
- A simple blog

Because that's all you really need!

# Getting Started

Dundas Peak starts with a very simple, almost flat directory structure. Here is a sample site directory with all the required files:

```
site/
    content/
        files/
        posts/
        projects/
        header.md
```

There are two main sub-directories within your site directory:

- The content folder holds your site content. This includes your blog posts, projects, personal header, and any supporting files.
- The theme folder holds the html templates for the various elements of content.

Dundas-Peak will combine your Content with Your Theme to Generate Your Site. You can customize your theme, or simply use the provided "Clean" theme.

## Content

### Files

You can place any files you reference in your posts or links here. These will copy over to the public folder and will eventually be reachable at **<yoursite.com>/files/<your files here>**.

### Posts

You can create posts by simply placing markdown files into **site/posts**. Add them with a proper metadata header and these posts will be added to your blog. The post markdown file format looks as follows ("---" is necessary):

```
---
title: {Post Title}
tagline: {A Short Description of Your Post}
date: {Date Time with the following format: "YYYY/MM/DD HH:MM:SS"}
---

{Post Markdown Content}
```

### Projects

You can add projects by simply placing markdown files into **site/projects**. A proper project file follows the following format ("---" is necessary):

```
---
title: {Project Title}
date: {Date Time with the following format: "YYYY/MM/DD HH:MM:SS"}
tagline: {A Shirt Description of Your Project}
link: {A link to your projects website or github repository}
technologies: {A Comma Separated List with the following format: "{Tech}, {Tech}, {Tech}"}
---
```

### Header

You can alter the header of your website by editing **site/header.md**. A proper **header.md** looks like this ("---" is necessary):
```
---
name: {Name}
tagline: {A Short Description About Yourself}
links: {A Comma Separated List with the following format: "[{Link Text}]({Link Dest}), [{Link Text}]({Link Dest})"}
main_color: {Header Background Color}
secondary_color: {Non-Header Link Color}
top_address: {Your sites top address - Usually will be your domain}
---
```
