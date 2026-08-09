# Simple FTP Server & Client

FTP implementation in C for basic file transfer operations.

## Features

- Upload and download files
- List server/client directories
- Simple command-line interface

## Build

```bash
make
```

## Usage

### Start Server
```bash
# Run from server directory (has test files)
cd bin/server
./ftp_server [port]

# Or use make target
make run-server
```
Default port: 8080

### Connect Client
```bash
# Run from client directory (has test files)
cd bin/client  
./ftp_client <server_ip> [port]

# Or use make target
make run-client
```

### Commands
- `ls` - List server files
- `lls` - List local files  
- `get <filename>` - Download file
- `put <filename>` - Upload file
- `quit` - Exit

## Example Session
```bash
# Terminal 1 - Start server
make run-server

# Terminal 2 - Connect client
make run-client
ftp> ls                    # See server test files
ftp> get document.txt      # Download from server
ftp> lls                   # See local client files
ftp> put upload_me.txt     # Upload to server
ftp> quit
```

## Test Files
The build process creates test files in both directories:
- **Server files**: `server_readme.txt`, `document.txt`, `server.log`, `script.sh`

- **Client files**: `client_readme.txt`, `upload_me.txt`, `config.conf`, `data.txt`
