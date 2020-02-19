# Our Paint 3D

Hộ trợ Windows và Linux có gnome-desktop (hoặc có chương trình zenity, cần thiết cho giao diện chọn file)

## Requirements

### Windows

Visual Studio với build tools cho C/C++

### Linux

Cần các package: libx11-dev mesa-common-dev

- libx11: Thư viện X11 cần để chạy `GLFW3`

- mesa-common-dev: Thư viện OpenGL để chạy `glad`

Có thể cài bằng lệnh:
```
sudo apt-get install libx11-dev mesa-common-dev
```

## Generate project

### Windows

```
vendor\premake5.exe vs2019
```

Thay `2019` bằng phiên bản visual studio bạn dùng

### Linux

```
vendor/premake5 gmake2
```

## Build project

File chương trình sẽ được chứa trong thư mục `bin`

### Windows

Build bằng visual studio phiên bản tương ứng

### Linux

```
make
```