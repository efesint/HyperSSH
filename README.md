# HyperSSH

A small C++ menu for fast SSH connections. You store your servers once in a YAML
file and then just type an alias instead of `ssh user@host -p port`.

![HyperSSH in action](images/screen.png)

## Requirements

- Linux, a C++17 compiler (`g++`)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp) — Debian/Ubuntu: `sudo apt install libyaml-cpp-dev`
- an `ssh` client

## Install

The project **must** live in `~/HyperSSH`. The config path is hardcoded to
`~/HyperSSH/servers.yaml`, so clone it exactly there:

```sh
git clone https://github.com/efesint/HyperSSH.git ~/HyperSSH
cd ~/HyperSSH
make
sudo make install
```

> **Important:** if there is no `~/HyperSSH/` folder in your home directory,
> HyperSSH cannot save or read its config and will fail. If you keep the source
> somewhere else, at least create the folder and put `servers.yaml` in it:
>
> ```sh
> mkdir -p ~/HyperSSH
> ```

## Usage

```sh
hyperssh
```

1. `[2]` — create `~/HyperSSH/servers.yaml` (restart the program afterwards)
2. `[1]` — pick a server by alias and connect

## Configuration

`servers.yaml`:

```yaml
servers:
 - alias: router
   name: "Samsung"
   host: 192.168.10.1
   user: root
   port: 22
 - alias: server
   name: "HP"
   host: 192.168.10.146
   user: admin
   port: 22
```

Notes:

- `[2]` **rewrites the whole file** and writes a single server — add more servers
  by editing `servers.yaml` by hand
- `host` must be a plain IPv4 address (no hostnames, no leading zeros like `01`)
- always enter the `port` (`22` for standard): pressing Enter on an empty port
  crashes the program (known bug)
- an older Ruby version lives in `ruby-v1.1/`

## License

GNU General Public License v3.0
