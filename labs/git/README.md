# Rappels sur git / github

## git

Init et status:

````
$ mkdir myrpoject
$ cd myrpoject
$ git init
Initialized empty Git repository in /home/blottiere/tmp/git/.git/
$ git status
On branch master
nothing to commit, working tree clean
````

Branche, add, commit et merge:

````
$ git checkout -b mybranch
Switched to a new branch 'mybranch'
$ git status
On branch mybranch
nothing to commit, working tree clean
$ touch myfile.txt
$ git status
On branch mybranch
Untracked files:
  (use "git add <file>..." to include in what will be committed)

	myfile.txt

nothing added to commit but untracked files present (use "git add" to track)
$ git add myfile.txt
 On branch mybranch
Changes to be committed:
  (use "git reset HEAD <file>..." to unstage)

	new file:   myfile.txt
$ git commit -m 'My first message'
[mybranch 6f44af5] My first message
 1 file changed, 0 insertions(+), 0 deletions(-)
 create mode 100644 myfile.txt
````

Log et reset:

````
$ touch myfile2.txt
$ git add myfile2.txt
$ git status
On branch mybranch
Changes to be committed:
  (use "git reset HEAD <file>..." to unstage)

	new file:   myfile2.txt
$ git reset myfile2.txt
On branch mybranch
Untracked files:
  (use "git add <file>..." to include in what will be committed)

	myfile2.txt

nothing added to commit but untracked files present (use "git add" to track)
$ git add myfile2.txt
$ git commit -m 'My second message'
[mybranch 942785d] My second message
 1 file changed, 0 insertions(+), 0 deletions(-)
 create mode 100644 myfile2.txt
$ git log
commit 942785d56b2c1dddbc8ebc3960fa6ccdeebab7a7
Author: Blottiere Paul <blottiere.paul@gmail.com>
Date:   Fri May 25 19:57:59 2018 +0200

    My second message

commit 6f44af5206758b3240fdb69c8e3b0d99111ffa75
Author: Blottiere Paul <blottiere.paul@gmail.com>
Date:   Fri May 25 19:55:03 2018 +0200

    My first message
$ git reset HEAD~
$ git log
commit 6f44af5206758b3240fdb69c8e3b0d99111ffa75
Author: Blottiere Paul <blottiere.paul@gmail.com>
Date:   Fri May 25 19:55:03 2018 +0200

    My first message
$ git status
On branch mybranch
Untracked files:
  (use "git add <file>..." to include in what will be committed)

	myfile2.txt

nothing added to commit but untracked files present (use "git add" to track)
$ git add myfile2.txt
$ git commit -m 'My second message'
[mybranch b7d6240] My second message
 1 file changed, 0 insertions(+), 0 deletions(-)
 create mode 100644 myfile2.txt
````

Rebase et squash:

````
$ git rebase -i HEAD~2
 pick 6f44af5 My first message
 s b7d6240 My second message

 # Rebase ef20ef6..b7d6240 onto ef20ef6 (2 commands)
$ git log
commit 174877fd97f61cbbcb790154bbbc1835ddd94e2c
Author: Blottiere Paul <blottiere.paul@gmail.com>
Date:   Fri May 25 19:55:03 2018 +0200

    My first message
$ git reset HEAD~
$ git status
On branch mybranch
Untracked files:
  (use "git add <file>..." to include in what will be committed)

	myfile.txt
	myfile2.txt

nothing added to commit but untracked files present (use "git add" to track)
$ git add myfile.txt myfile2.txt
$ git commit -m 'My message'
[mybranch f287055] My message
 2 files changed, 0 insertions(+), 0 deletions(-)
 create mode 100644 myfile.txt
 create mode 100644 myfile2.txt
````

Rebase and merge:

````
$ git rebase master # conflist possible
Current branch mybranch is up to date.
$ git checkout master
$ git merge mybranch
````

Stash:

````
$ echo 'temp modification' > myfile.txt
$ git status
On branch master
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

	modified:   myfile.txt

no changes added to commit (use "git add" and/or "git commit -a")
$ git stash
Saved working directory and index state WIP on master: f287055 My message
HEAD is now at f287055 My message
Saved working directory and index state WIP on master: f287055 My message
HEAD is now at f287055 My message
$ git status
On branch master
nothing to commit, working tree clean
$ git stash pop
On branch master
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

	modified:   myfile.txt

no changes added to commit (use "git add" and/or "git commit -a")
Dropped refs/stash@{0} (caceef3a30784d91998ccd57e93b83e92d6406d4)
````

## github
