## Artifacts

> [!WARNING]
> Added in build 34.
This article covers following topics:
1. [`artifact` instruction](#artifact)
2. [`fork` instruction](#forking-the-artifact)
3. `home` pointer

### `artifact`
Artifacts are loadable containers containing compiled code. To make it easier to understand, they behave like Java's class files.
To create an artifact, use the artifact instruction:

```asm
.start
    artifact MyArtifact -> { ;this creates a file MyArtifact.asmartifact
        [force] .start ; tell the compiler optimizer to ignore the duplicate .start section
        proc ArtifactProcedure ; now equivalent to java class methods
            mov tlr, "Hello from the artifact"
            [native] call print
            halt 0
        end
    }
```

Now, to use an artifact, we have to load it into a program, as this thing above just creates an artifact binary next to your app binary; so to create this compiled artifact, you have to run your program.

### Forking the artifact
To load an artifact, use `fork`:

```asm
; completely different program
.start
    fork MyArtifact ; has to match the name MyArtifact.asmartifact

```

If the artifact loaded with no exceptions, we can simply call its procedures using the `home` pointer or by directly addressing the procedure with its home artifact:

```asm
    [home] call ArtifactProcedure ; home ptr must be set by fork instruction, fork is like fetch but for artifacts
    ; or java-style
    call MyArtifact.ArtifactProcedure
```