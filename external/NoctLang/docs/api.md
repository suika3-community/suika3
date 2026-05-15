Standard API
============

The standard API is implemented in a modular library called
`libnoctapi`, separated with the core `libnoct` library. The
separation allows you to include only the core components or the
entire API in your binary through build options.

There are no mandatory APIs, and developers are free to choose which
ones to integrate into their applications.

Currently, the `noct` command includes all APIs. We plan to introduce
some different runtimes including the `gnoct` command that includes
GUI libraries.

## System

### System.import()

Imports a script file or a bytecode file.

```
System.import("script.noct");
```

### System.shell()

Runs a shell command.

```
System.shell("ls -lha");
```

---

## Math

### abs()

```
var a = abs(x);
```

### random()

```
var r = random(); // 0 .. 1.0
```

### Math.sin()

```
var y = sin(x);
```

### Math.cos()

```
var y = cos(x);
```

### Math.tan()

```
var y = tan(x);
```

---

## Console

### Console.print()

Prints a text to the console.

```
Console.print("Hello, world!");
```
