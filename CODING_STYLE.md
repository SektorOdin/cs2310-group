***

# Contributing Guidelines

This project loosely follows the **LLVM Coding Standards**. Please adhere to the following rules to ensure code consistency and maintainability.

## 1. Naming Conventions
*   **Types/Classes:** `UpperCamelCase` (e.g., `MyClass`)
*   **Variables:** `lowerCamelCase` and should be a noun (e.g., `myVariable`, `userList`)
*   **Functions:** `lowerCamelCase` and must be a `verb + noun` (e.g., `calculateSum`, `getAge`). Avoid single-word functions (e.g., use `emptyList()` instead of `empty()`).

## 2. Formatting & Style
*   **Indentation:** 4 spaces.
*   **Braces:** Allow single-line statements without braces if desired (e.g., `if (x) return;`).
*   **Parentheses:** Space before opening parenthesis only in control flow (`if`, `for`), not in function calls.
*   **Early Returns:** Use early exits and `continue` to simplify logic. Do not use `else` after a `return`.

## 3. General Practices
*   **Namespace:** **Never** use `using namespace std;`. Use fully qualified names or specific aliases like `using std::cout;`.
*   **IO:** Use `\n` instead of `std::endl` (buffer flushing is rarely needed).
*   **Loops:** Use range-based for loops (`for (auto& item : items)`) where possible.
*   **Increment:** Prefer pre-increment (`++i`) over post-increment.
*   **Assertions:** Use `assert()` to indicate implicit conditions that must be met.
*   **Visibility:** Functions and methods should have the most restricted visibility possible.

## 4. Architecture & Quality
*   **No Magic Numbers:** Use `const` or `constexpr` for all literal values.
*   **No God Objects:** Keep classes and files focused on a single responsibility.
*   **No Spaghetti Code:** Use early returns to flatten logic; avoid deep nesting.
*   **YAGNI:** Do not write functions that are not currently used.
*   **Simplicity:** Write verbosely and clean up afterwards. Avoid premature optimization or excessive abstraction.

***