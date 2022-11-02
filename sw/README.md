


# Compilation


### For linux on linux

```
go build .
```

### For windows on linux

```
sudo apt-get install gcc-multilib gcc-mingw-w64
GOOS=windows GOARCH=amd64 CGO_ENABLED=1 CC=x86_64-w64-mingw32-gcc go build .
```
