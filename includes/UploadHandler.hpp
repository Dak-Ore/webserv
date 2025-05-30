#pragma once

class HttpRequest;

#include "Config.hpp"

class UploadHandler
{
public:
    UploadHandler(const HttpRequest& request, const Config* config);
    bool    hasMultipart();
    int     getError();
private:
    void parseMultipartBody(const std::string& boundary);
    void saveFile(const std::string& filename, const std::string& content);
    std::string sanitizeFilename(const std::string& filename);
    std::string handleUploadDir();
    const HttpRequest& _request;  // Référence à la requête HTTP
    const Config* _config;        // Configuration du serveur
    int _error;                   // Code d'erreur pour gérer les erreurs de téléchargement
};