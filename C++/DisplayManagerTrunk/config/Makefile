################################################################################################
## ENVIRONMENT VARIABLES CONTROLLING THE BUILD
##
## DM_DIR_BIN = /opt/displaymanager/bin
## DM_DIR_CFG = /etc/opt/displaymanager
## DM_DIR_PER = /opt/displaymanager/personality
## DM_DIR_VID = /opt/displaymanager/video
## DM_DIR_LOG = /var/opt/displaymanager/log
## DM_DIR_XML = /var/opt/displaymanager/xml
## DM_DIR_IMG = /var/opt/displaymanager/images
## DM_DIR_RUN = /var/opt/displaymanager/run
## DM_DIR_IND = /etc/init.d/
## DM_LOG_ROT = /etc/logrotate.d/
## NGP_DIR_CFG= /etc/config/
##
################################################################################################
## USEFUL TARGETS SUPPORTED BY THIS MAKEFILE
##
##
## install_config	Copy the configuration files into correct destination directories and then 
##					replace install dir located in displaymanager.properties file by the correct one 
##					marked as XXXREPLACE_... label.
##
##
################################################################################################

.PHONY: all
all:
	@echo "Doing nothing for target 'all' or default."

.PHONY: install_config
install_config:
	@echo "+++++++++++++++++++++++++++++++"
	@echo "TARGET====>install_config<====(${DM_BUILD_TYPE})"
	@echo "DM_BUILD_TYPE=${DM_BUILD_TYPE};DM_DIR_BIN=${DM_DIR_BIN};DM_DIR_CFG=${DM_DIR_CFG};DM_DIR_PER=${DM_DIR_PER};DM_DIR_LOG=${DM_DIR_LOG};DM_DIR_XML=${DM_DIR_XML};DM_DIR_IMG=${DM_DIR_IMG};DM_DIR_RUN=${DM_DIR_RUN};DM_DIR_IND=${DM_DIR_IND};DM_DIR_VID=${DM_DIR_VID}"
	@install -m 644 -p displaymanager.properties ${DM_DIR_PKG}/${DM_DIR_CFG}/
	@install -m 644 -p jquery-1.4.2.min.js ${DM_DIR_PKG}/${DM_DIR_CFG}/
	@javascriptminifier/jsmin < glibrary.js > glibrary.min.js
	@install -m 644 -p glibrary.min.js ${DM_DIR_PKG}/${DM_DIR_CFG}/
	@install -m 644 -p displaymanager.ini ${DM_DIR_PKG}/${NGP_DIR_CFG}/
	@install -m 644 -p dm_stdout ${DM_DIR_PKG}/${DM_LOG_ROT}/
	@install -m 644 -p default.html ${DM_DIR_PKG}/${DM_DIR_VID}/
	@mkdir -p ${DM_DIR_PKG}/${DM_DIR_PER}/
	@chmod 755 ${DM_DIR_PKG}/${DM_DIR_PER}/
	@find ${DM_DIR_PKG}/${DM_DIR_PER}/ -name ".svn" -exec rm -rf '{}' \; 2>/dev/null;echo
	@sed -i "s|XXXREPLACE_DM_DIR_BIN|$(DM_DIR_BIN)|g" ${DM_DIR_PKG}/${DM_DIR_IND}/displaymanager
	@sed -i "s|XXXREPLACE_DM_DIR_RUN|$(DM_DIR_RUN)|g" ${DM_DIR_PKG}/${DM_DIR_IND}/displaymanager
	@sed -i "s|XXXREPLACE_DM_DIR_CFG|$(DM_DIR_CFG)|g" ${DM_DIR_PKG}/${DM_DIR_IND}/displaymanager
	@sed -i "s|XXXREPLACE_DM_DIR_LOG|$(DM_DIR_LOG)|g" ${DM_DIR_PKG}/${DM_DIR_CFG}/displaymanager.properties	
	@sed -i "s|XXXREPLACE_DM_DIR_XML|$(DM_DIR_XML)|g" ${DM_DIR_PKG}/${DM_DIR_CFG}/displaymanager.properties
	@sed -i "s|XXXREPLACE_DM_DIR_CFG|$(DM_DIR_CFG)|g" ${DM_DIR_PKG}/${DM_DIR_CFG}/displaymanager.properties
	@sed -i "s|XXXREPLACE_DM_DIR_PER|$(DM_DIR_PER)|g" ${DM_DIR_PKG}/${DM_DIR_CFG}/displaymanager.properties
	@sed -i "s|XXXREPLACE_DM_DIR_IMG|$(DM_DIR_IMG)|g" ${DM_DIR_PKG}/${DM_DIR_CFG}/displaymanager.properties
	@sed -i "s|XXXREPLACE_DM_DIR_LOG|$(DM_DIR_LOG)|g" ${DM_DIR_PKG}/${DM_LOG_ROT}/displaymanager
	@sed -i "s|XXXREPLACE_DM_DIR_LOG|$(DM_DIR_LOG)|g" ${DM_DIR_PKG}/${DM_LOG_ROT}/dm_stdout
	@echo "+++++++++++++++++++++++++++++++"
