#/bin/bash
# Check for new line at the end the file

# collect the list of files changed

files_changed=( "${@}" )

echo "checking updated files"

num_failed=0
# process each file
for f in ${files_changed[@]}; do
  echo -n "$f"
  if [ "$(tail -c 1 "$f" | wc -l)" -eq 0 ]; then
    # red cross mark
    printf " \033[31m\xE2\x9D\x8C\033[0m\n"
    num_failed=$((num_failed+1))
  else
    # green tick mark
    printf " \033[32m\xE2\x9C\x94\033[0m\n"
  fi
done

if [ $num_failed -gt 0 ]; then
  echo "Some files failed new_line check!"
  exit 1
fi
