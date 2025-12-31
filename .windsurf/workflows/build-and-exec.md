---
auto_execution_mode: 1
---

- Find all available presets using `cmake --list-presets`
- For each preset:
  - Configure the project using `cmake --preset <preset-name>`
  - Build the project using `cmake --build --preset <preset-name>`
  - Execute the program using `./exec.sh <preset-name>`
  - If any step fails, continue to the next preset
- Log results for each preset
