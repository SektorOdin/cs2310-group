- Use `.hpp` for header files, use `.cpp` for cpp files
- Put header files inside `/include/`; put cpp files inside `/src/` under proper directories

- I don't even know what to call this project to be honest, it is this sort of thing that auto-calculate and summarize your performance in university
- Some ideas on expansion, but right now the flow
    1. User double click the `.exe`
    2. The program generates a `/data` folder and a `/summary` folder if they are missing
    3. The program executes the init procedure if it cannot find any correctly formatted `.json` files inside `/data`
        
        The init procedure
        1. Ask the user their display name, it can be changed later and need no be their real name, used in summary generation, e.g., "Lorem"
        2. Ask the user their degree name, the full degree name in `std::string` (e.g., "BSc Computer Science")
        3. Ask the user their currect semester, the format is a number followed by either a 'A', a 'B' or a 'S' which indicates summer semester - the number should be [1, 4] (for now, unless there is exception), e.g., "2A"
        4. Ask whether the user want to add at least one course to the semester record, reply with 'Y' / 'y' for yes and 'N' /'n' for no - a semester with no course is blocked from summary generation, and user must add at least 1 course later to access the summary feature
        5. no -> setup complete; yes -> ask the user to input course code, it should be two letters followed by four numbers (in most cases), e.g., "CS2312"
        6. Ask the user to input the course name, this should be a `std::string`, e.g., "Computer Programming"
        7. Ask the user to input at least one component, otherwise this course would be invalid; first ask them about the name of this component, e.g., "Midterms"
        8. Then, ask the user the percentage of this component (in total), the user should input a number (0, 100) with decimal support (up to 2 digits?)
        9. By default, components are of "single" type, meaning the percentage represents the entire component (e.g., midterms), ask the user if they want to change this to "a total", i.e. quizzes or assignments that has multiple parts, the user should input 'N' / 'n' if they do not want to chage this, and a number (0, 100; integer) for equal "partition"
        10. setup complete
    
    4. If no init is needed, the program goes into the main menu with these options
        
        1. Edit user data
        2. Add new semester record
        3. Add new courses
        4. Add new components
        5. Create record
    
    5. Edit user data - allow the user to edit these data: display name, degree mame
    6. Add new semester record - add a record of new semester (a new file in `/data`), see init step 3. When this procedure ends, there should be 3 options. Option 1 - continue to "Add new semeester record"; Option 2 - continue to "Add new courses"; Option 3 - quit to menu, expected input for this last question - [1,3]
    7. Add new courses - add a new course given the semester (only if there is more than one semester record; inherit from "Add new semester record" if re-directed from there), see init step 4. When this procedure ends, there should be 3 options. Option 1 - continue to "Add new courses"; Option 2 - continue to "Add new components"; Option 3 - quit to menu, expected input for the last question - [1,3]
    8. Add new components - add a new component to a course given the course code or course name, for now, a semester should also be provided for searching. Flow: select option -> enter semester -> select code or name -> enter code or name. See init step 7 and 8. When this procedure ends, there should be 2 options. Option 1 - "Add new components"; option 2 - quit to menu
    9. Create record - ask user to input the semester, validate that the file in `/data` that matches the semester and generate a Markdown file with the format "Year <number> Semester <letter> Report" with... some formatting that shows their semester record and calculation

    
