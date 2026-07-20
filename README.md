# Zipcode File-Structures Buffer/Indexing Project

![CI](https://github.com/dagron27/zipcode-file-structure-indexing/actions/workflows/ci.yml/badge.svg)

**Course:** `CSCI 331, File Structures, Spring 2024`

**Assignment:** `csci331-spring-24-project4`

This repository originally lived under a GitHub Classroom organization (`github.com/St-Cloud-State/csci331-spring-24-project4-dagron27`); it has since been moved to this personal GitHub account (`github.com/dagron27/zipcode-file-structure-indexing` -- note singular "structure", the actual repo name differs slightly from this project's title above). The Classroom copy remains the official course submission record and was left untouched.

## Assignment Intent

A group assignment (individual credit required working within the assigned
group); per the author, this particular submission was completed solo
despite the group framing. It extends an earlier zip-code project into a real file-structures
exercise: convert the raw zip-code spreadsheet data to CSV (including a
row-randomized variant, with a helper "random order" column stripped before
saving), process it sequentially with the buffer class from the earlier
project, then convert both files into a custom length-indicated record
format with a header record, build a buffer class that can read/unpack
records from that format, and build a primary-key index (with its own
header record) that can be built in RAM, written to disk, read back, and
used to look up individual zip codes from the command line (e.g.
`-Z56301`) -- reporting a clear not-found message when a code doesn't
exist, and the full labeled record when it does. Everything is expected to
work identically for a column-reordered version of the same data.
Documentation requirements included Doxygen-tagged source, a Doxygen-
generated PDF (UML diagrams, class dependency graphs, source links), a
user guide, a design document, and a test document.

**Confirmed implemented**, per the actual code (see Overview below):
`src/fileConverter.cpp` converts CSV to the `.zcr` length-indicated format with
a header record (`src/headerRecord.cpp`/`src/headerBuffer.cpp`); `src/buffer2.cpp`/
`src/zipcode2.cpp` read/unpack records from that format; `src/primaryKeyIndex.cpp`
builds, writes, and reads a header-recorded `.idx` file; `src/findZipcode.cpp`
parses `-Z<zipcode>` command-line flags, reports a clear message for a
zip code not in the file, and prints the full record with labeled fields
when found. `src/findZipcode.cpp`'s `main()` holds exactly a `HeaderRecord`, a
`Buffer2`, and a `PrimaryKeyIndex` after opening the data file, matching
the assignment's RAM-contents constraint. Multiple data-file variants are
present (`us_postal_codes`, `_place`, `_ROWS_RANDOMIZED`, `_rr_state`),
consistent with the row-randomized and column-reordered repeat runs the
assignment asks for.

**Update: the documentation deliverables have since been added.**
`docs/Report.pdf` (Doxygen-generated -- confirmed by its `@starID`
javadoc-style tags, same signature as this portfolio's other
Doxygen-generated PDFs), `docs/Design Document.pdf`, `docs/Test
Document.pdf`, and `docs/User Document.pdf` are all now present. Their
content hasn't been reviewed in detail yet (structure, accuracy against
the actual code, formatting); that's a separate pass. No `Doxyfile` is
committed, so the Doxygen PDF isn't independently regeneratable from this
repository as-is.

## Overview

This is a C++ course project on file structures: it implements custom buffered file I/O and a primary-key index over a flat file of US zip code records (zip code, place name, state, county, latitude, longitude), sourced from `data/us_postal_codes.csv` (roughly 40,933 records).

The project evolved across two assignments (Project 3 and Project 4), and the repository contains both generations of code side by side:

- **Project 3 (original)** — `src/zipcode.h/.cpp`, `src/buffer.h/.cpp`, `src/proj3.cpp`. Reads a plain `.csv` file line-by-line through a simple windowed `Buffer` (reads/writes `Zipcode` records directly against a `.csv`, no header, no index) and reports the easternmost/westernmost/northernmost/southernmost zip code per state.
- **Project 3 updated / Project 4** — `src/zipcode2.h/.cpp`, `src/buffer2.h/.cpp`, `src/headerRecord.h/.cpp`, `src/headerBuffer.h/.cpp`, `src/primaryKeyIndex.h/.cpp`, `src/proj3Updated.cpp`, `src/fileConverter.cpp`, `src/findZipcode.cpp`. Introduces a custom binary-ish text file format:
  - **`.zcr`** ("Zipcode Record" file) — a 450-byte fixed-size text header (`HeaderRecord`/`HeaderBuffer`) describing the file structure, followed by length-indicated records (each line is `<byte-length> <packed-record>`), packed/unpacked by `Zipcode2`/`Buffer2`.
  - **`.idx`** — a primary-key (zip code) index file, itself with its own 450-byte header, mapping `key,offset` pairs into the `.zcr` file so a record can be located with a single seek instead of a linear scan (`PrimaryKeyIndex`).
  - `src/fileConverter.cpp` converts a `.csv` into a `.zcr` (writes the header, then streams packed/length-indicated records).
  - `src/findZipcode.cpp` opens a `.zcr` file, rebuilds/verifies its index (`src/primaryKeyIndex.cpp`), and looks up individual zip codes either from `-Z<zipcode>` command-line flags or interactive stdin input, printing the matching record(s).

In short: this is a hand-rolled, direct-access file-structures exercise (headers, length-indicated records, buffering, and a primary key index) built on top of a real-world zip code dataset, not a general-purpose library.

## Dependencies

- A C++ compiler with C++11 (or later) support. All build evidence in this repo uses `g++` on Linux (the recorded terminal sessions show a devcontainer/Codespace at `/workspaces/csci331-spring-24-project4-dagron27`).
- No third-party libraries — only the C++ standard library (`<fstream>`, `<sstream>`, `<string>`, `<vector>`, `<map>`, `<iostream>`, `<iomanip>`, `<cstring>`, `<cerrno>`).
- No build system file (Makefile/CMakeLists.txt) is present in the repository; see Known Issues.

## Environment Setup

There is no Makefile or CMakeLists.txt in this repository. The exact build commands were recovered from the recorded terminal transcripts (`docs/transcripts/req3_script.txt`, `docs/transcripts/req4_script.txt`, `docs/transcripts/req7_script.txt`, `docs/transcripts/req8_script.txt`), which are `script(1)` session recordings (they contain raw terminal escape/backspace sequences, not clean shell scripts). The compile commands actually used were:

All commands below are run from the repository root (not from inside
`src/`). The data files (`us_postal_codes*`, `test1/2/3.*`) live under
`data/`, and every binary opens them via a relative path rooted at the
repository root (e.g. `data/us_postal_codes.zcr`) -- so the working
directory when running any of these binaries always needs to be the
repository root, not `data/` itself.

```sh
# proj3_original — Project 3, CSV-based, no header/index
g++ -o proj3_original src/proj3.cpp src/buffer.cpp src/zipcode.cpp

# proj3_updated — Project 3 logic re-targeted at the .zcr format
g++ -o proj3_updated src/proj3Updated.cpp src/buffer2.cpp src/zipcode2.cpp

# converter — CSV to ZCR file converter
g++ -o converter src/fileConverter.cpp src/headerRecord.cpp src/headerBuffer.cpp src/buffer.cpp src/zipcode.cpp

# proj4_norm / proj4_norm_place / proj4_rand / proj4_rand_state
# — all four are the SAME source combination, compiled four separate times:
g++ -o proj4_norm src/findZipcode.cpp src/primaryKeyIndex.cpp src/headerRecord.cpp src/headerBuffer.cpp src/buffer2.cpp src/zipcode2.cpp
```

Note: `src/findZipcode.cpp` hard-codes the input filename `"data/us_postal_codes.zcr"` (see `src/findZipcode.cpp:53`) rather than accepting it as a parameter. The transcripts show the workflow was to rename the desired data-file variant (`us_postal_codes_place.*`, `us_postal_codes_ROWS_RANDOMIZED.*`, `us_postal_codes_rr_state.*`) to `us_postal_codes.zcr`/`.idx` (within `data/`) and rebuild/rename the binary, which is why four near-identical `proj4_*` binaries exist instead of one binary taking a filename argument. See Known Issues for a suggested fix.

To reproduce a build locally on Linux/WSL/macOS:

```sh
g++ -std=c++11 -o findzipcode src/findZipcode.cpp src/primaryKeyIndex.cpp src/headerRecord.cpp src/headerBuffer.cpp src/buffer2.cpp src/zipcode2.cpp
./findzipcode -Z501
```

Running `converter` first (against `data/us_postal_codes.csv`, entered at
its "Enter the name of the CSV file:" prompt) regenerates
`data/us_postal_codes.zcr`; `findZipcode`/`proj4_*` then build
`data/us_postal_codes.idx` from that `.zcr` file automatically on
startup.

## Repository Organization

The original submission had every `.cpp`/`.h` source file, the four
`req*_script.txt` terminal transcripts, and `docs/Report.pdf`/`Design
Document.pdf`/`Test Document.pdf`/`User Document.pdf` all at the
repository root, alongside a committed `.vscode/` editor-config folder.
Source files were grouped into `src/`, the four PDF deliverables and
the transcripts into `docs/` (transcripts later separated further into
`docs/transcripts/`, see below), and `.vscode/` was removed entirely as
editor-specific local config with no reason to be tracked. None of this
required source changes -- unlike the `data/` move described next.

The 18 data files (`us_postal_codes*.csv/.idx/.zcr`, `test1/2/3.csv/.zcr`)
originally sat loose at the repository root. They have been moved into
`data/` to match this portfolio's general code/data separation
convention. This was a source change, not just a file move:
`src/findZipcode.cpp` hard-codes the data file it opens
(`"us_postal_codes.zcr"`, now `"data/us_postal_codes.zcr"`), and both
`src/findZipcode.cpp` and `PrimaryKeyIndex::createIndex()`
(`src/primaryKeyIndex.cpp`) open the `.idx` index file using the bare
filename persisted inside the `.zcr`'s own header record (e.g. "Index
File Name: us_postal_codes.idx;") -- that persisted value has no
directory component, so both call sites now prefix it with `"data/"`
before opening it. Every binary is still invoked from the repository
root (not from inside `data/`), matching the original "everything opens
via a path relative to the working directory" design; see Environment
Setup above for exact commands.

## Known Issues

### Dead Code

- **`src/headerRecord.h:194` / `src/headerRecord.cpp:194`** — `fields` is a `std::vector<std::pair<std::string,std::string>>` sized once in the constructor (based on the `numFields` argument, default 6) and never resized afterward. `setField()` (`src/headerRecord.cpp:131-139`) does bounds-check against `fields.size()`, and `Unpack()` (`src/headerRecord.cpp:236-242`) is now bounds-checked too (see Security/Robustness Findings below — Fixed). This item remains dead-code-adjacent because the mismatch between `fieldsPerRecord` (file-driven) and `fields.size()` (constructor-driven) is masked in normal operation (every call site happens to construct the object with the matching field count).
- **`src/headerBuffer.cpp:61-71`** — `ReadHeader()` hard-codes reading exactly `16` lines regardless of the actual `fieldsPerRecord` value it is about to parse. This "16" is only correct because every header in the codebase happens to describe 6 fields (9 metadata lines + 6 field lines + 1 primary-key line). It is unused/unreachable flexibility: the code implies headers of arbitrary field count are supported (`HeaderRecord(int numFields = 6)`, `HeaderRecord indexHeader(2)`), but `ReadHeader()` was never wired to actually handle a non-6-field header being read back (the 2-field index header is only ever written, never read via `HeaderBuffer::ReadHeader`).
- **`src/primaryKeyIndex.cpp:74-76`** — the `else` branch for non-`"Int"` primary key types is an empty comment (`// Handle error: unknown type`) with no actual handling; `createIndex()` will still report success even though no index entries were generated.
- **`src/primaryKeyIndex.h:69`** — `headerOffset` is initialized to a literal `450` and then immediately overwritten by `setHeaderRecord()` before first real use in every current code path; the initializer is vestigial.
- `src/proj3.cpp` and `src/proj3Updated.cpp` are near-duplicates of each other (same state-extremes logic, one against `.csv`/`Buffer`, one against `.zcr`/`Buffer2`) with no shared helper — acceptable for incremental coursework submissions, but effectively redundant code if the project were to continue.

### Security / Robustness Findings

- **[Fixed, PII] PII exposure in all four `docs/` PDFs.** `Design
  Document.pdf`, `Report.pdf`, `Test Document.pdf`, and `User
  Document.pdf` all contained a PII exposure, found and remediated.
  True-redacted via PyMuPDF (search + black-fill annotation +
  apply-redactions), verified via re-extracted text showing zero
  remaining hits and an intact page count in all four files.
- **[Fixed] Unvalidated file-driven vector index — `src/headerRecord.cpp:207,212,229,234,236-242`.** `HeaderRecord::Unpack()` parses `headerRecordSize`, `recordSizeInteger`, `recordCount`, and `fieldsPerRecord` directly from file content via `std::stoi()`, then used `fieldsPerRecord` as a loop bound to write into `fields[i]` (`operator[]`, no bounds check). If a `.zcr`/`.idx` file's header is corrupted or maliciously crafted to claim more fields than the `HeaderRecord` object was constructed with (`fields.size()`), this was an out-of-bounds vector write — undefined behavior, and a plausible memory-corruption vector on corrupted data files, not just a crash. **Fix applied:** each field's name/type is now read into local strings first, and the write into `fields[i]` only happens when `i < fields.size()`; if `fieldsPerRecord` (from the file) exceeds the number of allocated field slots, the extra field data is still consumed from the stream (so parsing of the remaining header, e.g. Primary Key, stays aligned) but is not written out of bounds, and a warning is printed to `std::cerr`. Well-formed headers (`fieldsPerRecord == fields.size()`, the only case that occurs in this codebase today) are unaffected.
- **[Fixed] Unhandled exception on malformed filename input — `src/fileConverter.cpp:62`.** `csvFileName.substr(csvFileName.rfind("."))` — if the user-entered filename contains no `.` character, `rfind` returns `std::string::npos`, and `substr(npos)` threw `std::out_of_range` with no surrounding `try/catch`. This was not theoretical: it was reproduced verbatim in the project's own recorded test transcript, `docs/transcripts/req4_script.txt:153-158` — entering the filename `sfsdfsdfsdfsdf` crashed the program (`terminate called after throwing an instance of 'std::out_of_range'` / `Aborted (core dumped)`). **Fix applied:** the `.` position is now looked up once and checked against `std::string::npos` before `substr` is called; a missing `.` now falls through to the existing "Error: Input file is not a CSV file." message and a clean `return 1`, instead of throwing.
- **Same unchecked-`stoi`/`substr` pattern recurs elsewhere.** `src/headerRecord.cpp` (4 separate `std::stoi(tmp)` calls in `Unpack()`, none in a `try/catch` at the call site — only `src/headerBuffer.cpp`'s caller wraps the whole `Unpack()` call in a `try/catch`, so a throw is caught there, but silently turns into "FAILED TO READ HEADER" without indicating which field was bad), and `src/buffer2.cpp:114/161` (`std::stoi(process_line.substr(0, space_pos))` — if a line contains no space, `find(' ')` returns `npos` and the same `substr(0, npos)` pattern is used, which is actually safe for the two-argument `substr(pos, count)` form since `count` is clamped, but if `space_pos` were used as a starting `pos` instead — as it is in `src/buffer2.cpp:118/165`, `substr(space_pos + 1, ...)` — an all-non-space line would make `space_pos + 1` wrap or exceed the string length and throw). Severity: Low-Medium (robustness against corrupted `.zcr` data rows). Fix-it: centralize a "safe integer parse" helper that checks `find()` results and catches `std::invalid_argument`/`std::out_of_range` at every call site, rather than relying on outer `try/catch` blocks that only exist in some callers.
- **Same pattern also present in `Buffer2::Unpack` itself — `src/buffer2.cpp:212,220,222`.** `zipcode.setZipcode(std::stoi(field))`, `setLat(std::stod(field))`, and `setLon(std::stod(field))` parse each `.zcr` record's Zip Code/Lat/Long fields with no `try/catch` anywhere in this function or its traced callers. Unlike the two call sites above (which run during index construction), this one runs on *every* record read by `findZipcode`/`proj4_*` -- i.e. the main successful-path record decode, not just an edge case. A corrupted or hand-edited `.zcr` data row with a non-numeric Zip/Lat/Long field would crash the program the same way the already-fixed `fileConverter.cpp` bug used to (see above). Same severity and same fix-it plan as the finding above -- fold this call site into the same centralized safe-parse helper when that work happens. Not fixed directly here since no C++ compiler was available locally to verify a change to this codebase's core record-decode path.
- **No path/extension sanitization on user-supplied filenames.** `src/fileConverter.cpp:55` (`std::cin >> csvFileName`), `src/proj3.cpp:31`, and `src/proj3Updated.cpp:32` (`std::getline(std::cin, inputFileName)`) take a filename directly from stdin and pass it straight to `std::fstream`/`Buffer`/`Buffer2` constructors with no validation beyond the `.csv` extension check in `src/fileConverter.cpp`. There is no command-injection risk (no `system()`/`popen()`/`exec*` calls exist anywhere in the codebase — confirmed by search), but arbitrary relative/absolute paths are opened without restriction. Severity: Low (this is a single-user CLI academic tool, not a service; flagged for completeness since file I/O is unvalidated).
- **`src/fileConverter.cpp:96-103` `openFile()` calls `exit(1)` directly on failure**, bypassing destructors/RAII cleanup for any already-open file handles in the caller. Not memory-unsafe in practice (the OS reclaims file descriptors on process exit), but it is an abrupt-termination pattern repeated in several files (`src/buffer.cpp:16-19` does the same in the `Buffer` constructor) that makes the code harder to test and to embed as a library. Severity: Low.
- **No unsafe C string functions found.** A repository-wide search for `strcpy`, `strcat`, `sprintf`, `gets`, `memcpy`, `malloc`/`free`, `scanf`, and raw `new[]`/`delete[]` found no matches — all string handling goes through `std::string`/`std::stringstream`, and the only C library calls present are `strerror()` (`src/findZipcode.cpp:57`, read-only/safe) and `strncmp()` (`src/findZipcode.cpp:134`, bounded, safe). No `system()`, `popen()`, `exec*`, or other command-execution calls exist anywhere in the `.cpp` files.
- **Integer parsing of command-line zip codes — `src/findZipcode.cpp:135`.** `std::stoi(argv[i] + 2)` on the `-Z<digits>` argument is not wrapped in a `try/catch`; a malformed flag (e.g. `-Zabc`) will throw `std::invalid_argument` and crash the program before any record lookup happens. Severity: Low (CLI argument, not file/network-controlled data, but still an easy crash to trigger).

### Committed Compiled Binaries (Hygiene) — [Fixed]

Verified via `file`: **all seven** of `proj3_original`, `proj3_updated`, `proj4_norm`, `proj4_norm_place`, `proj4_rand`, `proj4_rand_state`, and `converter` were genuine compiled artifacts — `ELF 64-bit LSB pie executable, x86-64, ... dynamically linked, ... for GNU/Linux, not stripped` — not scripts or text files. They ranged from ~87 KB to ~164 KB each. `docs/transcripts/req3_script.txt`, `docs/transcripts/req4_script.txt`, `docs/transcripts/req7_script.txt`, and `docs/transcripts/req8_script.txt` are, by contrast, genuinely plain text (`script(1)` terminal-session transcripts), not binaries, and were left in place.

Compiled executables generally should not be committed to source control: they bloat repository size and history, are platform/toolchain-specific (Linux x86-64 binaries are useless on other platforms and can go stale relative to the source), and can be regenerated trivially from the `g++` commands documented above.

**Fix applied:**
1. All seven binaries (`proj3_original`, `proj3_updated`, `converter`, `proj4_norm`, `proj4_norm_place`, `proj4_rand`, `proj4_rand_state`) were deleted from the working tree.
2. A `.gitignore` was added at the repository root. Since these binaries have no file extension, it lists their exact filenames explicitly, plus common build-output patterns (`*.o`, `*.obj`, `*.out`, `*.exe`, `*.a`, `*.so`, `*.dylib`):
   ```
   proj3_original
   proj3_updated
   converter
   proj4_norm
   proj4_norm_place
   proj4_rand
   proj4_rand_state
   *.o
   *.obj
   *.out
   *.exe
   *.a
   *.so
   *.dylib
   ```
3. This cleanup happened in the local working copy before this repository's current git history began (its first commit already reflects the binaries as absent) -- confirmed via `git log --all --diff-filter=A -- proj3_original proj3_updated converter proj4_norm proj4_norm_place proj4_rand proj4_rand_state`, which returns no results: these filenames were never added in any commit that exists in this repository's actual history.
4. Not done: a real `Makefile` was not added. The binaries remain reproducible on demand from the `g++` invocations documented under Environment Setup.

The large data files (`data/us_postal_codes*.csv/.idx/.zcr`, `data/test1/2/3.csv/.zcr`) are left out of this recommendation — they are the project's actual input/generated data, not build artifacts, though the repository's total size (several files >1.8 MB each, with multiple near-duplicate variants: base, `_place`, `_ROWS_RANDOMIZED`, `_rr_state`) is itself worth a future look if repository size becomes a concern.

## Status

Coursework artifact — CSCI 331 (File Structures), Spring 2024, St. Cloud State University, originally submitted through GitHub Classroom (`github.com/St-Cloud-State/csci331-spring-24-project4-dagron27`) and since moved to this personal repository (`origin` = `github.com/dagron27/zipcode-file-structure-indexing`). Not maintained as an active project. A GitHub Actions CI workflow (`.github/workflows/ci.yml`, see the badge at the top of this README) compiles all four build targets and runs runtime smoke tests on every push -- there is still no automated test suite beyond that, and no build system beyond the ad hoc `g++` commands recovered from terminal-session transcripts.
