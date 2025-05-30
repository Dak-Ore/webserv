# 🌐 webserv

> Projet de l’école 42 : Implémentation d’un serveur HTTP en C++

## 📚 Description

Ce projet consiste à implémenter un serveur web compatible HTTP/1.1 en C++. Il doit être capable de gérer plusieurs connexions simultanées, de lire/parsing des requêtes HTTP, de générer des réponses appropriées, et de supporter des fonctionnalités comme le CGI, les erreurs personnalisées, ou encore les méthodes GET/POST/DELETE.

---

## ✅ Fonctions principales

- 🌐 Support de HTTP/1.1
- 🧠 Parsing complet des requêtes (Request line, headers, body)
- 📤 Génération de réponses HTTP dynamiques
- 🗂️ Gestion de fichiers statiques
- 🔀 Multiplexage avec `select()`
- 🧪 Gestion des méthodes : `GET`, `POST`, `DELETE`
- 🧩 Exécution de scripts CGI (`.py`, `.php`)
- 🛠️ Configuration par fichier `.conf` (multi-server & multi-location)
- 📄 Pages d’erreurs personnalisées
- 🧾 Autoindex
