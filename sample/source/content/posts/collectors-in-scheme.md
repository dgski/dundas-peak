---
title: "Collector Functions in Scheme"
date: 2018-10-16T13:34:41-04:00
description: "Building and Passing Lambda Sandwiches"
type: post
tags:
    - Scheme
    - Functional Programming
draft: false
---

The Collector paradigm is a functional way of assembling and processing collections of data. As a design pattern it also offers a few benefits, and at the very least provides a unique academic exercise.

I've recently been working through the classic book; [The Little Schemer](https://www.goodreads.com/book/show/548914.The_Little_Schemer), which is a cute and concise introduction to Scheme. The Chapter covering this topic delivered the first real friction, so I took some time to chew through it, understand it, and expand its explanation.

Let's start by defining a function which takes a list of numbers, and uses the collector paradigm to sort the numbers into two categories, Odd and Even:

``` scheme
(define sep-odds
  (lambda (lat col)
    (cond
      ((null? lat)
        (col '() '()))
      ((= (modulo (car lat) 2) 0)
        (sep-odds (cdr lat) (lambda (odds evens)
                              (col odds (cons (car lat) evens)))))
      (else
        (sep-odds (cdr lat) (lambda (odds evens)
                                (col (cons (car lat) odds) evens)))))))
```

This function will consume two arguments: a list of numbers (lat) and a collector function (col). Its body consist's of one **cond** statement with 3 branches:

Branch                       | Description
----------                   |------
(null? lat)                  | Checks if **lat** is empty
(= (modulo (car lat) 2) 0)   | Checks if the first entry in the list is even
else                         | Concludes first list entry must be odd


Take a note as to how **sep-odds** recurs: It does not build a list in the simple way of functionally building up a collection. Instead, if **sep-odds** is to recur it builds a lambda function which will perform a particular task (in this case using **cons** to prepend the number to one of the lists) and then passes this lambda function to the next call of **sep-odds** as the **col** argument.

Wrapping a task in a lambda is a neat trick: It delays execution of the wrapped code until the lambda is called.

Another neat trick is that **sep-odds** stuffs the previous collector function (bound to **col** as a function parameter) into the new lambda's body. I like to visualize this as a sandwich, with previous collectors being stuffed inside new ones. Finally, when one of these lambdas is called, it does its work, and then calls the collector function embedded inside it.

For our **sep-odds** function, you may notice that we need to provide an initial collector. Let's get to it.

**First Time with Scheme?** *Please Note: (car x) returns the first element of the list x, and (cdr x) returns list x excluding the first element.*

We will define **list** as our 0th collector. This will be the very centre of the lambda function sandwich:

``` scheme
(define col-0 list)
```

Now let's step through what happens when we perform the following function call:

``` scheme
(sep-odds '(1 2 3 4) col-0) ; return value -> ((1 3) (2 4))
```

**(car lat)** for the first call of sep-odds is **1**, therefore the third condition (**else**) is matched. This will generate a new lambda with **col-0** as **col** and **1 (car lat)** inside it. For clarity, we will label this newly generated lambda function as **col-1**:

``` scheme
; consumes two lists (odds and evens) and adds 1 to the odds list
(define col-1
  (lambda (odds evens)
    (col-0 (cons 1 odds) evens)))
```

This conditional branch will then call sep-odds again, with the rest of the list **(cdr lat)** and the newly-generated **col-1** as arguments:

``` scheme
(sep-odds '(2 3 4) col-1)
```

After this second function call the second conditional branch **(= (modulo (car lat) 2) 0)** is triggered as **2** is an even number. Same story again: a new lambda is built, but this time **col-1** is bound to **col**:

``` scheme
; consumes two lists (odds and evens) and adds 2 to the evens list
(define col-2
  (lambda (odds evens)
    (col-1 odds (cons 2 evens))))
```

Now **col-2** and the remainder of the list will serve as the arguments for the **sep-odds** recursive call:

``` scheme
(sep-odds '(3 4) col-2)
```

We repeat, again with an odd number (**3**):

``` scheme
; consumes two lists (odds and evens) and adds 3 to the odds list
(define col-3
  (lambda (odds evens)
    (col-2 (cons 3 odds) evens)))
```

and our next call:

``` scheme
(sep-odds '(4) col-3)
```

Again, with an even number (**4**):

``` scheme
; consumes two lists (odds and evens) and adds 4 to the evens list
(define col-4
  (lambda (odds evens)
    (col-3 odds (cons 4 evens))))
```

Finally, since the input list is empty, the **null?** conditional is triggered with this final call:

``` scheme
(sep-odds '() col-4)
```

That **null?** conditional applies the **col** function. In this case it is our latest, **col-4**:

``` scheme
; excerpt:
((null? lat) (col-4 '() '()))
```

At this moment our sandwich is at its juiciest. Each layer takes an input from the previous one, as the layers are stripped away. We will begin with the outside 'bun' of the sandwich and go inwards. Each collector function calls the collector embedded within it:

``` scheme
(define col-4
  (lambda (odds evens)
    (col-3 odds (cons 4 evens))))
;   |____|
;     |
;     |
      (define col-3
        (lambda (odds evens)
          (col-2 (cons 3 odds) evens)))
;         |____|
;           |
;           |
            (define col-2
              (lambda (odds evens)
                (col-1 odds (cons 2 evens))))
;               |____|
;                 |
;                 |
                  (define col-1
                    (lambda (odds evens)
                      (col-0 (cons 1 odds) evens)))

```

We are lucky we used labels, In reality the lambda generated looks like this:

``` scheme
(lambda (odds evens)
    ((lambda (odds evens)
          ((lambda (odds evens)
                ((lambda (odds evens)
                      (col-0 (cons 1 odds) evens))
                  odds (cons 2 evens)))
            (cons 3 odds) evens))
      odds (cons 4 evens)))
```


And this is how the function calls look with the computed values of the arguments:

``` scheme
1. (col-4 '() '())
2. (col-3 '() '(4))
3. (col-2 '(3) '(4))
4. (col-1 '(3) '(2 4))
5. (col-0 '(1 3) '(2 4))
6. (list '(1 3) '(2 4))
7. ((1 3) (2 4))
```

All of these expressions have the same value.

So now the big question: Why? Why complicate things like this? Why not just use multiple lists as arguments to gather the data we want?

Well, Collector functions offer two advantages: Delayed Execution, and Continuations.

To address the first point, in this example, each generated lambda is doing something relatively inexpensive: using **cons** to prepend a number to a list. But what if it was performing some sort of expensive operation? For example: performing operations on a matrix with massive proportions, or rendering graphics. Perhaps there is hypothetical example where we do not want to do anything computationally expensive until the data input has been completely verified, from start to finish? In that case a collector paradigm would work well. Building the collector works sort of as a type of manifest or queue, of operations to be performed.

Secondly, if you have a function which represents a series of steps, and each step has their own state, you can travel back to these states. In effect, you save a snapshot of that point in time. And in Scheme, since you can pass function along; you can pass these snapshots around. This gives you flexibility to return back to times where data was valid or matched some sort of pattern. One thing that comes to mind is traversing a Tree data structure and if you are exploring the children of a branch, you can pass forward a collector that represents that root, until you reach something you need and feed that data into the collector.
