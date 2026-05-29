# MacUSBMon

MacUSBMon is a C++17 macOS USB capture tool skeleton built with Dear ImGui docking, GLFW3, Metal, and IOKit-oriented architecture. It is intended to build on macOS because the entry point uses Cocoa, CAMetalLayer, Metal, and Objective-C++.

The current implementation uses a simulated capture worker so the GUI, packet model, ring buffer, parser, HID decoder, and PCAP export path can be developed before the DriverKit/DEXT transport is connected.

## Dependencies

```bash
brew install cmake ninja glfw
```

Fetch UI dependencies:

```bash
git clone --branch docking https://github.com/ocornut/imgui.git third_party/imgui
git clone https://github.com/epezent/implot.git third_party/implot
```

The repository keeps `third_party/` as a destination for those dependencies; the actual upstream source files are not vendored here. The Dear ImGui `docking` branch is required because the app enables dock spaces.

## Build

```bash
cmake -S . -B build -G Ninja
cmake --build build
```

## Run

```bash
./build/bin/MacUSBMon.app/Contents/MacOS/MacUSBMon
```

## Universal Binary

```bash
lipo -info ./build/bin/MacUSBMon.app/Contents/MacOS/MacUSBMon
```

## Adhoc Signing

```bash
codesign --force --deep --sign - ./build/bin/MacUSBMon.app
```

## GitHub Actions macOS Package

This repository includes `.github/workflows/macos-build.yml`.

The workflow:

- runs on macOS GitHub-hosted runners
- uses `macos-15-intel` for `x86_64` and `macos-15` for `arm64`
- installs `cmake`, `ninja`, and `glfw`
- clones Dear ImGui and ImPlot into `third_party/`
- builds release `.app` packages for `x86_64` and `arm64`
- adhoc signs the app
- uploads zipped artifacts
- publishes both zip files on tags like `v0.1.0`

Manual run:

1. Open the repository on GitHub.
2. Go to Actions.
3. Choose `macOS Build`.
4. Click `Run workflow`.

Release build:

```bash
git tag v0.1.0
git push origin v0.1.0
```

## Current Features

- Dear ImGui docking UI with Devices, Packets, and Hex View panels.
- Simulated high-frequency USB packet producer.
- Single-producer/single-consumer ring buffer.
- Packet table with clipping for large captures.
- Basic HID boot keyboard report labeling.
- PCAP export to `/tmp/macusbmon_capture.pcap`.

## Next Steps

1. Replace `src/usb/dext_client.cpp` simulation with real DriverKit/DEXT communication:
   - `IOServiceGetMatchingService`
   - `IOConnectCallMethod`
   - shared memory via `mmap`
   - lock-free ring buffer
   - notification port

2. Extend Wireshark compatibility:
   - USBPcap pseudo header
   - URB direction
   - setup packet metadata
   - transfer status

3. Improve HID decoding:
   - Boot keyboard
   - NKRO
   - mouse reports
   - gamepad usages
   - HID descriptor parser

4. Add memory and throughput controls:
   - payload small-vector
   - string interning
   - lazy summaries
   - bounded capture sessions
