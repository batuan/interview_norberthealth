
search_dir=./IR
for entry in "$search_dir"/*
do
  echo "$entry"
  ./find_contours -image=0 "$entry"
done