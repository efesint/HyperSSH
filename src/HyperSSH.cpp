/*
 * HyperSSH 
 * Copyright (C) 2026 efesint ('ts)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Third-party: uses yaml-cpp (https://github.com/jbeder/yaml-cpp),
 * MIT License, Copyright (c) 2008-2015 Jesse Beder.
 */



#include <iostream>
#include <yaml-cpp/yaml.h>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <filesystem>

std::string rd = "\033[31m";
std::string wh = "\033[0m";


  void banner() {
  std::cout << rd;
  std::cout << R"(
  ___ ___                              _________ _________ ___ ___  
 /   |   \ ___.__.______   ___________/   _____//   _____//   |   \ 
/    ~    <   |  |\____ \_/ __ \_  __ \_____  \ \_____  \/    ~    \
\    Y    /\___  ||  |_> >  ___/|  | \/        \/        \    Y    /
 \___|_  / / ____||   __/ \___  >__| /_______  /_______  /\___|_  / 
       \/  \/     |__|        \/             \/        \/       \/  
                  === v2.0 ===
  )" << std::endl;
  std::cout << wh;
  }

  bool check_port(int port){
    return (port >= 1 && port <= 65535);
  }
      
  std::string home() {
    const char* home = std::getenv("HOME");
    if (home == nullptr) {
        return "/home";   // if /home doesnt work
    }

    return std::string(home);
  }

std::string path = home() + "/HyperSSH/servers.yaml";

  bool check_ipv4(std::string ip){

    std::stringstream ss (ip);
    std::string part; 
    int count = 0;

      while (std::getline(ss, part, '.')) {
        count++;
          if (part.empty()) {
           return false;
          }  
          if (part.size() > 1 && part[0] == '0') {
            return false;
          }
          for(char c : part) {
             if(c < '0' || c > '9') { // i wanna cry
               return false;
             }
          } 
         if (part.size() > 3) {
          return false;
         }
         int num = std::stoi(part);
         if (num > 255) return false;
      }
return count == 4;
}
  void first_time() {
      int port = 22; 
      std::string alias, user, name, host;

      std::cout << rd << "WARNING: " << wh << "this change will fully rewrite config" << std::endl;
      std::cout << "If you want to add 2 or more servers edit the servers.yaml yourself. " << std::endl;
    
      std::cout << "Alias: ";   std::getline(std::cin, alias);
      std::cout << "User: ";    std::getline(std::cin, user);
      std::cout << "Name: ";    std::getline(std::cin, name);
      std::cout << "Host: ";    std::getline(std::cin, host);
      std::cout << "Port (default 22): ";
      std::string port_str;
      std::getline(std::cin, port_str);

      if (alias.empty()) {
        std::cout << "Alias cannot be empty!\n";
        return;
      }

      if (!check_ipv4(host)) {
        std::cout << "Invalid IPv4 address!" << std::endl;
        return;
      }
      port = std::stoi(port_str);
        if (!check_port(port)) {
          std::cout << "Invalid port (1-65535)!\n";
          return;
       }

      std::ofstream ffile(path);

       if(!ffile) {
        std::cerr << "Cannot save config to " << path << std::endl;
        return;
       }

     ffile << "servers:\n"
           << " - alias: " << alias << "\n"
           << "   name:  " << name << "\n"
           << "   host: " << host << "\n"
           << "   user:  " << user << "\n"
           << "   port: " << port << "\n";
     
    std::cout << "Done. Restart the program." << std::endl;
  } 




  void connect() {
     std::ifstream f(path);
       if(!f) {
       std::cout << rd << "cannot find file 'servers.yaml'. run [2] first" << wh <<std::endl;

       }
    
    YAML::Node config = YAML::LoadFile(path);
    YAML::Node servers = config["servers"];

    std::cout << "Available servers:" << std::endl;
      for (auto s : servers) {
      std::cout << "  " << s["alias"].as<std::string>()
                  << " (" << s["host"].as<std::string>() << ")\n";
    }

    std::cout << "Enter alias: ";
    std::string alias;
    std::getline(std::cin, alias);

    for (auto s : servers) {
        if (s["alias"].as<std::string>() == alias) {
            std::string user = s["user"].as<std::string>();
            std::string host = s["host"].as<std::string>();
            int port = s["port"].as<int>();

            std::string cmd = "ssh ";
            if (port != 22) cmd += "-p " + std::to_string(port) + " ";
            cmd += user + "@" + host;

            std::cout << "Running: " << cmd << "\n";
            system(cmd.c_str());
            return;
        }
    }

    std::cout << "Server not found.\n";
  }

  int list_servers() {
   std::ifstream f(path);

    if(!f.is_open()) {
     std::cerr << "Cannot open file. Check if servers.yaml is exist" << std::endl;
     return 1;
    }
   std::cout << f.rdbuf() << std::endl;
 
   return 0;
   
  }

  int help() {
   std::cout << " -h | --help = list of all commands" << std::endl;
   std::cout << " -m | --menu = return menu" << std::endl;
   std::cout << " -l | --list = list of all servers" << std::endl;
   return 0;
  }

  int menu() {
    banner();
    int choice;  

    std::cout << std::endl << "Pick one" << std::endl;
    std::cout << "[1] connect via SSH" << std::endl;
    std::cout << "[2] edit servers.yaml (config)" << std::endl;
    std::cout << "[0] exit" << std::endl;

    std::cin >> choice;
    std::cin.ignore(10000, '\n');

    if(choice == 1) {
      connect();

    }

    else if (choice == 2) {
     first_time();
    }

    else if (choice == 0) {
      std::cout << "exiting..." << std::endl;
      return 0;
    }
   return 0;
  }

  int main(int argc, char* argv[]) {
    if(argc < 2) {
      return menu();
    }

    std::string ar = argv[1];
    if (ar == "-h" || ar == "--help"){
        return help();
    } 
    if (ar == "-m" || ar == "--menu") {
        return menu();
    }
    if (ar == "-l" || ar == "--list") {
      return list_servers();
    }

return 0;

   
}
