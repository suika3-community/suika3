Suika3
======

The next-generation visual novel engine for the 2030s and beyond.

`Suika3` is a high-performance visual novel engine developed by the
`Suika3 Community`. Built to be fast, native, and truly universal,
Suika3 aims to empower creators across the globe for the decades to
come.

Games made with Suika3 run on the `Suika3 JIT Virtual Machine` (VM).
The VM executes `NovelML 3.0` along with `SeedScript 3.0`.

`NovelML 3.0` is a tag-based, human-readable markup for visual novels.
Familiar to developers from other engines, it uses concise, declarative tags
for dialogue and scenes, so scripts are readable at a glance.

`SeedScript 3.0` is our original scripting language that runs at true native
speed within the VM. Inspired by JavaScript, SeedScript offers a
streamlined syntax that is more intuitive for beginners yet powerful
enough for seasoned developers. Because both the language and VM are
developed in-house, you never have to worry about being left behind by
upstream language updates or breaking changes. Your scripts will remain
stable and compatible for the long term.

## Status: Work in Progress

This repository is currently under active development. Features and
APIs are subject to change.

## Key Features

* **High Performance**: Powered by the Suika JIT VM, it delivers unparalleled execution speed.
* **Lightweight**: Engineered to run smoothly even on low-spec hardware and in developing regions.
* **Universal Platform**: Designed with a "port anywhere" strategy, ensuring compatibility with almost every modern platform.
* **Extensible**: NovelML can be seamlessly expanded by developers using SeedScript.
* **Reliable**: Built on the proven codebase of Suika2 and integrated with the modern Playfield Engine.
* **Jot-and-Run**: A powerful markup language designed to bring your stories to life in an instant.

## Getting Started

A quick path to try Suika3 locally.

### Using prebuilt binary

Please download from [Releases](releases).

### Building locally

Refer to [build.md](docs/mkdocs-en/build.md) (when available) for platform-specific instructions.

```
git clone https://github.com/suika3-community/suika3.git
cd suika3
mkdir build
cd build
cmake ..
cmake --build .
cp -R ../game/* .
./suika3
```

## Quick Start

`start.novel` file looks like:
```
[bg file="back01.png" time="1.0"]
[click]
```

## Examples

See the /game directory for:
* minimal demo projects
* sample assets and NovelML snippets
* SeedScript usage patterns

## Building

For contributors who want to build from source. (under construction)

## Documentation

Primary docs will include:
* NovelML 3.0 reference
* SeedScript 3.0 language guide
* API docs

## Roadmap

TBD

## Supported Platforms

Suika3 targets desktop platforms first (Windows, Linux, macOS) with a
clear migration path for mobile and embedded targets via the
Playfield Engine.

|Type       |OS         |Status       |Confirmed On                        |
|-----------|-----------|-------------|------------------------------------|
|Desktop    |Windows    |✅ Works     |Windows 11 x64                      |
|           |macOS      |✅ Works     |macOS 15 Sequoia Arm64              |
|           |Linux      |✅ Works     |Ubuntu 24.04 LTS x86_64             |
|Mobile     |iOS        |✅ Works     |iPhone iOS 18                       |
|           |Android    |✅ Works     |Android 16                          |
|BSD        |FreeBSD    |✅ Works     |FreeBSD 14.3 amd64                  |
|           |NetBSD     |✅ Works     |NetBSD 10.1 amd64                   |
|           |OpenBSD    |✅ Works     |OpenBSD 7.7 amd64                   |
|Others     |WebAssembly|✅ Works     |Chrome                              |
|           |Chromebook |✅ Works     |Chrome                              |
|           |Unity      |✅ Works     |Unity 6.2 Windows 11 x86_64         |

## Lineage: The Grand Journey

Suika3 represents the culmination of over two decades of relentless innovation and development:

* **Suika Studio (2001–2004)**: The origin of our codebase, featuring our first GUI-based editors. 
* **Unfruitiful (2005–2015)**: A decade of R&D focused on establishing a robust portability layer for cross-platform support.
* **Suika2 (2016–2024)**: The cornerstone of our current architecture and the fruit of a decade of dedicated R&D.
  It delivers a full visual novel experience with extensive platform compatibility.
* **Playfield Engine (2025–)**: A versatile 2D engine derived from the core portability layer of the Suika series.
* **Suika3 (2026–)**: By synthesizing these legacies and introducing **NovelML 3.0 / SeedScript 3.0**,
  Suika3 delivers the rock-solid stability of its predecessors alongside the cutting-edge flexibility of modern technology.

## License

This software is released under the `zlib License`.

```
Suika3
Copyright (c) 2026 The Suika3 Community
```

## Support & Contact

For issues, open an issue on GitHub. For larger discussions and design
proposals, check the community channels (Discord coming soon).

## Community

Our official Discord server is coming soon! We are hard at work
creating a space for creators to connect, share, and grow. Stay tuned
for updates.

## Contributing

We welcome contributions from everyone! Whether you are fixing a bug,
improving documentation, or proposing new features, your help is what
drives **Suika3** forward.

Please see [CONTRIBUTING.md](CONTRIBUTING.md) for our contribution
guidelines and code of conduct.

Please also see [MANIFESTO.md](MANIFESTO.md) for our manifesto.

## Acknowledgements

Thanks to the Suika2 codebase, Playfield Engine contributors, and the
growing community of authors and testers who have shaped this project.
