#/bin/bash
# list of updated files in given commit

git diff-tree --no-commit-id --name-only $1 -r
