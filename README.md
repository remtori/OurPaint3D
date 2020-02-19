# Our Paint 3D

Hộ trợ Windows và Linux có gnome-desktop (hoặc có chương trình zenity, cần thiết cho giao diện chọn file)

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