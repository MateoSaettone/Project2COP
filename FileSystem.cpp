#include "FileSystem.hpp"
#include <stdexcept>
#include <sstream>

FileSystemNode::FileSystemNode(std::string name, bool isDir) 
    : name(name), isDirectory(isDir), parent(nullptr) {}

FileSystemNode::~FileSystemNode() {
    for (auto child : children) {
        delete child;
    }
}

FileSystem::FileSystem() {
    root = new FileSystemNode("/", true);
    currentDirectory = root;
}

FileSystem::~FileSystem() {
    delete root;
}

void FileSystem::mkdir(const std::string& name) {
    // check if directory already exists
    bool exists = false;
    for (int i = 0; i < currentDirectory->children.size(); i++) {
        FileSystemNode *child = currentDirectory->children[i];
        if (child->name == name && child->isDirectory) {
            exists = true;
            break;
        }
    }
    if (exists) {
        throw std::runtime_error("File already exists");
    }
    // create new directory node
    FileSystemNode *newDir = new FileSystemNode(name, true);
    newDir->parent = currentDirectory; // set parent
    currentDirectory->children.push_back(newDir); // add to children
}

void FileSystem::touch(const std::string& name) {
    // check if file already exists
    bool exists = false;
    for (int i = 0; i < currentDirectory->children.size(); i++) {
        FileSystemNode *child = currentDirectory->children[i];
        if (child->name == name && !child->isDirectory) {
            exists = true;
            break;
        }
    }
    if (exists) {
        throw std::runtime_error("File already exists");
    }
    // create new file node
    FileSystemNode *newFile = new FileSystemNode(name, false);
    newFile->parent = currentDirectory; // set parent
    currentDirectory->children.push_back(newFile); // add to children
}

std::string FileSystem::ls() {
    // list the contents of the current directory
    std::stringstream ss;
    std::vector<std::string> entries;
    // collect entries
    for (int i = 0; i < currentDirectory->children.size(); i++) {
        FileSystemNode *child = currentDirectory->children[i];
        std::string entry = child->name;
        if (child->isDirectory) {
            entry += "/";
        }
        entries.push_back(entry);
    }
    // output entries
    for (int i = 0; i < entries.size(); i++) {
        ss << entries[i] << "\n";
    }
    return ss.str();
}

void FileSystem::cd(const std::string& path) {
    if (path == "/") {
        // go to root directory
        currentDirectory = root;
    } else if (path == "..") {
        // go to parent directory
        if (currentDirectory->parent != nullptr) {
            currentDirectory = currentDirectory->parent;
        }
    } else {
        // go to child directory
        bool found = false;
        for (int i = 0; i < currentDirectory->children.size(); i++) {
            FileSystemNode *child = currentDirectory->children[i];
            if (child->name == path && child->isDirectory) {
                currentDirectory = child;
                found = true;
                break;
            }
        }
        if (!found) {
            throw std::runtime_error("Directory not found");
        }
    }
}

void FileSystem::rm(const std::string& name) {
    // remove a file or directory
    bool found = false;
    for (int i = 0; i < currentDirectory->children.size(); i++) {
        FileSystemNode *child = currentDirectory->children[i];
        if (child->name == name) {
            // delete the node
            delete child;
            currentDirectory->children.erase(currentDirectory->children.begin() + i);
            found = true;
            break;
        }
    }
    if (!found) {
        throw std::runtime_error("File or directory not found");
    }
}

std::string FileSystem::pwd() {
    // get the current working directory
    if (currentDirectory == root) {
        return "/"; // root directory
    }
    std::string path = "";
    FileSystemNode *node = currentDirectory;
    while (node != root) {
        path = "/" + node->name + path; // prepend name
        node = node->parent; // move up
    }
    path += "/"; // add trailing slash
    return path;
}
FileSystemNode* FileSystem::findNode(FileSystemNode* startNode, const std::string& name) {
    if (startNode->name == name) {
        return startNode;
    }
    for (auto child : startNode->children) {
        FileSystemNode* found = findNode(child, name);
        if (found) {
            return found;
        }
    }
    return nullptr;
}

FileSystemNode* FileSystem::find(const std::string& name) {
    return findNode(root, name);
}
std::string FileSystem::displayTree(FileSystemNode* node, std::string indent) {
    std::stringstream ss;
    ss << indent << node->name << (node->isDirectory ? "/" : "") << "\n";
    if (node->isDirectory) {
        for (auto child : node->children) {
            ss << displayTree(child, indent + "  ");
        }
    }
    return ss.str();
}

std::string FileSystem::tree() {
    return displayTree(root, "");
}