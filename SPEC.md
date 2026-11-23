# Grade Tool — SPEC

## Project
Auto-calculate and summarize university performance. CLI. C++23, no external
deps besides `libs/nlohmann/json.hpp`.

## File layout
- `.hpp` in `/include/`, `.cpp` in `/src/`
- See `CODING_STYLE.md` and `.clang-format` for style

## Data
One JSON file per semester, in `/data/<semester>.json`
(e.g. `/data/2A.json`). If `/data/` or the file is missing, init runs.

### Data model
```json
{
  "display_name": "leidenfreude",
  "degree": "BSc Computer Science",
  "semester": "2A",
  "courses": [
    {
      "code": "CS2310",
      "name": "Computer Programming",
      "components": [
        {
          "name": "Midterm",
          "type": "single",
          "weight": 20,
          "partitions": 1,
          "marks": [89.50]
        },
        {
          "name": "E-Quiz",
          "type": "total",
          "weight": 20,
          "partitions": 10,
          "marks": [98.04, 93.33, 83.33, 93.83, 93.10, 97.44, 95.75, 95.49, 89.92, -1]
        },
        {
          "name": "Final",
          "type": "single",
          "weight": 55,
          "partitions": 1,
          "marks": [-1]
        }
      ]
    }
  ]
}
```
`type`: "single" (one mark) or "total" (multiple partitions)
`partitions`: number of sub-parts; always 1 for single
`weight`: percentage (0–100). At init, user can type `"remainder"` which
resolves to `100 - (sum of fixed weights)` and is stored as a number
`marks`: array. `-1` = not entered yet. For single, exactly 1 element.
For total, `partitions` elements
No stored subtotals — all computed at display time

### Calculation
- Component sub-total = mean of `marks` entries that are `>= 0`\
 (i.e. ignore `-1` entries). If all are `-1`, sub-total is undefined.
- Course total = sum over components: `(sub-total / 100) * weight`\
 (only for components that have at least one entered mark)
- Semester total = mean of course totals

### Startup
1. Create `/data/` if missing
2. If no `.json` files in `/data/` → run init
3. If `.json` files exist but any fail to parse → delete the bad file, re-init
4. Enter main menu

### Init procedure
1. Ask display name (string, can be anything, e.g. "Lorem")
2. Ask degree name (string, e.g. "BSc Computer Science")
3. Ask semester (format: number [1,4] + letter A/B/S, e.g. "2A")
4. Ask "add a course now?" (Y/N)
    - N → save, done
    - Y → loop:
        1. Ask course code (e.g. "CS2310")
        2. Ask course name (e.g. "Computer Programming")
        3. Loop components:
            1. Ask component name (e.g. "Midterm")
            2. Ask weight: number (0, 100) or `"remainder"`.\
            Remaining weight decreases as components are added.\
                Invalid if > remaining or <= 0.
            3. Ask type: `"single"` or `"total"`.\
                    single → marks = [-1]\
                    total → ask partition count N, marks = [-1, -1, ..., -1] (N entries)\
            4. Ask "add another component?" (Y/N). Stop when weights hit 100 or N.\
        4. Ask "add another course?" (Y/N)
5. Save to `/data/<semester>.json`

### Main menu
```
1. Edit user data        [TODO]
2. Add new semester record
3. Add new courses       [TODO]
4. Add new components    [TODO]
5. Show report
6. Record a mark
0. Quit
```
### 2. Add new semester record
Same as init (new file in `/data/`).

### 5. Show report

Prints a receipt to terminal. One block per semester file in `/data/`:
```
NAME: <display_name>
DEGREE: <degree>
SEMESTER: <semester>

COURSE: <course_name> (<code>)
  (1) <component_name>......... | <weight>% | <sub_total> / 100 | <subtotal>
  (2) <component_name>......... | <weight>% | <sub_total> / 100 | <subtotal>
     (a) <partition 1>         | <mark> / 100
     (b) <partition 2>         | <mark> / 100
  ...
  course total: <X.XX>%
```
- Unentered marks show as `?` in the partition sub-rows
- Total components expand to show each partition
- Single components do not expand

### 6. Record a mark
1. List available semesters, ask which (auto-select if only one)
2. Ask course code
3. List components in that course, ask which
4. For each partition (or the single mark), prompt:\
    "mark (empty = keep current): "
5. Save to JSON, print "updated"

### Stretch / future (not required for deadline)
- Markdown export of the report
- Degree-level report (aggregate over all /data/ files)
- Browse data (hierarchical: degree > year > semester > course > component)
- Edit user data / Add courses / Add components (menu items 1, 3, 4)
- Chaining flow (after adding a semester, offer to add courses, etc.)
- Unequal partition weights (currently isEqual is always true)