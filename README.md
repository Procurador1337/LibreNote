# LibreNote

A simple, lightweight plain-text notepad for Linux, built with Qt6/C++.

## Reuires QT6 dev tools to build


**Ubuntu/Debian:**
```bash
sudo apt install cmake build-essential qt6-base-dev
```

**Fedora:**
```bash
sudo dnf install cmake gcc-c++ qt6-qtbase-devel
```

**Arch:**
```bash
sudo pacman -S cmake base-devel qt6-base
```

### LFS install

https://github.com/fusion809/lfs_packaging/blob/master/LibreNote/build.sh

### Build & Install
```bash
git clone https://github.com/Procurador1337/LibreNote
cd LibreNote
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
sudo make install
```

This installs the binary, a `.desktop` entry, and an icon, so LibreNote will
appear in your application menu.

## Features
- Plain-text editing with configurable font, colors, tab size, word wrap
- Fully remappable keyboard shortcuts via `~/.config/LibreNote/settings.txt`
