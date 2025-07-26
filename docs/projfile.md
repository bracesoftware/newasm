- [Go back...](../README.md)

## Project files
**Project files** (`.newasm_proj` file) are files that define information about your New-ASM project. General name format for them is:

```
<entry file name>.newasm_proj
```
For example, if my `-input` file is `input.asm`, project file for that would be `input.asm.newasm_proj`. This is basically an INI file.

### Keys and values available
| Key name | Description |
| -------- | ----------- |
| `name` | Name for your project. |
| `version` | Version of your project. |
| `dlibs` | Dynamic libraries your project is using. |

Example `index.asm.newasm_proj` file:
```ini
name : Unnamed project
version : 0.0.1
dlibs : testlib, sayhi
```
