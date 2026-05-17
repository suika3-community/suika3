List of Feature Requests
========================

This file manages the list of feature requests proposed on GitHub Issues and other channels.

---
Template

## Add a feature to run game by double-clicking the `start.novel` file

| Item          | Details                                                                 |
|---------------|-------------------------------------------------------------------------|
| ID            | FR-20260420-001                                                         |
| Priority      | <High / Mid / Low>                                                      |
| Status        | <**Proposed** / Discussing / Accepted / WaitForImpl / Testing / Done>   |
| Target LTS    | 27.07 LTS                                                               |
| Refereneces   | [GitHub Issue #123](https://...)                                        |
| Created       | 20 April 2026                                                           |
| Updated       | 28 April 2026                                                           |
| Requested By  | <List of the origins>                                                   |

### Description

A feature to lauch a game by double-clicking a `start.novel` file on File Explorer.

### Outline of The Discussion

Background:
- To put a copy of `suika3.exe` file into a game folder is considered
  an extra step for file management on a game development because
  binary files are usually excluded in the Git repository.
- Launching a game by opening a `start.novel` file is a usuful feature
  in this context.

Concerns:
- None

### Notes on Progress

- 2026-04-28: Agreed to implement in the next feature-add window for 27.07 LTS.

### Related Commits

* d0aaa Add file extension association

---

## Separating Screenshot Size from Display Size

| Item          | Details                                                                 |
|---------------|-------------------------------------------------------------------------|
| ID            |                                                                         |
| Priority      | <High / Mid / Low>                                                      |
| Status        | <**Proposed** / Discussing / Accepted / WaitForImpl / Testing / Done>   |
| Target LTS    | 27.07 LTS                                                               |
| Refereneces   | [GitHub Issue #26]([https://...)](https://github.com/awemorris/suika3/issues/26)                                        |
| Created       | 28 April 2026                                                           |
| Updated       | 28 April 2026                                                           |
| Requested By  | <List of the origins>                                                   |

### Description

Data Layer: Continue using save.thumb.width / save.thumb.height to control the actual screenshot resolution.
Display Layer: Allow explicit declaration of width and height within the Save Button (or UI component).
Rendering Logic: The engine should automatically scale (stretch/shrink) the "Original Thumbnail" to fit the declared "Display Size".

### Outline of The Discussion

Background:


Concerns:
- None

### Notes on Progress

- 2026-04-28: Agreed to implement in the next feature-add window for 27.07 LTS.

### Related Commits

* d0aaa Add file extension association

---

## Suggest renaming the issue to: Save data structure and additional support for image saves

| Item          | Details                                                                 |
|---------------|-------------------------------------------------------------------------|
| ID            | FR-20260518-001                                                         |
| Priority      | <Mid>                                                                   |
| Status        | <Accepted>                                                              |
| Target LTS    | 27.07 LTS                                                               |
| Refereneces   | [GitHub Issue #33](https://github.com/awemorris/suika3/issues/33)       |
| Created       | 18 May 2026                                                             |
| Updated       | 18 May 2026                                                             |
| Requested By  | @lalalll-lalalll                                                        |

---

I would like to supplement the details of this feature. Please refer to the discussion in issues #26. We propose two distinct modes for save files: `System Mode` and `User Mode` for the moment (To be discussed).

#### 1. System Mode

This mode treats save data as part of the system cache. It is invisible to the user to maintain program robustness and portability.

- **File Naming**: The filename is a hash value with **no file extension**.
- **Example**:
  ```text
  3030312e736176
  3030322e736176
  ...
  ```
- **Requirement**: Since files are not user-readable, the engine requires a solution for **Save Import/Export**.

#### 2. User Mode

This mode treats save data as user cache. It is visible to the user, allowing them to manage save files manually (e.g., via copy/paste).

- **File Naming**:
  - Filenames should be based on **Save Slot Names** (one file per slot).
  - A separate main save file is needed for global data (e.g., CG gallery status,scenario locks).
- **File Extension**: `.sav` is recommended (for visual distinction only; internal logic remains unchanged).
- **Example**:
  ```text
  save_00.sav
  save_01.sav
  Sys.sav
  ...
  ```

#### 3. Extension: PNG Saves

An optional feature based on User Mode to support saving files as images.

- **Feature**: Allows saving user mode files as PNG or other image formats for visual identification.
- **Configuration**: Image dimensions are controlled by `save.thumb.width` / `save.thumb.height`.
- **Example**:
  ```text
  save_00.png
  save_01.png
  Sys.sav
  ...
  ```
