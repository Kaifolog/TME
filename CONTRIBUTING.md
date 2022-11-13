# Welcome to TME contributing guide <!-- omit in toc -->

Thank you for investing your time in contributing to my project! :sparkles:

## Getting started

To begin with you should read the instruction and documentation.

## Commit your changes

Before committing any update you should verify your changes by linters and formatters.

The project uses clang toolchain therefore you should use clang-format and clang-tidy tools.

### clang-format

> You should **NEVER** format themes or text modules.

The project uses Microsoft code style:

```
clang-format -style=Microsoft -dump-config > .clang-format
```

**Before committing** you should apply formatter by command:

```
find ./src/ -iname *.hpp -o -iname *.cpp | xargs clang-format -i
```

The last step before committing is to try to build the current application version.