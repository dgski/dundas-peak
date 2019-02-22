# Dundas Peak Simple Static Site Generator
**Dundas Peak** is No-Nonsense, Low Configuration Personal Site Generator for Developers. It doesn't offer much flexibility because it just creates websites with the features most developers need:
- Showcased links (contact information, repos, resume, email etc...)
- An about page
- A portfolio for projects
- A simple blog

Because that's all you really need!

# Getting Started

Dundas Peak starts with a very simple, almost flat directory structure. Here is a sample directory with all the required files:

```
site/
    content/
        files/
        posts/
        projects/
        header.md    
    theme/
        home.html
        posts.html
        post.html
        post_preview.html
        projects.html
        project.html
        project_preview.html
        style.css
```

There are two main sub-directories within your site directory:

- The content folder holds your site content. This includes your blog posts, projects, personal header, and any supporting files.
- The theme folder holds the html templates for the various elements of content.

Dundas-Peak will combine your content with Your Theme to Generate Your Site. You can customize your theme, or simply use the provided "Clean" theme.

## Content

### Files

You can place any files you reference in your posts or links here.

### Posts

T

### Projects

### Header

## Theme

If you would like to alter the look and layout of your site, you can create a custom theme. All it is is just 8 template files:

- home.html
- posts.html
- post.html
- post_preview.html
- projects.html
- project.html
- project_preview.html
- style.css
