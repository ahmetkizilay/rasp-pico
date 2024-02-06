#/bin/bash
# Check for new line at the end the file

# collect the list of files changed

files_changed=()
while read line
do
  echo "$line"
  files_changed+=( "$line" )
done

echo "checking updated files"

num_failed=0
# process each file
for f in ${files_changed[@]}; do
  echo -n "$f"
  if [ "$(tail -c 1 "$f" | wc -l)" -eq 0 ]; then
    echo " ... failed"
    num_failed=$((num_failed+1))
  else 
    echo " ... passing"
  fi
done

if [ $num_failed -gt 0 ]; then
  echo "Some files failed new_line check!"
  exit 1
fi