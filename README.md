# ft webserv

## Description

This project is a web server. It is able to handle HTTP requests and responses, 
and is able to serve static files. It is also able to handle python and binary cgi scripts.

## Installation

### Requirements

- C++ compiler
- Make
- Python 3.8
- Python3-dev

### Build

To build the project, run `make` in the root directory of the project.

### Run

To run the project, run `./webserv` and pass it a configuration file as an argument.

example: `./webserv default.conf`

## Configuration

The configuration file is a simple text file that contains the configuration for the server.

### Syntax

The configuration file is inspired by the nginx configuration file syntax.
Each block starts with a line containing the name of the block, followed by a colon, 
and then a list of arguments. The arguments are separated by spaces. The block ends when a new block starts.

### Blocks

#### server

The server block is the root block of the configuration file. It contains the configuration for the server.

##### Arguments

- `host` - The host to listen on. Defaults to `localhost`.
- `port` - The port to listen on. Defaults to `80`.
- `error_page` - The path to the error page. Defaults to `error.html`.
- `server_name` - The server name. Defaults to `localhost`.
- `root` - The root directory. Defaults to `./`.
- `index` - The index file. Defaults to `index.html`.
- `client_max_body_size` - The maximum size of the request body.
- `autoindex` - Whether to enable autoindex. Defaults to `off`.
- `cgi` - Whether to enable cgi. Defaults to `off`.
- `cgi_folder` - The folder of the single cgi script.

#### location

The location block contains the configuration for a location.

##### Arguments

- `path` - The path of the location.
- `root` - The root directory. Defaults to the root directory of the server.
- `limit_except` - The methods that are allowed for this location.
- `cgi_lock` - flag to indicate if the route is a cgi route.

#### Contributors

<p align="center">
<table>
  <tr>
    <td>
      <a href="https://github.com/andersonhsporto">
        <img src="https://avatars.githubusercontent.com/u/47704550?v=4" width="140px" alt="Anderson Porto avatar"/><br>
        <sub><b>Anderson Porto</b></sub>
      </a>
    </td>
    <td>
      <a href="https://github.com/DaviPrograme">
        <img src="https://avatars.githubusercontent.com/u/56012877?v=4" width="140px" alt="Davi Moreira avatar"/><br>
        <sub><b>Davi Moreira</b></sub>
      </a>
    </td>
    <td>
      <a href="https://github.com/GitFlaviobc">
        <img src="https://avatars.githubusercontent.com/u/46327033?v=4" width="140px" alt="Flavio Bonini Campos  avatar"/><br>
        <sub><b>Flavio Bonini Campos</b></sub>
      </a>
    </td>
  </tr>
</table>
