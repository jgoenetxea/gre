Goe render

Instruction:
-------------------------------------------------
1º CMake Configure
2º Add GLM library path
3º apt-get install libglfw-dev

4º Include qt paths in CMake "Qt/5.4/gcc_64/lib/cmake/Qt5Widgets" & "Qt/5.4/gcc_64/lib/cmake/Qt5OpenGL"

Description:
-------------------------------------------------
Opensource Render Engine molon

Dependencies:
--------------------
- GLM


Git tips:
-------------------
    Note:   Edit variables $(var_name) assiming origin is the remote.
    Note2:  Terminal code prefix $

· Clone with all submodules:              $ git clone --recursive $(url_repository)
· Create new branch:                      $ git branch $(branch_name)
· Change to branch:                       $ git checkout $(branch_name)
· Remove a branch:                        $ git branch -d $(branch_name)
· Remove branch from origin:              $ git push :$(branch_name)
· Checkout a new branch from origin:      $ git checkout -b $(branch_name) origin/$(branch_name)
· Add new file to repository:             $ git add $(file_or_macro)
· Remove file from repository:            $ git rm $(file_or_macro)
· Save edit changes in your PC:           $ git commit -a -m "$(description)"
· Update your repository from origin:     $ git pull origin $(your_branch):$(their_branch)
· Share your changes                      $ git push origin $(your_branch):$(their_branch)
· Create a new tag in current branch:     $ git tag -a $(tag_name)
· Add a submodule:                        $ git submodule add $(url_repository) $(relative_path)


* How to merge:
  $ git checkout master
  $ git merge --no-ff hotfix-1.2.1
  $ git tag -a 1.2.1
  $ git push origin master

Git Methodology: A successful Git branching model --> http://nvie.com/posts/a-successful-git-branching-model/

Tools tips:
----------------------
· CMake-gui
· Eclipse IDE
· Atom text editor
· Linux :-D
