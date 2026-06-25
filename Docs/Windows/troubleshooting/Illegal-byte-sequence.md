# Troubleshooting: `std::filesystem_error` (Illegal byte sequence)

## Issue Description
When running the `paxon` command in the cmd or powershell, sopsh, etc. the application crashes with the following error:

```bash
terminate called after throwing an instance of 'std::filesystem::__cxx11::filesystem_error'
  what():  filesystem error: Cannot convert character sequence: Illegal byte sequence
```

## Solutions
+ Move the project to a pure english path (Recommended)
+ Change your windows username or profile folder to english
+ Enable windows UTF-8 global support