#include "UploadHandler.hpp"
#include "HttpRequest.hpp"
#include "Config.hpp"
#include "utils.hpp"
#include <unistd.h>

UploadHandler::UploadHandler(const HttpRequest& request, const Config* config)
    : _request(request), _config(config), _error(0)
{}

bool UploadHandler::hasMultipart()
{
    // Check if Content-Type exist
    std::map<std::string, std::string>::const_iterator it = _request.getHeaders().find("Content-Type");
    if (it == _request.getHeaders().end())
        return (false);
    
    // Check if Content-Type == multipart/form-data
    const std::string& contentType = it->second;
    if (contentType.find("multipart/form-data") != 0)
        return (false);

    // Check if upload is enabled
    if (!_config->getUploadEnabled())
    {
        this->_error = 403;
        return (false);
    }

    // Extract Boundary value
    std::string boundaryKey = "boundary=";
    size_t boundaryPos = contentType.find(boundaryKey);
    if (boundaryPos == std::string::npos)
    {
        this->_error = 402;
        return (false);
    }

    std::string boundary = "--" + contentType.substr(boundaryPos + boundaryKey.length());
    parseMultipartBody(boundary);
    return (true);
}

void UploadHandler::parseMultipartBody(const std::string& boundary)
{
    size_t pos = 0;
    // Start of file
    size_t start = _request.getBody().find("Content-Disposition:");
    if (start == std::string::npos)
        return;

    // End of file
    size_t end = _request.getBody().find("\r\n", pos);
    if (end == std::string::npos)
        end = _request.getBody().length();
    
    std::string contentDispositionLine = _request.getBody().substr(start, end - start);
    std::string filename = utils::getHeaderParam(contentDispositionLine, "filename");
    while ((pos = _request.getBody().find(boundary, pos)) != std::string::npos)
    {
        pos += boundary.length();
        if (_request.getBody().compare(pos, 2, "\r\n") == 0)
            pos += 2;

        size_t nextPart = _request.getBody().find(boundary, pos);
        if (nextPart == std::string::npos)
            break;

        std::string part = _request.getBody().substr(pos, nextPart - pos);
        size_t headerEnd = part.find("\r\n\r\n");
        if (headerEnd == std::string::npos)
            continue;

        std::string headerSection = part.substr(0, headerEnd);
        std::string content = part.substr(headerEnd + 4);
        if (content.size() >= 2 && content.substr(content.size() - 2) == "\r\n")
            content = content.substr(0, content.size() - 2);

        std::istringstream headerStream(headerSection);
        std::string line;
        std::string filename;
        while (std::getline(headerStream, line))
        {
            int eof = line.size() - 1;
            if (line[eof] == '\r') line.erase(eof);
            if (line.find("Content-Disposition:") == 0)
                filename = utils::getHeaderParam(line, "filename");
        }

        if (!filename.empty())
            saveFile(filename, content);
    }
}

void UploadHandler::saveFile(const std::string& filename, const std::string& content)
{
	std::string path = handleUploadDir();
	std::string	upload = utils::joinPath(path, filename);
	std::ofstream fichier;
	fichier.open(upload.c_str(), std::ofstream::out);
	if (!fichier.is_open())
		throw std::runtime_error("Failed to create file: " + filename);
	fichier << content;
	fichier.close();
    this->_error = 200;
}

std::string UploadHandler::handleUploadDir()
{
    std::string path;
    if (!_config->getUploadPath().empty())
    {
        std::vector<std::string>::const_iterator it = _config->getUploadPath().begin();
        while (it != _config->getUploadPath().end() && !utils::isDirectory(*it))
            it++;
        // Check if we find a PATH and we can write in it
        if (it != _config->getUploadPath().end())
        {
            path = *it;
            if (access(path.c_str(), W_OK) != 0)
                return path;
        }
        std::cout << "Warning no valid upload path found using default instead" << std::endl;
    }

    path = "uploads";
    if (!utils::isDirectory(path))
    {
        if (mkdir(path.c_str(), 0755) != 0)
            throw std::runtime_error("Failed to create uploads directory");
    }

    // Check we have write 
    if (access(path.c_str(), W_OK) != 0)
        throw std::runtime_error("Upload directory is not writable");

    return (path);
}

int     UploadHandler::getError(){return this->_error;}
