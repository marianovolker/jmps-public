CREATE TABLE IF NOT EXISTS `login`.`profile`
(
  `profile_id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'auto incrementing profile_id of each profile, unique index',
  
  `profile_description` varchar(64) COLLATE utf8_unicode_ci NOT NULL COMMENT 'profile description',
  
  `time_to_expire_session_seconds` int(10) DEFAULT 60 COMMENT 'quantity of time that the session is valid for each profile',
  
  `profile_type` ENUM( 'administrador', 'perfil_tipo_1', 'perfil_tipo_2', 'perfil_tipo_3' ) NOT NULL DEFAULT 'perfil_tipo_1',
   
  PRIMARY KEY (`profile_id`),
  
  UNIQUE KEY `profile_type` (`profile_type`)
  
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='profile data';
