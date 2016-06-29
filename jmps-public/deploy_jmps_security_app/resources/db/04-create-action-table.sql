CREATE TABLE IF NOT EXISTS `login`.`action`
(
  `action_id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'auto incrementing action_id of each action, unique index',
  
  `action_description` varchar(64) COLLATE utf8_unicode_ci NOT NULL COMMENT 'action description',
  
  `action_status` ENUM( 'abierto', 'encendido', 'cerrado', 'apagado', 'desconocido' ) NOT NULL DEFAULT 'desconocido',
   
  PRIMARY KEY (`action_id`),
  
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='action data';
