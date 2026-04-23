<div align="center">
    <img height="300" src="readme_resources/newasm_logo.png">
</div>

<h1 align = "center">Welcome to the <i><b>NewASM</b></i> Wiki</h1>

<div align="center" style="border-radius: 5px;">
    <h3><img border-radius="5px" height="20" align="center" src="readme_resources/brace_product.png"></h3>

![Language](https://img.shields.io/github/languages/top/bracesoftware/newasm?style=flat&color=brightgreen)
![Issues](https://img.shields.io/github/issues/bracesoftware/newasm?style=flat&color=brightgreen)
![Pull Requests](https://img.shields.io/github/issues-pr/bracesoftware/newasm?style=flat&color=brightgreen)
![Downloads](https://img.shields.io/github/downloads/bracesoftware/newasm/total?style=flat&color=brightgreen)
![License](https://img.shields.io/github/license/bracesoftware/newasm?style=flat&color=brightgreen)

</div>

<h3 align = "center">Low-Level Language that Mimics Assembly</h3>

**NewASM** is a low-level programming language which combines explicit memory and register control, giving it a breeze of assembly-like feel, with high-level functionalities such as objects, threads and more.

> [!NOTE]
> **NewASM** language runs inside a NewASM Virtual Machine.

> [!CAUTION]
> All of the following documentation applies exclusively to the version currently being in development. For docs about the version you downloaded, check the files you downloaded for the releases page.

# Documentation
Below is the simple `Hello World` program written in New-Assembly.

```asm
using "ios"
.data
    string text : "Hello world\n"
    intg len : $-text
.start
    mov tlr, text
    mov fdx, 1
    mov bos, len

    sysenter "ios"
    
    syscall

    ret 0
```
> [!WARNING]
> The wiki is being completely reworked, so it may be missing some stuff.

NewASM allows you to write semi-efficient low-level programs in one universal assembly-like language. This language features many things immediatelly out-of-the-box *such as concurrency, thread channels, basic crypto, file, math and console functionality, etc.*, so you do not have to worry about any library installation. However, if you wish to add any new functionality, you can easily integrate dynamic libraries (DLLs, SOs) into your project.

Since NewASM runs in a virtual machine, it is thus running in a fully controlled sealed environment, so it is great for learning and experimenting.

# Table of contents
- [Using the executeable](#using-the-executeable)
  - [Launch modes](#launch-modes)
  - [Shell mode](#shell-mode)
  - [Interpreter mode](#interpreter-mode)
- [The language](#the-language)

## Using the executeable
NewASM build consists of 2 programs, `newasm` and `nprogwin`. What YOU need is the `newasm` executable file which may be run in 2 modes.
Arguments are provided using the `newasm_args` variable you create before running the executeable, ergo:
1. Windows
```bat
set newasm_args=arg1,arg2
```
2. Linux
```bash
export newasm_args=arg1,arg2
```

| Argument | Parameters | Description |
| ---------------- | --------- | ----------- |
| `h` | - | Displays help about these commands. |
| `l` | - | Turn on the logging system. |
| `nv` | - | Disable version checking. |
| `std` | - | Use the standard library. |
| `nodbg` | - | Disable the debug window. |
| `out` | `<binary name>` | Provide binary name for the compiler, default is `a.out`. This is argument is useful only in compiler/interpreter mode. |
| `nolco` | - | Tells the compiler not to log any optimization information into the console. |

Example:
```bat
set newasm_args=l,h,std,out=app
```

### Launch modes
When running the `newasm` executeable, you can run it in 2 different modes:
* **shell**: this is the shell/control console mode, the core of the virtual machine - application will run as the command prompt with its own commands, you can install packages and maintain your project;
* **compiler/interpreter**: this mode is used for developing applications - linking libraries and files, generating reusable binaries, testing the app and getting profiler information.

To run the interpreter, use `newasm <filename>.asm`, but to run the shell, just run the `newasm` app.

> [!NOTE]
> To read more about configuring your NewASM application, [click here...](docs/config.md)

***

#### Shell mode
Shell mode brings commands with it. The shell mode is the PRIMARY mode for end users. Basically the "operating system" for the virtual machine. [Click here](docs/cmd.md) to view a list of available commands.

#### Compiler/interpreter mode
Compiler/interpreter mode compiles and runs your application through several phases.
1. **Linker phase**: In this phase, the linker links all files included in the application into one internal format.
2. **Internal compilation**: In this phase, the system tokenizes and resolves some compile-time stuff before running the program. This ensures safe and stable program execution. In this phase, the system is telling the virtual machine what kernel modules will be used during the execution of the program.
3. **Binary format generation**: In this phase, the compiler spews out the binary for your app. Links the dynamic libraries you provided in the project file. This binary can be then reused in the shell.
4. **Execution**: Final phase (used for app devs), the system runs the compiled code!

> [!TIP]
> We're planning to add JIT compilation into native code, but that's in the testing phase.

> [!NOTE]
> [Click here...](docs/comptins.md) to read more about compile-time instructions/directives.

***
## The language
This section of the wiki provides a deep walkthrough of the language itself.
### Table of contents
- [Code sections](#code-sections)
- [Available instructions](#available-instructions)
- [Language concepts](#language-concepts)
- [Other helpful resources](#other-resources)

***
#### Code sections
NewASM code is, as in other assemblers, divided into different sections that have their own syntax.
There are 4 different sections and each one has a different purpose:
1. `.start` or `.text` - this is the code section that contains functional code (instructions), can contain procedure (function) and thread definitions;
2. `.data` - this is where you declare your variables, references, containers, and more;

You change a section by doing:
```asm
.section_name
```

For example:

```asm
.start ; .text is same
    ; code
.data
    ; declarations
    ; etc...
```

***
#### Storing data into variables and containers
In NewASM there's a huge variety of built in variable types and data containers. [Read more...](docs/data.md)

***
#### Available instructions
NewASM features many instructions, around 70 of them. Here is a list:
1. [Data manipulation instructions](docs/instructions/data_manip.md)
2. [Manual memory allocation instructions](docs/instructions/malloc.md)
3. [Kernel-related instructions](docs/instructions/kernel.md)
4. [Execution flow instructions](docs/instructions/exec_flow.md)
5. [Miscellaneous instructions](docs/instructions/misc.md)
6. [Mathematical instructions and bit operations](docs/instructions/math.md)
7. [Thread-related instructions](docs/instructions/thread.md)
8. [Union-related instructions](docs/instructions/union.md)
9. [Switch block instructions](docs/instructions/switch.md)
10. [I/O port-related instructions](docs/instructions/ioports.md)
11. [Calculated calls and returns](docs/instructions/calc.md)
12. [Instructions for address fetching](docs/instructions/fetch.md)

***
#### Language concepts
NewASM features different concepts such as decorators, namespaces, primitive classes and more advanced stuff.
1. [Decorators](docs/concepts/deco.md)
2. [Namespaces](docs/concepts/namespaces.md)
3. [Anonymous procedures](docs/concepts/lambda.md)
4. [Environment variables](docs/concepts/env.md)
5. [Sealed labels](docs/concepts/sealed.md)
6. [Attributes](docs/concepts/attrib.md)
7. [Events](docs/concepts/events.md)
8. [Macros](docs/concepts/macros.md)

## Other resources
This is a list of extra resources that might help.
1. [Error exit codes](docs/exit_codes.md)
2. [Calling external libraries (DLLs, SOs)](docs/interop.md)
3. [Interesting examples](docs/examples.md)