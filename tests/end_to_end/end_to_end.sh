set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT_DIR=$(CDPATH= cd -- "$SCRIPT_DIR/../.." && pwd)
DATA_DIR="$SCRIPT_DIR/data"
ANSW_DIR="$SCRIPT_DIR/answ"
PROG="$ROOT_DIR/build/3D_triangles"

pass=0
total=0
tmp_out=$(mktemp)
trap 'rm -f "$tmp_out"' EXIT HUP INT

cd "$ROOT_DIR"

for in_file in "$DATA_DIR"/*.dat; do
  total=$((total+1))
  fname=$(basename "$in_file")
  base=${fname%.*}
  ans_file="$ANSW_DIR/$base.res"

  if [ ! -f "$ans_file" ]; then
    printf "%-6s %s\n" "$base" "NO_ANSW"
    continue
  fi

  if "$PROG" < "$in_file" > "$tmp_out" 2>/dev/null; then
    if diff -q "$tmp_out" "$ans_file" >/dev/null 2>&1; then
      pass=$((pass+1))
      printf "%-6s %s\n" "$base" "OK ✅"
    else
      printf "%-6s %s\n" "$base" "FAIL ❌"
      [ "${VERBOSE:-0}" = "1" ] && diff -u "$ans_file" "$tmp_out" | sed -n '1,50p'
    fi
  else
    printf "%-6s %s\n" "$base" "RUNTIME_ERROR"
  fi
done

echo "------------"
printf "Passed %d/%d\n" "$pass" "$total"