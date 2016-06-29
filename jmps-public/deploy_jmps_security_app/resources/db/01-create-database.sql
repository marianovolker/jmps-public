DROP USER 'jmps'@'localhost';
CREATE USER 'jmps'@'localhost' IDENTIFIED BY 'SolucionesSimples0192';
DROP DATABASE `login`;
CREATE DATABASE `login`;
GRANT ALL PRIVILEGES ON login.* TO jmps@localhost;
FLUSH PRIVILEGES;
